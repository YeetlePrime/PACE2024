#ifndef __OCR_GRAPH_HPP__
#define __OCR_GRAPH_HPP__

#include <vector>
#include <stdexcept>
#include <algorithm>
#include <queue>

class OcrGraph
{
private:
    const size_t numberOfFixedNodes;
    const size_t numberOfFreeNodes;

    std::vector<std::vector<size_t>> adjacencyList;

    void checkIdentifierForFixedNode(size_t identifierForFixedNode) const noexcept(false) {
        if (identifierForFixedNode < 1 || identifierForFixedNode > numberOfFixedNodes)
        {
            throw std::out_of_range(
                "The identifier for the fixed node has to be between 1 and "
                + std::to_string(numberOfFixedNodes)
                + ", but was "
                + std::to_string(identifierForFixedNode)
                + "."
            );
        }
    }

    void checkIdentifierForFreeNode(size_t identifierForFreeNode) const noexcept(false) {
        if (identifierForFreeNode < numberOfFixedNodes + 1 || identifierForFreeNode > numberOfFixedNodes + numberOfFreeNodes)
        {
            throw std::out_of_range(
                "The identifier for the fixed node has to be between "
                + std::to_string(numberOfFixedNodes + 1)
                + " and "
                + std::to_string(numberOfFixedNodes + numberOfFreeNodes)
                + ", but was "
                + std::to_string(identifierForFreeNode)
                + "."
            );
        }
    }

    bool edgeExists(size_t identifierForFixedNode, size_t identifierForFreeNode) const noexcept(false) {
        checkIdentifierForFixedNode(identifierForFixedNode);
        checkIdentifierForFreeNode(identifierForFreeNode);

        size_t indexForFixedNode = identifierForFixedNode - 1;

        const std::vector<size_t>& neighboursOfFixedNode{ adjacencyList.at(indexForFixedNode) };

        return std::find(neighboursOfFixedNode.begin(), neighboursOfFixedNode.end(), identifierForFreeNode) != neighboursOfFixedNode.end();
    }

    bool addEdge(size_t identifierForFixedNode, size_t identifierForFreeNode) noexcept(false) {
        checkIdentifierForFixedNode(identifierForFixedNode);
        checkIdentifierForFreeNode(identifierForFreeNode);

        if (edgeExists(identifierForFixedNode, identifierForFreeNode))
        {
            return false;
        }

        adjacencyList.at(identifierForFixedNode - 1).push_back(identifierForFreeNode);
        return true;
    }

    public:
    OcrGraph(size_t numberOfFixedNodes, size_t numberOfFreeNodes) noexcept
        : numberOfFixedNodes(numberOfFixedNodes)
        , numberOfFreeNodes(numberOfFreeNodes)
        , adjacencyList(std::vector<std::vector<size_t>>(numberOfFixedNodes, std::vector<size_t>()))
    {
    }

    OcrGraph(size_t numberOfFixedNodes, size_t numberOfFreeNodes, const std::vector<std::pair<size_t, size_t>>& edges)
        : numberOfFixedNodes(numberOfFixedNodes)
        , numberOfFreeNodes(numberOfFreeNodes)
        , adjacencyList(std::vector<std::vector<size_t>>(numberOfFixedNodes, std::vector<size_t>()))
    {
        for (auto& edge : edges) {
            if (!addEdge(edge.first, edge.second)) {
                throw std::invalid_argument(
                    "Each edge has to be unique, but two of the same edges were given. ("
                    + std::to_string(edge.first)
                    + " -> "
                    + std::to_string(edge.second)
                    + ")"
                );
            }
        }
    }

    std::string to_string() const
    {
        std::string result = "OcrGraph{\n\t#fixedNodes: " + std::to_string(numberOfFixedNodes) + "\n\t#freeNodes: " + std::to_string(numberOfFreeNodes) + "\n\tadjacencyList:";

        size_t currentFixedIdentifier{ 1 };
        for (const auto& neighboursOfFixedNode : adjacencyList) {
            result = result + "\n\t\t" + std::to_string(currentFixedIdentifier) + ": ";

            for (const auto& neighbour : neighboursOfFixedNode) {
                result = result + std::to_string(neighbour) + " ";
            }

            currentFixedIdentifier++;
        }

        result += "\n}";

        return result;
    }

};

#endif