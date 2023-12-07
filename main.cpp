#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "UFOHashTable.h"
#include "UFONaryTree.h"
using namespace std;
using namespace std::chrono;


int main() {
    fstream fin;

    fin.open("ufo_sightings.csv", ios::in);

    if (!fin.is_open()) {
        cout << "File could not be read";
        return 1;
    }

    vector<string> row;
    vector<vector<string>> col;
    string line, temp;
    while (fin) {
        row.clear();

        getline(fin, line);

        stringstream lineStream(line);
        string cell;

        while (getline(lineStream, cell, ','))
        {
            row.push_back(cell);
        }
        col.push_back(row);
    }

    HashTable statesTable(10);
    for(unsigned int i = 0; i < col.size();i++){
        if(col.at(i).size() > 1){
            statesTable.insert(col.at(i).at(1));
        }
    }

    // Create n-ary tree
    TreeNode* root = new TreeNode("Root");


    // Load UFO sightings data into n-ary tree
    for (unsigned int i = 0; i < col.size(); i++) {
        if (col.at(i).size() > 1) {
            root->insertSighting(col.at(i).at(1));
        }
    }

    // Menu
    int choice;

    do {
        cout << "Welcome to the UFO Alien helper finder!" << endl;
        cout << "Menu:\n";
        cout << "1. Print top N states\n";
        cout << "2. Remove state\n";
        cout << "3. Get state count\n";
        cout << "4. Quit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Clear input buffer, had to look this up: cite https://www.geeksforgeeks.org/clearing-the-input-buffer-in-cc/
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                int n;
                cout << "Enter the number of top states to print: ";
                cin >> n;

                cout << "N-ary Tree Results:" << endl;
                auto start3 = high_resolution_clock::now();
                root->printTopStates(n);
                auto stop3 = high_resolution_clock::now();
                auto duration3 = duration_cast<nanoseconds>(stop3 - start3);
                cout << "Time taken to print top N states: " << duration3.count() << " nanoseconds" << endl;
                cout << endl;

                cout << "Hash Table Results:" << endl;
                auto start4 = high_resolution_clock::now();
                statesTable.printTopStates(n);
                auto stop4 = high_resolution_clock::now();
                auto duration4 = duration_cast<nanoseconds>(stop4 - start4);
                cout << "Time taken to print top N states: " << duration4.count() << " nanoseconds" << endl;
                cout << endl;

                break;

            }
            case 2: {
                string stateToRemove;
                cout << "Enter the state to remove (place the state's abbreviation in quotes): ";
                cin >> stateToRemove;

                auto start5 = high_resolution_clock::now();
                root->removeChild(stateToRemove);
                auto stop5 = high_resolution_clock::now();
                auto duration5 = duration_cast<nanoseconds>(stop5 - start5);
                cout << "Time taken to remove state for N-ary Tree: " << duration5.count() << " nanoseconds" << endl;

                auto start6 = high_resolution_clock::now();
                statesTable.remove(stateToRemove);
                auto stop6 = high_resolution_clock::now();
                auto duration6 = duration_cast<nanoseconds>(stop6 - start6);
                cout << "Time taken to remove state for Hash Table: " << duration6.count() << " nanoseconds" << endl;

                cout << "State removed.\n";
                break;
            }
            case 3: {
                string state;
                cout << "Enter the state you would like to see aliens at(abbreviated and capitalized): ";
                cin >> state;

                auto start7 = high_resolution_clock::now();
                cout << statesTable.get(state) << " sightings\n";
                auto stop7 = high_resolution_clock::now();
                auto duration7 = duration_cast<nanoseconds>(stop7 - start7);
                cout << "Time taken to remove state for Hash Table: " << duration7.count() << " nanoseconds" << endl;
                
            }
            case 4:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    // Cleanup: Release memory taken up by tree nodes
    delete root;

    return 0;
}
