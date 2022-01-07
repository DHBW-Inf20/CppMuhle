/* 
Grüße gehen Raus an Torsten 
https://github.com/HopfTorsten/dhbw-cpp-journey/blob/master/day3/Task1/Split.hpp
*/
#ifndef UTILIY_HPP
#define UTILIY_HPP

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
template <class Container>
void split(const std::string& str, Container& cont, char delim = ',')
{
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delim)) {
        cont.push_back(token);
    }
}




#endif