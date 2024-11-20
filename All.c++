#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <algorithm>
#include <climits> // For INT_MAX and INT_MIN

using namespace std;

struct Process {
    int pid;
    int arrival;
    int burst;
    int priority; // For priority scheduling
    int remaining; // For preemptive scheduling
    int completion;
    int tat; // Turnaround Time
    int wt;  // Waiting Time
    bool isComplete; // For non-preemptive SJF
};

// Helper function to print the process table
void printTable(const vector<Process> &processes) {
    cout << left << setw(10) << "PID"
         << setw(15) << "Arrival Time"
         << setw(15) << "Burst Time"
         << setw(15) << "Priority"
         << setw(20) << "Completion Time"
         << setw(20) << "Turnaround Time"
         << setw(15) << "Waiting Time" << endl;

    for (const auto &p : processes) {
        cout << left << setw(10) << p.pid
             << setw(15) << p.arrival
             << setw(15) << p.burst
             << setw(15) << (p.priority != -1 ? to_string(p.priority) : "-")
             << setw(20) << p.completion
             << setw(20) << p.tat
             << setw(15) << p.wt << endl;
    }
}

// First Come First Serve (FCFS)
void fcfs(vector<Process> &processes) {
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival < b.arrival;
    });

    int currentTime = 0;
    for (auto &p : processes) {
        if (currentTime < p.arrival) {
            currentTime = p.arrival;
        }
        p.completion = currentTime + p.burst;
        p.tat = p.completion - p.arrival;
        p.wt = p.tat - p.burst;
        currentTime = p.completion;
    }

    printTable(processes);
}

// Shortest Job First (Non-Preemptive)
void sjfNonPreemptive(vector<Process> &processes) {
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival < b.arrival;
    });

    int currentTime = 0, completed = 0;
    while (completed < processes.size()) {
        int shortest = -1;
        for (int i = 0; i < processes.size(); i++) {
            if (!processes[i].isComplete && processes[i].arrival <= currentTime) {
                if (shortest == -1 || processes[i].burst < processes[shortest].burst) {
                    shortest = i;
                }
            }
        }

        if (shortest != -1) {
            auto &p = processes[shortest];
            p.completion = currentTime + p.burst;
            p.tat = p.completion - p.arrival;
            p.wt = p.tat - p.burst;
            p.isComplete = true;
            currentTime = p.completion;
            completed++;
        } else {
            currentTime++;
        }
    }

    printTable(processes);
}

// Shortest Job First (Preemptive)
void sjfPreemptive(vector<Process> &processes) {
    int currentTime = 0, completed = 0;
    while (completed < processes.size()) {
        int shortest = -1, minBurst = INT_MAX;

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival <= currentTime && processes[i].remaining > 0 && processes[i].remaining < minBurst) {
                shortest = i;
                minBurst = processes[i].remaining;
            }
        }

        if (shortest != -1) {
            auto &p = processes[shortest];
            p.remaining--;
            currentTime++;

            if (p.remaining == 0) {
                p.completion = currentTime;
                p.tat = p.completion - p.arrival;
                p.wt = p.tat - p.burst;
                completed++;
            }
        } else {
            currentTime++;
        }
    }

    printTable(processes);
}

// Priority Scheduling (Non-Preemptive)
void priorityNonPreemptive(vector<Process> &processes) {
    int currentTime = 0, completed = 0;
    while (completed < processes.size()) {
        int highestPriority = -1;
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival <= currentTime && processes[i].completion == 0) {
                if (highestPriority == -1 || processes[i].priority < processes[highestPriority].priority) {
                    highestPriority = i;
                }
            }
        }

        if (highestPriority != -1) {
            auto &p = processes[highestPriority];
            currentTime += p.burst;
            p.completion = currentTime;
            p.tat = p.completion - p.arrival;
            p.wt = p.tat - p.burst;
            completed++;
        } else {
            currentTime++;
        }
    }

    printTable(processes);
}

