#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Coach {
    int number;
    string type;
    int capacity;
    int passengers;
    Coach* next;
    Coach* prev;

    Coach(int n, const string& t, int cap, int pas)
        : number(n), type(t), capacity(cap), passengers(pas), next(nullptr), prev(nullptr) {}

    int available() const { return capacity - passengers; }
};

class Train {
private:
    Coach* head;
    Coach* current;

    void printCoach(const Coach* c) const {
        cout << "Coach: " << c->number << " | Type: " << c->type
             << " | Capacity: " << c->capacity << " | Passengers: " << c->passengers
             << " | Available: " << c->available() << endl;
    }

    void linkAfter(Coach* node, Coach* newCoach) {
        Coach* after = node->next;
        newCoach->prev = node;
        newCoach->next = after;
        node->next = newCoach;
        after->prev = newCoach;
    }

    void removeNode(Coach* target) {
        if (target->next == target) {
            head = nullptr;
            current = nullptr;
        } else {
            target->prev->next = target->next;
            target->next->prev = target->prev;
            if (head == target)
                head = target->next;
            if (current == target)
                current = target->next;
        }
        delete target;
    }

public:
    Train() : head(nullptr), current(nullptr) {}

    ~Train() {
        while (head != nullptr)
            removeNode(head);
    }

    Train(const Train&) = delete;
    Train& operator=(const Train&) = delete;

    Coach* findCoach(int number) const {
        if (head == nullptr)
            return nullptr;
        Coach* c = head;
        do {
            if (c->number == number)
                return c;
            c = c->next;
        } while (c != head);
        return nullptr;
    }

    bool addCoach(int number, const string& type, int capacity, int passengers) {
        if (findCoach(number) != nullptr) {
            cout << "Coach " << number << " already exists." << endl;
            return false;
        }
        Coach* newCoach = new Coach(number, type, capacity, passengers);
        if (head == nullptr) {
            newCoach->next = newCoach;
            newCoach->prev = newCoach;
            head = newCoach;
            current = newCoach;
        } else {
            linkAfter(head->prev, newCoach);
        }
        cout << "Coach added." << endl;
        return true;
    }

    void insertCoach(int afterNumber, int number, const string& type, int capacity, int passengers) {
        Coach* after = findCoach(afterNumber);
        if (after == nullptr) {
            cout << "Coach " << afterNumber << " not found." << endl;
            return;
        }
        if (findCoach(number) != nullptr) {
            cout << "Coach " << number << " already exists." << endl;
            return;
        }
        linkAfter(after, new Coach(number, type, capacity, passengers));
        cout << "Coach inserted after coach " << afterNumber << "." << endl;
    }

    void removeCoach(int number) {
        Coach* target = findCoach(number);
        if (target == nullptr) {
            cout << "Coach " << number << " not found." << endl;
            return;
        }
        removeNode(target);
        cout << "Coach removed." << endl;
    }

    void moveForward() {
        if (current == nullptr) {
            cout << "The train is empty." << endl;
            return;
        }
        current = current->next;
        displayCurrent();
    }

    void moveBackward() {
        if (current == nullptr) {
            cout << "The train is empty." << endl;
            return;
        }
        current = current->prev;
        displayCurrent();
    }

    void displayClockwise() const {
        if (head == nullptr) {
            cout << "The train is empty." << endl;
            return;
        }
        Coach* c = head;
        do {
            printCoach(c);
            c = c->next;
        } while (c != head);
    }

    void displayAntiClockwise() const {
        if (head == nullptr) {
            cout << "The train is empty." << endl;
            return;
        }
        Coach* start = head->prev;
        Coach* c = start;
        do {
            printCoach(c);
            c = c->prev;
        } while (c != start);
    }

    void searchCoach(int number) const {
        Coach* c = findCoach(number);
        if (c == nullptr) {
            cout << "Coach " << number << " not found." << endl;
        } else {
            cout << "Found: ";
            printCoach(c);
        }
    }

    void findMaxAvailable() const {
        if (head == nullptr) {
            cout << "The train is empty." << endl;
            return;
        }
        Coach* best = head;
        Coach* c = head->next;
        while (c != head) {
            if (c->available() > best->available())
                best = c;
            c = c->next;
        }
        cout << "Most available seats: ";
        printCoach(best);
    }

    void displayCurrent() const {
        if (current == nullptr) {
            cout << "The train is empty." << endl;
            return;
        }
        cout << "Current coach -> ";
        printCoach(current);
    }

