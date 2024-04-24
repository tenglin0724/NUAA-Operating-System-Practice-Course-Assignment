#include "std.h"
#include "file.h"

//读出一行数据，并将最后的换行符除去，读取成功返回1
int read_line(FILE *f, char *line)
{
    if (fgets(line, LINE_SIZE, f) == NULL)
        return 0;

    char *nl = strchr(line, '\n');
    assert(nl != NULL);
    *nl = 0;
    return 1;
}

//写一行并写入换行符
void write_line(FILE *f, char *line)
{
    fputs(line, f);
    fputc('\n', f);
}

//加载一行数据
int load_line(char *path, char *line)
{
    FILE *f = fopen(path, "r");
    if (f == NULL)
        return 0;
    
    int count = read_line(f, line);
    fclose(f);
    return count;
}

//存储一行数据
void store_line(char *path, char *line)
{
    FILE *f = fopen(path, "w");
    write_line(f, line);
    fclose(f);
}
//.dg/HEAD
//判断给定文件是否是文件夹
bool file_is_dir(char *path)
{
    struct stat info;
    stat(path, &info);

    return S_ISDIR(info.st_mode);
}

//判断给定文件是否存在
bool file_exists(char *path)
{
    return access(path, F_OK) == 0;
}
//获取文件大小
int get_file_size(char *path)
{
    struct stat info;
    stat(path, &info);
    return info.st_size;
}

//将内容写入文件
void make_file(char *path, char *content)
{
    FILE *f = fopen(path, "w");
    fputs(content, f); 
    // printf("%s\n",content);
    fclose(f);
}

//创建文件
void create_file(char*path){
    FILE *f = fopen(path, "w");
    if(f==NULL){
        perror("file create fail!");
    }
    fclose(f);
}

//复制文件内容
void copy_file(char *source_path, char *target_path)
{
    FILE *source_f = fopen(source_path, "r");
    FILE *target_f = fopen(target_path, "w");
    char buff[1024];
    int count;
    while (count = fread(buff, 1, sizeof(buff), source_f))
        fwrite(buff, 1, count, target_f);
    fclose(source_f);
    fclose(target_f);
}
