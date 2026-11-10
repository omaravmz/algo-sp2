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
#include <algorithm>

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

vector<vector<long long>> readMatrix(ifstream &in, int n) {
    vector<vector<long long>> m(n, vector<long long>(n));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            in >> m[i][j];
        }
    }
    return m;
}

vector<Point> readPoints(ifstream &in, int n) {
    vector<Point> pts(n);
    char c; // used to read '(' ',' ')'
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

// Converts Prim's parent array into a list of edges (u, v)
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

// Union-Find: returns the root of the group a city belongs to
// (with path compression).
int findRoot(vector<int> &parent, int city) {
	int root = city;
	while (parent[root] != root) {
		root = parent[root];
	}
	while (parent[city] != root) {
		int next = parent[city];
		parent[city] = root;
		city = next;
	}
	return root;
}

// ---------------------------------------------------------
// Part 2: Shortest route that visits each neighborhood once
// and returns to the origin (TSP - Traveling Salesman Problem)
// Returns the route as a list of indices, starting and
// ending at 0 (A).
// ---------------------------------------------------------

/*
    Cheapest-Link heuristic: edges are sorted by weight and added from
    cheapest to most expensive, as long as neither neighborhood already
    has 2 neighbors and the edge does not close a cycle too early
    (checked with Union-Find). The resulting path is the delivery route.

    Time complexity: O(n^2 log n), from sorting the ~n^2/2 edges.
    Space complexity: O(n^2) for the edge list.
*/

vector<int> travelingSalesman(const vector<vector<long long>> &dist) {
	vector<int> tour;
	int cityCount = dist.size();
	if (cityCount == 0) {
		return tour;
	}

	vector<pair<long long, pair<int, int>>> edges;
	for (int first = 0; first < cityCount; first++) {
		for (int second = first + 1; second < cityCount; second++) {
			edges.push_back({dist[first][second], {first, second}});
		}
	}
	sort(edges.begin(), edges.end());

	vector<int> parent(cityCount, 0);
	for (int i = 0; i < cityCount; i++) {
		parent[i] = i;
	}
	vector<vector<int>> neighbors(cityCount);
	int edgesAdded = 0;
	size_t edgeIndex = 0;
	while (edgesAdded < cityCount - 1 && edgeIndex < edges.size()) {
		int first = edges[edgeIndex].second.first;
		int second = edges[edgeIndex].second.second;
		if (neighbors[first].size() < 2 && neighbors[second].size() < 2) {
			int firstRoot = findRoot(parent, first);
			int secondRoot = findRoot(parent, second);
			if (firstRoot != secondRoot) {
				parent[firstRoot] = secondRoot;
				neighbors[first].push_back(second);
				neighbors[second].push_back(first);
				edgesAdded++;
			}
		}
		edgeIndex++;
	}

	int start = 0;
	while (neighbors[start].size() == 2) {
		start++;
	}
	int previous = -1;
	int current = start;
	while (current != -1) {
		tour.push_back(current);
		int next = -1;
		for (size_t i = 0; i < neighbors[current].size(); i++) {
			if (neighbors[current][i] != previous) {
				next = neighbors[current][i];
			}
		}
		previous = current;
		current = next;
	}

	// Rotate so the route starts at A, then return to A to close the cycle
	int zeroPosition = 0;
	while (tour[zeroPosition] != 0) {
		zeroPosition++;
	}
	rotate(tour.begin(), tour.begin() + zeroPosition, tour.end());
	tour.push_back(0);

	return tour;
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
// Part 4: Closest region to each exchange
// Returns a list of polygons; polygon i contains the area
// of the plane closest to exchange i.
// ---------------------------------------------------------
struct HalfPlane{
    double a, b, c;
};

HalfPlane makeHalfPlane(const Point &p1, const Point &p2) {
    HalfPlane hp;
    hp.a = p2.x - p1.x;
    hp.b = p2.y - p1.y;
    double mx = (p1.x + p2.x) / 2.0;
    double my = (p1.y + p2.y) / 2.0;
    hp.c = -(hp.a * mx + hp.b * my);
    return hp;
}

double evaluateHalfPlane(const HalfPlane &hp, const Point &p) {
    return hp.a * p.x + hp.b * p.y + hp.c;
}

Point intersection(const Point &p1, const Point &p2, double f1, double f2) {
    double t = f1 / (f1 - f2);
    Point p;
    p.x = p1.x + t * (p2.x - p1.x);
    p.y = p1.y + t * (p2.y - p1.y);
    return p;
}

vector<Point> cutPolygon(const vector<Point> &polygon, const HalfPlane &hp) {
    vector<Point> result;
    int n = polygon.size();
    for(int i = 0; i < n; i++){
        const Point &p1 = polygon[i];
        const Point &p2 = polygon[(i + 1) % n];
        double f1 = evaluateHalfPlane(hp, p1);
        double f2 = evaluateHalfPlane(hp, p2);

        if(f1 <= 0) result.push_back(p1);
        if(f1 * f2 < 0) result.push_back(intersection(p1, p2, f1, f2));
    }
    return result;
}

vector<vector<Point>> closestRegions(const vector<Point> &centrals) {
    vector<vector<Point>> polygons;
    int n = centrals.size();
    if(n == 0) return polygons;

    double minX = centrals[0].x, maxX = centrals[0].x;
    double minY = centrals[0].y, maxY = centrals[0].y;
    for(int i = 1; i < n; i++) {
        minX = min(minX, centrals[i].x);
        maxX = max(maxX, centrals[i].x);
        minY = min(minY, centrals[i].y);
        maxY = max(maxY, centrals[i].y);
    }
    double margin = max(maxX - minX, maxY - minY);
    if (margin == 0) margin = 1;
    
    vector<Point> boundingBox = {
        {minX - margin, minY - margin},
        {maxX + margin, minY - margin},
        {maxX + margin, maxY + margin},
        {minX - margin, maxY + margin}
    };

    for(int i = 0; i < n; i++) {
        vector<Point> polygon = boundingBox;
        for(int j = 0; j < n; j++) {
            if(i == j) continue;
            HalfPlane hp = makeHalfPlane(centrals[i], centrals[j]);
            polygon = cutPolygon(polygon, hp);
        }
        polygons.push_back(polygon);
    }
    
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

void printMaxFlow(long long flow) {
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

    vector<vector<long long>> dist = readMatrix(in, n);
    vector<vector<long long>> capacity = readMatrix(in, n);
    vector<Point> centrals = readPoints(in, n);

    in.close();

    // 1. Cableado
    vector<Edge> wiring = minimumSpanningTree(dist);
    printWiring(wiring);

    // 2. Ruta del repartidor
    vector<int> route = travelingSalesman(dist);
    printRoute(route);

    // 3. Flujo maximo (nodo inicial = 0, nodo final = n - 1)
    long long flow = maxFlow(capacity, 0, n - 1);
    printMaxFlow(flow);

    // 4. Poligonos de cercania a centrales
    vector<vector<Point>> polygons = closestRegions(centrals);
    printPolygons(polygons);

    return 0;
}