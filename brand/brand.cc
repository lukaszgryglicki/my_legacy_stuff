#include <boost/random/random_device.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <ctime>

int main() 
{
    std::string chars(
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890"
        "!@#$%^&*()"
        "`~-_=+[{]{\\|;:'\",<.>/? ");
    boost::random::random_device rng;
    boost::random::mt19937 gen(std::time(0));
    boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1), plen(8, 32);
    int llen = plen(gen);

    for(int i = 0; i < llen; ++i) 
    {
        std::cout << chars[index_dist(rng)];
    }
    std::cout << std::endl;
}
