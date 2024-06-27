#ifndef READFILE_H_
#define READFILE_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

// struct Point {
//     int x, y;
//     bool operator==(const Point& other) const {
//         return x == other.x && y == other.y;
//     }
//      // Define comparison operators for use in std::map
//     bool operator<(const Point& other) const {
//         return tie(x, y) < tie(other.x, other.y);
//     }
// };

struct Point {
    int x, y;
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
     // Define comparison operators for use in std::map
    bool operator<(const Point& other) const {
        return tie(x, y) < tie(other.x, other.y);
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

struct Block {
    string block_name;
    int through_block_net_num;      // max num that net can feedthrough
    vector<int> through_block_edge_net_num;     
    vector<int> block_port_region;     // ä»»ï¿½??Netï¿??ï¿???????????blockï¿??å°±ï¿½??ï¿??ï¿??ï¿??block_port_regionï¿?????, ï¿???????ï¿½ç©ºå°±don't care
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
    map<string, vector<int>> MUST_THROUGH;      // this net must through
    map<string, vector<int>> HMFT_MUST_THROUGH;     // must go through non-feedthrough block
    vector<float> TX_COORD;                 // corordinate that starting block shift
    vector<vector<float>> RX_COORD;          // corordinate that target block shift
};

struct Component {
    string name;
    string blocktype;
    int x;
    int y;
    string orientation;
};

struct Region {
    string name;
    vector<Point> vertices;
};

struct DieArea {
    int x1;
    int y1;
    int x2;
    int y2;
};


struct OnlyBlock {
    string name;
    vector<Point> vertices;
    int width;
    int height;
};




void readJsonFiles(const string& blockFilePath, const string& netFilePath, vector<Block>& block, vector<Net>& nets);
void readDefFile(const std::string& defFilePath, std::vector<Component>& components, std::vector<Region>& regions, int& num_Comp, int& UNITS_DISTANCE_MICRONS, DieArea& diearea);
void readCompFile(const std::string& compFilePath, std::vector<OnlyBlock>& onlyblocks);

void completeRectangle(OnlyBlock& block);
void WidthHeight(OnlyBlock& onlyb);
Point rotatePoint(const Point& pt, int angle, const int& width, const int& height);
Point reflectPoint(const Point& pt, bool isYAxis, const int& width, const int& height, bool haveRotate);
vector<Point> transformVertices(const vector<Point>& vertices, const int& width, const int& height, const Point& origin, const string& orientation);
void updateBlocksWithVertices(vector<Block>& blocks, const vector<Component>& components, const vector<OnlyBlock>& onlyblocks);

#endif