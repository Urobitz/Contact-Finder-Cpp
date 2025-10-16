
#ifndef SQL_FUNCTIONS_H
#define SQL_FUNCTIONS_H

#include <iostream>
#include <string>
#include <sqlite3.h>


struct Contact
{
    unsigned int id;
    std::string name;
    std::string phone;
    std::string email;
};

int createTable(sqlite3 *db);

int addContact(Contact newContact, sqlite3 *db);
int seekContact(sqlite3 *db, Contact contactToSeek);
int callback(void *  unused, int col_n, char ** data, char ** col_name);
int printTable(sqlite3 *db);
int deleteContact(sqlite3 *db, int id);

#endif // SQL_FUNCTIONS_H