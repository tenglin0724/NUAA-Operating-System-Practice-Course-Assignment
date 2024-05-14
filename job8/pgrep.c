#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
//定义数据结构，用于存储任务队列
typedef struct {
    char path[256];
    char target[256];
}job_node;
/*定义队列*/
typedef struct {
    job_node *array;
    int head;
    int tail;
    int capacity;
} Queue;
// 初始化队列
void init(Queue*q,int queue_len) {
    q->array=(job_node*)malloc(sizeof(job_node)*queue_len);
    q->capacity = queue_len;
    q->head = q->tail = 0;
}
// 入队
void enqueue(Queue*q, char* path,char*target) {
    if ((q->tail + 1) % q->capacity == q->head) {
        return;
    }
    if(path==NULL&&target==NULL){
        q->array[q->tail].path[0]='\0';
        q->array[q->tail].target[0]='\0';
    }else{
        strcpy(q->array[q->tail].path,path);
        strcpy(q->array[q->tail].target,target);
    }
    //printf("主程序存入数据：%s   %s,此时队列中数据情况：%d       %d\n",path,target,q->head,q->tail);
    q->tail = (q->tail + 1) % q->capacity;
}
// 出队
job_node*dequeue(Queue *q) {
    if (q->head >= q->tail) {
        return NULL;
    }
    job_node*result=(job_node*)malloc(sizeof(job_node));
    strcpy(result->path, q->array[q->head].path);
    strcpy(result->target,q->array[q->head].target);
    //printf("dequeue:%s  %s\n",result->path,result->target);
    q->head = (q->head + 1) % q->capacity;
    return result;
}
// 获取队列大小
int size(Queue *q) {
    return (q->tail - q->head + q->capacity) % q->capacity;
}

//销毁
void destroy(Queue*q){
    free(q->array);
}


typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

//定义信号量操作
void sema_init(sema_t *sema, int value)
{
    sema->value = value;
    pthread_mutex_init(&sema->mutex, NULL);
    pthread_cond_init(&sema->cond, NULL);
}

void sema_wait(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    while (sema->value <= 0)
        pthread_cond_wait(&sema->cond, &sema->mutex);
    sema->value--;
    pthread_mutex_unlock(&sema->mutex);
}

void sema_signal(sema_t *sema)
{
    pthread_mutex_lock(&sema->mutex);
    ++sema->value;
    pthread_cond_signal(&sema->cond);
    pthread_mutex_unlock(&sema->mutex);
}

//定义全局变量
Queue ge;
sema_t wmutex;
//sema_t rmutex;
//int readcount=0;

//开辟的两个线程需要调用的函数
void pgrep_file(char *path, char *target)
{
    FILE *file = fopen(path, "r");

    char line[256];
    while (fgets(line, sizeof(line), file)) { 
        if (strstr(line, target))
            printf("%s:%s", path, line);
    }

    fclose(file);
}
//相当于写者
void write_job(char *path,char*target){
    sema_wait(&wmutex);
    enqueue(&ge,path,target);
    //printf("写入文件：%s     %s\n",path,target);
    sema_signal(&wmutex);
}
//相当于读者
void *read_job(void *arg){
    char *string = (char *)arg;
    while(1){
        //sema_wait(&rmutex);
        //if(readcount==0) 
        sema_wait(&wmutex);
        //readcount++;
        //sema_signal(&rmutex);
        job_node*temp;
        //进行任务操作
        temp=dequeue(&ge);
        //sema_wait(&rmutex);
        //readcount--;
        //if(readcount==0) 
        sema_signal(&wmutex);
        //sema_signal(&rmutex);

        if(temp!=NULL){
            //printf("%s:取走数据%s   %s,此时队列中数据情况：%d     %d\n",string,temp->path,temp->target,ge.head,ge.tail);
            if(temp->path[0]=='\0'&&temp->target[0]=='\0'){
                pthread_exit(NULL);
            }else{
                pgrep_file(temp->path,temp->target);
            }
        }
        free(temp);
    }
}

void grep_dir(char *path, char *target)
{

    DIR *dir = opendir(path);
    struct dirent *entry;
    while (entry = readdir(dir)) {
        if (strcmp(entry->d_name, ".") == 0) 
            continue;

        if (strcmp(entry->d_name, "..") == 0) 
            continue;

        char sub_path[300];
        sprintf(sub_path, "%s/%s", path, entry->d_name);

        if (entry->d_type == DT_DIR) 
            grep_dir(sub_path, target);

        if (entry->d_type == DT_REG) 
            write_job(sub_path, target);
    }
    closedir(dir);
}



void usage()
{
    puts("Usage:");
    puts(" pgrep string path");
    puts(" pgrep -r string path");
}
int main(int argc, char *argv[])
{
    if (argc < 3){
        usage();
        return 0;
    }

    char *string = argv[argc - 2];
    char *path = argv[argc - 1];
    if (strcmp(argv[1], "-r") == 0){
        init(&ge,200);
        pthread_t twork[2];
        sema_init(&wmutex,1);
        //sema_init(&rmutex,1);
        //void *arg[2] = {"h1","h2"};
        for(int i=0;i<2;i++){
            pthread_create(&twork[i], NULL, read_job, NULL);
        }
        grep_dir(path, string);
        //printf("主进程已结束！此时状态为：%d      %d\n",ge.head,ge.tail);
        for(int i=0;i<2;i++){
            enqueue(&ge,NULL,NULL);
        }
        for(int i=0;i<2;i++){
            pthread_join(twork[i], NULL);
        }
    }
    else{
        pgrep_file(path, string);
    }
    return 0;
}