#include <iostream>
#include "sql_functions.h"
#include <sqlite3.h>
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
            // Search Contact
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nEnter contact name to search: ";
            std::getline(std::cin, searchContact.name);
            seekContact(db, searchContact);
            std::cout << std::endl; //for visual clarity
            break;

        case 3:

        std::cout << std::endl; //for visual clarity
        printTable(db); 
        std::cout << std::endl; //for visual clarity

        break;

        case 4:
        {

            if(!isEmpty(db)){
                // Delete Contact
                unsigned int id = 0;

                std::cout << std::endl;

                std::cout << "Enter id of contact to delete: ";
                std::cin >> id;

                deleteContact(db, id);

                std::cout << "Contact with ID #"<<id << " has been deleted";
                break;
            }
            else
            {
                std::cout << "\nThe contacts table is empty. No contacts to delete.\n";
                break;
            }
        }
        
        default:
            std::cout << "Exiting the program.\n";
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
    MenuManager(database_handle);

    return 0;
}

