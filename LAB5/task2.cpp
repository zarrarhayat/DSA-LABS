#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Photo {
    int id;
    string name;
    string date;
    string location;
    Photo* next;
    Photo* prev;

    Photo(int i, const string& n, const string& d, const string& l)
        : id(i), name(n), date(d), location(l), next(nullptr), prev(nullptr) {}
};

class PhotoAlbum {
private:
    Photo* head;
    Photo* current;

    void printPhoto(const Photo* p) const {
        cout << "ID: " << p->id << " | Name: " << p->name
             << " | Date: " << p->date << " | Location: " << p->location << endl;
    }

    void linkAfter(Photo* node, Photo* newPhoto) {
        Photo* after = node->next;
        newPhoto->prev = node;
        newPhoto->next = after;
        node->next = newPhoto;
        after->prev = newPhoto;
    }

    void makeFirst(Photo* newPhoto) {
        newPhoto->next = newPhoto;
        newPhoto->prev = newPhoto;
        head = newPhoto;
        current = newPhoto;
    }

    void removeNode(Photo* target) {
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
    PhotoAlbum() : head(nullptr), current(nullptr) {}

    ~PhotoAlbum() {
        while (head != nullptr)
            removeNode(head);
    }

    PhotoAlbum(const PhotoAlbum&) = delete;
    PhotoAlbum& operator=(const PhotoAlbum&) = delete;

    Photo* findPhoto(int id) const {
        if (head == nullptr)
            return nullptr;
        Photo* p = head;
        do {
            if (p->id == id)
                return p;
            p = p->next;
        } while (p != head);
        return nullptr;
    }

    bool addPhoto(int id, const string& name, const string& date, const string& location) {
        if (findPhoto(id) != nullptr) {
            cout << "A photo with ID " << id << " already exists." << endl;
            return false;
        }
        Photo* newPhoto = new Photo(id, name, date, location);
        if (head == nullptr)
            makeFirst(newPhoto);
        else
            linkAfter(head->prev, newPhoto);
        cout << "Photo added." << endl;
        return true;
    }

    void insertAfterCurrent(int id, const string& name, const string& date, const string& location) {
        if (findPhoto(id) != nullptr) {
            cout << "A photo with ID " << id << " already exists." << endl;
            return;
        }
        Photo* newPhoto = new Photo(id, name, date, location);
        if (current == nullptr)
            makeFirst(newPhoto);
        else
            linkAfter(current, newPhoto);
        cout << "Photo inserted after the current photo." << endl;
    }

    void removePhoto(int id) {
        Photo* target = findPhoto(id);
        if (target == nullptr) {
            cout << "Photo with ID " << id << " not found." << endl;
            return;
        }
        removeNode(target);
        cout << "Photo removed." << endl;
    }

    void removeCurrent() {
        if (current == nullptr) {
            cout << "The album is empty." << endl;
            return;
        }
        removeNode(current);
        cout << "Current photo removed." << endl;
    }

    void moveNext() {
        if (current == nullptr) {
            cout << "The album is empty." << endl;
            return;
        }
        current = current->next;
        cout << "Current photo -> ";
        printPhoto(current);
    }

    void movePrevious() {
        if (current == nullptr) {
            cout << "The album is empty." << endl;
            return;
        }
        current = current->prev;
        cout << "Current photo -> ";
        printPhoto(current);
    }

    void displayForward() const {
        if (current == nullptr) {
            cout << "The album is empty." << endl;
            return;
        }
        Photo* p = current;
        do {
            printPhoto(p);
            p = p->next;
        } while (p != current);
    }

    void displayBackward() const {
        if (current == nullptr) {
            cout << "The album is empty." << endl;
            return;
        }
        Photo* p = current;
        do {
            printPhoto(p);
            p = p->prev;
        } while (p != current);
    }

    void searchPhoto(int id) const {
        Photo* p = findPhoto(id);
        if (p == nullptr) {
            cout << "Photo with ID " << id << " not found." << endl;
        } else {
            cout << "Found: ";
            printPhoto(p);
        }
    }

    int countPhotos() const {
        if (head == nullptr)
            return 0;
        int count = 0;
        Photo* p = head;
        do {
            count++;
            p = p->next;
        } while (p != head);
        return count;
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

bool readPhoto(int& id, string& name, string& date, string& location) {
    cout << "Enter photo ID: ";
    if (!readInt(id)) {
        if (!cin.eof())
            cout << "Invalid ID." << endl;
        return false;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return readLine("Enter photo name: ", name) &&
           readLine("Enter date taken: ", date) &&
           readLine("Enter location: ", location);
}

int main() {
    PhotoAlbum album;

    int n;
    while (true) {
        cout << "Enter the number of photos: ";
        if (readInt(n) && n >= 0)
            break;
        if (cin.eof())
            return 0;
        cout << "Please enter a whole number of 0 or more." << endl;
    }
    for (int i = 1; i <= n; i++) {
        cout << "\n--- Photo " << i << " ---" << endl;
        int id;
        string name, date, location;
        while (!(readPhoto(id, name, date, location) &&
                 album.addPhoto(id, name, date, location))) {
            if (cin.eof())
                return 0;
        }
    }

    int choice = 0;
    do {
        cout << "\n===== Photo Album Menu =====" << endl;
        cout << "1. Add Photo" << endl;
        cout << "2. Insert Photo After Current" << endl;
        cout << "3. Remove Photo" << endl;
        cout << "4. Remove Current Photo" << endl;
        cout << "5. Move Next" << endl;
        cout << "6. Move Previous" << endl;
        cout << "7. Display Album Forward" << endl;
        cout << "8. Display Album Backward" << endl;
        cout << "9. Search Photo" << endl;
        cout << "10. Count Photos" << endl;
        cout << "11. Exit" << endl;
        cout << "Enter your choice: ";

        if (!readInt(choice)) {
            if (cin.eof())
                break;
            cout << "Please enter a number from 1 to 11." << endl;
            choice = 0;
            continue;
        }

        switch (choice) {
            case 1:
            case 2: {
                int id;
                string name, date, location;
                if (!readPhoto(id, name, date, location))
                    break;
                if (choice == 1)
                    album.addPhoto(id, name, date, location);
                else
                    album.insertAfterCurrent(id, name, date, location);
                break;
            }
            case 3: {
                int id;
                cout << "Enter photo ID to remove: ";
                if (readInt(id))
                    album.removePhoto(id);
                else
                    cout << "Invalid ID." << endl;
                break;
            }
            case 4:
                album.removeCurrent();
                break;
            case 5:
                album.moveNext();
                break;
            case 6:
                album.movePrevious();
                break;
            case 7:
                album.displayForward();
                break;
            case 8:
                album.displayBackward();
                break;
            case 9: {
                int id;
                cout << "Enter photo ID to search: ";
                if (readInt(id))
                    album.searchPhoto(id);
                else
                    cout << "Invalid ID." << endl;
                break;
            }
            case 10:
                cout << "Total photos: " << album.countPhotos() << endl;
                break;
            case 11:
                cout << "Exiting." << endl;
                break;
            default:
                cout << "Please enter a number from 1 to 11." << endl;
        }
    } while (choice != 11 && !cin.eof());

    return 0;
}