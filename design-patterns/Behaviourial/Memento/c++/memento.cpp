/**
 * this is a implementation of memento pattern
 */
#include<string>
#include<stack>
struct CursorPosition {
    int X;
    int Y;
};

struct ScrollPosition{
    int ScrollX;
    int ScrollY;
};

struct BrowserState{
    std::string url;
    CursorPosition cursorPosition;
    ScrollPosition scrollPosition;
};
template <typename R>  class HistoryStore;
class BrowserHistoryStore;
template <typename T> class Memento;
class BrowserMemento;

class BrowserNav{
    private:
        BrowserState browserState;
        BrowserHistoryStore* history;   
    public:
        BrowserNav(std::string url){
            this->browserState.url = url;
            this->browserState.cursorPosition.X=0;
            this->browserState.cursorPosition.Y=0;
            this->browserState.scrollPosition.ScrollX =0;
            this->browserState.scrollPosition.ScrollY =0; 
            this->history= new BrowserHistoryStore();
        }

        void saveState(){
            BrowserMemento memento = BrowserMemento(this->browserState);
            this->history->push_state(memento);
        }

        void useLastState(){
            BrowserMemento memento = this->history->pop_state();
            this->browserState = memento.getState();
        }

};

template<typename T>
class Memento{
    public:
        virtual T getState() const = 0; 
};

class BrowserMemento : public Memento<BrowserState>{
    private:
        BrowserState browserState;
        friend class BrowserNav; // important why ?
        BrowserState getState() const override{
            return this->browserState;
        }
    public:
        BrowserMemento(BrowserState browserState){
            this->browserState = browserState;
        }

};

template <typename R>
class HistoryStore{
    public:
        virtual void push_state(R) =0;
        virtual R pop_state() = 0;
};


class BrowserHistoryStore : public HistoryStore<BrowserMemento>{
    private:
        std::stack<BrowserMemento> historyStack;
    public:
        BrowserHistoryStore(){};
        void push_state(BrowserMemento memento){
            historyStack.push(memento);
        }

        BrowserMemento pop_state(){
            BrowserMemento top = historyStack.top();
            historyStack.pop();
            return top;
        }
};

/**
 * key takes : friend keywqord in cpp helps in allowing private properties of a class to be accesssed by the friend class
 * 
 */