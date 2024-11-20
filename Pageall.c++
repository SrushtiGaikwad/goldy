#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <list>
#include <climits>
using namespace std;

// Function for FIFO Page Replacement
int calculatePageFaultsFIFO(vector<int> &referenceString, int frameSize) {
    unordered_set<int> frames;     // To store the pages currently in memory
    queue<int> pageOrder;          // To maintain the order of pages for FIFO
    int pageFaults = 0;

    for (int page : referenceString) {
        if (frames.find(page) == frames.end()) { // Page fault
            pageFaults++;
            if (frames.size() == frameSize) { // If frames are full, remove the oldest page
                int oldestPage = pageOrder.front();
                pageOrder.pop();
                frames.erase(oldestPage);
            }
            frames.insert(page);
            pageOrder.push(page);
        }
    }
    return pageFaults;
}

// Function for LRU Page Replacement
int calculatePageFaultsLRU(vector<int>& referenceString, int frameSize) {
    unordered_map<int, list<int>::iterator> pageMap; // Stores the page and its position in the list
    list<int> frames;  // Doubly linked list to maintain the order of pages
    int pageFaults = 0;

    for (int page : referenceString) {
        if (pageMap.find(page) == pageMap.end()) { // Page fault
            pageFaults++;
            if (frames.size() == frameSize) { // If memory is full, remove the least recently used page
                int lruPage = frames.back();
                frames.pop_back();
                pageMap.erase(lruPage);
            }
            frames.push_front(page);
        } else { // If the page is already in memory, move it to the front
            frames.erase(pageMap[page]);
            frames.push_front(page);
        }
        pageMap[page] = frames.begin();
    }
    return pageFaults;
}

// Function for Optimal Page Replacement
int findOptimalVictim(const vector<int> &referenceString, const unordered_set<int> &frames, int currentIndex) {
    unordered_map<int, int> futureUse;
    for (int page : frames) {
        futureUse[page] = INT_MAX; // Initialize all frame pages with a large value
    }
    for (int i = currentIndex + 1; i < referenceString.size(); i++) {
        if (futureUse.find(referenceString[i]) != futureUse.end() && futureUse[referenceString[i]] == INT_MAX) {
            futureUse[referenceString[i]] = i; // Store the next usage index
        }
    }
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
    unordered_set<int> frames;
    int pageFaults = 0;

    for (int i = 0; i < referenceString.size(); i++) {
        int page = referenceString[i];
        if (frames.find(page) == frames.end()) { // Page fault
            pageFaults++;
            if (frames.size() == frameSize) {
                int victimPage = findOptimalVictim(referenceString, frames, i);
                frames.erase(victimPage);
            }
            frames.insert(page);
        }
    }
    return pageFaults;
}

// Main Function with Menu
int main() {
    int choice;
    do {
        cout << "\nPage Replacement Algorithms:\n";
        cout << "1. FIFO (First-In-First-Out)\n";
        cout << "2. LRU (Least Recently Used)\n";
        cout << "3. Optimal Page Replacement\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice >= 1 && choice <= 3) {
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

            int pageFaults = 0;
            switch (choice) {
                case 1:
                    pageFaults = calculatePageFaultsFIFO(referenceString, frameSize);
                    break;
                case 2:
                    pageFaults = calculatePageFaultsLRU(referenceString, frameSize);
                    break;
                case 3:
                    pageFaults = calculatePageFaultsOptimal(referenceString, frameSize);
                    break;
            }
            cout << "\nNumber of page faults: " << pageFaults << endl;
        }
    } while (choice != 4);

    cout << "Exiting program. Goodbye!" << endl;
    return 0;
}
