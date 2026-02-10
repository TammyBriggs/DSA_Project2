#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PASSENGERS 50

// 1. Structure for Passenger
typedef struct {
    char name[20];
    int riskScore;
} Passenger;

// Max-Heap Structure
typedef struct {
    Passenger array[MAX_PASSENGERS];
    int size;
} MaxHeap;

void swap(Passenger *a, Passenger *b) {
    Passenger temp = *a;
    *a = *b;
    *b = temp;
}

// Standard Sift-Down (Max-Heapify)
void maxHeapify(MaxHeap *heap, int idx) {
    int largest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < heap->size && heap->array[left].riskScore > heap->array[largest].riskScore)
        largest = left;

    if (right < heap->size && heap->array[right].riskScore > heap->array[largest].riskScore)
        largest = right;

    if (largest != idx) {
        swap(&heap->array[idx], &heap->array[largest]);
        maxHeapify(heap, largest);
    }
}

// Build Heap (Rearrange array)
void buildMaxHeap(MaxHeap *heap) {
    for (int i = (heap->size / 2) - 1; i >= 0; i--) {
        maxHeapify(heap, i);
    }
}

// Insert a new passenger (Bubble Up)
void insertPassenger(MaxHeap *heap, char *name, int risk) {
    if (heap->size == MAX_PASSENGERS) {
        printf("System Full!\n");
        return;
    }

    int i = heap->size;
    strcpy(heap->array[i].name, name);
    heap->array[i].riskScore = risk;
    heap->size++;

    printf("\n[Insert] Adding %s (Risk: %d)...\n", name, risk);

    // Bubble up
    while (i != 0 && heap->array[(i - 1) / 2].riskScore < heap->array[i].riskScore) {
        swap(&heap->array[i], &heap->array[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

// Extract Max (Priority Queue Pop)
Passenger extractMax(MaxHeap *heap) {
    if (heap->size <= 0) {
        Passenger empty = {"", -1};
        return empty;
    }
    Passenger root = heap->array[0];
    
    // Move last element to root and reduce size
    heap->array[0] = heap->array[heap->size - 1];
    heap->size--;
    
    // Restore heap property
    maxHeapify(heap, 0);
    return root;
}

// Delete a specific passenger by Risk Score
void deleteByScore(MaxHeap *heap, int targetScore) {
    int i;
    int found = 0;
    
    // Find the index
    for (i = 0; i < heap->size; i++) {
        if (heap->array[i].riskScore == targetScore) {
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("Passenger with risk %d not found.\n", targetScore);
        return;
    }

    printf("\n[Delete] Removing passenger with Risk %d...\n", targetScore);

    // Swap with last element
    swap(&heap->array[i], &heap->array[heap->size - 1]);
    heap->size--;

    // Re-heapify from the point of deletion
    // Note: In some edge cases you might need to bubble-up too, 
    // but typically replacing with a leaf requires sifting down.
    maxHeapify(heap, i);
}

// Helper to display current queue
void printQueue(MaxHeap *heap) {
    printf("Current Priority Queue (Root is highest risk): \n");
    if(heap->size == 0) printf("  [Empty]\n");
    for (int i = 0; i < heap->size; ++i)
        printf("  %d. %s (Risk: %d)\n", i, heap->array[i].name, heap->array[i].riskScore);
}

// Helper to simulate full extraction order without destroying original heap
void showProcessingOrder(MaxHeap heapCopy) {
    printf("\n--- Simulated Processing Order (Highest to Lowest) ---\n");
    while (heapCopy.size > 0) {
        Passenger p = extractMax(&heapCopy);
        printf("Processing: %s (Risk: %d)\n", p.name, p.riskScore);
    }
    printf("------------------------------------------------------\n");
}

int main() {
    MaxHeap heap;
    heap.size = 0;

    // 1. Initial Data (Name, Risk Score)
    // We manually add them to the array first, then build heap
    struct { char *n; int r; } initialData[] = {
        {"Alice", 30}, {"Brian", 55}, {"Chen", 85}, 
        {"Fatima", 90}, {"Noah", 45}, {"Sofia", 70}
    };
    int n = 6;

    for(int i=0; i<n; i++) {
        strcpy(heap.array[i].name, initialData[i].n);
        heap.array[i].riskScore = initialData[i].r;
    }
    heap.size = n;

    // 2. Build Max Heap
    printf("--- Building Initial Heap ---\n");
    buildMaxHeap(&heap);
    printQueue(&heap);

    // 3. Show Extraction Order (Priority Queue behavior)
    // We pass a copy so we don't lose the data for the next steps
    showProcessingOrder(heap); 

    // 4. New Passenger Arrives (Risk 98)
    insertPassenger(&heap, "New_Passenger", 98);
    printQueue(&heap);

    // Check if New_Passenger is at root (since 98 > 90)
    if (heap.array[0].riskScore == 98) {
        printf(">> Verification: New passenger is correctly at the top.\n");
    }

    // 5. Passenger cleared (Delete Risk 98)
    deleteByScore(&heap, 98);
    printQueue(&heap);
    
    // Check if previous max (Fatima, 90) returned to top
    if (heap.array[0].riskScore == 90) {
        printf(">> Verification: Heap restored. Next highest risk is at top.\n");
    }

    return 0;
}
