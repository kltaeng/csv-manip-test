#include "csv_funcs.h";

int main(int argc, char* argv[]) {

	if (argc > 2) {
		showHelp();
		return 0;
	}

	string fileName = argv[1];

	FileData csvFile;
	int*  visibleColumns;
	Stats* colStats;
	int numColumns = 0;

	bool sorted = false;
	
	cout << "Loading file: " << fileName << endl;
	int totalRows = getFileData(fileName, csvFile, colStats, numColumns);

	if (totalRows == 0) {
		cout << "Could not open file. " << fileName << endl;
		return 0;
	}

	cout << "Loaded file with: " << totalRows << " rows, and " << numColumns << " columns. \n>: ";

	visibleColumns = new int[numColumns];
	for (int i = 0; i < numColumns; ++i) {
		visibleColumns[i] = 1;
	}

	string command;
	while (command != "exit") {
		string col, col2;
		string value;
		int op = -1;
		cin >> command;
		if (command == "sort") {
			cin >> col;
			int column = atoi(col.c_str());
			
			if (column < 0 || column >= numColumns) {
				cout << "Please enter a valid column number between 0 and " << numColumns << endl;	
			} else {
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
