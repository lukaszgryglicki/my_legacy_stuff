#include <iostream>
#include <stack>
#include <sstream>
#include <exception>
#include <vector>
#include <fstream>
#include <iterator>
#include <utility>
#include <queue>
#include <ctime>
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

class cell
{
    public:
	cell(unsigned short i = 0, unsigned short j = 0)
        {
	    x = i;
	    y = j;
        }

	bool operator==(const cell& c) const
	{
	    return x == c.x && y == c.y;
	}

	bool operator<(const cell& c) const
	{
	    return true;
	}

	friend std::ostream& operator<<(std::ostream&, const cell&);

    private:
	unsigned short x, y;
	friend class maze;
	friend class distance_exit;
};

std::ostream& operator<<(std::ostream& out, const cell& c)
{
    out << "(" << c.x << ", " << c.y << ")";
    return out;
}

class distance_exit
{
    public:
	distance_exit(const cell& c, const cell& e) : exit_cell(e), curr_cell(c) {}
	bool operator<(const distance_exit& c) const;
	cell get_cell() const;

    private:

	float get_distance() const;
	cell exit_cell, curr_cell;
};

bool distance_exit::operator<(const distance_exit& d) const
{
    return get_distance() < d.get_distance();
}

cell distance_exit::get_cell() const
{
    return curr_cell;
}

float distance_exit::get_distance() const
{
    float d;
    d = (exit_cell.x - curr_cell.x) * (exit_cell.x - curr_cell.x) + (exit_cell.y - curr_cell.y) * (exit_cell.y - curr_cell.y);
    //std::cout << "distance: " << d << std::endl;
    return d;
}


class maze
{
    public:
	maze();
	maze(const char, const char, const char, const char, const char);
	maze(const std::vector<char>&);
	void construct(std::ifstream&);
	void construct(std::vector<std::string>&);
	void exit_maze();
	void generate_maze();
	//maze(const maze&);
	//maze operator=(const maze&);

    private:
	cell curr_cell, exit_cell, entry_cell;
	char exit_marker, entry_marker, visited, passage, wall;
	std::stack<cell> maze_stack;
	std::vector< std::vector< char > > store;
	unsigned short rows, cols;

	bool push_unvisited(unsigned short, unsigned short);
	void print_maze() const;
	//friend std::ostream& operator<<(std::ostream&, const maze&);
};

/*
maze maze::operator=(const maze& _m)
{
    maze m;
    std::cout << "maze operator=" << std::endl;
    return m;
}

maze :: maze(const maze& m)
{
    std::cout << "maze copy constructor" << std::endl;
}
*/

maze :: maze() : exit_marker('e'), entry_marker('b'), visited('.'), passage(' '), wall('#') 
{
    //std::cout<< "construct default maze" << std::endl;
}

maze :: maze(const char _ex, const char _en, const char _v, const char _p, const char _w) : exit_marker(_ex), entry_marker(_en), visited(_v), passage(_p), wall(_w) 
{
}

maze :: maze(const std::vector<char>& _v)
{
    //std::cout<< "construct maze from vector" << std::endl;
    if (_v.size() != 5) 
    {
	std::stringstream se;
	se << "cannot construct maze letters from vector of size: " << _v.size() << ", size must at least 5\n";
	for (std::vector<char>::const_iterator it=_v.begin(); it!=_v.end(); ++it)
	{
	    se << *it << " ";
	}
	stream_exc::throw_exc( &se );
    }

    exit_marker = _v[0];
    entry_marker = _v[1];
    visited = _v[2];
    if (_v[3] == '0') passage = ' ';
    else passage = _v[3];
    wall = _v[4];
}

bool maze::push_unvisited(unsigned short r, unsigned short c)
{
    //std::cout << "(" << r << ", " << c << ") " << store[r][c] << std::endl;
    if (store[r][c] == passage || store[r][c] == exit_marker)
    {
	maze_stack.push(cell(r, c));
	//std::cout<< "pushed\n";
	return true;
    }
    else 
    {
	//std::cout<<"cannot go there\n";
	return false;
    }
}

