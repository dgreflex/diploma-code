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


def generate_random_graph(num_vertices, coef):
    nodes = [generate_random_node(i, coef) for i in range(num_vertices)]
    edges = []

    for i in range(1, num_vertices):
        edges.append(generate_random_edge(i, 0, coef))

    return {"nodes": nodes, "edges": edges}


def save_graph_to_json(graph, filename):
    with open(filename, "w") as file:
        json.dump(graph, file, indent=4)


def gen_test(num_vertices, graphs_quantity):
    phys = generate_random_graph(num_vertices, 1)
    virts = [
        generate_random_graph(
            num_vertices - random.randint(0, num_vertices // 2), graphs_quantity * 2
        )
        for _ in range(graphs_quantity)
    ]

    filename = "test_{}.json".format(num_vertices)
    save_graph_to_json({"physical": phys, "virtuals": virts}, filename)


def main():
    # num_vertices = int(input()) case 4 one test
    # graphs_quantity = int(input())
    # gen_test(num_vertices, graphs_quantity)
    num_vertices = int(input("Input num vertices\n"))
    graph_quant_from_to = int(input("Input graph quantity\n"))
    gen_test(num_vertices, graph_quant_from_to)


if __name__ == "__main__":
    main()
