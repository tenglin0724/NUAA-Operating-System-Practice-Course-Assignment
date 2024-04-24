#include "utils/std.h"
#include "utils/file.h"
#include "utils/dir.h"
#include "config.h"
#include "cmd.h"
#include "head.h"
#include "index.h"
#include "blob.h"
#include "commit.h"
#include "utils/list.h"

//新建一个提交
commit_t *commit_new(void)
{
    commit_t *this = malloc(sizeof(commit_t));
    return this;
}
//释放提交结构
void commit_delete(commit_t *this)
{
    free(this);
}
//打印提交的信息
void commit_dump(commit_t *this)
{
    printf("commit %s\n", this->oid.data);
    printf("time   %s\n", this->time);
    printf("msg    %s\n", this->msg);
    printf("\n");
}

//加载commit
commit_t *commit_load(oid_t *oid)
{
    commit_t *this = malloc(sizeof(commit_t));
    this->oid = *oid;

    FILE *f = fopen_oid(oid, "r");
    char line[LINE_SIZE];

    // 1. commit
    read_line(f, line);

    // 2. size
    read_line(f, line);

    // 3. parent_oid
    read_line(f, line);
    oid_set(&this->parent_oid, line);

    // 4. tree_oid
    read_line(f, line);
    oid_set(&this->tree_oid, line);

    // 5. time
    read_line(f, line);
    strcpy(this->time, line);

    // 6. message
    read_line(f, line);
    strcpy(this->msg, line);

    fclose(f);
    return this;
}

//将commit转化成文本
void commit_make_text(commit_t *this, char *text)
{
    char *p = text;

    p += sprintf(p, "commit\n");
    p += sprintf(p, "0\n");
    p += sprintf(p, "%s\n", this->parent_oid.data);
    p += sprintf(p, "%s\n", this->tree_oid.data);
    p += sprintf(p, "%s\n", this->time);
    p += sprintf(p, "%s\n", this->msg);
}

//计算提交时间
void commit_make_time(commit_t *this)
{
    time_t time_long = time(NULL);
    char *time_text = ctime(&time_long);
    int time_last = strlen(time_text) - 1;
    time_text[time_last] = 0;
    strcpy(this->time, time_text);
}

void commit_store(commit_t *this, oid_t *oid)
{
    char text[4096];    
    commit_make_text(this, text);

    get_string_oid(text, oid);
    this->oid = *oid;
    printf("commit %s %s\n", oid->data, this->msg);

    FILE *f = fopen_oid(oid, "w");
    fputs(text, f);
    fclose(f);
}

void git_commit(int argc, char *argv[])
{
    //加载HEAD
    oid_t *pre_commit_oid=malloc(sizeof(oid_t));
    head_load_oid(pre_commit_oid);

    //将index文件保存到objects目录下
    oid_t *index_oid=malloc(sizeof(oid_t));
    index_dup(index_oid);


    //创建新的提交
    commit_t*commit=commit_new();
    //得到提交时间
    commit_make_time(commit);
    //parent_oid
    commit->parent_oid=*pre_commit_oid;
    free(pre_commit_oid);
    //tree(index)_oid
    commit->tree_oid=*index_oid;
    free(index_oid);
    //msg
    strcpy(commit->msg,argv[2]);
    //oid
    oid_t*new_oid=malloc(sizeof(oid_t));
    commit_store(commit,new_oid);

    //清空commit
    commit_delete(commit);
    //将得到的oid写入到HEAD中
    head_store_oid(new_oid);

}

//打印日志信息
void git_log(int argc, char *argv[])
{
    //得到当前HEAD中保存的oid
    oid_t*cru_oid=malloc(sizeof(oid_t));
    head_load_oid(cru_oid);
    //判断当前是否有提交记录
    if(oid_is_zero(cru_oid)){
        perror("您当前尚无任何提交！");
        return;
    }
    while(!oid_is_zero(cru_oid)){
        //有记录,读取当前记录文件
        commit_t*commit=commit_load(cru_oid);
        //打印commit
        commit_dump(commit);
        //释放空间
        commit_delete(commit);
        //根据parent指向上一个记录
        oid_set(cru_oid,commit->parent_oid.data);
    }
    free(cru_oid);
}

//等待完善，删除文件前判断是否该文件被add且commit过
void empty_dir(char *path)
{
    dir_t *dir = dir_open(path);
    entry_t entry;

    while (dir_read(dir, &entry)) {
        if (strcmp(entry.name, GIT_DIR) == 0)
            continue;

        char *sub_path = entry.path;
        printf("unlnk %s\n", sub_path);

        if (entry_is_dir(&entry)) {
            empty_dir(sub_path);
            rmdir(sub_path);
        } else {
            unlink(sub_path);
        }
    }

    dir_close(dir);
}

void git_checkout(int argc, char *argv[])
{
    if (argc != 2)
        usage();

    //清空当下文件夹所有文件
    empty_dir(".");

    char *abbr = argv[1];
    oid_t oid;
    //根据abbr找到commit的oid
    make_oid(abbr, &oid);

    //根据HEAD找到当前commit
    oid_t*head_oid=malloc(sizeof(oid_t));
    head_load_oid(head_oid);

    //根据head_oid找到cru_commit
    commit_t*cru_commit=commit_load(head_oid);
    free(head_oid);

    //根据cru_commit打印第一条提示信息
    char show[5];
    strncpy(show,cru_commit->oid.data,4);
    show[4]='\0';
    printf("之前的 HEAD 位置是 %s %s\n",show,cru_commit->msg);
    commit_delete(cru_commit);

    //根据commit的oid找到提交的内容
    commit_t*change_commit=commit_load(&oid);

    //根据commit中的treeid,找到原来的tree
    tree_t*tree=tree_load(&(change_commit->tree_oid));

    //遍历树
    node_t *node;
    leaf_t *leaf;
    list_each(&tree->leaf_list, node, leaf){
        if(oid_is_zero(&(leaf->oid))){
            //目录，创建
            if (mkdir(leaf->path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
                perror("检出失败！");
            }
        }else{
            //文件，从objects中复制过来
            copy_blob_content(&(leaf->oid),leaf->path);
        }
    }


    //打印相关信息
    printf("HEAD 目前位于 %s %s\n",abbr,change_commit->msg);

    //更新HEAD
    head_store_oid(&(change_commit->oid));

    commit_delete(change_commit);
    tree_delete(tree);
}
