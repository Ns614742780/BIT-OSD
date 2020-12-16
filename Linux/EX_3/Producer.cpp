#include <bits/stdc++.h>
#include <sys/shm.h>
#include <sys/unistd.h>
#include <sys/sem.h>
#include <unistd.h>
#include <iostream>
using namespace std;
#define CACHE_SIZE 4
#define CONSUMER_NUM 4
#define PRODUCER_NUM 3

int CHACHE_SHMID = -1;
int POINTER_SHMID = -1;
int SEMID = -1;
//规定信号序列号0是mutex=1，1是full=0，2是empty=CACHE_SIZE
enum
{
    mutex_i,
    full_i,
    empty_i
};

void P(int i)
{
    sembuf op;
    op.sem_flg = 0;
    op.sem_num = i;
    op.sem_op = -1;
    semop(SEMID, &op, 1);
}
void V(int i)
{
    sembuf op;
    op.sem_flg = 0;
    op.sem_num = i;
    op.sem_op = +1;
    semop(SEMID, &op, 1);
}

int main(int argc, char const *argv[])
{
    cout << "I'm Producer!" << endl;
    //初始化
    CHACHE_SHMID = atoi(argv[0]);
    POINTER_SHMID = atoi(argv[1]);
    SEMID = atoi(argv[2]);

    int *pt = (int *)shmat(POINTER_SHMID, NULL, 0);
    char *cache = (char *)shmat(CHACHE_SHMID, NULL, 0);
    pt = pt + 1;

    const char *NAME = "WZC";

    for (int i = 0; i < 4; i++)
    {
        srand(clock());
        //随机睡眠
        sleep(rand() % 2);
        //随机选一个字母提交
        char c = NAME[rand() % 3];

        P(empty_i);
        P(mutex_i);
        //放入字符
        cache[*pt] = c;
        cout << "puted:" << c << endl;
        //修改指针
        *pt = (*pt + 1) % CACHE_SIZE;

        V(full_i);
        V(mutex_i);
    }

    return 0;
}