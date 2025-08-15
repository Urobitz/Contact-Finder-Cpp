
#ifndef SQL_FUNCTIONS_H
#define SQL_FUNCTIONS_H

#include <iostream>
#include <string>
#include <sqlite3.h>


struct Contact
{
    std::string name;
    std::string phone;
    std::string email;
};

int createTable(sqlite3 *db);

int addContact(Contact newContact, sqlite3 *db);
int seekContact(sqlite3 *db, Contact contactToSeek);

#endif // SQL_FUNCTIONS_H