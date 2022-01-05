#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <bitset>

int main()
{
    int bspField =   0b000000110011101101110011;
    int lastPieceRight = 0b000000000000000000000001;
    int lastPieceWrong = 0b000010000000000000000000;
    #define CURRENTPIECE lastPieceRight
    std::vector<int> three = {};
    std::map<std::string, int> lookupMap = {{"a1", 0}, {"d1", 1}, {"g1", 2}, {"b2", 3}, {"d2", 4}, {"f2", 5}, {"c3", 6}, {"d3", 7}, {"e3", 8}, {"a4", 9}, {"b4", 10}, {"c4", 11}, {"e4", 12}, {"f4", 13}, {"g4", 14}, {"c5", 15}, {"d5", 16}, {"e5", 17}, {"b6", 18}, {"d6", 19}, {"f6", 20}, {"a7", 21}, {"d7", 22}, {"g7", 23}};
    std::vector<std::vector<std::string>> testCases = {
        {"a1", "d1", "g1"},
        {"b2", "d2", "f2"},
        {"c3", "d3", "e3"},
        {"a4", "b4", "c4"},
        {"e4", "f4", "g4"},
        {"c5", "d5", "e5"},
        {"b6", "d6", "f6"},
        {"a7", "d7", "g7"},
        {"a1", "a4", "a7"},
        {"b2", "b4", "b6"},
        {"c3", "c4", "c5"},
        {"d1", "d2", "d3"},
        {"d5", "d6", "d7"},
        {"e3", "e4", "e5"},
        {"f2", "f4", "f6"},
        {"g1", "g4", "g7"}};
    std::cout << "Beispiel Spielfeld: ";
    for (std::vector<std::string> outer : testCases)
    {
        for (std::string inner : outer)
        {
            std::cout << std::bitset<24>(bspField) << " ||  ";
        }
    }
    std::cout << "\n";
    int i = 0;
    int j = 0;
    int t = 0;
    std::string suffix = "";
    std::cout << "3er Kombinationen:   ";
    std::cout << "std::vector<int> matchinThreeList = {";
    for (std::vector<std::string> outer : testCases)
    {
        i = 0;
        for (std::string inner : outer)
        {
            switch (i)
            {
            case 0:
                t = (1 << lookupMap.at(inner));
                break;
            case 1:
            case 2:
                t |= (1 << lookupMap.at(inner));
                break;
            }
            i++;
        }
        // std::cout << "\n";
        std::cout << t << ",  ";
        // if( std::bitset<24>(t&bspField).count() == 3 ){
        //     three.push_back(t);
        // }
        j++;
    }
    std::cout << "\n";

        j = 0;
    std::cout << "\nMatching Three :        ";
    for(int t : three){
         std::cout << std::bitset<24>(t) << " ||  ";
     }
    std::cout << "\nCurrent Piece :          ";
    for(int t : three){
         std::cout << std::bitset<24>(CURRENTPIECE) << " ||  ";
     }
    std::cout << "\nLast :                            ";
     for(int t : three){
         std::cout << std::bitset<24>(CURRENTPIECE&t) << " ||  ";
         if((CURRENTPIECE& t)){
             std::cout << "\ntrue";
             break;
         }
     }
}
