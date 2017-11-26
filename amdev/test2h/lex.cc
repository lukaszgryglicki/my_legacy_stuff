#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

class RunStrCmp
{
    public:
	enum cmp_mode { normal, nocase };

    private:
	const cmp_mode mode;

	static bool nocase_cmp(char c1, char c2)
	{
	    return toupper(c1) < toupper(c2);
	}

    public:
	RunStrCmp( cmp_mode m = normal ) : mode(m) {
	}

	bool operator(const string& s1, const string& s2) const
	{
	    if (mode == normal) 
	    {
		return s1 < s2;
	    }
	    else
	    {
		return lexicographical_compare( s1.begin(), s1.end(), s2.begin(), s2.end(), nocase_cmp);
	    }
	}
};

typedef map< string, string, RunStrCmp > SSMap;

void fillPrint(SSMap& m);

int main()
{
    SSMap m1;

    fillPrint( m1 );

    RunStrCmp icase( RunStrCmp :: nocase );

    SSMap m2( icase );

    fillPrint( m2 );
}

void fillPrint(SSMap& m)
{
    m[ "Deutschland" ] = "Germany";
    m[ "deutsch" ] = "German";
    m[ "Haken" ] = "snag";
    m[ "arbeiten" ] = "work";
    m[ "Hund" ] = "dog";
    m[ "gehen" ] = "go";
    m[ "Unternehmen" ] = "enterprise";
    m[ "unternehmen" ] = "undertake";
    m[ "gehen" ] = "walk";
    m[ "Bestatter" ] = "undertaker";

    SSMap::iterator pos;

    cout.setf(ios::left, ios::adjustfield);

    for (pos = m.begin(); pos != m.end(); ++pos)
    {
	cout << setw(15) << pos->first.c_str() << " " << pos->second << endl;
    }

    cout << endl;
}

