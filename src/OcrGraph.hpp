#ifndef __OCR_GRAPH_HPP__
#define __OCR_GRAPH_HPP

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <sstream>

class OcrGraph {
private:
    size_t numberOfFixedNodes;
    size_t numberOfFreeNodes;

    size_t numberOfEdges;

    std::vector<std::vector<size_t>> adjacencyList;
    std::vector<size_t> orderingOfFreeNodes;

    static std::vector<std::vector<size_t>> newEmptyAdjacencyList(size_t numberOfFixedNodes) {
        return std::vector<std::vector<size_t>>(numberOfFixedNodes, std::vector<size_t>());
    }

    static std::vector<size_t> newInitialOrdering(size_t numberOfFixedNodes, size_t numberOfFreeNodes) {
        std::vector<size_t> result{};
        for (size_t i{ 0 }; i < numberOfFreeNodes; i++) {
            result.push_back(i + numberOfFixedNodes + 1);
        }
        return result;
    }

    void checkIdentifierOfFixedNode(size_t identifierOfFixedNode) const noexcept(false) {
        if (identifierOfFixedNode < 1 || identifierOfFixedNode > numberOfFixedNodes) {
            throw std::out_of_range(
                "The identifier of the fixed Node has to be between 1 and "
                + std::to_string(numberOfFixedNodes)
                + ", but was "
                + std::to_string(identifierOfFixedNode)
                + "."
            );
        }
    }

    void checkIdentifierOfFreeNode(size_t identifierOfFreeNode) const noexcept(false) {
        if (identifierOfFreeNode <= numberOfFixedNodes || identifierOfFreeNode > numberOfFixedNodes + numberOfFreeNodes) {
            throw std::out_of_range(
                "The identifier of the free Node has to be between "
                + std::to_string(numberOfFixedNodes + 1)
                + " and "
                + std::to_string(numberOfFixedNodes + numberOfFreeNodes)
                + ", but was "
                + std::to_string(identifierOfFreeNode)
                + "."
            );
        }
    }

    const std::vector<size_t>& getNeighboursOfFixedNode(size_t identifierOfFixedNode) const noexcept(false) {
        checkIdentifierOfFixedNode(identifierOfFixedNode);
        return adjacencyList.at(identifierOfFixedNode - 1);
    }

    std::vector<size_t>& getNeighboursOfFixedNode(size_t identifierOfFixedNode) noexcept(false) {
        checkIdentifierOfFixedNode(identifierOfFixedNode);
        return adjacencyList.at(identifierOfFixedNode - 1);
    }

    bool addEdge(size_t identifierOfFixedNode, size_t identifierOfFreeNode) {
        checkIdentifierOfFixedNode(identifierOfFixedNode);
        checkIdentifierOfFreeNode(identifierOfFreeNode);

        auto& neighboursOfFixedNode = getNeighboursOfFixedNode(identifierOfFixedNode);

        if (edgeExists(identifierOfFixedNode, identifierOfFreeNode)) {
            return false;
        }

        numberOfEdges++;
        neighboursOfFixedNode.push_back(identifierOfFreeNode);

        return true;
    }

public:
    OcrGraph(size_t numberOfFixedNodes, size_t numberOfFreeNodes) noexcept
        : numberOfFixedNodes(numberOfFixedNodes)
        , numberOfFreeNodes(numberOfFreeNodes)
        , numberOfEdges(0)
        , adjacencyList(newEmptyAdjacencyList(numberOfFixedNodes))
        , orderingOfFreeNodes(newInitialOrdering(numberOfFixedNodes, numberOfFreeNodes)) {

    }

    OcrGraph(size_t numberOfFixedNodes, size_t numberOfFreeNodes, const std::vector<std::pair<size_t, size_t>>& edges) noexcept(false)
        : numberOfFixedNodes(numberOfFixedNodes)
        , numberOfFreeNodes(numberOfFreeNodes)
        , numberOfEdges(0)
        , adjacencyList(newEmptyAdjacencyList(numberOfFixedNodes))
        , orderingOfFreeNodes(newInitialOrdering(numberOfFixedNodes, numberOfFreeNodes)) {
        for (const auto& edge : edges) {
            addEdge(edge.first, edge.second);
        }
    }

    bool edgeExists(size_t identifierOfFixedNode, size_t identifierOfFreeNode) const {
        checkIdentifierOfFixedNode(identifierOfFixedNode);
        checkIdentifierOfFreeNode(identifierOfFreeNode);

        const auto& neighboursOfFixedNode = getNeighboursOfFixedNode(identifierOfFixedNode);
        return std::find(neighboursOfFixedNode.begin(), neighboursOfFixedNode.end(), identifierOfFreeNode) != neighboursOfFixedNode.end();
    }

    size_t computeNumberOfCrossings() const {
        size_t crossings = 0;

        // Get edges from the graph
        std::vector<std::pair<int, int>> edges = computeEdges();

        for (size_t i = 0; i < edges.size(); ++i) {
            for (size_t j = i + 1; j < edges.size(); ++j) {
                // Check if edges (i, j) and (j, i) cross
                int xi = edges[i].first;
                int yi = edges[i].second;
                int xj = edges[j].first;
                int yj = edges[j].second;

                if ((xi < xj && yi > yj) || (xi > xj && yi < yj)) {
                    crossings++;
                }
            }
        }

        return crossings;
    }

    std::string to_string() const {
        std::stringstream result{};

        result << "OcrGraph{"
            << "\n\t#fixedNodes: " << numberOfFixedNodes
            << "\n\t#freeNodes: " << numberOfFreeNodes
            << "\n\t#edges: " << numberOfEdges
            << "\n\t#crossings:" << computeNumberOfCrossings()
            << "\n\tfreeOrdering: ";


        for (auto identifierOfFreeNode : orderingOfFreeNodes) {
            result << identifierOfFreeNode << " ";
        }

        result << "\n\tadjacencyList: ";

        for (size_t identifierOfFixedNode{ 1 }; identifierOfFixedNode <= numberOfFixedNodes; identifierOfFixedNode++) {
            result << "\n\t\t" << identifierOfFixedNode << " -> ";
            for (auto identifierOfFreeNode : getNeighboursOfFixedNode(identifierOfFixedNode)) {
                result << identifierOfFreeNode << " ";
            }
        }
        result << "\n}";

        return result.str();
    }

    void setOrderingOfFreeNodes(std::vector<size_t>&& ordering) {
        orderingOfFreeNodes = std::move(ordering);
    }

    size_t getNumberOfFixedNodes() const {
        return numberOfFixedNodes;
    }

    size_t getNumberOfFreeNodes() const {
        return numberOfFreeNodes;
    }

    std::vector<std::pair<int, int>> computeEdges() const {
        std::vector<std::pair<int, int>> edges;
        for (int fixedNode{ 1 }; fixedNode <= static_cast<int>(numberOfFixedNodes); fixedNode++) {
            for (auto freeNode : getNeighboursOfFixedNode(fixedNode)) {
                edges.push_back({ fixedNode, freeNode });
            }
        }

        return edges;
    }
};

#endif