#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // sleep 함수를 위해 사용

// 실습 자료와 동일한 글로벌 변수명 사용
pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;

// 실습 자료와 동일한 함수명: thread1_start
void* thread1_start(void* arg) {
    printf("[Thread 1] mutex_a 요구(잠금 시도)...\n");
    pthread_mutex_lock(&mutex_a); // 조건 1, 3: mutex_a 점유
    printf("[Thread 1] mutex_a 점유 완료!\n");

    // 약간의 지연 (다른 스레드가 mutex_b를 점유할 시간을 줌)
    sleep(1); 

    printf("[Thread 1] mutex_b 요구(잠금 시도) -> 여기서 대기(Deadlock)...\n");
    pthread_mutex_lock(&mutex_b); // 조건 2, 4: mutex_a를 쥔 상태로 mutex_b 요구

    // 아래 코드는 데드락 때문에 영원히 실행되지 않습니다.
    printf("[Thread 1] mutex_b 점유 완료!\n");
    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);
    
    return NULL;
}

// 실습 자료와 동일한 함수명: thread2_start
void* thread2_start(void* arg) {
    printf("[Thread 2] mutex_b 요구(잠금 시도)...\n");
    pthread_mutex_lock(&mutex_b); // 조건 1, 3: mutex_b 점유
    printf("[Thread 2] mutex_b 점유 완료!\n");

    // 약간의 지연
    sleep(1); 

    printf("[Thread 2] mutex_a 요구(잠금 시도) -> 여기서 대기(Deadlock)...\n");
    pthread_mutex_lock(&mutex_a); // 조건 2, 4: mutex_b를 쥔 상태로 mutex_a 요구

    // 아래 코드는 데드락 때문에 영원히 실행되지 않습니다.
    printf("[Thread 2] mutex_a 점유 완료!\n");
    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);
    
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    printf("--- 프로그램 시작 (Deadlock 발생 테스트) ---\n");

    // 스레드 생성
    pthread_create(&thread1, NULL, thread1_start, NULL);
    pthread_create(&thread2, NULL, thread2_start, NULL);

    // 스레드 종료 대기
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("--- 프로그램 정상 종료 (이 문구는 출력되지 않음) ---\n");
    return 0;
}
