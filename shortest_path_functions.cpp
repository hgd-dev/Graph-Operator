#include <iostream>
#include <map>
#include <deque>
#include <queue>
#include <set>
#include <algorithm>
#include <string>
#include <sstream>
#include <random>
#include <time.h>
#include <iterator>
using namespace std;

bool is_integer(string str) //Test if a given string is an integer
{
    if (str.empty()) return false;
    while (str.front() == ' ') {
        str = str.substr(1);
    }
    size_t start = 0;
    if (str[0] == '-' || str[0] == '+') {
        if (str.size() == 1) return false;
        start = 1;
    }
    for (size_t i = start; i < str.size(); ++i) {
        if (!std::isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

int e = 0;
bool weighted = false, max_mode = false;
map <string, set <string>> mst_graph, all_graph;
map <pair <string, string>, int> all_weights;
deque <pair <string, string>> all_edges, mst_edges;
set <string> all_vertices, mst_vertices;
int path_ans = 0;
deque <deque <string>> paths_count;


bool comp(pair <string, string> a, pair <string, string> b) {
    return (all_weights[make_pair(a.first, a.second)] < all_weights[make_pair(b.first, b.second)]);
}
bool comp_max(pair <string, string> a, pair <string, string> b) {
    return (all_weights[make_pair(a.first, a.second)] > all_weights[make_pair(b.first, b.second)]);
}
void clear_all() {
    e = 0, path_ans = 0;
    mst_graph.clear();
    all_edges.clear();
    mst_edges.clear();
    all_vertices.clear();
    mst_vertices.clear();
    paths_count.clear();
}

//Shortest Path Menu
void main_dijkstra() {
    if (!weighted) {
        for (pair <string, string> x : all_edges) {
            all_weights[x] = 1;
            all_weights[make_pair(x.second, x.first)] = 1;
        }
    }
    string start;
    while (true) {
        cout << "Enter a source node to find shortest paths to the rest of the graph:\n";
        getline(cin, start);
        if (all_vertices.find(start) != all_vertices.end()) { break; }
        else { cout << "Invalid source node! Attempt aborted"; }
    }
    const int INF = 1e9;
    map <string, int> dist;
    map <string, string> parent;
    for (string x : all_vertices) {
        dist[x] = INF;
        parent[x] = "";
    }
    dist[start] = 0;
    priority_queue<pair <int, string>, vector<pair <int, string>>, greater<pair <int, string>>> pq;
    pq.push({0, start});
    cout << "\nStarting Dijkstra from " << start << ".\n";
    while (!pq.empty()) {
        int d = pq.top().first;
        string u = pq.top().second;
        pq.pop();
        if (d > dist[u]) { continue; }
        cout << "Visiting node " << u << " with current distance " << d << "\n";
        for (string v : all_graph[u]) {
            int w = all_weights[make_pair(u, v)];
            if (dist[u] + w < dist[v]) {
                cout << "  Updating distance of " << v << " from " << (dist[v] == INF ? -1 : dist[v]) << " to " << dist[u] + w << " via " << u << '\n';
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
                parent[v] = u;
            }
        }
    }
    cout << "\nList of Shortest Path Weights from Source " << start << ":\n";
    for (string x : all_vertices) {
        if (dist[x] == INF) { cout << "Node " << x << " is not connected to source " << start << '\n'; }
        if (weighted) { cout << "Shortest Path { " << start << ' ' << x << " } has weight: " << dist[x] << '\n'; }
        else { cout << "Shortest Path { " << start << ' ' << x << " } has length: " << dist[x] << "\n  "; }
        deque <string> path;
        string cur = parent.at(cur);
        while (!cur.empty()) {
            path.push_back(cur);
            cur = parent.at(cur);
        }
        reverse(path.begin(), path.end());
        for (string y : path) { cout << y << " -> "; }
        cout << x << '\n';
    }
    if (!weighted) { all_weights.clear(); }
    cout << "Dijkstra finished.\n";
}
void main_bellman_ford() {
    if (!weighted) {
        for (pair <string, string> x : all_edges) {
            all_weights[x] = 1;
            all_weights[make_pair(x.second, x.first)] = 1;
        }
    }
    string start;
    while (true) {
        cout << "Enter a source node to find shortest paths to the rest of the graph:\n";
        getline(cin, start);
        if (all_vertices.find(start) != all_vertices.end()) { break; }
        else { cout << "Invalid source node! Attempt aborted"; }
    }
    const int INF = 1e9;
    map <string, int> dist;
    map <string, string> parent;
    for (string x : all_vertices) { dist[x] = INF; }
    dist[start] = 0;
    cout << "Starting Bellman-Ford from " << start << "\n";
    for (int i = 1; i < all_vertices.size(); ++i) {
        cout << "Iteration " << i << ":\n";
        for (pair <string, string> x : all_edges) {
            int w = all_weights[x];
            if (dist[x.first] != INF && dist[x.first] + w < dist[x.second]) {
                cout << "  Relaxing edge " << dist[x.first] << " -> " << dist[x.second] << " with "; 
                if (weighted)
                cout << " weight " << w << ".\n";
                cout << "Updating " << dist[x.second] << " from " << (dist[x.second] == INF? -1 : dist[x.second]) << " to " << dist[x.first] + all_weights[x] << ".\n";
                dist[x.second] = dist[x.first] + w;
                parent[x.second] = x.first;
            }
        }
    }
    bool neg_cy = false;
    for (pair <string, string> x : all_edges) {
        if (dist[x.first] != INF && dist[x.first] + all_weights[x] < dist[x.second]) {
            cout << "Negative weight cycle detected!\n";
            neg_cy = true;
            break;
        }
    }
    if (neg_cy) { cout << "\nGraph contains a negative cycle!\nThe Shortest Path can be reduced indefinitely.\n"; }
    else {
        cout << "\nList of Shortest Path Weights from Source " << start << ":\n";
        for (string x : all_vertices) {
            if (dist[x] == INF) { cout << "Node " << x << " is not connected to source " << start << '\n'; }
            if (weighted) { cout << "Shortest Path { " << start << ' ' << x << " } has weight: " << dist[x] << '\n'; }
            else { cout << "Shortest Path { " << start << ' ' << x << " } has length: " << dist[x] << '\n'; }
            deque <string> path;
            string cur = parent.at(cur);
            while (!cur.empty()) {
                path.push_back(cur);
                cur = parent.at(cur);
            }
            reverse(path.begin(), path.end());
            for (string y : path) { cout << y << " -> "; }
            cout << x << '\n';
        }
    }
    cout << "\nBellman-Ford finished.\n";
    if (!weighted) { all_weights.clear(); }
}
void main_floyd_warshall() {
    if (!weighted) {
        for (pair <string, string> x : all_edges) {
            all_weights[x] = 1;
            all_weights[make_pair(x.second, x.first)] = 1;
        }
    }
    string start;
    while (true) {
        cout << "Enter a source node to find shortest paths to the rest of the graph:\n";
        getline(cin, start);
        if (all_vertices.find(start) != all_vertices.end()) { break; }
        else { cout << "Invalid source node! Attempt aborted"; }
    }
    const int INF = 1e9;
    map <pair <string, string>, int> dist;
    map <pair <string, string>, string> next;

    for (string u : all_vertices) {
        for (string v : all_vertices) {
            dist[{u, v}] = (u == v ? 0 : INF);
            next[{u, v}] = (u == v ? u : "");
        }
    }
    for (pair <string, string> x : all_edges) {
        if (all_weights[x] < dist[x]) {
            dist[x] = all_weights[x];
            next[x] = x.second;
        }
    }
    cout << "Starting Floyd-Warshall\n";
    for (string k : all_vertices) {
        cout << "Considering intermediate node " << k << "\n";
        for (string i : all_vertices) {
            for (string j : all_vertices) {
                if (dist[make_pair(i, k)] < INF && dist[make_pair(k, j)] < INF && dist[make_pair(i, k)] + dist[make_pair(k, j)] < dist[make_pair(i, j)]) {
                    cout << "  Updating dist[" << i << "][" << j << "] from " << (dist[make_pair(i, j)] == INF ? -1 : dist[make_pair(i, j)]) << " -> " << dist[make_pair(i, k)] + dist[make_pair(k, j)] << " via " << k << "\n";
                    dist[make_pair(i, j)] = dist[make_pair(i, k)] + dist[make_pair(k, j)];
                    next[make_pair(i, j)] = next[make_pair(i, k)];
                }
            }
        }
    }
    cout << "\nList of Shortest Path Weights from Source " << start << ":\n";
    for (string x : all_vertices) {
        if (dist[make_pair(start, x)] == INF) { cout << "Node " << x << " is not connected to source " << start << '\n'; }
        if (weighted) { cout << "Shortest Path { " << start << ' ' << x << " } has weight: " << dist[make_pair(start, x)] << '\n'; }
        else { cout << "Shortest Path { " << start << ' ' << x << " } has length: " << dist[make_pair(start, x)] << "\n  "; }
        if (!next.at(make_pair(start, x)).empty()) {
            cout << start;
            string cur = start;
            while (cur != x) {
                cur = next.at(make_pair(cur, x));
                cout << " -> " << cur;
            }
            cout << '\n';
        }
    }
    if (!weighted) { all_weights.clear(); }
    cout << "Floyd-Warshall finished.\n";
}
