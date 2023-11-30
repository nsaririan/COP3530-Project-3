#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;


int main() {
	// File pointer 
	fstream fin;

	// Open file 
	fin.open("ufo_sightings.csv", ios::in);
	
	if (!fin.is_open()) {
		cout << "File could not be read";
	}


	vector<string>   row;
	vector<vector<string>> col;
	string                line, temp;
	while (fin) {
		row.clear();

		getline(fin, line);

		stringstream          lineStream(line);
		string                cell;

		while (getline(lineStream, cell, ','))
		{
			row.push_back(cell);
		}
		col.push_back(row);
	}

	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < col.at(i).size(); j++) {
			cout << col.at(i).at(j) << " ";
		}
		cout << endl << endl;
	}

	return 0;
}