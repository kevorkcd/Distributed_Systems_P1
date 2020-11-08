#pragma once

#include "singleton.hpp"
#include <string>

using namespace std;

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
            int ID;
            cin >> ID;
            Network->make_node(ID);
        }
       else if (input == "mknodes") {
            int node_amount;
            cin >> node_amount;
            Network->make_nodes(node_amount);
        }
        else if (input == "btnode") {
            int boot_ID;
            cin >> boot_ID;
            Network->boot_node(boot_ID);
        }
        else if (input == "btnodes") {
            cout << "Booting all offline nodes." << endl;
            Network->boot_nodes();
        }
        else if (input == "kill") {
            int shut_ID;
            cin >> shut_ID;
            Network->shutdown_node(shut_ID);
        }
        else if (input == "killall") {
            Network->shutdown_nodes();
        }
        else if (input == "crash") { 
            int fail_ID;
            cin >> fail_ID;
            Network->fail(fail_ID);
        }
        else if (input == "ls") {
            Network->list_nodes();
        }
        else if (input == "help") {
            Network->help();
        }
        else {
            cout << "Input is unrecognized, retry." << endl;
        }
    }
}