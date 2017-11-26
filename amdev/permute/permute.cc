#include <string>
#include <set>
#include <iostream>
#include <iterator>
#include <map>
#include <utility>
#include <vector>

int main(int lb, char** par)
{
    if (lb < 2) 
    {
	std::cout << "string argument required" << std::endl;
	return 1;
    }
    std::string s = par[1];

    std::cout << "input sting: " << s << std::endl;

    std::set<char> st;
    std::map<int, char> m;

    for (std::string::iterator it = s.begin(); it != s.end(); ++it)
    {
	st.insert( *it );
    }

    int i = 0;
    for (std::set<char>::iterator it=st.begin(); it != st.end(); ++it)
    {
	m.insert(std::make_pair(i++, *it));
    }

    std::cout << "number of distnct chars: " << st.size() << std::endl;

    std::ostream_iterator<char> chr_cout(std::cout, ", ");
    std::copy(st.begin(), st.end(), chr_cout);
    std::cout << std::endl;

    int n = m.size();

    char c;
    std::vector<char> v;

    for (i=0;i<n;i++) 
    {
	v.push_back( m[i] );
    }

    std::sort(v.begin(), v.end());
    std::cout<< " permutations: " << std::endl;
    i = 0;

    do
    {
	std::copy(v.begin(), v.end(), chr_cout);
	std::cout << std::endl;
	i ++;
    }
    while (next_permutation(v.begin(), v.end()));

    std::cout << "permutations: " << i << std::endl;

    return 0;
}

