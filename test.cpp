#include "singleton.hpp"
#include "bully_cli.hpp"

int main() {
    {
        SingletonBully* network = SingletonBully::get_instance();
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        network->make_nodes(10);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        network->boot_nodes();
        std::this_thread::sleep_for(std::chrono::milliseconds(1500));
        network->shutdown(6);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        network->fail(5);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        network->fail(4);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        network->fail(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        network->shutdown(2);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        network->shutdown(10);
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        network->list_nodes();
        BullyCLI* UI = new BullyCLI();
        UI->run();
        network->join_threads();
        // Online: 3, 7, 8, 9
    }

    return 0;
}