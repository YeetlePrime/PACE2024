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

    OcrGraph graph{GraphBuilder::buildOcrGraphFromFile(filepath)};

    std::cout << graph.to_string() << std::endl;

    graph.setOrderingOfFreeNodes({5, 7, 6, 8});

    std::cout << graph.to_string() << std::endl;

    return EXIT_SUCCESS;
}