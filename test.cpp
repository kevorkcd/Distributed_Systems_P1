#include "bully.hpp"
#include "singleton.hpp"

/**
 * TO-DO:
 * 1. Compare ID's
 * 2. Network interface with bully class friend
 * 3. Vector of bullies - Possible in network interface
 * 4. The Leader Election algoritm
 * 
 * TO-DO: Optional
 * 1. Interpreter - SO COOL
 *    - Commands: mknode->ID, mknodes->amount, shutdown->ID, fail->ID, boot->ID,
 *                lsnodes, rmnode->ID
 * 2. Implement nodes on threads
 * 3. Bully.run() function for thread
 * 
 */

int main() {
    {   // Testing basic message logic
        Bully* b0 = new Bully(0);
        Bully* b1 = new Bully(1);

        b0->send_message(ELECTION, b1);
        b1->send_message(COORDINATOR, b0);

        cout << "Leader ID: " << b0->get_leader() << endl;
    }
    {   // Testing singleton class
        SingletonBully* network1 = SingletonBully::get_instance();
        SingletonBully* network2 = SingletonBully::get_instance();
    }

    return 0;
}