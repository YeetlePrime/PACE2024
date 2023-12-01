#ifndef __OCR_GRAPH_BUILDER_HPP__
#define __OCR_GRAPH_BUILDER_HPP__

#include <iostream>

#include "OcrGraph.hpp"

class OcrGraphBuilder
{
private:
    struct OcrGraphInfo
    {
        std::string problemDescriptor = "";
        size_t numberOfFixedNodes = 0;
        size_t numberOfFreeNodes = 0;
        size_t numberOfEdges = 0;
        std::vector<std::pair<size_t, size_t>> edgeEntries = {};
    };

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

        std::vector<std::string> result{};
        auto current{string.begin()};

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

    static void readPLine(const std::string &pLine, OcrGraphInfo &ocrGraphInfo)
    {
        if (!stringStartsWith(pLine, 'p'))
            throw std::runtime_error("The p-line doesn't start with p.");

        auto strings = splitString(pLine, ' ');

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

    static void readEdgeLine(const std::string &edgeLine, OcrGraphInfo ocrGraphInfo)
    {
        auto strings = splitString(edgeLine, ' ');
        std::pair<size_t, size_t> edgeInfo;

        try
        {
            if (strings.size() != 2)
                throw;

            edgeInfo.first = std::stol(strings.at(0));
            edgeInfo.first = std::stol(strings.at(1));

            ocrGraphInfo.edgeEntries.push_back(edgeInfo);
        }
        catch (...)
        {
            throw std::runtime_error("The edge-line \"" + edgeLine + "\" is badly formatted.");
        }
    }

public:
    static OcrGraph buildFromInputStream(std::istream &stream)
    {
        std::string line;

        // skip comment lines
        while (std::getline(stream, line) && stringStartsWith(line, 'c'))
        {
        }

        // first non-comment line has to be a p-line
        if (!stringStartsWith(line, 'p'))
            throw std::runtime_error("The file doesn't start with a p line (after comments).");

        OcrGraphInfo ocrGraphInfo;

        readPLine(line, ocrGraphInfo);

        std::vector<bool> entries(ocrGraphInfo.numberOfFixedNodes * ocrGraphInfo.numberOfFreeNodes);

        while (std::getline(stream, line))
        {
            if (stringStartsWith(line, 'c'))
                continue;

            readEdgeLine(line, ocrGraphInfo);
        }

        return OcrGraph(ocrGraphInfo.numberOfFixedNodes, ocrGraphInfo.numberOfFreeNodes, ocrGraphInfo.numberOfEdges, ocrGraphInfo.edgeEntries);
    }
};

#endif