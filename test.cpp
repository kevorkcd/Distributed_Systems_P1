#include "singleton.hpp"
#include "bully_cli.hpp"
#include <fstream>

#define SAMPLE_SIZE     10
#define MAX_NODE_AMOUNT 100
#define DATA_POINTS     10

int main() {
    {
        ofstream output;
        output.open("test1.txt");

        SingletonBully* network = SingletonBully::get_instance();

        for (int i = 10; i <= MAX_NODE_AMOUNT; i = i + MAX_NODE_AMOUNT/DATA_POINTS) {
            vector<int> sample;

            network->make_nodes(10);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            network->boot_nodes();
            std::this_thread::sleep_for(std::chrono::milliseconds(i/DATA_POINTS * 1000));
            network->list_nodes();
            std::this_thread::sleep_for(std::chrono::milliseconds(500));

            output << "Node Amount: " << i << endl;
            output << "\tm_b" << "\tm_a" << "\tm" << endl;
            for (int j = 0; j < SAMPLE_SIZE; j++) {
                int m_b = network->get_message_no();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                network->shutdown_node(i);
                std::this_thread::sleep_for(std::chrono::milliseconds(i/DATA_POINTS * 1000));
                network->list_nodes();
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                int m_a = network->get_message_no();
                int m = m_a-m_b;
                sample.push_back(m);

                output << j+1 << "\t" << m_b << "\t" << m_a << "\t" << m << endl;

                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                network->boot_node(i);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
            double avg = 0;
            for (int j = 0; j < sample.size(); j++) {
                avg += sample[j];
            }
            avg /= SAMPLE_SIZE;
            output << "AVERAGE: " << avg << endl;
        }
        network->join_threads();
    }

    return 0;
}