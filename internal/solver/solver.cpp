#include "solver.hpp"

NetworkSolver *NetworkSolver::instance = nullptr;

NetworkSolver *NetworkSolver::getInstance()
{
    if (instance == nullptr)
    {
        instance = new NetworkSolver();
    }
    return instance;
}

void NetworkSolver::solve(PhysicalNetwork &physicalNetwork, std::vector<VirtualNetwork> &virtualNetworks)
{
    for (auto &virtualNetwork : virtualNetworks)
    {
        int step = 0;
        while (!virtualNetwork.vertexIsEmpty())
        {
            std::shared_ptr<Vertex> virtualNode;
            switch (step)
            {
            case 0:
                virtualNode = virtualNetwork.frequency.get_max_value();
                break;
            case 1:
                virtualNode = virtualNetwork.RAM.get_max_value();
                break;
            case 2:
                virtualNode = virtualNetwork.memory.get_max_value();
                break;
            default:
                std::runtime_error("NetworkSolver::solve error");
            }
            auto t1 = virtualNetwork.frequency.get_text_debug();
            auto t2 = virtualNetwork.RAM.get_text_debug();
            auto t3 = virtualNetwork.memory.get_text_debug();

            auto fHeapNode = virtualNetwork.frequency.find_node_by_value(virtualNode);
            auto rHeapNode = virtualNetwork.RAM.find_node_by_value(virtualNode);
            auto mHeapNode = virtualNetwork.memory.find_node_by_value(virtualNode);
            virtualNetwork.frequency.delete_node(fHeapNode);
            virtualNetwork.RAM.delete_node(rHeapNode);
            virtualNetwork.memory.delete_node(mHeapNode);

            bool found = false;
            auto fHeap = physicalNetwork.frequencyUsage;
            auto rHeap = physicalNetwork.RAMUsage;
            auto mHeap = physicalNetwork.memoryUsage;
            std::function<double(FibonacciHeap<std::shared_ptr<Vertex>, double> &, FibonacciHeap<std::shared_ptr<Vertex>, double> &, FibonacciHeap<std::shared_ptr<Vertex>, double> &)> heapMax;
            heapMax = [&](FibonacciHeap<std::shared_ptr<Vertex>, double> &a, FibonacciHeap<std::shared_ptr<Vertex>, double> &b, FibonacciHeap<std::shared_ptr<Vertex>, double> &c) -> double
            {
                return std::max(std::max(a.get_max(), b.get_max()), c.get_max());
            };
            while (!found && !(fHeap.is_empty() || rHeap.is_empty() || mHeap.is_empty()))
            {
                std::shared_ptr<Vertex> physicalNode;
                if (fHeap.get_max() == heapMax(fHeap, rHeap, mHeap))
                {
                    physicalNode = fHeap.get_max_value();
                    if (virtualNode->coreFrequency > ((long long)physicalNode->coreFrequency * fHeap.get_max()) || virtualNode->RAM > ((long long)physicalNode->RAM * fHeap.get_max()) || virtualNode->hardMemory > ((long long)physicalNode->hardMemory * fHeap.get_max()))
                    {
                        fHeap = fHeap.copy_without_max();
                        continue;
                    }
                }
                else if (rHeap.get_max() == heapMax(fHeap, rHeap, mHeap))
                {
                    physicalNode = rHeap.get_max_value();
                    if (virtualNode->coreFrequency > ((long long)physicalNode->coreFrequency * rHeap.get_max()) || virtualNode->RAM > ((long long)physicalNode->RAM * rHeap.get_max()) || virtualNode->hardMemory > ((long long)physicalNode->hardMemory * rHeap.get_max()))
                    {
                        rHeap = rHeap.copy_without_max();
                        continue;
                    }
                }
                else
                {
                    physicalNode = mHeap.get_max_value();
                    if (virtualNode->coreFrequency > ((long long)physicalNode->coreFrequency * mHeap.get_max()) || virtualNode->RAM > ((long long)physicalNode->RAM * mHeap.get_max()) || virtualNode->hardMemory > ((long long)physicalNode->hardMemory * mHeap.get_max()))
                    {
                        mHeap = mHeap.copy_without_max();
                        continue;
                    }
                }

                // Отображаем виртуальную вершину на физическую вершину
                physicalNetwork.mapped_vertices[physicalNode->id].push_back(virtualNode);
                virtualNetwork.mapped_nodes_on_physical_network[virtualNode->id] = physicalNode->id;

                // Обновляем веса вершин физической сети
                auto fPNode = physicalNetwork.frequencyUsage.find_node_by_value(physicalNode);
                auto rPNode = physicalNetwork.RAMUsage.find_node_by_value(physicalNode);
                auto mPNode = physicalNetwork.memoryUsage.find_node_by_value(physicalNode);
                physicalNetwork.frequencyUsage.decrease_key(fPNode, fPNode->key - (double) virtualNode->coreFrequency / physicalNode->coreFrequency);
                physicalNetwork.RAMUsage.decrease_key(rPNode, rPNode->key - (double) virtualNode->RAM / physicalNode->RAM);
                physicalNetwork.memoryUsage.decrease_key(mPNode, mPNode->key - (double) virtualNode->hardMemory / physicalNode->hardMemory);

                found = true;
                break;
            }
            if (!found)
            {
                std::runtime_error("Cannot find solution");
                return;
            }
            step = (step + 1) % 3;
        }

        // Отображаем ребра виртуальной сети на ребра физической сети
        for (const auto &[from, virtualEdgeWithToPairs] : virtualNetwork.g->getEdges())
        {
            for (const auto &[to, virtualEdge] : virtualEdgeWithToPairs)
            {
                // Находим отображенные вершины в физической сети
                auto physicalSrc = virtualNetwork.mapped_nodes_on_physical_network[from];
                auto physicalDst = virtualNetwork.mapped_nodes_on_physical_network[to];

                // Ищем кратчайший путь между отображенными вершинами
                auto shortestPath = physicalNetwork.g->findShortestPath(physicalSrc, physicalDst);

                if (shortestPath.empty())
                {
                    std::runtime_error("Cannot find solution");
                    return;
                }

                // Отображаем ребро виртуальной сети на ребра физической сети
                for (size_t i = 0; i < shortestPath.path.size() - 1; ++i)
                {
                    auto pathFrom = shortestPath.path[i];
                    auto pathTo = shortestPath.path[i + 1];
                    auto physicalEdge = physicalNetwork.g->getEdge(pathFrom, pathTo);
                    if (physicalEdge == nullptr) {
                        physicalEdge = physicalNetwork.g->getEdge(pathTo, pathFrom);
                    }
                    physicalNetwork.mapped_edges[physicalEdge].push_back(virtualEdge);
                    virtualNetwork.mapped_edges_on_physical_network[virtualEdge].push_back(physicalEdge);

                    // Обновляем веса ребер физической сети
                    auto physicalVirtualHeapNode = physicalNetwork.capacityUsage.find_node_by_value(physicalEdge);
                    physicalNetwork.capacityUsage.decrease_key(physicalVirtualHeapNode, physicalVirtualHeapNode->key - virtualEdge->capacity / physicalEdge->capacity);
                }
            }
        }
    }
}