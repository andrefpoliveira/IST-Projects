#include <iostream>
#include <vector>
#include <list> 
#include <stack> 
#include<bits/stdc++.h> 
using namespace std;

class Graph 
{ 
    int V;    // No. of vertices 
    list<int> *adj;    // A dynamic array of adjacency lists 
  
    // A Recursive DFS based function used by SCC() 
    void SCCUtil(int u, int disc[], int low[], 
                 stack<int> *st, bool stackMember[], int *grades); 
public: 
    Graph(int V);   // Constructor 
    void addEdge(int v, int w);   // function to add an edge to graph 
    void SCC(int *grades);    // prints strongly connected components 
    void UpdateSccGrades(vector<int> scc, int *grades, int max);
}; 
  
Graph::Graph(int V) 
{ 
    this->V = V; 
    adj = new list<int>[V]; 
}
  
void Graph::addEdge(int v, int w) 
{ 
    adj[v].push_back(w); 
}

void Graph::SCCUtil(int u, int disc[], int low[], stack<int> *st, 
                    bool stackMember[], int *grades) 
{ 
    // A static variable is used for simplicity, we can avoid use 
    // of static variable by passing a pointer. 
    static int time = 0; 
  
    // Initialize discovery time and low value 
    disc[u] = low[u] = ++time; 
    st->push(u); 
    stackMember[u] = true; 
  
    // Go through all vertices adjacent to this 
    list<int>::iterator i; 
    for (i = adj[u].begin(); i != adj[u].end(); ++i) 
    { 
        int v = *i;  // v is current adjacent of 'u' 
  
        // If v is not visited yet, then recur for it 
        if (disc[v] == -1) 
        { 
            SCCUtil(v, disc, low, st, stackMember, grades); 
  
            // Check if the subtree rooted with 'v' has a 
            // connection to one of the ancestors of 'u' 
            // Case 1 (per above discussion on Disc and Low value) 
            low[u]  = min(low[u], low[v]); 
        } 
  
        // Update low value of 'u' only of 'v' is still in stack 
        // (i.e. it's a back edge, not cross edge). 
        // Case 2 (per above discussion on Disc and Low value) 
        else if (stackMember[v] == true) 
            low[u]  = min(low[u], disc[v]);

        if (grades[u] < grades[v])  {
            grades[u] = grades[v];
        }
    } 
  
    int w = 0;  // To store stack extracted vertices 
    int maxScc = INT_MIN;
    vector<int> scc;
    if (low[u] == disc[u]) 
    { 
        while (st->top() != u) 
        { 
            w = (int) st->top(); 
            stackMember[w] = false; 
            st->pop(); 
            scc.push_back(w);

            if (maxScc < grades[w]) maxScc = grades[w];
        }
        w = (int) st->top();

        if (maxScc < grades[w]) maxScc = grades[w];

        stackMember[w] = false; 
        st->pop();
        scc.push_back(w);

        if (!st->empty()) {
            if (grades[w] > grades[st->top()]) {
                grades[st->top()] = grades[w];
            }
        }

        UpdateSccGrades(scc, grades, maxScc);
    } 
} 
  
// The function to do DFS traversal. It uses SCCUtil() 
void Graph::SCC(int *grades) 
{ 
    int *disc = new int[V]; 
    int *low = new int[V]; 
    bool *stackMember = new bool[V]; 
    stack<int> *st = new stack<int>(); 
  
    // Initialize disc and low, and stackMember arrays 
    for (int i = 0; i < V; i++) 
    { 
        disc[i] = -1; 
        low[i] = -1; 
        stackMember[i] = false; 
    } 
  
    // Call the recursive helper function to find strongly 
    // connected components in DFS tree with vertex 'i' 
    for (int i = 0; i < V; i++) 
        if (disc[i] == -1) 
            SCCUtil(i, disc, low, st, stackMember, grades); 
}

void Graph::UpdateSccGrades(vector<int> scc, int *grades, int maxGrade) {
    for (int student: scc) {
        if (grades[student] < maxGrade) {
            grades[student] = maxGrade;
        }
    }
}

int main() 
{
    int students, connections;
    scanf("%d,%d", &students, &connections);
    int *grades = (int*)malloc(sizeof(int) * students);

    Graph g(students);

    for (int i = 0; i < students; i++) {
        scanf("%d", &grades[i]);
    }

    for (int i = 0; i < connections; i++) {
        int x,y;
        scanf("%d %d", &x, &y);
        g.addEdge(x-1,y-1);
    }

    g.SCC(grades);

    for (int i = 0; i < students; i++) {
        cout << grades[i] << "\n";
    }

    free(grades);

    return 0;
}
