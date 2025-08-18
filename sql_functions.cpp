#include "sql_functions.h"
#include <iostream>
#include <sqlite3.h>

int createTable(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS contacts ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT NOT NULL, "
                      "phone TEXT NOT NULL, "
                      "email TEXT NOT NULL);";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return rc;
    }

    return SQLITE_OK;
}

int addContact(Contact newContact, sqlite3 *db)
{
    const char *add_data = "INSERT INTO contacts (name, phone, email) VALUES (?, ?, ?);";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, add_data, -1, &stmt, nullptr); // Prepare the SQL statement

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }
    sqlite3_bind_text(stmt, 1, newContact.name.c_str(), -1, SQLITE_STATIC); // bind name
    sqlite3_bind_text(stmt, 2, newContact.phone.c_str(), -1, SQLITE_STATIC); // bind phone
    sqlite3_bind_text(stmt, 3, newContact.email.c_str(), -1, SQLITE_STATIC);  // bind email

    rc = sqlite3_step(stmt); //Execute the statement
    if (rc != SQLITE_DONE) 
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << "\n"; 
    }

    sqlite3_finalize(stmt);
    return rc;
}

int seekContact(sqlite3 *db, Contact contactToSeek)
{
    const char * seek_data = "SELECT * FROM contacts WHERE name = ?;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, seek_data, -1, &stmt, nullptr); // Prepare the SQL statement

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    sqlite3_bind_text(stmt, 1, contactToSeek.name.c_str(), -1, SQLITE_STATIC); // bind name
    
    bool found = false;
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        found = true;
        std::cout << "\nContact Found:\n";
        std::cout << "id: " << sqlite3_column_int(stmt, 0) << "\n";
        std::cout << "Name: " << sqlite3_column_text(stmt, 1) << "\n";
        std::cout << "Phone: " << sqlite3_column_text(stmt, 2) << "\n";
        std::cout << "Email: " << sqlite3_column_text(stmt, 3) << "\n";
    }

    if (!found)
    {
        std::cout << "No contact found with the name: " << contactToSeek.name << "\n";
    }

    sqlite3_finalize(stmt);

    return rc;
}

int callback(void *  unused, int col_n, char ** data, char ** col_name)
{

    for (int i = 0; i < col_n; i++)
    {
        std::cout << col_name[i] << ": "<< data[i] << "\n";
    }

    std::cout<<"\n";

    return 0;
    
}

int printTable(sqlite3 *db)
{
    const char * printContent = "Select * FROM contacts";
    sqlite3_stmt * stmt;

    std::cout << "-------RESULTS-------\n";

    int rc = sqlite3_exec(db, printContent, callback, nullptr, nullptr);

    std::cout << "---------------------";

    return rc;
}


