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
        int index = hashFunction(key);
        for (const auto& entry : table[index]) {
            if (entry.first == key) {
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
        int index = hashFunction(key);
        auto& entries = table[index];
        for (auto it = entries.begin(); it != entries.end(); ++it) {
            if (it->first == key) {
                entries.erase(it);
                return;
            }
        }
        throw out_of_range("Key not found");
    }
};



int main() {
	// File pointer 
	fstream fin;

	// Open file 
	fin.open("ufo_sightings.csv", ios::in);
	
	if (!fin.is_open()) {
		cout << "File could not be read";
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

    HashTable states(10);


	for (unsigned int i = 0; i < col.size(); i++) {
		for (unsigned int j = 0; j < col.at(i).size(); j++) {
			states.insert(col.at(i).at(1));
		}
	}
	

	return 0;
}
