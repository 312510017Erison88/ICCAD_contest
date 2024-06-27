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

struct Point_2 {
    int x, y;
    bool operator==(const Point_2& other) const {
        return x == other.x && y == other.y;
    }
     // Define comparison operators for use in std::map
    bool operator<(const Point_2& other) const {
        return tie(x, y) < tie(other.x, other.y);
    }

    bool operator!=(const Point_2& other) const {
        return !(*this == other);
    }
};


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

bool isValid(int x, int y, int rows, int cols);
bool canMove(const Point_2& from, const Point_2& to, const EdgeMap& edgeMap, BlockMap& blockMap, const Net& net);
int manhattanDistance(const Point_2& a, const Point_2& b);
vector<Point_2> Greedy(Point_2 start, Point_2 goal, const EdgeMap& edgeMap, BlockMap& blockMap, const Net& net, int ROW, int COL);
void printPath(const vector<Point_2>& path, ofstream& file);
void populateEdgeAndBlockMaps(const vector<Block>& blockList, EdgeMap& edgeMap, BlockMap& blockMap);


#endif