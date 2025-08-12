#include "sql_functions.h"
#include <iostream>
#include <sqlite3.h>

int createTable(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS contacts ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT NOT NULL, "
                      "phone TEXT NOT NULL);"
                      "email TEXT);";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return rc;
    }

    return SQLITE_OK;
}

int adContact(Contact newContact, sqlite3 *db)
{
    const char *add_data = "INSERT INTO contacts (id, name, phone, email) VALUES (?, ?, ?);";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, add_data, -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    sqlite3_bind_int(stmt, 1, 1); // initialize id to 1 for autoincrement
    sqlite3_bind_text(stmt, 2, newContact.name.c_str(), -1, SQLITE_STATIC); // bind name
    sqlite3_bind_text(stmt, 3, newContact.phone.c_str(), -1, SQLITE_STATIC); // bind phone
    sqlite3_bind_text(stmt, 4, newContact.email.c_str(), -1, SQLITE_STATIC);  // bind email

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << "\n";
    }

    sqlite3_finalize(stmt);
    return rc;


}
