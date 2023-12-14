#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class DfasGraph {
private:
    int numberOfNodes;
    vector<vector<int>> adjacencyList;

    void addEdge(int node1, int node2) {
        adjacencyList[node1 - 1].push_back(node2 - 1);
    }

public:
    DfasGraph(int numberOfNodes)
        : numberOfNodes(numberOfNodes)
        , adjacencyList(std::vector<std::vector<int>>(numberOfNodes, std::vector<int>())) {
    }

    DfasGraph(size_t numberOfNodes, const std::vector<std::pair<int, int>>& edges)
        : numberOfNodes(numberOfNodes)
        , adjacencyList(std::vector<std::vector<int>>(numberOfNodes, std::vector<int>())) {
        for (const auto& edge : edges) {
            addEdge(edge.first, edge.second);
        }
    }

    
    std::vector<int> sortVertices() {
        std::vector<int> in_degree(V, 0);
        for (int u = 0; u < V; ++u) {
            for (int v : adj[u]) {
                in_degree[v]++;
            }
        }

        std::queue<int> q;
        for (int u = 0; u < V; ++u) {
            if (in_degree[u] == 0) {
                q.push(u);
            }
        }

        std::vector<int> sorted_vertices;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            sorted_vertices.push_back(u);

            for (int v : adj[u]) {
                if (--in_degree[v] == 0) {
                    q.push(v);
                }
            }
        }

        return sorted_vertices;
    }

    std::vector<int> sortFAS() {
        std::vector<int> sorted_vertices = sortVertices();
        std::vector<int> feedback_arc_set;
        std::unordered_set<int> removed;

        for (int u : sorted_vertices) {
            for (int v : adj[u]) {
                if (!removed.count(v)) {
                    feedback_arc_set.push_back(v);
                    removed.insert(v);
                }
            }
        }

        return feedback_arc_set;
    }
};