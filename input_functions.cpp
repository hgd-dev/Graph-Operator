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

//Input Menu:
void graph_input() {
    clear_all();
    string temp;
    while (true) {
        cout << "Enter 1 if the graph is weighted and 0 if not:\n";
        getline(cin, temp);
        if (temp == "1") {
            weighted = true;
            break;
        }
        else if (temp == "0") {
            weighted = false;
            break;
        }
        else { cout << "Invalid graph! Attempt aborted.\n\n"; }
    }
    while (true) {
        cout << "Enter your graph's number of edges (ε(G)):\n";
        getline(cin, temp);
        if (is_integer(temp) && stoi(temp) > 0) {
            e = stoi(temp);
            break;
        }
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
                else {
                    inputValid = false;
                    break;
                }
            }
            if (!inputValid) {
                cout << "Invalid edge! Attempt aborted.\n\n";
                continue;
            }
            else { all_edges[i].first = token; }


            token = "";
            while (token.empty()) {
                if (getline(iss, token, ' ')) {}
                else {
                    inputValid = false;
                    break;
                }
            }
            if (!inputValid) {
                cout << "Invalid edge! Attempt aborted.\n\n";
                continue;
            }
            else { all_edges[i].second = token; }


            if (weighted) {
                token = "";
                while (token.empty()) {
                    if (getline(iss, token, ' ')) {}
                    else {
                        inputValid = false;
                        break;
                    }
                }
                if (!inputValid) {
                    cout << "Invalid edge! Attempt aborted.\n\n";
                    continue;
                }
                else {
                    if (is_integer(token)) {
                        all_weights[make_pair(all_edges[i].first, all_edges[i].second)] = (stoi(token));
                        all_weights[make_pair(all_edges[i].second, all_edges[i].first)] = (stoi(token));
                    }
                    else { inputValid = false; }
                }
            }


            if (!inputValid) {
                cout << "Invalid edge! Attempt aborted.\n\n";
                continue;
            }
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
            if (weighted && !mst_vertices.empty()) {
                mst_vertices.erase(temp), mst_vertices.insert(new_name);
            }
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
            else {
                inputValid = false;
                break;
            }
        }
        if (!inputValid) {
            cout << "Invalid edge! Attempt aborted.\n\n";
            continue;
        }
        else { pending_add.first = token; }

        token = "";
        while (token.empty()) {
            if (getline(iss, token, ' ')) {}
            else {
                inputValid = false;
                break;
            }
        }
        if (!inputValid) {
            cout << "Invalid edge! Attempt aborted.\n\n";
            continue;
        }
        else { pending_add.second = token; }

        if (weighted) {
            token = "";
            while (token.empty()) {
                if (getline(iss, token, ' ')) {}
                else {
                    inputValid = false;
                    break;
                }
            }
            if (!inputValid) {
                cout << "Invalid edge! Attempt aborted.\n\n";
                continue;
            }
            else {
                if (is_integer(token)) {
                    all_weights[make_pair(pending_add.first, pending_add.second)] = (stoi(token));
                    all_weights[make_pair(pending_add.second, pending_add.first)] = (stoi(token));
                }
                else { inputValid = false; }
            }
        }

        if (!inputValid) {
            cout << "Invalid edge! Attempt aborted.\n\n";
            continue;
        }
        else { break; }
    }
    cout << "\nNew edge added between nodes { " << pending_add.first << ' ' << pending_add.second;
    if (weighted == true) {
        cout << " } of weight " << all_weights[make_pair(pending_add.first, pending_add.second)] << '\n';
    }
    bool firstdisplay = false;
    if (all_vertices.find(pending_add.first) == all_vertices.end()) {
        all_vertices.insert(pending_add.first);
        cout << "along with new node " << pending_add.first;
        firstdisplay = true;
    }
    if (all_vertices.find(pending_add.second) == all_vertices.end()) {
        all_vertices.insert(pending_add.second);
        if (firstdisplay) {
            cout << " and new node " << pending_add.second;
        }
        else {
            cout << "along with new node " << pending_add.second;
        }
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
            else {
                inputValid = false;
                break;
            }
        }
        if (!inputValid) {
            cout << "Invalid edge! Attempt aborted.\n\n";
            continue;
        }
        else {
            if (all_vertices.find(token) == all_vertices.end()) {
                cout << "Invalid edge! Attempt aborted.\n\n";
                continue;
            }
            pending_remove.first = token;
        }


        token = "";
        while (token.empty()) {
            if (getline(iss, token, ' ')) {}
            else {
                inputValid = false;
                break;
            }
        }
        if (!inputValid) {
            cout << "Invalid edge! Attempt aborted.\n\n";
            continue;
        }
        else {
            if (all_vertices.find(token) == all_vertices.end()) {
                cout << "Invalid edge! Attempt aborted.\n\n";
                continue;
            }
            pending_remove.second = token;
        }
        bool is_edge = false;
        for (pair <string, string> x : all_edges) {
            if (x == pending_remove || make_pair(x.second, x.first) == pending_remove) {
                is_edge = true;
                break;
            }
        }
        if (!is_edge) {
            cout << "Invalid edge! Attempt aborted.\n\n";
            continue;
        }
        else { break; }
    }
    cout << "\nRemoved edge between nodes { " << pending_remove.first << ' ' << pending_remove.second;
    if (weighted == true) {
        cout << " } of weight " << all_weights[make_pair(pending_remove.first, pending_remove.second)] << '\n';
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
            else {
                inputValid = false;
                break;
            }
        }
        if (!inputValid) {
            cout << "Invalid edge! Attempt aborted.\n\n";
            continue;
        }
        else {
            if (all_vertices.find(token) == all_vertices.end()) {
                cout << "Invalid edge! Attempt aborted.\n\n";
                continue;
            }
            pending_change.first = token;
        }

        token = "";
        while (token.empty()) {
            if (getline(iss, token, ' ')) {}
            else {
                inputValid = false;
                break;
            }
        }
        if (!inputValid) {
            cout << "Invalid edge! Attempt aborted.\n\n";
            continue;
        }
        else {
            if (all_vertices.find(token) == all_vertices.end()) {
                cout << "Invalid edge! Attempt aborted.\n\n";
                continue;
            }
            pending_change.second = token;
        }

        token = "";
        while (token.empty()) {
            if (getline(iss, token, ' ')) {}
            else {
                inputValid = false;
                break;
            }
        }
        if (!inputValid) {
            cout << "Invalid edge! Attempt aborted.\n\n";
            continue;
        }
        else {
            if (is_integer(token)) {
                all_weights[make_pair(pending_change.first, pending_change.second)] = (stoi(token));
                all_weights[make_pair(pending_change.second, pending_change.first)] = (stoi(token));
            }
            else { inputValid = false; }
        }

        if (!inputValid) {
            cout << "Invalid edge! Attempt aborted.\n\n";
            continue;
        }
        else { break; }
    }
    cout << "\nNew update to edge between nodes { " << pending_change.first << ' ' << pending_change.second;
    if (weighted == true) {
        cout << " } for new weight " << all_weights[make_pair(pending_change.first, pending_change.second)] << '\n';
    }
    cout << '\n';
}
void clear_weights_input() {
    all_weights.clear();
    weighted = false;
}
void add_weights_input() {
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
            else {
                cout << "Invalid weight! Attempt aborted.\n\n";
                continue;
            }
        }
    }
    cout << "\nAll new weights updated!\n";
}
