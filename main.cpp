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

int MenuManager(sqlite3 *db)
{

    int choice = 0;

    do
    {
        printTitle();

        std::cout << "1. Add Contact\n";
        std::cout << "2. Search Contact\n";
        std::cout << "3. View Contacts\n";
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
        Contact searchContact;

        switch (choice)
        {
        case 1:
            
        // Add Contact  
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear leftover newline
            std::cout << "\nEnter contact information\n";
            std::cout << "Full Name: ";
            std::getline(std::cin, newContact.name);
            std::cout << "Phone: ";
            std::cin >> newContact.phone;
            std::cout << "Email: ";
            std::cin >> newContact.email;

            addContact(newContact, db);
            break;

        case 2:
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nEnter contact name to search: ";
            std::getline(std::cin, searchContact.name);
            seekContact(db, searchContact);
            std::cout << std::endl; //for visual clarity
            printTitle();
            break;

        case 3:

        std::cout << std::endl;
        printTable(db);
        std::cout << std::endl;

        break;
        
            
        
        default:
            std::cout << "Exiting the program.\n";
            return 0;
            break;
        }

        std::cout << std::endl;

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
    
    while (MenuManager(database_handle) != 0)
    {
        MenuManager(database_handle);
    }
    



    
}
