#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <set>
   
using namespace std;
 
bool use[26];
int useless[26];
set< string > set_of_strings[26];
 
bool dfs(char);
bool check(string);
   
int main() {
    ifstream in;
    ofstream out;
  
    in.open("useless.in");
    out.open("useless.out");
 
    int n = 0;
    char s = 0;
    memset(use, false, 26);
 
    in >> n >> s;
    string line = "";
    getline(in, line);
    for (int i = 0; i < n; i++) {
        getline(in, line);
        char from = line[0];
        from -= 'A';
        useless[from] = 2;
        if (!use[from]) {
            use[from] = true;
        }
        if (line.length() > 5) {
            string tmp = line.substr(5, line.length());
            set_of_strings[from].insert(tmp);
            for (int j = 0; j < tmp.length(); j++) {
                if (tmp[j] >= 'A' && tmp[j] <= 'Z') {
                    useless[tmp[j] - 'A'] = 2;
                }
            }
        } else {
            set_of_strings[from].insert("a");
        }
    }
 
    dfs(s);
    while (true) {
        bool flag = false;
        for (int i = 0; i < 26; i++) {
            if (use[i] && useless[i] != 2) {
                for (set< string >::iterator str = set_of_strings[i].begin(); str != set_of_strings[i].end(); str++) {
                    if (check(*str)) {
                        use[i] = false;
                        useless[i] = 1;
                        flag = true;
                    }
                }
            }
        }
 
        if (!flag) {
            break;
        }
    }
 
    for (int i = 0; i < 26; i++) {
        if (useless[i] == -1 || useless[i] == 2) {
            char tmp = i;
            tmp += 'A';
            out << tmp << " ";
        }
    }
 
    in.close();
    out.close();
    return 0;
}
 
bool dfs(char ch) {
    if (ch >= 'a' && ch <= 'z') {
        return true;
    }
 
    ch -= 'A';
    useless[ch] = -1;
    if (!use[ch]) {
    } else {
        for (set< string >::iterator str = set_of_strings[ch].begin(); str != set_of_strings[ch].end(); str++) {
            for (int j = 0; j < (*str).length(); j++) {
                dfs((*str)[j]);
            }
        }
    }
    return true;
}
 
bool check(string str) {
    for (int i = 0; i < str.length(); i++) {
        if (!((str[i] >= 'a' && str[i] <= 'z') || useless[str[i] - 'A'] == 1)) {
            return false;
        }
    }
    return true;
}