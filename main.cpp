#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <stdexcept>

using namespace std;

class TreeNode {
public:
    string state;
    int sightingsCount;
    vector<TreeNode*> children;

    TreeNode(const string& state) : state(state), sightingsCount(0) {}

    // Insert UFO sighting into the tree
    void insertSighting(const string& state) {
        auto it = find_if(children.begin(), children.end(),
                          [&state](const TreeNode* node) {
                              return node->state == state;
                          });

        if (it != children.end()) {
            // State exists, increment count
            (*it)->sightingsCount++;
        }
        else {
            // State doesn't exist, create a new node
            TreeNode* newNode = new TreeNode(state);
            newNode->sightingsCount++;
            children.push_back(newNode);
        }
    }

    // Remove child node using state abbreviation
    void removeChild(const string& stateAbbreviation) {
        auto it = remove_if(children.begin(), children.end(),
                            [&stateAbbreviation](TreeNode* node) {
                                return node->state == stateAbbreviation;
                            });

        // Delete removed nodes
        for (auto iter = it; iter != children.end(); ++iter) {
            delete *iter;
        }

        // Erase removed nodes from vector
        children.erase(it, children.end());
    }


    // Perform DFS traversal of tree and print top states
    void printTopStates(int n) const {
        // Sort children using sightings count
        vector<TreeNode*> sortedChildren = children;
        sort(sortedChildren.begin(), sortedChildren.end(),
             [](const TreeNode* a, const TreeNode* b) {
                 return a->sightingsCount > b->sightingsCount;
             });

        for (int i = 0; i < min(n, static_cast<int>(sortedChildren.size())); ++i) {
            cout << sortedChildren[i]->state << ": " << sortedChildren[i]->sightingsCount << " sightings\n";
        }
    }

    // Destructor: free memory taken up by tree nodes
    ~TreeNode() {
        for (TreeNode* child : children) {
            delete child;
        }
    }
};

int main() {
    fstream fin;

    fin.open("/Users/niloufarsaririan/CLionProjects/ufo/ufo_sightings.csv", ios::in);

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
                break;
            }
            case 2: {
                string stateToRemove;
                cout << "Enter the state to remove: ";
                cin >> stateToRemove;
                root->removeChild(stateToRemove);
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

