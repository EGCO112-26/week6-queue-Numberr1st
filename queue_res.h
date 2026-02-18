#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "node_res.h"

typedef struct Queue {
    order *head, *tail;
    int size;
} Queue;

// Initialize queue
Queue* initQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

// Enqueue operation
void enqueue(Queue* q, int order_num, char* item, int price, int customer_id) {
    if (!q) return;
    
    order* new_order = (order*)malloc(sizeof(order));
    if (new_order) {
        new_order->next = NULL;
        new_order->order_number = order_num;
        strcpy(new_order->item, item);
        new_order->price = price;
        new_order->customer_id = customer_id;
        new_order->payment_received = 0;
        
        if (q->size == 0) {
            q->head = new_order;
            q->tail = new_order;
        } else {
            q->tail->next = new_order;
            q->tail = new_order;
        }
        q->size++;
    }
}

// Dequeue operation
order* dequeue(Queue* q) {
    if (!q || q->size == 0) return NULL;
    
    order* temp = q->head;
    q->head = q->head->next;
    q->size--;
    
    if (q->size == 0) {
        q->tail = NULL;
    }
    
    return temp;
}

// Get queue size
int getQueueSize(Queue* q) {
    return q ? q->size : 0;
}

// Display all orders in queue
void displayQueue(Queue* q) {
    if (!q || q->size == 0) {
        printf("Queue is empty\n");
        return;
    }
    
    order* current = q->head;
    int count = 0;
    printf("\n========== Current Queue ==========\n");
    while (current) {
        count++;
        printf("Position %d: Order #%d | Customer #%d | %s | Price: %d\n",
               count, current->order_number, current->customer_id,
               current->item, current->price);
        current = current->next;
    }
    printf("===================================\n\n");
}

// Clear entire queue
void clearQueue(Queue* q) {
    if (!q) return;
    
    while (q->head) {
        order* temp = q->head;
        q->head = q->head->next;
        free(temp);
    }
    q->tail = NULL;
    q->size = 0;
}

// Free queue
void freeQueue(Queue* q) {
    if (q) {
        clearQueue(q);
        free(q);
    }
}

#endif // QUEUE_H
