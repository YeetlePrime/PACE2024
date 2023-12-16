#include "GraphBuilder.hpp"

#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cout << "Invalid number of parameters." << std::endl;
        return EXIT_FAILURE;
    }

    std::string filepath(argv[0]);
    filepath = filepath.substr(0, filepath.size() - 8);
    filepath.append(argv[1]);

    OcrGraph ocrGraph{GraphBuilder::buildOcrGraphFromFile(filepath)};
    DfasGraph dfasGraph = GraphBuilder::convertOcrGraphToGraph(ocrGraph);
    
    std::cout << ocrGraph.to_string() << std::endl << "Computed Order: ";
    for (auto nodeIndex : dfasGraph.sortFAS()) {
        std::cout << nodeIndex + 1 << " ";
    }

    std::cout << std::endl;


    return EXIT_SUCCESS;
}