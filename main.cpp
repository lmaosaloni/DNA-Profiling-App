//
//  Project 1 Starter Code - DNA Profiling
//
// Author: Saloni Mhatre
// NetID: smhatr4 (@uic.edu)
// Date started: 8/23/2021
//
// Originally released as a submission to CS 251 (Prof. Hummel)// Project 1
// solution
//

///////////////////////////////////////////////////////////////////////////////
// CREATIVE COMPONENT: "relative_to" command
// Takes a name from the user as an input, outputs the person with the most
// similar DNA to that person from the database provided.
//
// Biological Background: Person A is said to be relative to
// Person B if their DNA are similar.
// Now since these are all humans, everyone is relative to
// Person A, but Person B's DNA is most
// similar to Person A than any other human being.
// Hence the closest relative.
//
// Mathematically speaking, the difference between their sum of STRs
// would be minimum for Person A and Person B.
//
// To run this command simply type:
// "relative_to" <individual's name>
///////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include "ourvector.h"

using namespace std;

// Funtion Prototypes:
void executeUserCom(string userCom,
					          ourvector<ourvector<char>>& headVector,
                    ourvector<ourvector<string>>& dataVector,
                    ourvector<char>& DNA1, ourvector<int>& PDNA);

void fillVectors(string filename,
				        ourvector<ourvector<char>>& headVector,
                ourvector<ourvector<string>>& dataVector);

void fillHead(ifstream& infile,
			        ourvector<ourvector<char>>& headVector);

void fillData(ifstream& infile,
			        ourvector<ourvector<string>>& dataVector);

void outputVector(ourvector<ourvector<string>>& V);

void executeDisplay(ourvector<ourvector<char>>& headVector,
                    ourvector<ourvector<string>>& dataVector,
                    ourvector<char>& DNA1, ourvector<int>& PDNA);

void outputDNA(ourvector<char>& V);

void fillDNA(string filename, ourvector<char>& DNA1);

void executeProcess(ourvector<ourvector<char>>& headVector,
					          ourvector<char>& DNA1,
                    ourvector<int>& PDNA);

int countSTR(ourvector<char>& DNA1, ourvector<char>& query);

ourvector<char> makeBlock(ourvector<char>& DNA, int start, int end);

bool blockEqual(ourvector<char>& block1, ourvector<char>& block2);

void outputSTR(ourvector<ourvector<char>>& headVector,
			         ourvector<int>& PDNA);
void executeSearch(ourvector<ourvector<string>>& dataVector,
				           ourvector<char>& DNA1,
				           ourvector<int>& PDNA);
bool compareSTR(ourvector<string>& row, ourvector<int>& PDNA);
bool readyForSearch(ourvector<ourvector<string>>& dataVector,
                    ourvector<char>& DNA1, ourvector<int>& PDNA);
bool readyForProcess(ourvector<ourvector<char>>& headVector,
                    ourvector<char>& DNA1);
bool findSTR(ourvector<char>& query, ourvector<char>& DNA1);
bool checkBlock3(ourvector<char>& block, int end,
                 ourvector<char>& DNA, int index);
void executeRelativeTo(string name, ourvector<ourvector<string>>& dataVector);
int searchName(string name, ourvector<ourvector<string>>& dataVector);
int sumSTR(ourvector<string> V);
////////////////////////////////////////////////////////////////////////////////

// executeUserCom
//
// accepts the user-Command from main and runs it
//
void executeUserCom(string userCom, ourvector<ourvector<char>>& headVector,
                    ourvector<ourvector<string>>& dataVector,
                    ourvector<char>& DNA1, ourvector<int>& PDNA) {
  if (userCom == "load_db") {
    string filename;
    cin >> filename;
    cout << "Loading database..." << endl;
    fillVectors(filename, headVector, dataVector);
  } else if (userCom == "display") {
    executeDisplay(headVector, dataVector, DNA1, PDNA);
  } else if (userCom == "load_dna") {
    string filename;
    cin >> filename;
    cout << "Loading DNA..." << endl;
    fillDNA(filename, DNA1);
  } else if (userCom == "process") {
  	if ( readyForProcess(headVector, DNA1) ) {
    	cout << "Processing DNA..." << endl;
    	executeProcess(headVector, DNA1, PDNA);
  	}
  } else if (userCom == "search") {
  	if ( readyForSearch(dataVector, DNA1, PDNA) ) {
  		cout << "Searching database..." << endl;
  		executeSearch(dataVector, DNA1, PDNA);
  	}
  } else if (userCom == "relative_to") {
    string name;
    cin >> name;
    executeRelativeTo(name, dataVector);
    } else {
    cout << "Invalid command: '" << userCom << "'" << endl;
  }
}

