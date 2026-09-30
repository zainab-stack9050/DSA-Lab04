#include <iostream>
#include <string>
using namespace std;

struct Song {      // Doubly linked list node
    int id;
    string name;
    int minutes, seconds;
    Song *next, *prev;
    Song(int i, const string& n, int m, int s)
        : id(i), name(n), minutes(m), seconds(s), next(nullptr), prev(nullptr) {}
};

class Playlist {        
    Song *head = nullptr, *tail = nullptr, *current = nullptr;   // current = "now playing"

    static void print(const Song* s) {
        cout << "  [" << s->id << "] " << s->name << "  ("
             << s->minutes << ":" << (s->seconds < 10 ? "0" : "") << s->seconds << ")\n";
    }
    Song* find(int id) const {
        for (Song* p = head; p; p = p->next) if (p->id == id) return p;
        return nullptr;
    }
public:
    ~Playlist() {
        while (head) { Song* t = head; head = head->next; delete t; }
    }

    // insertion at the end of playlist
    bool addSong(int id, const string& name, int m, int s) {
        if (find(id)) { cout << "A song with ID " << id << " already exists.\n"; return false; }
        Song* n = new Song(id, name, m, s);
        if (!head) head = tail = current = n;
        else { tail->next = n; n->prev = tail; tail = n; }
        cout << "Song added.\n";
        return true;
    }

    // removes a song by ID
    bool deleteSong(int id) {
        Song* p = find(id);
        if (!p) { cout << "Song not found.\n"; return false; }
        if (p == current) current = p->next ? p->next : p->prev;
        if (p->prev) p->prev->next = p->next; else head = p->next;
        if (p->next) p->next->prev = p->prev; else tail = p->prev;
        delete p;
        cout << "Song deleted.\n";
        return true;
    }
    //display function
    void displayForward() const {
        if (!head) { cout << "Playlist is empty.\n"; return; }
        cout << "Playlist (first -> last):\n";
        for (Song* p = head; p; p = p->next) print(p);
    }

    void displayBackward() const {
        if (!tail) { cout << "Playlist is empty.\n"; return; }
        cout << "Playlist (last -> first):\n";
        for (Song* p = tail; p; p = p->prev) print(p);
    }

    void searchSong(int id) const {
        Song* p = find(id);
        if (p) { cout << "Found:\n"; print(p); }
        else cout << "Song not found.\n";
    }

    void nowPlaying() const {
        if (!current) { cout << "Nothing to play.\n"; return; }
        cout << "Now playing:\n"; print(current);
    }

    void playNext() {
        if (!current) { cout << "Playlist is empty.\n"; return; }
        if (!current->next) { cout << "End of playlist - no next song.\n"; print(current); return; }
        current = current->next;
        nowPlaying();
    }

    void playPrevious() {
        if (!current) { cout << "Playlist is empty.\n"; return; }
        if (!current->prev) { cout << "Start of playlist - no previous song.\n"; print(current); return; }
        current = current->prev;
        nowPlaying();
    }

    // swaps next and prev in every node, then swap head/tail
    void reverse() {
        Song* p = head;
        while (p) {
            Song* t = p->next;
            p->next = p->prev;
            p->prev = t;
            p = t;            
        }
        Song* t = head; head = tail; tail = t;
        cout << "Playlist reversed.\n";
    }
};

static int readInt(const string& prompt) {
    int x = 0;
    cout << prompt;
    cin >> x;
    return x;
}

int main() {
    Playlist pl;
    int choice;
    do {
        cout << "\n===== PLAYLIST MENU =====\n"
             << "1. Add Song\n2. Delete Song\n3. Display Forward\n4. Display Backward\n"
             << "5. Search Song\n6. Play Next\n7. Play Previous\n8. Now Playing\n"
             << "9. Reverse Playlist\n0. Exit\n";
        choice = readInt("Choice: ");
        switch (choice) {
            case 1: {
                int id = readInt("Song ID: ");
                string name;
                cout << "Song name: ";
                getline(cin >> ws, name);
                
                int m = 0, s = 0;
                char colon;
                cout << "Duration (mm:ss, e.g., 5:35): ";
                cin >> m >> colon >> s;

                // adds the song
                pl.addSong(id, name, m, s);
                break;
            }
            case 2: pl.deleteSong(readInt("Song ID to delete: ")); break;
            case 3: pl.displayForward(); break;
            case 4: pl.displayBackward(); break;
            case 5: pl.searchSong(readInt("Song ID to search: ")); break;
            case 6: pl.playNext(); break;
            case 7: pl.playPrevious(); break;
            case 8: pl.nowPlaying(); break;
            case 9: pl.reverse(); break;
            case 0: cout << "Goodbye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0 && cin);
    return 0;
}