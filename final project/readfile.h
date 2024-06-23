#ifndef READFILE_H_
#define READFILE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct Point {
    float x;
    float y;
};

struct Block {
    string block_name;
    int through_block_net_num;      // max num that net can feedthrough
    vector<int> through_block_edge_net_num;     
    vector<float> block_port_region;     
    bool is_feedthroughable;        // this block feedthrough or not
    bool is_tile;                   // don't care
    Point position;
    vector<Point> vertices;

};

struct Net {
    int ID;
    string TX;      // starting block
    vector<string> RX;      // target block
    int NUM;                // how much tracks that this net will use
    map<string, vector<float>> MUST_THROUGH;      // this net must through
    map<string, vector<float>> HMFT_MUST_THROUGH;     // must go through non-feedthrough block
    vector<float> TX_COORD;                 // corordinate that starting block shift
    vector<vector<float>> RX_COORD;          // corordinate that target block shift
};

struct Component {
    string name;
    string blocktype;
    float x;
    float y;
    string orientation;
};

struct Region {
    string name;
    vector<Point> vertices;
};

struct DieArea {
    float x1;
    float y1;
    float x2;
    float y2;
};


struct OnlyBlock {
    string name;
    vector<Point> vertices;
    float width;
    float height;
};




void readJsonFiles(const string& blockFilePath, const string& netFilePath, vector<Block>& blocks, vector<Net>& nets);
void readDefFile(const std::string& defFilePath, std::vector<Component>& components, std::vector<Region>& regions, int& num_Comp, float& UNITS_DISTANCE_MICRONS, DieArea& diearea);
void readCompFile(const std::string& compFilePath, std::vector<OnlyBlock>& onlyblocks);

void completeRectangle(OnlyBlock& block);
void WidthHeight(OnlyBlock& onlyb);
Point rotatePoint(const Point& pt, int angle, const float& width, const float& height);
Point reflectPoint(const Point& pt, bool isYAxis, const float& width, const float& height, bool haveRotate);
vector<Point> transformVertices(const vector<Point>& vertices, const float& width, const float& height, const Point& origin, const string& orientation);
void updateBlocksWithVertices(vector<Block>& blocks, const vector<Component>& components, const vector<OnlyBlock>& onlyblocks);

#endif