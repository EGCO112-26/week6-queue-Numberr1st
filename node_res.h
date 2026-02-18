#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <string.h>

typedef struct node {
    struct node* next;
    int order_number;
    char item[100];
    int price;
    int customer_id;
    int payment_received;
} order;

#endif // NODE_H
