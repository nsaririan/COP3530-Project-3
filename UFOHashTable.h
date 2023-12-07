#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <list>

using namespace std;

class HashTable {
private:

    int size;
    int num = 0;
    vector<list<pair<string, int>>> table;
    float maxLoadFac = 1.5;

    int hashFunction(const string key) const {
        int num = 0;
        for (unsigned int i = 0; i < key.length(); i++) {
            num += static_cast<int>(key.at(i)) * 7 ^ (i);
        }
        int index = num % size;
        return index;
    }

public:
    // Constructor
    HashTable(int initialSize = 10) : size(initialSize), table(initialSize) {}

    // Insert a key-value pair into the hash table
    void insert(const string& key) {
        int index = hashFunction(key);
        for (auto& entry : table[index]) {
            if (entry.first == key) {
                // Update value if key already exists
                entry.second++;
                return;
            }
        }
        // If key doesn't exist, add a new key-value pair
        table[index].push_back({key, 1});
        num++;

        if ((float)(num / size) == maxLoadFac) {
            rehash();
        }
    }

    // Get the value associated with a key
    int get(const string& key) const {
        string stateWithoutQuotes = key.substr(1, key.length() - 2);
        int index = hashFunction(stateWithoutQuotes);
        for (const auto& entry : table[index]) {
            if (entry.first == stateWithoutQuotes) {
                return entry.second;
            }
        }
        throw out_of_range("Key not found");
    }

    void rehash() {
        int newSize = size * 2;  // Double the size
        vector<list<pair<string, int>>> newTable(newSize);

        // Insert existing values into the new table
        for (const auto& entries : table) {
            for (const auto& entry : entries) {
                int newIndex = hashFunction(entry.first);
                newTable[newIndex].push_back(entry);
            }
        }

        // Update the hash table with the new size and content
        size = newSize;
        table = move(newTable);
        newTable.clear();
    }


    // Remove a key-value pair from the hash table
    void remove(const string& key) {
        string stateWithoutQuotes = key.substr(1, key.length() - 2);
        int index = hashFunction(stateWithoutQuotes);
        auto& entries = table[index];
        for (auto it = entries.begin(); it != entries.end(); ++it) {
            if (it->first == stateWithoutQuotes) {
                entries.erase(it);
                return;
            }
        }
        throw out_of_range("Key not found");
    }

    bool compareSecond(const pair<string, int>& a, const pair<string, int>& b) {
        return a.second > b.second;
    }


    void printTopStates(int n) {
        vector<pair<string, int>> states;
        
        for (const auto& entries : table) {
            for (const auto& entry : entries) {
                states.push_back(entry);
            }
        }

        sort(states.begin(), states.end(), [this](const auto& a, const auto& b) {
            return compareSecond(a, b);
            });

        for (int i = 0; i < min(n, states.size()); i++) {
            cout << states.at(i).first << ": " << states.at(i).second << " sightings\n";
        }
    }
};
