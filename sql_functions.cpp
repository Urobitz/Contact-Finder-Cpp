<<<<<<< HEAD
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
=======
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
>>>>>>> 4a1a34ac5d9dacc09ec90e47cd14c359a29aaab0
}