#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/**
 * Node and LinkedQueue definitions
 */

typedef struct Node {
    void *item;
    struct Node *next;
} Node;

typedef struct LinkedQueue {
    int n;
    Node *first;
    Node *last;
} LinkedQueue;

// Validates internal consistency of the queue
int checkQueue(LinkedQueue *queue) {
    if (queue->n < 0) return 0;
    if (queue->n == 0) return queue->first == NULL && queue->last == NULL;
    if (queue->n == 1) return queue->first == queue->last && queue->first->next == NULL;

    if (queue->first == NULL || queue->last == NULL || queue->first == queue->last) return 0;

    int count = 0;
    for (Node *x = queue->first; x != NULL && count <= queue->n; x = x->next)
        count++;
    if (count != queue->n) return 0;

    Node *lastNode = queue->first;
    while (lastNode->next != NULL) lastNode = lastNode->next;
    return queue->last == lastNode;
}

LinkedQueue* createQueue() {
    LinkedQueue *queue = malloc(sizeof(LinkedQueue));
    if (!queue) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    queue->n = 0;
    queue->first = queue->last = NULL;
    assert(checkQueue(queue));
    return queue;
}

int isEmpty(LinkedQueue *queue) {
    return queue->first == NULL;
}

int size(LinkedQueue *queue) {
    return queue->n;
}

void* peek(LinkedQueue *queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "Queue underflow\n");
        exit(EXIT_FAILURE);
    }
    return queue->first->item;
}

void enqueue(LinkedQueue *queue, void *item) {
    Node *oldLast = queue->last;
    Node *newNode = malloc(sizeof(Node));
    if (!newNode) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->item = item;
    newNode->next = NULL;
    queue->last = newNode;
    if (isEmpty(queue)) queue->first = newNode;
    else oldLast->next = newNode;
    queue->n++;
    assert(checkQueue(queue));
}

void* dequeue(LinkedQueue *queue) {
    if (isEmpty(queue)) {
        fprintf(stderr, "Queue underflow\n");
        exit(EXIT_FAILURE);
    }
    Node *oldFirst = queue->first;
    void *item = oldFirst->item;
    queue->first = oldFirst->next;
    free(oldFirst);
    queue->n--;
    if (isEmpty(queue)) queue->last = NULL;
    assert(checkQueue(queue));
    return item;
}

void freeQueue(LinkedQueue *queue) {
    while (!isEmpty(queue)) {
        dequeue(queue);
    }
    free(queue);
}

#define MAX_STR_LEN 100

int main(void) {
    LinkedQueue *queue = createQueue();
    char input[MAX_STR_LEN];

    printf("Enter strings to enqueue, or '-' to dequeue. Ctrl+D to end:\n");

    while (scanf("%s", input) == 1) {
        if (strcmp(input, "-") != 0) {
            char *item = strdup(input);
            if (!item) {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            enqueue(queue, item);
        } else if (!isEmpty(queue)) {
            char *item = (char *) dequeue(queue);
            printf("%s ", item);
            free(item);
        }
    }

    printf("\n(%d left on queue)\n", size(queue));
    freeQueue(queue);
    return 0;
}
