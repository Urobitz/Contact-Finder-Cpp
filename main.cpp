#include <iostream>
#include <sqlite3.h>
#include "sql_functions.h"
#include <limits>

void printTitle()
{
    std::cout << "*********************\n";
    std::cout << "* Contact Book App *\n";
    std::cout << "*********************\n"; 
}

void MenuManager(sqlite3 *db)
{

    int choice = 0;

    do
    {
        std::cout << "1. Add Contact\n";
        std::cout << "2. View Contacts\n";
        std::cout << "3. Search Contact\n";
        std::cout << "4. Delete Contact\n";
        std::cout << "5. Exit\n";

        std::cout << "Choose an option: ";
        std::cin >> choice;

        if(std::cin.fail() || choice < 1 || choice > 5)
        {
            std::cin.clear(); // Clear the error flag
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            choice = 0; // Reset choice to an invalid option

            std::cout << "Invalid choice. Please try again.\n\n";
            printTitle();
            continue; // Skip the rest of the loop and prompt again
        }

        Contact newContact;

        switch (choice)
        {
        case 1:
            
        // Add Contact  
            std::cout << "Enter contact information: \n";
            std::cout << "Name: ";
            std::cin >> newContact.name;
            std::cout << "Phone: ";
            std::cin >> newContact.phone;
            std::cout << "Email: ";
            std::cin >> newContact.email;


            break;
        
        default:
            break;
        }

    } while (choice != 5);

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

    createTable(database_handle);

    printTitle();
    MenuManager(database_handle);



    
}
