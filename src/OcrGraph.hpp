#ifndef __OCR_GRAPH_HPP__
#define __OCR_GRAPH_HPP__

#include <iostream>
#include <vector>
#include <exception>
#include <algorithm>

class OcrGraph
{
private:
    const size_t numberOfFixedNodes;
    const size_t numberOfFreeNodes;
    std::vector<std::vector<size_t>> adjacencyList;
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

    size_t getIndexForFixedNode(size_t identifierOfFixedNode) {
        checkIdentifierOfFixedNode(identifierOfFixedNode);
        return --identifierOfFixedNode;
    }

    size_t getIndexForIdentifiers(size_t identifierOfFixedNode, size_t identifierOfFreeNode) {
        checkIdentifierOfFixedNode(identifierOfFixedNode);
        checkIdentifierOfFreeNode(identifierOfFreeNode);

        size_t indexOfFixedNode{identifierOfFixedNode - 1};
        size_t indexOfFreeNode{identifierOfFreeNode - numberOfFixedNodes - 1};
        return indexOfFixedNode + (indexOfFreeNode * numberOfFixedNodes);
    }

    bool vectorContainsElement(const std::vector<size_t>& vector, size_t element) {
        return std::find(vector.begin(), vector.end(), element) != vector.end();
    }

    bool addEdge(size_t identifierOfFixedNode, size_t identifierOfFreeNode)
    {
        checkIdentifierOfFixedNode(identifierOfFixedNode);
        checkIdentifierOfFreeNode(identifierOfFreeNode);
        size_t indexOfFixedNode = getIndexForFixedNode(identifierOfFixedNode);

        auto& neighboursOfFixedNode{adjacencyList.at(indexOfFixedNode)};

        if (vectorContainsElement(neighboursOfFixedNode, identifierOfFreeNode)) 
            return false;

        neighboursOfFixedNode.push_back(identifierOfFreeNode);
        
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
            catch (std::invalid_argument& invalidArgument)
            {
            }
        }

        return numberOfAddedEdges;
    }

public:
    OcrGraph(size_t numberOfFixedNodes, size_t numberOfFreeNodes) : numberOfFixedNodes(numberOfFixedNodes), numberOfFreeNodes(numberOfFreeNodes), adjacencyList({std::vector<std::vector<size_t>>(numberOfFixedNodes, std::vector<size_t>())}), numberOfEdges(0) {}
    OcrGraph(size_t numberOfFixedNodes, size_t numberOfFreeNodes, size_t expectedNumberOfEdges, const std::vector<std::pair<size_t, size_t>> &edgeEntries) : numberOfFixedNodes(numberOfFixedNodes), numberOfFreeNodes(numberOfFreeNodes), adjacencyList({std::vector<std::vector<size_t>>(numberOfFixedNodes, std::vector<size_t>())}), numberOfEdges(addEdges(edgeEntries))
    {
        if (numberOfEdges != expectedNumberOfEdges)
            throw std::invalid_argument("The number of distinct edges in the list does not match the number of expected edges. (" + std::to_string(expectedNumberOfEdges) + " expected, but " + std::to_string(numberOfEdges) + " provided)");
    }

    std::string to_string()
    {
        std::string result = "OcrGraph{\n\t#fixedNodes: " + std::to_string(numberOfFixedNodes) + "\n\t#freeNodes: " + std::to_string(numberOfFreeNodes) + "\n\t#edges: " + std::to_string(numberOfEdges) + "\n\tentries:\n";
    
        for (size_t fixedIdentifier{1}; fixedIdentifier <= numberOfFixedNodes; fixedIdentifier++) {
            result += "\t\t";
            result = result + std::to_string(fixedIdentifier) + ": ";
            size_t indexOfFixedNode = getIndexForFixedNode(fixedIdentifier);
            auto& neighboursOfFixedNode{adjacencyList.at(indexOfFixedNode)};

            for (auto& entry : neighboursOfFixedNode) {
                result = result + std::to_string(entry) + " ";
            }


            result += "\n";
        }
        result += "}";

        return result;
    }
};

#endif