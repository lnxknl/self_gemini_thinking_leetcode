#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TASKS 100

typedef struct Task {
    int id;
    int duration;
    int num_prerequisites;
    int prerequisites[MAX_TASKS];
    int in_degree;
    int finish_time;
} Task;

int solve_tasks(Task tasks[], int num_tasks) {
    int in_degree[MAX_TASKS] = {0};
    int adj[MAX_TASKS][MAX_TASKS] = {0}; // Adjacency list for dependencies
    int finish_times[MAX_TASKS];
    int queue[MAX_TASKS];
    int front = 0, rear = -1;
    int processed_tasks = 0;
    int max_finish_time = 0;

    // Calculate in-degrees and build adjacency list
    for (int i = 0; i < num_tasks; i++) {
        for (int j = 0; j < tasks[i].num_prerequisites; j++) {
            int prereq_id = tasks[i].prerequisites[j];
            for (int k = 0; k < num_tasks; k++) {
                if (tasks[k].id == prereq_id) {
                    in_degree[i]++;
                    // Find the index of the prerequisite task
                    for (int l = 0; l < num_tasks; l++) {
                        if (tasks[l].id == prereq_id) {
                            adj[l][i] = 1; // Edge from prereq to current task
                            break;
                        }
                    }
                    break;
                }
            }
        }
        tasks[i].in_degree = in_degree[i];
        finish_times[i] = 0;
    }

    // Initialize queue with tasks having in-degree 0
    for (int i = 0; i < num_tasks; i++) {
        if (tasks[i].in_degree == 0) {
            queue[++rear] = i;
            finish_times[i] = tasks[i].duration;
        }
    }

    while (front <= rear) {
        int current_task_index = queue[front++];
        processed_tasks++;

        for (int i = 0; i < num_tasks; i++) {
            if (adj[current_task_index][i]) {
                tasks[i].in_degree--;
                if (tasks[i].in_degree == 0) {
                    int max_prereq_finish_time = 0;
                    for (int j = 0; j < tasks[i].num_prerequisites; j++) {
                        int prereq_id = tasks[i].prerequisites[j];
                        for (int k = 0; k < num_tasks; k++) {
                            if (tasks[k].id == prereq_id) {
                                if (finish_times[k] > max_prereq_finish_time) {
                                    max_prereq_finish_time = finish_times[k];
                                }
                                break;
                            }
                        }
                    }
                    finish_times[i] = max_prereq_finish_time + tasks[i].duration;
                    queue[++rear] = i;
                }
            }
        }
    }

    if (processed_tasks != num_tasks) {
        return -1; // Cycle detected
    }

    for (int i = 0; i < num_tasks; i++) {
        if (finish_times[i] > max_finish_time) {
            max_finish_time = finish_times[i];
        }
    }

    return max_finish_time;
}

int main() {
    Task tasks[] = {
        {1, 3, 0, {}, 0, 0},
        {2, 2, 1, {1}, 0, 0},
        {3, 4, 1, {1}, 0, 0},
        {4, 1, 2, {2, 3}, 0, 0}
    };
    int num_tasks = sizeof(tasks) / sizeof(tasks[0]);

    int result = solve_tasks(tasks, num_tasks);
    printf("Minimum time to complete all tasks: %d\n", result); // Expected output: 10

    // Example with a cycle
    Task tasks_cycle[] = {
        {1, 3, 1, {2}, 0, 0},
        {2, 2, 1, {1}, 0, 0}
    };
    num_tasks = sizeof(tasks_cycle) / sizeof(tasks_cycle[0]);
    result = solve_tasks(tasks_cycle, num_tasks);
    printf("Minimum time to complete all tasks (cycle): %d\n", result); // Expected output: -1

    return 0;
}
