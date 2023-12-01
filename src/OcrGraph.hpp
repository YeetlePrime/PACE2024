#ifndef __OCR_GRAPH_HPP__
#define __OCR_GRAPH_HPP__

#include <iostream>
#include <vector>
#include <exception>

class OcrGraph
{
private:
    const size_t numberOfFixedNodes;
    const size_t numberOfFreeNodes;
    std::vector<bool> edges;
    const size_t numberOfEdges;

    bool addEdge(size_t identifierOfFixedNode, size_t identifierOfFreeNode)
    {
        if (identifierOfFixedNode < 1 || identifierOfFixedNode > numberOfFixedNodes)
            throw std::invalid_argument("There is no fixed node with identifier \"" + std::to_string(identifierOfFixedNode) + "\".");

        if (identifierOfFreeNode <= numberOfFixedNodes || identifierOfFreeNode > numberOfFreeNodes + numberOfFixedNodes)
            throw std::invalid_argument("There is no free node with identifier \"" + std::to_string(identifierOfFreeNode) + "\".");

        size_t indexOfFixedNode = identifierOfFixedNode - 1;
        size_t indexOfFreeNode = identifierOfFreeNode - numberOfFixedNodes - 1;
        size_t index = indexOfFixedNode + (indexOfFreeNode * numberOfFixedNodes);

        if (edges.at(index))
            return false;

        edges.at(index) = true;
        return true;
    }

    size_t addEdges(const std::vector<std::pair<size_t, size_t>> &edgeEntries)
    {
        size_t numberOfAddedEdges{0};

        for (auto entry : edgeEntries)
        {
            try
            {
                if (addEdge(entry.first, entry.second))
                    numberOfAddedEdges++;
            }
            catch (std::invalid_argument invalidArgument)
            {
            }
        }

        return numberOfAddedEdges;
    }

public:
    OcrGraph(size_t numberOfFixedNodes, size_t numberOfFreeNodes) : numberOfFixedNodes(numberOfFixedNodes), numberOfFreeNodes(numberOfFreeNodes), numberOfEdges(0), edges(std::vector<bool>(numberOfFixedNodes * numberOfFreeNodes, false)) {}
    OcrGraph(size_t numberOfFixedNodes, size_t numberOfFreeNodes, size_t expectedNumberOfEdges, const std::vector<std::pair<size_t, size_t>> &edgeEntries) : numberOfFixedNodes(numberOfFixedNodes), numberOfFreeNodes(numberOfFreeNodes), edges({}), numberOfEdges(addEdges(edgeEntries))
    {
        if (numberOfEdges != expectedNumberOfEdges)
            throw std::invalid_argument("The number of distinct edges in the list does not match the number of expected edges. (" + std::to_string(expectedNumberOfEdges) + " expected, but " + std::to_string(numberOfEdges) + "provided)");
    }
};

#endif