#include "boost/shared_ptr.hpp"
#include <vector>
#include <iostream>

class A
{
    public:
	virtual void sing()=0;
    protected:
	virtual ~A() {};
};

class B : public A
{
    virtual void sing()
    {
	std::cout << "AAAAA!" << std::endl;
    }

    public:
    virtual ~B()
    {
	std::cout << "Killed" << std::endl;
    }
};

boost::shared_ptr<A> CreateA()
{
    boost::shared_ptr<A> p(new B());
    return p;
}

int main()
{
    typedef std::vector<boost::shared_ptr<A> > container_type;
    typedef container_type::iterator iterator;

    container_type c;

    for (int i=0;i<10;i++) c.push_back(CreateA());

    std::cout << "Sing: " << std::endl;
    iterator end = c.end();

    for( iterator it=c.begin(); it!=end; ++it)
    {
	(*it)->sing();
    }
}

