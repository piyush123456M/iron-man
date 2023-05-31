#include <iostream>
#include <queue>
#include <vector>
#include <omp.h>

using namespace std;

// Graph class representing a directed graph using adjacency list representation
class Graph {
    int numNodes; // Number of nodes in the graph
    vector<vector<int>> adjList; // Adjacency list

public:
    Graph(int numNodes) : numNodes(numNodes), adjList(numNodes) {}

    // Function to add an edge to the graph
    void addEdge(int src, int dest) {
        adjList[src].push_back(dest);
    }

    // Function for breadth-first search
    void BFS(int startNode) {
        vector<bool> visited(numNodes, false); // Mark all nodes as not visited

        queue<int> q; // Create a queue for BFS
        visited[startNode] = true; // Mark the current node as visited and enqueue it
        q.push(startNode);

        while (!q.empty()) {
            int current = q.front(); // Dequeue a node from the queue and print it
            cout << current << " ";
            q.pop();

            // Get all adjacent nodes of the dequeued node
            // If an adjacent node has not been visited, mark it as visited and enqueue it
            #pragma omp parallel for
            for (int i = 0; i < adjList[current].size(); ++i) {
                int adjNode = adjList[current][i];
                if (!visited[adjNode]) {
                    visited[adjNode] = true;
                    q.push(adjNode);
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

    cout << "BFS traversal starting from node " << startNode << ": ";
    graph.BFS(startNode);
    cout << endl;

    return 0;
}
