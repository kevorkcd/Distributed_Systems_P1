#include "bully.hpp"
#include "singleton.hpp"

int main() {
    {   // Testing basic message logic
    /*
        Bully* b0 = new Bully(0);
        Bully* b1 = new Bully(1);

        b0->send_message(ELECTION, b1);
        b1->send_message(COORDINATOR, b0);

        cout << "Leader ID: " << b0->get_leader() << endl;
    */
    }
    // {   // Testing singleton class
    //     SingletonBully* network1 = SingletonBully::get_instance();
    //     SingletonBully* network2 = SingletonBully::get_instance();
    // }
    {
        SingletonBully* network = SingletonBully::get_instance();
        network->make_node(5);
        network->make_node(10);
        network->list_nodes();
        network->boot(5);
        network->boot(10);
        network->list_nodes();
    }

    return 0;
}