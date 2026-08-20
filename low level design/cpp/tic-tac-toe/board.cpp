#include<vector>
#include<string>
#include<unordered_map>
#include<unordered_set>
#include "./utils.cpp"
#include "./player.cpp"

namespace symbols{
    std::string Cross = "X";
    std::string Circle = "O"; 
}
namespace  Games{
    class Games{
        public:
            virtual ~Games(){};
            virtual void addPlayer() =0 ;
            virtual void move() =0 ;
            virtual void forFeit() = 0 ;
    };

    class GameRegistry {
        private:
            static std::unordered_map<utils::ID , GameDetails> gameRegistry;
            static void addGame(Games &ref){
                utils::ID gameID = utils::UID<Games>::generateUID();
                GameRegistry::gameRegistry[gameID] = ref;
                return;
            }
            friend Games;
        public : 
            static Games getGame(utils::ID id){
                if(GameRegistry::gameRegistry.find(gameID) != GameRegistry::gameRegistry.end()  ){
                    return GameRegistry::gameRegistry[id];
                }else{
                    // error log no such game found;
                    return null;
                }
            }
            
    };
    class TicTacToeBoardGame : public Games{
        private: 
            int board_dimension_row;
            int board_dimension_col;
            // define turn to prevent multiple input form same iuser 
            std::vector<std::vector<std::string>> ticTacToeBoard;
            std::unordered_map<utils::ID, std::string> playerIDToSymbolMap;
            std::unordered_set<std::string> remainingSymbols;

        private:
            bool isAValidMove(int move_to_row, int move_to_col) const {
                if(move_to_row >=0  && move_to_col >=0 && move_to_col < this->board_dimension_col && move_to_row < this->board_dimension_row ){
                    // board validation logic for valid space goes here 
                    if(this->ticTacToeBoard[move_to_row][move_to_col] == "")
                    {
                        return true;
                    }
                }
                return false;
            }

            void placeOnBoard(int move_to_row, int move_to_col, std::string symbol){
                this->ticTacToeBoard[move_to_row][move_to_col] = symbol;
                return;
            }

            bool hasThePlayerWon(std::string symbol) const {
                // get the boardState
                // auto boardState
                // currently i am useing basic n^2 strategy and will improve on it : MAIN problem.
                
                for(int i =0 ; i < this->board_dimension_row; i++){
                    bool hasWonFromRowIteration = true;
                    for(int j = 0 ; j <this->board_dimension_col ;j++ ){
                        if(this->ticTacToeBoard[i][j]!=symbol){
                            hasWonFromRowIteration = false;
                            break;
                        }
                    }
                    if(hasWonFromRowIteration)return true;
                }
                for(int j = 0 ; j <this->board_dimension_col ;j++ ){
                    bool hasWonFromColIteration = true;
                    for(int i =0 ; i < this->board_dimension_row; i++){
                        if(this->ticTacToeBoard[i][j]!=symbol){
                            hasWonFromColIteration = false;
                            break;
                        }
                    }
                    if(hasWonFromColIteration)return true;
                }
                
                bool hasWonFromDiagonalLRIteration = true;

                for(int i =0 , j =0 ; i < this->board_dimension_col && j < this->board_dimension_col ;i++,j++){
                    if(this->ticTacToeBoard[i][j]!=symbol){
                        hasWonFromDiagonalLRIteration = false;
                        break;
                    }
                }

                if(hasWonFromDiagonalLRIteration)return true;

                bool hasWonFromDiagonalRLIteration = true;

                for(int i =0 , j =this->board_dimension_col-1 ; i < this->board_dimension_col && j >= 0;i++,j--){
                    if(this->ticTacToeBoard[i][j]!=symbol){
                        hasWonFromDiagonalRLIteration = false;
                        break;
                    }
                }

                if(hasWonFromDiagonalRLIteration)return true;

                return false;
            }
        
            

        public: 
            TicTacToeBoardGame( const int rows, const int cols){
                this->board_dimension_col =  rows;
                this->board_dimension_col =  cols;
                this->ticTacToeBoard.resize(rows,std::vector<std::string>(cols,""));
                GameRegistry::addGame(this);
                // set and inintialize the boardContainer
            }

            void joinGame(Player &player){
                if(playerIDToSymbolMap.size()>=2) {
                    // log  : already 2 players : wait or create a new game
                }
                // option to choose the symbols
                string selectedSymbol ;
                utils::ID plid= player.getPlayerID();
                playerIDToSymbolMap[plid] = selectedSymbol;
            }
            void move(Player& player ,int move_to_row, int move_to_col ){
                
                bool isAValidMove = this->isAValidMove(move_to_row, move_to_col);
                
                if(!isAValidMove){
                    // error logger to throw bad move error;
                    return;
                }

                if(this->playerIDToSymbolMap.count(player.getPlayerID())<=0){
                    // error log , this player is not in the game.
                    return 
                }
                std::string playerSymbol = this->playerIDToSymbolMap.[player.getPlayerID()];
                this->placeOnBoard(move_to_row, move_to_col,playerSymbol);

                bool hasWonOnThisMove = this->hasThePlayerWon(playerSymbol);

                if(hasWonOnThisMove){
                    // successLogger for winning player
                    // failureLogger for the loosing player
                    // save the game snapshot 
                    // clear the pannel
                }

                return;
            }
            
            void forFeit(Player& player ){
                utils::ID plid= player.getPlayerID();
                if(this->playerIDToSymbolMap.count(plid)<=0){
                    // error log , this player is not in the game.
                    return 
                }
                
                int removalStatus = this->playerIDToSymbolMap.erase(pid);
                
                if(removalStatus == )

            }
    };
};