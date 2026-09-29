#include <iostream>
#include <limits>
using namespace std;

const int MAX_PEOPLE = 10000;

struct Person {
    int id;
    Person* next;

    explicit Person(int i) : id(i), next(nullptr) {}
};

class JosephusCircle {
private:
    Person* last;
    int size;

    int* eliminated;
    int eliminatedCount;
    int survivor;

public:
    JosephusCircle()
        : last(nullptr), size(0), eliminated(nullptr), eliminatedCount(0), survivor(0) {}

    ~JosephusCircle() {
        clear();
    }

    JosephusCircle(const JosephusCircle&) = delete;
    JosephusCircle& operator=(const JosephusCircle&) = delete;

    void clear() {
        if (last != nullptr) {
            Person* cur = last->next;
            for (int i = 0; i < size; i++) {
                Person* nextPerson = cur->next;
                delete cur;
                cur = nextPerson;
            }
        }
        last = nullptr;
        size = 0;

        delete[] eliminated;
        eliminated = nullptr;
        eliminatedCount = 0;
        survivor = 0;
    }

    void createCircle(int n) {
        clear();
        for (int id = 1; id <= n; id++) {
            Person* p = new Person(id);
            if (last == nullptr) {
                p->next = p;
            } else {
                p->next = last->next;
                last->next = p;
            }
            last = p;
            size++;
        }
        eliminated = new int[n];
    }

    void displayCircle() const {
        if (last == nullptr) {
            cout << "The circle is empty." << endl;
            return;
        }
        Person* cur = last->next;
        for (int i = 0; i < size; i++) {
            cout << cur->id;
            if (i < size - 1)
                cout << " -> ";
            cur = cur->next;
        }
        cout << " -> (back to " << last->next->id << ")" << endl;
    }

    void eliminate(int k) {
        if (last == nullptr) {
            cout << "Create the circle first." << endl;
            return;
        }
        Person* prev = last;
        int round = 1;

        while (size > 1) {
            int steps = (k - 1) % size;
            for (int step = 0; step < steps; step++)
                prev = prev->next;

            Person* victim = prev->next;
            prev->next = victim->next;
            if (victim == last)
                last = prev;

            cout << "Round " << round << ": person " << victim->id << " is eliminated." << endl;
            eliminated[eliminatedCount++] = victim->id;
            delete victim;
            size--;
            round++;
        }
        survivor = last->id;
    }

    void displayEliminatedOrder() const {
        if (eliminatedCount == 0) {
            cout << "Nobody was eliminated." << endl;
            return;
        }
        cout << "Elimination order: ";
        for (int i = 0; i < eliminatedCount; i++) {
            cout << eliminated[i];
            if (i < eliminatedCount - 1)
                cout << ", ";
        }
        cout << endl;
    }

    void displaySurvivor() const {
        if (survivor == 0)
            cout << "The game has not been played yet." << endl;
        else
            cout << "Survivor: person " << survivor << endl;
    }
};

bool readPositive(const char* prompt, int minValue, int& value) {
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minValue)
            return true;
        if (cin.eof())
            return false;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please enter a whole number of at least " << minValue << "." << endl;
    }
}

int main() {
    int n, k;
    if (!readPositive("Enter the number of people (N): ", 1, n))
        return 0;
    while (n > MAX_PEOPLE) {
        cout << "Please enter at most " << MAX_PEOPLE << " people." << endl;
        if (!readPositive("Enter the number of people (N): ", 1, n))
            return 0;
    }
    if (!readPositive("Enter the step count (k): ", 1, k))
        return 0;

    JosephusCircle circle;
    circle.createCircle(n);

    cout << "\nCircle created:" << endl;
    circle.displayCircle();

    cout << "\nElimination process (k = " << k << "):" << endl;
    circle.eliminate(k);

    cout << endl;
    circle.displayEliminatedOrder();
    circle.displaySurvivor();

    return 0;
}