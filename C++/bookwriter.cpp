#include <fstream>
#include <iostream>
#include <vector>

int main() {
    // Define the vector
    std::vector<std::vector<std::vector<std::vector<int>>>> myvector = 
    {{{{30,10,20,50,40,21,11,31},{1,2,3,4,5,6,7,8},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},{-1,-2,-3,-4,-5,-6,-7,-8},{-30,-10,-20,-50,-40,-21,-11,-31}},{{4,1,3,3,3}}},
    {{{30,10,20,50,40,21,11,31},{1,2,3,0,5,6,7,8},{0,0,0,0,0,0,0,0},{0,0,0,4,0,0,0,0},{0,0,0,-4,0,0,0,0},
    {0,0,0,0,0,0,0,0},{-1,-2,-3,0,-5,-6,-7,-8},{-30,-10,-20,-50,-40,-21,-11,-31}},{{10,0,1,2,2}}},
    {{{30,10,20,50,40,21,11,31},{1,2,3,0,5,6,7,8},{0,0,0,0,0,0,0,0},{0,0,0,4,0,0,0,0},{0,0,0,0,0,-6,0,0},
    {0,0,0,0,0,0,0,0},{-1,-2,-3,-4,-5,0,-7,-8},{-30,-10,-20,-50,-40,-21,-11,-31}},{{10,0,1,2,2}}},
    {{{30,10,20,50,40,21,11,31},{1,2,3,0,5,6,7,8},{0,0,0,0,0,0,0,0},{0,0,0,4,0,0,0,0},{0,0,0,0,0,0,0,0},
    {0,0,0,0,0,-6,0,0},{-1,-2,-3,-4,-5,0,-7,-8},{-30,-10,-20,-50,-40,-21,-11,-31}},{{5,1,4,3,4}}}};

    // Write the vector to a file
    std::ofstream outfile("openingbook.bin", std::ios::binary);
    int outer_size = myvector.size();
    outfile.write(reinterpret_cast<char*>(&outer_size), sizeof(outer_size));
    for (int i = 0; i < outer_size; i++) {
        int outer_inner_size = myvector[i].size();
        outfile.write(reinterpret_cast<char*>(&outer_inner_size), sizeof(outer_inner_size));
        for (int j = 0; j < outer_inner_size; j++) {
        int inner_inner_size = myvector[i][j].size();
        outfile.write(reinterpret_cast<char*>(&inner_inner_size), sizeof(inner_inner_size));
        for (int k = 0; k < inner_inner_size; k++) {
            int inner_most_size = myvector[i][j][k].size();
            outfile.write(reinterpret_cast<char*>(&inner_most_size), sizeof(inner_most_size));
            for (int l = 0; l < inner_most_size; l++) {
            int value = myvector[i][j][k][l];
            outfile.write(reinterpret_cast<char*>(&value), sizeof(value));
            }
        }
        }
    }
    outfile.close();

    return 0;
}