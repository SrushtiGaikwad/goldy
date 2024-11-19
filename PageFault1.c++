// FIFO

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
using namespace std;

int calculatePageFaultsFIFO(vector<int> &referenceString, int frameSize) {
    unordered_set<int> frames;     // To store the pages currently in memory
    queue<int> pageOrder;          // To maintain the order of pages for FIFO
    int pageFaults = 0;

    for (int page : referenceString) {
        // If the page is not already in the set, it's a page fault
        if (frames.find(page) == frames.end()) {
            pageFaults++;

            // If frames are full, remove the oldest page (FIFO order)
            if (frames.size() == frameSize) {
                int oldestPage = pageOrder.front();
                pageOrder.pop();
                frames.erase(oldestPage);
            }

            // Add the new page to frames and the queue
            frames.insert(page);
            pageOrder.push(page);
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

    int pageFaults = calculatePageFaultsFIFO(referenceString, frameSize);

    cout << "\nNumber of page faults using FIFO: " << pageFaults << endl;

    return 0;
}

