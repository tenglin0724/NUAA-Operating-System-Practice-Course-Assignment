#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define CAPACITY 4
int buffer1[CAPACITY],buffer2[CAPACITY];
int in;
int cap;
int out;

int buffer_1_is_empty()
{
    return in == cap;
}

int buffer_1_is_full()
{
    return (in + 1) % CAPACITY == cap;
}
int buffer_2_is_empty()
{
    return cap == out;
}

int buffer_2_is_full()
{
    return (cap + 1) % CAPACITY == out;
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

typedef struct {
    int value;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
} sema_t;

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

sema_t mutex_sema_1;
sema_t empty_buffer_sema_1;
sema_t full_buffer_sema_1;
sema_t mutex_sema_2;
sema_t empty_buffer_sema_2;
sema_t full_buffer_sema_2;

#define ITEM_COUNT (CAPACITY * 2)
void *consume(void *arg)
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        sema_wait(&full_buffer_sema_2);
        sema_wait(&mutex_sema_2);

        item = get_item_from_2();
        printf("%c\n", item); 

        sema_signal(&mutex_sema_2);
        sema_signal(&empty_buffer_sema_2);
    }

    return NULL;
}

void *calculate(void *arg){
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 

        sema_wait(&full_buffer_sema_1);
        sema_wait(&mutex_sema_1);
        int item;
        item = buffer1[cap];
        sema_signal(&mutex_sema_1);
        sema_signal(&empty_buffer_sema_1);



        sema_wait(&empty_buffer_sema_2);
        sema_wait(&mutex_sema_2);
        buffer2[cap] = item+'A'-'a';
        cap = (cap + 1) % CAPACITY;
        sema_signal(&mutex_sema_2);
        sema_signal(&full_buffer_sema_2);
    }

    return NULL;
}



void *produce()
{
    int i;
    int item;

    for (i = 0; i < ITEM_COUNT; i++) { 
        sema_wait(&empty_buffer_sema_1);
        sema_wait(&mutex_sema_1);

        item = i + 'a';
        put_item_in_1(item);

        sema_signal(&mutex_sema_1);
        sema_signal(&full_buffer_sema_1);
    }

    return NULL;
}

int main()
{ 
    pthread_t consumer_tid;
    pthread_t calculate_tid;


    sema_init(&empty_buffer_sema_1, CAPACITY - 1);
    sema_init(&full_buffer_sema_1, 0);
    sema_init(&mutex_sema_1, 1);

    sema_init(&empty_buffer_sema_2, CAPACITY - 1);
    sema_init(&full_buffer_sema_2, 0);
    sema_init(&mutex_sema_2, 1);

    pthread_create(&consumer_tid, NULL, consume, NULL);
    pthread_create(&calculate_tid, NULL, calculate, NULL);
    produce();
    pthread_join(consumer_tid, NULL);
    pthread_join(calculate_tid, NULL);
    return 0;
}