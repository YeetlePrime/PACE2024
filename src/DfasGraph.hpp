#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>

using namespace std;

class DfasGraph {
    int numberOfNodes;
    vector<vector<int>> adjacencyList;

    void addEdge(int u, int v) {
        adjacencyList[u - 1].push_back(v - 1);
    }

public:
    DfasGraph(int numberOfNodes) : numberOfNodes(numberOfNodes), adjacencyList(vector<vector<int>>(numberOfNodes, vector<int>())) {}

    DfasGraph(int numberOfNodes, const vector<pair<int, int>>& edges) : numberOfNodes(numberOfNodes), adjacencyList(vector<vector<int>>(numberOfNodes, vector<int>())) {
        for (const auto& edge : edges) {
            addEdge(edge.first, edge.second);
        }
    }

    vector<int> sortNodes() {
        vector<int> inDegree(numberOfNodes, 0);
        for (int u = 0; u < numberOfNodes; ++u) {
            for (int v : adjacencyList[u]) {
                inDegree[v]++;
            }
        }

        queue<int> q;
        for (int u = 0; u < numberOfNodes; ++u) {
            if (inDegree[u] == 0) {
                q.push(u);
            }
        }

        vector<int> sortedNodes;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            sortedNodes.push_back(u);

            for (int v : adjacencyList[u]) {
                if (--inDegree[v] == 0) {
                    q.push(v);
                }
            }
        }

        return sortedNodes;
    }

    vector<int> sortFAS() {
        vector<int> sortedNodes = sortNodes();
        vector<int> feedbackArcSet;
        unordered_set<int> removedNodes;

        for (int u : sortedNodes) {
            for (int v : adjacencyList[u]) {
                if (!removedNodes.count(v)) {
                    feedbackArcSet.push_back(v);
                    removedNodes.insert(v);
                }
            }
        }

        return feedbackArcSet;
    }
};