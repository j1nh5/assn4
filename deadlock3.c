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
    while (1) { // 성공할 때까지 계속 재도전
        pthread_mutex_lock(&mutex_b); // B를 먼저 잡음
        sleep(1);
        
        // 💡 해결 포인트: A가 잠겨있는지 찔러보기 (trylock)
        // 0이 나오면 성공, 0이 아니면 남이 쓰고 있다는 뜻
        if (pthread_mutex_trylock(&mutex_a) == 0) { 
            printf("[Thread 2] 작업 완료!\n");
            pthread_mutex_unlock(&mutex_a);
            pthread_mutex_unlock(&mutex_b);
            break; // 작업 성공했으니 반복문 탈출!
        } 
        else {
            // 데드락 위험(Avoid)! 쥐고 있던 B를 풀고 도망간다.
            printf("[Thread 2] A가 잠겨있어 데드락 위험! 쥐고있던 B를 풀고 양보합니다.\n");
            pthread_mutex_unlock(&mutex_b);
            sleep(1); // 스레드 1이 끝날 때까지 잠깐 기다려줌
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
