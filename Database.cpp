//****************************************************************************
// @file Database.cpp
//
//****************************************************************************

#include "Database.hpp"
#include <string>
#include <sstream>
#include <vector>
#include "TableInfo.hpp"
#include "Table.h"
#include "Menu.hpp"
#include <unordered_map>

//------------------------------------------------------------------------
// Variables
//------------------------------------------------------------------------
std::string add;
std::string category;
std::string column;
std::string data;
std::string dir;
std::string sql;
std::string tableName;
std::string value;
sqlite3* DB;
char* messageError;
int col;

//------------------------------------------------------------------------
// Store table information
//------------------------------------------------------------------------
std::unordered_map<std::string, std::vector<std::string>> tableMap;
std::vector<std::string> tableKeys;

//----------------------------------------------------------------------------
//! Used to traverse the SELECT statements
//! argc: holds the number of results, argv: holds each value in an array,
//! azColName: holds each column returned in an array
//----------------------------------------------------------------------------
int Database::Callback( void* NotUsed, int argc, char** argv, char** azColName )
{
	for( int i = 0; i < argc; i++ )
	{
		std::cout << azColName[ i ] << " : " << argv[ i ] << std::endl;
	}
	std::cout << std::endl;
	return 0;
}

//------------------------------------------------------------------------
// Create the database
// @return directory
//------------------------------------------------------------------------
const char* Database::CreateDatabase()
{
	std::cout << "Enter the location and name of database (C:\\\\Documents\\\\Database.db)" << std::endl;
	std::cin.ignore();
	std::getline( std::cin, dir );
	int exit = sqlite3_open( dir.c_str(), &DB );
	return dir.c_str();
}

//------------------------------------------------------------------------
// Create the table
//------------------------------------------------------------------------
void Database::CreateTable( const char* directory )
{
	std::vector<std::string> newEntry;	
	std::cout << "Enter the table name" << std::endl;
	std::cin >> tableName;
	
	//newEntry.push_back( tableName );
	std::cout << "How many columns do you want?" << std::endl;
	std::cin >> col;
	while( std::cin.fail() )
	{
		std::cout << "Please enter a integer" << std::endl;
		std::cin.clear();
		std::cin.ignore( 256, '\n' );
		std::cin >> col;
	}
	std::string table = "CREATE TABLE IF NOT EXISTS " + tableName + "(ID INTEGER PRIMARY KEY AUTOINCREMENT, ";
	for( int i = 0; i < col; i++ )
	{
		std::cout << "Enter column name" << std::endl;
		std::cin >> add;
		newEntry.push_back( add );
		if( i + 1 != col )
		{
			table += add + " TEXT NOT NULL, ";
		}
		else
		{
			table += add + " TEXT NOT NULL );";
		}
	}
	std::cout << std::endl;
	try
	{
		int exit = 0;
		exit = sqlite3_open( directory, &DB );

		exit = sqlite3_exec( DB, table.c_str(), NULL, 0, &messageError );

		if( exit != SQLITE_OK )
		{
			std::cerr << "Error creating table\n" << std::endl;
			sqlite3_free( messageError );
		}
		else
		{
			std::cout << "Table created successfully!\n" << std::endl;
			tableMap[ tableName ] = newEntry;
			tableKeys.push_back( tableName );
			
			sqlite3_close( DB );
		}
	}
	catch( const std::exception& error )
	{
		std::cerr << error.what();
	}
	sqlite3_close( DB );
} // end of function Database::CreateTable()

//------------------------------------------------------------------------
// Insert data into the table
//------------------------------------------------------------------------
void Database::InsertData( const char* directory )
{
	int exit = sqlite3_open( directory, &DB );
	int table = SelectTable( tableKeys );

	sql = "INSERT INTO " + tableKeys[ table ] + " ( ";
	for( auto x : tableMap[ tableKeys[ table ] ] )
	{
		if( x != tableMap[ tableKeys[ table ] ].back() )
		{
			sql += x + ", ";
		}
		else
		{
			sql += x + ") VALUES (";
		}
	}
	for( auto x : tableMap[ tableKeys[ table ] ] )
	{
		std::cout << "Enter value for " << x << std::endl;
		std::cin >> value;
		if( x != tableMap[ tableKeys[ table ] ].back() )
		{
			sql += "'" + value + "', ";
		}
		else
		{
			sql += "'" + value + "');";
		}
	}
	
	exit = sqlite3_exec( DB, sql.c_str(), NULL, 0, &messageError );
	if( exit != SQLITE_OK )
	{
		std::cerr << "Error Inserting Entry\n\n";
		sqlite3_free( messageError );
	}
	else
	{
		std::cout << "Entry inserted successfully!\n\n";
	}
	sqlite3_close( DB );
} // end of function Database::InsertData()

