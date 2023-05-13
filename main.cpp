#include "internal/parser/parser.hpp"
#include "internal/graph/graph.hpp"
#include "internal/fibonacci_heap/max_heap.hpp"
#include "internal/networks/mapping_graphs.hpp"
#include "internal/solver/solver.hpp"
#include "sys/types.h"
#include "sys/sysinfo.h"
#include <chrono>

double calc_accuracy(PhysicalNetwork &physical);
double calculate_mean(const std::vector<double> &values);
double calculate_standard_deviation(const std::vector<double> &values);

int main(int argc, char *argv[])
{
    // std::string filename1 = "/home/reflex/Desktop/Diplom/code/tests/diploma.json";
    // auto parser1 = JsonGraphParser::getInstance();
    // auto [physical1, virtuals1] = parser1->parseJsonToGraphs(filename1);
    // physical1->visualizeGraph(physical1->getName() + ".png");

    // return 0;

    // std::string filename = "/home/reflex/Desktop/Diplom/code/tests/full_connect/test_9_2.json";

    auto parser = JsonGraphParser::getInstance();
    if (argc < 3)
    {
        std::cerr << "Not enough arguments" << std::endl;
        return 1;
    }

    auto [physical, virtuals] = parser->parseJsonToGraphs(std::string(argv[1]));
    bool doVisual = static_cast<int>(std::stoi(argv[2]));

    auto t_start = std::chrono::high_resolution_clock::now();
    auto physicalNetwork = PhysicalNetwork(std::move(physical));
    std::vector<VirtualNetwork> virtualNetworks;
    for (auto &v : virtuals)
        virtualNetworks.push_back(VirtualNetwork(std::move(v)));
    auto solver = NetworkSolver::getInstance();
    solver->solve(physicalNetwork, virtualNetworks);
    auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << calc_accuracy(physicalNetwork) << std::endl
              << std::chrono::duration<double, std::milli>(t_end - t_start).count() << std::endl;

    if (doVisual)
        physicalNetwork.g->visualizeGraph(physicalNetwork.g->getName() + ".png");
    for (auto &v : virtualNetworks)
    {
        if (doVisual)
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

double calc_accuracy(PhysicalNetwork &physical)
{
    double accuracy = 0;
    std::vector<double> fraq;
    for (int physNum = 0; physNum < physical.g->getVertices().size(); physNum++)
    {
        if (physical.mapped_vertices.find(physNum) == physical.mapped_vertices.end())
        {
            fraq.push_back(0.0);
            continue;
        }
        auto virtuals = physical.mapped_vertices[physNum];
        long long cpu, RAM, hard;
        cpu = 0;
        RAM = 0;
        hard = 0;
        for (auto &v : virtuals)
        {
            cpu += v->coreFrequency;
            RAM += v->RAM;
            hard += v->hardMemory;
        }
        double cpuFraq = (double)cpu / physical.g->getVertex(physNum)->coreFrequency;
        double RAMFraq = (double)RAM / physical.g->getVertex(physNum)->RAM;
        double hardFraq = (double)hard / physical.g->getVertex(physNum)->hardMemory;
        fraq.push_back((cpuFraq + RAMFraq + hardFraq) / 3);
    }

    return calculate_standard_deviation(fraq);
}

double calculate_mean(const std::vector<double> &values)
{
    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return sum / values.size();
}

double calculate_standard_deviation(const std::vector<double> &values)
{
    double mean = calculate_mean(values);
    double sum_of_squares = 0.0;

    for (const auto &value : values)
    {
        sum_of_squares += std::pow(value - mean, 2);
    }

    double variance = sum_of_squares / values.size();
    return std::sqrt(variance);
}
