#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <iostream>
#include <map>
#include <vector>
#include <utility>
#include <graphviz/gvc.h>
#include <graphviz/cgraph.h>
#include <sstream>
#include <iomanip>
#include <memory>
#include <queue>
#include <limits>
#include <unordered_map>
#include <algorithm>

class Vertex
{
public:
    int id = 0;
    long long coreFrequency;
    long long RAM;
    long long hardMemory;
    Vertex(long long coreFrequency = 0, long long RAM = 0, long long hardMemory = 0, int id = 0) : coreFrequency(coreFrequency), RAM(RAM), hardMemory(hardMemory), id(id) {}

    std::string toString() const;

    bool operator<(const std::shared_ptr<Vertex> &other) const;
    bool operator==(const std::shared_ptr<Vertex> &other) const;
    bool operator<=(const std::shared_ptr<Vertex> &other) const;
    bool operator>(const std::shared_ptr<Vertex> &other) const;
    bool operator>=(const std::shared_ptr<Vertex> &other) const;
    bool operator!=(const std::shared_ptr<Vertex> &other) const;
};

class Edge
{
public:
    long long capacity;
    long long ping;
    int from;
    int to;
    Edge(long long capacity = 0, long long ping = 0, int from = 0, int to = 0) : capacity(capacity), ping(ping), from(from), to(to) {}

    bool operator<(const std::shared_ptr<Edge> &other) const;
    bool operator==(const std::shared_ptr<Edge> &other) const;
    bool operator<=(const std::shared_ptr<Edge> &other) const;
    bool operator>(const std::shared_ptr<Edge> &other) const;
    bool operator>=(const std::shared_ptr<Edge> &other) const;
    bool operator!=(const std::shared_ptr<Edge> &other) const;
};

class Graph
{
private:
    std::string name;
    std::map<int, std::shared_ptr<Vertex>> vertices;
    std::map<int, std::vector<std::pair<int, std::shared_ptr<Edge>>>> adjacencyList;

public:
    Graph(std::string name) : name(name) {}
    Graph() : Graph("") {}

    void addVertex(int id, long long coreFrequency = 0, long long RAM = 0, long long hardMemory = 0);

    void addEdge(int from, int to, long long capacity = 0, long long ping = 0);

    std::shared_ptr<Vertex> getVertex(int id);

    std::vector<std::pair<int, std::shared_ptr<Edge>>> getAdjacentVertices(int id);

    std::map<int, std::vector<std::pair<int, std::shared_ptr<Edge>>>> getEdges() const;

    std::shared_ptr<Edge> getEdge(const int from, const int to);

    std::map<int, std::shared_ptr<Vertex>> getVertices() const;

    // Функция для получения списка смежности графа
    std::map<int, std::vector<std::pair<int, std::shared_ptr<Edge>>>> getAdjacencyList() const;

    const std::string &getName() const;

    void visualizeGraph(const std::string &outputFilename) const;

    class GraphShortestPathResult
    {
    public:
        int source;
        int target;
        double distance;
        std::vector<int> path;

        GraphShortestPathResult(int source, int target, double distance, const std::vector<int> &path)
            : source(source), target(target), distance(distance), path(path) {}

        bool empty() const
        {
            return path.empty();
        }
    };

    GraphShortestPathResult findShortestPath(int source, int target)
    {
        if (source == target)
        {
            return GraphShortestPathResult(source, target, 0, {source});
        }

        std::unordered_map<int, double> distances;
        std::unordered_map<int, int> previous;
        std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> queue;

        for (const auto &vertex : vertices)
        {
            distances[vertex.first] = std::numeric_limits<double>::infinity();
        }

        distances[source] = 0;
        queue.push({0, source});

        while (!queue.empty())
        {
            int current = queue.top().second;
            queue.pop();

            if (current == target)
            {
                break;
            }

            for (const auto &neighbor : adjacencyList[current])
            {
                int neighbor_id = neighbor.first;
                double new_distance = distances[current] + neighbor.second->capacity;

                if (new_distance < distances[neighbor_id])
                {
                    distances[neighbor_id] = new_distance;
                    previous[neighbor_id] = current;
                    queue.push({new_distance, neighbor_id});
                }
            }
        }

        if (previous.find(target) == previous.end())
        {
            throw std::runtime_error("No path found between the source and target vertices.");
        }

        std::vector<int> path;
        int current_vertex = target;
        while (current_vertex != source)
        {
            path.push_back(current_vertex);
            current_vertex = previous[current_vertex];
        }
        path.push_back(source);
        std::reverse(path.begin(), path.end());

        return GraphShortestPathResult(source, target, distances[target], path);
    }
};

#endif