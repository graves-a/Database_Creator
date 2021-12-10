//****************************************************************************
//
// HEADER NAME:
//      Database.hpp
//
//****************************************************************************

#ifndef _Database_hpp
#define _Database_hpp

#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include <ostream>

class Database
{
public:
	//------------------------------------------------------------------------
	// Functions
	//------------------------------------------------------------------------
	static int Callback( void* NotUsed, int argc, char** argv, char** azColName );

	static const char* CreateDatabase();

	static void CreateTable( const char* directory );

	static void CreateItem();

	static void SelectAll( const char* directory );

	static void UpdateData( const char* directory );

	static void InsertData( const char* directory );

	static void SelectData( const char* directory );

	static void DeleteTable( const char* directory );

	static void DeleteItem( const char* directory );

	static void DisplayColumnNames( std::string table );

	static void ShowAllTables();

	static std::string GetChoice();
};

#endif
