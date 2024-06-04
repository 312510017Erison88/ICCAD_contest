#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> // Include this header for std::reverse
#include "BFS.h"

using namespace std;

const int INF = 10000000;
const int dx[4] = {1, -1, 0, 0}; // Directions: up, down, left, right
const int dy[4] = {0, 0, 1, -1};


// vector<vector<int>> grid;
vector<vector<int>> dist;
vector<vector<Point_2>> parent;

bool isValid(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}


vector<Point_2> BFS(Point_2 start, Point_2 goal, vector<vector<int>> grid) {
    // initailization
    int rows = grid.size();
    int cols = grid[0].size();
    dist = vector<vector<int>>(rows, vector<int>(cols, INF));
    parent = vector<vector<Point_2>>(rows, vector<Point_2>(cols, Point_2(-1, -1)));
    
    priority_queue<Cell> pq;
    pq.push(Cell(start.x, start.y, 0));
    dist[start.x][start.y] = 0;

    while (!pq.empty()) {
        Cell current = pq.top();
        pq.pop();
        
        if (current.x == goal.x && current.y == goal.y) break;
        
        for (int i = 0; i < 4; i++) {       // four possibility to go
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];
            
            // dist[current.x][current.y] + 1: new move distance
            // dist[][] is original diatance
            if (isValid(nx, ny, rows, cols) && grid[ny][nx] == 0 && dist[nx][ny] > dist[current.x][current.y] + 1) {
                dist[nx][ny] = dist[current.x][current.y] + 1;
                parent[nx][ny] = Point_2(current.x, current.y);
                pq.push(Cell(nx, ny, dist[nx][ny]));
            }
        }
    }

    return backtrack(start, goal);
}

vector<Point_2> backtrack(Point_2 start, Point_2 goal) {
    vector<Point_2> path;

    for (Point_2 at = goal; at.x != -1; at = parent[at.x][at.y]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}


