#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> // Include this header for std::reverse

using namespace std;

const int INF = 10000000;
const int dx[4] = {1, -1, 0, 0}; // Directions: up, down, left, right
const int dy[4] = {0, 0, 1, -1};

struct Point {
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

struct Cell {
    int x, y, dist;
    Cell(int _x, int _y, int _dist) : x(_x), y(_y), dist(_dist) {}
    bool operator<(const Cell& other) const {
        return dist > other.dist; // Min-heap based on distance
    }
};

vector<vector<int>> grid;
vector<vector<int>> dist;
vector<vector<Point>> parent;

bool isValid(int x, int y, int rows, int cols) {
    return x >= 0 && x < cols && y >= 0 && y < rows;
}

void mikamiTabuchi(Point start, Point goal) {
    // initailization
    int rows = grid.size();
    int cols = grid[0].size();
    dist = vector<vector<int>>(rows, vector<int>(cols, INF));
    parent = vector<vector<Point>>(rows, vector<Point>(cols, Point(-1, -1)));
    
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
                parent[nx][ny] = Point(current.x, current.y);
                pq.push(Cell(nx, ny, dist[nx][ny]));
            }
        }
    }
}

vector<Point> backtrack(Point start, Point goal) {
    vector<Point> path;

    for (Point at = goal; at.x != -1; at = parent[at.x][at.y]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

int main() {
    // grid = vector<vector<int>>(rows, vector<int>(cols, 0));
    //  1: obstacles, 0: can pass
    grid = {
        {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
        {1, 1, 0, 0, 1, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 1, 1, 1, 0, 0},
        {1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
        {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}
    };
    // (1, 0) is downward, (0, 1) is go right 

    Point start(0, 0);
    Point goal(9, 9);

    mikamiTabuchi(start, goal);
    vector<Point> path = backtrack(start, goal);

    cout << "Path: ";
    for (auto p : path) {
        cout << "(" << p.x << "," << p.y << ") ";
    }
    cout << endl;

    return 0;
}

