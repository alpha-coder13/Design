#include<vector>
#include<string>
class TicTacToeBoardGame {
    private: 
        int board_dimension_row;
        int board_dimension_col;
        // define turn to prevent multiple input form same iuser 
        std::vector<std::vector<std::string>> ticTacToeBoard;

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
            // set and inintialize the boardContainer
        }

        void move(int move_to_row, int move_to_col, std::string symbol){
            
            // validation logics,
            bool isAValidMove = this->isAValidMove(move_to_row, move_to_col);
            
            if(!isAValidMove){
                // error logger to throw bad move error;
                return;
            }
            // movement logics,
            // place the value on the board;
            this->placeOnBoard(move_to_row, move_to_col,symbol);

            bool hasWonOnThisMove = this->hasThePlayerWon(symbol);
            // winning logics

            if(hasWonOnThisMove){
                // successLogger for winning player
            }

            return;
        }
};