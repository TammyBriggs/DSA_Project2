#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define V 7  // Number of workstations (A-G)
#define MAX_QUEUE 100

// Graph Data (Adjacency Matrix)
// 0 = A, 1 = B, 2 = C, 3 = D, 4 = E, 5 = F, 6 = G
int adj[V][V];

// Queue Structure for BFS
typedef struct {
    int items[MAX_QUEUE];
    int front;
    int rear;
} Queue;

// Queue Operations
void initQueue(Queue* q) {
    q->front = -1;
    q->rear = -1;
}

int isEmpty(Queue* q) {
    return q->rear == -1;
}

void enqueue(Queue* q, int value) {
    if (q->rear == MAX_QUEUE - 1) return;
    if (q->front == -1) q->front = 0;
    q->items[++q->rear] = value;
}

int dequeue(Queue* q) {
    if (isEmpty(q)) return -1;
    int item = q->items[q->front];
    if (q->front >= q->rear) {
        q->front = -1;
        q->rear = -1;
    } else {
        q->front++;
    }
    return item;
}

// Convert index to Char name (0 -> 'A')
char getName(int i) {
    return 'A' + i;
}

// Convert Char name to index ('A' -> 0)
int getIndex(char c) {
    if (c >= 'a' && c <= 'z') c -= 32; // Handle lowercase
    return c - 'A';
}

void bfsAndAnalyze(int startNode) {
    Queue q;
    initQueue(&q);

    bool visited[V] = {false};
    
    // Variables for Risk Analysis
    int maxWeight = -1;
    int riskiestNeighbor = -1;

    // 1. Start BFS
    printf("\n--- Analyzing Network Traffic starting from Workstation %c ---\n", getName(startNode));
    
    visited[startNode] = true;
    enqueue(&q, startNode);

    printf("BFS Traversal Order: ");

    while (!isEmpty(&q)) {
        int current = dequeue(&q);
        printf("%c ", getName(current));

        // 2. If we are currently at the suspected node, analyze its immediate connections
        if (current == startNode) {
            // We look ahead at neighbors without dequeuing them yet
            for (int i = 0; i < V; i++) {
                if (adj[current][i] > 0) { // If there is a connection
                    int weight = adj[current][i];
                    printf("\n   > Direct Contact discovered: %c (Transfer Time: %d mins)", 
                           getName(i), weight);
                    
                    // Check for highest risk
                    if (weight > maxWeight) {
                        maxWeight = weight;
                        riskiestNeighbor = i;
                    }
                }
            }
        }

        // 3. Continue BFS: Enqueue unvisited neighbors
        for (int i = 0; i < V; i++) {
            if (adj[current][i] > 0 && !visited[i]) {
                visited[i] = true;
                enqueue(&q, i);
            }
        }
    }
    printf("\n");

    // 4. Report Findings
    printf("\n-------------------------------------------------\n");
    printf("RISK ASSESSMENT REPORT FOR WORKSTATION %c\n", getName(startNode));
    printf("-------------------------------------------------\n");
    if (riskiestNeighbor != -1) {
        printf("Highest Risk Exposure: Workstation %c\n", getName(riskiestNeighbor));
        printf("Reason: Highest data transfer volume (%d mins).\n", maxWeight);
    } else {
        printf("No outgoing connections found from this workstation.\n");
    }
    printf("-------------------------------------------------\n");
}

int main() {
    // Initialize Matrix with 0
    for(int i=0; i<V; i++) 
        for(int j=0; j<V; j++) 
            adj[i][j] = 0;

    // Load Graph Data from Image (Screenshot 2026-02-10 13-51-13)
    // Undirected graph: adj[u][v] = w AND adj[v][u] = w
    
    // A(0) connections
    adj[0][1] = adj[1][0] = 6;  // A-B
    // A-D line exists but has no number. Treated as 0.

    // B(1) connections
    adj[1][2] = adj[2][1] = 11; // B-C
    adj[1][3] = adj[3][1] = 5;  // B-D

    // C(2) connections
    adj[2][3] = adj[3][2] = 17; // C-D
    adj[2][6] = adj[6][2] = 25; // C-G

    // D(3) connections
    adj[3][4] = adj[4][3] = 22; // D-E
    // D-F line exists but has no number. Treated as 0.

    // E(4) connections
    adj[4][5] = adj[5][4] = 10; // E-F

    // F(5) connections
    adj[5][6] = adj[6][5] = 22; // F-G

    // User Input
    char input;
    printf("Enter the suspected compromised workstation (A-G): ");
    scanf(" %c", &input);

    int startIdx = getIndex(input);

    if (startIdx >= 0 && startIdx < V) {
        bfsAndAnalyze(startIdx);
    } else {
        printf("Invalid Workstation ID.\n");
    }

    return 0;
}
