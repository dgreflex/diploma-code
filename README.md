Для работы нужен
- `graphviz-dev` для отрисовки графов
- `nlohmann/json.hpp` для парса jsonа

Для запуска:
- Дебаг мод: `DEBUG=True cmake .; make`
- Просто билд: `cmake .; make`
Затем `./main PATH_TO_TEST

Создаваемые программой `.png` файлы - изображения, отображающие структуры графов, полученных на вход программе.
Пример для `"tests/test1.json"`:

`physical.png`

<img src="https://github.com/dgreflex/diploma-code/blob/master/etc/physical.png" alt="physical.png" height="30%">

`0.png`

<img src="https://github.com/dgreflex/diploma-code/blob/master/etc/0.png" alt="0.png" height="30%">

`1.png`

<img src="https://github.com/dgreflex/diploma-code/blob/master/etc/1.png" alt="1.png" height="30%">

Формат входных данных -- Json.
Пример:
```json
{
    "physical": {
        "nodes": [
            {
                "id": 0,
                "coreFrequency": 5,
                "RAM": 200,
                "hardMemory": 100
            },
            {
                "id": 1,
                "coreFrequency": 10,
                "RAM": 150,
                "hardMemory": 250
            },
            {
                "id": 2,
                "coreFrequency": 10,
                "RAM": 450,
                "hardMemory": 450
            },
            {
                "id": 3,
                "coreFrequency": 10,
                "RAM": 550,
                "hardMemory": 550
            }
        ],
        "edges": [
            {
                "from": 0,
                "to": 1,
                "capacity": 100,
                "ping": 10
            },
            {
                "from": 1,
                "to": 2,
                "capacity": 200,
                "ping": 10
            },
            {
                "from": 0,
                "to": 3,
                "capacity": 200,
                "ping": 10
            },
            {
                "from": 1,
                "to": 3,
                "capacity": 200,
                "ping": 10
            },
            {
                "from": 2,
                "to": 3,
                "capacity": 500,
                "ping": 10
            }
        ]
    },
    "virtuals": [
        {
            "nodes": [
                {
                    "id": 0,
                    "coreFrequency": 5,
                    "RAM": 200,
                    "hardMemory": 100
                },
                {
                    "id": 1,
                    "coreFrequency": 10,
                    "RAM": 150,
                    "hardMemory": 250
                },
                {
                    "id": 2,
                    "coreFrequency": 10,
                    "RAM": 450,
                    "hardMemory": 450
                },
                {
                    "id": 3,
                    "coreFrequency": 10,
                    "RAM": 550,
                    "hardMemory": 550
                }
            ],
            "edges": [
                {
                    "from": 0,
                    "to": 1,
                    "capacity": 100,
                    "ping": 10
                },
                {
                    "from": 1,
                    "to": 3,
                    "capacity": 200,
                    "ping": 10
                },
                {
                    "from": 2,
                    "to": 3,
                    "capacity": 500,
                    "ping": 10
                }
            ]
        },
        {
            "nodes": [
                {
                    "id": 0,
                    "coreFrequency": 5,
                    "RAM": 200,
                    "hardMemory": 100
                },
                {
                    "id": 1,
                    "coreFrequency": 10,
                    "RAM": 150,
                    "hardMemory": 250
                },
                {
                    "id": 2,
                    "coreFrequency": 10,
                    "RAM": 450,
                    "hardMemory": 450
                },
                {
                    "id": 3,
                    "coreFrequency": 10,
                    "RAM": 550,
                    "hardMemory": 550
                }
            ],
            "edges": [
                {
                    "from": 1,
                    "to": 2,
                    "capacity": 200,
                    "ping": 10
                },
                {
                    "from": 0,
                    "to": 3,
                    "capacity": 200,
                    "ping": 10
                },
                {
                    "from": 1,
                    "to": 3,
                    "capacity": 200,
                    "ping": 10
                },
                {
                    "from": 2,
                    "to": 3,
                    "capacity": 500,
                    "ping": 10
                }
            ]
        }
    ]
}
```