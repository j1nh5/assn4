#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex_a = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_b = PTHREAD_MUTEX_INITIALIZER;

void* thread1_start(void* arg) {
    printf("[Thread1] Request mutex_a...\n");
    pthread_mutex_lock(&mutex_a);
    printf("[Thread1] Success mutex_a!\n");

    sleep(1); 

    printf("[Thread1] Request mutex_b...\n");
    pthread_mutex_lock(&mutex_b);
    printf("[Thread1] Success mutex_b!\n");
    pthread_mutex_unlock(&mutex_b);
    pthread_mutex_unlock(&mutex_a);
    printf("[Thread1] Complete!\n");
    
    return NULL;
}

void* thread2_start(void* arg) {
    printf("[Thread2] Request mutex_b...\n");
    pthread_mutex_lock(&mutex_b);
    printf("[Thread2] Success mutex_b!\n");

    sleep(1); 

    printf("[Thread2] Request mutex_a...\n");
    pthread_mutex_lock(&mutex_a);
    printf("[Thread2] Success mutex_a!\n");
    pthread_mutex_unlock(&mutex_a);
    pthread_mutex_unlock(&mutex_b);
    printf("[Thread2] Complete!\n");
    
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    printf("--- Program Start ---\n");
    
    pthread_create(&thread1, NULL, thread1_start, NULL);
    pthread_create(&thread2, NULL, thread2_start, NULL);
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("--- Program End ---\n");
    return 0;
}
