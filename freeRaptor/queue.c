#include"queue.h"
queue* queue_build() {
    queue *q = (queue *)malloc(sizeof(queue));
    q->data = (int *)malloc(INITIAL_CAPACITY * sizeof(int));
    q->front = 0;
    q->rear = -1;
    q->capacity = INITIAL_CAPACITY;
    q->size = 0;
    return q;
}
int queue_isempty(queue *q) {
    return q->size == 0;
}
void queue_increament(queue *q) {
    int newCapacity = q->capacity * 2;
    q->data = (int *)realloc(q->data, newCapacity * sizeof(int));
    if (!q->data) {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    if (q->rear < q->front) {
        for (int i = 0; i < q->rear + 1; i++) {
            q->data[q->capacity + i] = q->data[i];
        }
        q->rear += q->capacity;
    }
    q->capacity = newCapacity;
}
void queue_push(queue *q, int value) {
    if (q->size == q->capacity) {
        queue_increament(q);
    }
    q->rear = (q->rear + 1) % q->capacity;
    q->data[q->rear] = value;
    q->size++;
}
int queue_pop(queue *q) {
    if (queue_isempty(q)) {
        printf("queue underflow\n");
        exit(EXIT_FAILURE);
    }
    int value = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return value;
}

void queue_display(queue *q) {
    if (queue_isempty(q)) {
        printf("queue is empty\n");
        return;
    }
    printf("queue: ");
    for (int i = 0; i < q->size; i++) {
        int index = (q->front + i) % q->capacity;
        printf("%d ", q->data[index]);
    }
    printf("\n");
}
// int main() {
//     queue *q = queue_build();
//     for(int i=0;i<100;i++){
//         queue_push(q,10*i);
//         queue_display(q);
//     }
//     for(int i=0;i<100;i++){
//         printf("%d ",queue_pop(q));
//     }
//
//     printf(" \n");
//     return 0;
// }
//
