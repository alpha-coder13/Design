/**
 * this is a basic implementation of command pattern
 * command pattern states that between sender code , and the reciever code, the functions executed
 * from various endpoint doing the same set of instruction should be invoking the class encapsualting the 
 * inner working of the functions 
 */

#include<iostream>
#include<stack>


template <typename t> class Memento;
class NotepadMemento;

template <typename r> class History;
class NotepadHistory;

class Application;

class Button;
class Command {
    protected:
        Application* app;
    public :
        virtual void execute() = 0  ;
 };

 class CopyCommand : public Command{
    public:
        CopyCommand(Application* app){
            this->app = app; 
        }
        void execute() {
            this->app->handleSelection("cpy");
            std::cout<<"Copy command executed";
        }
 };
 
 class PasteCommand : public Command {
    public :
        PasteCommand(Application* app){
            this->app = app; 
        }
        void execute() {
            this->app->applySelection();
            std::cout << "Paste command executed";
        }
 };
class CutCommand : public Command{
     public :
        CutCommand(Application* app){
            this->app = app; 
        }
        void execute() {
            this->app->handleSelection("cut");
            this->app->takeSnapshot();
            std::cout << "Cut command executed";
        }
};
class UndoCommand : public Command{
    public :
        UndoCommand(Application* app){
            this->app = app; 
        }
        void execute() {
            this->app->undo();
            std::cout << "Undo command executed";
        }
};

class Application {
     public:
        virtual void addToClipBoard() = 0;
        virtual void takeSnapshot() =0;
        virtual void handleSelection(std::string mode) = 0;
        virtual void applySelection() = 0;
        virtual void undo() = 0;
};
struct Selection{};
struct NotePadState{};

class NotePad : public Application{
    private:
        NotepadHistory* history;
        Selection select;
        NotePadState state;
        Button* CopyButton, *PasteButton, *CutButton;
        ShortCut* CopyShortCut, *PasteShortCut, *CutShortCut;

    public:
        NotePad() : state(NotePadState{}) , select(Selection{}){
            this->history = new NotepadHistory();
            CopyCommand copy= CopyCommand(this);
            PasteCommand paste = PasteCommand(this);
            CutCommand cut = CutCommand(this);
            this->CopyButton = new Button(&copy);
            this->CutButton = new Button(&cut);
            this->PasteButton = new Button(&paste);
            this->CopyShortCut = new ShortCut(&copy);
            this->CutShortCut = new ShortCut(&cut);
            this->PasteShortCut = new ShortCut(&paste);
        }

        void takeSnapshot(){
            NotepadMemento* memento =  new NotepadMemento(this->state);
            this->history->pushState(memento);
        }

        void handleSelection (std::string mode){
            //some selection event triggered:
            Selection selected = Selection{}; // create from event varlues
            this->select =  selected;

            if(mode == "cut"){
                // do remove the section;
            }
        }


        void applySelection(){
            // get pointer l;ocation;
            // append values from the selection
        }

        void undo() {
            NotepadMemento* memento = this->history->popState();
            this->state = memento->getState();
            delete(memento);
        }

};

template <typename t>
class Memento {
    public:  
       virtual  t getState() = 0;
};

class NotepadMemento : public Memento<NotePadState>{
    private:
        NotePadState state;
        friend class NotePad;
        NotePadState getState(){
            return this -> state;
        }
    public:
        NotepadMemento(NotePadState appState){
            this->state = appState;
        }
};

template <typename r>
class History{
    public :
        virtual void pushState(r* historyState) = 0;
        virtual r* popState() =0;
};

class NotepadHistory : public History<NotepadMemento>{
    private:
        std::stack<NotepadMemento*> history;
    public:
        void pushState(NotepadMemento* historyState){
            history.push(historyState);
        }

        NotepadMemento* popState(){
            NotepadMemento* memento =  history.top();
            history.pop();
            return memento;
        } 
};


class Button {
    protected : 
        Command* cmd;
    public :
        Button(Command* cmd){
            this->cmd = cmd;
        }    
        void click(){
            cmd->execute();
        }
    };


class ShortCut {
    protected : 
        Command* cmd;
    public :
        ShortCut(Command* cmd){
            this->cmd = cmd;
        }    
        void trigger(){
            cmd->execute();
        }
    };