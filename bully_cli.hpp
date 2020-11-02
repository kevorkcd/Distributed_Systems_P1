#include "bully.hpp"
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

class BullyCLI : public Bully {
    public:
        BullyCLI();
    private:
        void make_node();
        void make_nodes();
        void shutdown();
        void fail();
        void shutdown();
        void fail();
        void boot();
        void lsnode();
        void rmnode();
};

BullyCLI::BullyCLI() {
    string input;
    cin >> input;

    while (true) {
        if (input == "mknode") {
            cout << "Enter ID: ";
            int ID;
            cin >> ID;
            make_node(ID);
        }
    }
}