#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>

using namespace std;


vector<double* > typedef FileData;

enum OPS {
	PLUS, DIVIDE, MULT, SUB
};

const char DELIM = ',';

struct Stats {
	Stats () {
		min = FLT_MAX;
		max = FLT_MIN;
		median = 0;
		average = 0;
	}
	double min, max, median, average;
};

int getFileData(string fileName, FileData &csvFile, Stats* &colStats, int &numColumns);
void findMedian(vector<double*> &medianWorkers, Stats* &colStats, int numColumns, int numRows);
void heapSort(FileData &csvFile, int numRows, int sortColumn);
void printCSVData(FileData &csvFile, int numColumns, int*  visibleColumns);
void printCSVStats(Stats* colStats, int numCols);
void performOp(FileData &csvFile, OPS op, int colLeft, int colRight);
void showHelp();
