#include <cstdio>
#include <iostream>
#include <vector>
#include <queue>
 
#pragma comment(linker, "/STACK:64000000")
  
#define MAX_N 50001
  
using namespace std;
  
class State {
public:
    int id;
    State* previous;
    State* next;
  
    State() {
 
    }
  
    State(int _id) {
        id = _id;
        previous = NULL;
        next = NULL;
    }
};
  
class MyQueue {
public:
    bool in[MAX_N][26];
    queue< pair< int, int > > q;
 
    MyQueue(int _size) {
        for (int i = 0; i < _size; i++) {
            for (int j = 0; j < 26; j++) {
                in[i][j] = false;
            }
        }
    }
  
    pair< int, int > pop() {
        pair< int, int > tmp = q.front();
        q.pop();
        in[tmp.first][tmp.second] = false;
        return tmp;
    }
  
    void add(pair< int, int > element) {
        q.push(element);
        in[element.first][element.second] = true;
    }
};
  
int color[MAX_N];
 
State* part[MAX_N];
 
int power[MAX_N];
int old_power[MAX_N];
 
State* position[MAX_N];
 
int len[MAX_N];
int equal_state[MAX_N];
 
bool used[MAX_N];
 
int partition[MAX_N];
int nums[MAX_N];
  
class DKA {
public:
    int n;
    int count;
    int transitions[MAX_N][26];
    vector< int > r_transitions[MAX_N][26];
    bool terminal[MAX_N];   
 
    DKA(int _n) {
        n = _n;
        count = 2;
        for (int i = 0; i < _n; i++) {
            terminal[i] = false;
            for (int j = 0; j < 26; j++) {
                transitions[i][j] = 0;
            }
        }
    }
 
