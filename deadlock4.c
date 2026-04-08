#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;

void* thread1_start(void* arg) {
    pthread_mutex_lock(&mutex_a);
    sleep(1);
    pthread_mutex_lock(&mutex_b);
    printf("[Thread 1] 작업 완료!\n");
    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);
    return NULL;
}

void* thread2_start(void* arg) {
    int fail_count = 0; // 실패 횟수를 기억할 변수

    pthread_mutex_lock(&mutex_b);
    
    // 💡 해결 포인트: trylock이 실패할 때마다 실패 횟수를 셉니다.
    while (pthread_mutex_trylock(&mutex_a) != 0) {
        fail_count++;
        printf("[Thread 2] A 잠금 실패... (실패 횟수: %d)\n", fail_count);
        sleep(1);

        // 탐지(Detect): 3번 연속 실패하면 데드락으로 간주!
        if (fail_count >= 3) {
            printf("[!] Deadlock 탐지됨! 복구(Recover)를 위해 B를 내려놓습니다.\n");
            pthread_mutex_unlock(&mutex_b); // 복구(Recover): 내 자원을 포기
            
            sleep(3); // 스레드 1이 일 다 할 때까지 넉넉히 쉬어줌
            
            // 처음부터 다시 시작
            printf("[Thread 2] 다시 B부터 잠그고 시작합니다.\n");
            pthread_mutex_lock(&mutex_b);
            fail_count = 0; // 카운트 초기화
        }
    }
    
    printf("[Thread 2] 작업 완료!\n");
    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);
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
