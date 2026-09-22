#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
    Node(int val) : data(val), next(nullptr) {}
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList() : head(nullptr) {}

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }

    void insertAtHead(int value) {
        Node* newNode = new Node(value);
        newNode->next = head;
        head = newNode;
    }

    void insertAtEnd(int value) {
        Node* newNode = new Node(value);
        if (head == nullptr) {
            head = newNode;
            return;
        }
        Node* current = head;
        while (current->next != nullptr)
            current = current->next;
        current->next = newNode;
    }

    void insertAtThird(int value) {
        if (head == nullptr || head->next == nullptr) {
            cout << "List has fewer than 2 nodes. Inserting at the end instead." << endl;
            insertAtEnd(value);
            return;
        }
        Node* newNode = new Node(value);
        Node* second = head->next;
        newNode->next = second->next;
        second->next = newNode;
    }

    void displayList() {
        if (head == nullptr) {
            cout << "List is empty." << endl;
            return;
        }
        Node* current = head;
        while (current != nullptr) {
            cout << current->data << " -> ";
            current = current->next;
        }
        cout << "NULL" << endl;
    }

    void deleteLast() {
        if (head == nullptr) {
            cout << "List is empty. Nothing to delete." << endl;
            return;
        }
        if (head->next == nullptr) {
            delete head;
            head = nullptr;
            cout << "Last node deleted. List is now empty." << endl;
            return;
        }
        Node* current = head;
        while (current->next->next != nullptr)
            current = current->next;
        delete current->next;
        current->next = nullptr;
        cout << "Last node deleted." << endl;
    }

    int countNodes() {
        int count = 0;
        Node* current = head;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        return count;
    }

    void reverseList() {
        Node* prev = nullptr;
        Node* current = head;
        Node* next = nullptr;

        while (current != nullptr) {
            next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        head = prev;
    }

    void searchValue(int value) {
        Node* current = head;
        int position = 1;
        while (current != nullptr) {
            if (current->data == value) {
                cout << "Value " << value << " found at position " << position << "." << endl;
                return;
            }
            current = current->next;
            position++;
        }
        cout << "Value " << value << " not found in the list." << endl;
    }
};

int main() {
    LinkedList list;
    int choice, value;

    do {
        cout << "\n===== Linked List Menu =====" << endl;
        cout << "1. Insert at Head" << endl;
        cout << "2. Insert at 3rd Position" << endl;
        cout << "3. Display List" << endl;
        cout << "4. Delete Last Node" << endl;
        cout << "5. Count Nodes" << endl;
        cout << "6. Reverse List" << endl;
        cout << "7. Search Value" << endl;
        cout << "8. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter value to insert at head: ";
                cin >> value;
                list.insertAtHead(value);
                list.displayList();
                break;
            case 2:
                cout << "Enter value to insert at 3rd position: ";
                cin >> value;
                list.insertAtThird(value);
                list.displayList();
                break;
            case 3:
                list.displayList();
                break;
            case 4:
                list.deleteLast();
                list.displayList();
                break;
            case 5:
                cout << "Number of nodes: " << list.countNodes() << endl;
                break;
            case 6:
                list.reverseList();
                cout << "List reversed." << endl;
                list.displayList();
                break;
            case 7:
                cout << "Enter value to search: ";
                cin >> value;
                list.searchValue(value);
                break;
            case 8:
                cout << "Exiting program." << endl;
                break;
            default:
                cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 8);

    return 0;
}