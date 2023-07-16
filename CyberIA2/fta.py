def calculate_response_time(task_list):
    schedulable = "SCHED_YES"

    for i in range(len(task_list)):
        response_old = 0
        response_new = task_list[i]["WCET"]

        while response_new != response_old:
            response_old = response_new
            response_new = task_list[i]["WCET"]

            for j in range(len(task_list)):
                if task_list[j]["priority"] > task_list[i]["priority"]:
                    response_new += (
                        (response_old // task_list[j]["period"]) + 1
                    ) * task_list[j]["WCET"]

        if response_new > task_list[i]["deadline"]:
            schedulable = "SCHED_NO"
            break

    return schedulable


def main():
    data_tasks = [
        {"name": "T1", "period": 25, "WCET": 10, "priority": 5, "deadline": 25},
        {"name": "T2", "period": 25, "WCET": 8, "priority": 4, "deadline": 25},
        {"name": "T3", "period": 50, "WCET": 5, "priority": 3, "deadline": 50},
        {"name": "T4", "period": 50, "WCET": 4, "priority": 2, "deadline": 50},
        {"name": "T5", "period": 100, "WCET": 2, "priority": 1, "deadline": 100},
    ]

    tasks1 = [
        {"name": "T1", "period": 50, "WCET": 12, "priority": 1, "deadline": 50},
        {"name": "T2", "period": 40, "WCET": 10, "priority": 2, "deadline": 40},
        {"name": "T3", "period": 30, "WCET": 10, "priority": 3, "deadline": 50},
    ]

    tasks2 = [
        {"name": "T1", "period": 80, "WCET": 12, "priority": 1, "deadline": 50},
        {"name": "T2", "period": 40, "WCET": 10, "priority": 2, "deadline": 40},
        {"name": "T3", "period": 20, "WCET": 10, "priority": 3, "deadline": 50},
    ]

    tasks3 = [
        {"name": "T1", "period": 7, "WCET": 3, "priority": 3, "deadline": 7},
        {"name": "T2", "period": 12, "WCET": 3, "priority": 2, "deadline": 12},
        {"name": "T3", "period": 20, "WCET": 5, "priority": 1, "deadline": 20},
    ]

    tasks4 = [
        {"name": "T1", "period": 7, "WCET": 3, "priority": 3, "deadline": 3},
        {"name": "T2", "period": 12, "WCET": 3, "priority": 2, "deadline": 6},
        {"name": "T3", "period": 20, "WCET": 5, "priority": 1, "deadline": 20},
   
    ]
    test_cases = [data_tasks, tasks1, tasks2, tasks3, tasks4]

    for idx, task_list in enumerate(test_cases):
        schedulable = calculate_response_time(task_list)
        print(f"Response time test {idx+1}: {schedulable}")


if __name__ == "__main__":
    main()
