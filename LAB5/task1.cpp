#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Tab {
    int id;
    string title;
    string url;
    Tab* next;
    Tab* prev;

    Tab(int i, const string& t, const string& u)
        : id(i), title(t), url(u), next(nullptr), prev(nullptr) {}
};

class TabManager {
private:
    Tab* current;

    void printTab(const Tab* t) const {
        cout << "ID: " << t->id << " | Title: " << t->title << " | URL: " << t->url << endl;
    }

public:
    TabManager() : current(nullptr) {}

    ~TabManager() {
        while (current != nullptr)
            closeCurrentTab(false);
    }

    TabManager(const TabManager&) = delete;
    TabManager& operator=(const TabManager&) = delete;

    Tab* findTab(int id) const {
        if (current == nullptr)
            return nullptr;
        Tab* t = current;
        do {
            if (t->id == id)
                return t;
            t = t->next;
        } while (t != current);
        return nullptr;
    }

    void openNewTab(int id, const string& title, const string& url) {
        if (findTab(id) != nullptr) {
            cout << "A tab with ID " << id << " is already open." << endl;
            return;
        }
        Tab* newTab = new Tab(id, title, url);

        if (current == nullptr) {
            newTab->next = newTab;
            newTab->prev = newTab;
            current = newTab;
        } else {
            Tab* after = current->next;
            newTab->prev = current;
            newTab->next = after;
            current->next = newTab;
            after->prev = newTab;
        }
        cout << "Tab opened." << endl;
    }

    void closeCurrentTab(bool showMessage = true) {
        if (current == nullptr) {
            if (showMessage)
                cout << "No tabs are open." << endl;
            return;
        }
        Tab* toClose = current;

        if (toClose->next == toClose) {
            current = nullptr;
        } else {
            toClose->prev->next = toClose->next;
            toClose->next->prev = toClose->prev;
            current = toClose->next;
        }
        delete toClose;
        if (showMessage)
            cout << "Tab closed." << endl;
    }

    void moveNext() {
        if (current == nullptr) {
            cout << "No tabs are open." << endl;
            return;
        }
        current = current->next;
        displayCurrentTab();
    }

    void movePrevious() {
        if (current == nullptr) {
            cout << "No tabs are open." << endl;
            return;
        }
        current = current->prev;
        displayCurrentTab();
    }

    void displayCurrentTab() const {
        if (current == nullptr) {
            cout << "No tabs are open." << endl;
            return;
        }
        cout << "Current tab -> ";
        printTab(current);
    }

    void displayForward() const {
        if (current == nullptr) {
            cout << "No tabs are open." << endl;
            return;
        }
        Tab* t = current;
        do {
            printTab(t);
            t = t->next;
        } while (t != current);
    }

    void displayBackward() const {
        if (current == nullptr) {
            cout << "No tabs are open." << endl;
            return;
        }
        Tab* t = current;
        do {
            printTab(t);
            t = t->prev;
        } while (t != current);
    }

    void searchTab(int id) const {
        Tab* t = findTab(id);
        if (t == nullptr) {
            cout << "Tab with ID " << id << " not found." << endl;
        } else {
            cout << "Found: ";
            printTab(t);
        }
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

int main() {
    TabManager tabs;
    int choice = 0;

    do {
        cout << "\n===== Browser Tab Manager =====" << endl;
        cout << "1. Open New Tab" << endl;
        cout << "2. Close Current Tab" << endl;
        cout << "3. Move Next" << endl;
        cout << "4. Move Previous" << endl;
        cout << "5. Display Current Tab" << endl;
        cout << "6. Display All Tabs Forward" << endl;
        cout << "7. Display All Tabs Backward" << endl;
        cout << "8. Search Tab" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter your choice: ";

        if (!readInt(choice)) {
            if (cin.eof())
                break;
            cout << "Please enter a number from 1 to 9." << endl;
            choice = 0;
            continue;
        }

        switch (choice) {
            case 1: {
                int id;
                string title, url;
                cout << "Enter tab ID: ";
                if (!readInt(id)) {
                    cout << "Invalid ID." << endl;
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                if (!readLine("Enter website title: ", title)) break;
                if (!readLine("Enter URL: ", url)) break;
                tabs.openNewTab(id, title, url);
                break;
            }
            case 2:
                tabs.closeCurrentTab();
                break;
            case 3:
                tabs.moveNext();
                break;
            case 4:
                tabs.movePrevious();
                break;
            case 5:
                tabs.displayCurrentTab();
                break;
            case 6:
                tabs.displayForward();
                break;
            case 7:
                tabs.displayBackward();
                break;
            case 8: {
                int id;
                cout << "Enter tab ID to search: ";
                if (readInt(id))
                    tabs.searchTab(id);
                else
                    cout << "Invalid ID." << endl;
                break;
            }
            case 9:
                cout << "Exiting." << endl;
                break;
            default:
                cout << "Please enter a number from 1 to 9." << endl;
        }
    } while (choice != 9 && !cin.eof());

    return 0;
}