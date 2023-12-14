#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>

using namespace std;

class Graph {
    int numberOfNodes;
    vector<vector<int>> adjacencyList;

    void addEdge(int u, int v) {
        adjacencyList[u - 1].push_back(v - 1);
    }

public:
    Graph(int numberOfNodes) : numberOfNodes(numberOfNodes), adjacencyList(vector<vector<int>>(numberOfNodes, vector<int>())) {}

    Graph(int numberOfNodes, const vector<pair<int, int>>& edges) : numberOfNodes(numberOfNodes), adjacencyList(vector<vector<int>>(numberOfNodes, vector<int>())) {
        for (const auto& edge : edges) {
            addEdge(edge.first, edge.second);
        }
    }

    vector<int> sortVertices() {
        vector<int> in_degree(numberOfNodes, 0);
        for (int u = 0; u < numberOfNodes; ++u) {
            for (int v : adjacencyList[u]) {
                in_degree[v]++;
            }
        }

        queue<int> q;
        for (int u = 0; u < numberOfNodes; ++u) {
            if (in_degree[u] == 0) {
                q.push(u);
            }
        }

        vector<int> sorted_vertices;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            sorted_vertices.push_back(u);

            for (int v : adjacencyList[u]) {
                if (--in_degree[v] == 0) {
                    q.push(v);
                }
            }
        }

        return sorted_vertices;
    }

    vector<int> sortFAS() {
        vector<int> sorted_vertices = sortVertices();
        vector<int> feedback_arc_set;
        unordered_set<int> removed;

        for (int u : sorted_vertices) {
            for (int v : adjacencyList[u]) {
                if (!removed.count(v)) {
                    feedback_arc_set.push_back(v);
                    removed.insert(v);
                }
            }
        }

        return feedback_arc_set;
    }
};