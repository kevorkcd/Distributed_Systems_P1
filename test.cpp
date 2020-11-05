#include "singleton.hpp"

int main() {
    {
        SingletonBully* network = SingletonBully::get_instance();
        network->make_node(5);
        network->make_node(10);
        network->list_nodes();
        network->boot(5);
        network->list_nodes();
        // this_thread::sleep_for(chrono::milliseconds 2000);
        network->boot(10);
        network->list_nodes();
        network->join_threads();
        network->list_nodes();
    }

    return 0;
}