#ifndef _READFILE_H_
#define _READFILE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Block {
    string block_name;
    int through_block_net_num;
    vector<int> through_block_edge_net_num;
    vector<int> block_port_region;
    bool is_feedthroughable;
    bool is_tile;
};

struct Net {
    int ID;
    string TX;
    vector<string> RX;
    int NUM;
    map<string, vector<int>> MUST_THROUGH;
    map<string, vector<int>> HMFT_MUST_THROUGH;
    vector<float> TX_COORD;
    vector<vector<float>> RX_COORD;
};

struct Component {
    string name;
    string macro;
    int x;
    int y;
    string orientation;
};

struct Region {
    string name;
    int x1;
    int y1;
    int x2;
    int y2; 
};

struct DieArea {
    int x1;
    int y1;
    int x2;
    int y2;
};

struct Point {
    int x;
    int y;
};



void readJsonFiles(const string& blockFilePath, const string& netFilePath, vector<Block>& block, vector<Net>& nets);
void readDefFile(const std::string& defFilePath, std::vector<Component>& components, std::vector<Region>& regions, int& num_Comp, int& UNITS_DISTANCE_MICRONS, DieArea& diearea);
std::vector<Point> readCompFile(const std::string& compFilePath);

#endif