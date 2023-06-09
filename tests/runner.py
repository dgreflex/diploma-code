import subprocess

def run_program_and_get_first_line(program, *args):
    command = [program] + list(args)
    result = subprocess.run(command, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)

    if result.returncode == 0:
        first_line = result.stdout.splitlines()[0]
        time = result.stdout.splitlines()[1]
        memory = result.stderr.splitlines()[9].split()[-1]

        return [first_line, time, memory]
    else:
        raise RuntimeError(f"Программа завершилась с ошибкой (код {result.returncode}): {result.stderr}")

def main():
    typeTs = ["full_connect"]#, "star_topology", "random_topology", "dragonfly"]

    for typeT in typeTs:
        testname = "t3"
        numiters = 10
        program = "/usr/bin/time"
        args = ["-v", "/home/reflex/Desktop/Diplom/code/build/main", "/home/reflex/Desktop/Diplom/code/tests/{}/test_{}.json".format("star_topology", "5")]
        vertices = [4,5,6,7,8,9,10,11,12]
        graphs_quantity = [1, 5]
        with open("{}_{}_{}.csv".format(typeT, testname, numiters), "w") as file:
            file.write("test_size,graphs_quantity,time,accuracy,memory\n")
            for z in range(numiters):
                for gq in range(graphs_quantity[0], graphs_quantity[1] + 1):
                    for i in vertices:
                        print("{} iter: {}, gq: {}, test_size: {}".format(typeT, z, gq, i))
                        try:
                            args[-1] = "/home/reflex/Desktop/Diplom/code/tests/{}/test_{}_{}.json".format(typeT, i, gq)
                            first_line = run_program_and_get_first_line(program, *args)
                            file.write("{},{},{},{},{}\n".format(i, gq, first_line[1], first_line[0], first_line[2]))
                            # with open("output_first_line.txt", "a") as file:
                            #     file.write(first_line)
                            # print("Первая строка вывода сохранена в файл output_first_line.txt")
                        except RuntimeError as error:
                            print(str(error))

if __name__ == "__main__":
    main()
