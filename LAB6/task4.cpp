#include <iostream>
using namespace std;

const int MAX = 100;

class WaitingRoom {
    int queue[MAX];
    int frontIndex;
    int rear;
    int nextId;

public:
    WaitingRoom() {
        frontIndex = 0;
        rear = -1;
        nextId = 1;
    }

    void RegisterPatient() {
        if (rear == MAX - 1) {
            cout << "Waiting room is full" << endl;
            return;
        }
        rear++;
        queue[rear] = nextId;
        cout << "Patient registered with ID " << nextId << endl;
        nextId++;
    }

    void ServePatient() {
        if (CanDoctorGoHome()) {
            cout << "No patients are waiting" << endl;
            return;
        }
        cout << "Now serving patient " << queue[frontIndex] << endl;
        frontIndex++;
    }

    void CancelAll() {
        frontIndex = 0;
        rear = -1;
        cout << "All appointments cancelled" << endl;
    }

    bool CanDoctorGoHome() {
        return frontIndex > rear;
    }

    void ShowAllPatient() {
        if (CanDoctorGoHome()) {
            cout << "No patients are waiting" << endl;
            return;
        }

        int temp[MAX];
        int n = 0;
        for (int i = frontIndex; i <= rear; i++) {
            temp[n] = queue[i];
            n++;
        }

        for (int i = 0; i < n - 1; i++) {
            for (int j = 0; j < n - i - 1; j++) {
                if (temp[j] > temp[j + 1]) {
                    int t = temp[j];
                    temp[j] = temp[j + 1];
                    temp[j + 1] = t;
                }
            }
        }

        cout << "Waiting patients: ";
        for (int i = 0; i < n; i++) {
            cout << temp[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    WaitingRoom room;
    int choice;

    do {
        cout << endl;
        cout << "1. Register patient" << endl;
        cout << "2. Serve patient" << endl;
        cout << "3. Cancel all appointments" << endl;
        cout << "4. Can doctor go home?" << endl;
        cout << "5. Show all waiting patients" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            room.RegisterPatient();
        } else if (choice == 2) {
            room.ServePatient();
        } else if (choice == 3) {
            room.CancelAll();
        } else if (choice == 4) {
            if (room.CanDoctorGoHome()) {
                cout << "Yes, no one is waiting" << endl;
            } else {
                cout << "No, patients are still waiting" << endl;
            }
        } else if (choice == 5) {
            room.ShowAllPatient();
        } else if (choice != 0) {
            cout << "Invalid choice" << endl;
        }
    } while (choice != 0);

    return 0;
}
