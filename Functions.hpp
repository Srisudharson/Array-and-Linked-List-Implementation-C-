#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits> // Required for numeric_limits
#include <iomanip> // Required for setw
#include <algorithm> // For min in displayTransactions
#include "json.hpp" // For nlohmann/json library
#include "DatasetHandler.hpp"
using json = nlohmann::json;
using namespace std;

// --- Custom Dynamic Array Class (from previous iteration) ---
class CustomArray {
private:
    Transaction* data;
    int capacity;
    int currentSize;

    void resize(int newCapacity) {
        if (newCapacity < currentSize) {
            newCapacity = currentSize;
        }
        if (newCapacity == 0) {
            delete[] data;
            data = nullptr;
            capacity = 0;
            return;
        }

        Transaction* newData = new Transaction[newCapacity];
        for (int i = 0; i < currentSize; ++i) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    CustomArray() : data(nullptr), capacity(0), currentSize(0) {}

    // Destructor
    ~CustomArray() {
        delete[] data;
    }

    // Copy constructor
    CustomArray(const CustomArray& other) : data(nullptr), capacity(0), currentSize(0) {
        if (other.data != nullptr) {
            capacity = other.capacity;
            currentSize = other.currentSize;
            data = new Transaction[capacity];
            for (int i = 0; i < currentSize; ++i) {
                data[i] = other.data[i];
            }
        }
    }

    // Copy assignment operator
    CustomArray& operator=(const CustomArray& other) {
        if (this != &other) {
            delete[] data;
            data = nullptr;
            capacity = 0;
            currentSize = 0;

            if (other.data != nullptr) {
                capacity = other.capacity;
                currentSize = other.currentSize;
                data = new Transaction[capacity];
                for (int i = 0; i < currentSize; ++i) {
                    data[i] = other.data[i];
                }
            }
        }
        return *this;
    }

    void add(const Transaction& item) {
        if (currentSize == capacity) {
            int newCapacity = capacity == 0 ? 100 : capacity + 1000;
            resize(newCapacity);
        }
        data[currentSize++] = item;
    }

    Transaction& get(int index) {
        if (index < 0 || index >= currentSize) {
            throw out_of_range("Index out of bounds");
        }
        return data[index];
    }
    const Transaction& get(int index) const {
        if (index < 0 || index >= currentSize) {
            throw out_of_range("Index out of bounds");
        }
        return data[index];
    }

    int size() const {
        return currentSize;
    }

    bool isEmpty() const {
        return currentSize == 0;
    }

    // Operator[] for easy access
    Transaction& operator[](int index) {
        return get(index);
    }
    const Transaction& operator[](int index) const {
        return get(index);
    }
};

// --- Sorting Algorithms for CustomArray (from previous iteration) ---

// Bubble Sort for CustomArray based on payment_channel
void bubbleSort(CustomArray& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j].payment_channel > arr[j + 1].payment_channel) {
                Transaction temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

// Merge function for sorting by transaction type
CustomArray merge(const CustomArray& left, const CustomArray& right) {
    CustomArray result;
    int i = 0, j = 0;

    while (i < left.size() && j < right.size()) {
        if (left[i].transaction_type <= right[j].transaction_type) {
            result.add(left[i]);
            i++;
        } else {
            result.add(right[j]);
            j++;
        }
    }

    while (i < left.size()) {
        result.add(left[i]);
        i++;
    }

    while (j < right.size()) {
        result.add(right[j]);
        j++;
    }

    return result;
}

// Merge Sort for CustomArray based on transaction_type
CustomArray mergeSortByTransactionType(const CustomArray& arr) {
    if (arr.size() <= 1) {
        return arr;
    }

    int mid = arr.size() / 2;
    CustomArray left, right;

    for (int i = 0; i < mid; ++i) {
        left.add(arr[i]);
    }
    for (int i = mid; i < arr.size(); ++i) {
        right.add(arr[i]);
    }

    left = mergeSortByTransactionType(left);
    right = mergeSortByTransactionType(right);

    return merge(left, right);
}

// --- Searching Algorithms for CustomArray (from previous iteration) ---

// Linear Search for CustomArray by transaction_type
CustomArray linearSearchByTransactionType(const CustomArray& arr, const string& transactionType) {
    CustomArray results;
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i].transaction_type == transactionType) {
            results.add(arr[i]);
        }
    }
    return results;
}


// --- Custom Linked List Implementation ---

struct Node {
    Transaction data;
    Node* next;

    Node(const Transaction& val) : data(val), next(nullptr) {}
};

