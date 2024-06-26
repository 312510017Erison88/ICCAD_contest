#ifndef GREEDY_H_
#define GREEDY_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <utility> // for std::pair
#include <algorithm>
#include <cmath>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include "readfile.h"

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
// 
//     bool operator!=(const Point& other) const {
//         return !(*this == other);
//     }
// };


struct Cell {
    int x, y, dist;
};


struct pair_hash {
    template <class T1, class T2>
    size_t operator() (const pair<T1, T2>& p) const noexcept {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

using Coordinate = pair<int, int>;
using BlockMap = unordered_map<int, Block>;
using EdgeMap = unordered_map<Coordinate, int, pair_hash>;
using RegionMap = unordered_map<int, Region>;

bool isValid(int x, int y, int rows, int cols);
bool canMove(const Point& from, const Point& to, const EdgeMap& edgeMap, BlockMap& blockMap, const Net& net);
int manhattanDistance(const Point& a, const Point& b);
vector<Point> Greedy(Point start, Point goal, const EdgeMap& edgeMap, BlockMap& blockMap, const Net& net, int ROW, int COL);
void printPath(const vector<Point>& path, ofstream& file);
void populateEdgeAndBlockMaps(const vector<Block>& blockList, EdgeMap& edgeMap, BlockMap& blockMap);
void populateRegionMaps(vector<Region> regions, RegionMap& regionMap);
Point convertPoint(const Point &point, const vector<float> &coord);
Point getReferencePoint(const string &identifier, BlockMap& blockMap, RegionMap& regionMap);


#endif