import random
import json

freqValue = [1000, 4000]
RAMValue = [4000, 16000]
hardValue = [20000, 40000]
capacityValue = [2000, 10000]
pingValue = [1, 100]


def generate_random_node(node_id, coef):
    return {
        "id": node_id,
        "coreFrequency": random.randint(freqValue[0] // coef, freqValue[1] // coef),
        "RAM": random.randint(RAMValue[0] // coef, RAMValue[1] // coef),
        "hardMemory": random.randint(hardValue[0] // coef, hardValue[1] // coef),
    }


def generate_random_edge(node_from, node_to, coef):
    return {
        "from": node_from,
        "to": node_to,
        "capacity": random.randint(capacityValue[0] // coef, capacityValue[1] // coef),
        "ping": random.randint(pingValue[0], pingValue[1]),
    }


def generate_dragonfly_topology(num_vertices, num_groups, num_switches, coef):
    total_switches = num_groups * num_switches

    nodes = [generate_random_node(i, coef) for i in range(num_vertices)]
    edges = []

    for group in range(num_groups):
        for switch in range(num_switches):
            for next_switch in range(switch + 1, num_switches):
                edges.append(
                    generate_random_edge(
                        group * num_switches + switch,
                        group * num_switches + next_switch,
                        coef,
                    )
                )

            for next_group in range(group + 1, num_groups):
                for next_switch in range(num_switches):
                    edges.append(
                        generate_random_edge(
                            group * num_switches + switch,
                            next_group * num_switches + next_switch,
                            coef,
                        )
                    )

    return {"nodes": nodes, "edges": edges}


def save_graph_to_json(graph, filename):
    with open(filename, "w") as file:
        json.dump(graph, file, indent=4)


def main():
    num_vertices = int(input("Введите количество вершин: "))
    num_groups = int(input())
    num_switches = int(input())
    num_graphs = int(input())
    dragonfly_topology = generate_dragonfly_topology(
        num_vertices, num_groups, num_switches, 1
    )
    virtuals = []
    for _ in range(num_graphs):
        virt = generate_dragonfly_topology(num_vertices, num_groups, num_switches, 10)
        virtuals.append(virt)

    filename = "test_{}.json".format(num_vertices)
    save_graph_to_json({"physical": dragonfly_topology, "virtuals": virtuals}, filename)
    


if __name__ == "__main__":
    main()
