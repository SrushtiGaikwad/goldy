#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm> // For std::sort

using namespace std;

struct Process {
    int pid;       // Process ID
    int arrival;   // Arrival Time
    int burst;     // CPU Burst Time
    int completion; // Completion Time
    int tat;       // Turnaround Time
    int wt;        // Waiting Time
};

void calculateTimes(vector<Process> &processes) {
    int n = processes.size();
    int currentTime = 0;

    for (int i = 0; i < n; i++) {
        // Ensure the process starts after it arrives
        if (currentTime < processes[i].arrival) {
            currentTime = processes[i].arrival;
        }

        // Completion Time = Current Time + Burst Time
        processes[i].completion = currentTime + processes[i].burst;

        // Turnaround Time = Completion Time - Arrival Time
        processes[i].tat = processes[i].completion - processes[i].arrival;

        // Waiting Time = Turnaround Time - Burst Time
        processes[i].wt = processes[i].tat - processes[i].burst;

        // Update current time to the completion time of this process
        currentTime = processes[i].completion;
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
    }

    // Sort processes by Arrival Time
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