//------------------------------------------------------------------------
// Select data from the table
//------------------------------------------------------------------------
void Database::SelectData( const char* directory )
{
	int table = SelectTable( tableKeys );
	Database::DisplayColumnNames( tableKeys[ table ] );
	int exit = sqlite3_open( directory, &DB );
		
	std::cout << "Enter column name or '*' for all columns" << std::endl;
	std::cin >> column;
	std::cout << "Enter selection criteria" << std::endl;
	std::cin >> data;
	std::cout << "Enter value" << std::endl;
	std::cin >> value;
	std::cout << "\n";
	sql = "SELECT " + column + " FROM " + tableKeys[ table ] + " WHERE " + data + " = '" + value + "'";
	exit = sqlite3_exec( DB, sql.c_str(), Database::Callback, NULL, NULL );
	sqlite3_close( DB );
} // end of function Database::SelectData()

//------------------------------------------------------------------------
// Select all data from a table
//------------------------------------------------------------------------
void Database::SelectAll( const char* directory )
{
	int exit = sqlite3_open( directory, &DB );
	int table = SelectTable( tableKeys );
	sql = "SELECT * FROM " + tableKeys[ table ];
	exit = sqlite3_exec( DB, sql.c_str(), Database::Callback, NULL, NULL );
	sqlite3_close( DB );
} // end of function Database::SelectAll()

//------------------------------------------------------------------------
// Display all available tables
//------------------------------------------------------------------------
void Database::ShowAllTables()
{
	for( int i = 0; i < tableKeys.size(); i++ )
	{
		std::cout << i + 1 << ") " << tableKeys[ i ] << std::endl;
	}
} // end of function Database::ShowAllTables()

//------------------------------------------------------------------------
// Delete a table
//------------------------------------------------------------------------
void Database::DeleteTable( const char* directory )
{
	int exit = sqlite3_open( directory, &DB );
	int table = SelectTable( tableKeys );
	sql = "DROP TABLE " + tableKeys[ table];
	exit = sqlite3_exec( DB, sql.c_str(), Database::Callback, NULL, NULL );
	if( exit != SQLITE_OK )
	{
		std::cerr << "Error Deleting table\n\n";
		sqlite3_free( messageError );
	}
	else
	{
		std::cout << "Table Deleted successfully!\n\n";
		tableMap.erase( tableKeys[ table ] );
		tableKeys.clear();
		for( auto key : tableMap )
		{
			tableKeys.push_back( key.first );
		}
	}
	sqlite3_close( DB );
} // end of function Database::DeleteTable()

//------------------------------------------------------------------------
// Delete an item from a table
//------------------------------------------------------------------------
void Database::DeleteItem( const char* directory )
{
	int exit = sqlite3_open( directory, &DB );
	int table = SelectTable( tableKeys );
	Database::DisplayColumnNames( tableKeys[ table ] );

	std::cout << "Select a category" << std::endl;
	
	std::cin >> category;
	std::cout << "\n";
	std::cout << "Enter value" << std::endl;
	std::cin >> value;
	sql = "DELETE FROM " + tableKeys[ table ] + " WHERE " + category + " = '" + value + "';";
	exit = sqlite3_exec( DB, sql.c_str(), Database::Callback, NULL, NULL );
	if( exit != SQLITE_OK )
	{
		std::cerr << "Error Deleting entry\n\n";
		sqlite3_free( messageError );
	}
	else
	{
		std::cout << "Entry Deleted successfully!\n\n";
	}
	sqlite3_close( DB );
} // end of function Database::DeleteItem()

//------------------------------------------------------------------------
// Update data in a specific table
//------------------------------------------------------------------------
void Database::UpdateData( const char* directory )
{
	int exit = sqlite3_open( directory, &DB );
	int table = SelectTable( tableKeys );
	Database::DisplayColumnNames( tableKeys[ table ] );

	std::cout << "Select Category to update" << std::endl;
	std::cin >> category;
	std::cout << "Enter old value" << std::endl;
	std::cin >> data;
	std::cout << "Enter new value" << std::endl;
	std::cin >> value;
	std::cout << "\n";
	sql = "UPDATE " + tableKeys[ table ] + " SET " + category + " = '" + value + "' WHERE " + category + " = '" + data + "';";
	exit = sqlite3_exec( DB, sql.c_str(), Database::Callback, NULL, NULL );
	if( exit != SQLITE_OK )
	{
		std::cerr << "Error Updating entry\n\n";
		sqlite3_free( messageError );
	}
	else
	{
		std::cout << "Entry Updated successfully!\n\n";
	}
	sqlite3_close( DB );
} // end of function Database::UpdateData()

//------------------------------------------------------------------------
// Display the column names
//------------------------------------------------------------------------
void Database::DisplayColumnNames( std::string table )
{
	for( auto name : tableMap[ table ] )
	{
		if( name != tableMap[ table ].back() )
		{
			std::cout << name << " : ";
		}
		else
		{
			std::cout << name;
		}
	}
	std::cout << std::endl;
} // end of function Database::DisplayColumnNames()
