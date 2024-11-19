// Optimal Page Replacement

#include <iostream>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <climits>
using namespace std;

// Function to find the page that will not be used for the longest time in the future
int findOptimalVictim(const vector<int> &referenceString, const unordered_set<int> &frames, int currentIndex) {
    unordered_map<int, int> futureUse;

    // Initialize all frame pages with a large value indicating they are not used in the future
    for (int page : frames) {
        futureUse[page] = INT_MAX;
    }

    // Update the future use based on the reference string
    for (int i = currentIndex + 1; i < referenceString.size(); i++) {
        if (futureUse.find(referenceString[i]) != futureUse.end() && futureUse[referenceString[i]] == INT_MAX) {
            futureUse[referenceString[i]] = i; // Store the next usage index
        }
    }

    // Find the page with the farthest future use or no use at all
    int victimPage = -1, farthestUse = -1;
    for (const auto &entry : futureUse) {
        if (entry.second > farthestUse) {
            farthestUse = entry.second;
            victimPage = entry.first;
        }
    }

    return victimPage;
}

int calculatePageFaultsOptimal(vector<int> &referenceString, int frameSize) {
    unordered_set<int> frames; // Stores the pages currently in memory
    int pageFaults = 0;

    for (int i = 0; i < referenceString.size(); i++) {
        int page = referenceString[i];

        // If the page is not in memory (page fault)
        if (frames.find(page) == frames.end()) {
            pageFaults++;

            // If frames are full, replace the optimal victim
            if (frames.size() == frameSize) {
                int victimPage = findOptimalVictim(referenceString, frames, i);
                frames.erase(victimPage);
            }

            // Add the new page to memory
            frames.insert(page);
        }
    }

    return pageFaults;
}

int main() {
    int n, frameSize;

    cout << "Enter the number of pages in the reference string: ";
    cin >> n;

    vector<int> referenceString(n);
    cout << "Enter the reference string (space-separated page numbers): ";
    for (int i = 0; i < n; i++) {
        cin >> referenceString[i];
    }

    cout << "Enter the number of frames: ";
    cin >> frameSize;

    int pageFaults = calculatePageFaultsOptimal(referenceString, frameSize);

    cout << "\nNumber of page faults using Optimal Page Replacement: " << pageFaults << endl;

    return 0;
}
