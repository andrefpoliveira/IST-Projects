#include <iostream> 
#include <limits.h> 
#include <string.h> 
#include <list>
#include <queue> 
using namespace std;

struct Edge {
    int v, capacity;

    Edge(int v, int capacity) {
        this->v = v;
        this->capacity = capacity;
    }
};

vector<int> parent;
vector<vector<Edge>> graph;

Edge findEdge(vector<Edge> arr, int node) {
    int size = arr.size();
    for (int i = 0; i < size; i++) {
        if (arr.at(i).v == node) {
            return arr.at(i);
        }
    }
    return Edge(-1,0);
    
}

bool bfs(int s, int t, int size) { 
    vector<bool> visited; 
    for (int i = 0; i < size; i++) {visited.push_back(false);}

    queue <int> q; 
    q.push(s); 
    visited[s] = true; 
    parent[s] = -1; 
  
    while (!q.empty()) 
    { 
        int u = q.front(); 
        q.pop(); 
  
        int sizeG = graph[u].size();
        for (int v=0; v<sizeG; v++) 
        { 
            Edge e = graph[u].at(v);
            if (visited[e.v]==false && e.capacity > 0)
            {      
                q.push(e.v); 
                parent[e.v] = u; 
                visited[e.v] = true; 
                if (e.v == t) {
                    return true;
                }
            } 
        } 
    } 

    return (visited[t] == true); 
} 
  
int edmondsKarp(int s, int t, int size, int citizens) 
{ 
    int u, v;
  
    int max_flow = 0;   

    while (bfs(s,t,size)) 
    {  
        int last = t;

        for (v=t; v != s; v=parent[v]) 
        { 
            
            u = parent[v];
            int uSize = graph[u].size();
            int lastSize = graph[last].size();

            for (int i = 0; i < uSize; i++) {
                if (graph[u].at(i).v == last) {
                    graph[u].at(i).capacity = 0;
                    break;
                }
            }

            for (int i = 0; i < lastSize; i++) {
                if (graph[last].at(i).v == u) {
                    graph[last].at(i).capacity = 1;
                    break;
                }
            }

            last = u;
            
        }
        max_flow++; 
    } 
    return max_flow; 
}

int escape(int m, int n, int citizens, int markets) {
	int totalNumberNodes = m*n*2 +2;

    for (int i = 0; i < totalNumberNodes; i++) {
        vector<Edge> arr;
        graph.push_back(arr);
    }

    //Ligar Vin a Vout
	for (int i = 1; i < m*n*2; i+=2) {
		graph[i].push_back(Edge(i+1, 1));
        graph[i+1].push_back(Edge(i,0));
	}

    //Ligar mercados a sink
	for (int i = 0; i < markets; i++) {
        int x,y;
        cin >> x >> y;
        int point = ((x-1) * 2) + (2* m* (y-1) + 1);

        if (findEdge(graph[point+1], totalNumberNodes-1).v == -1) {
            graph[point+1].push_back(Edge(totalNumberNodes-1, 1));
            graph[totalNumberNodes-1].push_back(Edge(point+1, 0));
        }
	}

    //Ligar source aos cidadaos
	for (int i = 0; i < citizens; i++) {
        int x,y;
        cin >> x >> y;
        int point = ((x-1) * 2) + (2* m* (y-1) + 1);

        if (findEdge(graph[0], point).v == -1) {
            graph[0].push_back(Edge(point, 1));
            graph[point].push_back(Edge(0, 0));
        }
		
	}

    //Right
	for (int i = 2; i < m*n*2+1; i+=2) {
		if (i % (2*m) != 0) {
			graph[i].push_back(Edge(i+1, 1));
            graph[i+1].push_back(Edge(i, 0));
		}
	}

    //Left
	for (int i = 2; i < m*n*2+1; i+=2) {
		if ((i-2) % (2*m) != 0) {
			graph[i].push_back(Edge(i-3, 1));
            graph[i-3].push_back(Edge(i, 0));
		}
	}

    //Up
	for (int i = 2; i < m*n*2+1; i+=2) {
		if (i - (2*m) > 0) {
			graph[i].push_back(Edge(i - (2 * m) - 1, 1));
            graph[i - (2*m) - 1].push_back(Edge(i, 0));
		}
	}

    //Down
	for (int i = 2; i < m*n*2 +1; i+=2) {
		if (i + (2*m) < m*n*2+1) {
			graph[i].push_back(Edge(i + (2*m) - 1, 1));
            graph[i + (2*m) - 1].push_back(Edge(i, 0));
		}
	}
	return edmondsKarp(0, totalNumberNodes-1, totalNumberNodes, citizens);
}

int main() 
{ 
    int m, n, super, people;
    cin >> m >> n >> super >> people;

    int points = m*n*2+2;
        
    for (int i = 0; i < points; i++) {parent.push_back(-1);}

    int maxFlow = escape(m,n,people, super);
  
    cout << maxFlow << endl;
  
    return 0;
}