    void rev_transitions() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < 26; j++) {
                r_transitions[transitions[i][j]][j].push_back(i);
            }
        }
    }
  
    void add_node(State* _node, int _pos) {
        color[_node->id] = _pos;
        power[_pos]++;
        State* tmp = part[_pos];
        _node->next = tmp;
        if (tmp != NULL) {
            tmp->previous = _node;
        }
        part[_pos] = _node;
    }
 
    void dfs(int v = 1) {
        used[v] = true;
        for (int i = 0; i < 26; i++) {
            if(!used[transitions[v][i]]) {
                dfs(transitions[v][i]);
            }
        }
    }
 
    DKA* build_min_dka() {
        rev_transitions();
 
        MyQueue q(n);
        for (int i = 0; i < n; i++) {
            if (terminal[i]) {
                add_node(position[i], 1);
            } else {
                add_node(position[i], 2);
            }
        }
 
        int tmp_tmp = 2;
        if (power[1] < power[2]) {
            tmp_tmp = 1;
        }
        for (int i = 0; i < 26; i++) {
            q.add(make_pair(tmp_tmp, i));
        }
 
        vector< int > inv;
        vector< int > positions;
        while (!q.q.empty()) {
            positions.clear();
            inv.clear();
 
            pair< int, int > tmp = q.pop();
            for (State* _node = part[tmp.first]; _node != NULL; _node = _node->next) {
                positions.push_back(_node->id);
                for (int j = 0; j < r_transitions[_node->id][tmp.second].size(); j++) {
                    int x = r_transitions[_node->id][tmp.second][j];
                    int i = color[x];
                    if (len[i] == 0) {
                        old_power[i] = power[i];
                        inv.push_back(i);
                        len[i] = 1;
                    } else {
                        len[i]++;
                    }
                }
            }
 
            for (int h = 0; h < positions.size(); h++) {
                for (int j = 0; j < r_transitions[positions[h]][tmp.second].size(); j++) {
                    int x = r_transitions[positions[h]][tmp.second][j];
                    int y = color[x];
                    if (len[y] < old_power[y]) {
                        if (equal_state[y] == 0) {
                            count++;
                            equal_state[y] = count;
                        }
                        State* node = position[x];
                        power[y]--;
                        if (node->previous != NULL) {
                            node->previous->next = node->next;
                            if (node->next != NULL) {
                                node->next->previous = node->previous;
                            }
                        } else {
                            part[y] = node->next;
                            if (node->next != NULL) {
                                node->next->previous = NULL;
                            }
                        }
                        node->previous = NULL;
                        node->next = NULL;
                        add_node(node, equal_state[y]);
                    }
                }
            }
 
            for (int i = 0; i < inv.size(); i++) {
                int u = inv[i];
                if (len[u] < old_power[u]) {
                    int tmp = u;
                    if (power[u] >= power[equal_state[u]]) {
                        tmp = equal_state[u];
                    }
                    for (int j = 0; j < 26; j++) {
                        if (q.in[u][j]) {
                            q.add(make_pair(equal_state[u], j));
                        } else {
                            q.add(make_pair(tmp, j));
                        }
                    }
                }
            }
 
            for (int i = 0; i < inv.size(); i++) {
                equal_state[inv[i]] = 0;
                old_power[inv[i]] = 0;
                len[inv[i]] = 0;
            }
        }
 
        dfs();
        int count = 0;
        for (int i = 0; i < n; i++) {
            nums[i] = -1;
            partition[i] = -1;
        }
 
        nums[0] = count++;
        partition[0] = nums[0];
        for (int i = 1; i < n; i++) {
            if (used[i] && color[i] != color[0]) {
                if (nums[color[i]] == -1) {
                    nums[color[i]] = count++;
                }
                partition[i] = nums[color[i]];
            } else {
                partition[i] = 0;
            }
        }
 
        DKA* new_dka = new DKA(count);
        count = 0;
        for (int i = 1; i < n; i++) {
            if (partition[i] > 0) {
                new_dka->terminal[partition[i]] |= terminal[i];
            }
            if (partition[i] > count) {
                count = partition[i];
                for (int j = 0; j < 26; j++) {
                    new_dka->transitions[partition[i]][j] = partition[transitions[i][j]];
                }
            }
        }
 
        return new_dka;
    }
  
    void print_dka() {
        string e = "";
        int new_m = 0;
        vector< int > new_terminal;
 
        for (int i = 1; i < n; i++) {
            if (terminal[i]) {
                new_terminal.push_back(i);
            }
            for (int j = 0; j < 26; j++) {
                if (transitions[i][j] != 0) {
                    new_m++;
                }
            }
        }
 
        cout << (n - 1) << " " << new_m << " " << new_terminal.size() << endl;
 
        for (int i = 0; i < new_terminal.size(); i++) {
            cout << new_terminal[i] << " ";
        }
        cout << endl;
        
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < 26; j++) {
                if (transitions[i][j] != 0) {
                    cout << i << " " << transitions[i][j] << " " << (char) (j + 'a') << endl;
                }
            }
        }
    }
};
  
int main() {
    freopen("minimization.in", "r", stdin);
    freopen("minimization.out", "w", stdout);
     
    int n = 0, m = 0, k = 0;
    char c = 0;
 
    cin >> n >> m >> k;
    for (int i = 0; i < n + 1; i++) {
        part[i] = NULL;
        position[i] = new State(i);
    }
  
    DKA* dka = new DKA(n + 1);
  
    for (int i = 0; i < k; i++) {
        int tmp = 0;
        cin >> tmp;
        dka->terminal[tmp] = true;
    }
 
    for (int i = 0; i < m; i++) {
        int tmp1 = 0, tmp2 = 0;
        char tmp3 = 0;
        cin >> tmp1 >> tmp2 >> tmp3;
        dka->transitions[tmp1][tmp3 - 'a'] = tmp2;
    }
  
    DKA* new_dka = dka->build_min_dka();
    new_dka->print_dka();
 
    return 0;
}