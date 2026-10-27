// Lab 8 - Task 1: Peak Element in a 2D Array
// Zarrar (543233)

#include <iostream>
#include <vector>
using namespace std;

int maxRowInColumn(const vector<vector<int> > &a, int col) {
    int best = 0;
    for (int i = 1; i < (int)a.size(); i++) {
        if (a[i][col] > a[best][col])
            best = i;
    }
    return best;
}

void findPeak(const vector<vector<int> > &a, int &peakRow, int &peakCol) {
    int cols = a[0].size();
    int low = 0, high = cols - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        int row = maxRowInColumn(a, mid);
        int value = a[row][mid];

        bool leftBigger = (mid > 0 && a[row][mid - 1] > value);
        bool rightBigger = (mid < cols - 1 && a[row][mid + 1] > value);

        if (!leftBigger && !rightBigger) {
            peakRow = row;
            peakCol = mid;
            return;
        }
        if (leftBigger)
            high = mid - 1;
        else
            low = mid + 1;
    }
}

int main() {
    int rows, cols;
    cout << "Enter number of rows and columns: ";
    cin >> rows >> cols;
    if (rows <= 0 || cols <= 0) {
        cout << "Rows and columns must be positive." << endl;
        return 0;
    }

    vector<vector<int> > a(rows, vector<int>(cols));
    cout << "Enter the elements row by row:" << endl;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            cin >> a[i][j];

    int peakRow, peakCol;
    findPeak(a, peakRow, peakCol);

    cout << "Peak element: " << a[peakRow][peakCol]
         << " at row " << peakRow << ", column " << peakCol << endl;
    return 0;
}