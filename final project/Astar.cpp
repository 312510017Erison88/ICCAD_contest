#include <queue>
#include <algorithm> // Include this header for std::reverse

#include "Astar.h"


using namespace std;

const int INF = 10000000;
const int dx[4] = {1, -1, 0, 0}; // Directions: up, down, left, right
const int dy[4] = {0, 0, 1, -1};

bool isValid(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}


// ray-casting algorithm
bool isPointInsideBlock(const Point_2& pt, const Block& block) {
    // Check if the point is within the bounding box of the polygon
    int min_x = block.vertices[0].x, max_x = block.vertices[0].x;
    int min_y = block.vertices[0].y, max_y = block.vertices[0].y;
    for (const auto& vertex : block.vertices) {
        min_x = min(min_x, vertex.x);
        max_x = max(max_x, vertex.x);
        min_y = min(min_y, vertex.y);
        max_y = max(max_y, vertex.y);
    }
    if (pt.x < min_x || pt.x > max_x || pt.y < min_y || pt.y > max_y) {
        return false;
    }
    
    int n = block.vertices.size();  // Number of vertices in the polygon
    int j = n - 1;                  // Index of the last vertex
    bool inside = false;
    // Check if the point is between the y-coordinates of the current and previous vertices
    for (int i = 0; i < n; i++) {
        if ((block.vertices[i].y > pt.y) != (block.vertices[j].y > pt.y) &&
        // Check if the point is to the left of the line segment from vertices[j] to vertices[i]
            pt.x < (block.vertices[j].x - block.vertices[i].x) * (pt.y - block.vertices[i].y) / (block.vertices[j].y - block.vertices[i].y) + block.vertices[i].x) {
            inside = !inside;
        }
        j = i;
    }
    return inside;
}

void preprocessBlockAreas(unordered_set<Point_2>& nonTraversableSet, const vector<Block>& blockList, int ROW, int COL) {
    for (const auto& block : blockList) {
        for (int x = 0; x < ROW; ++x) {
            for (int y = 0; y < COL; ++y) {
                Point_2 pt{x, y};
                if (isPointInsideBlock(pt, block) && (!block.is_feedthroughable || 
                    (block.through_block_edge_net_num.size() > 0 /*&& find(block.through_block_edge_net_num.begin(), block.through_block_edge_net_num.end(), net.ID) == block.through_block_edge_net_num.end()*/))) {
                    nonTraversableSet.insert(pt);
                }
            }
        }
    }
}

bool canMove(const Point_2& to, const unordered_set<Point_2>& nonTraversableSet) {
    return nonTraversableSet.find(to) == nonTraversableSet.end();
}

int heuristic(const Point_2& a, const Point_2& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

vector<Point_2> AStar(Point_2 start, Point_2 goal, const vector<Block>& blockList, const Net& net, int ROW, int COL) {
    unordered_set<Point_2> nonTraversableSet;
    preprocessBlockAreas(nonTraversableSet, blockList, ROW, COL);

    priority_queue<Cell> openSet;
    unordered_map<Point_2, int> gScore, fScore;
    unordered_map<Point_2, Point_2> parent;

    openSet.push(Cell{start.x, start.y, 0});
    gScore[start] = 0;
    fScore[start] = heuristic(start, goal);
    parent[start] = Point_2{-1, -1};

    while (!openSet.empty()) {
        Cell current = openSet.top();
        openSet.pop();
        Point_2 currentPoint{current.x, current.y};

        if (currentPoint == goal) break;

        for (int i = 0; i < 4; i++) {
            Point_2 next = {current.x + dx[i], current.y + dy[i]};
            
            if (isValid(next.x, next.y, ROW, COL) && canMove(next, nonTraversableSet)) {
                int tentative_gScore = gScore[currentPoint] + 1;
                if (gScore.find(next) == gScore.end() || tentative_gScore < gScore[next]) {
                    parent[next] = currentPoint;
                    gScore[next] = tentative_gScore;
                    fScore[next] = gScore[next] + heuristic(next, goal);
                    openSet.push(Cell{next.x, next.y, fScore[next]});
                }
            }
        }
    }

    return backtrack(start, goal, parent);
}

vector<Point_2> backtrack(Point_2 start, Point_2 goal, const unordered_map<Point_2, Point_2>& parent) {
    vector<Point_2> path;
    for (Point_2 at = goal; at.x != -1; at = parent.at(at)) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

void printPath(const vector<Point_2>& path, ofstream& file) {
    for (const auto& point : path) {
        file << "(" << point.x << ", " << point.y << ") ";
    }
    file << endl;
}