//
// fillVector
//
// accepts a filename as input, returns a vector containing many
// vectors
//
void fillVectors(string filename, ourvector<ourvector<char>>& headVector,
                 ourvector<ourvector<string>>& dataVector) {
  // headVector stores just the first line of the file
  // dataVector stores the data besides the first row

  if (headVector.size() > 0) {
    ourvector<ourvector<char>> newHead;
    for (int i = 0; i < headVector.size(); ++i) {
      headVector[i].clear();
    }
    headVector = newHead;
  }
  if (dataVector.size() > 0) {
    ourvector<ourvector<string>> newData;
    for (int j = 0; j < dataVector.size(); ++j) {
      dataVector[j].clear();
    }
    dataVector = newData;
  }
  ifstream infile;
  infile.open(filename);
  if (!infile.good()) {
    cout << "Error: unable to open '" << filename << "'" << endl;
    return;
  }
  fillHead(infile, headVector);
  fillData(infile, dataVector);
  /*for (auto row : headVector){
          row._output();
  } */
  /*for (auto data : dataVector){
          data._output();
  }*/
  infile.close();
}

//
// fillHead
//
// makes a headVector from reading the file.
// headVector is the first row of the file that has the name and STRs
//
void fillHead(ifstream& infile, ourvector<ourvector<char>>& headVector) {
  string head;
  infile >> head;

  ourvector<char> tempRow;
  for (auto& ch : head) {
    if (ch == ',') {
      headVector.push_back(tempRow);
      tempRow.clear();
    } else {
      tempRow.push_back(ch);
    }
  }

  headVector.push_back(tempRow);
}

//
// fillData
//
// makes a dataVector from reading the file.
// dataVector is the rows below the first row that has individuals'
// names and the count of each STR.
//
void fillData(ifstream& infile, ourvector<ourvector<string>>& dataVector) {
  ourvector<string> tempRow;
  string line, value;
  getline(infile, line);
  // from last infile, we are left with "\n", this to accept that newline
  getline(infile, line);
  while ( !infile.eof() ) {
    stringstream parser;
    parser.str(line);

    while ( !parser.eof() ) {
      getline(parser, value, ',');
      tempRow.push_back(value);
    }
    dataVector.push_back(tempRow);
    tempRow.clear();

    getline(infile, line);
    parser.str(line);
  }
}

//
// outputVector
//
// outputs the elements of a 2D vector (string)
//
//
void outputVector(ourvector<ourvector<string>>& V) {
  ourvector<string> tempV;

  for (int i = 0; i < V.size(); ++i) {
    tempV = V[i];
    // cout << "line 171: size = " << tempV.size() << endl;

    cout << tempV[0];
    for (int j = 1; j < tempV.size(); ++j) {
      cout << " " << tempV[j];
    }
    cout << endl;
  }
}

//
// outputDNA
//
// outputs a vector containing DNA strand
//
void outputDNA(ourvector<char>& V) {
  for (int i = 0; i < V.size(); ++i) {
    cout << V[i];
  }
}

//
// executeDisplay
//
// executes the "display" command
//
void executeDisplay(ourvector<ourvector<char>>& headVector,
                    ourvector<ourvector<string>>& dataVector,
                    ourvector<char>& DNA1, ourvector<int>& PDNA) {
  if (dataVector.size() == 0) {
    cout << "No database loaded." << endl;
  } else {
    cout << "Database loaded: " << endl;
    outputVector(dataVector);
  }
  cout << endl;
  if (DNA1.size() == 0) {
    cout << "No DNA loaded." << endl;
  } else {
    cout << "DNA loaded: " << endl;
    outputDNA(DNA1);
  }
  cout << endl;
  if (PDNA.size() == 0) {
    cout << "No DNA has been processed." << endl;
  } else {
    cout << "DNA processed, STR counts: " << endl;
    outputSTR(headVector, PDNA);
  }
}

//
// fillDNA
//
// takes a file as an input and
// fills the vector with DNA strand
//
void fillDNA(string filename, ourvector<char>& DNA1) {
  ifstream infile;
  infile.open(filename);
  if (!infile.good()) {
    cout << "Error: unable to open '" << filename << "'" << endl;
    return;
  }

  string temp;
  infile >> temp;

  // clearing DNA1 in case if filled previously
  DNA1.clear();
  for (size_t i = 0; i < temp.size(); ++i) {
    DNA1.push_back(temp[i]);
  }
}

