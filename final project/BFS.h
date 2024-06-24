#ifndef BFS_H_
#define BFS_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <utility> // for std::pair
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "readfile.h"

using namespace std;

struct Point_2 {
    int x, y;
    bool operator==(const Point_2& other) const {
        return x == other.x && y == other.y;
    }
     // Define comparison operators for use in std::map
    bool operator<(const Point_2& other) const {
        return tie(x, y) < tie(other.x, other.y);
    }
};


struct Cell {
    int x, y, dist;
    // Cell(int _x, int _y, int _dist) : x(_x), y(_y), dist(_dist) {}
    // bool operator<(const Cell& other) const {
    //     return dist > other.dist; // Min-heap based on distance
    // }
};

// ??ªå??ç¾©hash??½æ??
struct pair_hash {
    template <class T1, class T2>
    size_t operator() (const pair<T1, T2>& p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

using Coordinate = pair<int, int>;
using BlockMap = unordered_map<int, Block>;
using EdgeMap = unordered_map<Coordinate, int>;


void populateEdgeAndBlockMaps(vector<Block> blockList, unordered_map<pair<int, int>, int, pair_hash> &edgeMap, unordered_map<int, Block> &blockMap);
bool isValid(int x, int y, int rows, int cols);
bool isPointInsideBlock(const Point_2& pt, const Block& block);
bool canMove(const Point_2& from, const Point_2& to, const vector<Block>& blockList, const Net& net);
vector<Point_2> BFS(Point_2 start, Point_2 goal, const unordered_map<pair<int, int>, int, pair_hash>& edgeMap, unordered_map<int, Block>& blockMap, const Net& net, int ROW, int COL);
// vector<Point_2> BFS(Point_2 start, Point_2 goal, const vector<Block>& blockList, const Net& net, int ROW, int COL);
vector<Point_2> backtrack(Point_2 start, Point_2 goal, const map<Point_2, Point_2>& parent);
void printPath(const vector<Point_2>& path, ofstream& file);


#endif