#include<string>
class Player{
    private:
        std::string name;
        char symbol;
    public:
        Player(std::string &name, char symbol){
            this -> name =  name;
            this -> symbol = symbol;
        }

        void forFeit(){
            // forFeits the game
        }

        void move(){
            // movement logic of the game
        }
};