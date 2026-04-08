#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;

void* thread1_start(void* arg) {
     printf("[Thread1] mutex_a 요구 중...\n");
     pthread_mutex_lock(&mutex_a);
     printf("[Thread1] mutex_a 점유 성공!\n");

     sleep(1); 

     printf("[Thread1] mutex_b 요구 중...\n");
     pthread_mutex_lock(&mutex_b);
     printf("[Thread1] mutex_b 점유 성공!\n");
     pthread_mutex_unlock(&mutex_b);
     pthread_mutex_unlock(&mutex_a);
    
     return NULL;
}

void* thread2_start(void* arg) {
    while (1) {
        printf("[Thread2] mutex_b 요구 중...\n");
        pthread_mutex_lock(&mutex_b);
        printf("[Thread2] mutex_b 점유 성공!\n");
        sleep(1);
        
        if (pthread_mutex_trylock(&mutex_a) == 0) { 
            printf("[Thread2] 작업 완료!\n");
            pthread_mutex_unlock(&mutex_a);
            pthread_mutex_unlock(&mutex_b);
            break;
        } 
        else {
            printf("[Thread2] Thread1이 mutex_a 사용 중 mutex_b 풀어주기.\n");
            pthread_mutex_unlock(&mutex_b);
            sleep(1);
        }
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread1_start, NULL);
    pthread_create(&thread2, NULL, thread2_start, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("--- 프로그램 정상 종료 ---\n");
    return 0;
}
