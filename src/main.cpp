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
            << "Computed Order: ";
        for (auto nodeIndex : dfasGraph.sortFAS()) {
            std::cout << nodeIndex + 1 << " ";
        }
        std::cout << std::endl
            << "-----------------------------------------------" << std::endl;

    }



    return EXIT_SUCCESS;
}