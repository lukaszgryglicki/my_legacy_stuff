#include <set>
#include <iostream>

using namespace std;

int main()
{
    set<int> s;
    pair<set<int>::iterator, bool> sts;

    s.insert(5);
    s.insert(1);
    s.insert(8);

    for (int i=0;i<=10;i++)
    {
	sts = s.insert(i);

	if (sts.second)
	{
	    cout << i << " inserted as element ";
	}
	else
	{
	    cout << i << " already exists as element ";
	}

	cout << distance(s.begin(), sts.first) + 1 << endl;
    }
}

