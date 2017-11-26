#include <iostream>

using namespace std;

auto sum = [](double a, double b) -> double{ return a+b; };

int main()
{
	cout << sum(2, 3) << endl;
	int A[] = {1,2,3,4,5,6,7,8,9};
	for (int el : A)
	{
		[](int e)
		{
			cout << e << ", ";
		}(el);
	}
	cout << endl;
	return 0;
}

