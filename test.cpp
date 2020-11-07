#include "singleton.hpp"
#include "bully_cli.hpp"

int main() {
    {
        SingletonBully* network = SingletonBully::get_instance();
        network->make_nodes(10);
        network->boot_nodes();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        network->shutdown(6);
        network->fail(5);
        network->fail(4);
        network->fail(1);
        network->shutdown(2);
        network->shutdown(10);
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        network->list_nodes();
        BullyCLI* UI = new BullyCLI();
        UI->run();
        network->join_threads();
        // Online: 3, 7, 8, 9
    }

    return 0;
}