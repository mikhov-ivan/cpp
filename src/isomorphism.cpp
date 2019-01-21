#include <iostream>
#include <stdio.h>
 
using namespace std;
  
#define MAX_N 100000
 
int n1 = 0, m1 = 0, k1 = 0;
int n2 = 0, m2 = 0, k2 = 0;
int u1 = 0, v1 = 0;
 
bool terminal1[MAX_N];
bool terminal2[MAX_N];
bool used[MAX_N];

int b[MAX_N];
 
int transitions1[MAX_N][26];
int transitions2[MAX_N][26];
 
bool check(int = 0);
 
int main() {
    ios_base::sync_with_stdio(0);
    freopen("isomorphism.in", "r", stdin);
    freopen("isomorphism.out", "w", stdout);
 
    scanf("%d %d %d", &n1, &m1, &k1);

    for (int i = 0; i < MAX_N; i++) {
        b[i] = -1;
        used[i] = false;
        terminal1[i] = false;
        terminal2[i] = false;
    }
 
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < 26; j++) {
            transitions1[i][j] = -1;
        }
    }
 
    int tmp = 0;
    for (int i = 0; i < k1; i++) {
        scanf("%d", &tmp);
        tmp--;
        terminal1[tmp] = true;
    }
 
    int from = 0, to = 0;
    char ch = 0;
    for (int i = 0; i < m1; i++) {
        scanf("%d %d %c", &from, &to, &ch);
        from--;
        to--;
        ch -= 'a';
        transitions1[from][ch] = to;
    }
 
    scanf("%d %d %d", &n2, &m2, &k2);
 
 
    for (int i = 0; i < n2; i++) {
        for (int j = 0; j < 26; j++) {
            transitions2[i][j] = -1;
        }
    }
 
    tmp = 0;
    for (int i = 0; i < k2; i++) {
        scanf("%d", &tmp);
        tmp--;
        terminal2[tmp] = true;
    }
 
    from = 0;
    to = 0;
    ch = 0;
    for (int i = 0; i < m2; i++) {
        scanf("%d %d %c", &from, &to, &ch);
        from--;
        to--;
        ch -= 'a';
        transitions2[from][ch] = to;
    }
 
    b[0] = 0;
    if (((n1 == n2) && (m1 == m2) && (terminal1[0] == terminal2[0])) && check()) {
        cout << "YES";
    } else {
        cout << "NO";
    }
 
    return 0;
}
 
bool check(int u) {
    used[u] = true;
    int v = b[u];
 
    for (int i = 0; i < 26; i++) {
        if (((transitions1[u][i] != -1) && (transitions2[v][i] == -1)) ||
            ((transitions1[u][i] == -1) && (transitions2[v][i] != -1))) {
            return false;
        }
 
        if (transitions1[u][i] != -1) {
            u1 = transitions1[u][i];
            v1 = transitions2[v][i];

            if (b[u1] == -1) {      
                if (terminal1[u1] != terminal2[v1]) {
                    return false;
                } else {
                    b[u1] = v1;
                }
            } else {
                if (b[u1] != v1) {
                    return false;
                }
            }
 
            if (!used[u1]) {
                return check(u1);
            }
        }
    }
 
    return true;
}