class CustomLinkedList {
private:
    Node* head;
    int currentSize;

public:
    CustomLinkedList() : head(nullptr), currentSize(0) {}

    // Destructor to free memory
    ~CustomLinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        currentSize = 0;
    }

    // Copy constructor
    CustomLinkedList(const CustomLinkedList& other) : head(nullptr), currentSize(0) {
        if (other.head == nullptr) return;

        Node* currentOther = other.head;
        Node* tail = nullptr;

        while (currentOther != nullptr) {
            Node* newNode = new Node(currentOther->data);
            if (head == nullptr) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
            currentOther = currentOther->next;
            currentSize++;
        }
    }

    // Copy assignment operator
    CustomLinkedList& operator=(const CustomLinkedList& other) {
        if (this == &other) return *this; // Handle self-assignment

        // Clear existing list
        Node* current = head;
        while (current != nullptr) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
        head = nullptr;
        currentSize = 0;

        // Copy elements from other
        if (other.head == nullptr) return *this;

        Node* currentOther = other.head;
        Node* tail = nullptr;

        while (currentOther != nullptr) {
            Node* newNode = new Node(currentOther->data);
            if (head == nullptr) {
                head = newNode;
                tail = newNode;
            } else {
                tail->next = newNode;
                tail = newNode;
            }
            currentOther = currentOther->next;
            currentSize++;
        }
        return *this;
    }

    // Add to the end (for general purpose, not sorted insertion)
    void add(const Transaction& item) {
        Node* newNode = new Node(item);
        if (head == nullptr) {
            head = newNode;
        } else {
            Node* current = head;
            while (current->next != nullptr) {
                current = current->next;
            }
            current->next = newNode;
        }
        currentSize++;
    }

    // Special insertion for sorting by location alphabetically
    // This is the "alphabetical insertion" you described
    void insertSortedByLocation(const Transaction& item) {
        Node* newNode = new Node(item);

        // If list is empty or new node should be the new head
        if (head == nullptr || item.location < head->data.location) {
            newNode->next = head;
            head = newNode;
        } else {
            Node* current = head;
            // Traverse to find the correct insertion point
            // Stop when current->next is null OR new item's location is less than current->next's location
            while (current->next != nullptr && current->next->data.location < item.location) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
        currentSize++;
    }

    int size() const {
        return currentSize;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    Node* getHead() const {
        return head;
    }

    // Merge Sort for Linked List (by location)
    // Helper function to split the list into two halves
    void splitList(Node* source, Node** frontRef, Node** backRef) {
        Node* fast;
        Node* slow;
        slow = source;
        fast = source->next;

        // Advance 'fast' by two nodes, and 'slow' by one node
        while (fast != nullptr) {
            fast = fast->next;
            if (fast != nullptr) {
                slow = slow->next;
                fast = fast->next;
            }
        }

        // 'slow' is before the midpoint in the list, so split it in two
        *frontRef = source;
        *backRef = slow->next;
        slow->next = nullptr; // Break the link
    }

    // Helper function to merge two sorted lists
    Node* sortedMerge(Node* a, Node* b) {
        Node* result = nullptr;

        // Base cases
        if (a == nullptr) return b;
        if (b == nullptr) return a;

        // Pick either a or b, and recur
        if (a->data.location <= b->data.location) {
            result = a;
            result->next = sortedMerge(a->next, b);
        } else {
            result = b;
            result->next = sortedMerge(a, b->next);
        }
        return result;
    }

    // The main function that sorts the linked list using Merge Sort
    void mergeSort(Node** headRef) {
        Node* currentHead = *headRef;
        Node* a;
        Node* b;

        // Base case: if list is empty or has only one node
        if ((currentHead == nullptr) || (currentHead->next == nullptr)) {
            return;
        }

        // Split currentHead into 'a' and 'b' sublists
        splitList(currentHead, &a, &b);

        // Recursively sort the sublists
        mergeSort(&a);
        mergeSort(&b);

        // Merge the two sorted sublists
        *headRef = sortedMerge(a, b);
    }

    // Public method to initiate merge sort
    void sortListByLocation() {
        mergeSort(&head);
    }

    // Binary Search for Linked List (by location)
    // Note: Binary search on a linked list is not efficient (O(N) to find middle),
    // but implemented as requested.
    CustomArray binarySearchByLocation(const string& searchLocation) const {
        CustomArray results;
        if (head == nullptr) return results;

        // Convert linked list to a temporary CustomArray for efficient binary search
        // This is a common practical approach if binary search is truly needed on a linked list
        // without modifying the list structure or making it doubly linked with random access.
        // If the list is already sorted, this conversion is O(N).
        CustomArray tempArray;
        Node* current = head;
        while(current != nullptr) {
            tempArray.add(current->data);
            current = current->next;
        }

        // Now perform binary search on the temporary array
        int low = 0;
        int high = tempArray.size() - 1;
        bool found = false;

        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (tempArray[mid].location == searchLocation) {
                // Found one, now collect all occurrences (since there can be duplicates)
                found = true;
                // Collect from mid backwards
                int i = mid;
                while (i >= 0 && tempArray[i].location == searchLocation) {
                    results.add(tempArray[i]);
                    i--;
                }
                // Collect from mid forwards
                i = mid + 1;
                while (i < tempArray.size() && tempArray[i].location == searchLocation) {
                    results.add(tempArray[i]);
                    i++;
                }
                break; // All found
            } else if (tempArray[mid].location < searchLocation) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }
        return results;
    }
};

