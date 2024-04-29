#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
//定义数组的结构体
struct array{
    int*array;
    int start;
    int end;
};


void *select_sort(void*arg){
    struct array*arr=(struct array*)arg;
    //进行选择排序
    for(int i=arr->start;i<arr->end-1;i++){
        //选择最小的
        int min_id=i;
        for(int j=i+1;j<arr->end;j++){
            if(arr->array[min_id]>arr->array[j]){
                min_id=j;
            }
        }
        //交换
        int temp=arr->array[min_id];
        arr->array[min_id]=arr->array[i];
        arr->array[i]=temp;
    }


}

void merge(int*array,int l,int mid,int r){

    int al=l,ar=mid,bl=mid,br=r;
    int *temp_array=malloc(sizeof(int)*(r-l));
    int tid=l;
    while(al<ar&&bl<br){
        if(array[al]<array[bl]){
            temp_array[tid++]=array[al++];
        }else{
            temp_array[tid++]=array[bl++];
        }
    }
    while(al<ar){
        temp_array[tid++]=array[al++];
    }
    while(bl<br){
        temp_array[tid++]=array[bl++];
    }
    //交换
    for(int i=l;i<r;i++){
        array[i]=temp_array[i];
    }
}

//综合排序
void parallel_sort(int *array, int size)
{
    //开辟两个线程，分别进入选择排序
    int pth_num=2;
    int area=size/pth_num;
    struct array arrs[pth_num];
    pthread_t workers[pth_num];

    //使用进程
    for(int i=0;i<pth_num;i++){
        struct array *arr;
        arr = &arrs[i];
        arr->array=array;
        arr->start = i * area; 
        arr->end = (size<(i+1)*area)?size:(i+1)*area;   
        pthread_create(&workers[i], NULL, select_sort, arr);
    }

    for (int i = 0; i < pth_num; i++) {
        pthread_join(workers[i], NULL);
    }
    
    //使用merge(本题使用两个进程，所以只需要merge一次)
    merge(array,0,area,size);
}

int main()
{
    int array[8] = {3, 1, 2, 4, 5, 6, 7, 0};
    parallel_sort(array, 8);
    int i;
    for (i = 0; i < 8; i++)
        printf("%d\n", array[i]);
    return 0;
}