//
// executeProcess
//
// executes the "process" command
// Counts the number of consecutive STRs in the DNA strand provided
//
void executeProcess(ourvector<ourvector<char>>& headVector,
					ourvector<char>& DNA1, ourvector<int>& PDNA) {
	// pushing back (-1) to match the index of headVector and PDNA
	// So headVector[i]'s count will be PDNA[i]
	// Also clearing the DNA in order to clear previous data (if any)
	PDNA.clear();
	PDNA.push_back(-1);
	for (int i = 1; i < headVector.size(); ++i) {
    	int count = countSTR(DNA1, headVector[i]);
		PDNA.push_back(count);
	}
	// cout << "Line 281: ";
	// PDNA._output();
}

//
// countSTR
//
// counts the max number of STRs appearing consecutively
// searches the query in DNA1
//
int countSTR(ourvector<char>& DNA1, ourvector<char>& query) {
  int count = 0;
  int maxCount = 0;
  // will make blocks of size of query and iterate through dna strand
  // (pseudo code) DNA[start1:end1] = DNA[start2:end2]
  ourvector<char> block1;
  ourvector<char> block2;
  int start1 = 0;
  int end1 = query.size();
  int start2 = end1;
  int end2 = end1 + end1;
  // the i < (..) to prevent the comparisions from going out of bounds
  for (int i = 0; i < (DNA1.size() - 2 * query.size() + 1); ++i) {
    block1 = makeBlock(DNA1, start1, end1);
    block2 = makeBlock(DNA1, start2, end2);

	// cout << "Line 326: " << i << "th iteration." << endl;
	// block1._output();
	// block2._output();
    if ( blockEqual(block1, query) && blockEqual(block1, block2) ) {
      if ( checkBlock3(block2, end2, DNA1, i) ) {
        if (maxCount < count) {
          count++;
          maxCount = count;
          count = 0;
        } else if (count == 0 && blockEqual(block1, block2)) {
          // this else-if for when the max consecutive STRs = 2
          count++;
          if (maxCount < count) {
            maxCount = count;
          }
          count = 0;
        }
      } else {
        count++;
      }
    }

    start1++;
    end1++;
    start2++;
    end2++;
  }

  // this counts for strands that are 2 or more
  // checking for single strands now
  if ( findSTR(query, DNA1) && maxCount == 0 ) {
  	return 1;
  } else if (maxCount == 0) {
  	return 0;
  } else {
  	return (maxCount+1);
  // +1 to account for the comparisions
	// Ex: ATG (1) == ATG (2) => count =  1 (but actually 2)
	// as it is ATGATG, but [0:3] is compared to [3:6]
  }
}

//
// checkBlock3
//
// checks if block3 (after block 2) is matching or not
// purpose is to find where the consecutive strands end
// returns true if block3 != block2, which marks the termination
//
bool checkBlock3(ourvector<char>& block, int end,
                 ourvector<char>& DNA, int index)  {
  if (index >= DNA.size() - block.size()) {
    return false;
  }

  ourvector<char> block3;
  int start3 = end;
  int end3 = start3 + block.size();
  block3 = makeBlock(DNA, start3, end3);

  if ( !blockEqual(block3, block) ) {
    return true;
  } else {
    return false;
  }
}

//
// makeBlock
//
// Given a DNA storing vector, returns another vector
// from given index of start to end
//
ourvector<char> makeBlock(ourvector<char>& DNA, int start, int end) {
  ourvector<char> block;

  for (int i = start; i < end; ++i) {
    block.push_back(DNA[i]);
  }

  return block;
}

//
// blockEqual
//
// checks if block1 = block2
//
bool blockEqual(ourvector<char>& block1, ourvector<char>& block2) {
  bool answer = false;
  if (block1.size() != block2.size()) {
    // cout << "Line 350: HERE" << endl;
    return answer;
  }

  int Tcount = 0;  // true value count
  for (int i = 0; i < block1.size(); ++i) {
    if (block1[i] == block2[i]) {
      Tcount++;
    }
  }

  if (Tcount == block1.size()) {
    // cout << "Line 359: " << Tcount  << ":" << block1.size() << endl;
    answer = true;
    return answer;
  } else {
    // cout << "Line 367: HERE" << endl;
    return answer;
  }
}

//
// match
//
// will compare query with DNA strand to check for consecutive DNA str
//
bool findSTR(ourvector<char>& query, ourvector<char>& DNA1) {
	ourvector<char> temp;
	for (int i = 0; i < DNA1.size() - query.size(); ++i) {
		temp = makeBlock(DNA1, i, i+query.size());
		if (blockEqual(temp, query)) {
			return true;
		}
	}
	return false;
}

//
// outputSTR
//
// outputs the processed data
//
void outputSTR(ourvector<ourvector<char>>& headVector, ourvector<int>& PDNA) {
  for (int i = 1; i < headVector.size(); ++i) {
    outputDNA(headVector[i]);
    cout << ": " << PDNA[i] << endl;
  }
}

