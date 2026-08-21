namespace utils{
    struct ID{
        int counter; 
    };

    template<typename T>
    class UID{
        private:
            static ID id{0};
            static long long generateUID(){
                return ++id.counter;
            }
            friend T;
        public:    
            UID() :{}
            ~UID(){}
    };
}