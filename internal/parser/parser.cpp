#include "parser.hpp"

JsonGraphParser *JsonGraphParser::instance = nullptr;

std::unique_ptr<Graph> JsonGraphParser::parseJsonToGraph(const json &graphJson, std::string name)
{
    auto result = std::make_unique<Graph>(name);

    // Добавление вершин в граф
    const auto &vertices = graphJson["nodes"];
    for (const auto &vertex : vertices)
    {
        int id = vertex["id"];
        auto coreFrequencyJson = vertex["coreFrequency"];
        if (!coreFrequencyJson.is_number())
        {
            throw std::runtime_error("Invalid vertex coreFrequency: " +
                                     coreFrequencyJson.dump());
        }
        int coreFrequency = coreFrequencyJson;

        auto RAMJson = vertex["RAM"];
        if (!RAMJson.is_number())
        {
            throw std::runtime_error("Invalid vertex RAM: " +
                                     RAMJson.dump());
        }
        int RAM = RAMJson;

        auto hardMemoryJson = vertex["hardMemory"];
        if (!hardMemoryJson.is_number())
        {
            throw std::runtime_error("Invalid vertex hardMemory: " +
                                     hardMemoryJson.dump());
        }
        int hardMemory = hardMemoryJson;
        result->addVertex(id, coreFrequency, RAM, hardMemory);
    }

    // Добавление ребер в граф
    const auto &edges = graphJson["edges"];
    for (const auto &edge : edges)
    {
        int from = edge["from"];
        int to = edge["to"];
        auto capacityJson = edge["capacity"];
        if (!capacityJson.is_number())
        {
            throw std::runtime_error("Invalid edge capacity: " +
                                     capacityJson.dump());
        }
        int capacity = capacityJson;

        auto pingJson = edge["ping"];
        if (!pingJson.is_number())
        {
            throw std::runtime_error("Invalid edge ping: " +
                                     pingJson.dump());
        }
        int ping = pingJson;
        result->addEdge(from, to, capacity, ping);
    }

    return result;
}

std::pair<std::unique_ptr<Graph>, std::vector<std::unique_ptr<Graph>>> JsonGraphParser::parseJsonToGraphs(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Cannot open JSON file: " + filename);
    }

    json j;
    file >> j;

    auto physical = parseJsonToGraph(j["physical"], "physical");
    std::vector<std::unique_ptr<Graph>> virtuals;

    int i = 0;
    for (const auto &virtualGraph : j["virtuals"])
    {
        virtuals.push_back(this->parseJsonToGraph(virtualGraph, std::to_string(i++)));
    }
    return std::make_pair(std::move(physical), std::move(virtuals));
}

JsonGraphParser *JsonGraphParser::getInstance()
{
    if (instance == nullptr)
    {
        instance = new JsonGraphParser();
    }
    return instance;
}

