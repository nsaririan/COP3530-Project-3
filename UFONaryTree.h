#pragma once
#include <iostream>
using namespace std;

#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
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


    // Get sightings count for given state
    int getSightings(const string& stateToFind) const {
        for (const TreeNode* child : children) {
            if (child->state == stateToFind) {
                return child->sightingsCount;
            }
        }
        throw out_of_range("State not found");
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

