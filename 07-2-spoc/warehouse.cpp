#include<iostream>
#include<semaphore.h>
#include<stdio.h>
#include<pthread.h>
#define MAX 500

using namespace std;
int countA, countB;
int M, N;
sem_t leftA, leftB;
sem_t lock;

bool check(){
    return (countA - countB >=-M && countA -countB <=N);
}

void print(){
    cout<<"countA: "<<countA<<' '<<"countB: "<<countB<<" Minus"<<countA-countB<<' '<<check()<<endl;
    if(!check()) cout<<"Error!"<<endl;
}

void* procB(void* ){
    for(int i=0;i<MAX; i++){
        sem_wait(&leftB);
        sem_wait(&lock);
        if(!check()){
            sem_post(&lock);
            sem_post(&leftB);
            continue;
        }
        countB++;
        print();
        sem_post(&lock);
        sem_post(&leftA);
    }
    return NULL;
}

void* procA(void* ){
    for(int i=0;i<MAX; i++){
        sem_wait(&leftA);
        sem_wait(&lock);
        if(!check()){
            sem_post(&lock);
            sem_post(&leftA);
            continue;
        }
        countA++;
        print();
        sem_post(&lock);
        sem_post(&leftB);
    }
    return NULL;
}
int main(){
    cin>>M>>N;
    sem_init(&leftA,0, N);
    sem_init(&leftB,0, M);
    sem_init(&lock, 0, 1);
    pthread_t threads[2];
    pthread_create(&threads[0], NULL, procA, NULL);
    pthread_create(&threads[1], NULL, procB, NULL);
    for(int i=0;i<2;i++)    { pthread_join(threads[i], NULL);}
    return 0;
}
