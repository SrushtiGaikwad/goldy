#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>
#include <algorithm>
using namespace std;

struct Process {
    int pid;       // Process ID
    int arrival;   // Arrival Time
    int burst;     // CPU Burst Time
    int remaining; // Remaining Burst Time
    int completion; // Completion Time
    int tat;       // Turnaround Time
    int wt;        // Waiting Time
};

void roundRobinScheduling(vector<Process> &processes, int timeQuantum) {
    int n = processes.size();
    int currentTime = 0;
    int completed = 0;
    queue<int> readyQueue;

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival < b.arrival;
    });

    // Push the first process into the ready queue
    readyQueue.push(0);
    vector<bool> visited(n, false); // Track whether a process has been added to the queue
    visited[0] = true;

    while (!readyQueue.empty()) {
        int index = readyQueue.front();
        readyQueue.pop();

        // If the process starts after current time, move time forward
        if (processes[index].arrival > currentTime) {
            currentTime = processes[index].arrival;
        }

        // Process execution
        int timeExecuted = min(timeQuantum, processes[index].remaining);
        processes[index].remaining -= timeExecuted;
        currentTime += timeExecuted;

        // If the process is completed
        if (processes[index].remaining == 0) {
            completed++;
            processes[index].completion = currentTime;
            processes[index].tat = processes[index].completion - processes[index].arrival;
            processes[index].wt = processes[index].tat - processes[index].burst;
        }

        // Add newly arrived processes to the ready queue
        for (int i = 0; i < n; i++) {
            if (!visited[i] && processes[i].arrival <= currentTime && processes[i].remaining > 0) {
                readyQueue.push(i);
                visited[i] = true;
            }
        }

        // If the current process is not completed, re-add it to the queue
        if (processes[index].remaining > 0) {
            readyQueue.push(index);
        }

        // If the queue is empty but there are processes still to execute, move time to the next arrival
        if (readyQueue.empty() && completed < n) {
            for (int i = 0; i < n; i++) {
                if (processes[i].remaining > 0) {
                    readyQueue.push(i);
                    visited[i] = true;
                    break;
                }
            }
        }
    }
}

void printTable(const vector<Process> &processes) {
    cout << left << setw(10) << "PID"
         << setw(15) << "Arrival Time"
         << setw(15) << "Burst Time"
         << setw(20) << "Completion Time"
         << setw(20) << "Turnaround Time"
         << setw(15) << "Waiting Time" << endl;

    for (const auto &p : processes) {
        cout << left << setw(10) << p.pid
             << setw(15) << p.arrival
             << setw(15) << p.burst
             << setw(20) << p.completion
             << setw(20) << p.tat
             << setw(15) << p.wt << endl;
    }
}

int main() {
    int n, timeQuantum;

    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    // Input process details
    for (int i = 0; i < n; i++) {
        cout << "Enter Arrival Time and Burst Time for Process " << i + 1 << ":\n";
        processes[i].pid = i + 1;
        cout << "Arrival Time: ";
        cin >> processes[i].arrival;
        cout << "Burst Time: ";
        cin >> processes[i].burst;
        processes[i].remaining = processes[i].burst; // Initialize remaining burst time
    }

    cout << "Enter the Time Quantum: ";
    cin >> timeQuantum;

    // Perform Round Robin Scheduling
    roundRobinScheduling(processes, timeQuantum);

    // Print results
    cout << "\nScheduling Table:\n";
    printTable(processes);

    return 0;
}
