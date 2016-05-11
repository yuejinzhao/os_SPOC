#include<iostream>
#include<semaphore.h>
#include<stdio.h>
#include<pthread.h>
#define MAX_LOOP 1000

using namespace std;
int Acount, Bcount;     //仓库中A B货物的数量
int M, N;               //限制条件中的M,N
sem_t A_left, B_left;   //A B剩余的可以装入的数目
sem_t Metux;            //互斥锁

bool check(){
    return (Acount - Bcount >=-M && Acount -Bcount <=N);        //可装填的条件
}
void print(){
    //cout<<"Acount: "<<Acount<<' '<<"Bcount: "<<Bcount<<" Minus"<<Acount-Bcount<<' '<<check()<<endl;
    if(!check()) cout<<"Error!"<<endl;
    //cout等输出太慢，以致于体现不出多线程的调度效果，但是在限定条件之后的输出结果可以看出，输出结果符合预期。
}
void* procA(void* ){
    for(int i=0;i<MAX_LOOP; i++){
        sem_wait(&A_left);
        sem_wait(&Metux);
        if(!check()){   //如果当前不可装填，将之前所做工作的影响都恢复
            sem_post(&Metux);
            sem_post(&A_left);
            continue;
        }
        Acount++;
        print();
        sem_post(&Metux);
        sem_post(&B_left);  //当执行完一次A货物的装填工作时， 需要将剩余B的数量+1
    }
    return NULL;
}
void* procB(void* ){
    for(int i=0;i<MAX_LOOP; i++){
        sem_wait(&B_left);
        sem_wait(&Metux);
        if(!check()){
            sem_post(&Metux);
            sem_post(&B_left);
            continue;
        }
        Bcount++;
        print();
        sem_post(&Metux);
        sem_post(&A_left);
    }
    return NULL;
}
int main(){
    cin>>M>>N;
    sem_init(&A_left,0, N);
    sem_init(&B_left,0, M);
    sem_init(&Metux, 0, 1);
    pthread_t threads[2];
    pthread_create(&threads[0], NULL, procA, NULL);
    pthread_create(&threads[1], NULL, procB, NULL);
    for(int i=0;i<2;i++)    { pthread_join(threads[i], NULL);}
    return 0;
}