    void reverse() {
        if (head == nullptr) {
            cout << "The train is empty." << endl;
            return;
        }
        Coach* c = head;
        do {
            Coach* originalNext = c->next;
            c->next = c->prev;
            c->prev = originalNext;
            c = originalNext;
        } while (c != head);
        head = head->next;
        cout << "Train direction reversed." << endl;
    }
};

bool readInt(int& value) {
    if (cin >> value)
        return true;
    if (!cin.eof()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    return false;
}

bool readLine(const string& prompt, string& text) {
    while (true) {
        cout << prompt;
        if (!getline(cin, text))
            return false;
        if (text.find_first_not_of(" \t\r") != string::npos)
            return true;
        cout << "This field cannot be empty." << endl;
    }
}

bool readCoach(int& number, string& type, int& capacity, int& passengers) {
    cout << "Enter coach number: ";
    if (!readInt(number)) {
        if (!cin.eof())
            cout << "Invalid coach number." << endl;
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (!readLine("Enter coach type: ", type))
        return false;
    cout << "Enter passenger capacity: ";
    if (!readInt(capacity) || capacity <= 0) {
        if (!cin.eof())
            cout << "Capacity must be a positive whole number." << endl;
        return false;
    }
    cout << "Enter current passengers: ";
    if (!readInt(passengers) || passengers < 0 || passengers > capacity) {
        if (!cin.eof())
            cout << "Passengers must be between 0 and the capacity." << endl;
        return false;
    }
    return true;
}

int main() {
    Train train;

    int n;
    while (true) {
        cout << "Enter the number of coaches: ";
        if (readInt(n) && n >= 0)
            break;
        if (cin.eof())
            return 0;
        cout << "Please enter a whole number of 0 or more." << endl;
    }
    for (int i = 1; i <= n; i++) {
        cout << "\n--- Coach " << i << " ---" << endl;
        int number, capacity, passengers;
        string type;
        while (!(readCoach(number, type, capacity, passengers) &&
                 train.addCoach(number, type, capacity, passengers))) {
            if (cin.eof())
                return 0;
        }
    }

    int choice = 0;
    do {
        cout << "\n===== Train Coach Menu =====" << endl;
        cout << "1. Add Coach" << endl;
        cout << "2. Insert Coach" << endl;
        cout << "3. Remove Coach" << endl;
        cout << "4. Move Forward" << endl;
        cout << "5. Move Backward" << endl;
        cout << "6. Display Train Clockwise" << endl;
        cout << "7. Display Train Anti-clockwise" << endl;
        cout << "8. Search Coach" << endl;
        cout << "9. Find Maximum Available Capacity" << endl;
        cout << "10. Display Current Coach" << endl;
        cout << "11. Reverse Train Direction" << endl;
        cout << "12. Exit" << endl;
        cout << "Enter your choice: ";

        if (!readInt(choice)) {
            if (cin.eof())
                break;
            cout << "Please enter a number from 1 to 12." << endl;
            choice = 0;
            continue;
        }

        switch (choice) {
            case 1: {
                int number, capacity, passengers;
                string type;
                if (readCoach(number, type, capacity, passengers))
                    train.addCoach(number, type, capacity, passengers);
                break;
            }
            case 2: {
                int afterNumber, number, capacity, passengers;
                string type;
                cout << "Insert after coach number: ";
                if (!readInt(afterNumber)) {
                    if (!cin.eof())
                        cout << "Invalid coach number." << endl;
                    break;
                }
                if (readCoach(number, type, capacity, passengers))
                    train.insertCoach(afterNumber, number, type, capacity, passengers);
                break;
            }
            case 3: {
                int number;
                cout << "Enter coach number to remove: ";
                if (readInt(number))
                    train.removeCoach(number);
                else if (!cin.eof())
                    cout << "Invalid coach number." << endl;
                break;
            }
            case 4:
                train.moveForward();
                break;
            case 5:
                train.moveBackward();
                break;
            case 6:
                train.displayClockwise();
                break;
            case 7:
                train.displayAntiClockwise();
                break;
            case 8: {
                int number;
                cout << "Enter coach number to search: ";
                if (readInt(number))
                    train.searchCoach(number);
                else if (!cin.eof())
                    cout << "Invalid coach number." << endl;
                break;
            }
            case 9:
                train.findMaxAvailable();
                break;
            case 10:
                train.displayCurrent();
                break;
            case 11:
                train.reverse();
                break;
            case 12:
                cout << "Exiting." << endl;
                break;
            default:
                cout << "Please enter a number from 1 to 12." << endl;
        }
    } while (choice != 12 && !cin.eof());

    return 0;
}