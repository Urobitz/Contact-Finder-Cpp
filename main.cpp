#include <iostream>
#include <sqlite3.h>

void printTitle()
{
    std::cout << "*********************\n";
    std::cout << "* Contact Book App *\n";
    std::cout << "*********************\n"; 
}

int main()
{
    sqlite3 * database_handle = nullptr;
    
    signed int error_code = 0;

    error_code = sqlite3_open("Contact_Book.db", &database_handle);

    if (error_code != SQLITE_OK)
    {
        std::cerr << "Failed to open database." << std::endl;
        return 1;
    }

    std::cout << "Database opened successfully." << std::endl;

    printTitle();
}
