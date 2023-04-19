#include "internal/parser/parser.hpp"
#include "internal/graph/graph.hpp"
#include "internal/fibonacci_heap/max_heap.hpp"
#include "internal/networks/mapping_graphs.hpp"
#include "internal/solver/solver.hpp"

int main(int argc, char *argv[])
{
    // std::string filename1 = "/home/reflex/Desktop/Diplom/code/tests/diploma.json";
    // auto parser1 = JsonGraphParser::getInstance();
    // auto [physical1, virtuals1] = parser1->parseJsonToGraphs(filename1);
    // physical1->visualizeGraph(physical1->getName() + ".png");

    // return 0;

    // std::string filename = "/home/reflex/Desktop/Diplom/code/tests/full_connect/test_5.json";
    auto parser = JsonGraphParser::getInstance();
    if (argc < 2)
    {
        std::cerr << "Not enough arguments" << std::endl;
        return 1;
    }

    auto [physical, virtuals] = parser->parseJsonToGraphs(std::string(argv[1]));

    auto physicalNetwork = PhysicalNetwork(std::move(physical));
    std::vector<VirtualNetwork> virtualNetworks;
    for (auto &v : virtuals)
        virtualNetworks.push_back(VirtualNetwork(std::move(v)));

    auto solver = NetworkSolver::getInstance();
    solver->solve(physicalNetwork, virtualNetworks);
    physicalNetwork.g->visualizeGraph(physicalNetwork.g->getName() + ".png");
    for (auto &v : virtualNetworks)
    {
        v.g->visualizeGraph(v.g->getName() + ".png");
        std::cout << "Virtual Graph: " << v.g->getName() << std::endl;
        std::cout << "Nodes:" << std::endl;
        for (auto &[virtNodeMap, physNodeMap] : v.mapped_nodes_on_physical_network)
            std::cout << "From: " << virtNodeMap << " to: " << physNodeMap << std::endl;
        std::cout << "Edges:" << std::endl;
        for (auto &[from, toVector] : v.mapped_edges_on_physical_network)
            for (auto &to : toVector)
                std::cout << "From: " << from->from << " " << from->to << " to: " << to->from << " " << to->to << std::endl;
        std::cout << std::endl;
    }

    return 0;
}