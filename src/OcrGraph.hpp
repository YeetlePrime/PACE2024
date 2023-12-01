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

    void checkIdentifierOfFixedNode(size_t identifierOfFixedNode)
    {
        if (identifierOfFixedNode < 1 || identifierOfFixedNode > numberOfFixedNodes)
            throw std::invalid_argument("There is no fixed node with identifier \"" + std::to_string(identifierOfFixedNode) + "\".");
    }

    void checkIdentifierOfFreeNode(size_t identifierOfFreeNode)
    {
        if (identifierOfFreeNode <= numberOfFixedNodes || identifierOfFreeNode > numberOfFreeNodes + numberOfFixedNodes)
            throw std::invalid_argument("There is no free node with identifier \"" + std::to_string(identifierOfFreeNode) + "\".");
    }

    size_t getIndexForIdentifiers(size_t identifierOfFixedNode, size_t identifierOfFreeNode) {
        checkIdentifierOfFixedNode(identifierOfFixedNode);
        checkIdentifierOfFreeNode(identifierOfFreeNode);

        size_t indexOfFixedNode{identifierOfFixedNode - 1};
        size_t indexOfFreeNode{identifierOfFreeNode - numberOfFixedNodes - 1};
        return indexOfFixedNode + (indexOfFreeNode * numberOfFixedNodes);
    }

    bool addEdge(size_t identifierOfFixedNode, size_t identifierOfFreeNode)
    {
        size_t index{getIndexForIdentifiers(identifierOfFixedNode, identifierOfFreeNode)};

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
    OcrGraph(size_t numberOfFixedNodes, size_t numberOfFreeNodes) : numberOfFixedNodes(numberOfFixedNodes), numberOfFreeNodes(numberOfFreeNodes), numberOfEdges(0), edges({std::vector<bool>(numberOfFixedNodes * numberOfFreeNodes, false)}) {}
    OcrGraph(size_t numberOfFixedNodes, size_t numberOfFreeNodes, size_t expectedNumberOfEdges, const std::vector<std::pair<size_t, size_t>> &edgeEntries) : numberOfFixedNodes(numberOfFixedNodes), numberOfFreeNodes(numberOfFreeNodes), edges(std::vector<bool>(numberOfFixedNodes * numberOfFreeNodes, false)), numberOfEdges(addEdges(edgeEntries))
    {
        if (numberOfEdges != expectedNumberOfEdges)
            throw std::invalid_argument("The number of distinct edges in the list does not match the number of expected edges. (" + std::to_string(expectedNumberOfEdges) + " expected, but " + std::to_string(numberOfEdges) + " provided)");
    }

    bool getEntry(size_t identifierOfFixedNode, size_t identifierOfFreeNode)
    {
        size_t index{getIndexForIdentifiers(identifierOfFixedNode, identifierOfFreeNode)};

        return edges.at(index);
    }

    std::string to_string()
    {
        std::string result = "OcrGraph{\n\t#fixedNodes: " + std::to_string(numberOfFixedNodes) + "\n\t#freeNodes: " + std::to_string(numberOfFreeNodes) + "\n\t#edges: " + std::to_string(numberOfEdges) + "\n\tentries:\n";
    
        for (size_t fixedIdentifier{1}; fixedIdentifier <= numberOfFixedNodes; fixedIdentifier++) {
            result += "\t\t";
            for (size_t freeIdentifier{numberOfFixedNodes + 1}; freeIdentifier <= numberOfFixedNodes + numberOfFreeNodes; freeIdentifier++) {
                result += getEntry(fixedIdentifier, freeIdentifier) ? "1 " : "0 ";
            }
            result += "\n";
        }
        result += "}";

        return result;
    }
};

#endif