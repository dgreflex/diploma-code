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
    typeTs = ["star_topology", "random_topology", "full_connect", "dragonfly"]

    for typeT in typeTs:
        fromA = 603
        toA = 1000
        program = "/usr/bin/time"
        args = ["-v", "/home/reflex/Desktop/Diplom/code/build/main", "/home/reflex/Desktop/Diplom/code/tests/{}/test_{}.json".format("star_topology", "5")]
        vertices = [2, 4, 8, 16, 32, 64, 128, 256, 512, 768, 1024]

        with open("{}_{}_{}.csv".format(typeT, fromA, toA), "w") as file:
            file.write("test_size,time,accuracy,memory\n")

            for i in vertices:
                print(typeT, i)
                try:
                    args[-1] = "/home/reflex/Desktop/Diplom/code/tests/{}/test_{}.json".format(typeT, i)
                    first_line = run_program_and_get_first_line(program, *args)
                    file.write("{},{},{},{}\n".format(i, first_line[1], first_line[0], first_line[2]))
                    
                    # with open("output_first_line.txt", "a") as file:
                    #     file.write(first_line)
                    # print("Первая строка вывода сохранена в файл output_first_line.txt")
                except RuntimeError as error:
                    print(str(error))

if __name__ == "__main__":
    main()
