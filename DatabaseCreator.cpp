//****************************************************************************
// @file DatabaseCreator.cpp : This file includes the main function
//
//****************************************************************************
#include <iostream>
#include <stdio.h>
#include <sqlite3.h>
#include "Database.hpp"
#include "Menu.hpp"
#include "TableInfo.hpp"
#include <string>
#include <vector>
#include "Table.h"

int main()
{
    const char* directory = "D:\\Test.db";
    int selection = 0;
    int tableSelection = 0;

    while( selection != 9 )
    {
        std::cout << "1) Create new database\n"
                     "2) Access existing database\n";
        std::cin >> selection;
        std::cout << "\n";
        switch( selection )
        {
            case 1:
                directory = Database::CreateDatabase();
                break;
            case 2:
                while( selection != 9 )
                {
                    selection = AccessDatabase();                 
                    switch( selection )
                    {
                        // Create new table
                        case 1:
                            Database::CreateTable( directory );
                            break;
                        // Insert data
                        case 2:
                            Database::InsertData( directory );
                            break;
                        // Select data
                        case 3:
                            Database::SelectData( directory );
                            break;
                        // Select all data
                        case 4:
                            Database::SelectAll( directory );
                            break;
                        // Update data
                        case 5:
                            Database::UpdateData( directory );
                            break;
                        // Delete data
                        case 6:
                            Database::DeleteItem( directory );                            
                            break;
                        // Delete table
                        case 7:
                            Database::DeleteTable( directory );                        
                            break;
                        // Show all tables
                        case 8:
                            Database::ShowAllTables();
                            break;
                        // Exit
                        case 9:
                            break;
                    }
                }                
        }
    }
}
