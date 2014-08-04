#include "csv_funcs.h";

// Shows help about commands and usage
void showHelp() {
	cout << "Usage: csv_manip.exe (filename)" << endl;
	cout << "{add, mult, div, sub} [column number] [column number]" << endl;
	cout << "print : Prints entire CSV list, unless specific columns specifed by 'select'" << endl;
	cout << "stats : Prints out statistics for each column (Average, Median, Max, Min)" << endl;
	cout << "sort  [column number] : Sorts column using heap sort" << endl;
	cout << "select [column number] [column number] ... : selects specific columns to display" << endl;
	cout << "clear : clears selection of specific columns to display" << endl;
	cout << "exit  : exits the application" << endl;
}

void printCSVData(FileData &csvFile, int numColumns, int* visibleColumns) {
	FileData::iterator start = csvFile.begin();
	FileData::iterator end = csvFile.end();

	while(start != end) {
		for (int i = 0; i < numColumns; ++i) {
			if (visibleColumns[i] == 1) {
				cout << setw(5) << (*start)[i] << "\t";
			}
		}
		cout << endl;
		start++;
	}
}

void printCSVStats(Stats* colStats, int numCols) {
	for (int i = 0; i < numCols; ++i) {
		cout << "Statistics for column " << i << ": " << endl;
		cout << setw(10) << "Median" << setw(10) << "Average" << setw(10) << "Max" << setw(10) << "Min" << endl;
		cout << setw(10) << colStats[i].median << setw(10) << colStats[i].average << setw(10) << colStats[i].max << setw(10) << colStats[i].min << endl;
	}
}

void performOp(FileData &csvFile, OPS op, int colLeft, int colRight) {
	FileData::iterator start = csvFile.begin();
	FileData::iterator end = csvFile.end();

	switch(op) {
	case PLUS:
		while( start != end ) {
			cout << (*start)[colLeft] + (*start)[colRight] << endl;
			start++;
		}
		break;
	case DIVIDE:
		while( start != end ) {
			cout << (*start)[colLeft] / (*start)[colRight] << endl;
			start++;
		}
		break;
	case MULT:
		while( start != end ) {
			cout << (*start)[colLeft] * (*start)[colRight] << endl;
			start++;
		}
		break;
	case SUB:
		while( start != end ) {
			cout << (*start)[colLeft] - (*start)[colRight] << endl;
			start++;
		}
		break;
	};
}


int getFileData(string fileName, FileData &csvFile, Stats* &colStats, int &numColumns) {
	if (fileName.length() == 0) {
		return 0;
	}

	ifstream filestream;

	filestream.open(fileName, ifstream::in);

	if (filestream.fail()) {
		return 0;
	}

	string row = "";

	getline(filestream, row);

	numColumns = 1;

	for(unsigned i = 0; i < row.length(); ++i) {
		if (row[i] == DELIM) {
			numColumns++;
		}
	}

	filestream.seekg(ios_base::beg);

	if(filestream.good()){
		row = "";

		colStats = new Stats[numColumns];
		int numRows = 0;
		while(getline(filestream, row)) {
			double* rowVals = new double[numColumns];

			stringstream iss;
			iss << row;

			string value = "";

			for(int i = 0; getline(iss, value, DELIM); ++i) {
				rowVals[i] = atof(value.c_str());
				if (rowVals[i] <= colStats[i].min) {
					colStats[i].min = rowVals[i];
				}

				if (rowVals[i] >= colStats[i].max) {
					colStats[i].max = rowVals[i];
				}

				colStats[i].average += rowVals[i];
			}

			csvFile.push_back(rowVals);
			numRows++;
		}

		findMedian(csvFile, colStats, numColumns, numRows);

		for (int i = 0; i < numColumns; ++i) {
			colStats[i].average /= numRows;
		}

		filestream.close();

		return numRows;
	} else {
		return 0;
	}
}


void shiftRight(FileData &csvFile, int low, int high, int sortCol)
{
    int root = low;
    while ((root*2)+1 <= high)
    {
        int leftChild = (root * 2) + 1;
        int rightChild = leftChild + 1;
        int swapIdx = root;
        /*Check if root is less than left child*/
        if (csvFile[swapIdx][sortCol] < csvFile[leftChild][sortCol])
        {
            swapIdx = leftChild;
        }
        /*If right child exists check if it is less than current root*/
        if ((rightChild <= high) && (csvFile[swapIdx][sortCol] < csvFile[rightChild][sortCol]))
        {
            swapIdx = rightChild;
        }
        /*Make the biggest element of root, left and right child the root*/
        if (swapIdx != root)
        {
            double* tmp = csvFile[root];
            csvFile[root] = csvFile[swapIdx];
            csvFile[swapIdx] = tmp;
            /*Keep shifting right and ensure that swapIdx satisfies
            heap property aka left and right child of it is smaller than
            itself*/
            root = swapIdx;
        }
        else
        {
            break;
        }
    }
    return;
}

void heapify(FileData &csvFile, int low, int high, int sortCol)
{
    /*Start with middle element. Middle element is chosen in
    such a way that the last element of array is either its
    left child or right child*/
    int midIdx = (high - low -1)/2;
    while (midIdx >= 0)
    {
        shiftRight(csvFile, midIdx, high, sortCol);
        --midIdx;
    }
    return;
}

void heapSort(FileData &csvFile, int numRows, int sortColumn)
{
    /*This will put max element in the index 0*/
    heapify(csvFile, 0, numRows-1, sortColumn);
    int high = numRows - 1;
    while (high > 0)
    {
        /*Swap max element with high index in the array*/
        double* tmp = csvFile[high];
        csvFile[high] = csvFile[0];
        csvFile[0] = tmp;
        --high;
        /*Ensure heap property on remaining elements*/
        shiftRight(csvFile, 0, high, sortColumn);
    }
    return;
}

double partitions(double* column, int low, int high)
{
    int p = low, r = high, i = p - 1;
	double x = column[(p+r)/2];

    for(int j = p; j <= r - 1; j++)
    {
        if (column[j] <= x)
        {
            i = i + 1;
            swap(column[i], column[j]);
        }
    }

    swap(column[i+1], column[r]);

    return i+1;
}

double selection_algorithm(double* column, int left,int right,int kth)
{
	while(true) {
		int pivotIndex = partitions(column, left, right);

		int len = pivotIndex - left + 1;

		if(kth == len) {
			return column[pivotIndex];
		}
		else if(kth < len) {
			right = pivotIndex - 1;
		}
		else {
			kth = kth - len;
			left = pivotIndex+1;
		}
	}
}

void findMedian(FileData & csvFile, Stats* &colStats, int numColumns, int numRows) {
	vector<double*> medianWorkers;

	for(int i = 0; i < numColumns; ++i) {
		double* columnToFindMedian = new double[numRows];
		medianWorkers.push_back(columnToFindMedian);
	}
		
	FileData::iterator start = csvFile.begin();
	FileData::iterator end = csvFile.end();

	int rowCounter = 0;
	while(start != end) {
		for( int i = 0; i < numColumns; ++i) {
			double* data = *(start);
			medianWorkers[i][rowCounter] = data[i];
		}

		rowCounter++;
		start++;
	}

	for (int i = 0; i < numColumns; ++i) {
		colStats[i].median   = selection_algorithm(medianWorkers[i], 1, numRows-1, numRows/2);
		delete medianWorkers[i];
	}
}