#include<string>
#include<iostream>
#include "./utils.cpp"
#include "./board.cpp"
class Player{
    private:
        std::string name;
        char symbol;
        utils::ID playerId;
        Games::Games current;

    public:
        Player(std::string &name){
            this->playerId = utils::UID<Player>::generateUID();
            this -> name =  name;
        }
        utils::ID getPlayerID(){
            return playerId;
        }

        void joinGame(utils::ID gameId){
            current = Games::GameRegistry::getGame(gameId);
            current.joinGame();
            return;
        }
        void forFeit(){
            // forFeits the game
            current.forFeit();
            return;
        }
        void move(){
            // movement logic of the game
        }
        void hasWon(){
            // log  : You have
        }
        void hasLost(){
            // log  : You lost
        }
        
        
};


class PlayerCreator{
    public:
     Player createPlayer(){
        // prompt to EnterName;
        std::string name;
        std::cout << " Enter your name\n";
        std::cin >> name;
        return Player(name);
     }
};

