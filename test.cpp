#include "singleton.hpp"

int main() {
    {
        SingletonBully* network = SingletonBully::get_instance();
        network->make_nodes(3);
        network->boot_nodes();
        network->list_nodes();
        std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        network->shutdown(2);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        network->list_nodes();
        network->join_threads();
        network->list_nodes();
    }

    return 0;
}