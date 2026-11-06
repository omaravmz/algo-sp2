// Integrated Activity 2 - Advanced Algorithms
// Team Members:
//   - Diego Rodriguez Sánchez
//   - Diego Israel Najera
//   - Omar Ávila Meza A01743547

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <climits>
#include <cmath>

using namespace std;

// Structures

struct Point { 
    double x;
    double y;
};

struct Edge {
    int u;
    int v;
    long long weight;
};

// Utilities

char toLetter(int i) {
    return 'A' + i;
}

vector<vector<int>> readMatrix(ifstream &in, int n) {
    vector<vector<int>> m(n, vector<int>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            in >> m[i][j];
        }
    }
    return m;
}

vector<Point> readPoints(ifstream &in, int n) {
    vector<Point> pts(n);
    char c; // para leer '(' ',' ')'
    for (int i = 0; i < n; i++) {
        in >> c >> pts[i].x >> c >> pts[i].y >> c;
    }
    return pts;
}

// ALGORITHMS

// ---------------------------------------------------------
// Part 1: Optimal fiber optic wiring
// (MST - Minimum Spanning Tree)
// Returns the list of edges (u, v) that form the wiring.
// ---------------------------------------------------------

/*
    Prim's algorithm (adjacency matrix version).
    Returns parent[v] = the node v is connected to in the tree
    (-1 for the root or for nodes that could not be connected).

    A distance of 0 outside the diagonal is treated as
    "no wiring is possible between those neighborhoods".

    Time complexity: O(n^2). There are n iterations, and in each one
    the n nodes are scanned twice (once to pick the closest node
    outside the tree and once to update the keys).
    Space complexity: O(n) for the parent, key and inTree arrays.
*/

vector<int> primMST(const vector<vector<long long>> &D) {
    int n = D.size();
    vector<int> parent(n, -1);
    vector<long long> key(n, LLONG_MAX);
    vector<bool> inTree(n, false);
    key[0] = 0;
 
    for (int it = 0; it < n; it++) {
        int v = -1;
        for (int u = 0; u < n; u++)
            if (!inTree[u] && (v == -1 || key[u] < key[v])) v = u;
        inTree[v] = true;
 
        for (int u = 0; u < n; u++) {
            if (!inTree[u] && D[v][u] > 0 && D[v][u] < key[u]) {
                key[u] = D[v][u];
                parent[u] = v;
            }
        }
    }
 
    return parent;
}
 
// Convierte el arreglo parent de Prim a una lista de aristas (u, v)
vector<Edge> minimumSpanningTree(const vector<vector<long long>> &dist) {
    vector<Edge> result;
    vector<int> parent = primMST(dist);
    for (int v = 0; v < (int)dist.size(); v++) {
        if (parent[v] != -1) {
            result.push_back({parent[v], v, dist[parent[v]][v]});
        }
    }
    return result;
}

// ---------------------------------------------------------
// Parte 2: Ruta mas corta que visita cada colonia una vez
// y regresa al origen (TSP - Traveling Salesman Problem)
// Regresa la ruta como lista de indices, empezando y
// terminando en 0 (A).
// ---------------------------------------------------------
vector<int> travelingSalesman(const vector<vector<int>> &dist) {
    vector<int> route;
    // TODO: implementar
    return route;
}

// ---------------------------------------------------------
// Part 3: Maximum information flow from the initial node
// to the final node (Ford-Fulkerson with BFS / Edmonds-Karp)
// ---------------------------------------------------------

/*
    BFS over the residual graph, only through edges with residual > 0.
    Saves in parent[v] the node from which v was reached, so the
    augmenting path can be rebuilt from the sink back to the source.

    Time complexity: O(V^2). Every node enters the queue at most once,
    and for each one the full row of the matrix (V entries) is scanned.
    Space complexity: O(V) for the queue and the parent vector.
*/

bool bfs(const vector<vector<long long>> &residual, int source, int sink, vector<int> &parent) {
    int n = residual.size();
    parent.assign(n, -1);   // -1 means not visited
    queue<int> q;
    q.push(source);
    parent[source] = source;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v = 0; v < n; v++) {
            if (residual[u][v] > 0 && parent[v] == -1) {
                parent[v] = u;
                if (v == sink) return true;
                q.push(v);
            }
        }
    }

    return false;
}

