#include "sql_functions.h"
#include <iostream>
#include <sqlite3.h>

/*
 * Create the contacts table if it does not already exist.
 * Params:
 *  - db : open sqlite3 database handle
 * Returns:
 *  - SQLITE_OK on success, or an sqlite error code on failure.
 */
int createTable(sqlite3 *db) {
    const char *sql = "CREATE TABLE IF NOT EXISTS contacts ("
                      "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                      "name TEXT NOT NULL, "
                      "phone TEXT NOT NULL, "
                      "email TEXT NOT NULL);";

    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql, nullptr, nullptr, &errMsg);

    if (rc != SQLITE_OK) {
        // Print error message provided by SQLite and free it.
        std::cerr << "SQL error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return rc;
    }

    return SQLITE_OK;
}

bool isEmpty(sqlite3 *db)
{

    //EXISTS returns 1 if the subquery returns any rows, 0 if it returns none.
    //Thus if the table is empty, EXIST will create a row with value 0.
    const char * checkEmpty = "SELECT EXISTS (SELECT 1 FROM contacts)"; //Check if there is any data in the table
    
        sqlite3_stmt * stmt;

        int rc = sqlite3_prepare_v2(db, checkEmpty, -1, &stmt, nullptr);
        if (rc != SQLITE_OK)
        {
            std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(nullptr) << std::endl;
            return true; // Assume empty on error
        }

        rc = sqlite3_step(stmt);
        int exist_return_value = 0;
        if (rc == SQLITE_ROW)
        {
            exist_return_value = sqlite3_column_int(stmt, 0);
        }

        return exist_return_value == 0;
    
}

/*
 * Insert a new contact into the contacts table using a prepared statement.
 * Params:
 *  - newContact : Contact object containing name, phone, email
 *  - db         : open sqlite3 database handle
 * Returns:
 *  - Result code from sqlite3_step (SQLITE_DONE on success) or error code.
 */
int addContact(Contact newContact, sqlite3 *db)
{
    const char *add_data = "INSERT INTO contacts (name, phone, email) VALUES (?, ?, ?);";

    sqlite3_stmt *stmt;
    // Prepare the SQL statement (compiles the SQL and returns a statement handle).
    int rc = sqlite3_prepare_v2(db, add_data, -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    // Bind parameters (1-based indexes). Using c_str() provides a pointer
    // valid while newContact remains in scope. For safety across API
    // boundaries you can use SQLITE_TRANSIENT instead of SQLITE_STATIC.
    sqlite3_bind_text(stmt, 1, newContact.name.c_str(), -1, SQLITE_STATIC); // bind name
    sqlite3_bind_text(stmt, 2, newContact.phone.c_str(), -1, SQLITE_STATIC); // bind phone
    sqlite3_bind_text(stmt, 3, newContact.email.c_str(), -1, SQLITE_STATIC); // bind email

    // Execute the statement. SQLITE_DONE indicates successful completion of INSERT.
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE)
    {
        std::cerr << "Execution failed: " << sqlite3_errmsg(db) << "\n";
    }

    // Finalize the statement to release resources.
    sqlite3_finalize(stmt);
    return rc;
}

/*
 * Search for contacts by name and print matching rows.
 * Params:
 *  - db            : open sqlite3 database handle
 *  - contactToSeek : Contact with name set to search for
 * Returns:
 *  - Last sqlite3_step return code (or prepare error code).
 */
int seekContact(sqlite3 *db, Contact contactToSeek)
{
    const char * seek_data = "SELECT * FROM contacts WHERE name = ?;";

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, seek_data, -1, &stmt, nullptr);

    if (rc != SQLITE_OK)
    {
        std::cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << std::endl;
        return rc;
    }

    // Bind the name parameter (search term).
    sqlite3_bind_text(stmt, 1, contactToSeek.name.c_str(), -1, SQLITE_STATIC);

    bool found = false;
    // Iterate over result rows. sqlite3_step returns SQLITE_ROW for each row.
    while(sqlite3_step(stmt) == SQLITE_ROW)
    {
        found = true;
        std::cout << "\nContact Found:\n";
        std::cout << "id: " << sqlite3_column_int(stmt, 0) << "\n";
        // sqlite3_column_text returns const unsigned char*, but streaming it is OK.
        std::cout << "Name: " << sqlite3_column_text(stmt, 1) << "\n";
        std::cout << "Phone: " << sqlite3_column_text(stmt, 2) << "\n";
        std::cout << "Email: " << sqlite3_column_text(stmt, 3) << "\n";
    }

    if (!found)
    {
        std::cout << "No contact found with the name: " << contactToSeek.name << "\n";
    }

    // Finalize to free the prepared statement.
    sqlite3_finalize(stmt);

    return rc;
}

/*
 * Callback used with sqlite3_exec to print rows returned by a query.
 * Params:
 *  - unused   : user-provided pointer (unused here)
 *  - col_n    : number of columns in the result row
 *  - data     : array of strings for each column value (may be null)
 *  - col_name : array of strings for column names
 * Returns:
 *  - 0 to indicate success to sqlite3_exec
 */
int callback(void *  unused, int col_n, char ** data, char ** col_name)
{
    for (int i = 0; i < col_n; i++)
    {
        std::cout << col_name[i] << ": " << (data[i] ? data[i] : "NULL") << "\n";
    }

    std::cout<<"\n";

    return 0;
}

/*
 * Print the entire contacts table using sqlite3_exec and the callback above.
 * Params:
 *  - db : open sqlite3 database handle
 * Returns:
 *  - Result code from sqlite3_exec.
 */
int printTable(sqlite3 *db)
{
    const char * printContent = "Select * FROM contacts";
    sqlite3_stmt * stmt;

    int rc;

    if(!isEmpty)
    {
        std::cout << "-------RESULTS-------\n";

        rc = sqlite3_exec(db, printContent, callback, nullptr, nullptr);

        std::cout << "---------------------";
    }
    else
    {
        std::cout << "The contacts table is empty.\n";
        return SQLITE_OK;
    }

    return rc;
}

/*
 * Delete a contact by id using a prepared statement.
 * Params:
 *  - db : open sqlite3 database handle
 *  - id : id of the contact to delete
 * Returns:
 *  - SQLITE_OK on completion (does not check for errors here).
 */
int deleteContact(sqlite3 *db, int id)
{
    const char * removeContact = "DELETE FROM contacts WHERE id = ?";

    sqlite3_stmt * stmt;

    // Prepare the delete statement.
    sqlite3_prepare_v2(db, removeContact, -1, &stmt, nullptr);

    // Bind the id parameter and execute.
    sqlite3_bind_int(stmt, 1, id);

    sqlite3_step(stmt);

    // Finalize to free the statement object.
    sqlite3_finalize(stmt);

    return SQLITE_OK;
}
