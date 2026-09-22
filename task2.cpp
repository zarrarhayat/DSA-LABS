#include <iostream>
#include <string>
using namespace std;

class StringPool {
private:
    string* stringPool;
    int currentSize;
    int maxSize;

public:
    StringPool(int max = 5) {
        maxSize = max;
        currentSize = 0;
        stringPool = new string[maxSize];
    }

    ~StringPool() {
        delete[] stringPool;
    }

    void addString(string value) {
        if (currentSize >= maxSize) {
            cout << "Pool is full! Cannot add \"" << value << "\"." << endl;
            return;
        }
        stringPool[currentSize] = value;
        currentSize++;
    }

    void removeString(int index) {
        if (index < 0 || index >= currentSize) {
            cout << "Invalid index!" << endl;
            return;
        }

        for (int i = index; i < currentSize - 1; i++) {
            stringPool[i] = stringPool[i + 1];
        }

        currentSize--;
    }

    void displayPoolStatus() {
        cout << "Pool status (" << currentSize << "/" << maxSize << "): ";
        if (currentSize == 0) {
            cout << "(empty)";
        } else {
            for (int i = 0; i < currentSize; i++)
                cout << stringPool[i] << " ";
        }
        cout << endl;
    }

    void fixMemoryLeak() {
        int fixedCount = 0;
        for (int i = currentSize; i < maxSize; i++) {
            if (!stringPool[i].empty()) {
                stringPool[i] = "";
                fixedCount++;
            }
        }
        cout << fixedCount << " leaked slot(s) cleared." << endl;
    }
};

int main() {
    StringPool pool(5);

    cout << "--- Adding strings to the pool ---" << endl;
    pool.addString("apple");
    pool.addString("banana");
    pool.addString("cherry");
    pool.addString("date");
    pool.displayPoolStatus();

    cout << "\n--- Removing strings without freeing memory ---" << endl;
    pool.removeString(1);
    pool.removeString(0);
    pool.displayPoolStatus();

    cout << "\n--- Detecting and fixing the memory leak ---" << endl;
    pool.fixMemoryLeak();
    pool.displayPoolStatus();

    return 0;
}