/*
std::ostream& operator<<(std::ostream& out, const maze& m)
{
    for (int i=0;i<m.rows;i++)
    {
	out << m.store[i] << std::endl;
    }
    out << std::endl;

    return out;
}
*/
void maze::print_maze() const
{
    for (int i=0;i<rows;i++)
    {
	for (int j=0;j<cols;j++) std::cout << store[i][j];
	std::cout << std::endl;
    }
    std::cout << std::endl;
}

void maze::exit_maze()
{
    unsigned short r, c;
    int steps = 0;
    cell ce;

    //std::cout<<"exit maze algorithm" << std::endl;
    curr_cell = entry_cell;

    while (!(curr_cell == exit_cell))
    {
	r = curr_cell.x;
	c = curr_cell.y;

	//std::cout << "single step " << curr_cell << std::endl;
	//print_maze();

	if (!(curr_cell == entry_cell)) store[r][c] = visited;

	std::priority_queue<distance_exit> q;

	q.push( distance_exit(cell(r-1, c), exit_cell) );
	q.push( distance_exit(cell(r+1, c), exit_cell) );
	q.push( distance_exit(cell(r, c-1), exit_cell) );
	q.push( distance_exit(cell(r, c+1), exit_cell) );

	while (!q.empty())
	{
	    ce = q.top().get_cell();
	    if (push_unvisited( ce.x, ce.y )) steps ++;
	    q.pop();
	}

	//push_unvisited(r-1, c);
	//push_unvisited(r+1, c);
	//push_unvisited(r, c-1);
	//push_unvisited(r, c+1);


	if (maze_stack.empty())
	{
	    //std::cout << *this << ", cannot exit" << std::endl;
	    print_maze();
	    std::cout << "failure after " << steps << " steps" << std::endl;
	    return;
	}
	else
	{
	    curr_cell = maze_stack.top();
	    maze_stack.pop();
	}
    }

    //std::cout<< *this << ", success" << std::endl;
    print_maze();
    std::cout<<"success after " << steps << " steps" << std::endl;
}

void maze::generate_maze()
{
    time_t t;
    unsigned short rr, cc, diff;

    time( &t );
    srand( t );

    std::vector<std::string> ss;
    std::string str;

    rr = 10 + rand() % 11;
    cc = 10 + rand() % 11;

    for (int i=0;i<rr;i++)
    {
	diff = 200 + rand() % 200;
	str.clear();
	for (int j=0;j<cc;j++)
	{
	    t = rand() % 1000;
	    if (t <= diff) str += "#";
	    else str += " ";
	}

	ss.push_back( str );
    }

    unsigned short px, py;

    px = rand() % rr/4;
    py = rand() % cc/4;
    ss[px][py] = 'b';

    px = (3*rr)/4 + rand() % rr/4;
    py = (3*cc)/4 + rand() % cc/4;
    ss[px][py] = 'e';

    construct( ss );
}

