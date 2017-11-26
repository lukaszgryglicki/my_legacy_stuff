#include <fstream>
#include <iostream>
#include <string>
#include <set>
#include <queue>
#include <iomanip>
#include <sstream>
#include <exception>
#include <boost/smart_ptr.hpp>

class stream_exc : std::exception
{
    public:
    stream_exc() throw() {}

    stream_exc(std::stringstream* _ss)
    {
	std::string s1;
	while (*_ss >> s1)
	{
	    s += s1 + " ";
	}
	if (s.size() > 1) s.resize(s.size()-1);
    }

    stream_exc(const std::string& _s)
    {
	this->s = _s;
    }

    ~stream_exc() throw()
    {
    }

    stream_exc(const stream_exc& _se)
    {
	this->s = _se.s;
    }

    std::string out()
    {
	return std::string("exception: ") + this->s + std::string("\n");
    }

    static void throw_exc(std::stringstream* se)
    {
	throw boost::shared_ptr<stream_exc>( new stream_exc(se) );
    }

    static void throw_exc(std::string s)
    {
	throw boost::shared_ptr<stream_exc>( new stream_exc(s) );
    }

    private:
    std::string s;
};


class occ_string
{
    public:
	occ_string()
	{
	    n = 0;
	}

	void set(const std::string& _s, const unsigned long _n)
	{
	    if (_n <= 0) 
	    {
		std::stringstream se;
		se << "negative n value: " << _n;
		stream_exc::throw_exc( &se );
		//throw boost::shared_ptr<stream_exc>( new stream_exc(&se) );
	    }

	    else if (_n > max_occ) 
	    {
		std::stringstream se;
		se << "n more than " << max_occ << ": " << _n;
		stream_exc::throw_exc( &se );
		//throw boost::shared_ptr<stream_exc>(new stream_exc( &se ));
	    }

	    s = _s;
	    n = _n;
	}

	std::string gets() const
	{
	    return s;
	}

	unsigned int getn() const
	{
	    return n;
	}

    private:
	std::string s;
	unsigned int n;
	static const int max_occ = 10000;
};

bool operator<(const occ_string& l, const occ_string& r)
{
    if (l.getn() == r.getn()) return l.gets() > r.gets();
    else return (l.getn() < r.getn());
}

void fprint(std::stringstream& t)
{
    std::string ss;

    stream_exc::throw_exc( std::string( "this should'n be called!" ) );

    while (t >> ss)
    {
	std::cout << "F> " << ss << std::endl;
    }
}

int main(int lb, char** par)
{
    try
    {
    if (lb < 2)
    {
	std::cout << par[0] << ": filename" << std::endl;

	std::stringstream se;
	se << "need at least 1 args, got: " << lb - 1;
	stream_exc::throw_exc( &se );
    }

    std::string str;
    unsigned int N = 10;
    if (lb >= 3)
    {
	if (std::stringstream( str = std::string( par[2] ) ) >> N)
	{
	    std::cout << "Top " << N << " numbers" <<  std::endl;
	}
	else
	{
	    std::cout << str << ": is not a integer value" << std::endl;
	    std::stringstream se;
	    se << str << " is not a number";
	    stream_exc::throw_exc( &se );
	}
    }

    std::ifstream fin( par[1] );

    if (!fin)
    {
	std::cout << par[1] << ": cannot be read" << std::endl;
	std::stringstream se;
	se << "cannot read file: " << par[1];
	stream_exc::throw_exc( &se );
	return 2;
    }

    std::string s;
    std::multiset<std::string> ms;
    std::set<std::string> st;

    int nw = 0;
    while (!fin.eof())
    {
	fin >> s;
	ms.insert( s );
	st.insert( s );
	nw ++;
    }

    std::cout << "words: " << nw << std::endl;
    std::cout << "distinct words: " << st.size() << std::endl;

    std::pair<std::multiset<std::string>::iterator, std::multiset<std::string>::iterator> p;
    size_t d;
    occ_string ocs;
    std::priority_queue<occ_string> q;

    for (std::set<std::string>::iterator it=st.begin();it!=st.end();++it)
    {
	p = ms.equal_range( *it );
	d = distance( p.first, p.second );

	//std::cout << d << " --> " << *it << std::endl;

	ocs.set( *it, d);
	q.push( ocs );
    }

    //std::cout << std::endl;
    std::cout<< "qsize: " << q.size() << std::endl;

    int n = q.size() > N ? N : q.size();

    for (int i=0;i<n;i++)
    {
	ocs = q.top();
	q.pop();

	std::cout << std::setw( 10 );
	std::cout<< ocs.getn() << " times --> " << ocs.gets() << std::endl;
    }

    std::stringstream t;
    t << "a b c ddd eee " << 1 << " " << 3.1415 << " koniec";
    fprint(t);
    }
	
    catch (boost::shared_ptr<stream_exc> e)
    {
	std::cout << e->out();
	// no need to free pointer - it's a shared boost pointer :)
	return 1;
    }

    std::cout << "all ok, bye!" << std::endl;
    return 0;
}