// Function to read CSV and populate CustomArray instances (from previous iteration)
void readCsvToCustomArray(const string& filename,
                          CustomArray& card_transactions,
                          CustomArray& wire_transfer_transactions,
                          CustomArray& upi_transactions,
                          CustomArray& ach_transactions,
                          int max_load_count) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header

    int lines_read = 0;
    while (getline(file, line) && (max_load_count == -1 || lines_read < max_load_count)) {
        stringstream ss(line);
        string segment;
        Transaction t;

        getline(ss, t.transaction_id, ',');
        getline(ss, t.timestamp, ',');
        getline(ss, t.sender_account, ',');
        getline(ss, t.receiver_account, ',');
        getline(ss, segment, ','); try { t.amount = stod(segment); } catch (...) { t.amount = 0.0; }
        getline(ss, t.transaction_type, ',');
        getline(ss, t.merchant_category, ',');
        getline(ss, t.location, ',');
        getline(ss, t.device_used, ',');
        getline(ss, segment, ','); t.is_fraud = (segment == "TRUE" || segment == "true" || segment == "1");
        getline(ss, t.fraud_type, ',');
        getline(ss, segment, ','); try { t.time_since_last_transaction = stod(segment); } catch (...) { t.time_since_last_transaction = 0.0; }
        getline(ss, segment, ','); try { t.spending_deviation_score = stod(segment); } catch (...) { t.spending_deviation_score = 0.0; }
        getline(ss, segment, ','); try { t.velocity_score = stod(segment); } catch (...) { t.velocity_score = 0.0; }
        getline(ss, segment, ','); try { t.geo_anomaly_score = stod(segment); } catch (...) { t.geo_anomaly_score = 0.0; }
        getline(ss, t.payment_channel, ',');
        getline(ss, t.ip_address, ',');
        getline(ss, t.device_hash, '\r');

        if (t.payment_channel == "card") {
            card_transactions.add(t);
        } else if (t.payment_channel == "wire_transfer") {
            wire_transfer_transactions.add(t);
        } else if (t.payment_channel == "UPI") {
            upi_transactions.add(t);
        } else if (t.payment_channel == "ACH") {
            ach_transactions.add(t);
        }
        lines_read++;
    }
    file.close();
    cout << "Data loaded and separated into custom array-based collections. (Read " << lines_read << " lines)" << endl;
}

// Function to read CSV and populate CustomLinkedList (for location-based sorting)
void readCsvToCustomLinkedList(const string& filename,
                               CustomLinkedList& location_transactions,
                               int max_load_count) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << endl;
        return;
    }

    string line;
    getline(file, line); // Skip header

    int lines_read = 0;
    while (getline(file, line) && (max_load_count == -1 || lines_read < max_load_count)) {
        stringstream ss(line);
        string segment;
        Transaction t;

        getline(ss, t.transaction_id, ',');
        getline(ss, t.timestamp, ',');
        getline(ss, t.sender_account, ',');
        getline(ss, t.receiver_account, ',');
        getline(ss, segment, ','); try { t.amount = stod(segment); } catch (...) { t.amount = 0.0; }
        getline(ss, t.transaction_type, ',');
        getline(ss, t.merchant_category, ',');
        getline(ss, t.location, ','); // This is the key for this list
        getline(ss, t.device_used, ',');
        getline(ss, segment, ','); t.is_fraud = (segment == "TRUE" || segment == "true" || segment == "1");
        getline(ss, t.fraud_type, ',');
        getline(ss, segment, ','); try { t.time_since_last_transaction = stod(segment); } catch (...) { t.time_since_last_transaction = 0.0; }
        getline(ss, segment, ','); try { t.spending_deviation_score = stod(segment); } catch (...) { t.spending_deviation_score = 0.0; }
        getline(ss, segment, ','); try { t.velocity_score = stod(segment); } catch (...) { t.velocity_score = 0.0; }
        getline(ss, segment, ','); try { t.geo_anomaly_score = stod(segment); } catch (...) { t.geo_anomaly_score = 0.0; }
        getline(ss, t.payment_channel, ',');
        getline(ss, t.ip_address, ',');
        getline(ss, t.device_hash, '\r');

        // Insert directly into the linked list in sorted order by location
        location_transactions.insertSortedByLocation(t);
        lines_read++;
    }
    file.close();
    cout << "Data loaded and separated into custom linked list (sorted by location). (Read " << lines_read << " lines)" << endl;
}

