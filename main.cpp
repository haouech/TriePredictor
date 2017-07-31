#include <bits/stdc++.h>
using namespace std;
const int MAX = 1009;
char in[MAX];

struct node{
	bool leaf;
	int cnt;
	vector<int> adj; // each element points to the id of the adjacent node inside the trie vector
	node(){
		leaf = 0;
		cnt = 0;
		adj.resize(26,-1); // -1 means there is no child yet in this node
	}
};
vector<node> trie;

void insert(char* s){
	int cur = 0;
	for(int i=0;s[i];i++){
		if(trie[cur].adj[ s[i]-'a' ]==-1){
			trie[cur].adj[ s[i]-'a' ] = trie.size();
			trie.push_back(node());
		}
		cur = trie[cur].adj[ s[i]-'a' ];
		trie[cur].cnt++;
	}
	trie[cur].leaf = 1;
}


vector<pair<int,string> > get(char* in)
{
    /// get all the words to suggest
    /// return: each vector element contains frequency count and the string to suggest
    int cur = 0;
    vector<pair<int, string> > res;
    string s;
    // Getting the path of the input word inside the Trie tree
    for(int i=0;in[i];i++){
        if(trie[cur].adj[in[i]-'a'] == -1) return res;
        cur = trie[cur].adj[in[i]-'a'];
        s += in[i];
    }

    // Apply DFS on rest of the tree
    // Root node of DFS is the last char of the input word
    // Non recursive DFS using a stack having node id and current string pair elements
    stack<pair<int,string> > St;
    St.push({cur,s});
    vector<bool> vis;   // visited array to keep track of already visited nodes
    vis.assign(trie.size(),false);
    while(!St.empty())
    {
        int u = St.top().first;
        string u_str = St.top().second;
        vis[u] = true;
        // Push only the nodes that have values and complete the string that we reached so far
        for(int i=0;i<26;i++){
            if(trie[u].adj[i] != -1 && !vis[trie[u].adj[i]]){
                string cur_str = u_str + char(i+'a');
                St.push({trie[u].adj[i],u_str + char(i+'a')});
            }
        }
        int v = St.top().first;
        string v_str = St.top().second;
        // if visited then we are going up in our DFS, we pop the current node because
        // we visited all of its children
        if(vis[v]){
            St.pop();
        } else if(trie[v].leaf){    // if leaf, this is a good string to recommend
            res.push_back({trie[v].cnt, v_str});
        }
    }
    return res;
//
//    bool done = false;
//    while(true)
//    {
//        if(trie[cur].leaf){
//            done = true;
//        }
//        for(int i=0; i<26; i++){
//            if(trie[cur].adj[i] != -1){
//                ret += char(i+'a');
//                cur = trie[cur].adj[i];
//                break;
//            }
//        }
//        if(done) break;
//    }
//    return ret;
}
void train(char* filename)
{
    ifstream infile;
    infile.open(filename);
    char word[MAX];
    while(infile >> word)
    {
        insert(word);
    }
}
void add(char* filename, char * in)
{
    ofstream outfile;
    outfile.open(filename, ios::app);
    outfile << in;
    outfile << '\n';
}
int main()
{
    char filename[20] = "train.in";
    trie.push_back(node());
    train(filename);
    while(true)
    {
        char in[MAX];
        scanf("%s",in);
        vector<pair<int,string> > res = get(in);
        sort(res.begin(), res.end()); // show the strings that have high count values
        if(!res.size()){ // if there is no matching string, add current to trie and to dictionary
            insert(in);
            add(filename, in);
        } else { // show a maximum of 10 strings
            for(int i=0;i<(int)res.size() && i <10; i++){
                cout << res[i].second << endl;
            }
        }
    }
    return 0;
}
