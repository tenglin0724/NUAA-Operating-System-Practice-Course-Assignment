#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define NR_TOTAL 10000000
#define NR_CPU 10
#define NR_CHILD (NR_TOTAL/NR_CPU)

struct param {
    double start;
    double end;
};

struct result {
    double sum;
};

void *compute(void*arg){
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

int main()
{ 
    pthread_t workers[NR_CPU];
    struct param params[NR_CPU]; 
    int i;

    for (i = 0; i < NR_CPU; i++) {
        struct param *param;
        param = &params[i];
        param->start = i * NR_CHILD+1; 
        param->end = (i + 1) * NR_CHILD;
        pthread_create(&workers[i], NULL, compute, param);
    }

    double sum = 0;
    for (i = 0; i < NR_CPU; i++) {
        struct result *result;
        pthread_join(workers[i], (void **)&result);
        sum += result->sum;
        free(result);
    }
    printf("%.7lf\n", sum*4);
    return 0;
}