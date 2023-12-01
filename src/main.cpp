#include "OcrGraphBuilder.hpp"
#include "OcrGraph.hpp"

int main(int argc, char* argv[]) {
    OcrGraph graph{OcrGraphBuilder::buildFromFile("/mnt/c/Users/Jonas/Desktop/dev/PACE2024/test.gr")};

    std::cout << graph.to_string() << std::endl;
    
    return EXIT_SUCCESS;
}