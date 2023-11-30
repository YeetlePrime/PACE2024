#include "Graph.hpp"

int main(int argc, char* argv[]) {
    Graph graph = Graph::loadFromFile("test.gr");

    std::cout << graph.to_string() << std::endl;
    
    return EXIT_SUCCESS;
}