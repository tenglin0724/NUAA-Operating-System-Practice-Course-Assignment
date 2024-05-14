#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define CAPACITY 4
int buffer1[CAPACITY],buffer2[CAPACITY];
int in;
int cam;
int out;

int buffer_1_is_empty()
{
    return in == cam;
}

int buffer_1_is_full()
{
    return (in + 1) % CAPACITY == cam;
}

int buffer_2_is_empty()
{
    return cam == out;
}

int buffer_2_is_full()
{
    return (cam + 1) % CAPACITY == out;
}

void get_and_put()
{
    int item;

    item = buffer1[cam];
    buffer2[cam] = item+'A'-'a';
    cam = (cam + 1) % CAPACITY;
}

void put_item_in_1(int item)
{
    buffer1[in] = item;
    in = (in + 1) % CAPACITY;
}

int get_item_from_2()
{
    int item;

    item = buffer2[out];
    out = (out + 1) % CAPACITY;
    return item;
}

pthread_mutex_t mutex1,mutex2;
pthread_cond_t wait_empty_1_buffer;
pthread_cond_t wait_full_1_buffer;
pthread_cond_t wait_empty_2_buffer;
pthread_cond_t wait_full_2_buffer;

#define ITEM_COUNT (CAPACITY * 2)

void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        pthread_mutex_lock(&mutex2);
        while (buffer_2_is_empty())
            pthread_cond_wait(&wait_full_2_buffer, &mutex2);

        item = get_item_from_2(); 
        printf("%c\n", item); 

        pthread_cond_signal(&wait_empty_2_buffer);
        pthread_mutex_unlock(&mutex2);
    }
    return NULL;
}
void *calculate(void *arg){
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        pthread_mutex_lock(&mutex1);
        pthread_mutex_lock(&mutex2);
        while (buffer_1_is_empty())
            pthread_cond_wait(&wait_full_1_buffer, &mutex1);
        while(buffer_2_is_full())
            pthread_cond_wait(&wait_empty_2_buffer,&mutex2);
        get_and_put(); 
        pthread_cond_signal(&wait_empty_1_buffer);
        pthread_mutex_unlock(&mutex1);
        pthread_cond_signal(&wait_full_2_buffer);
        pthread_mutex_unlock(&mutex2);       
    }
    return NULL;
}

void *produce(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        pthread_mutex_lock(&mutex1);
        while (buffer_1_is_full()) 
            pthread_cond_wait(&wait_empty_1_buffer, &mutex1);

        item = 'a' + i;
        put_item_in_1(item);

        pthread_cond_signal(&wait_full_1_buffer);
        pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}

int main()
{ 
    pthread_t consumer_tid;
    pthread_t calculate_tid;

    pthread_mutex_init(&mutex1, NULL);
    pthread_mutex_init(&mutex2, NULL);
    pthread_cond_init(&wait_empty_1_buffer, NULL);
    pthread_cond_init(&wait_full_1_buffer, NULL);
    pthread_cond_init(&wait_empty_2_buffer, NULL);
    pthread_cond_init(&wait_full_2_buffer, NULL);

    pthread_create(&consumer_tid, NULL, consume, NULL);
    pthread_create(&calculate_tid, NULL, calculate, NULL);
    produce(NULL); 
    pthread_join(calculate_tid,NULL);
    pthread_join(consumer_tid,NULL);
    return 0;
}
