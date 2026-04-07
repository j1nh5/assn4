#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;

void* thread1_start(void* arg) {
    printf("[Thread 1] mutex_a 점유 시도...\n");
    pthread_mutex_lock(&mutex_a);
    printf("[Thread 1] mutex_a 점유 완료!\n");
    sleep(1);
    printf("[Thread 1] mutex_b 점유 시도...\n");
    pthread_mutex_lock(&mutex_b);
    printf("[Thread 1] mutex_b 점유 완료! (작업 성공)\n");

    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);
    return NULL;
}

void* thread2_start(void* arg) {
    // 💡 해결 포인트: 스레드 2도 무조건 mutex_a부터 잡게 만듭니다!
    printf("[Thread 2] mutex_a 점유 시도...\n");
    pthread_mutex_lock(&mutex_a);
    printf("[Thread 2] mutex_a 점유 완료!\n");
    sleep(1);
    printf("[Thread 2] mutex_b 점유 시도...\n");
    pthread_mutex_lock(&mutex_b);
    printf("[Thread 2] mutex_b 점유 완료! (작업 성공)\n");

    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    pthread_create(&thread1, NULL, thread1_start, NULL);
    pthread_create(&thread2, NULL, thread2_start, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    printf("--- 프로그램 정상 종료 (Deadlock 예방 성공!) ---\n");
    return 0;
}
