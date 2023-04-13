Для работы нужен
- `graphviz-dev` для отрисовки графов
- `nlohmann/json.hpp` для парса jsonа

Для запуска:
- Дебаг мод: `DEBUG=True cmake .; make`
- Просто билд: `cmake .; make`
Затем `./main PATH_TO_TEST

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