// Function to export a CustomArray of transactions to JSON (from previous iteration)
void exportToJson(const string& filename, const CustomArray& transactions) {
    nlohmann::json j_array;
    for (int i = 0; i < transactions.size(); ++i) {
        j_array.push_back(transactions[i]);
    }

    ofstream o(filename);
    if (o.is_open()) {
        o << setw(4) << j_array << endl;
        o.close();
        cout << "Data successfully exported to " << filename << endl;
    } else {
        cerr << "Error: Could not open file for writing JSON: " << filename << endl;
    }
}

// Function to export a CustomLinkedList of transactions to JSON
void exportToJson(const string& filename, const CustomLinkedList& transactions) {
    nlohmann::json j_array;
    Node* current = transactions.getHead();
    while (current != nullptr) {
        j_array.push_back(current->data);
        current = current->next;
    }

    ofstream o(filename);
    if (o.is_open()) {
        o << setw(4) << j_array << endl;
        o.close();
        cout << "Data successfully exported to " << filename << endl;
    } else {
        cerr << "Error: Could not open file for writing JSON: " << filename << endl;
    }
}

// Function to display transactions from a CustomArray (from previous iteration)
void displayTransactions(const CustomArray& arr, const string& channelName) {
    if (arr.isEmpty()) {
        cout << "No " << channelName << " transactions to display." << endl;
        return;
    }
    cout << "\n--- " << channelName << " Transactions (" << arr.size() << " records) ---" << endl;
    int display_limit = min(arr.size(), 10);
    for (int i = 0; i < display_limit; ++i) {
        const Transaction& t = arr[i];
        cout << "ID: " << t.transaction_id
                  << ", Type: " << t.transaction_type
                  << ", Amount: " << t.amount
                  << ", Location: " << t.location
                  << ", Fraud: " << (t.is_fraud ? "Yes" : "No")
                  << ", Channel: " << t.payment_channel << endl;
    }
    if (arr.size() > display_limit) {
        cout << "... (" << arr.size() - display_limit << " more records not shown)" << endl;
    }
}

// Function to display transactions from a CustomLinkedList
void displayTransactions(const CustomLinkedList& list, const string& listName) {
    if (list.isEmpty()) {
        cout << "No " << listName << " transactions to display." << endl;
        return;
    }
    cout << "\n--- " << listName << " Transactions (" << list.size() << " records) ---" << endl;
    Node* current = list.getHead();
    int count = 0;
    int display_limit = min(list.size(), 10);
    while (current != nullptr && count < display_limit) {
        const Transaction& t = current->data;
        cout << "ID: " << t.transaction_id
                  << ", Type: " << t.transaction_type
                  << ", Amount: " << t.amount
                  << ", Location: " << t.location
                  << ", Fraud: " << (t.is_fraud ? "Yes" : "No")
                  << ", Channel: " << t.payment_channel << endl;
        current = current->next;
        count++;
    }
    if (list.size() > display_limit) {
        cout << "... (" << list.size() - display_limit << " more records not shown)" << endl;
    }
}

// Function to search transactions by type (array-based)
CustomArray searchByTransactionType(const CustomArray& arr, const string& transactionType) {
    CustomArray results;
    for (int i = 0; i < arr.size(); ++i) {
        if (arr[i].transaction_type == transactionType) {
            results.add(arr[i]);
        }
    }
    return results;
}

// Function to combine all transaction arrays into one
CustomArray combineAllTransactions(const CustomArray& card, const CustomArray& wire,
                                  const CustomArray& upi, const CustomArray& ach) {
    CustomArray all;
    for (int i = 0; i < card.size(); i++) all.add(card[i]);
    for (int i = 0; i < wire.size(); i++) all.add(wire[i]);
    for (int i = 0; i < upi.size(); i++) all.add(upi[i]);
    for (int i = 0; i < ach.size(); i++) all.add(ach[i]);
    return all;
}

