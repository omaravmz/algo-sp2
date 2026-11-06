// Integrated Activity 2 - Advanced Algorithms
// Team Members:
//   - Diego Rodriguez Sánchez
//   - Diego Israel Najera
//   - Omar Ávila Meza A01743547

#include <iostream>
#include <fstream>
#include <vector>
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
    int weight;
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
// Parte 1: Cableado optimo con fibra optica
// (Arbol de expansion minima / MST - Minimum Spanning Tree)
// Regresa la lista de aristas (u, v) que forman el cableado.
// ---------------------------------------------------------
vector<Edge> minimumSpanningTree(const vector<vector<int>> &dist) {
    vector<Edge> result;
    // TODO: implementar
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
// Parte 3: Flujo maximo del nodo inicial al nodo final
// Regresa el valor del flujo maximo.
// ---------------------------------------------------------
int maxFlow(const vector<vector<int>> &capacity, int source, int sink) {
    int flow = 0;
    // TODO: implementar
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