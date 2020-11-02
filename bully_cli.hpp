#include "bully.hpp"

/* Commands:
 *   mknode->ID
 *   mknodes->amount
 *   shutdown->ID
 *   fail->ID
 *   boot->ID
 *   lsnodes
 *   rmnode->ID
*/

class BullyCLI{
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

    while(true) {
        switch(input) {
            case mknodes:

        }
    }
}