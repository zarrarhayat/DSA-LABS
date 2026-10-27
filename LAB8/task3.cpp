// Lab 8 - Task 3: Tower of Hanoi (Iterative)
// Zarrar (543233)

#include <iostream>
#include <stack>
using namespace std;

void moveBetween(stack<int> &p1, stack<int> &p2, char name1, char name2) {
    if (p1.empty()) {
        p1.push(p2.top());
        p2.pop();
        cout << "Move disk " << p1.top() << " from " << name2 << " to " << name1 << endl;
    }
    else if (p2.empty()) {
        p2.push(p1.top());
        p1.pop();
        cout << "Move disk " << p2.top() << " from " << name1 << " to " << name2 << endl;
    }
    else if (p1.top() < p2.top()) {
        p2.push(p1.top());
        p1.pop();
        cout << "Move disk " << p2.top() << " from " << name1 << " to " << name2 << endl;
    }
    else {
        p1.push(p2.top());
        p2.pop();
        cout << "Move disk " << p1.top() << " from " << name2 << " to " << name1 << endl;
    }
}

void towerOfHanoi(int n) {
    stack<int> A, B, C;
    char src = 'A', aux = 'B', dest = 'C';

    for (int disk = n; disk >= 1; disk--)
        A.push(disk);

    stack<int> *s = &A, *a = &B, *d = &C;
    if (n % 2 == 0) {
        stack<int> *temp = a;
        a = d;
        d = temp;
        char t = aux;
        aux = dest;
        dest = t;
    }

    long long totalMoves = (1LL << n) - 1;
    for (long long i = 1; i <= totalMoves; i++) {
        if (i % 3 == 1)
            moveBetween(*s, *d, src, dest);
        else if (i % 3 == 2)
            moveBetween(*s, *a, src, aux);
        else
            moveBetween(*a, *d, aux, dest);
    }

    cout << "Total moves: " << totalMoves << endl;
}

int main() {
    int n;
    cout << "Enter number of disks: ";
    cin >> n;
    if (n <= 0 || n > 62) {
        cout << "Number of disks must be between 1 and 62." << endl;
        return 0;
    }
    towerOfHanoi(n);
    return 0;
}