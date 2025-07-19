#include <string>
#include <iostream>
#include "BrowserTabs.h"

enum Command {
    GO,
    INSERT,
    BACK,
    FORWARD,
    REMOVE,
    PRINT,
    SORT,
    UNKNOWN
};

Command hashCommand(const std::string& cmd) 
{
    if (cmd == "GO") 
        return GO;
    if (cmd == "INSERT") 
        return INSERT;
    if (cmd == "BACK") 
        return BACK;
    if (cmd == "FORWARD") 
        return FORWARD;
    if (cmd == "REMOVE") 
        return REMOVE;
    if (cmd == "PRINT") 
        return PRINT;
    if (cmd == "SORT")
        return SORT;
    return UNKNOWN; 
}

void processCommand(BrowserTabs& tabs, const std::string& command) 
{
    std::string action, argument;

    if (command.substr(0, 2) == "$ ") 
    {
        action = command.substr(2);
    }
    else 
    {
        std::cout << "Invalid command format. Commands should start with '$ '." << std::endl;
        return;
    }

    if (action.find(" ") != std::string::npos) 
    {
        argument = action.substr(action.find(" ") + 1);

        argument.erase(argument.find_last_not_of(" \t\n\r") + 1);

        action = action.substr(0, action.find(" "));
    }

    switch (hashCommand(action)) {
        case GO:
            tabs.go(argument);
            break;
        case INSERT:
            tabs.insert(argument);
            break;
        case BACK:
            tabs.back();
            break;
        case FORWARD:
            tabs.forward();
            break;
        case REMOVE:
            tabs.remove();
            break;
        case PRINT:
            tabs.print();
            break;
        case SORT:
            tabs.sort(argument);
            break;
        default:
            std::cout << "Unknown command: " << action << std::endl;
    }
}

int main() {
    BrowserTabs tabs;
    std::string command;

    std::cout << "Enter browser commands (type EXIT to quit):" << std::endl;

    while (true) 
    {
        std::getline(std::cin, command);

        if (command == "$ EXIT") 
        {
            break;
        }

        processCommand(tabs, command);
    }
}