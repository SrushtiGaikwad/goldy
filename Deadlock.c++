#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

bool isSafeState(vector<vector<int>>& allocation, vector<vector<int>>& max, vector<int>& available) {
    int numProcesses = allocation.size();
    int numResources = available.size();

    // Calculate the Need matrix
    vector<vector<int>> need(numProcesses, vector<int>(numResources));
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Display Allocation, Max, and Need matrices
    cout << "\nProcess\t  Allocation\t  Max\t\t  Need\n";
    for (int i = 0; i < numProcesses; i++) {
        cout << "P" << i << "\t  ";
        for (int j = 0; j < numResources; j++) {
            cout << allocation[i][j] << " ";
        }
        cout << "\t\t";
        for (int j = 0; j < numResources; j++) {
            cout << max[i][j] << " ";
        }
        cout << "\t\t";
        for (int j = 0; j < numResources; j++) {
            cout << need[i][j] << " ";
        }
        cout << "\n";
    }

    // Work and Finish arrays
    vector<int> work = available;
    vector<bool> finish(numProcesses, false);
    int completed = 0;

    // Try to find a safe sequence
    cout << "\nStep-by-step Execution:\n";
    while (completed < numProcesses) {
        bool progressMade = false;

        for (int i = 0; i < numProcesses; i++) {
            // Skip already finished processes
            if (finish[i]) continue;

            // Check if the process's need can be satisfied with available resources
            bool canFinish = true;
            for (int j = 0; j < numResources; j++) {
                if (need[i][j] > work[j]) {
                    canFinish = false;
                    break;
                }
            }

            // If process can be completed
            if (canFinish) {
                cout << "Process P" << i << " executes.\n";

                // Add its allocated resources to work
                for (int j = 0; j < numResources; j++) {
                    work[j] += allocation[i][j];
                }

                // Mark this process as finished
                finish[i] = true;
                completed++;
                progressMade = true;
                break; // Start again with the updated work
            }
        }

        // If no progress is made in this iteration, system is in unsafe state
        if (!progressMade) {
            cout << "No further processes can execute.\n";
            return false;
        }
    }

    // If all processes are finished
    return true;
}

int main() {
    int numProcesses, numResources;

    // Input number of processes and resources
    cout << "Enter number of processes: ";
    cin >> numProcesses;
    cout << "Enter number of resources: ";
    cin >> numResources;

    // Input available resources
    vector<int> available(numResources);
    cout << "Enter available resources: ";
    for (int i = 0; i < numResources; i++) {
        cin >> available[i];
    }

    // Input allocation matrix
    vector<vector<int>> allocation(numProcesses, vector<int>(numResources));
    cout << "Enter allocation matrix (rows are processes, columns are resources):\n";
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            cin >> allocation[i][j];
        }
    }

    // Input max matrix
    vector<vector<int>> max(numProcesses, vector<int>(numResources));
    cout << "Enter max matrix (rows are processes, columns are resources):\n";
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numResources; j++) {
            cin >> max[i][j];
        }
    }

    // Check if the system is in a safe state
    if (isSafeState(allocation, max, available)) {
        cout << "\nThe system is in a safe state.\n";
    } else {
        cout << "\nThe system is not in a safe state.\n";
    }

    return 0;
}
