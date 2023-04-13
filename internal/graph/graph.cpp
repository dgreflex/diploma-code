#include "graph.hpp"

bool Vertex::operator<(const std::shared_ptr<Vertex> &other) const
{
    return this->coreFrequency < other->coreFrequency && this->RAM < other->RAM && this->hardMemory < other->hardMemory;
}
bool Vertex::operator==(const std::shared_ptr<Vertex> &other) const
{
    return this->coreFrequency == other->coreFrequency && this->RAM == other->RAM && this->hardMemory == other->hardMemory;
}
bool Vertex::operator<=(const std::shared_ptr<Vertex> &other) const
{
    return *this < other || *this == other;
}
bool Vertex::operator>(const std::shared_ptr<Vertex> &other) const
{
    return !(*this <= other);
}
bool Vertex::operator>=(const std::shared_ptr<Vertex> &other) const
{
    return !(*this < other);
}
bool Vertex::operator!=(const std::shared_ptr<Vertex> &other) const
{
    return !(*this == other);
}

bool Edge::operator<(const std::shared_ptr<Edge> &other) const
{
    return this->capacity < other->capacity;
}
bool Edge::operator==(const std::shared_ptr<Edge> &other) const
{
    return this->capacity == other->capacity;
}
bool Edge::operator<=(const std::shared_ptr<Edge> &other) const
{
    return *this < other || *this == other;
}
bool Edge::operator>(const std::shared_ptr<Edge> &other) const
{
    return !(*this <= other);
}
bool Edge::operator>=(const std::shared_ptr<Edge> &other) const
{
    return !(*this < other);
}
bool Edge::operator!=(const std::shared_ptr<Edge> &other) const
{
    return !(*this == other);
}

std::string Vertex::toString() const
{
    std::stringstream result;

    result << std::setprecision(0) << std::scientific << static_cast<double>(coreFrequency) << " Hz" << std::endl
           << static_cast<double>(RAM) << " bytes" << std::endl
           << static_cast<double>(hardMemory) << " bytes" << std::endl;

    return result.str();
}

void Graph::addVertex(int id, long long coreFrequency, long long RAM, long long hardMemory)
{
    vertices[id] = std::make_shared<Vertex>(coreFrequency, RAM, hardMemory, id);
    adjacencyList[id] = std::vector<std::pair<int, std::shared_ptr<Edge>>>();
}

void Graph::addEdge(int from, int to, long long capacity, long long ping)
{
    if (vertices.count(from) == 0 || vertices.count(to) == 0)
    {
        std::cerr << "Error: Invalid vertex ID" << std::endl;
        return;
    }
    adjacencyList[from].push_back(std::make_pair(to, std::make_shared<Edge>(capacity, ping, from, to)));
}

std::shared_ptr<Edge> Graph::getEdge(const int from, const int to)
{
    if (this->adjacencyList.find(from) != this->adjacencyList.end())
    {
        auto toEdge = this->adjacencyList[from];
        for (auto [toVector, edge] : toEdge)
            if (to == toVector)
                return edge;
    }

    return nullptr;
}

std::shared_ptr<Vertex> Graph::getVertex(int id)
{
    if (vertices.count(id) == 0)
    {
        std::cerr << "Error: Invalid vertex ID" << std::endl;
        return nullptr;
    }
    return vertices[id];
}

std::vector<std::pair<int, std::shared_ptr<Edge>>> Graph::getAdjacentVertices(int id)
{
    if (adjacencyList.count(id) == 0)
    {
        std::cerr << "Error: Invalid vertex ID" << std::endl;
        return std::vector<std::pair<int, std::shared_ptr<Edge>>>();
    }
    return adjacencyList[id];
}

std::map<int, std::shared_ptr<Vertex>> Graph::getVertices() const
{
    return vertices;
}

std::map<int, std::vector<std::pair<int, std::shared_ptr<Edge>>>> Graph::getAdjacencyList() const
{
    return adjacencyList;
}

const std::string &Graph::getName() const
{
    return this->name;
}

std::map<int, std::vector<std::pair<int, std::shared_ptr<Edge>>>> Graph::getEdges() const {
    return this->adjacencyList;
}

void Graph::visualizeGraph(const std::string &outputFilename) const
{
    GVC_t *gvc;
    Agraph_t *G;
    Agnode_t *nodes[100];
    Agedge_t *edge;

    gvc = gvContext();
    G = agopen((char *)"G", Agstrictundirected, NULL);

    agsafeset(G, (char *)"label", (char *)this->getName().c_str(), (char *)"");
    agsafeset(G, (char *)"labelloc", (char *)"t", (char *)"");  // Устанавливаем расположение метки заголовка сверху
    agsafeset(G, (char *)"fontsize", (char *)"20", (char *)""); // Устанавливаем размер шрифта заголовка

    // Добавление вершин в граф
    for (const auto &vertexPair : this->getVertices())
    {
        int id = vertexPair.first;
        std::string label = "v" + std::to_string(id) + "\n" + vertexPair.second->toString();
        nodes[id] = agnode(G, (char *)std::to_string(id).c_str(), 1);
        agsafeset(nodes[id], "color", "red", "");
        agset(nodes[id], (char *)"label", (char *)label.c_str());
    }

    // Добавление ребер в граф
    for (const auto &adjacencyPair : this->getAdjacencyList())
    {
        int from = adjacencyPair.first;
        for (const auto &edgePair : adjacencyPair.second)
        {
            int to = edgePair.first;
            int capacity = edgePair.second->capacity;
            std::string label = std::to_string(capacity);
            edge = agedge(G, nodes[from], nodes[to], (char *)(std::to_string(from) + ":" + std::to_string(to)).c_str(), 1);
            agset(edge, (char *)"label", (char *)label.c_str());
            agset(edge, (char *)"fontsize", (char *)"14");
        }
    }

    // Визуализация графа в файл
    gvLayout(gvc, G, "dot");
    gvRenderFilename(gvc, G, "png", (char *)outputFilename.c_str());

    // Очистка
    gvFreeLayout(gvc, G);
    agclose(G);
    gvFreeContext(gvc);
}
