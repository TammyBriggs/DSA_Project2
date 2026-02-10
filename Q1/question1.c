#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

// Structure to represent a Job
typedef struct {
    char id;
    int priority;
} Job;

// Structure to represent the Max-Heap
typedef struct {
    Job array[MAX_SIZE];
    int size;
} MaxHeap;

// Function to swap two jobs
void swap(Job *a, Job *b) {
    Job temp = *a;
    *a = *b;
    *b = temp;
}

// Function to maintain Max-Heap property (Sift Down)
// Used during Build Heap and Delete
void maxHeapify(MaxHeap *heap, int idx) {
    int largest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    // Check if left child exists and is greater than root
    if (left < heap->size && heap->array[left].priority > heap->array[largest].priority)
        largest = left;

    // Check if right child exists and is greater than largest so far
    if (right < heap->size && heap->array[right].priority > heap->array[largest].priority)
        largest = right;

    // If largest is not root
    if (largest != idx) {
        swap(&heap->array[idx], &heap->array[largest]);
        // Recursively heapify the affected sub-tree
        maxHeapify(heap, largest);
    }
}

// Function to build Max-Heap from an arbitrary array
void buildMaxHeap(MaxHeap *heap) {
    // Start from the last non-leaf node and move up
    int startIdx = (heap->size / 2) - 1;
    for (int i = startIdx; i >= 0; i--) {
        maxHeapify(heap, i);
    }
}

// Function to insert a new job (Bubble Up)
void insertJob(MaxHeap *heap, char id, int priority) {
    if (heap->size == MAX_SIZE) {
        printf("Heap Overflow\n");
        return;
    }

    // Insert at the end
    int i = heap->size;
    heap->array[i].id = id;
    heap->array[i].priority = priority;
    heap->size++;

    printf("\n[Operation] Inserting Job %c (Priority %d)...\n", id, priority);

    // Fix the max heap property if it is violated (Bubble Up)
    while (i != 0 && heap->array[(i - 1) / 2].priority < heap->array[i].priority) {
        swap(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Function to delete the root (Max priority job)
void deleteRoot(MaxHeap *heap) {
    if (heap->size <= 0) return;

    printf("\n[Operation] Deleting Root Job %c (Priority %d)...\n", 
           heap->array[0].id, heap->array[0].priority);

    // Replace root with last element
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;

    // Restore Max-Heap property
    maxHeapify(heap, 0);
}

// Helper to print the heap
void printHeap(MaxHeap *heap) {
    printf("Current Heap State: ");
    for (int i = 0; i < heap->size; ++i)
        printf("[%c: %d] ", heap->array[i].id, heap->array[i].priority);
    printf("\n");
}

int main() {
    MaxHeap heap;
    heap.size = 0;

    // Initial Data
    int priorities[] = {42, 17, 93, 28, 65, 81, 54, 60, 99, 73, 88};
    int n = sizeof(priorities) / sizeof(priorities[0]);

    // 1. Assign unique identifiers and load into array
    printf("--- Initial Array (Binary Tree Representation) ---\n");
    for (int i = 0; i < n; i++) {
        heap.array[i].priority = priorities[i];
        heap.array[i].id = 'A' + i; // Assign IDs A, B, C...
    }
    heap.size = n;
    printHeap(&heap);

    // 2. Build Max-Heap
    printf("\n--- Building Max-Heap ---\n");
    buildMaxHeap(&heap);
    printHeap(&heap);

    // 3. Insert Urgent Job (Priority 100)
    // We'll use 'L' as the ID for the next job
    insertJob(&heap, 'L', 100);
    printHeap(&heap);

    // 4. Urgent Job Aborted: Remove Job with Priority 100
    // Since it's a Max-Heap, 100 will be at the root (index 0)
    if (heap.array[0].priority == 100) {
        deleteRoot(&heap);
    } else {
        printf("Error: Priority 100 is not at the root!\n");
    }
    printHeap(&heap);

    return 0;
}
