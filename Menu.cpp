//****************************************************************************
// @file Menu.cpp
//
//****************************************************************************

#include "Menu.hpp"
#include <iostream>

//------------------------------------------------------------------------
// Main menu while inside of database
//------------------------------------------------------------------------
int AccessDatabase()
{
	int selection;
	std::cout <<
		"1) Create new table\n"
		"2) Insert data\n"
		"3) Select data\n"
		"4) Select all\n"
		"5) Update data\n"
		"6) Delete entry\n"
		"7) Delete table\n"
		"8) Show all tables\n"
		"9) Exit\n";
	std::cin >> selection;
	std::cout << "\n";
	return selection;
} // end of function AccessDatabase()

//------------------------------------------------------------------------
// Display list of available tables
//------------------------------------------------------------------------
int SelectTable( std::vector<std::string> tableKeys )
{
	int tableSelection;
	std::cout << "Please select a table" << std::endl;
	for( int i = 0; i < tableKeys.size(); i++ )
	{
		std::cout << i + 1 << ") " << tableKeys[i] << std::endl;
	}
	std::cin >> tableSelection;
	return tableSelection - 1;
} // end of function SelectTable()
