#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;

// Stores each row as a dynamic array of doubles
// using a vector to ease up sorting, and accessing data
vector<double* > typedef FileData;

// List of operations that can be performed on the data
enum OPS {
	PLUS, DIVIDE, MULT, SUB
};

// Delimiter for CSV
const char DELIM = ',';

// Data storage for statistics for each column
struct Stats {
	Stats () {
		min = FLT_MAX;
		max = FLT_MIN;
		median = 0;
		average = 0;
	}
	double min, max, median, average;
};

// Loads up the file, and calculates the stats for each column on load
int getFileData(string fileName, FileData &csvFile, Stats* &colStats, int &numColumns);

// Helper function to find the median on load, and each time a column is sorted
void findMedian(vector<double*> &medianWorkers, Stats* &colStats, int numColumns, int numRows);

// Heap sort function 
void heapSort(FileData &csvFile, int numRows, int sortColumn);

// Dumps all CSV data to console window
void printCSVData(FileData &csvFile, int numColumns, int*  visibleColumns);

// Dumps all stats to console window
void printCSVStats(Stats* colStats, int numCols);

// Performs one of the four operations outlined in the README and above
void performOp(FileData &csvFile, OPS op, int colLeft, int colRight);

// Shows basic instructions on usage
void showHelp();
