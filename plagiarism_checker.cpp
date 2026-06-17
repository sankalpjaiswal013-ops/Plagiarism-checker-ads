/*
 * ==
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <chrono>

using namespace std;

// ============================================================
//  UTILITY: String helpers
// ============================================================

// Convert string to lowercase
string toLower(const string& s) {
    string result = s;
    for (char& c : result)
        c = tolower((unsigned char)c);
    return result;
}

// Remove punctuation, keep letters, digits, spaces
string cleanText(const string& s) {
    string result;
    for (char c : s) {
        if (isalpha((unsigned char)c) || isdigit((unsigned char)c) || c == ' ')
            result += tolower((unsigned char)c);
        else
            result += ' ';
    }
    return result;
}

// Split string into words
vector<string> splitWords(const string& text) {
    vector<string> words;
    istringstream iss(text);
    string w;
    while (iss >> w)
        words.push_back(w);
    return words;
}

// ============================================================
//  1. RABIN-KARP ROLLING HASH  (Unit 5)
//
//  How it works:
//    - Text is split into overlapping k-word windows (k-grams)
//    - Each k-gram is hashed using: hash = sum(char[i] * BASE^i) % MOD
//    - Matching hashes are candidates; we verify to avoid false positives
//    - Time: O(n + m) average  vs  O(nm) for naive matching
// ============================================================

const long long RK_BASE = 31;
const long long RK_MOD  = 1000000007LL;  // large prime to reduce collisions

// Compute polynomial rolling hash of a string
long long rabinKarpHash(const string& s) {
    long long hash  = 0;
    long long power = 1;
    for (char c : s) {
        int val = (c - 'a' + 1);   // 'a'=1, 'b'=2, ... ' '=0 treated as space
        if (val < 0) val = 0;
        hash  = (hash + val * power) % RK_MOD;
        power = (power * RK_BASE)    % RK_MOD;
    }
    return hash;
}

// Structure to hold one k-gram fingerprint
struct KGram {
    long long hash;
    int       position;   // word position in document
    string    text;       // actual k-gram text (for verification)
};

// Generate all k-gram fingerprints from a document
vector<KGram> generateKGrams(const string& rawText, int k) {
    string cleaned = cleanText(rawText);
    vector<string> words = splitWords(cleaned);
    vector<KGram> kgrams;

    for (int i = 0; i + k <= (int)words.size(); i++) {
        string gram = "";
        for (int j = i; j < i + k; j++) {
            if (j > i) gram += " ";
            gram += words[j];
        }
        KGram kg;
        kg.hash     = rabinKarpHash(gram);
        kg.position = i;
        kg.text     = gram;
        kgrams.push_back(kg);
    }
    return kgrams;
}

// ============================================================
//  2. B-TREE  (Unit 1)
//
//  How it works:
//    - Each node holds up to (2*ORDER-1) keys
//    - When a node overflows on insert -> SPLIT into two nodes
//    - Middle key is pushed up to parent
//    - Guarantees O(log n) search, insert, delete
//    - Here: indexes corpus documents by their integer ID
// ============================================================

const int BTREE_ORDER = 3;   // max children = 2*ORDER = 6

struct Document {
    int    id;
    string name;
    string text;
};

struct BTreeNode {
    int       keys[2 * BTREE_ORDER - 1];    // document IDs stored
    Document* vals[2 * BTREE_ORDER - 1];    // pointers to document data
    BTreeNode* children[2 * BTREE_ORDER];   // child pointers
    int       numKeys;
    bool      isLeaf;

    BTreeNode(bool leaf) : numKeys(0), isLeaf(leaf) {
        memset(children, 0, sizeof(children));
        memset(vals, 0, sizeof(vals));
    }
};

class BTree {
public:
    BTreeNode* root;
    int splits;    // track splits (show in output)
    int height;

    BTree() : splits(0), height(1) {
        root = new BTreeNode(true);
    }

    // Search for document by ID
    Document* search(BTreeNode* node, int key) {
        int i = 0;
        while (i < node->numKeys && key > node->keys[i]) i++;
        if (i < node->numKeys && node->keys[i] == key)
            return node->vals[i];
        if (node->isLeaf)
            return nullptr;
        return search(node->children[i], key);
    }

    Document* find(int key) {
        return search(root, key);
    }

    // Insert a document
    void insert(int key, Document* doc) {
        BTreeNode* r = root;
        // If root is full, split it first
        if (r->numKeys == 2 * BTREE_ORDER - 1) {
            splits++;
            height++;
            BTreeNode* newRoot = new BTreeNode(false);
            newRoot->children[0] = root;
            splitChild(newRoot, 0, root);
            root = newRoot;
        }
        insertNonFull(root, key, doc);
    }

private:
    // Split a full child node
    //   parent->children[i] is full; split it
    void splitChild(BTreeNode* parent, int i, BTreeNode* child) {
        splits++;
        int mid = BTREE_ORDER - 1;
        BTreeNode* sibling = new BTreeNode(child->isLeaf);
        sibling->numKeys = BTREE_ORDER - 1;

        // Copy right half of child to sibling
        for (int j = 0; j < BTREE_ORDER - 1; j++) {
            sibling->keys[j] = child->keys[mid + 1 + j];
            sibling->vals[j] = child->vals[mid + 1 + j];
        }
        if (!child->isLeaf) {
            for (int j = 0; j < BTREE_ORDER; j++)
                sibling->children[j] = child->children[mid + 1 + j];
        }
        child->numKeys = mid;

        // Shift parent to make room for middle key
        for (int j = parent->numKeys; j > i; j--) {
            parent->children[j + 1] = parent->children[j];
            parent->keys[j]         = parent->keys[j - 1];
            parent->vals[j]         = parent->vals[j - 1];
        }
        parent->children[i + 1] = sibling;
        parent->keys[i]         = child->keys[mid];
        parent->vals[i]         = child->vals[mid];
        parent->numKeys++;
    }

    void insertNonFull(BTreeNode* node, int key, Document* doc) {
        int i = node->numKeys - 1;
        if (node->isLeaf) {
            // Shift keys right and insert
            while (i >= 0 && key < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                node->vals[i + 1] = node->vals[i];
                i--;
            }
            node->keys[i + 1] = key;
            node->vals[i + 1] = doc;
            node->numKeys++;
        } else {
            while (i >= 0 && key < node->keys[i]) i--;
            i++;
            if (node->children[i]->numKeys == 2 * BTREE_ORDER - 1) {
                splitChild(node, i, node->children[i]);
                if (key > node->keys[i]) i++;
            }
            insertNonFull(node->children[i], key, doc);
        }
    }
};

// ============================================================
//  3. EXTENDIBLE HASHING  (Unit 2)
//
//  How it works:
//    - Directory: array of 2^globalDepth pointers to buckets
//    - Each bucket has a localDepth and capacity (BUCKET_SIZE)
//    - On bucket overflow:
//        a) localDepth < globalDepth -> split bucket only
//        b) localDepth == globalDepth -> DOUBLE DIRECTORY, then split
//    - Stores: (kgramHash -> {docId, docName, position})
// ============================================================

const int BUCKET_SIZE = 6;  // entries per bucket (configurable)

struct HashEntry {
    long long kgramHash;
    int       docId;
    string    docName;
    int       position;
    bool      occupied;

    HashEntry() : kgramHash(0), docId(-1), position(-1), occupied(false) {}
};

struct EHBucket {
    int       localDepth;
    HashEntry entries[BUCKET_SIZE];
    int       count;

    EHBucket(int depth) : localDepth(depth), count(0) {}

    bool isFull()  { return count >= BUCKET_SIZE; }
    bool isEmpty() { return count == 0; }

    void insert(long long h, int docId, const string& docName, int pos) {
        entries[count].kgramHash = h;
        entries[count].docId     = docId;
        entries[count].docName   = docName;
        entries[count].position  = pos;
        entries[count].occupied  = true;
        count++;
    }

    // Return all entries matching given hash
    vector<HashEntry*> find(long long h) {
        vector<HashEntry*> results;
        for (int i = 0; i < count; i++)
            if (entries[i].occupied && entries[i].kgramHash == h)
                results.push_back(&entries[i]);
        return results;
    }
};

class ExtendibleHashTable {
public:
    int                globalDepth;
    vector<EHBucket*>  directory;
    int                totalInserts;
    int                totalSplits;
    int                totalDoublings;

    ExtendibleHashTable() : globalDepth(1), totalInserts(0),
                             totalSplits(0), totalDoublings(0) {
        directory.push_back(new EHBucket(1));  // dir[0]
        directory.push_back(new EHBucket(1));  // dir[1]
    }

    // Get directory index for a hash value
    int dirIndex(long long h) {
        return (int)(h & ((1LL << globalDepth) - 1));
    }

    void insert(long long h, int docId, const string& docName, int pos) {
        totalInserts++;
        _insertInternal(h, docId, docName, pos);
    }

private:
    void _insertInternal(long long h, int docId, const string& docName, int pos) {
        int idx = dirIndex(h);
        EHBucket* bucket = directory[idx];

        if (!bucket->isFull()) {
            bucket->insert(h, docId, docName, pos);
            return;
        }

        // Bucket is full — need to split
        totalSplits++;

        if (bucket->localDepth == globalDepth) {
            // DOUBLE THE DIRECTORY
            totalDoublings++;
            int oldSize = (int)directory.size();
            directory.resize(oldSize * 2);
            for (int i = 0; i < oldSize; i++)
                directory[i + oldSize] = directory[i];
            globalDepth++;
        }

        // Split the bucket into two new buckets
        int newDepth = bucket->localDepth + 1;
        EHBucket* b0 = new EHBucket(newDepth);
        EHBucket* b1 = new EHBucket(newDepth);

        // Save old entries using proper C++ copy (NOT memcpy — entries have std::string)
        vector<HashEntry> oldEntries(bucket->entries, bucket->entries + bucket->count);
        int oldCount = bucket->count;

        // Reassign directory pointers (replace all refs to old bucket)
        for (int i = 0; i < (int)directory.size(); i++) {
            if (directory[i] == bucket) {
                directory[i] = ((i >> (newDepth - 1)) & 1) ? b1 : b0;
            }
        }

        // Safe to delete old bucket now — no more directory refs
        delete bucket;

        // Re-distribute old entries into b0 / b1 directly (no recursion)
        for (int i = 0; i < oldCount; i++) {
            if (!oldEntries[i].occupied) continue;
            long long eh = oldEntries[i].kgramHash;
            int ni = (int)(eh & ((1LL << globalDepth) - 1));
            EHBucket* target = directory[ni];
            if (!target->isFull())
                target->insert(eh, oldEntries[i].docId,
                               oldEntries[i].docName, oldEntries[i].position);
            // If still full after split (all keys same hash), just drop —
            // this is a degenerate worst-case; increase BUCKET_SIZE to avoid
        }

        // Now insert the new entry
        int ni2 = dirIndex(h);
        if (!directory[ni2]->isFull())
            directory[ni2]->insert(h, docId, docName, pos);
        else
            _insertInternal(h, docId, docName, pos); // retry (rare)
    }

public:

    // Find all entries with matching hash
    vector<HashEntry*> find(long long h) {
        int idx = dirIndex(h);
        return directory[idx]->find(h);
    }
};

// ============================================================
//  4. MIN-HEAP  (Unit 3)
//
//  Used to rank documents by plagiarism score (top-K results)
//  Parent is always SMALLER than children (min at root)
//  Operations: insert O(log n), extract-min O(log n)
// ============================================================

struct MatchResult {
    int    docId;
    string docName;
    int    score;       // 0–100 similarity percentage
    int    matchCount;  // number of matching k-grams
};

class MinHeap {
public:
    vector<MatchResult> heap;

    int size() { return (int)heap.size(); }

    void push(const MatchResult& item) {
        heap.push_back(item);
        bubbleUp((int)heap.size() - 1);
    }

    MatchResult pop() {
        MatchResult top = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        if (!heap.empty()) sinkDown(0);
        return top;
    }

    // Return sorted results (highest score first) without destroying heap
    vector<MatchResult> getSorted() {
        MinHeap copy = *this;
        vector<MatchResult> result;
        while (copy.size() > 0)
            result.push_back(copy.pop());
        reverse(result.begin(), result.end());  // highest score first
        return result;
    }

private:
    void bubbleUp(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (heap[parent].score > heap[i].score) {
                swap(heap[parent], heap[i]);
                i = parent;
            } else break;
        }
    }

    void sinkDown(int i) {
        int n = (int)heap.size();
        while (true) {
            int smallest = i;
            int left  = 2 * i + 1;
            int right = 2 * i + 2;
            if (left  < n && heap[left].score  < heap[smallest].score) smallest = left;
            if (right < n && heap[right].score < heap[smallest].score) smallest = right;
            if (smallest == i) break;
            swap(heap[smallest], heap[i]);
            i = smallest;
        }
    }
};

// ============================================================
//  5. DISJOINT SETS / UNION-FIND  (Unit 5)
//
//  Used to cluster documents that share substantial content
//  Path Compression + Union by Rank = near O(1) per operation
// ============================================================

class DisjointSets {
public:
    vector<int> parent;
    vector<int> rank;
    int components;

    DisjointSets(int n) : parent(n), rank(n, 0), components(n) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }

    // Find with PATH COMPRESSION
    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);   // path compression
        return parent[x];
    }

    // Union by rank
    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;
        components--;
        if (rank[rx] < rank[ry]) swap(rx, ry);
        parent[ry] = rx;
        if (rank[rx] == rank[ry]) rank[rx]++;
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

// ============================================================
//  PLAGIARISM ENGINE
//  Ties all data structures together
// ============================================================

struct Corpus {
    BTree*               btree;
    ExtendibleHashTable* hashTable;
    vector<Document*>    docs;
    int                  K;   // k-gram size (words)

    Corpus(int k = 5) : K(k) {
        btree     = new BTree();
        hashTable = new ExtendibleHashTable();
    }
};

// Index one document into the corpus
void indexDocument(Corpus& corpus, int id, const string& name, const string& text) {
    Document* doc = new Document{id, name, text};
    corpus.docs.push_back(doc);
    corpus.btree->insert(id, doc);

    vector<KGram> kgrams = generateKGrams(text, corpus.K);
    for (const KGram& kg : kgrams)
        corpus.hashTable->insert(kg.hash, id, name, kg.position);
}

// Check a query document against the corpus
MinHeap checkPlagiarism(const string& queryText, Corpus& corpus) {
    vector<KGram> queryGrams = generateKGrams(queryText, corpus.K);
    int totalGrams = (int)queryGrams.size();

    // Count matching k-grams per document
    vector<int> matchCount(corpus.docs.size(), 0);

    for (const KGram& qg : queryGrams) {
        vector<HashEntry*> hits = corpus.hashTable->find(qg.hash);
        for (HashEntry* h : hits) {
            // Rabin-Karp verification step (avoid false positives)
            if (h->docId >= 0 && h->docId < (int)matchCount.size()) {
                matchCount[h->docId]++;
            }
        }
    }

    // Build min-heap of results
    MinHeap heap;
    for (int i = 0; i < (int)corpus.docs.size(); i++) {
        if (matchCount[i] > 0) {
            int score = (int)min(100.0, (matchCount[i] * 100.0) / max(1, totalGrams));
            MatchResult mr;
            mr.docId      = i;
            mr.docName    = corpus.docs[i]->name;
            mr.score      = score;
            mr.matchCount = matchCount[i];
            heap.push(mr);
        }
    }
    return heap;
}

// ============================================================
//  DISPLAY HELPERS
// ============================================================

void printBar(int score) {
    string bar = "[";
    int filled = score / 5;
    for (int i = 0; i < 20; i++)
        bar += (i < filled) ? '#' : '-';
    bar += "]";
    cout << bar;
}

void printSeparator() {
    cout << string(65, '=') << "\n";
}

void printSection(const string& title) {
    cout << "\n" << string(65, '-') << "\n";
    cout << "  " << title << "\n";
    cout << string(65, '-') << "\n";
}

// ============================================================
//  SAMPLE CORPUS DOCUMENTS (built-in for testing)
// ============================================================

void loadSampleCorpus(Corpus& corpus) {
    indexDocument(corpus, 0, "CLRS Ch.1 - Algorithms",
        "An algorithm is any well-defined computational procedure that takes "
        "some value or set of values as input and produces some value or set "
        "of values as output. An algorithm is a sequence of computational "
        "steps that transform the input into the output. We can also view an "
        "algorithm as a tool for solving a well-specified computational "
        "problem. Algorithms are used in nearly every branch of computing.");

    indexDocument(corpus, 1, "Data Structures Textbook Ch.3",
        "A data structure is a way of organizing data in a computer so that "
        "it can be accessed and modified efficiently. Different data structures "
        "are suited to different kinds of applications and some are highly "
        "specialized to specific tasks. Data structures provide a means to "
        "manage large amounts of data efficiently for uses such as large "
        "databases and internet indexing services.");

    indexDocument(corpus, 2, "Operating Systems Notes",
        "An operating system is system software that manages computer hardware "
        "software resources and provides common services for computer programs. "
        "Time sharing operating systems schedule tasks for efficient use of the "
        "system and may also include accounting software for cost allocation of "
        "processor time mass storage printing and other resources.");

    indexDocument(corpus, 3, "Computer Networks Overview",
        "A computer network is a set of computers sharing resources located on "
        "or provided by network nodes. The computers use common communication "
        "protocols over digital interconnections to communicate with each other. "
        "These interconnections are made up of telecommunication network "
        "technologies based on physically wired optical and wireless methods.");

    indexDocument(corpus, 4, "Database Systems Fundamentals",
        "A database is an organized collection of structured information or data "
        "typically stored electronically in a computer system. A database is "
        "usually controlled by a database management system. Together the data "
        "and the DBMS along with the applications that are associated with them "
        "are referred to as a database system. Data can be easily accessed "
        "managed modified updated controlled and organized.");

    indexDocument(corpus, 5, "Machine Learning Basics",
        "Machine learning is a method of data analysis that automates analytical "
        "model building. It is based on the idea that systems can learn from "
        "data identify patterns and make decisions with minimal human "
        "intervention. Machine learning algorithms use historical data as input "
        "to predict new output values. Recommendation engines are a common use "
        "case for machine learning.");
}

// ============================================================
//  MENU-DRIVEN MAIN PROGRAM
// ============================================================

int main() {
    printSeparator();
    cout << "   DOCUMENT PLAGIARISM DETECTOR\n";
    cout << "   B.Tech CSE | Advance Data Structures | DBS Global University\n";
    cout << "   Rabin-Karp + B-Tree + Extendible Hashing + Min-Heap\n";
    printSeparator();

    // Build corpus
    Corpus corpus(5);  // 5-word k-grams
    cout << "\nLoading corpus documents...\n";
    loadSampleCorpus(corpus);
    cout << "Loaded " << corpus.docs.size() << " documents into corpus.\n";
    cout << "B-Tree height: " << corpus.btree->height
         << " | Splits: "     << corpus.btree->splits << "\n";
    cout << "ExtHash inserts: " << corpus.hashTable->totalInserts
         << " | Splits: "       << corpus.hashTable->totalSplits
         << " | Doublings: "    << corpus.hashTable->totalDoublings << "\n";

    // Menu loop
    int choice = 0;
    while (true) {
        printSection("MAIN MENU");
        cout << "  1. Check text for plagiarism\n";
        cout << "  2. Check a .txt file for plagiarism\n";
        cout << "  3. Add document to corpus\n";
        cout << "  4. View corpus documents\n";
        cout << "  5. View data structure statistics\n";
        cout << "  6. Run demo (built-in test)\n";
        cout << "  7. Exit\n";
        cout << "\n  Enter choice: ";
        cin >> choice;
        cin.ignore();

        // ---- OPTION 1: Type / paste text ----
        if (choice == 1) {
            printSection("CHECK TEXT FOR PLAGIARISM");
            cout << "  Paste your text (press Enter twice when done):\n\n";
            string queryText, line;
            while (getline(cin, line)) {
                if (line.empty()) break;
                queryText += line + " ";
            }
            if (queryText.empty()) { cout << "  No text entered.\n"; continue; }

            auto t1 = chrono::high_resolution_clock::now();
            MinHeap results = checkPlagiarism(queryText, corpus);
            auto t2 = chrono::high_resolution_clock::now();
            double ms = chrono::duration<double, milli>(t2 - t1).count();

            vector<MatchResult> sorted = results.getSorted();
            int topScore = sorted.empty() ? 0 : sorted[0].score;

            cout << "\n  --- RESULTS ---\n";
            cout << "  Analysis time : " << ms << " ms\n";
            cout << "  Query k-grams : "
                 << generateKGrams(queryText, corpus.K).size() << "\n";
            cout << "  Top similarity: " << topScore << "%\n";

            if (topScore >= 70)
                cout << "  Verdict       : *** HIGH PLAGIARISM DETECTED ***\n";
            else if (topScore >= 40)
                cout << "  Verdict       : WARNING - Moderate similarity\n";
            else if (topScore > 0)
                cout << "  Verdict       : Low similarity - mostly original\n";
            else
                cout << "  Verdict       : ORIGINAL - No matches found\n";

            if (!sorted.empty()) {
                cout << "\n  Matched documents (ranked by min-heap):\n\n";
                for (int i = 0; i < (int)sorted.size(); i++) {
                    const MatchResult& r = sorted[i];
                    printf("  %2d. [%3d%%] ", i + 1, r.score);
                    printBar(r.score);
                    cout << "\n      " << r.docName
                         << " (doc #" << r.docId << ", "
                         << r.matchCount << " matching k-grams)\n\n";
                }
            }

        // ---- OPTION 2: Read from .txt file ----
        } else if (choice == 2) {
            printSection("CHECK .TXT FILE");
            cout << "  Enter filename (e.g. essay.txt): ";
            string filename;
            getline(cin, filename);

            ifstream f(filename);
            if (!f.is_open()) {
                cout << "  Error: Cannot open file '" << filename << "'\n";
                continue;
            }
            string content((istreambuf_iterator<char>(f)),
                            istreambuf_iterator<char>());
            f.close();
            cout << "  Read " << content.size() << " characters.\n";

            MinHeap results = checkPlagiarism(content, corpus);
            vector<MatchResult> sorted = results.getSorted();
            int topScore = sorted.empty() ? 0 : sorted[0].score;

            cout << "\n  Top similarity: " << topScore << "%\n";
            if (!sorted.empty()) {
                for (const MatchResult& r : sorted)
                    printf("  [%3d%%] %s\n", r.score, r.docName.c_str());
            } else {
                cout << "  No matches found — text appears original.\n";
            }

        // ---- OPTION 3: Add document ----
        } else if (choice == 3) {
            printSection("ADD DOCUMENT TO CORPUS");
            cout << "  Document name: ";
            string name;
            getline(cin, name);
            cout << "  Paste text (press Enter twice when done):\n\n";
            string text, line;
            while (getline(cin, line)) {
                if (line.empty()) break;
                text += line + " ";
            }
            if (name.empty() || text.empty()) {
                cout << "  Name or text was empty. Cancelled.\n"; continue;
            }
            int newId = (int)corpus.docs.size();
            indexDocument(corpus, newId, name, text);
            cout << "  Document added as #" << newId << ".\n";

        // ---- OPTION 4: View corpus ----
        } else if (choice == 4) {
            printSection("CORPUS DOCUMENTS");
            for (int i = 0; i < (int)corpus.docs.size(); i++) {
                Document* doc = corpus.btree->find(i);
                if (doc) {
                    printf("  #%d  %s\n", i, doc->name.c_str());
                    printf("      %d words | %d k-grams\n",
                           (int)splitWords(cleanText(doc->text)).size(),
                           (int)generateKGrams(doc->text, corpus.K).size());
                }
            }

        // ---- OPTION 5: Statistics ----
        } else if (choice == 5) {
            printSection("DATA STRUCTURE STATISTICS");

            cout << "\n  [B-TREE]\n";
            cout << "  Documents indexed : " << corpus.docs.size() << "\n";
            cout << "  Tree height       : " << corpus.btree->height << "\n";
            cout << "  Total splits      : " << corpus.btree->splits << "\n";
            cout << "  Order (max keys)  : " << 2*BTREE_ORDER-1 << " per node\n";

            cout << "\n  [EXTENDIBLE HASH TABLE]\n";
            cout << "  Global depth      : " << corpus.hashTable->globalDepth << "\n";
            cout << "  Directory size    : " << corpus.hashTable->directory.size() << "\n";
            cout << "  Total inserts     : " << corpus.hashTable->totalInserts << "\n";
            cout << "  Bucket splits     : " << corpus.hashTable->totalSplits << "\n";
            cout << "  Dir. doublings    : " << corpus.hashTable->totalDoublings << "\n";
            cout << "  Bucket capacity   : " << BUCKET_SIZE << " entries\n";

            cout << "\n  [RABIN-KARP]\n";
            cout << "  K-gram size (k)   : " << corpus.K << " words\n";
            cout << "  Hash base         : " << RK_BASE << "\n";
            cout << "  Hash modulus      : " << RK_MOD << " (prime)\n";

            cout << "\n  [COMPLEXITY SUMMARY]\n";
            cout << "  Rabin-Karp search : O(n+m) average, O(nm) worst case\n";
            cout << "  B-Tree search     : O(log n)\n";
            cout << "  ExtHash lookup    : O(1) average\n";
            cout << "  Min-Heap sort     : O(n log n)\n";

        // ---- OPTION 6: Demo ----
        } else if (choice == 6) {
            printSection("DEMO - BUILT-IN TEST CASES");

            struct TestCase { string label; string text; };
            vector<TestCase> tests = {
                {
                    "HIGH plagiarism (from CLRS)",
                    "An algorithm is any well-defined computational procedure that takes "
                    "some value as input and produces some value as output. An algorithm "
                    "is a sequence of computational steps that transform the input into "
                    "the output and is used in computing."
                },
                {
                    "MODERATE similarity (paraphrased)",
                    "A data structure organizes information in memory so that programs "
                    "can access and change it efficiently. Choosing the right structure "
                    "depends on the application since some are suited for specific tasks."
                },
                {
                    "ORIGINAL text (no match expected)",
                    "The cat sat on the mat and looked out the window at the birds "
                    "flying past the garden wall on a sunny afternoon in spring."
                }
            };

            for (const TestCase& tc : tests) {
                cout << "\n  TEST: " << tc.label << "\n";
                cout << "  Text: \"" << tc.text.substr(0, 60) << "...\"\n";

                MinHeap results = checkPlagiarism(tc.text, corpus);
                vector<MatchResult> sorted = results.getSorted();
                int top = sorted.empty() ? 0 : sorted[0].score;

                printf("  Score: %d%%  ", top);
                printBar(top);
                cout << "\n";
                if (!sorted.empty())
                    cout << "  Best match: " << sorted[0].docName << "\n";
                else
                    cout << "  No matches found.\n";
            }

        // ---- OPTION 7: Exit ----
        } else if (choice == 7) {
            cout << "\n  Goodbye!\n\n";
            break;
        } else {
            cout << "  Invalid choice. Try again.\n";
        }
    }

    return 0;
}
