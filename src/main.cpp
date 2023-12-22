#include "GraphBuilder.hpp"

#include <unistd.h>
#include <filesystem>
#include <chrono>

int main(int argc, char* argv[])
{
    if (argc != 2) {
        std::cout << "Invalid number of parameters." << std::endl;
        return EXIT_FAILURE;
    }

    std::string baseFilePath{ argv[0] };
    baseFilePath = baseFilePath.substr(0, baseFilePath.size() - 8);

    std::cout << std::endl << std::endl;

    for (const auto& entry : std::filesystem::directory_iterator(baseFilePath + argv[1])) {
        auto begin{ std::chrono::high_resolution_clock::now() }, end{ std::chrono::high_resolution_clock::now() };

        std::cout << "Reading Graph from file \"" << entry.path() << "\"...";

        OcrGraph ocrGraph{ GraphBuilder::buildOcrGraphFromFile(entry.path()) };
        DfasGraph dfasGraph = GraphBuilder::convertOcrGraphToGraph(ocrGraph);

        end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsedMilliseconds{ end - begin };

        std::cout << " done. (" << elapsedMilliseconds.count() << " ms)" << std::endl
            << "Computing free Node ordering for least crossings...";

        begin = std::chrono::high_resolution_clock::now();

        auto ordering = dfasGraph.sortFAS();
        std::vector<size_t> newOrderAsSizeT{};
        for (auto& nodeIndex : ordering) {
            newOrderAsSizeT.push_back(nodeIndex + 1);
        }
        ocrGraph.setOrderingOfFreeNodes(std::move(newOrderAsSizeT));

        end = std::chrono::high_resolution_clock::now();
        elapsedMilliseconds = end - begin;

        std::cout << " done. (" << elapsedMilliseconds.count() << " ms)" << std::endl
            << "Ordering: ";

        for (const auto& node : ocrGraph.getOrderingOfFreeNodes()) {
            std::cout << node << " ";
        }

        std::cout << std::endl << "Computing number of crossings with new ordering...";
        begin = std::chrono::high_resolution_clock::now();

        auto numberOfCrossings = ocrGraph.computeNumberOfCrossings();

        end = begin = std::chrono::high_resolution_clock::now();
        elapsedMilliseconds = end - begin;

        std::cout << " done. (" << elapsedMilliseconds.count() << " ms)" << std::endl
            << "Number of Crossings: " << numberOfCrossings << std::endl << std::endl;
    }



    return EXIT_SUCCESS;
}