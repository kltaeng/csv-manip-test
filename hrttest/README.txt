CSV Manip Tool
Akshay Nagula

Overview:
	This tool is intended to view a csv containing numbers, and no header, and perform basic operations on it, such as add, subtract, multiply, and divide. 
	It will also provide simple statistics such as average, median, max, and min. You can also sort the data by column, and even choose only to display certain columns.
	The program requires that you have no empty cells, and the values must be comma seperated

How to use:
	Initially,you will want to run the tool with the path to your file as the single command line argument.
		ex: csv_manip_tool.exe my_data.csv
	This will load up the data, and you may type in commands after the program alerts you that it has loaded in all of its data.

Commands:
	sort [column number] -- will heap sort the data in the specific column (will keep rows intact)
	add [column number] [column number] - you must enter 2 valid numbers, otherwise, it defaults to column 0. 
		ex: add 0 1 -- will add contents from column 0, and 1
			add 0 c -- will add column 0 with itself
	sub, mult, and div also function this way. sub and div are order dependent. 
	select [column number] [column number] ... -- this will select a specific subset of columns to display
	clear -- this will clear your subset selection
	print -- prints all data in csv
	stats -- prints average, median, max, and min for all columns
	exit -- exits the application


