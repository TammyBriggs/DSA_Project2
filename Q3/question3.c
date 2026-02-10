#include <stdio.h>
#include <stdlib.h>

// Number of Smart City Hubs (A, B, C, D, E, F, G)
#define V 7 

// Edge Structure
typedef struct {
    int src, dest, weight;
} Edge;

// Subset structure for Union-Find (Cycle Detection)
typedef struct {
    int parent;
    int rank;
} Subset;

// Find function (path compression)
int find(Subset subsets[], int i) {
    if (subsets[i].parent != i)
        subsets[i].parent = find(subsets, subsets[i].parent);
    return subsets[i].parent;
}

// Union function (by rank)
void Union(Subset subsets[], int x, int y) {
    int xroot = find(subsets, x);
    int yroot = find(subsets, y);

    if (subsets[xroot].rank < subsets[yroot].rank)
        subsets[xroot].parent = yroot;
    else if (subsets[xroot].rank > subsets[yroot].rank)
        subsets[yroot].parent = xroot;
    else {
        subsets[yroot].parent = xroot;
        subsets[xroot].rank++;
    }
}

// Comparator for sorting edges
int compareEdges(const void* a, const void* b) {
    Edge* a1 = (Edge*)a;
    Edge* b1 = (Edge*)b;
    return a1->weight - b1->weight;
}

// Convert node index to Character (0->A, 1->B ...)
char getNodeName(int index) {
    return 'A' + index;
}

void KruskalMST(int graph[V][V]) {
    // 1. Gather all valid edges from the matrix
    Edge edgeList[V * V]; 
    int e_count = 0;

    // Iterate upper triangle to avoid duplicates
    for (int i = 0; i < V; i++) {
        for (int j = i + 1; j < V; j++) {
            if (graph[i][j] != 0) { // If there is a connection
                edgeList[e_count].src = i;
                edgeList[e_count].dest = j;
                edgeList[e_count].weight = graph[i][j];
                e_count++;
            }
        }
    }

    // 2. Sort edges by weight (Smallest -> Largest)
    qsort(edgeList, e_count, sizeof(Edge), compareEdges);

    // 3. Initialize Union-Find
    Subset* subsets = (Subset*)malloc(V * sizeof(Subset));
    for (int v = 0; v < V; ++v) {
        subsets[v].parent = v;
        subsets[v].rank = 0;
    }

    Edge results[V]; // MST will have V-1 edges
    int resultIndex = 0;
    int i = 0; 
    int totalCost = 0;

    printf("\n--- Kruskal's Algorithm Steps ---\n");

    // 4. Iterate and Select Edges
    while (resultIndex < V - 1 && i < e_count) {
        Edge next_edge = edgeList[i++];

        int x = find(subsets, next_edge.src);
        int y = find(subsets, next_edge.dest);

        // If no cycle is formed, include this edge
        if (x != y) {
            results[resultIndex++] = next_edge;
            totalCost += next_edge.weight;
            Union(subsets, x, y);
            printf("Selected: %c -- %c (Cost: %d)\n", 
                   getNodeName(next_edge.src), getNodeName(next_edge.dest), next_edge.weight);
        } else {
            printf("Skipped:  %c -- %c (Cost: %d) -> Forms Cycle\n", 
                   getNodeName(next_edge.src), getNodeName(next_edge.dest), next_edge.weight);
        }
    }

    // 5. Final Output
    printf("\n-------------------------------------------------\n");
    printf("FINAL MINIMUM SPANNING TREE (Fiber-Optic Network):\n");
    printf("-------------------------------------------------\n");
    for (int j = 0; j < resultIndex; j++) {
        printf("Link: Hub %c <--> Hub %c  [Cost: $%dM]\n", 
               getNodeName(results[j].src), getNodeName(results[j].dest), results[j].weight);
    }
    printf("-------------------------------------------------\n");
    printf("TOTAL INSTALLATION COST: $%d Million\n", totalCost);
    printf("-------------------------------------------------\n");

    free(subsets);
}

int main() {
    // Mapping: A=0, B=1, C=2, D=3, E=4, F=5, G=6
    // 0 indicates NO direct connection
    int graph[V][V] = {
        // A,  B,  C,  D,  E,  F,  G
        {  0,  6,  0,  0,  0,  0,  0 }, // A: Connects to B(6). (A-D assumed 0/None)
        {  6,  0, 11,  5,  0,  0,  0 }, // B: Connects to A(6), C(11), D(5)
        {  0, 11,  0, 17,  0,  0, 25 }, // C: Connects to B(11), D(17), G(25)
        {  0,  5, 17,  0, 22,  0,  0 }, // D: Connects to B(5), C(17), E(22). (D-F assumed 0)
        {  0,  0,  0, 22,  0, 10,  0 }, // E: Connects to D(22), F(10)
        {  0,  0,  0,  0, 10,  0, 22 }, // F: Connects to E(10), G(22)
        {  0,  0, 25,  0,  0, 22,  0 }  // G: Connects to C(25), F(22)
    };

    printf("Calculating Fiber-Optic Network for %d Hubs...\n", V);
    KruskalMST(graph);

    return 0;
}