void maze :: construct(std::vector<std::string>& ss)
{
    //std::cout << "letters: " << exit_marker << entry_marker << visited << passage << wall << std::endl;
    //
    unsigned int l = 0, n = ss.size();
    for (std::vector<std::string>::iterator it=ss.begin(); it!=ss.end(); ++it)
    {
	if (it->size() > l) l = it->size();
    }

    bool bpad = false;
    bool epad = false;

    unsigned int l2 = l;
    for (unsigned int i=0;i<n;i++)
    {
	if(!bpad && ss[i].at(0) != '#') 
	{
	    bpad = true;
	    l2 ++;
	    //std::cout<<"begin padding at line: " << i << std::endl;
	}
	if (ss[i].size() == l)
	{
	    //std::cout<<"need to check line " << i << ", because it has length: " << l << std::endl;
	    if (!epad && ss[i].at(l-1) != '#')
	    {
		epad = true;
		l2 ++;
		//std::cout<<"end padding at line: " << i << std::endl;
	    }
	}
	//std::cout<< i << ": length: " << ss[i].size() << std::endl;
    }

    std::string spad;

    spad.resize( l2 );
    std::fill(spad.begin(), spad.end(), '#');
    //std::cout<< "padding string, length: " << l2 << ", '" << spad << "'" << std::endl;

    unsigned int n2 = n;
    std::vector<std::string> ss2;
    std::string str;

    std::cout << "final maze:" << std::endl;
    for (unsigned int i=0;i<n;i++)
    {
	if (!i && ss[i] != spad) 
	{
	    ss2.push_back( spad );
	    std::cout<< spad << std::endl;
	    n2 ++;
	}

	str.clear();
	if (bpad) str = "#";
	str += ss[i];
	if (ss[i].size() < l)
	{
	    for (unsigned int x=ss[i].size();x<l;x++) str += wall;
	}
	if (epad) str += "#";
	ss2.push_back( str );

	std::cout<< str << std::endl;

	if (i == n - 1 && ss[i] != spad)
	{
	    ss2.push_back( spad );
	    std::cout<< spad << std::endl;
	    n2 ++;
	}
    }

    rows = 0;
    cols = l2;

    ss.clear();
    std::vector<char> vrow;
    unsigned int ic;

    for (std::vector<std::string>::iterator r=ss2.begin(); r!=ss2.end();++r)
    {
	vrow.clear();
	ic = 0;
	for (std::string::iterator c=r->begin(); c!=r->end(); ++c)
	{
	    vrow.push_back( *c );

	    if (*c == exit_marker)
	    {
		exit_cell.x = rows;
		exit_cell.y = ic;
	    }
	    else if (*c == entry_marker)
	    {
		entry_cell.x = rows;
		entry_cell.y = ic;
	    }
	    ic ++;
	}
	store.push_back( vrow );
	rows ++;
    }

    ss2.clear();

    std::cout << "start at: " << entry_cell << std::endl;
    std::cout << "end at:   " << exit_cell << std::endl;
}

void maze :: construct(std::ifstream& s)
{
    std::string str;
    std::vector<std::string> ss;

    while (getline(s, str))
    {
	ss.push_back( str );
    }
    s.close();

    construct( ss );
}

int main(int lb, char** par)
{
    try
    {
	boost::shared_ptr<maze> mp;

	if (lb < 2)
	{
	    //std::stringstream se;
	    //se << "need 1 or 2 file args arg, got args: " << lb - 1;
	    //stream_exc::throw_exc( &se );
	    mp = boost::shared_ptr<maze>(new maze);
	    mp->generate_maze();
	    std::cout << "try to exit" << std::endl;
	    mp->exit_maze();
	    return 0;
	}


	if (lb > 2)
	{
	    std::ifstream ifsl(par[1], std::ios::in);
	    if (!ifsl)
	    {
		std::cout << "letters file: " << par[1] << " cannot be open, using defaults" << std::endl;
		mp = boost::shared_ptr<maze>(new maze);
	    }
	    else
	    {
		std::vector<char> v;
		std::copy(std::istream_iterator<char>(ifsl), std::istream_iterator<char>(), std::back_inserter(v));
    	
		std::ostream_iterator<char> chr_cout(std::cout, ", ");
        	std::cout << v.size() << " letters from " << par[1] << ": ";
		std::copy(v.begin(), v.end(), chr_cout);
		std::cout << std::endl;

		if (v.size() > 5) v.resize(5);

		mp = boost::shared_ptr<maze>(new maze(v));
	    }
	}
	else 
	{
	    std::cout << "no letters file given, using defaults" << std::endl;
	    mp = boost::shared_ptr<maze>(new maze);
	}

	int p = lb > 2 ? 2 : 1;
	std::ifstream ifs(par[p], std::ios::in);
	if (!ifs)
	{
	    std::stringstream se;
	    se << par[p] << " cannot be open";
	    stream_exc::throw_exc( &se );
	}
	mp->construct( ifs );

	std::cout << "try to exit" << std::endl;
	mp->exit_maze();
    }
    catch (boost::shared_ptr<stream_exc> e)
    {
	std::cout << e->out();
	// no need to free pointer - it's a shared boost pointer :)
	return 1;
    }
    catch (std::exception e)
    {
	std::cout<< "standard exception: " << e.what() << std::endl;
	return 2;
    }
    catch (...)
    {
	std::cout << "unhandled unknown exception" << std::endl;
	return 3;
    }

}

