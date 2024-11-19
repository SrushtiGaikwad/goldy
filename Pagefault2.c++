// Least Recently Used 
#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
using namespace std;

int calculatePageFaultsLRU(vector<int>& referenceString, int frameSize) {
    unordered_map<int, list<int>::iterator> pageMap;  // Stores the page and its position in the list
    list<int> frames;  // Doubly linked list to maintain the order of pages
    int pageFaults = 0;

    for (int page : referenceString) {
        // If page is not in memory (page fault)
        if (pageMap.find(page) == pageMap.end()) {
            pageFaults++;

            // If there is space in memory, simply insert the new page
            if (frames.size() < frameSize) {
                frames.push_front(page);
            }
            // If memory is full, remove the least recently used page
            else {
                int lruPage = frames.back();
                frames.pop_back();  // Remove the least recently used page
                pageMap.erase(lruPage);
                frames.push_front(page);  // Insert the new page
            }
        }
        else {
            // If the page is already in memory, move it to the front (most recently used)
            frames.erase(pageMap[page]);
            frames.push_front(page);
        }

        // Update the position of the page in the map
        pageMap[page] = frames.begin();
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

    int pageFaults = calculatePageFaultsLRU(referenceString, frameSize);

    cout << "\nNumber of page faults using LRU: " << pageFaults << endl;

    return 0;
}


