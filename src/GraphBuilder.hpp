#ifndef __GRAPH_BUILDER_HPP__
#define __GRAPH_BUILDER_HPP__

#include <iostream>
#include <fstream>

#include "OcrGraph.hpp"
#include "DfasGraph.hpp"

class GraphBuilder
{
private:
    struct GraphInfo
    {
        std::string problemDescriptor{ "" };
        size_t numberOfFixedNodes{ 0 };
        size_t numberOfFreeNodes{ 0 };
        size_t numberOfEdges{ 0 };
        std::vector<std::pair<size_t, size_t>> edges{};
    };

    static bool stringStartsWith(const std::string& string, char character) noexcept
    {
        if (string.empty())
            return false;

        return string.at(0) == character;
    }

    static bool stringEndsWith(const std::string& string, const std::string& endString) noexcept
    {
        if (string.size() < endString.size())
            return false;

        auto stringIter{ string.rbegin() };
        auto endStringIter{ endString.rbegin() };

        while (endStringIter != endString.rend() && stringIter != string.rend()) {
            if (*stringIter != *endStringIter)
                return false;
            stringIter++;
            endStringIter++;
        }

        return true;
    }

    static std::vector<std::string> splitString(const std::string& string, char delimiter) noexcept
    {
        if (string.empty())
            return {};

        std::vector<std::string> result{};
        auto current{ string.begin() };

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

    static void readPLine(const std::string& pLine, GraphInfo& ocrGraphInfo)
    {
        if (!stringStartsWith(pLine, 'p'))
            throw std::runtime_error("The p-line doesn't start with p.");

        auto strings{ splitString(pLine, ' ') };

        try
        {
            if (strings.size() != 5)
                throw;
            ocrGraphInfo.problemDescriptor = strings.at(1);
            ocrGraphInfo.numberOfFixedNodes = std::stol(strings.at(2));
            ocrGraphInfo.numberOfFreeNodes = std::stol(strings.at(3));
            ocrGraphInfo.numberOfEdges = std::stol(strings.at(4));
        }
        catch (...)
        {
            throw std::runtime_error("The p-line \"" + pLine + "\" is badly formatted.");
        }
    }

    static void readEdgeLine(const std::string& edgeLine, GraphInfo& ocrGraphInfo)
    {
        auto strings = splitString(edgeLine, ' ');
        std::pair<size_t, size_t> edgeInfo;

        try
        {
            if (strings.size() != 2)
                throw;

            edgeInfo.first = std::stol(strings.at(0));
            edgeInfo.second = std::stol(strings.at(1));

            ocrGraphInfo.edges.push_back(edgeInfo);
        }
        catch (...)
        {
            throw std::runtime_error("The edge-line \"" + edgeLine + "\" is badly formatted.");
        }
    }

public:
    static GraphInfo buildGraphInfoFromInputStream(std::istream& stream) {
        std::string line;

        // skip comment lines
        while (std::getline(stream, line) && (stringStartsWith(line, 'c')))
        {
        }

        // first non-comment line has to be a p-line
        if (!stringStartsWith(line, 'p'))
            throw std::runtime_error("The file doesn't start with a p line (after comments).");

        GraphInfo result;

        readPLine(line, result);

        while (std::getline(stream, line))
        {
            if (stringStartsWith(line, 'c'))
                continue;

            readEdgeLine(line, result);
        }

        return result;
    }

    static GraphInfo buildGraphInfoFromFile(const std::string& filepath) {
        if (!stringEndsWith(filepath, ".gr"))
            throw std::invalid_argument("The provided file has to be of type \".gr\".");

        std::ifstream file(filepath);
        if (file.fail())
            throw std::runtime_error("The file \"" + filepath + "\" does not exist.");

        return buildGraphInfoFromInputStream(file);
    }

    static OcrGraph buildOcrGraphFromFile(const std::string& filepath) {
        GraphInfo graphInfo{ buildGraphInfoFromFile(filepath) };

        return OcrGraph(graphInfo.numberOfFixedNodes, graphInfo.numberOfFreeNodes, graphInfo.edges);
    }

    static DfasGraph convertOcrGraphToGraph(const OcrGraph& ocrGraph) {
        return DfasGraph(ocrGraph.getNumberOfFixedNodes() + ocrGraph.getNumberOfFreeNodes(), ocrGraph.computeEdges());
    }
};

#endif