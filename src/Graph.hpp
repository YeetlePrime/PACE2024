#ifndef __GRAPH_HPP__
#define __GRAPH_HPP__

#include <iostream>
#include <vector>
#include <fstream>

class Graph
{
private:
    Graph(const std::vector<bool> &graph, size_t numberOfFixedNodes, size_t numberOfFreeNodes, size_t numberOFEdges) : graph(graph), numberOfFixedNodes(numberOfFixedNodes), numberOfFreeNodes(numberOfFreeNodes), numberOfEdges(numberOFEdges) {}
    Graph(std::vector<bool> &&graph, size_t numberOfFixedNodes, size_t numberOfFreeNodes, size_t numberOFEdges) : graph(std::move(graph)), numberOfFixedNodes(numberOfFixedNodes), numberOfFreeNodes(numberOfFreeNodes), numberOfEdges(numberOFEdges) {}

    std::vector<bool> graph;
    size_t numberOfFixedNodes;
    size_t numberOfFreeNodes;
    size_t numberOfEdges;

    struct PLineInfo
    {
        std::string problemDescriptor;
        size_t numberOfFixedNodes;
        size_t numberOfFreeNodes;
        size_t numberOfEdges;

        std::string to_string() const
        {
            return "descriptor: " + problemDescriptor + ", #fixed: " + std::to_string(numberOfFixedNodes) + ", #free: " + std::to_string(numberOfFreeNodes) + ", #edges: " + std::to_string(numberOfEdges);
        }
    };

    static PLineInfo readPLine(const std::string &pLine)
    {
        if (!stringStartsWith(pLine, 'p'))
            throw std::runtime_error("The p-line doesn't start with p.");

        auto strings = splitString(pLine, ' ');
        PLineInfo result;

        try
        {
            if (strings.size() != 5)
                throw;
            result.problemDescriptor = strings.at(1);
            result.numberOfFixedNodes = std::stol(strings.at(2));
            result.numberOfFreeNodes = std::stol(strings.at(3));
            result.numberOfEdges = std::stol(strings.at(4));
        }
        catch (...)
        {
            throw std::runtime_error("The p-line \"" + pLine + "\" is badly formatted.");
        }

        return result;
    }

    struct EdgeLineInfo
    {
        size_t fixedNodeIndex;
        size_t freeNodeIndex;

        std::string to_string() const
        {
            return "fixedIndex: " + std::to_string(fixedNodeIndex) + ", freeIndex: " + std::to_string(freeNodeIndex);
        }
    };

    static EdgeLineInfo readEdgeLine(const std::string &edgeLine)
    {
        auto strings = splitString(edgeLine, ' ');
        EdgeLineInfo result;

        try
        {
            if (strings.size() != 2)
                throw;

            result.fixedNodeIndex = std::stol(strings.at(0));
            result.freeNodeIndex = std::stol(strings.at(1));
        }
        catch (...)
        {
            throw std::runtime_error("The edge-line \"" + edgeLine + "\" is badly formatted.");
        }

        return result;
    }

    static bool filepathIsGraphFile(const std::string &filepath) noexcept
    {
        if (filepath.length() < 4)
            return false;

        return (0 == filepath.compare(filepath.length() - 3, 3, ".gr"));
    }

    static bool stringStartsWith(const std::string &string, char character) noexcept
    {
        if (string.empty())
            return false;

        return string.at(0) == character;
    }

    static std::vector<std::string> splitString(const std::string &string, char delimiter) noexcept
    {
        if (string.empty())
            return {};

        std::vector<std::string> result = {};
        auto current = string.begin();

        // load word by word
        while (current != string.end())
        {
            std::string nextString("");
            while (current != string.end() && *current != delimiter)
            {
                nextString.push_back(*current);
                current++;
            }
            if (!nextString.empty())
                result.push_back(nextString);

            if (current != string.end())
                current++;
        }

        return result;
    }

public:
    static Graph loadFromFile(const std::string &filepath)
    {
        if (!filepathIsGraphFile(filepath))
            throw std::invalid_argument("The file has to be a .gr file.");

        std::ifstream file(filepath);

        if (file.fail())
            throw std::runtime_error("The file \"" + filepath + "\" does not exist.");

        std::string line;

        // skip comment lines
        while (std::getline(file, line) && stringStartsWith(line, 'c'))
        {
            std::cout << "read line: " << line << std::endl;
        }

        if (!stringStartsWith(line, 'p'))
            throw std::runtime_error("The file doesn't start with a p line (after comments).");

        auto pLineInfo = readPLine(line);

        std::cout << "read line: " << line << std::endl
                  << pLineInfo.to_string() << std::endl;

        std::vector<bool> entries(pLineInfo.numberOfFixedNodes * pLineInfo.numberOfFreeNodes);

        size_t numberOfInsertedEdges = 0;
        while (std::getline(file, line) && numberOfInsertedEdges < pLineInfo.numberOfEdges)
        {
            if (stringStartsWith(line, 'c'))
                continue;

            auto edgeLineInfo{readEdgeLine(line)};
            if (edgeLineInfo.fixedNodeIndex > pLineInfo.numberOfFixedNodes || edgeLineInfo.fixedNodeIndex < 1 || edgeLineInfo.freeNodeIndex - pLineInfo.numberOfFixedNodes > pLineInfo.numberOfFreeNodes || edgeLineInfo.freeNodeIndex < pLineInfo.numberOfFixedNodes) {
                throw std::runtime_error("An index of an edge-line is out of bounds.");
            }
            entries.at(edgeLineInfo.fixedNodeIndex - 1 + (edgeLineInfo.freeNodeIndex - pLineInfo.numberOfFreeNodes - 1) * pLineInfo.numberOfFreeNodes) = true;
        }

        return Graph(entries, pLineInfo.numberOfFreeNodes, pLineInfo.numberOfFreeNodes, pLineInfo.numberOfEdges);
    }

    std::string to_string() const {
        std::string result = "";
        for (size_t freeIter{0}; freeIter < numberOfFreeNodes; freeIter++) {
            for (size_t fixedIter{0}; fixedIter < numberOfFixedNodes; fixedIter++) {
                result.push_back(graph.at(freeIter + (fixedIter * numberOfFreeNodes)) ? '1' : '0');
                result.push_back(' ');
            }
            result.push_back('\n');
        }
        return result;
    }
};

#endif