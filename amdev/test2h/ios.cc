#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>

using namespace std;

int main()
{
    vector<string> c1;

    copy(istream_iterator<string>(cin), istream_iterator<string>(), back_inserter(c1));
    sort(c1.begin(), c1.end());
    unique_copy(c1.begin(), c1.end(), ostream_iterator<string>(cout, "\n"));
}

