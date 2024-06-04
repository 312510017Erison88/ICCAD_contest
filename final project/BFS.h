#ifndef _BFS_H_
#define _BFS_H_

#include <iostream>
#include <vector>

using namespace std;

struct Point_2 {
    int x, y;
    Point_2(int _x, int _y) : x(_x), y(_y) {}
};

struct Cell {
    int x, y, dist;
    Cell(int _x, int _y, int _dist) : x(_x), y(_y), dist(_dist) {}
    bool operator<(const Cell& other) const {
        return dist > other.dist; // Min-heap based on distance
    }
};

bool isValid(int x, int y, int rows, int cols);
vector<Point_2> BFS(Point_2 start, Point_2 goal, vector<vector<int>> grid);


#endif