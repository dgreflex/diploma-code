import json
import math
from operator import itemgetter
import timeit

def mean(numbers):
    return sum(numbers) / len(numbers)

def standard_deviation(numbers):
    avg = mean(numbers)
    variance = sum((x - avg) ** 2 for x in numbers) / len(numbers)
    return math.sqrt(variance)

def find_all_mappings(physical_graph, virtual_graphs):
    best_accuracy = 1
    physical_nodes = physical_graph["nodes"]
    all_virt_nodes = []
    for i in virtual_graphs:
        all_virt_nodes += i["nodes"]
    align = [0 for _ in range(len(all_virt_nodes))]
    accuracies = []
    aligns = []
    while True:
        cpu = [0 for _ in range(len(physical_nodes))]
        ram = [0 for _ in range(len(physical_nodes))]
        mem = [0 for _ in range(len(physical_nodes))]
        for i in range(len(align)):
            cpu[align[i]] += all_virt_nodes[i]["coreFrequency"]
            ram[align[i]] += all_virt_nodes[i]["RAM"]
            mem[align[i]] += all_virt_nodes[i]["hardMemory"]

        need_to_skip = False
        fraq = [0 for _ in range(len(physical_nodes))]
        for i in range(len(physical_nodes)):
            pcpu = physical_nodes[i]["coreFrequency"]
            if pcpu < cpu[i]:
                need_to_skip = True
                break
            
            pram = physical_nodes[i]["RAM"]
            if pram < ram[i]:
                need_to_skip = True
                break
            
            pmem = physical_nodes[i]["hardMemory"]
            if pmem < mem[i]:
                need_to_skip = True
                break
            fraq[i] = ((cpu[i] / pcpu + ram[i] / pram + mem[i] / pmem) / 3)
        if not need_to_skip:
            accuracies.append(standard_deviation(fraq))
            aligns.append(align.copy())
        for i in range(len(align)):
            if align[- (i + 1)] == len(physical_nodes) - 1:
                align[- (i + 1)] = 0
            else:
                align[- (i + 1)] += 1
                break
        if sum(align) == 0:
            break

    if len(accuracies) == 0:
        return -1
    else:
        index, element = min(enumerate(accuracies), key=itemgetter(1))
        return element, aligns[index]


tests = [
    [4,1],
    [4,2],
    [4,3],
    [4,4],
    [4,5],
    [5,1],
    [5,2],
    [5,3],
    [5,4],
    [5,5],
    [6,1],
    [6,2],
    [6,3],
    [7,1],
    [7,2],
    [7,3],
    [8,1],
    [8,2],
    [9,1],
    [9,2],
    [10,1],
    [11,1],
    [12,1],
]

with open("get_best.csv", "w") as f:
    f.write("test_size,graphs_quantity,execution_time,best_accuracy,best_solution\n")
    for test in tests:
        print("test_{}_{} started".format(test[0], test[1]))
        start = timeit.default_timer()
        fileP = "/home/reflex/Desktop/Diplom/code/tests/full_connect/test_{}_{}.json".format(test[0], test[1])

        fileD = open(fileP, 'r')
        graph_json = fileD.read()
        fileD.close()

        graph_data = json.loads(graph_json)

        physical_graph = graph_data["physical"]
        virtual_graphs = graph_data["virtuals"]

        best_accuracy, best_solution = find_all_mappings(physical_graph, virtual_graphs)
        stop = timeit.default_timer()
        print("test_{}_{} finished for {}".format(test[0], test[1], stop-start))
        f.write("{},{},{},{},{}\n".format(str(test[0]), str(test[1]), stop-start, best_accuracy, "_".join(map(str, best_solution))))
        