#include<stdio.h>
#include<stdlib.h> 
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
int max = 4;
int czekaA = 0;
int czekaB = 0;
void miastoA(int num);
void miastoB(int num);
void *most(void *num);
pthread_mutex_t mutex;
pthread_mutex_t mutex2;
sem_t sem;

int main(int argc, char*argv[]){

    if(argc != 2){
        printf("Zła ilość argumentów");
        exit(0);
    }
    srand(time(NULL));
    int samochody = atoi(argv[1]);
    printf("ilość samochodów: %d\n\n",samochody);
    pthread_t samochodyL[samochody];
    int index[samochody];
    
    if(sem_init(&sem,0,1)==-1||pthread_mutex_init(&mutex,NULL)==-1||pthread_mutex_init(&mutex2,NULL)==-1){
        perror("Inicjalizacja mutexa/u (?odmiana?) się powiodła zakończeniem");
        exit(-1);
    }
    

    for(int i=0;i<samochody;i++){
        index[i]=i+1;
        pthread_create(&samochodyL[i], NULL, &most, (index+i));
    }
    for(int i=0;i<samochody;i++){
        pthread_join(samochodyL[i], NULL);
    }

}

void *most(void *num){
    while(1){

        int* ind = (int*)num;
        
        pthread_mutex_lock(&mutex);
        czekaB++;
        pthread_mutex_unlock(&mutex);
        sem_wait(&sem);
        czekaB--;
        miastoA(*ind);

        pthread_mutex_lock(&mutex);
        czekaA++;
        pthread_mutex_unlock(&mutex);
        sem_wait(&sem);
        czekaA--;
        miastoB(*ind);
    }
}

void miastoA(int num){
    pthread_mutex_lock(&mutex2);
    int czas = (rand() % max)+1;
    printf("B %d >> [>>%d>>] >> %d A ||czas w mieście: %d\n",czekaB,num,czekaA, czas);
    pthread_mutex_unlock(&mutex2);
    sem_post(&sem);
    sleep(czas);
}

void miastoB(int num){
    pthread_mutex_lock(&mutex2);
    int czas = (rand() % max)+1;
    printf("B %d << [<<%d<<] << %d  A ||czas w mieście: %d\n",czekaB,num,czekaA,czas);
    pthread_mutex_unlock(&mutex2);
    sem_post(&sem);
    sleep(czas);
}