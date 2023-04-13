#include "internal/parser/parser.hpp"
#include "internal/graph/graph.hpp"
#include "internal/fibonacci_heap/max_heap.hpp"
#include "internal/networks/mapping_graphs.hpp"
#include "internal/solver/solver.hpp"

int main(int argc, char *argv[])
{
    auto parser = JsonGraphParser::getInstance();
    if (argc < 2)
    {
        std::cerr << "Not enough arguments" << std::endl;
        return 1;
    }

    auto [physical, virtuals] = parser->parseJsonToGraphs(argv[1]);

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

    // Graph my_graph;
    // my_graph.addVertex(1);
    // my_graph.addVertex(2);
    // my_graph.addVertex(3);
    // my_graph.addVertex(5);
    // my_graph.addEdge(1, 2, 10);
    // my_graph.addEdge(1, 3, 20);
    // my_graph.addEdge(2, 5, 15);
    // my_graph.addEdge(3, 5, 7);
    // my_graph.addVertex(4);
    // my_graph.addEdge(5, 4, 1);

    // int source_vertex_id = 1;
    // int target_vertex_id = 4;

    // try
    // {
    //     Graph::GraphShortestPathResult result = my_graph.findShortestPath(source_vertex_id, target_vertex_id);
    //     std::cout << "Shortest path from vertex " << source_vertex_id << " to vertex " << target_vertex_id << " has distance " << result.distance << " and follows the vertices: ";
    //     for (int vertex_id : result.path)
    //     {
    //         std::cout << vertex_id << " ";
    //     }
    //     std::cout << std::endl;
    // }
    // catch (const std::runtime_error &e)
    // {
    //     std::cerr << "Error: " << e.what() << std::endl;
    // }
    // FibonacciHeap<int, int> fib_heap;
    // fib_heap.insert(8, 1);
    // fib_heap.insert(4, 2);
    // fib_heap.insert(1, 3);
    // fib_heap.insert(7, 4);

    // auto node_to_delete = fib_heap.find_node_by_value(2);
    // if (node_to_delete)
    // {
    //     fib_heap.delete_node(node_to_delete);
    //     std::cout << "Node with value 2 deleted." << std::endl;
    // }
    // else
    // {
    //     std::cout << "Node with value 2 not found" << std::endl;
    // }

    // while(!fib_heap.is_empty()) {
    //     std::cout << fib_heap.get_max() << " " << fib_heap.get_max_value() << std::endl;
    //     fib_heap.delete_max();
    // }

    return 0;
}