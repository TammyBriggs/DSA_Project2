#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Total Nodes: A, B, C, D, E, F, G, H, I, J
#define V 10 
#define MAX_EDGES 40 // Sufficient for bidirectional graph

// Edge Structure
typedef struct {
    int src, dest, weight;
} Edge;

// Graph Structure
typedef struct {
    Edge edges[MAX_EDGES];
    int numEdges;
} Graph;

// Helper to get Branch Name by Index
void getBranchName(int index, char *buffer) {
    // Mapping: 0=A, 1=B, 2=C, 3=D, 4=E, 5=F, 6=G, 7=H, 8=I, 9=J
    const char *names[] = {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"};
    
    if (index >= 0 && index < V) {
        strcpy(buffer, names[index]);
    } else {
        strcpy(buffer, "?");
    }
}

// Helper to add a bidirectional edge
void addEdge(Graph *g, int u, int v, int w) {
    // Add direction u -> v
    g->edges[g->numEdges].src = u;
    g->edges[g->numEdges].dest = v;
    g->edges[g->numEdges].weight = w;
    g->numEdges++;

    // Add direction v -> u (Undirected Graph)
    g->edges[g->numEdges].src = v;
    g->edges[g->numEdges].dest = u;
    g->edges[g->numEdges].weight = w;
    g->numEdges++;
}

// The Bellman-Ford Algorithm
void BellmanFord(Graph* graph, int src) {
    int dist[V];
    char nameBuffer[10];
    char uName[10], vName[10];

    // 1. Initialize distances
    // Set all to "Infinity" (INT_MAX), except source
    for (int i = 0; i < V; i++) dist[i] = INT_MAX;
    dist[src] = 0;

    // 2. Relax edges |V| - 1 times
    // This finds the shortest path for all nodes
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

    // 3. Detect Negative-Weight Cycles
    int cycleDetected = 0;
    for (int j = 0; j < graph->numEdges; j++) {
        int u = graph->edges[j].src;
        int v = graph->edges[j].dest;
        int weight = graph->edges[j].weight;

        // If we can still reduce the cost, a negative cycle exists
        if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
            cycleDetected = 1;
            getBranchName(u, uName);
            getBranchName(v, vName);
            printf("\n[ALERT] Negative Weight Cycle Detected involving %s -> %s!\n", 
                   uName, vName);
        }
    }

    if (!cycleDetected) {
        printf("\n[Status] No negative-weight cycles detected.\n");
    }

    // 4. Display Final Results
    printf("\n--- Shortest Transaction Paths from Branch A ---\n");
    printf("%-10s | %-15s\n", "Branch", "Min Risk (Cost)");
    printf("------------------------------\n");
    
    // Print alphabetically or by Index? Let's verify mapping:
    // A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7, I=8, J=9
    for (int i = 0; i < V; i++) {
        getBranchName(i, nameBuffer);
        if (dist[i] == INT_MAX)
            printf("%-10s | Unreachable\n", nameBuffer);
        else
            printf("%-10s | %d\n", nameBuffer, dist[i]);
    }
    printf("------------------------------\n");
}

int main() {
    Graph graph;
    graph.numEdges = 0;

    // NODE MAPPING INDEX:
    // A=0, B=1, C=2, D=3, E=4, F=5, G=6, H=7, I=8, J=9
    
    // Edges connected to A
    addEdge(&graph, 0, 1, 4);   // A-B
    addEdge(&graph, 0, 3, 16);  // A-D

    // Edges connected to B (excluding A)
    addEdge(&graph, 1, 2, 6);   // B-C
    addEdge(&graph, 1, 3, 6);   // B-D
    addEdge(&graph, 1, 9, 7);   // B-J

    // Edges connected to C (excluding B)
    addEdge(&graph, 2, 6, 9);   // C-G

    // Edges connected to D (excluding A, B)
    addEdge(&graph, 3, 9, 0);   // D-J (Line present, no number. Assuming 0)
    addEdge(&graph, 3, 4, 7);   // D-E

    // Edges connected to J (excluding B, D)
    addEdge(&graph, 9, 5, 3);   // J-F

    // Edges connected to E (excluding D)
    addEdge(&graph, 4, 5, 10);  // E-F
    addEdge(&graph, 4, 8, 2);   // E-I

    // Edges connected to F (excluding J, E)
    addEdge(&graph, 5, 8, 10);  // F-I
    addEdge(&graph, 5, 6, 0);   // F-G (Line present, no number. Assuming 0)

    // Edges connected to G (excluding C, F)
    addEdge(&graph, 6, 7, 13);  // G-H

    // (H and I are fully connected above)

    /* UNCOMMENT LINE BELOW TO TEST NEGATIVE CYCLE DETECTION */
    // addEdge(&graph, 9, 1, -20); // Creates cycle B-J-B with negative sum

    printf("Calculating Minimum Risk Paths (Bellman-Ford)...\n");
    BellmanFord(&graph, 0); // Start from Node 0 (Branch A)

    return 0;
}
