#include <stdio.h>
#include <stdlib.h>

typedef struct Process {
    int id;
    int priority;
    int state; // 0: ready, 1: running, 2: blocked, 3: terminated
    int remaining_cpu_time;
    int remaining_io_time;
    int io_blocked;
} Process;

typedef struct Process_Queue {
    int size;
    int front;
    int rear;
    Process *processes[100];
} Process_Queue;

void enqueue(Process_Queue *queue, Process *process) {
    if((queue->rear + 1) % 100 == queue->front) {
        printf("Queue is full.\n");
        return;
    }
    queue->processes[queue->rear] = process;
    queue->rear = (queue->rear + 1) % 100;
    queue->size++;
}

Process *dequeue(Process_Queue *queue) {
    if(queue->size == 0) {
        printf("Queue is empty.\n");
        return NULL;
    }
    Process *process = queue->processes[queue->front];
    queue->front = (queue->front + 1) % 100;
    queue->size--;
    return process;
}

void schedule(Process_Queue *ready_queue, Process_Queue *blocked_queue) {
    Process *current_process = NULL;
    while(1) {
        if(ready_queue->size == 0) {
            printf("No ready processes. Waiting for I/O...\n");
            // Wait for I/O operations to complete
            // Update the state of processes in the blocked_queue
            // Move ready processes from blocked_queue to ready_queue
            // Continue scheduling
        } else {
            current_process = dequeue(ready_queue);
            printf("Running process %d...\n", current_process->id);
            // Simulate CPU and I/O operations
            // Update the state of the current process
            // If the process is terminated, free its memory
            // If the process is blocked, enqueue it to the blocked_queue
            // If the process is ready, enqueue it to the ready_queue
        }
    }
}

int main() {
    Process_Queue ready_queue;
    Process_Queue blocked_queue;
    ready_queue.size = 0;
    ready_queue.front = 0;
    ready_queue.rear = 0;
    blocked_queue.size = 0;
    blocked_queue.front = 0;
    blocked_queue.rear = 0;

    // Create processes and enqueue them to the ready_queue or blocked_queue
    // Call the schedule function to start scheduling processes

    return 0;
}