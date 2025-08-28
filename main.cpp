#include <iostream>
#include <fstream> 
#include <sstream>
#include <limits> 
#include <iomanip> 
#include <algorithm> 
#include "json.hpp" 
#include "Functions.hpp"
#include "DatasetHandler.hpp"
using json = nlohmann::json;
using namespace std;

int main() {
    // Array-based storage
    CustomArray card_transactions_arr;
    CustomArray wire_transfer_transactions_arr;
    CustomArray upi_transactions_arr;
    CustomArray ach_transactions_arr;

    // Linked List-based storage for location-based sorting
    CustomLinkedList location_transactions_list;

    string csv_filepath = "C:\\financial_fraud_detection_dataset.csv";
    const int MAX_LOAD_COUNT = 50000; // Limit to 1000 rows for initial testing
    // Automatically load data into array-based structures
    cout << "Loading data into array-based structures (max " << MAX_LOAD_COUNT << " rows)..." << endl;
    readCsvToCustomArray(csv_filepath, card_transactions_arr, wire_transfer_transactions_arr,
                         upi_transactions_arr, ach_transactions_arr, MAX_LOAD_COUNT);

    // Automatically load data into linked list, sorted by location during insertion
    cout << "\nLoading data into linked list (sorted by location during insertion, max " << MAX_LOAD_COUNT << " rows)..." << endl;
    readCsvToCustomLinkedList(csv_filepath, location_transactions_list, MAX_LOAD_COUNT);

    int choice;
    do {
        cout << "\n--- Financial Transaction Analysis Menu ---" << endl;
        cout << "  --- Array-Based Operations ---" << endl;
        cout << "1. Sort Transactions by Payment Channel (Array - Bubble Sort)" << endl;
        cout << "2. Search Transactions by Type (Array - Linear Search)" << endl;
        cout << "3. Export Array-Based Data to JSON" << endl;
        cout << "  --- Linked List-Based Operations ---" << endl;
        cout << "4. Sort Transactions by Location (Linked List - Merge Sort)" << endl;
        cout << "5. Search Transactions by Location (Linked List - Binary Search)" << endl;
        cout << "6. Export Linked List Data to JSON" << endl;
        cout << "  --- Transaction Type Operations ---" << endl;
        cout << "7. Search Transactions by Type (Array)" << endl;
        cout << "8. Sort Transactions by Type (Array - Merge Sort)" << endl;
        cout << "9. Export Transaction Type Data to JSON" << endl;
        cout << "  --- General Operations ---" << endl;
        cout << "10. Display Transactions (for verification)" << endl;
        cout << "11. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

        switch (choice) {
            case 1: { // Sort Array
                if (card_transactions_arr.isEmpty() && wire_transfer_transactions_arr.isEmpty() &&
                    upi_transactions_arr.isEmpty() && ach_transactions_arr.isEmpty()) {
                    cout << "No array data loaded to sort." << endl;
                    break;
                }
                int sort_channel_choice;
                cout << "\nSelect Payment Channel to Sort (Array):" << endl;
                cout << "  1. Card" << endl;
                cout << "  2. Wire Transfer" << endl;
                cout << "  3. UPI" << endl;
                cout << "  4. ACH" << endl;
                cout << "Enter your choice: ";
                cin >> sort_channel_choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                switch (sort_channel_choice) {
                    case 1: bubbleSort(card_transactions_arr); cout << "'Card' transactions sorted." << endl; break;
                    case 2: bubbleSort(wire_transfer_transactions_arr); cout << "'Wire Transfer' transactions sorted." << endl; break;
                    case 3: bubbleSort(upi_transactions_arr); cout << "'UPI' transactions sorted." << endl; break;
                    case 4: bubbleSort(ach_transactions_arr); cout << "'ACH' transactions sorted." << endl; break;
                    default: cout << "Invalid channel choice." << endl; break;
                }
                break;
            }
            case 2: { // Search Array
                if (card_transactions_arr.isEmpty() && wire_transfer_transactions_arr.isEmpty() &&
                    upi_transactions_arr.isEmpty() && ach_transactions_arr.isEmpty()) {
                    cout << "No array data loaded to search." << endl;
                    break;
                }
                string searchType;
                cout << "Enter transaction type to search for (e.g., 'purchase', 'withdrawal', 'deposit'): ";
                getline(cin, searchType);

                cout << "\n--- Search Results for '" << searchType << "' (Array) ---" << endl;
                displayTransactions(linearSearchByTransactionType(card_transactions_arr, searchType), "Search Results (Card)");
                displayTransactions(linearSearchByTransactionType(wire_transfer_transactions_arr, searchType), "Search Results (Wire Transfer)");
                displayTransactions(linearSearchByTransactionType(upi_transactions_arr, searchType), "Search Results (UPI)");
                displayTransactions(linearSearchByTransactionType(ach_transactions_arr, searchType), "Search Results (ACH)");
                break;
            }
            case 3: { // Export Array
                if (card_transactions_arr.isEmpty() && wire_transfer_transactions_arr.isEmpty() &&
                    upi_transactions_arr.isEmpty() && ach_transactions_arr.isEmpty()) {
                    cout << "No array data loaded to export." << endl;
                    break;
                }
                int export_channel_choice;
                cout << "\nSelect Payment Channel to Export (Array):" << endl;
                cout << "  1. Card" << endl;
                cout << "  2. Wire Transfer" << endl;
                cout << "  3. UPI" << endl;
                cout << "  4. ACH" << endl;
                cout << "  5. All Channels" << endl;
                cout << "Enter your choice: ";
                cin >> export_channel_choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');

                switch (export_channel_choice) {
                    case 1: exportToJson("card_transactions_arr.json", card_transactions_arr); break;
                    case 2: exportToJson("wire_transfer_transactions_arr.json", wire_transfer_transactions_arr); break;
                    case 3: exportToJson("upi_transactions_arr.json", upi_transactions_arr); break;
                    case 4: exportToJson("ach_transactions_arr.json", ach_transactions_arr); break;
                    case 5:
                        exportToJson("card_transactions_arr.json", card_transactions_arr);
                        exportToJson("wire_transfer_transactions_arr.json", wire_transfer_transactions_arr);
                        exportToJson("upi_transactions_arr.json", upi_transactions_arr);
                        exportToJson("ach_transactions_arr.json", ach_transactions_arr);
                        break;
                    default: cout << "Invalid channel choice." << endl; break;
                }
                break;
            }
            case 4: { // Sort Linked List (Merge Sort)
                if (location_transactions_list.isEmpty()) {
                    cout << "No linked list data loaded to sort." << endl;
                    break;
                }
                cout << "Sorting linked list by location using Merge Sort..." << endl;
                location_transactions_list.sortListByLocation();
                cout << "Linked list sorted by location." << endl;
                break;
            }
            case 5: { // Search Linked List (Binary Search)
                if (location_transactions_list.isEmpty()) {
                    cout << "No linked list data loaded to search." << endl;
                    break;
                }
                string searchLocation;
                cout << "Enter location to search for: ";
                getline(cin, searchLocation);

                cout << "\n--- Search Results for '" << searchLocation << "' (Linked List) ---" << endl;
                CustomArray results_list = location_transactions_list.binarySearchByLocation(searchLocation);
                if (!results_list.isEmpty()) {
                    displayTransactions(results_list, "Search Results (Linked List)");
                } else {
                    cout << "No transactions found for location '" << searchLocation << "'." << endl;
                }
                break;
            }
            case 6: { // Export Linked List
                if (location_transactions_list.isEmpty()) {
                    cout << "No linked list data loaded to export." << endl;
                    break;
                }
                exportToJson("location_transactions_list.json", location_transactions_list);
                break;
            }
            case 7: { // Search by Transaction Type
                string transactionType;
                cout << "Enter transaction type (withdrawal/deposit/transfer/payment): ";
                getline(cin, transactionType);

                cout << "\n--- Search Results for '" << transactionType << "' ---" << endl;
                CustomArray card_results = searchByTransactionType(card_transactions_arr, transactionType);
                CustomArray wire_results = searchByTransactionType(wire_transfer_transactions_arr, transactionType);
                CustomArray upi_results = searchByTransactionType(upi_transactions_arr, transactionType);
                CustomArray ach_results = searchByTransactionType(ach_transactions_arr, transactionType);

                displayTransactions(card_results, "Card " + transactionType);
                displayTransactions(wire_results, "Wire " + transactionType);
                displayTransactions(upi_results, "UPI " + transactionType);
                displayTransactions(ach_results, "ACH " + transactionType);
                break;
            }
            case 8: { // Sort by Transaction Type
                cout << "Sorting all transactions by type using Merge Sort..." << endl;
                CustomArray allTransactions = combineAllTransactions(
                    card_transactions_arr,
                    wire_transfer_transactions_arr,
                    upi_transactions_arr,
                    ach_transactions_arr
                );
                allTransactions = mergeSortByTransactionType(allTransactions);
                cout << "All transactions sorted by type." << endl;
                
                // Optionally display or export the results
                exportToJson("sorted_by_type.json", allTransactions);
                break;
            }
            case 9: { // Export by Transaction Type
                string transactionType;
                cout << "Enter transaction type to export (withdrawal/deposit/transfer/payment): ";
                getline(cin, transactionType);

                CustomArray results = combineAllTransactions(
                    searchByTransactionType(card_transactions_arr, transactionType),
                    searchByTransactionType(wire_transfer_transactions_arr, transactionType),
                    searchByTransactionType(upi_transactions_arr, transactionType),
                    searchByTransactionType(ach_transactions_arr, transactionType)
                );

                if (!results.isEmpty()) {
                    exportToJson(transactionType + "_transactions.json", results);
                } else {
                    cout << "No transactions found for type '" << transactionType << "'." << endl;
                }
                break;
            }
            case 10: { // Display All
                if (card_transactions_arr.isEmpty() && location_transactions_list.isEmpty()) {
                    cout << "No data loaded to display." << endl;
                } else {
                    displayTransactions(card_transactions_arr, "Card (Array)");
                    displayTransactions(wire_transfer_transactions_arr, "Wire Transfer (Array)");
                    displayTransactions(upi_transactions_arr, "UPI (Array)");
                    displayTransactions(ach_transactions_arr, "ACH (Array)");
                    displayTransactions(location_transactions_list, "Location (Linked List)");
                }
                break;
            }
            case 11:
                cout << "Exiting program. Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
        }
    } while (choice != 11);

    return 0;
}
