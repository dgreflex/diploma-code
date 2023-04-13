#ifndef MAPPING_GRAPHS_HPP
#define MAPPING_GRAPHS_HPP

#include "../graph/graph.hpp"
#include "../fibonacci_heap/max_heap.hpp"
#include <memory>

class PhysicalNetwork
{
public:
    std::unique_ptr<Graph> g;
    std::map<int, std::vector<std::shared_ptr<Vertex>>> mapped_vertices; // Физ узел -> список отображенных на него вирт узлов
    FibonacciHeap<std::shared_ptr<Vertex>, double> frequencyUsage;
    FibonacciHeap<std::shared_ptr<Vertex>, double> RAMUsage;
    FibonacciHeap<std::shared_ptr<Vertex>, double> memoryUsage;

    std::map<std::shared_ptr<Edge>, std::vector<std::shared_ptr<Edge>>> mapped_edges; // Физ ребро -> список отображенных на него вирт. ребер
    FibonacciHeap<std::shared_ptr<Edge>, double> capacityUsage;

    PhysicalNetwork(std::unique_ptr<Graph> g) : g(std::move(g))
    {
        std::function<bool(std::shared_ptr<Vertex> &, std::shared_ptr<Vertex> &)> frequencyCmp = [&](std::shared_ptr<Vertex> const &a, std::shared_ptr<Vertex> const &b)
        {
            return a->coreFrequency >= b->coreFrequency;
        };
        std::function<bool(std::shared_ptr<Vertex> &, std::shared_ptr<Vertex> &)> RAMCmp = [&](std::shared_ptr<Vertex> const &a, std::shared_ptr<Vertex> const &b)
        {
            return a->RAM >= b->RAM;
        };
        std::function<bool(std::shared_ptr<Vertex> &, std::shared_ptr<Vertex> &)> memoryCmp = [&](std::shared_ptr<Vertex> const &a, std::shared_ptr<Vertex> const &b)
        {
            return a->hardMemory >= b->hardMemory;
        };
        // std::function<bool(std::shared_ptr<Edge> &, std::shared_ptr<Edge> &)> capacityCmp = [&](std::shared_ptr<Edge> const &a, std::shared_ptr<Edge> const &b)
        // {
        //     return a->capacity >= b->capacity;
        // };

        std::vector<std::shared_ptr<Vertex>> vertices;
        for (auto [vertexID, vertex] : this->g->getVertices())
            vertices.push_back(vertex);

        std::sort(vertices.begin(), vertices.end(), frequencyCmp);
        for (auto &vertex : vertices)
            this->frequencyUsage.insert(1.0, vertex);

        std::sort(vertices.begin(), vertices.end(), memoryCmp);
        for (auto &vertex : vertices)
            this->memoryUsage.insert(1.0, vertex);

        std::sort(vertices.begin(), vertices.end(), RAMCmp);
        for (auto &vertex : vertices)
            this->RAMUsage.insert(1.0, vertex);

        // std::vector<std::shared_ptr<Edge>> edges;
        // for (auto [vertexID, vertex] : this->g->getVertices())
        //     for (auto [vertexEndID, edge] : this->g->getAdjacentVertices(vertexID))
        //         edges.push_back(edge);

        // std::sort(edges.begin(), edges.end(), capacityCmp);
        // for (auto &edge : edges)
        //     this->capacityUsage.insert(1.0, edge);
    }

    // TODO вычитание сразу из всех куч узлов
};

class VirtualNetwork
{
public:
    std::unique_ptr<Graph> g;
    std::map<int, int> mapped_nodes_on_physical_network; // Вирт узел -> Физ узел на который произошло отображение
    FibonacciHeap<std::shared_ptr<Vertex>, long long> frequency;
    FibonacciHeap<std::shared_ptr<Vertex>, long long> RAM;
    FibonacciHeap<std::shared_ptr<Vertex>, long long> memory;

    std::map<std::shared_ptr<Edge>, std::vector<std::shared_ptr<Edge>>> mapped_edges_on_physical_network; // Вирт ребро -> Физ ребро
    // FibonacciHeap<std::shared_ptr<Edge>, long long> capacity;

    bool vertexIsEmpty() const
    {
        return this->frequency.is_empty() && this->memory.is_empty() && this->RAM.is_empty();
    }

    VirtualNetwork(std::unique_ptr<Graph> g) : g(std::move(g))
    {
        for (auto [vertexID, vertex] : this->g->getVertices())
        {
            this->frequency.insert(vertex->coreFrequency, vertex);
            this->RAM.insert(vertex->RAM, vertex);
            this->memory.insert(vertex->hardMemory, vertex);

            // for (auto [vertexEndID, edge] : this->g->getAdjacentVertices(vertexID))
            //     this->capacity.insert(edge->capacity, edge);
        }
    }
};

#endif