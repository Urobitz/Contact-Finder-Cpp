
#ifndef SQL_FUNCTIONS_H
#define SQL_FUNCTIONS_H

#include "sql_functions.cpp"

struct Contact
{
    std::string name;
    std::string phone;
    std::string email;
};

int createTable(sqlite3 *db);

int adContact(Contact newContact, sqlite3 *db);

#endif // SQL_FUNCTIONS_H