/**
 * this is  a implementation of state design patter
 */


 #include<iostream>

 class Player;
 class PlayerMoving;
 class PlayerStopped;
 class PlayerState {
    public:
        virtual void playerMove()  =0 ;
        virtual void playerStop()  =0; 
 };
 class PlayerMoving : public PlayerState{
    private : 
        Player* player;
    public:
        PlayerMoving(Player* player){
            this->player = player;
        }
        void playerMove(){
            // player is already moving
        }

        void playerStop(){
            player->setState(new PlayerStopped(player));
        }
 };

 class PlayerStopped : public PlayerState{
    private : 
        Player* player;
    public:
        PlayerStopped(Player* player){
            this->player = player;
        }
        void playerMove(){
            player->setState(new PlayerMoving(player));
        }

        void playerStop(){
            // player is already stopped
        }
 };


 class Player { // this is the context class
    private:
        PlayerState* state;

    public : 
       Player(){
        this->state = new PlayerStopped(this);
       }
       void setState(PlayerState* playerState){
        this->state =  playerState;
       }

       void playerMove(){
        this->state->playerMove();
       }

       void playerStop(){
        this->state->playerStop();
       }

 };