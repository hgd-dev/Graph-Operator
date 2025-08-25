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


//MST Menu:
bool detect_component_cycle_kruskal(string node, string parent, map <string, bool> vis)
{
    vis[node] = 1;
    for (string neighbor : mst_graph[node]) {
        if (parent == neighbor) { continue; }
        if (vis[neighbor] == 1) { return true; }
        if (detect_component_cycle_kruskal(neighbor, node, vis)) { return true; }
    }
    return false;
}
bool detect_graph_cycle_kruskal()
{
    map <string, bool> vis;
    for (string str : all_vertices) { vis[str] = false; }
    for (string str : all_vertices) {
        if (!vis[str]) {
            if (detect_component_cycle_kruskal(str, "-1", vis)) { return true; }
        }
    }
    return false;
}
void add_edge_kruskal(int index)
{
    mst_graph[all_edges[index].first].insert(all_edges[index].second);
    mst_graph[all_edges[index].second].insert(all_edges[index].first);
}
void remove_edge_kruskal(int index)
{
    mst_graph[all_edges[index].first].erase(all_edges[index].second);
    mst_graph[all_edges[index].second].erase(all_edges[index].first);
}
void perform_kruskal()
{
    int edge_index = 0;
    while (mst_vertices.size() != all_vertices.size()) {
        add_edge_kruskal(edge_index);
        if (detect_graph_cycle_kruskal()) {
            remove_edge_kruskal(edge_index);
        }
        else {
            mst_edges.push_back(all_edges[edge_index]);
            cout << "\nNew edge added between nodes { " << all_edges[edge_index].first << ' ' << all_edges[edge_index].second << " } of weight " << all_weights[make_pair(all_edges[edge_index].first, all_edges[edge_index].second)] << '\n';
            bool firstdisplay = false;
            if (mst_vertices.find(all_edges[edge_index].first) == mst_vertices.end()) {
                mst_vertices.insert(all_edges[edge_index].first);
                cout << "along with new node " << all_edges[edge_index].first;
                firstdisplay = true;
            }
            if (mst_vertices.find(all_edges[edge_index].second) == mst_vertices.end()) {
                mst_vertices.insert(all_edges[edge_index].second);
                if (firstdisplay) {
                    cout << " and new node " << all_edges[edge_index].second;
                }
                else {
                    cout << "along with new node " << all_edges[edge_index].second;
                }
            }
            cout << '\n';
        }
        ++edge_index;
    }
}
void main_kruskal()
{
    cout << "\nPerforming Kruskal Algorithm:\n";
    if (max_mode) { sort(all_edges.begin(), all_edges.end(), comp_max); }
    else { sort(all_edges.begin(), all_edges.end(), comp); }
    perform_kruskal();
    cout << "\n\nList of MST vertices:\n";
    for (string a : all_vertices) {
        cout << a << ' ';
    }
    cout << "\n\nList of MST Edges:\n";
    for (pair <string, string> b : mst_edges) {
        cout << "  Nodes: { " << b.first << ' ' << b.second << " }, Weight: " << all_weights[make_pair(b.first, b.second)] << '\n';
    }
    cout << "\n\n";
    int all_weight_kruskal = 0;
    for (pair <string, string> b : mst_edges) {
        all_weight_kruskal += all_weights[make_pair(b.first, b.second)];
    }
    cout << "\nAggregate ";
    if (max_mode) { cout << "Maximum"; }
    else { cout << "Minimum"; }
    cout << " Spanning Tree Weight: " << all_weight_kruskal << "\n\n\n";
}

void add_edge_prim(pair <string, string> new_edge)
{
    mst_vertices.insert(new_edge.first), mst_vertices.insert(new_edge.second);
    mst_edges.push_back(new_edge);
    cout << "\nNew edge added between nodes { " << new_edge.first << ' ' << new_edge.second << " } of weight " << all_weights[make_pair(new_edge.first, new_edge.second)] << '\n';
    if (all_vertices.find(new_edge.first) != all_vertices.end()) {
        all_vertices.erase(new_edge.first);
        cout << "along with new node " << new_edge.first << '\n';
    }
    else if (all_vertices.find(new_edge.second) != all_vertices.end()) {
        all_vertices.erase(new_edge.second);
        cout << "along with new node " << new_edge.second << '\n';
    }
}
void perform_prim()
{
    while (!all_vertices.empty()) {
        pair <string, string> to_add;
        deque <pair <string, string>> valid_edges;
        for (string node : mst_vertices) {
            for (string neighbor : all_graph[node]) {
                if (all_vertices.find(neighbor) != all_vertices.end()) {
                    to_add.first = node, to_add.second = neighbor;
                    valid_edges.push_back(to_add);
                }
            }
        }
        if (max_mode) { sort(valid_edges.begin(), valid_edges.end(), comp_max); }
        else { sort(valid_edges.begin(), valid_edges.end(), comp); }
        add_edge_prim(valid_edges[0]);
    }
}
void main_prim()
{
    cout << "\nPerforming Prim Algorithm:\n";
    string temp;
    while (true) {
        cout << "Enter any desired initial vertex, or \"rand\" for arbitrary choice:\n";
        getline(cin, temp);
        if (all_vertices.find(temp) != all_vertices.end()) {
            mst_vertices.insert(temp);
            all_vertices.erase(temp);
            cout << "\nStarting MST at vertex " << temp << ":\n";
            break;
        }
        else if (temp == "rand") {
            int random_index = (rand() % all_vertices.size());
            set <string>::iterator it = all_vertices.begin();
            advance(it, random_index);
            temp = *it;
            mst_vertices.insert(temp);
            all_vertices.erase(temp);
            cout << "\nStarting MST at vertex " << temp << ":\n";
            break;
        }
        else { cout << "Invalid start! Attempt aborted.\n\n"; }
    }
    perform_prim();
    cout << "\n\nList of MST Vertices:\n";
    for (string a : mst_vertices) {
        cout << a << ' ';
    }
    cout << "\n\nList of MST Edges:\n";
    for (pair <string, string> b : mst_edges) {
        cout << "  Nodes: { " << b.first << ' ' << b.second << " }, Weight: " << all_weights[make_pair(b.first, b.second)] << '\n';
    }
    cout << "\n\n";
    int all_weight_prim = 0;
    for (pair <string, string> b : mst_edges) {
        all_weight_prim += all_weights[make_pair(b.first, b.second)];
    }
    cout << "\nAggregate Minimum Spanning Tree Weight: " << all_weight_prim << "\n\n\n";
}
