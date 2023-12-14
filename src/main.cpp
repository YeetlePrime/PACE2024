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

    Graph graph{GraphBuilder::buildGraphFromFile(filepath)};

    std::cout << "Direted Feedback Arc Set (FAS): ";
    for (auto node : graph.sortFAS()) {
        std::cout << node + 1 << " ";
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}