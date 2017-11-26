#include <boost/lambda/lambda.hpp>
#include <boost/smart_ptr.hpp>
#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
    const int n = 100;
    boost::scoped_array<int> p(new int[n]);
    std::vector<int> v;

    //for_each(v.begin(), v.end(), boost::lambda::_1 = i++);

    std::generate_n(std::back_inserter(v), n, rand);
    for_each(v.begin(), v.end(), std::cout << boost::lambda::_1 << " ");
    std::cout << std::endl;

    std::random_shuffle(v.begin(), v.end());
    std::sort(v.begin(), v.end(), boost::lambda::_1 < boost::lambda::_2); 

    for_each(v.begin(), v.end(), std::cout << boost::lambda::_1 << " ");
    std::cout << std::endl;
}

