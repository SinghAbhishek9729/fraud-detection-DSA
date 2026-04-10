#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 100

// -------------------------------
// STRUCTURE (Linked List Node)
// -------------------------------
struct Transaction {
    char user_id[20];
    float amount;
    char location[20];
    struct Transaction* next;   // LINKED LIST POINTER
};

// -------------------------------
// LINKED LIST (Storage)
// -------------------------------
struct Transaction* head = NULL;

// Insert at beginning
void insertTransaction(char user_id[], float amount, char location[]) {
    struct Transaction* newNode = (struct Transaction*)malloc(sizeof(struct Transaction));

    strcpy(newNode->user_id, user_id);
    newNode->amount = amount;
    strcpy(newNode->location, location);

    newNode->next = head;
    head = newNode;
}

// -------------------------------
// QUEUE (FIFO Processing)
// -------------------------------
struct Transaction* queue[MAX];
int front = -1, rear = -1;

void enqueue(struct Transaction* t) {
    if (rear == MAX - 1) return;
    if (front == -1) front = 0;
    queue[++rear] = t;
}

struct Transaction* dequeue() {
    if (front == -1 || front > rear) return NULL;
    return queue[front++];
}

// -------------------------------
// HASHING (User Transaction Count)
// -------------------------------
struct Hash {
    char user_id[20];
    int count;
};

struct Hash hashTable[MAX];
int h_size = 0;

int getUserIndex(char user_id[]) {
    for (int i = 0; i < h_size; i++) {
        if (strcmp(hashTable[i].user_id, user_id) == 0)
            return i;
    }
    return -1;
}

void updateHash(char user_id[]) {
    int idx = getUserIndex(user_id);

    if (idx == -1) {
        strcpy(hashTable[h_size].user_id, user_id);
        hashTable[h_size].count = 1;
        h_size++;
    } else {
        hashTable[idx].count++;
    }
}

// -------------------------------
// STACK (Flagged Transactions)
// -------------------------------
struct Transaction* stack[MAX];
int top = -1;

void push(struct Transaction* t) {
    if (top == MAX - 1) return;
    stack[++top] = t;
}

// -------------------------------
// FRAUD DETECTION ALGORITHM
// -------------------------------
void checkFraud(struct Transaction* t) {
    int idx = getUserIndex(t->user_id);

    if (t->amount > 10000) {
        printf("⚠ Fraud: High Amount (%s)\n", t->user_id);
        push(t);
    }
    else if (strcmp(t->location, "India") != 0) {
        printf("⚠ Fraud: Foreign Location (%s)\n", t->user_id);
        push(t);
    }
    else if (idx != -1 && hashTable[idx].count > 3) {
        printf("⚠ Fraud: Too Many Transactions (%s)\n", t->user_id);
        push(t);
    }
    else {
        printf("✅ Safe Transaction (%s)\n", t->user_id);
    }
}

// -------------------------------
// MAIN FUNCTION
// -------------------------------
int main(int argc, char *argv[]) {

    // Step 1: Insert into Linked List
    insertTransaction(argv[1], atof(argv[2]), argv[3]);

    // Step 2: Add to Queue
    enqueue(head);

    // Step 3: Update Hashing
    updateHash(argv[1]);

    // Step 4: Process Queue
    struct Transaction* t = dequeue();

    if (t != NULL) {
        checkFraud(t);
    }

    return 0;
}