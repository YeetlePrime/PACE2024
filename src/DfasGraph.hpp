#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_set>
#include <sstream>

using namespace std;

class DfasGraph {
    size_t numberOfNodes;
    vector<vector<int>> adjacencyList;

    void addEdge(int u, int v) {
        adjacencyList[u - 1].push_back(v - 1);
    }

public:
    DfasGraph(size_t numberOfNodes) : numberOfNodes(numberOfNodes), adjacencyList(vector<vector<int>>(numberOfNodes, vector<int>())) {}

    DfasGraph(size_t numberOfNodes, const vector<pair<int, int>>& edges) : numberOfNodes(numberOfNodes), adjacencyList(vector<vector<int>>(numberOfNodes, vector<int>())) {
        for (const auto& edge : edges) {
            addEdge(edge.first, edge.second);
        }
    }

    vector<int> sortNodes() {
        vector<int> inDegree(numberOfNodes, 0);
        for (int u = 0; u < static_cast<int>(numberOfNodes); ++u) {
            for (int v : adjacencyList[u]) {
                inDegree[v]++;
            }
        }

        queue<int> q;
        for (int u = 0; u < static_cast<int>(numberOfNodes); ++u) {
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

    size_t computeNumberOfEdges() const {
        size_t result{ 0 };

        for (const auto& neighboursOfFixedNode : adjacencyList) {
            result += neighboursOfFixedNode.size();
        }

        return result;
    }

    std::string to_string() const {
        std::stringstream result{};

        result << "OcrGraph{"
            << "\n\t#nodes: " << numberOfNodes
            << "\n\t#edges: "
            << "\n\tadjacencyList: ";

        for (size_t identifierOfNode{ 1 }; identifierOfNode <= numberOfNodes; identifierOfNode++) {
            result << "\n\t\t" << identifierOfNode << " -> ";
            for (auto indexOfNeighbour : adjacencyList.at(identifierOfNode - 1)) {
                result << indexOfNeighbour + 1 << " ";
            }
        }
        result << "\n}";

        return result.str();
    }
};