/**
 * this is a implementation of template desing pattern in c++
 */

#include<iostream>

struct Point{
    int x;
    int y;
};

 class SphericalElements{
    public:
        virtual Point setCoordinates(int, int) = 0;
        virtual int setRadius(int) =0;
        virtual std::string getType() { 
            return "Spherical Element"; 
        } ;
        virtual void render(Point p , int radius, std::string type) final{
            std::cout << "this is the rendering of the " << type << " spherical element\n";
        };
        virtual void collide() {}
        virtual void create(int x, int y , int rad) final {
            Point coordinates = setCoordinates(x, y);
            int radius = setRadius(rad);
            std::string type = getType();
            render(coordinates, radius, type);
        }
 };



 class BouncingBall : public SphericalElements {
    public:
    Point setCoordinates(int x, int y) override{
        return Point{x, y};
      }
      int setRadius(int radius) override{
        return radius;
      }
      std::string getType() override{
        return "Bouncing Ball";
      }

 };


 class BombBall : public SphericalElements {
    public:
      Point setCoordinates(int x, int y) override{
        return Point{x, y};
      }
      int setRadius(int radius) override{
        return radius;
      }
       std::string getType() override{
        return "Bomb Ball";
      }
      void  collide() override {
        std::cout << "BOOM";
      }
 };


int main(){
    BouncingBall ball1 =  BouncingBall();
    ball1.create(10, 20, 5);
    SphericalElements* ball2 = new BombBall();
    ball2->create(15, 25, 10);
    ball2->collide();
    delete ball2;
}


/***
 * 
 * 
 * 
 * this is a  naive implementation of the template pattern , not to be exactly used in production code , 
 * but it is a good example to understand the template pattern and how it works in c++ ,
 */