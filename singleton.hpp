#include "bully.hpp"

class Singleton : public Bully {
    private:
        static Singleton* instance;
        static Bully* Network;
        Singleton();
    public:
        static Singleton* get_instance();
};

Singleton* Singleton::instance = 0;

Singleton* Singleton::get_instance() {
    if (instance = 0) {
        instance = new Singleton();
    }
    return instance;
}

Singleton::Singleton() {
    this->Network = new Bully();
}