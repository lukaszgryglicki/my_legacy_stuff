#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>
#include <exception>
#include <boost/smart_ptr.hpp>
#include <boost/timer/timer.hpp>

int main(int lb, char** par)
{
    try
    {
	boost::timer::auto_cpu_timer act;
	std::string str;
	unsigned long n;

	if (lb < 2) throw std::string("number argument required");

	if (std::stringstream( str = std::string( par[1] ) ) >> n)
	{
	    if (n <= 0) throw std::string("cannot allocate negative or zero memory");
	    std::cout << "will try to alloc: " << n << std::endl;
	}
	else
	{
	    std::cout << str << ": is not a integer value" << std::endl;
	    throw std::string("not a number");
	}

	boost::shared_array<long double> arr(new long double[n]);
	std::cout << "allocated: " << n << std::endl;

	for (unsigned long i=0;i<n;i++) arr[i] = 'X';
	std::cout << "assigned: " << n << std::endl;
    }
    catch(std::string s)
    {
	std::cout << "exception: " << s << std::endl;
    }
}


