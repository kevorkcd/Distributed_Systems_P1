#pragma once

#include "singleton.hpp"
#include <string>

using namespace std;

/* Commands:
 *   mknode->ID
 *   mknodes->amount
 *   shutdown->ID
 *   fail->ID
 *   boot->ID
 *   lsnodes
 *   rmnode->ID
*/

class BullyCLI {
    public:
        BullyCLI();
        void run();
};

BullyCLI::BullyCLI() {
}

void BullyCLI::run() {
    SingletonBully* Network = SingletonBully::get_instance();

    while (true) {
        cout << "$ ";
        string input;
        cin >> input;
        if (input == "mknode") {
            cout << "Enter ID to assemble: ";
            int ID;
            cin >> ID;
            Network->make_node(ID);
        }
       else if (input == "mknodes") {
            cout << "Enter how many nodes you want to create: ";
            int node_amount;
            cin >> node_amount;
            Network->make_nodes(node_amount);
        }
        else if (input == "boot") {
            cout << "Enter the ID of the node you want to boot: ";
            int boot_ID;
            cin >> boot_ID;
            Network->boot(boot_ID);
        }
        else if (input == "shutdown") {
            cout << "Enter the ID of the node you want to shutdown: ";
            int shut_ID;
            cin >> shut_ID;
            Network->shutdown(shut_ID);
        }
        else if (input == "fail") { 
            cout << "Enter ID for the node you want to simulate as a failure: ";
            int fail_ID;
            cin >> fail_ID;
            Network->fail(fail_ID);
        }
        else if (input == "ls") {
            Network->list_nodes();
        }/*
        else if (input == "rmnode") {
            int rm_node_ID;
            cin >> rm_node_ID;
            Network->remove_node(rm_node_ID);
        }*/
        else if (input == "help") {
            Network->help();
        }
        else {
            cout << "Input is unrecognized, retry." << endl;
        }
    }
}