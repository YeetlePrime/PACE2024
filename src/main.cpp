#include "GraphBuilder.hpp"

#include <unistd.h>
#include <filesystem>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Invalid number of parameters." << std::endl;
        return EXIT_FAILURE;
    }

    std::string baseFilePath{ argv[0] };
    baseFilePath = baseFilePath.substr(0, baseFilePath.size() - 8);

    for (const auto& entry : std::filesystem::directory_iterator(baseFilePath + argv[1])) {
        OcrGraph ocrGraph{ GraphBuilder::buildOcrGraphFromFile(entry.path()) };
        DfasGraph dfasGraph = GraphBuilder::convertOcrGraphToGraph(ocrGraph);

        std::cout << "----- " << entry.path() << " ----------" << std::endl
            << ocrGraph.to_string() << std::endl 
            << dfasGraph.to_string() << std::endl
            << "Computed Order: ";

        auto newOrder = dfasGraph.sortFAS();
        std::vector<size_t> newOrderAsSizeT{};
        for (auto& nodeIndex : newOrder) {
            newOrderAsSizeT.push_back(nodeIndex + 1);
            std::cout << nodeIndex + 1 << " ";
        }
        ocrGraph.setOrderingOfFreeNodes(std::move(newOrderAsSizeT));
        std::cout << std::endl
            << "Crossings with new order: " << ocrGraph.computeNumberOfCrossings() << std::endl
            << "-----------------------------------------------" << std::endl;

    }



    return EXIT_SUCCESS;
}