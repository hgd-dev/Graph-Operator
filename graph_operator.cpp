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

//All Variables
int e = 0;
bool weighted = false, max_mode = false;
map <string, set <string>> mst_graph, all_graph;
map <pair <string, string>, int> all_weights;
deque <pair <string, string>> all_edges, mst_edges;
set <string> all_vertices, mst_vertices;
int path_ans = 0;
deque <deque <string>> paths_count;

bool is_integer(string str) {
    if (str.empty()) return false;
    while (str.front() == ' ') { str = str.substr(1); }
    size_t start = 0;
    if (str[0] == '-' || str[0] == '+') {
        if (str.size() == 1) { return false; }
        start = 1;
    }
    for (size_t i = start; i < str.size(); ++i) { if (!isdigit(str[i])) { return false; } }
    return true;
}
bool comp(pair <string, string> a, pair <string, string> b) { return (all_weights[make_pair(a.first, a.second)] < all_weights[make_pair(b.first, b.second)]); }
bool comp_max(pair <string, string> a, pair <string, string> b) { return (all_weights[make_pair(a.first, a.second)] > all_weights[make_pair(b.first, b.second)]); }
void clear_all() {
    e = 0, path_ans = 0;
    all_graph.clear(), mst_graph.clear();
    all_edges.clear(), mst_edges.clear();
    all_vertices.clear(), mst_vertices.clear();
    paths_count.clear();
}

//Graph Display:
void graph_display() {
    cout << "\nDisplay of the current graph:\n\n";
    cout << "List of all vertices:\n  ";
    for (string x : all_vertices) { cout << x << ' '; }
    cout << "\n\nList of all edges ";
    if (weighted) { cout << "with weights:"; }
    cout << '\n';
    for (pair <string, string> x : all_edges) {
        cout << "  Edge { " << x.first << ' ' << x.second << " } ";
        if (weighted) { cout << "of weight " << all_weights[x]; }
        cout << '\n';
    }
}

