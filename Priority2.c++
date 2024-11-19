// PREEMPTIVE
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <climits> // For INT_MIN

using namespace std;

struct Process {
    int pid;         // Process ID
    int arrival;     // Arrival Time
    int burst;       // CPU Burst Time
    int remaining;   // Remaining Burst Time
    int priority;    // Priority (higher number = higher priority)
    int completion;  // Completion Time
    int tat;         // Turnaround Time
    int wt;          // Waiting Time
};

void calculateTimes(vector<Process> &processes) {
    int n = processes.size();
    int currentTime = 0;
    int completed = 0;
    int highestPriority = -1;
    int maxPriority = INT_MIN;

    while (completed < n) {
        // Find the process with the highest priority that has arrived
        highestPriority = -1;
        maxPriority = INT_MIN;

        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= currentTime && processes[i].remaining > 0) {
                if (processes[i].priority > maxPriority) {
                    highestPriority = i;
                    maxPriority = processes[i].priority;
                } else if (processes[i].priority == maxPriority && processes[i].arrival < processes[highestPriority].arrival) {
                    highestPriority = i; // Break ties using arrival time
                }
            }
        }

        if (highestPriority != -1) {
            // Execute the process with the highest priority for 1 unit of time
            processes[highestPriority].remaining--;
            currentTime++;

            // If the process is completed
            if (processes[highestPriority].remaining == 0) {
                completed++;
                processes[highestPriority].completion = currentTime;
                processes[highestPriority].tat = processes[highestPriority].completion - processes[highestPriority].arrival;
                processes[highestPriority].wt = processes[highestPriority].tat - processes[highestPriority].burst;
            }
        } else {
            // If no process is ready, increment the current time
            currentTime++;
        }
    }
}

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
             << setw(15) << p.priority
             << setw(20) << p.completion
             << setw(20) << p.tat
             << setw(15) << p.wt << endl;
    }
}

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    // Input process details
    for (int i = 0; i < n; i++) {
        cout << "Enter Arrival Time, Burst Time, and Priority for Process " << i + 1 << ":\n";
        processes[i].pid = i + 1;
        cout << "Arrival Time: ";
        cin >> processes[i].arrival;
        cout << "Burst Time: ";
        cin >> processes[i].burst;
        cout << "Priority (higher number = higher priority): ";
        cin >> processes[i].priority;
        processes[i].remaining = processes[i].burst; // Initialize remaining burst time
    }

    // Calculate Completion Time, Turnaround Time, and Waiting Time
    calculateTimes(processes);

    // Print results
    cout << "\nScheduling Table:\n";
    printTable(processes);

    return 0;
}
