// Lab 8 - Task 2: Knight's Tour
// Zarrar (543233)

#include <iostream>
#include <iomanip>
#include <vector>
using namespace std;

int N;
vector<vector<int> > board;
int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};

bool isSafe(int x, int y) {
    return x >= 0 && x < N && y >= 0 && y < N && board[x][y] == 0;
}

bool solveBacktracking(int x, int y, int moveCount) {
    if (moveCount == N * N)
        return true;

    for (int i = 0; i < 8; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (isSafe(nx, ny)) {
            board[nx][ny] = moveCount + 1;
            if (solveBacktracking(nx, ny, moveCount + 1))
                return true;
            board[nx][ny] = 0;
        }
    }
    return false;
}

int onwardMoves(int x, int y) {
    int count = 0;
    for (int i = 0; i < 8; i++)
        if (isSafe(x + dx[i], y + dy[i]))
            count++;
    return count;
}

int distanceFromCentre(int x, int y) {
    int cx = 2 * x - (N - 1);
    int cy = 2 * y - (N - 1);
    return cx * cx + cy * cy;
}

bool warnsdorffTour(int x, int y, int first) {
    for (int moveCount = 2; moveCount <= N * N; moveCount++) {
        int bestX = -1, bestY = -1;
        int bestDegree = 9, bestDistance = -1;

        for (int k = 0; k < 8; k++) {
            int i = (first + k) % 8;
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (!isSafe(nx, ny))
                continue;

            int degree = onwardMoves(nx, ny);
            int distance = distanceFromCentre(nx, ny);
            if (degree < bestDegree || (degree == bestDegree && distance > bestDistance)) {
                bestX = nx;
                bestY = ny;
                bestDegree = degree;
                bestDistance = distance;
            }
        }

        if (bestX == -1)
            return false;

        x = bestX;
        y = bestY;
        board[x][y] = moveCount;
    }
    return true;
}

void clearBoard(int x, int y) {
    board.assign(N, vector<int>(N, 0));
    board[x][y] = 1;
}

bool solveWarnsdorff(int x, int y) {
    for (int first = 0; first < 8; first++) {
        clearBoard(x, y);
        if (warnsdorffTour(x, y, first))
            return true;
    }
    clearBoard(x, y);
    return solveBacktracking(x, y, 1);
}

bool tourPossible(int x, int y) {
    if (N == 1) return true;
    if (N < 5) return false;
    if (N % 2 == 1 && (x + y) % 2 == 1) return false;
    return true;
}

void printBoard() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++)
            cout << setw(4) << board[i][j];
        cout << endl;
    }
}

int main() {
    int x, y, choice;
    cout << "Enter board size N: ";
    cin >> N;
    cout << "Enter starting position x y (0 to N-1): ";
    cin >> x >> y;

    if (N <= 0 || x < 0 || x >= N || y < 0 || y >= N) {
        cout << "Invalid board size or starting position." << endl;
        return 0;
    }

    cout << "1. Backtracking" << endl;
    cout << "2. Optimized (Warnsdorff's rule)" << endl;
    cout << "Enter choice: ";
    cin >> choice;
    if (choice != 1 && choice != 2) {
        cout << "Invalid choice." << endl;
        return 0;
    }

    if (!tourPossible(x, y)) {
        cout << "No knight's tour exists from (" << x << ", " << y << ") on a "
             << N << " x " << N << " board." << endl;
        return 0;
    }

    bool found;
    if (choice == 1) {
        clearBoard(x, y);
        found = solveBacktracking(x, y, 1);
    }
    else {
        found = solveWarnsdorff(x, y);
    }

    if (found) {
        cout << "Knight's tour (move numbers):" << endl;
        printBoard();
    }
    else {
        cout << "No knight's tour found." << endl;
    }
    return 0;
}