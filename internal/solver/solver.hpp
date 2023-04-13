#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "../networks/mapping_graphs.hpp"

class NetworkSolver {
public:
    static NetworkSolver *getInstance();

    void solve(PhysicalNetwork &physicalNetwork, std::vector<VirtualNetwork> &virtualNetworks);

private:
    static NetworkSolver *instance;

    NetworkSolver() {}

    NetworkSolver(const NetworkSolver &) = delete;

    NetworkSolver &operator=(const NetworkSolver &) = delete;
};

#endif