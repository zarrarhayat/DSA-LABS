#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Song {
    int id;
    string name;
    int minutes;
    int seconds;
    Song* prev;
    Song* next;

    Song(int i, const string& n, int m, int s)
        : id(i), name(n), minutes(m), seconds(s), prev(nullptr), next(nullptr) {}
};

class Playlist {
private:
    Song* head;
    Song* tail;
    Song* current;

    void printSong(const Song* s) const {
        cout << "ID: " << s->id << " | " << s->name << " | "
             << s->minutes << ":" << (s->seconds < 10 ? "0" : "") << s->seconds << endl;
    }

public:
    Playlist() : head(nullptr), tail(nullptr), current(nullptr) {}

    ~Playlist() {
        Song* cur = head;
        while (cur != nullptr) {
            Song* nextSong = cur->next;
            delete cur;
            cur = nextSong;
        }
    }

    Playlist(const Playlist&) = delete;
    Playlist& operator=(const Playlist&) = delete;

    Song* findSong(int id) const {
        Song* cur = head;
        while (cur != nullptr) {
            if (cur->id == id)
                return cur;
            cur = cur->next;
        }
        return nullptr;
    }

    void addSong(int id, const string& name, int minutes, int seconds) {
        if (findSong(id) != nullptr) {
            cout << "A song with ID " << id << " already exists." << endl;
            return;
        }
        Song* newSong = new Song(id, name, minutes, seconds);

        if (head == nullptr) {
            head = newSong;
            tail = newSong;
            current = newSong;
        } else {
            newSong->prev = tail;
            tail->next = newSong;
            tail = newSong;
        }
        cout << "Song added." << endl;
    }

    void deleteSong(int id) {
        Song* target = findSong(id);
        if (target == nullptr) {
            cout << "Song with ID " << id << " not found." << endl;
            return;
        }

        if (current == target)
            current = (target->next != nullptr) ? target->next : target->prev;

        if (target->prev != nullptr)
            target->prev->next = target->next;
        else
            head = target->next;

        if (target->next != nullptr)
            target->next->prev = target->prev;
        else
            tail = target->prev;

        delete target;
        cout << "Song deleted." << endl;
    }

    void displayForward() const {
        if (head == nullptr) {
            cout << "Playlist is empty." << endl;
            return;
        }
        for (Song* cur = head; cur != nullptr; cur = cur->next)
            printSong(cur);
    }

    void displayBackward() const {
        if (tail == nullptr) {
            cout << "Playlist is empty." << endl;
            return;
        }
        for (Song* cur = tail; cur != nullptr; cur = cur->prev)
            printSong(cur);
    }

    void searchSong(int id) const {
        Song* s = findSong(id);
        if (s == nullptr) {
            cout << "Song with ID " << id << " not found." << endl;
        } else {
            cout << "Found: ";
            printSong(s);
        }
    }

    void nowPlaying() const {
        if (current == nullptr) {
            cout << "Playlist is empty." << endl;
        } else {
            cout << "Now playing: ";
            printSong(current);
        }
    }

    void playNext() {
        if (current == nullptr)
            cout << "Playlist is empty." << endl;
        else if (current->next == nullptr)
            cout << "This is the last song." << endl;
        else {
            current = current->next;
            nowPlaying();
        }
    }

    void playPrevious() {
        if (current == nullptr)
            cout << "Playlist is empty." << endl;
        else if (current->prev == nullptr)
            cout << "This is the first song." << endl;
        else {
            current = current->prev;
            nowPlaying();
        }
    }

    void reverse() {
        if (head == nullptr) {
            cout << "Playlist is empty." << endl;
            return;
        }
        Song* cur = head;
        while (cur != nullptr) {
            Song* originalNext = cur->next;
            cur->next = cur->prev;
            cur->prev = originalNext;
            cur = originalNext;
        }
        Song* oldHead = head;
        head = tail;
        tail = oldHead;
        cout << "Playlist reversed." << endl;
    }
};

bool readInt(int& value) {
    if (cin >> value)
        return true;
    if (cin.eof())
        return false;
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return false;
}

int main() {
    Playlist playlist;
    int choice = 0;

    do {
        cout << "\n===== Playlist Menu =====" << endl;
        cout << "1. Add Song" << endl;
        cout << "2. Delete Song" << endl;
        cout << "3. Display Playlist Forward" << endl;
        cout << "4. Display Playlist Backward" << endl;
        cout << "5. Search Song" << endl;
        cout << "6. Play Next Song" << endl;
        cout << "7. Play Previous Song" << endl;
        cout << "8. Reverse Playlist" << endl;
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
                int id, minutes, seconds;
                char colon;
                string name;
                cout << "Enter song ID: ";
                if (!readInt(id)) {
                    cout << "Invalid ID." << endl;
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Enter song name: ";
                getline(cin, name);
                if (name.find_first_not_of(" \t\r") == string::npos) {
                    cout << "Song name cannot be empty." << endl;
                    break;
                }
                cout << "Enter duration (mm:ss): ";
                if (!(cin >> minutes >> colon >> seconds) || colon != ':' ||
                    minutes < 0 || seconds < 0 || seconds > 59) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid duration. Use mm:ss, e.g. 3:45." << endl;
                    break;
                }
                playlist.addSong(id, name, minutes, seconds);
                break;
            }
            case 2: {
                int id;
                cout << "Enter song ID to delete: ";
                if (readInt(id))
                    playlist.deleteSong(id);
                else
                    cout << "Invalid ID." << endl;
                break;
            }
            case 3:
                playlist.displayForward();
                break;
            case 4:
                playlist.displayBackward();
                break;
            case 5: {
                int id;
                cout << "Enter song ID to search: ";
                if (readInt(id))
                    playlist.searchSong(id);
                else
                    cout << "Invalid ID." << endl;
                break;
            }
            case 6:
                playlist.playNext();
                break;
            case 7:
                playlist.playPrevious();
                break;
            case 8:
                playlist.reverse();
                break;
            case 9:
                cout << "Exiting." << endl;
                break;
            default:
                cout << "Please enter a number from 1 to 9." << endl;
        }
    } while (choice != 9 && !cin.eof());

    return 0;
}