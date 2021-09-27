// The code for the max_flow class is from:
// https://github.com/stevenhalim/cpbook-code/blob/master/ch8/maxflow.cpp
// This solves UVA 11419 - SAM I AM
// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=2414
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef tuple<int, ll, ll> edge;
typedef vector<int> vi;
typedef pair<int, int> ii;

const ll INF = 1e18;                             // large enough

class max_flow {
private:
  int V;
  vector<edge> EL;
  vector<vi> AL;
  vi d, last;
  vector<ii> p;

  bool BFS(int s, int t) {                       // find augmenting path
    
    d.assign(V, -1);                             // initialize distance array
    d[s] = 0;                                    // s is the source, so d[s] = 0
    
    queue<int> q({s});                           // load s on the queue 
    
    p.assign(V, {-1, -1});                       // record BFS sp tree

    while (!q.empty()) {
    
      int u = q.front(); q.pop();                // current vertex to be processed

      if (u == t) break;                         // stop as sink t reached
     
      for (auto &idx : AL[u]) {                  // explore neighbors of u
        auto &[v, cap, flow] = EL[idx];          // stored in EL[idx]
        
        if ((cap-flow > 0) && (d[v] == -1)){     // positive residual edge + unexplored
            
            d[v] = d[u]+1;                       // set distance
            q.push(v);                           // push on queue
            p[v] = {u, idx};                     // set parent information
        
        }
      }
    }
    
    return d[t] != -1;                           // has an augmenting path
  
  }

  ll send_one_flow(int s, int t, ll f = INF) {   // send one flow from s->t
    
    if (s == t) return f;                        // bottleneck edge f found
    
    auto &[u, idx] = p[t];
    
    auto &cap = get<1>(EL[idx]), &flow = get<2>(EL[idx]);
    
    ll pushed = send_one_flow(s, u, min(f, cap-flow));
    
    flow += pushed;
    
    auto &rflow = get<2>(EL[idx^1]);             // back edge
    rflow -= pushed;                             // back flow
    
    return pushed;
  }


public:
  max_flow(int initialV) : V(initialV) {
    EL.clear();
    AL.assign(V, vi());
  }

  void print_edges(){
      for(int i = 0; i < V; i++){
        cout << i << "'s neighbors: " << endl;
        for(auto x : AL[i]){
            cout << x << endl;
        }
        cout << endl;
      }   
  }

  // if you are adding a bidirectional edge u<->v with weight w into your
  // flow graph, set directed = false (default value is directed = true)
  void add_edge(int u, int v, ll w, bool directed = true) {
    if (u == v) return;                          // safeguard: no self loop
    
    EL.emplace_back(v, w, 0);                    // u->v, cap w, flow 0
    AL[u].push_back(EL.size()-1);                // remember this index
    
    EL.emplace_back(u, directed ? 0 : w, 0);     // back edge
    AL[v].push_back(EL.size()-1);                // remember this index
  
  }

  ll edmonds_karp(int s, int t) {
    
    ll maxf = 0;                                 // maxf stands for max_flow
    
    while (BFS(s, t)) {                          // an O(V*E^2) algorithm
    
      ll f = send_one_flow(s, t);                // find and send 1 flow f
      if (f == 0) break;                         // if f == 0, stop
      maxf += f;                                 // if f > 0, add to maxf
    
    }
    
    return maxf;
  }


  vi reachable_set(int s){
      
    d.assign(V, -1);                             // initialize distance array
    d[s] = 0;                                    // s is the source, so d[s] = 0
    vi answer; 

    queue<int> q({s});                           // load s on the queue 
    
    p.assign(V, {-1, -1});                       // record BFS sp tree

    while (!q.empty()) {
    
      int u = q.front(); q.pop();                // current vertex to be processed

     
      for (auto &idx : AL[u]) {                  // explore neighbors of u
        auto &[v, cap, flow] = EL[idx];          // stored in EL[idx]
        
        if ((cap-flow > 0) && (d[v] == -1)){     // positive residual edge + unexplored
            
            d[v] = d[u]+1;                       // set distance
            q.push(v);                           // push on queue
            answer.push_back(v); 
        
        }
      }
    }
    
    return answer;                               // set of reachable vertices
  }


};

int main() {

  #ifndef ONLINE_JUDGE 
      freopen("in.txt", "r", stdin);
      freopen("out.txt", "w", stdout);
  #endif
  
  std::ios::sync_with_stdio(false);
  while(true){

    int r, c, n;
    cin >> r >> c >> n; 
    if(r == 0 && c == 0 && n == 0)
        break; 

    max_flow maxf(r+c+2);
    
    for(int i = 0; i < n; i++){
        int u, v; 
        cin >> u >> v; 
        maxf.add_edge(u,r+v,1); 
    }

    for(int i = 1; i < r+1; i++){
        maxf.add_edge(0,i,1); 
    }

    for(int i = 1; i < c+1; i++){
        maxf.add_edge(r+i,r+c+1,1); 
    }
    
    cout << maxf.edmonds_karp(0,r+c+1) << " "; 

    vi vc = maxf.reachable_set(0);

    for(int i = 1; i < r+1; i++){
        if(find(vc.begin(),vc.end(),i) == vc.end())
            cout << "r" << i << " ";
    }

    for(int i = 1; i < c+1; i++){
        if(find(vc.begin(),vc.end(),i+r) != vc.end())
            cout << "c" << i << " ";
    }

    cout << endl;
    
  }

  return 0;

}