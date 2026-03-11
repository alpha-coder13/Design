#include<iostream>
// factory pattern is a creational pattern
//  where we have creator class and Product interface, where the creator class creates comething of the product interface 


class Product {
    public:
        virtual ~Product(){
            std::cout << "Product Destructor called \n";
        };
        virtual std::string Operation() const  = 0;
};

class Product1 : public Product {
    public:
    ~Product1() override {
            std::cout << "Product Destructor called \n"; 
            // c++ automatically calls the base destructor
    };

    std::string Operation() const override{
        return "This is some operation on product 1";
    }
};

class Product2 : public Product {
    public:
    ~Product2() override {
            std::cout << "Product Destructor called \n"; 
            // c++ automatically calls the base destructor
    };

    std::string Operation() const override{
        return "This is some operation on product 2\n";
    }
};




class Creator{
    public:
        virtual ~Creator(){};
        virtual Product* createProduct() const = 0;

        void doSomething(Product* prd) {
            std::cout << "this is creator calling product operation\n";
            std::cout << prd->Operation();
        }
};


class Creator1 : public Creator{
    public:
        Product* createProduct() const override{
            std::cout << "Product 1 created\n";
            return new Product1();
        }
};

class Creator2 : public Creator{
    public:
        Product* createProduct() const override{
            std::cout << "Product 2 created\n";
            return new Product2();
        }
};


int main(){

    Creator1* c1 = new Creator1();
    Creator2* c2 = new Creator2();


    Product* P1 = c2->createProduct();
    Product* P2 = c1->createProduct();

    c1->doSomething(P1);
    c2->doSomething(P2);

    delete P1;
    delete P2;


    return 0;
}