//
// executeSearch
//
// executes the "search" command
//
void executeSearch(ourvector<ourvector<string>>& dataVector,
				   ourvector<char>& DNA1,
				   ourvector<int>& PDNA) {

	// I have stored the STR counts of each person in string
	// the easier thing is to convert the STRs obtained from
	// PDNA to strings. Since I was using a 2D vector,
	// it is impossible to store two different types in one vector
	for (int i = 0; i < dataVector.size(); ++i) {
    ourvector<string> row = dataVector[i];
		if (compareSTR(row, PDNA)) {
			cout << "Found in database! DNA matches: " << row[0];
			cout << endl;
			return;
		}
	}
	cout << "Not found in database." << endl;
}

//
// compareSTR
//
// compares the STR of a person to the STR we processed
//
bool compareSTR(ourvector<string>& row, ourvector<int>& PDNA) {
	// since my PDNA[0] = -1, it will help me set up
	// a for loop with same index.
	for (int i = 1; i < row.size(); ++i) {
		string str = to_string(PDNA[i]);
		if (row[i] != str) {
			return false;
		}
	}
	return true;
}

//
// readyForSearch
//
// checks if the program is ready for searching
// for a person in database or not
//
bool readyForSearch(ourvector<ourvector<string>>& dataVector,
                    ourvector<char>& DNA1, ourvector<int>& PDNA) {
	if (dataVector.size() == 0) {
		cout << "No database loaded." << endl;
		return false;
	} 
  if (DNA1.size() == 0) {
		cout << "No DNA loaded." << endl;
		return false;
	} 
  if (PDNA.size() == 0) {
		cout << "No DNA processed." << endl;
		return false;
	}
	return true;
}

//
// readyForProcess
//
// checks if the program is ready for processing
// a DNA strand or not
//
bool readyForProcess(ourvector<ourvector<char>>& headVector,
                    ourvector<char>& DNA1) {

	if (headVector.size() == 0) {
		cout << "No database loaded." << endl;
		return false;
	} if (DNA1.size() == 0) {
		cout << "No DNA loaded." << endl;
		return false;
	}
	return true;
}

//
// executeRelativeTO
//
// Creative Component Function: see header
//
void executeRelativeTo(string name, ourvector<ourvector<string>>& dataVector) {
  // finidng the data of "name" from database
  int indexName = searchName(name, dataVector);
  if (indexName == -1) {
    cout << "'" << name << "' not found!" << endl;
    return;
  }

  ourvector<string> nameRow = dataVector[indexName];
  int sumName = sumSTR(nameRow);

  // stores the difference between the sum of STRs between "name" and other people
  int diff = sumName;
  int newDiff = 0;
  string rName;  // stores the name of relative if found
  int pSum = 0;  // stores the sum of STRs of people in the database
  for (int i = 0; i < dataVector.size(); ++i) {
    if (i == indexName) {
      i++;
    }

    ourvector<string> tempRow = dataVector[i];
    pSum = sumSTR(tempRow);
    newDiff = abs(sumName - pSum);
    if (diff > newDiff) {
      rName = tempRow[0];
      diff = newDiff;
    }
  }

  cout << "The closest relative to '" << name;
  cout << "' is '" << rName << "'." << endl;
}

//
// searchName
//
// searches for the Name in database and outputs the whole data
// a linear search algorithm
//
int searchName(string name, ourvector<ourvector<string>>& dataVector) {
  for (int i = 0; i < dataVector.size(); ++i) {
    ourvector<string> tempRow;
    tempRow = dataVector[i];
    if (tempRow[0] == name) {
      return i;
    }
  }

  return -1;
}

//
// sumSTR
//
// finds sum of STRs of a person given one person's data
//
int sumSTR(ourvector<string> V) {
  int sum = 0;
  for (int i = 1; i < V.size(); ++i) {
    sum += stoi(V[i]);
  }
  return sum;
}
//
// main function
//
int main() {
  string filename, userCom;

  cout << "Welcome to the DNA Profiling Application." << endl;
  cout << "Enter command or # to exit: ";
  cin >> userCom;

  // exit condition
  if (userCom == "#") {
    return 0;
  }

  ourvector<ourvector<char>> V1;    // vector to store first row
  ourvector<ourvector<string>> V2;  // vector to store data
  ourvector<char> dna1;             // vector to store DNA
  ourvector<int> pdna;              // vector to store processed DNA

  while (userCom != "#") {
    executeUserCom(userCom, V1, V2, dna1, pdna);
    cout << "Enter command or # to exit: ";
    cin >> userCom;
  }

  return 0;
}
