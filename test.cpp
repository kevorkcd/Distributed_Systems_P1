#include "singleton.hpp"
#include "bully_cli.hpp"

int main() {
    {
        SingletonBully* network = SingletonBully::get_instance();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        network->make_nodes(2);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        network->boot_nodes();
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        network->list_nodes();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        network->shutdown(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        network->shutdown(2);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        network->list_nodes();
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        network->boot_node(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
        network->boot_node(2);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        network->list_nodes();
        /*BullyCLI* UI = new BullyCLI();
        UI->run();*/
        network->join_threads();
        // Online: 3, 7, 8, 9
    }

    return 0;
}