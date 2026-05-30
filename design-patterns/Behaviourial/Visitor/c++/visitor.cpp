/**
 * this is a implementation of visitor pattern 
 */
#include<iostream>
class People;
class Student;
class WorkingProfessional;
class Unemployed;

class Visitor{
    public :
        virtual void visitStudent(const Student*) const =0;
        virtual void visitWorkingProfessional(const WorkingProfessional*) const =0;
        virtual void visitUnemployed(const Unemployed*) const =0;
};
class DetailsVisitor : public Visitor {
    public :
        void visitStudent(const Student* people) const override{
            std::cout<<"exeute Print statements called for Student" ;
            /* further data manipulation if required*/
        }
        void visitWorkingProfessional(const WorkingProfessional* people) const override {
            std::cout<<"exeute Print statements called for WorkingProfessional" ;
            /* further data manipulation if required*/
        }
        void visitUnemployed(const Unemployed* people) const override{
            std::cout<<"exeute Print statements called for Unemployed" ;
            /* further data manipulation if required*/
        }
};
class VisitorAcceptor{
    public:
        virtual void acceptVisitor(Visitor*) const = 0;   
};

class People {
    public:
        virtual void getPersonalInfo() const = 0;
};

class Student : public People , public VisitorAcceptor{
    public:
        void getPersonalInfo() const override {
            std:: cout<< "Student Personal Info" << "\n";
        }
        void acceptVisitor(Visitor* visitor) const override{
            visitor->visitStudent(this);
        }
};

class WorkingProfessional : public People , public VisitorAcceptor{
    public:
        void getPersonalInfo() const override{
            std:: cout<< "WorkingProfessional Personal Info" << "\n";
        }
        void acceptVisitor(Visitor* visitor) const override{
            visitor->visitWorkingProfessional(this);
        }
};

class Unemployed : public People , public VisitorAcceptor{
    public:
        void getPersonalInfo() const override{
            std:: cout<< "Unemployed Personal Info" << "\n";
        }
        void acceptVisitor(Visitor* visitor) const override{
            visitor->visitUnemployed(this);
        }
};

int main(){
    Student st = Student();
    WorkingProfessional pro = WorkingProfessional();
    Unemployed sad = Unemployed();

    Visitor* peopleVisitor = new DetailsVisitor();

    st.acceptVisitor(peopleVisitor);
    pro.acceptVisitor(peopleVisitor);
    sad.acceptVisitor(peopleVisitor);
    return 0;
}