//Input Menu:
void graph_input() {
    clear_all();
    string temp;
    while (true) {
        cout << "Enter 1 if the graph is weighted and 0 if not:\n";
        getline(cin, temp);
        if (temp == "1") { weighted = true; break; }
        else if (temp == "0") { weighted = false; break; }
        else { cout << "Invalid graph! Attempt aborted.\n\n"; }
    }
    while (true) {
        cout << "Enter your graph's number of edges (ε(G)):\n";
        getline(cin, temp);
        if (is_integer(temp) && stoi(temp) > 0) { e = stoi(temp); break; }
        else { cout << "Invalid size! Attempt aborted.\n\n"; }
    }
    all_edges.resize(e);
    for (int i = 0; i < e; ++i) {
        while (true) {
            cout << "Input edge " << i + 1 << " in the form: node1 node2";
            if (weighted) { cout << " weight"; }
            cout << '\n';
            bool inputValid = true;
            getline(cin, temp);
            temp = temp + ' ';
            istringstream iss(temp);
            string token = "";
            while (token.empty()) {
                if (getline(iss, token, ' ')) {}
                else { inputValid = false; break; }
            }
            if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
            else { all_edges[i].first = token; }
            token = "";
            while (token.empty()) {
                if (getline(iss, token, ' ')) {}
                else { inputValid = false; break; }
            }
            if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
            else { all_edges[i].second = token; }
            if (weighted) {
                token = "";
                while (token.empty()) {
                    if (getline(iss, token, ' ')) {}
                    else { inputValid = false; break; }
                }
                if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
                else {
                    if (is_integer(token)) {
                        all_weights[make_pair(all_edges[i].first, all_edges[i].second)] = (stoi(token));
                        all_weights[make_pair(all_edges[i].second, all_edges[i].first)] = (stoi(token));
                    }
                    else { inputValid = false; }
                }
            }


            if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
            else { break; }
        }
    }
    for (pair <string, string> x : all_edges) {
        all_vertices.insert(x.first), all_vertices.insert(x.second);
        all_graph[x.first].insert(x.second), all_graph[x.second].insert(x.first);
    }
}
void rename_node_input() {
    string temp;
    while (true) {
        cout << "Enter the node you would like to rename:\n";
        getline(cin, temp);
        if (all_vertices.find(temp) != all_vertices.end()) {
            cout << "Enter the name you would like to reassign to this node:\n";
            string new_name;
            getline(cin, new_name);
            all_vertices.erase(temp), all_vertices.insert(new_name);
            if (weighted && !mst_vertices.empty()) { mst_vertices.erase(temp), mst_vertices.insert(new_name); }
            all_graph[new_name] = all_graph[temp];
            all_graph.erase(temp);
            if (weighted && !mst_vertices.empty()) {
                mst_graph[new_name] = mst_graph[temp];
                mst_graph.erase(temp);
            }
            for (int i = 0; i < all_edges.size(); ++i) {
                if (all_edges[i].first == temp) {
                    all_edges[i].first = new_name;
                    all_weights[make_pair(new_name, all_edges[i].second)] = all_weights[make_pair(temp, all_edges[i].second)];
                    all_weights[make_pair(all_edges[i].second, new_name)] = all_weights[make_pair(all_edges[i].second, temp)];
                    all_weights.erase(make_pair(temp, all_edges[i].second)), all_weights.erase(make_pair(all_edges[i].second, temp));
                }
                if (all_edges[i].second == temp) {
                    all_edges[i].second = new_name;
                    all_weights[make_pair(new_name, all_edges[i].first)] = all_weights[make_pair(temp, all_edges[i].first)];
                    all_weights[make_pair(all_edges[i].first, new_name)] = all_weights[make_pair(all_edges[i].first, temp)];
                    all_weights.erase(make_pair(temp, all_edges[i].first)), all_weights.erase(make_pair(all_edges[i].first, temp));
                }
            }
            for (int i = 0; i < mst_edges.size(); ++i) {
                if (mst_edges[i].first == temp) {
                    mst_edges[i].first = new_name;
                    all_weights[make_pair(new_name, mst_edges[i].second)] = all_weights[make_pair(temp, mst_edges[i].second)];
                    all_weights[make_pair(mst_edges[i].second, new_name)] = all_weights[make_pair(mst_edges[i].second, temp)];
                    all_weights.erase(make_pair(temp, mst_edges[i].second)), all_weights.erase(make_pair(mst_edges[i].second, temp));
                }
                if (mst_edges[i].second == temp) {
                    mst_edges[i].second = new_name;
                    all_weights[make_pair(new_name, mst_edges[i].first)] = all_weights[make_pair(temp, mst_edges[i].first)];
                    all_weights[make_pair(mst_edges[i].first, new_name)] = all_weights[make_pair(mst_edges[i].first, temp)];
                    all_weights.erase(make_pair(temp, mst_edges[i].first)), all_weights.erase(make_pair(mst_edges[i].first, temp));
                }
            }
            break;
        }
        else { cout << "Invalid node! Attempt aborted.\n\n"; }
    }
}
void add_edge_input() {
    string temp;
    pair <string, string> pending_add;
    while (true) {
        cout << "Input your desired new edge in the form: node1 node2";
        if (weighted) { cout << " weight"; }
        cout << '\n';
        bool inputValid = true;
        getline(cin, temp);
        temp = temp + ' ';
        istringstream iss(temp);
        string token = "";
        while (token.empty()) {
            if (getline(iss, token, ' ')) {}
            else { inputValid = false; break; }
        }
        if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
        else { pending_add.first = token; }
        token = "";
        while (token.empty()) {
            if (getline(iss, token, ' ')) {}
            else { inputValid = false; break; }
        }
        if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
        else { pending_add.second = token; }
        if (weighted) {
            token = "";
            while (token.empty()) {
                if (getline(iss, token, ' ')) {}
                else { inputValid = false;  break; }
            }
            if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
            else {
                if (is_integer(token)) {
                    all_weights[make_pair(pending_add.first, pending_add.second)] = (stoi(token));
                    all_weights[make_pair(pending_add.second, pending_add.first)] = (stoi(token));
                }
                else { inputValid = false; }
            }
        }

        if (!inputValid) {  cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
        else { break; }
    }
    cout << "\nNew edge added between nodes { " << pending_add.first << ' ' << pending_add.second << " } ";
    if (weighted == true) { cout << "of weight " << all_weights[make_pair(pending_add.first, pending_add.second)] << '\n'; }
    bool firstdisplay = false;
    if (all_vertices.find(pending_add.first) == all_vertices.end()) {
        all_vertices.insert(pending_add.first);
        cout << "along with new node " << pending_add.first;
        firstdisplay = true;
    }
    if (all_vertices.find(pending_add.second) == all_vertices.end()) {
        all_vertices.insert(pending_add.second);
        if (firstdisplay) { cout << " and new node " << pending_add.second; }
        else { cout << "along with new node " << pending_add.second; }
    }
    cout << '\n';
    all_graph[pending_add.first].insert(pending_add.second), all_graph[pending_add.second].insert(pending_add.first);
    all_edges.push_back(pending_add);
}
void remove_edge_input() {
    string temp;
    pair <string, string> pending_remove;
    while (true) {
        cout << "Input the desired edge you want to remove in the form: node1 node2\n";
        bool inputValid = true;
        getline(cin, temp);
        temp = temp + ' ';
        istringstream iss(temp);
        string token = "";
        while (token.empty()) {
            if (getline(iss, token, ' ')) {}
            else { inputValid = false; break; }
        }
        if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
        else {
            if (all_vertices.find(token) == all_vertices.end()) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
            pending_remove.first = token;
        }
        token = "";
        while (token.empty()) {
            if (getline(iss, token, ' ')) {}
            else { inputValid = false; break; }
        }
        if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
        else {
            if (all_vertices.find(token) == all_vertices.end()) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
            pending_remove.second = token;
        }
        bool is_edge = false;
        for (pair <string, string> x : all_edges) { if (x == pending_remove || make_pair(x.second, x.first) == pending_remove) { is_edge = true; break; } }
        if (!is_edge) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
        else { break; }
    }
    cout << "\nRemoved edge between nodes { " << pending_remove.first << ' ' << pending_remove.second << " } ";
    if (weighted == true) {
        cout << "of weight " << all_weights[make_pair(pending_remove.first, pending_remove.second)] << '\n';
        all_weights.erase(make_pair(pending_remove.first, pending_remove.second));
        all_weights.erase(make_pair(pending_remove.second, pending_remove.first));
    }
    cout << '\n';
    all_graph[pending_remove.first].erase(pending_remove.second), all_graph[pending_remove.second].erase(pending_remove.first);
    all_edges.erase(remove(all_edges.begin(), all_edges.end(), make_pair(pending_remove.first, pending_remove.second)), all_edges.end());
    all_edges.erase(remove(all_edges.begin(), all_edges.end(), make_pair(pending_remove.second, pending_remove.first)), all_edges.end());
}
void reweight_edge_input() {
    string temp;
    pair <string, string> pending_change;
    while (true) {
        cout << "Input the target edge and the desired weight in the form: node1 node2 weight\n";
        bool inputValid = true;
        getline(cin, temp);
        temp = temp + ' ';
        istringstream iss(temp);
        string token = "";
        while (token.empty()) {
            if (getline(iss, token, ' ')) {}
            else { inputValid = false; break; }
        }
        if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
        else {
            if (all_vertices.find(token) == all_vertices.end()) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
            pending_change.first = token;
        }
        token = "";
        while (token.empty()) {
            if (getline(iss, token, ' ')) {}
            else { inputValid = false; break; }
        }
        if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
        else {
            if (all_vertices.find(token) == all_vertices.end()) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
            pending_change.second = token;
        }
        token = "";
        while (token.empty()) {
            if (getline(iss, token, ' ')) {}
            else { inputValid = false; break; }
        }
        if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
        else {
            if (is_integer(token)) {
                all_weights[make_pair(pending_change.first, pending_change.second)] = (stoi(token));
                all_weights[make_pair(pending_change.second, pending_change.first)] = (stoi(token));
            }
            else { inputValid = false; }
        }
        if (!inputValid) { cout << "Invalid edge! Attempt aborted.\n\n"; continue; }
        else { break; }
    }
    cout << "\nNew update to edge between nodes { " << pending_change.first << ' ' << pending_change.second << " } ";
    cout << "for new weight " << all_weights[make_pair(pending_change.first, pending_change.second)] << '\n';
    cout << '\n';
}
void clear_weights_input() { all_weights.clear(), weighted = false; }
void add_weights_input() {
    weighted = true;
    string temp;
    for (pair <string, string> x : all_edges) {
        while (true) {
            cout << "\nInput the desired weight for the edge { " << x.first << ' ' << x.second <<  " } ";
            if (weighted) { cout << "(currently set as " << all_weights[make_pair(x.first, x.second)] << "):\n"; }
            else { cout << "(currently unweighted):\n"; }
            bool inputValid = true;
            getline(cin, temp);
            if (is_integer(temp)) {
                all_weights[make_pair(x.first, x.second)] = (stoi(temp));
                all_weights[make_pair(x.second, x.first)] = (stoi(temp));
                break;
            }
            else { cout << "Invalid weight! Attempt aborted.\n\n"; continue; }
        }
    }
    cout << "\nAll new weights updated!\n";
}

//MST Menu:
bool detect_component_cycle_kruskal(string node, string parent, map <string, bool> vis) {
    vis[node] = 1;
    for (string neighbor : mst_graph[node]) {
        if (parent == neighbor) { continue; }
        if (vis[neighbor] == 1) { return true; }
        if (detect_component_cycle_kruskal(neighbor, node, vis)) { return true; }
    }
    return false;
}
bool detect_graph_cycle_kruskal() {
    map <string, bool> vis;
    for (string str : all_vertices) { vis[str] = false; }
    for (string str : all_vertices) { if (!vis[str]) { if (detect_component_cycle_kruskal(str, "-1", vis)) { return true; } } }
    return false;
}
void add_edge_kruskal(int index) { mst_graph[all_edges[index].first].insert(all_edges[index].second), mst_graph[all_edges[index].second].insert(all_edges[index].first); }
void remove_edge_kruskal(int index) { mst_graph[all_edges[index].first].erase(all_edges[index].second), mst_graph[all_edges[index].second].erase(all_edges[index].first); }
void perform_kruskal() {
    int edge_index = 0;
    while (mst_vertices.size() != all_vertices.size()) {
        add_edge_kruskal(edge_index);
        if (detect_graph_cycle_kruskal()) { remove_edge_kruskal(edge_index); }
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
                if (firstdisplay) { cout << " and new node " << all_edges[edge_index].second; }
                else { cout << "along with new node " << all_edges[edge_index].second; }
            }
            cout << '\n';
        }
        ++edge_index;
    }
}
void main_kruskal() {
    cout << "\nPerforming Kruskal Algorithm:\n";
    if (max_mode) { sort(all_edges.begin(), all_edges.end(), comp_max); }
    else { sort(all_edges.begin(), all_edges.end(), comp); }
    perform_kruskal();
    cout << "\n\nList of MST vertices:\n";
    for (string a : all_vertices) { cout << a << ' '; }
    cout << "\n\nList of MST Edges:\n";
    for (pair <string, string> b : mst_edges) { cout << "  Nodes: { " << b.first << ' ' << b.second << " }, Weight: " << all_weights[make_pair(b.first, b.second)] << '\n'; }
    cout << "\n\n";
    int all_weight_kruskal = 0;
    for (pair <string, string> b : mst_edges) { all_weight_kruskal += all_weights[make_pair(b.first, b.second)]; }
    cout << "\nAggregate ";
    if (max_mode) { cout << "Maximum"; }
    else { cout << "Minimum"; }
    cout << " Spanning Tree Weight: " << all_weight_kruskal << "\n\n\n";
}
void add_edge_prim(pair <string, string> new_edge) {
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
void perform_prim() {
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
void main_prim() {
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
            mst_vertices.insert(temp), all_vertices.erase(temp);
            cout << "\nStarting MST at vertex " << temp << ":\n";
            break;
        }
        else { cout << "Invalid start! Attempt aborted.\n\n"; }
    }
    perform_prim();
    cout << "\n\nList of MST Vertices:\n";
    for (string a : mst_vertices) { cout << a << ' '; }
    cout << "\n\nList of MST Edges:\n";
    for (pair <string, string> b : mst_edges) { cout << "  Nodes: { " << b.first << ' ' << b.second << " }, Weight: " << all_weights[make_pair(b.first, b.second)] << '\n'; }
    cout << "\n\n";
    int all_weight_prim = 0;
    for (pair <string, string> b : mst_edges) { all_weight_prim += all_weights[make_pair(b.first, b.second)]; }
    cout << "\nAggregate Minimum Spanning Tree Weight: " << all_weight_prim << "\n\n\n";
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
    for (string neighbor : all_graph[node]) { if (!visited[neighbor]) { dfs_count_paths(neighbor, dest, visited, current_path); } }
    visited[node] = false;
}
void main_count_paths() {
    string src, dest;
    while (true) {
        cout << "Enter the initial source vertex:\n";
        getline(cin, src);
        if (all_graph.find(src) != all_graph.end()) { break; }
        else { cout << "Invalid source! Attempt aborted.\n\n"; }
    }
    while (true) {
        cout << "Enter the final destination vertex:\n";
        getline(cin, dest);
        if (all_graph.find(dest) != all_graph.end()) { break; }
        else { cout << "Invalid destination! Attempt aborted.\n\n"; }
    }
    cout << "\n\nCounting paths between initial source node " << src << " and final destination node " << dest << ":\n\n";
    map <string, bool> visited;
    path_ans = 0;
    dfs_count_paths(src, dest, visited, {});
    for (deque <string> path : paths_count) {
        for (string node : path) { cout << node << " -> "; }
        cout << dest << '\n';
    }
    cout << "\nTotal count: " << path_ans << " paths\n";
}
void main_count_vertices() {
    cout << "\n\nList of All Vertices:\n";
    for (string a : all_vertices) { cout << a << ' '; }
    cout << "\nCurrent total vertex count (order): " << all_vertices.size() << '\n';
}
void main_count_edges() {
    cout << "\n\nList of All Edges:\n";
    for (pair <string, string> x : all_edges) {
        cout << "  Nodes: { " << x.first << ' ' << x.second << " }";
        if (weighted) { cout << ", Weight: " << all_weights[make_pair(x.first, x.second)] << '\n'; }
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
            for (string x : all_graph[temp]) { cout << x << ' '; }
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
        if (all_graph[x].size() == stoi(temp)) { cout << x << ' '; count_degree++; }
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
    for (string x : all_vertices) { if (depth[x] == stoi(target)) { count_depth++; cout << x << ' '; } }
    cout << "\nCount of all vertices of depth " << target << " from root " << root << ": " << count_depth << '\n';
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
    for (string x : all_vertices) { dist[x] = INF, parent[x] = ""; }
    dist[start] = 0, parent[start] = "";
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
        if (weighted) { cout << "Shortest Path from " << start << " to " << x << ": weight: " << dist[x] << '\n'; }
        else { cout << "Shortest Path from " << start << " to " << x << ": length: " << dist[x] << "\n  "; }
        deque <string> path = {x};
        string cur = parent.at(x);
        while (!cur.empty()) {
            path.push_back(cur);
            cur = parent.at(cur);
        }
        reverse(path.begin(), path.end());
        for (int i = 0; i < path.size() - 1; ++i) { cout << path[i] << " -(" << all_weights[make_pair(path[i], path[i + 1])] << ")-> "; }
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
    dist[start] = 0, parent[start] = "";
    cout << "Starting Bellman-Ford from " << start << "\n";
    for (int i = 1; i < all_vertices.size(); ++i) {
        cout << "Iteration " << i << ":\n";
        for (pair <string, string> x : all_edges) {
            int w = all_weights[x];
            if (dist[x.first] != INF && dist[x.first] + w < dist[x.second]) {
                cout << "  Relaxing edge " << x.first << " -> " << x.second << " with "; 
                if (weighted)
                cout << "weight " << w << ".\n";
                cout << "Updating distance to " << x.second << " from ";
                if (dist[x.second] == INF) { cout << "Unvisited"; }
                else { cout << dist[x.second]; }
                cout << " to " << dist[x.first] + all_weights[x] << ".\n";
                dist[x.second] = dist[x.first] + w;
                parent[x.second] = x.first;
            }
        }
        cout << "Iteration " << i << " finished.\n";
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
            if (weighted) { cout << "Shortest Path from " << start << " to " << x << ": weight: " << dist[x] << '\n'; }
            else { cout << "Shortest Path from " << start << " to " << x << ": length: " << dist[x] << "\n  "; }
            deque <string> path = {x};
            string cur = parent.at(x);
            while (!cur.empty()) {
                path.push_back(cur);
                cur = parent.at(cur);
            }
            reverse(path.begin(), path.end());
            for (int i = 0; i < path.size() - 1; ++i) { cout << path[i] << " -(" << all_weights[make_pair(path[i], path[i + 1])] << ")-> "; }
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
        if (weighted) { cout << "Shortest Path from " << start << " to " << x << ": weight: " << dist[make_pair(start, x)] << '\n'; }
        else { cout << "Shortest Path from " << start << " to " << x << ": length: " << dist[make_pair(start, x)] << "\n  "; }
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

//Main Driver Function
int main() {
    srand(time(0));
    cout << "\nWelcome to Graph Operator!\n\nTo begin, please input an initial connected graph:\n";
    clear_all();
    graph_input();
    while (true) {
        cout << "\nSelect function menu from below:\n";
        cout << "  Enter d to display the current graph\n";
        cout << "  Enter i to input or manipulate the graph\n";
        cout << "  Enter m to execute MST algorithms (only for weighted graphs)\n";
        cout << "  Enter c to execute compute functions\n";
        cout << "  Enter s to execute Shortest Path algorithms\n";
        cout << "  Enter a to view all commands\n";
        cout << "  Enter e to exit this program\n";
        char key_pressed = cin.get();
        cin.ignore(2147483647, '\n');
        if (key_pressed == 'd' || key_pressed == 'i' || key_pressed == 'm' || key_pressed == 'c' || key_pressed == 'a' || key_pressed == 'e') { cout << "\nKey enter \"" << key_pressed << "\" detected.\n"; }
        string target, root;
        if (key_pressed == 'd') { graph_display(); }
        else if (key_pressed == 'i') {
            bool exitStatus = false;
            while (true) {
                cout << "\n Select input type from list below:\n";
                cout << "   Enter g to input a new connected graph\n";
                cout << "   Enter n to rename a node\n";
                cout << "   Enter a to add a new edge\n";
                cout << "   Enter d to delete an edge\n";
                cout << "   Enter r to re-weight an edge\n";
                cout << "   Enter c to clear weights from the current graph\n";
                cout << "   Enter w to add weights to the current graph\n";
                cout << "   Enter e to exit back to the main menu\n";
                char input_pressed = cin.get();
                cin.ignore(2147483647, '\n');
                if (input_pressed == 'g' || input_pressed == 'n' || input_pressed == 'a' || input_pressed == 'd' || input_pressed == 'r' || input_pressed == 'c' || input_pressed == 'w' || input_pressed == 'e') { cout << "\nKey enter \"" << input_pressed << "\" detected.\n"; }
                if (input_pressed == 'g') { graph_input(); }
                else if (input_pressed == 'n') { rename_node_input(); }
                else if (input_pressed == 'a') { add_edge_input(); }
                else if (input_pressed == 'd') { remove_edge_input(); }
                else if (input_pressed == 'r') { reweight_edge_input(); }
                else if (input_pressed == 'c') { clear_weights_input(); }
                else if (input_pressed == 'w') { add_weights_input(); }
                else if (input_pressed == 'e') { exitStatus = true; }
                else { cout << "\n\"" << input_pressed << "\" is an invalid key enter! Attempt aborted.\n"; }
                if (exitStatus) { break; }
            }
        }
        else if (key_pressed == 'm') {
            if (weighted) {
                bool exitStatus = false;
                cout << "\n The current graph is weighted and valid.\n";
                while (true) {
                    cout << "\n Select algorithm from list below:\n";
                    cout << "   Enter k to compute the ";
                    if (max_mode) { cout << "Maximum"; }
                    else { cout << "Minimum"; }
                    cout << " Spanning Tree (MST) using Kruskal's Algorithm\n";
                    cout << "   Enter p to compute the ";
                    if (max_mode) { cout << "Maximum"; }
                    else { cout << "Minimum"; }
                    cout << " Spanning Tree (MST) using Prim's Algorithm\n";
                    cout << "   Enter m to change between Minimum and Maximum Spanning Tree modes\n";
                    cout << "   Enter e to exit back to the main menu\n";
                    char algo_pressed = cin.get();
                    cin.ignore(2147483647, '\n');
                    if (algo_pressed == 'k' || algo_pressed == 'p' || algo_pressed == 'm' || algo_pressed == 'e') { cout << "\nKey enter \"" << algo_pressed << "\" detected.\n"; }
                    if (algo_pressed == 'k') { main_kruskal(); }
                    else if (algo_pressed == 'p') { main_prim(); }
                    else if (algo_pressed == 'm') { max_mode = !max_mode; }
                    else if (algo_pressed == 'e') { exitStatus = true; }
                    else { cout << "\n\"" << algo_pressed << "\" is an invalid key enter! Attempt aborted.\n"; }
                    if (exitStatus) { break; }
                }
            }
            else { cout << "\n The current graph is invalid and not weighted!\nUse input function w in menu i to add weights to the current graph.\n"; }
        }
        else if (key_pressed == 'c') {
            bool exitStatus = false;
            while (true) {
                cout << "\n Select function from list below:\n";
                cout << "   Enter o to compute the current graph's order (vertex count)\n";
                cout << "   Enter s to compute the current graph's size (edge count)\n";
                cout << "   Enter p to count the paths between two nodes\n";
                cout << "   Enter n to compute the neighbors of a given node (degree)\n";
                cout << "   Enter d to compute nodes with a given degree in the current graph\n";
                cout << "   Enter r to compute all nodes of a given depth from a given root\n";
                cout << "   Enter e to exit back to the main menu\n";
                char func_pressed = cin.get();
                cin.ignore(2147483647, '\n');
                if (func_pressed == 'o' || func_pressed == 's' || func_pressed == 'p' || func_pressed == 'e') { cout << "\nKey enter \"" << func_pressed << "\" detected.\n"; }
                if (func_pressed == 'o') { main_count_vertices(); }
                else if (func_pressed == 's') { main_count_edges(); }
                else if (func_pressed == 'p') { main_count_paths(); }
                else if (func_pressed == 'n') { main_count_neighbors(); }
                else if (func_pressed == 'd') { main_count_degree(); }
                else if (func_pressed == 'r') { main_count_root_depth(); }
                else if (func_pressed == 'e') { exitStatus = true; }
                else { cout << "\n\"" << func_pressed << "\" is an invalid key enter! Attempt aborted.\n"; }
                if (exitStatus) { break;}
            }
        }
        else if (key_pressed == 's') {
            bool exitStatus = false;
            cout << "\n The current graph is weighted and valid.\n";
            while (true) {
                cout << "\n Select algorithm from list below:\n";
                cout << "   Enter f to compute shortest paths using Floyd-Warshall's Algorithm:\n";
                cout << "   Enter d to compute shortest paths using Dijkstra's Algorithm:\n";
                cout << "   Enter b to compute shortest paths using Bellman-Ford's Algorithm:\n";
                cout << "   Enter e to exit back to the main menu\n";
                char algo_pressed = cin.get();
                cin.ignore(2147483647, '\n');
                if (algo_pressed == 'f' || algo_pressed == 'd' || algo_pressed == 'b' || algo_pressed == 'e') { cout << "\nKey enter \"" << algo_pressed << "\" detected.\n"; }
                if (algo_pressed == 'f') { main_floyd_warshall(); }
                else if (algo_pressed == 'd') { main_dijkstra(); }
                else if (algo_pressed == 'b') { main_bellman_ford(); }
                else if (algo_pressed == 'e') { exitStatus = true; }
                else { cout << "\n\"" << algo_pressed << "\" is an invalid key enter! Attempt aborted.\n"; }
                if (exitStatus) { break; }
            }
        }
        else if (key_pressed == 'a') {
            cout << " Below is the master list of all user accessible commands:\n";
            cout << "   Menu i operation d: display the current graph\n";
            cout << "   Menu i operation g: input a new connected graph\n";
            cout << "   Menu i operation n: rename a node\n";
            cout << "   Menu i operation a: add a new edge\n";
            cout << "   Menu i operation d: delete an edge\n";
            cout << "   Menu i operation r: re-weight an edge\n";
            cout << "   Menu i operation c: clear weights from the current graph\n";
            cout << "   Menu i operation w: add weights to the current graph\n";
            cout << "   Menu m operation k: compute the ";
            if (max_mode) { cout << "Maximum"; }
            else { cout << "Minimum"; }
            cout << " Spanning Tree (MST) using Kruskal's Algorithm\n";
            cout << "   Menu m operation p: compute the ";
            if (max_mode) { cout << "Maximum"; }
            else { cout << "Minimum"; }
            cout << " Spanning Tree (MST) using Prim's Algorithm\n";
            cout << "   Menu m operation m: change between Minimum and Maximum Spanning Tree modes\n";
            cout << "   Menu c operation o: compute the current graph's order (vertex count)\n";
            cout << "   Menu c operation s: compute the current graph's size (edge count)\n";
            cout << "   Menu c operation p: count the paths between two given nodes\n";
            cout << "   Menu c operation n: compute the neighbors of a given node (degree)\n";
            cout << "   Menu c operation d: compute nodes with a given degree in the current graph\n";
            cout << "   Menu c operation r: compute all nodes of a given depth from a given root\n";
            cout << "   Menu s operation f: compute shortest paths using Floyd-Warshall's Algorithm\n";
            cout << "   Menu s operation d: compute shortest paths using Dijkstra's Algorithm\n";
            cout << "   Menu s operation b: compute shortest paths using Bellman-Ford's Algorithm\n";
        }
        else if (key_pressed == 'e') { return 0; }
        else { cout << "\n\"" << key_pressed << "\" is an invalid key enter! Attempt aborted.\n"; }
    }
    return 0;
}