/*
    Ford-Fulkerson using BFS to find augmenting paths (Edmonds-Karp).
    The capacity matrix is copied into a residual matrix. Pushing flow
    along u -> v decreases residual[u][v] and increases residual[v][u],
    so the reverse edges are handled by the matrix itself.

    Time complexity: O(V * E^2). With BFS there are at most O(V * E)
    augmenting paths, and each one costs O(V^2) for the BFS on the
    matrix plus O(V) to walk the path back. Since the input is an
    N x N matrix, E can be up to V^2, which makes it O(V^5) in the
    worst case, although in practice far fewer paths are needed.
    Space complexity: O(V^2) for the residual matrix.
*/

long long maxFlow(const vector<vector<long long>> &capacity, int source, int sink) {
    if (source == sink) return 0;

    vector<vector<long long>> residual = capacity;
    vector<int> parent;
    long long flow = 0;

    while (bfs(residual, source, sink, parent)) {
        long long bottleneck = LLONG_MAX;

        // find the smallest residual capacity on the path
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            bottleneck = min(bottleneck, residual[u][v]);
        }

        // push the flow and update the reverse direction
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residual[u][v] -= bottleneck;
            residual[v][u] += bottleneck;
        }

        flow += bottleneck;
    }

    return flow;
}

// ---------------------------------------------------------
// Parte 4: Region mas cercana a cada central
// Regresa una lista de poligonos; el poligono i contiene
// la zona del plano mas cercana a la central i.
// ---------------------------------------------------------
vector<vector<Point>> closestRegions(const vector<Point> &centrals) {
    vector<vector<Point>> polygons;
    // TODO: implementar
    return polygons;
}

// Printing Functions

void printWiring(const vector<Edge> &edges) {
    cout << "1. Cableado con fibra optica:" << endl;
    for (const Edge &e : edges) {
        cout << "(" << toLetter(e.u) << "," << toLetter(e.v) << ")" << endl;
    }
    cout << endl;
}

void printRoute(const vector<int> &route) {
    cout << "2. Ruta del repartidor:" << endl;
    for (int i = 0; i < (int)route.size(); i++) {
        cout << toLetter(route[i]);
        if (i < (int)route.size() - 1) cout << " -> ";
    }
    cout << endl << endl;
}

void printMaxFlow(int flow) {
    cout << "3. Flujo maximo de informacion:" << endl;
    cout << flow << endl << endl;
}

void printPolygons(const vector<vector<Point>> &polygons) {
    cout << "4. Poligonos:" << endl;
    for (int i = 0; i < (int)polygons.size(); i++) {
        cout << "Central " << i + 1 << ": [";
        for (int j = 0; j < (int)polygons[i].size(); j++) {
            cout << "(" << polygons[i][j].x << "," << polygons[i][j].y << ")";
            if (j < (int)polygons[i].size() - 1) cout << ", ";
        }
        cout << "]" << endl;
    }
}

// Main

int main(int argc, char *argv[]) {
    string fileName = "input.txt";
    if (argc > 1) fileName = argv[1];

    ifstream in(fileName);
    if (!in.is_open()) {
        cout << "No se pudo abrir el archivo: " << fileName << endl;
        return 1;
    }

    int n;
    in >> n;

    vector<vector<int>> dist = readMatrix(in, n);
    vector<vector<int>> capacity = readMatrix(in, n);
    vector<Point> centrals = readPoints(in, n);

    in.close();

    // 1. Cableado
    vector<Edge> wiring = minimumSpanningTree(dist);
    printWiring(wiring);

    // 2. Ruta del repartidor
    vector<int> route = travelingSalesman(dist);
    printRoute(route);

    // 3. Flujo maximo (nodo inicial = 0, nodo final = n - 1)
    int flow = maxFlow(capacity, 0, n - 1);
    printMaxFlow(flow);

    // 4. Poligonos de cercania a centrales
    vector<vector<Point>> polygons = closestRegions(centrals);
    printPolygons(polygons);

    return 0;
}