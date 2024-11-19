//SJF WITH PREEMPTIVE

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <climits> // For INT_MAX

using namespace std;

struct Process {
    int pid;         // Process ID
    int arrival;     // Arrival Time
    int burst;       // CPU Burst Time
    int remaining;   // Remaining Burst Time
    int completion;  // Completion Time
    int tat;         // Turnaround Time
    int wt;          // Waiting Time
};

void calculateTimes(vector<Process> &processes) {
    int n = processes.size();
    int currentTime = 0;
    int completed = 0;
    int shortest = -1;
    int minRemaining = INT_MAX;

    while (completed < n) {
        // Find the process with the shortest remaining time
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival <= currentTime && processes[i].remaining > 0 && processes[i].remaining < minRemaining) {
                shortest = i;
                minRemaining = processes[i].remaining;
            }
        }

        if (shortest == -1) {
            // If no process is ready, increment time
            currentTime++;
            continue;
        }

        // Execute the shortest process for 1 unit of time
        processes[shortest].remaining--;
        currentTime++;

        // If the process is completed
        if (processes[shortest].remaining == 0) {
            completed++;
            processes[shortest].completion = currentTime;
            processes[shortest].tat = processes[shortest].completion - processes[shortest].arrival;
            processes[shortest].wt = processes[shortest].tat - processes[shortest].burst;
            minRemaining = INT_MAX; // Reset minRemaining to find the next shortest process
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
    int n;
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

    // Calculate Completion Time, Turnaround Time, and Waiting Time
    calculateTimes(processes);

    // Print results
    cout << "\nScheduling Table:\n";
    printTable(processes);

    return 0;
}
