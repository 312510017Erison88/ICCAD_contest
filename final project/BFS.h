#ifndef BFS_H_
#define BFS_H_

#include <iostream>
#include <vector>
#include <fstream>
#include <map>
#include <unordered_map>
#include "readfile.h"

using namespace std;

struct Point_2 {
    int x, y;
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




bool isValid(int x, int y, int rows, int cols);
// vector<Point_2> BFS(Point_2 start, Point_2 goal, vector<vector<int>> grid);
// vector<Point_2> backtrack(Point_2 start, Point_2 goal);

////////////////////////////////////////////////////////////////
bool isPointInsideBlock(const Point_2& pt, const Block& block);
bool canMove(const Point_2& from, const Point_2& to, const vector<Block>& blockList, const Net& net);
// vector<Point_2> BFS(Point_2 start, Point_2 goal, const vector<Block>& blockList, const Net& net);
vector<Point_2> BFS(Point_2 start, Point_2 goal, const vector<Block>& blockList, const Net& net, int ROW, int COL);
vector<Point_2> backtrack(Point_2 start, Point_2 goal, const map<Point_2, Point_2>& parent);
void printPath(const vector<Point_2>& path, ofstream& file);


#endif