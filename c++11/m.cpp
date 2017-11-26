#include <iostream>

using namespace std;

template<typename T> double  suma(T t)
{
	return (double)t;
}
template<typename T, typename ...Ts> double suma(T t, Ts... ts)
{
	cout << "t: " << t << " + " << sizeof...(ts) << " elems" << endl;
	return suma(t) + suma(ts...);
}

int main()
{
	cout << suma(1, 3.14) << endl <<  suma('a', 'b', 'c', 'd', 1) << endl;
	return EXIT_SUCCESS;
}

