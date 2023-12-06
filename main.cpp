#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>
#include "UFOHashTable.h"
#include "UFONaryTree.h"
using namespace std;


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

    // Create n-ary tree
    TreeNode* root = new TreeNode("Root");
    HashTable states(10);

    // Load UFO sightings data into n-ary tree
    for (unsigned int i = 0; i < col.size(); i++) {
        if (col.at(i).size() > 1) {
            root->insertSighting(col.at(i).at(1));
        }
    }

    // Menu
    int choice;

    do {
        cout << "Menu:\n";
        cout << "1. Print top N states\n";
        cout << "2. Remove state\n";
        cout << "3. Quit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        // Clear input buffer, had to look this up: cite
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                int n;
                cout << "Enter the number of top states to print: ";
                cin >> n;
                root->printTopStates(n);
                states->printTopStates(n);
                break;
            }
            case 2: {
                string stateToRemove;
                cout << "Enter the state to remove (place the state's abbreviation in quotes): ";
                cin >> stateToRemove;
                root->removeChild(stateToRemove);
                states->remove(stateToRemove);
                cout << "State removed.\n";
                break;
            }
            case 3:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 3);

    // Cleanup: Release memory taken up by tree nodes
    delete root;

    return 0;
}

