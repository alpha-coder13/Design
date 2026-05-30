#include<iostream>
#include<memory>
class Mouse{
    public:
     virtual ~Mouse()= default;
     virtual void click() = 0;
     virtual void scroll() = 0; 
};

class Monitor{
    public:
        virtual ~Monitor()= default;
        virtual void display() = 0;
        virtual void setResolution(int , int) =0;
};

class Creator {
    public:
        virtual ~Creator()= default;
        virtual std::unique_ptr<Mouse> createMouse() = 0;
        virtual std::unique_ptr<Monitor> createMonitor() = 0;
};

class HpMouse : public Mouse{
    public:
    ~HpMouse(){std::cout<< "hp mouse destructor called";}
    void click(){std::cout << "hp mouse clicked";}
    void scroll(){std::cout<<"hq mouse scrolled";}
};

class LenovoMouse : public Mouse{
    public:
    ~LenovoMouse(){std::cout<< "Lenovo mouse destructor called";}
    void click(){std::cout << "Lenovo mouse clicked";}
    void scroll(){std::cout<<"Lenovo mouse scrolled";}
};

class HpMonitor : public Monitor{
    private:
        int height, width;
    public:
    HpMonitor():height(1080), width(1920){}
    ~HpMonitor(){std::cout<<"hp monitor destructor called";}
    void display(){std::cout<<"hp monitor is displaying at"<<this->width<<"x"<<this->height;}
    void setResolution(int h, int w){ \
        std::cout << "changing Hp monitor resolution to "<<w<<"x"<<h; 
        this->height = h;
        this->width = w;
        this->display();
    }
};


class LenovoMonitor : public Monitor{
    private:
        int height, width;
    public:
    LenovoMonitor():height(1080), width(1920){}
    ~LenovoMonitor(){std::cout<<"lenovo monitor destructor called";}
    void display(){std::cout<<"lenovo monitor is displaying at"<<this->width<<"x"<<this->height;}
    void setResolution(int h, int w){ std::cout << "lenovo doesn't support display resolution change";}
};


class HpCreator : public Creator{
    public:
        ~HpCreator(){std::cout<<"hp creator's destructor called";}
        std::unique_ptr<Monitor> createMonitor(){
            return std::make_unique<HpMonitor>();
        }
       std::unique_ptr<Mouse> createMouse(){
            return std::make_unique<HpMouse>();;
        }
};

class LenovoCreator : public Creator{
    public:
        ~LenovoCreator(){std::cout<<"lenovo creator's destructor called";}
        std::unique_ptr<Monitor> createMonitor(){
            return std::make_unique<LenovoMonitor>();
        }
       std::unique_ptr<Mouse> createMouse(){
            return std::make_unique<LenovoMouse>();
        }
};


int main(){
    std::unique_ptr<Creator> creator = std::make_unique<LenovoCreator>();
    std::unique_ptr<Monitor> monitor = creator->createMonitor();
    std::unique_ptr<Mouse> mouse = creator->createMouse();

    monitor->display();
    monitor->setResolution(2040,3020);
    mouse->scroll();
    mouse->click();

    creator = std::make_unique<HpCreator>();
    monitor = creator->createMonitor();
    mouse = creator->createMouse();


    monitor->display();
    monitor->setResolution(2040,3020);
    mouse->scroll();
    mouse->click();
    return 0;
}