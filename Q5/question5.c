#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Total Nodes: A, B, C, D(L), D(R), E, F, G(T), G(B), H
#define V 10 
#define MAX_EDGES 40 // Enough for bidirectional edges

// Edge Structure
typedef struct {
    int src, dest, weight;
} Edge;

// Graph Structure
typedef struct {
    Edge edges[MAX_EDGES];
    int numEdges;
} Graph;

// Helper to handle the duplicate names in the diagram
void getBranchName(int index, char *buffer) {
    const char *names[] = {
        "A", "B", "C", 
        "D (Left)", "D (Right)", 
        "E", "F", 
        "G (Top)", "G (Bottom)", 
        "H"
    };
    if (index >= 0 && index < V) {
        strcpy(buffer, names[index]);
    } else {
        strcpy(buffer, "?");
    }
}

// Bellman-Ford Algorithm
void BellmanFord(Graph* graph, int src) {
    int dist[V];
    char nameBuffer[20];
    char nameBuffer2[20];

    // 1. Initialize distances
    for (int i = 0; i < V; i++) dist[i] = INT_MAX;
    dist[src] = 0;

    // 2. Relax edges |V| - 1 times
    for (int i = 1; i <= V - 1; i++) {
        for (int j = 0; j < graph->numEdges; j++) {
            int u = graph->edges[j].src;
            int v = graph->edges[j].dest;
            int weight = graph->edges[j].weight;

            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }

    // 3. Detect Negative Cycles
    int cycleDetected = 0;
    for (int j = 0; j < graph->numEdges; j++) {
        int u = graph->edges[j].src;
        int v = graph->edges[j].dest;
        int weight = graph->edges[j].weight;

        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            cycleDetected = 1;
            getBranchName(u, nameBuffer);
            getBranchName(v, nameBuffer2);
            printf("\n[ALERT] Negative Weight Cycle Detected between %s and %s!\n", 
                   nameBuffer, nameBuffer2);
        }
    }

    if (!cycleDetected) {
        printf("\n[Status] No negative-weight cycles detected in the current network.\n");
    }

    // 4. Print Results
    printf("\n--- Shortest Risk Paths from Branch A ---\n");
    printf("%-15s | %-10s\n", "Target Branch", "Min Cost");
    printf("--------------------------------\n");
    for (int i = 0; i < V; i++) {
        getBranchName(i, nameBuffer);
        if (dist[i] == INT_MAX)
            printf("%-15s | Unreachable\n", nameBuffer);
        else
            printf("%-15s | %d\n", nameBuffer, dist[i]);
    }
    printf("--------------------------------\n");
}

// Helper to add bidirectional edge
void addEdge(Graph *g, int u, int v, int w) {
    g->edges[g->numEdges].src = u;
    g->edges[g->numEdges].dest = v;
    g->edges[g->numEdges].weight = w;
    g->numEdges++;

    g->edges[g->numEdges].src = v;
    g->edges[g->numEdges].dest = u;
    g->edges[g->numEdges].weight = w;
    g->numEdges++;
}

int main() {
    Graph graph;
    graph.numEdges = 0;

    // Node Mapping:
    // 0:A, 1:B, 2:C, 3:D(L), 4:D(R), 5:E, 6:F, 7:G(T), 8:G(B), 9:H

    // Adding Edges from Image (Screenshot 2026-02-10 14-36-33)
    addEdge(&graph, 0, 1, 4);   // A-B
    addEdge(&graph, 0, 3, 16);  // A-D(L)
    addEdge(&graph, 1, 2, 6);   // B-C
    addEdge(&graph, 1, 3, 6);   // B-D(L)
    addEdge(&graph, 1, 4, 7);   // B-D(R)
    addEdge(&graph, 2, 7, 9);   // C-G(T)
    addEdge(&graph, 3, 5, 7);   // D(L)-E
    addEdge(&graph, 3, 4, 0);   // D(L)-D(R) [No Value Visible, assuming 0/Direct]
    addEdge(&graph, 4, 6, 3);   // D(R)-F
    addEdge(&graph, 5, 6, 10);  // E-F
    addEdge(&graph, 5, 8, 2);   // E-G(B)
    addEdge(&graph, 6, 7, 0);   // F-G(T) [No Value Visible, assuming 0/Direct]
    addEdge(&graph, 6, 8, 10);  // F-G(B)
    addEdge(&graph, 7, 9, 13);  // G(T)-H

    /* NOTE: To test "Negative Cycle Detection",
       uncomment the line below. It creates a cycle B -> D(R) -> B with net negative weight.
    */
    // addEdge(&graph, 4, 1, -10); 

    printf("Calculating Risk Paths for 10-Node Transaction Network...\n");
    BellmanFord(&graph, 0); // Start from A (Node 0)

    return 0;
}
