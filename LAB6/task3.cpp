#include <iostream>
using namespace std;

struct Request {
    int id;
    int arrival;
    int processing;
};

class CircularQueue {
    Request* arr;
    int capacity;
    int frontIndex;
    int count;

public:
    CircularQueue(int cap) {
        capacity = cap;
        arr = new Request[capacity];
        frontIndex = 0;
        count = 0;
    }

    ~CircularQueue() {
        delete[] arr;
    }

    bool isEmpty() {
        return count == 0;
    }

    bool isFull() {
        return count == capacity;
    }

    void resize(int newCapacity) {
        Request* newArr = new Request[newCapacity];

        for (int i = 0; i < count; i++) {
            newArr[i] = arr[(frontIndex + i) % capacity];
        }

        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
        frontIndex = 0;
        cout << "   (queue resized to capacity " << capacity << ")" << endl;
    }

    void enqueue(Request x) {
        if (isFull()) {
            resize(capacity * 2);
        }
        int rear = (frontIndex + count) % capacity;
        arr[rear] = x;
        count++;
    }

    Request front() {
        return arr[frontIndex];
    }

    Request dequeue() {
        Request x = arr[frontIndex];
        frontIndex = (frontIndex + 1) % capacity;
        count--;

        if (capacity > 2 && count <= capacity / 4) {
            resize(capacity / 2);
        }
        return x;
    }

    void display() {
        cout << "   Queue (size " << count << ", capacity " << capacity << "): ";
        if (isEmpty()) {
            cout << "empty";
        }
        for (int i = 0; i < count; i++) {
            Request r = arr[(frontIndex + i) % capacity];
            cout << "[ID " << r.id << "] ";
        }
        cout << endl;
    }
};

Request makeRequest(int id, int arrival, int processing) {
    Request r;
    r.id = id;
    r.arrival = arrival;
    r.processing = processing;
    return r;
}

int main() {
    const int N = 10;
    Request requests[N];
    requests[0] = makeRequest(1, 0, 5);
    requests[1] = makeRequest(2, 1, 3);
    requests[2] = makeRequest(3, 2, 4);
    requests[3] = makeRequest(4, 3, 2);
    requests[4] = makeRequest(5, 4, 6);
    requests[5] = makeRequest(6, 5, 1);
    requests[6] = makeRequest(7, 6, 3);
    requests[7] = makeRequest(8, 20, 2);
    requests[8] = makeRequest(9, 21, 4);
    requests[9] = makeRequest(10, 40, 1);

    CircularQueue queue(2);
    int clock = 0;
    int next = 0;
    int processed = 0;
    int totalWait = 0;

    cout << "===== Print Server Simulation =====" << endl;

    while (next < N || !queue.isEmpty()) {
        if (queue.isEmpty() && requests[next].arrival > clock) {
            clock = requests[next].arrival;
        }

        while (next < N && requests[next].arrival <= clock) {
            cout << "Time " << clock << ": enqueue request " << requests[next].id << endl;
            queue.enqueue(requests[next]);
            queue.display();
            next++;
        }

        Request r = queue.front();
        queue.dequeue();
        int wait = clock - r.arrival;
        totalWait += wait;
        processed++;

        cout << "Time " << clock << ": dequeue request " << r.id
             << " (waited " << wait << ")" << endl;
        queue.display();

        clock += r.processing;
    }

    cout << endl << "Requests processed: " << processed << endl;
    cout << "Average waiting time: " << (double)totalWait / processed << endl;

    return 0;
}
