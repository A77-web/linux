#include <iostream>
#include <cstdio> //== #inculde <stdio.h>
#include <cstdlib> //==#include <stdlib.h>
#include <queue>
#include <mutex>
#include <pthread.h>

#define MAXQ 10
class BlockQueue
{
private:
    std::queue<int> _queue;
    int _capacity;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond_pro;
    pthread_cond_t _cond_con;
public:
    BlockQueue(int maxq = MAXQ) :_capacity(maxq) {
        pthread_mutex_init(&_mutex, NULL);
        pthread_cond_init(&_cond_pro, NULL);
        pthread_cond_init(&_cond_con, NULL);
    }
    ~BlockQueue() {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond_pro);
        pthread_cond_destroy(&_cond_con);
    }
    bool Push(int data) {
        pthread_mutex_lock(&_mutex);
        while (_queue.size() == _capacity) {
            pthread_cond_wait(&_cond_pro, &_mutex);
        }
        _queue.push(data);
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&_cond_con);
        return true;
    }
    bool Pop(int* data) {
        pthread_mutex_lock(&_mutex);
        while (_queue.empty() == true) {
            pthread_cond_wait(&_cond_con, &_mutex);
        }
        *data = _queue.front();
        _queue.pop();
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&_cond_pro);
        return true;
    }
};

std::mutex g_mutex;
void* consumer(void* arg)
{
    BlockQueue* q = (BlockQueue*)arg;
    int data;
    while (1) {
        q->Pop(&data);
        printf("%p- pop data:%d\n", pthread_self(), data);
    }
    return NULL;
}
void* productor(void* arg)
{
    BlockQueue* q = (BlockQueue*)arg;
    int i = 0;
    while (1) {
        q->Push(i);
        printf("%p push data:%d\n", pthread_self(), i++);
    }
    return NULL;
}
int main()
{
    pthread_t ctid[4], ptid[14];
    int ret;
    BlockQueue q;

    for (int i = 0; i < 4; i++) {
        ret = pthread_create(&ctid[i], NULL, consumer, (void*)&q);
        if (ret != 0) {
            printf("thread create error\n");
            return -1;
        }
    }
    for (int i = 0; i < 14; i++) {
        ret = pthread_create(&ptid[i], NULL, productor, (void*)&q);
        if (ret != 0) {
            printf("thread create error\n");
            return -1;
        }
    }
    for (int i = 0; i < 4; i++) {
        pthread_join(ctid[i], NULL);
        pthread_join(ptid[i], NULL);
    }
    return 0;
}
