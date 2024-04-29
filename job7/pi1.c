/*
莱布尼兹级数公式: 1 - 1/3 + 1/5 - 1/7 + 1/9 - ... = PI/4
主线程创建 1 个辅助线程
主线程计算级数的前半部分
辅助线程计算级数的后半部分
主线程等待辅助线程运行結束后,将前半部分和后半部分相加
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

//定义存放函数返回值的结构体
struct result{
    double sum;
};
//定义存放函数参数的结构体
struct param{
    double start;
    double end;
};

void *compute(void*arg){
    //参数为长度为2的int数组，表示计算的是start到end项的级数
    struct result*re;
    struct param *pa;
    double sum=0;
    pa=(struct param*)arg;
    for(double i=pa->start;i<pa->end;i++){
        sum+=1/(2*i-1);
        i++;
        sum-=1/(2*i-1);
    }
    re=malloc(sizeof(struct result));
    re->sum=sum;
    return re;
}

int main(){
    double sum=0;
    //新线程计算51～100
    pthread_t worker_tid;
    struct param *pa;
    pa=malloc(sizeof(struct result));
    pa->start=10000001,pa->end=20000000;
    pthread_create(&worker_tid, NULL, &compute, pa); 
    struct result*re;
    pthread_join(worker_tid, (void **)&re);
    sum+=re->sum;


    //主程序计算1～50
    struct param *pa1;
    pa1=malloc(sizeof(struct result));
    pa1->start=1,pa1->end=10000000;
    struct result*re1;
    re1=(struct result*)compute(pa1);
    sum+=re1->sum;
    printf("%.7lf\n",sum*4.0);
    return 0;
}
