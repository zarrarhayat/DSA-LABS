
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

struct WordNode {
    string word;
    int count;
    WordNode *left;
    WordNode *right;
    WordNode(const string &w) : word(w), count(1), left(NULL), right(NULL) {}
};

struct FreqNode {
    string word;
    int count;
    FreqNode *left;
    FreqNode *right;
    FreqNode(const string &w, int c) : word(w), count(c), left(NULL), right(NULL) {}
};

class WordBST {
private:
    WordNode *root;

    WordNode *insertRec(WordNode *p, const string &w);
    void alphabeticalRec(WordNode *p) const;
    void buildFreqTree(WordNode *p, FreqNode *&froot) const;
    FreqNode *insertFreq(FreqNode *f, const string &w, int c) const;
    void frequencyRec(FreqNode *f) const;
    void destroyFreq(FreqNode *f) const;
    int maxCount(WordNode *p) const;
    void printWordsWithCount(WordNode *p, int c) const;
    int countNodes(WordNode *p) const;
    int sumCounts(WordNode *p) const;
    void destroy(WordNode *p);

public:
    WordBST() : root(NULL) {}
    ~WordBST() { destroy(root); }

    void insertWord(const string &w) { root = insertRec(root, w); }
    void insertParagraph(const string &text);
    void displayAlphabetical() const;
    void displayByFrequency() const;
    void findMostFrequentWord() const;
    int totalUniqueWords() const { return countNodes(root); }
    int totalWordCount() const { return sumCounts(root); }
};

WordNode *WordBST::insertRec(WordNode *p, const string &w) {
    if (p == NULL) return new WordNode(w);
    if (w < p->word)      p->left  = insertRec(p->left, w);
    else if (w > p->word) p->right = insertRec(p->right, w);
    else                  p->count++;
    return p;
}

void WordBST::insertParagraph(const string &text) {
    string current = "";
    for (size_t i = 0; i < text.size(); i++) {
        unsigned char ch = text[i];
        if (isalpha(ch)) {
            current += (char)tolower(ch);
        } else if (!current.empty()) {
            insertWord(current);
            current = "";
        }
    }
    if (!current.empty()) insertWord(current);
}

void WordBST::alphabeticalRec(WordNode *p) const {
    if (p != NULL) {
        alphabeticalRec(p->left);
        cout << "  " << p->word << " : " << p->count << endl;
        alphabeticalRec(p->right);
    }
}

void WordBST::displayAlphabetical() const {
    cout << "Words in alphabetical order:" << endl;
    alphabeticalRec(root);
}

FreqNode *WordBST::insertFreq(FreqNode *f, const string &w, int c) const {
    if (f == NULL) return new FreqNode(w, c);
    if (c > f->count || (c == f->count && w < f->word))
        f->left = insertFreq(f->left, w, c);
    else
        f->right = insertFreq(f->right, w, c);
    return f;
}

void WordBST::buildFreqTree(WordNode *p, FreqNode *&froot) const {
    if (p != NULL) {
        buildFreqTree(p->left, froot);
        froot = insertFreq(froot, p->word, p->count);
        buildFreqTree(p->right, froot);
    }
}

void WordBST::frequencyRec(FreqNode *f) const {
    if (f != NULL) {
        frequencyRec(f->left);
        cout << "  " << f->word << " : " << f->count << endl;
        frequencyRec(f->right);
    }
}

void WordBST::destroyFreq(FreqNode *f) const {
    if (f != NULL) {
        destroyFreq(f->left);
        destroyFreq(f->right);
        delete f;
    }
}

void WordBST::displayByFrequency() const {
    FreqNode *froot = NULL;
    buildFreqTree(root, froot);
    cout << "Words by frequency (highest to lowest):" << endl;
    frequencyRec(froot);
    destroyFreq(froot);
}

int WordBST::maxCount(WordNode *p) const {
    if (p == NULL) return 0;
    int best = p->count;
    int l = maxCount(p->left);
    int r = maxCount(p->right);
    if (l > best) best = l;
    if (r > best) best = r;
    return best;
}

void WordBST::printWordsWithCount(WordNode *p, int c) const {
    if (p != NULL) {
        printWordsWithCount(p->left, c);
        if (p->count == c) cout << "  " << p->word << " : " << p->count << endl;
        printWordsWithCount(p->right, c);
    }
}

void WordBST::findMostFrequentWord() const {
    if (root == NULL) {
        cout << "No words indexed." << endl;
        return;
    }
    int best = maxCount(root);
    cout << "Most frequent word(s):" << endl;
    printWordsWithCount(root, best);
}

int WordBST::countNodes(WordNode *p) const {
    if (p == NULL) return 0;
    return 1 + countNodes(p->left) + countNodes(p->right);
}

int WordBST::sumCounts(WordNode *p) const {
    if (p == NULL) return 0;
    return p->count + sumCounts(p->left) + sumCounts(p->right);
}

void WordBST::destroy(WordNode *p) {
    if (p != NULL) {
        destroy(p->left);
        destroy(p->right);
        delete p;
    }
}

int main() {
    string paragraph = "The quick brown fox jumps over the lazy dog. The fox is quick and smart.";

    // 1 and 2. Build the word index (case-insensitive, frequencies counted)
    WordBST index;
    index.insertParagraph(paragraph);

    // 3. Display operations
    index.displayAlphabetical();
    cout << endl;
    index.displayByFrequency();
    cout << endl;
    index.findMostFrequentWord();

    // 4. Summary information
    cout << "\nSummary" << endl;
    cout << "Total unique words: " << index.totalUniqueWords() << endl;
    cout << "Total word count: " << index.totalWordCount() << endl;

    return 0;
}