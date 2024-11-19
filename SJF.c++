//NON-PREEMPTIVE
#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

using namespace std;

struct Process {
    int pid;        // Process ID
    int arrival;    // Arrival Time
    int burst;      // CPU Burst Time
    int completion; // Completion Time
    int tat;        // Turnaround Time
    int wt;         // Waiting Time
    bool isComplete; // Indicates whether the process is completed
};

void calculateTimes(vector<Process> &processes) {
    int n = processes.size();
    int currentTime = 0;
    int completed = 0;

    while (completed < n) {
        // Find the process with the shortest burst time that has arrived
        int shortest = -1;
        for (int i = 0; i < n; i++) {
            if (!processes[i].isComplete && processes[i].arrival <= currentTime) {
                if (shortest == -1 || processes[i].burst < processes[shortest].burst) {
                    shortest = i;
                }
            }
        }

        if (shortest != -1) {
            // Process the selected process
            currentTime += processes[shortest].burst;
            processes[shortest].completion = currentTime;
            processes[shortest].tat = processes[shortest].completion - processes[shortest].arrival;
            processes[shortest].wt = processes[shortest].tat - processes[shortest].burst;
            processes[shortest].isComplete = true;
            completed++;
        } else {
            // If no process has arrived yet, increment the current time
            currentTime++;
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
        processes[i].isComplete = false; // Initialize as incomplete
    }

    // Sort processes by Arrival Time initially
    sort(processes.begin(), processes.end(), [](const Process &a, const Process &b) {
        return a.arrival < b.arrival;
    });

    // Calculate Completion Time, Turnaround Time, and Waiting Time
    calculateTimes(processes);

    // Print results
    cout << "\nScheduling Table:\n";
    printTable(processes);

    return 0;
}
