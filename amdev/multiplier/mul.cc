#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <iterator>

int main(int lb, char** par)
{
    std::vector<int> A;
    std::string str;
    int v;

    for (int i=1;i<lb;i++)
    {
	if (std::stringstream( str = std::string( par[i] ) ) >> v)
	{
	    std::cout << "store number: " << v << std::endl;
	    A.push_back( v );
	}
	else
	{
	    std::cout << str << ": is not a integer value" << std::endl;
	}
    }


    std::ostream_iterator<int> int_cout(std::cout, "\t");

    std::cout << "A   table: ";
    std::copy(A.begin(), A.end(), int_cout);
    std::cout << std::endl;


    int n = A.size();
    std::vector<int> B(n), C(n);

    if (n < 2)
    {
	std::cout << "need at least 2 elements" << std::endl;
	return 1;
    }

    std::copy(A.begin(), A.end(), C.begin());
    std::copy(A.begin(), A.end(), B.begin());

    for (int i=1;i<n;i++)
    {
	B[i] *= B[i-1];
    }

    for (int i=n-2;i>=0;i--)
    {
	C[i] *= C[i+1];
    }

    for (int i=0;i<n;i++)
    {
	if (i == 0) A[0] = C[1];
	else if (i == (n-1)) A[n-1] = B[n-2];
	else A[i] = B[i-1] * C[i+1];
    }

    std::cout << "MUL table: ";
    std::copy(A.begin(), A.end(), int_cout);
    std::cout << std::endl;

    std::cout << "B   table: ";
    std::copy(B.begin(), B.end(), int_cout);
    std::cout << std::endl;

    std::cout << "C   table: ";
    std::copy(C.begin(), C.end(), int_cout);
    std::cout << std::endl;

    return 0;
}

