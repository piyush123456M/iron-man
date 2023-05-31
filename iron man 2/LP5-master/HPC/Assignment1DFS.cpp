#include <iostream>
#include <vector>
#include <stack>
#include <omp.h>

using namespace std;

// Graph class representing a directed graph using adjacency list representation
class Graph {
    int numNodes; // Number of nodes in the graph
    vector<vector<int> > adjList; // Adjacency list

public:
    Graph(int numNodes) : numNodes(numNodes), adjList(numNodes) {}

    // Function to add an edge to the graph
    void addEdge(int src, int dest) {
        adjList[src].push_back(dest);
    }

    // Function for depth-first search
    void DFS(int startNode) {
        vector<bool> visited(numNodes, false); // Mark all nodes as not visited

        stack<int> st; // Create a stack for DFS
        st.push(startNode);

        while (!st.empty()) {
            int current;
            #pragma omp critical
            {
                current = st.top(); // Get the top element from the stack
                st.pop(); // Remove the top element from the stack
            }

            if (!visited[current]) { // If the current node has not been visited
                #pragma omp critical
                {
                    if (!visited[current]) { // Re-check visited status after critical section
                        cout << current << " "; // Print the current node
                        visited[current] = true; // Mark the current node as visited
                    }
                }

                // Get all adjacent nodes of the current node
                // If an adjacent node has not been visited, push it to the stack
                #pragma omp parallel for
                for (int i = 0; i < adjList[current].size(); ++i) {
                    int adjNode = adjList[current][i];
                    if (!visited[adjNode]) {
                        #pragma omp critical
                        {
                            if (!visited[adjNode]) { // Re-check visited status after critical section
                                st.push(adjNode); // Push the adjacent node to the stack
                            }
                        }
                    }
                }
            }
        }
    }
};

int main() {
    int numNodes, numEdges, startNode;
    cout << "Enter the number of nodes: ";
    cin >> numNodes;

    cout << "Enter the number of edges: ";
    cin >> numEdges;

    Graph graph(numNodes);

    cout << "Enter the edges (source node, destination node):\n";
    for (int i = 0; i < numEdges; ++i) {
        int src, dest;
        cin >> src >> dest;
        graph.addEdge(src, dest);
    }

    cout << "Enter the starting node: ";
    cin >> startNode;

    cout << "DFS traversal starting from node " << startNode << ": ";
    graph.DFS(startNode);
    cout << endl;

    return 0;
}
