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


//Compute and Counting Menu
void dfs_count_paths(string node, string dest, map <string, bool> visited, deque <string> current_path) {
    if (node == dest) {
        path_ans++;
        paths_count.push_back(current_path);
        return;
    }
    visited[node] = true;
    current_path.push_back(node);
    for (string neighbor : all_graph[node]) {
        if (!visited[neighbor]) {
            dfs_count_paths(neighbor, dest, visited, current_path);
        }
    }
    visited[node] = false;
}
void main_count_paths() {
    string src, dest;
    while (true) {
        cout << "Enter the initial source vertex:\n";
        getline(cin, src);
        if (all_graph.find(src) != all_graph.end()) {
            break;
        }
        else { cout << "Invalid source! Attempt aborted.\n\n"; }
    }
    while (true) {
        cout << "Enter the final destination vertex:\n";
        getline(cin, dest);
        if (all_graph.find(dest) != all_graph.end()) {
            break;
        }
        else { cout << "Invalid destination! Attempt aborted.\n\n"; }
    }
    cout << "\n\nCounting paths between initial source node " << src << " and final destination node " << dest << ":\n\n";
    map <string, bool> visited;
    path_ans = 0;
    dfs_count_paths(src, dest, visited, {});
    for (deque <string> path : paths_count) {
        for (string node : path) {
            cout << node << " -> ";
        }
        cout << dest << '\n';
    }
    cout << "\nTotal count: " << path_ans << " paths\n";
}
void main_count_vertices() {
    cout << "\n\nList of All Vertices:\n";
    for (string a : all_vertices) {
        cout << a << ' ';
    }
    cout << "\nCurrent total vertex count (order): " << all_vertices.size() << '\n';
}
void main_count_edges() {
    cout << "\n\nList of All Edges:\n";
    for (pair <string, string> x : all_edges) {
        cout << "  Nodes: { " << x.first << ' ' << x.second << " }";
        if (weighted) {
            cout << ", Weight: " << all_weights[make_pair(x.first, x.second)] << '\n';
        }
    }
    cout << "\nCurrent total edge count (size): " << all_edges.size() << '\n';
}
void main_count_neighbors() {
    string temp;
    while (true) {
        cout << "\nEnter a target node to find its neighbors:\n";
        getline(cin, temp);
        if (all_vertices.find(temp) != all_vertices.end()) {
            cout << "List of All Neighbors of Node " << temp << ":\n";
            for (string x : all_graph[temp]) {
                cout << x << ' '; 
            }
            cout << "\nCurrent total neighbor count (degree): " << all_graph[temp].size() << '\n';
            break;
        }
        else { cout << "\n\"" << temp << "\" is an invalid node! Attempt aborted.\n"; }
    }
}
void main_count_degree() {
    string temp;
    while (true) {
        cout << "\nEnter a target degree to find nodes with that given neighbor count:\n";
        getline(cin, temp);
        if (is_integer(temp) && (stoi(temp) >= 0)) { break; }
        else { cout << "\n\"" << temp << "\" is an invalid depth! Attempt aborted.\n"; }
    }
    int count_degree = 0;
    cout << "\nList of All Vertices of Degree " << temp << ":\n";
    for (string x : all_vertices) {
        if (all_graph[x].size() == stoi(temp)) {
            cout << x << ' ';
            count_degree++;
        }
    }
    cout << "\nCount of all vertices of degree " << temp << ": " << count_degree << '\n';
}
void main_count_root_depth() {
    string root, target;
    while (true) {
        cout << "\nEnter a node to serve as the root of the current graph:\n";
        getline(cin, root);
        if (all_vertices.find(root) != all_vertices.end()) { break; }
        else { cout << "\n\"" << root << "\" is an invalid node! Attempt aborted.\n"; }
    }
    while (true) {
        cout << "\nEnter a target depth to find nodes from the root:\n";
        getline(cin, target);
        if (is_integer(target) && (stoi(target) >= 0)) { break; }
        else { cout << "\n\"" << target << "\" is an invalid depth! Attempt aborted.\n"; }
    }
    int count_depth = 0;
    map <string, int> depth;
    for (string x : all_vertices) { depth[x] = -1; }
    queue <string> q;
    depth[root] = 0;
    q.push(root);
    while (!q.empty()) {
        string u = q.front();
        q.pop();
        for (string v : all_graph[u]) {
            if (depth[v] == -1) {
                depth[v] = depth[u] + 1;
                q.push(v);
            }
        }
    }
    cout << "\nList of All Vertices of Depth " << target << " from Root " << root << ":\n";
    for (string x : all_vertices) {
        if (depth[x] == stoi(target)) {
            count_depth++;
            cout << x << ' ';
        }
    }
    cout << "\nCount of all vertices of depth " << target << "from root " << root << ": " << count_depth << '\n';
}
