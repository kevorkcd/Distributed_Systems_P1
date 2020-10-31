class Bully {
    private:
        bool online;
        int ID;

    public:
        Bully(int ID);

        bool is_online();       // Return referenced node state

        // Election messages
        void election();        // Sendes election message to Object
        void OK();              // Replies to election message that it's heard
        void coordinator();     // Notifies all instances of 
        int get_id();
        void shutdown();
        void bootup();
};