// Round Robin
void roundRobin(vector<Process> &processes, int timeQuantum) {
    queue<int> readyQueue;
    vector<bool> visited(processes.size(), false);
    int currentTime = 0, completed = 0;

    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival < b.arrival;
    });

    readyQueue.push(0);
    visited[0] = true;

    while (!readyQueue.empty()) {
        int idx = readyQueue.front();
        readyQueue.pop();

        auto &p = processes[idx];
        if (p.arrival > currentTime) {
            currentTime = p.arrival;
        }

        int timeExecuted = min(timeQuantum, p.remaining);
        p.remaining -= timeExecuted;
        currentTime += timeExecuted;

        if (p.remaining == 0) {
            p.completion = currentTime;
            p.tat = p.completion - p.arrival;
            p.wt = p.tat - p.burst;
            completed++;
        }

        for (int i = 0; i < processes.size(); i++) {
            if (!visited[i] && processes[i].arrival <= currentTime && processes[i].remaining > 0) {
                readyQueue.push(i);
                visited[i] = true;
            }
        }

        if (p.remaining > 0) {
            readyQueue.push(idx);
        }

        if (readyQueue.empty() && completed < processes.size()) {
            for (int i = 0; i < processes.size(); i++) {
                if (processes[i].remaining > 0) {
                    readyQueue.push(i);
                    visited[i] = true;
                    break;
                }
            }
        }
    }

    printTable(processes);
}

// Priority Scheduling (Preemptive)
void priorityPreemptive(vector<Process> &processes) {
    int currentTime = 0, completed = 0;
    while (completed < processes.size()) {
        int highestPriority = -1;
        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrival <= currentTime && processes[i].remaining > 0) {
                if (highestPriority == -1 || processes[i].priority < processes[highestPriority].priority) {
                    highestPriority = i;
                }
            }
        }

        if (highestPriority != -1) {
            auto &p = processes[highestPriority];
            p.remaining--;
            currentTime++;

            if (p.remaining == 0) {
                p.completion = currentTime;
                p.tat = p.completion - p.arrival;
                p.wt = p.tat - p.burst;
                completed++;
            }
        } else {
            currentTime++;
        }
    }

    printTable(processes);
}


// Menu-driven Program
int main() {
    int choice, n, timeQuantum;
    vector<Process> processes;

    do {
        cout << "\nCPU Scheduling Algorithms:\n";
        cout << "1. First Come First Serve (FCFS)\n";
        cout << "2. Shortest Job First (Non-Preemptive)\n";
        cout << "3. Shortest Job First (Preemptive)\n";
        cout << "4. Priority Scheduling (Non-Preemptive)\n";
        cout << "5. Priority Scheduling (Preemptive)\n";
        cout << "6. Round Robin\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice >= 1 && choice <= 5) {
            cout << "Enter the number of processes: ";
            cin >> n;
            processes.resize(n);

            for (int i = 0; i < n; i++) {
                processes[i].pid = i + 1;
                cout << "Enter Arrival Time and Burst Time for Process " << i + 1 << ":\n";
                cin >> processes[i].arrival >> processes[i].burst;
                processes[i].remaining = processes[i].burst; // Initialize remaining burst time
                processes[i].priority = -1; // Default priority
                processes[i].completion = 0;
                processes[i].isComplete = false;
            }

            if (choice == 4) {
                for (int i = 0; i < n; i++) {
                    cout << "Enter Priority for Process " << i + 1 << ": ";
                    cin >> processes[i].priority;
                }
            }

            if (choice == 5) {
                cout << "Enter Time Quantum: ";
                cin >> timeQuantum;
            }

            switch (choice) {
                case 1:
                    fcfs(processes);
                    break;
                case 2:
                    sjfNonPreemptive(processes);
                    break;
                case 3:
                    sjfPreemptive(processes);
                    break;
                case 4:
                    priorityNonPreemptive(processes);
                    break;
                case 5:
                    priorityPreemptive(processes);
                    break;
                
                case 6:
                    roundRobin(processes, timeQuantum);
                    break;
            }
        }
    } while (choice != 7);

    cout << "Exiting program. Goodbye!" << endl;

    return 0;

}
