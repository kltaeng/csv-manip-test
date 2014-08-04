#include "csv_funcs.h";

int main(int argc, char* argv[]) {
	// Show help if we didn't get the proper args
	if (argc > 2) {
		showHelp();
		return 0;
	}

	// Grab filename
	string fileName = argv[1];

	// Set up our storage containers
	FileData csvFile;

	// Used to determine which columns are visible
	int*  visibleColumns;

	Stats* colStats;
	int numColumns = 0;

	// Flag to determine if we need to recalc the median if a row has been sorted
	bool sorted = false;
	
	cout << "Loading file: " << fileName << endl;
	// Initial load
	int totalRows = getFileData(fileName, csvFile, colStats, numColumns);

	// Something went wrong, lets just say we couldn't open the file
	if (totalRows == 0) {
		cout << "Could not open file. " << fileName << endl;
		return 0;
	}

	cout << "Loaded file with: " << totalRows << " rows, and " << numColumns << " columns. \n>: ";

	// By default, all columns are visible
	visibleColumns = new int[numColumns];
	for (int i = 0; i < numColumns; ++i) {
		visibleColumns[i] = 1;
	}

	string command;
	while (command != "exit") {
		string col, col2;
		string value;
		int op = -1;
		// Grab the command as it should be the first thing they type
		cin >> command;
		if (command == "sort") {
			cin >> col;
			int column = atoi(col.c_str());
			
			if (column < 0 || column >= numColumns) {
				cout << "Please enter a valid column number between 0 and " << numColumns << endl;	
			} else {
				// If we got a valid input, sort up the column
				heapSort(csvFile, totalRows, column);		
				sorted = true;
			}
		} else if (command == "print") {
			printCSVData(csvFile, numColumns, visibleColumns);
		} else if (command == "stats") {
			if (sorted) {
				findMedian(csvFile, colStats, numColumns, totalRows);
			}
			printCSVStats(colStats, numColumns);
		} else if (command == "add")  {
			op = OPS::PLUS;
		} else if (command == "div")  {
			op = OPS::DIVIDE;
		} else if (command == "sub")  {
			op = OPS::SUB;
		} else if (command == "mult")  {
			op = OPS::MULT;
		} else if (command == "select" ) {
			// each time we get a select, we clear all the rows 
			// for visibilty, and set just the ones the user wanted
			for (int i = 0; i < numColumns; ++i) {
				visibleColumns[i] = 0;
			}
			getline(cin, col);
			istringstream iss(col);
			while(getline(iss, col, ' ')) {
				if (col.length() == 0) {
					continue;
				}

				int colNum = atoi(col.c_str());
				if (colNum < 0 || colNum >= numColumns) {
					cout << "Please enter a valid column number between 0 and " << numColumns << endl;	
				} else {
					visibleColumns[colNum] = 1;
				}
			}
		} else if (command == "clear") {
			for (int i = 0; i < numColumns; ++i) {
				visibleColumns[i] = 1;
			}
		} else if (command == "help") {
			showHelp();
		}

		// if we recieved an op command, here is where we'll call the function w/ the right params
		if (op >= 0) {
			cin >> col;
			cin >> col2;

			int columnLeft = atoi(col.c_str());
			int columnRight = atoi(col2.c_str());

			if (columnLeft < 0 || columnLeft >= numColumns || columnRight < 0 || columnRight >= numColumns) {
				cout << "Please enter a valid column number between 0 and " << numColumns << endl;
			} else {
				switch(op) {
					case PLUS:
						cout << "Column " << col << " + " << col2 << endl;
						performOp(csvFile, OPS::PLUS, columnLeft, columnRight);
						break;
					case DIVIDE:
						cout << "Column " << col << " / " << col2 << endl;
						performOp(csvFile, OPS::DIVIDE, columnLeft, columnRight);
						break;
					case MULT:
						cout << "Column " << col << " * " << col2 << endl;
						performOp(csvFile, OPS::MULT, columnLeft, columnRight);
						break;
					case SUB:
						cout << "Column " << col << " - " << col2 << endl;
						performOp(csvFile, OPS::SUB, columnLeft, columnRight);
						break;
				};
			}
		}

		cout << ">: ";
	}

	return 0;
}
