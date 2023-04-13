#ifndef PARSER_HPP
#define PARSER_HPP

#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "../graph/graph.hpp"

using json = nlohmann::json;

class JsonGraphParser
{
public:
    static JsonGraphParser *getInstance();

    std::pair<std::unique_ptr<Graph>, std::vector<std::unique_ptr<Graph>>> parseJsonToGraphs(const std::string &filename);

private:
    static JsonGraphParser *instance;

    JsonGraphParser() {}

    JsonGraphParser(const JsonGraphParser &) = delete;

    JsonGraphParser &operator=(const JsonGraphParser &) = delete;

    std::unique_ptr<Graph> parseJsonToGraph(const json &graphJson, std::string name);
};

#endif