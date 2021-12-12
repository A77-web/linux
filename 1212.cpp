#include <iostream>
#include <cstdlib>
#include <vector>
#include <pthread.h>
#include <semaphore.h>

#define MAXQ 5

class RingQueue{
    private:
        std::vector<int> _arry;
        int _capacity;
        int _write_step;
        int _read_step;
        sem_t _sem_idle;//
        sem_t _sem_data;
        sem_t _sem_lock;
    public:
        RingQueue(int maxq = MAXQ):_arry(maxq),
        _capacity(maxq), _write_step(0), _read_step(0){
            //sem_init(信号量，1进程/0线程，初值)
            sem_init(&_sem_idle, 0, maxq);
            sem_init(&_sem_data, 0, 0);//初始没有数据
            sem_init(&_sem_lock, 0, 1);
        }
        ~RingQueue(){
            sem_destroy(&_sem_idle);
            sem_destroy(&_sem_data);
            sem_destroy(&_sem_lock);
        }
        bool Push(const int data){
            sem_wait(&_sem_idle);//空闲空间-1
            sem_wait(&_sem_lock);//加锁
            _arry[_write_step] = data;
            _write_step = (_write_step+1)%_capacity;
            sem_post(&_sem_lock);//解锁
            sem_post(&_sem_data);//数字资源+1
            return true;
        }
        bool Pop(int *data){
            sem_wait(&_sem_data);//数据资源-1
            sem_wait(&_sem_lock);//加锁
            *data = _arry[_read_step];
            _read_step = (_read_step+1)%_capacity;
            sem_post(&_sem_lock);//解锁
            sem_post(&_sem_idle);//空闲空间+1
            return true;
        }
};
void *consumer(void *arg)
{
    RingQueue *q = (RingQueue*)arg;
    int data;
    while(1) {
        q->Pop(&data);
        printf("%p- pop data:%d\n", pthread_self(), data);
    }
    return NULL;
}
void *productor(void *arg)
{
    RingQueue *q = (RingQueue*)arg;
    int i = 0;
    while(1) {
        q->Push(i);
        printf("%p push data:%d\n", pthread_self(), i++);
    }
    return NULL;
}
int main()
{
    pthread_t ctid[4], ptid[14];
    int ret;
    RingQueue q;

    for (int i = 0; i < 4; i++) {
        ret = pthread_create(&ctid[i], NULL, consumer, (void*)&q);
        if (ret != 0) {
            printf("thread create error\n");
            return -1;
        }
    }
    for (int i = 0; i < 4; i++) {
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
