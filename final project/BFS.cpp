#include <queue>
#include <algorithm> // Include this header for std::reverse

#include "BFS.h"


using namespace std;

const int INF = 10000000;
const int dx[4] = {1, -1, 0, 0}; // Directions: up, down, left, right
const int dy[4] = {0, 0, 1, -1};


void populateEdgeAndBlockMaps(vector<Block> blockList, unordered_map<pair<int, int>, int, pair_hash> &edgeMap, unordered_map<int, Block> &blockMap) {
    // Populate edgeMap and blockMap with your data
    // This is just an example; you'll need to adapt this to your actual data
    for (size_t i = 0; i < blockList.size(); ++i) {
        const Block& block = blockList[i];
        for (const auto& vertex : block.vertices) {
            edgeMap[{vertex.x, vertex.y}] = i;
        }
        blockMap[i] = block;
    }
}

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

// bool canMove(const Point_2& from, const Point_2& to, const vector<Block>& blockList, const Net& net) {
//     for (const auto& block : blockList) {
//         if (isPointInsideBlock(to, block)) {
//             if (!block.is_feedthroughable || (block.through_block_edge_net_num.size() > 0 
//                 // checks if the current net's ID (net.ID) is not in the list of allowed nets for this block edge 
//                 //&&find(block.through_block_edge_net_num.begin(), block.through_block_edge_net_num.end(), net.ID) == block.through_block_edge_net_num.end()
//             )){
//                 return false;
//             }
//         }
//     }
//     return true;
// }

bool canMove(const Point_2& from, const Point_2& to, const unordered_map<pair<int, int>, int, pair_hash>& edgeMap, unordered_map<int, Block>& blockMap, const Net& net) {
    if (edgeMap.find({to.x, to.y}) != edgeMap.end()) {
        int blockId = edgeMap.at({to.x, to.y});
        const Block& block = blockMap.at(blockId);
        if (!block.is_feedthroughable || block.through_block_net_num <= 0) {
            return false;
        }
    }
    return true;
}


vector<Point_2> BFS(Point_2 start, Point_2 goal, const unordered_map<pair<int, int>, int, pair_hash>& edgeMap, unordered_map<int, Block>& blockMap, const Net& net, int ROW, int COL) {
    queue<Cell> q;
    map<Point_2, int> dist;
    map<Point_2, Point_2> parent;

    q.push(Cell{start.x, start.y, 0});
    dist[start] = 0;
    parent[start] = Point_2{-1, -1};

    while (!q.empty()) {
        Cell current = q.front();
        q.pop();

        if (current.x == goal.x && current.y == goal.y) break;

        for (int i = 0; i < 4; i++) {
            Point_2 next = {current.x + dx[i], current.y + dy[i]};

            if (isValid(next.x, next.y, ROW, COL) && canMove(Point_2{current.x, current.y}, next, edgeMap, blockMap, net) && dist.find(next) == dist.end()) {
                dist[next] = dist[Point_2{current.x, current.y}] + 1;
                parent[next] = Point_2{current.x, current.y};
                q.push(Cell{next.x, next.y, dist[next]});

                if (edgeMap.find({next.x, next.y}) != edgeMap.end()) {
                    int blockId = edgeMap.at({next.x, next.y});
                    blockMap[blockId].through_block_net_num -= 1;
                    cout << "Updating thr_block_net_num at " << blockId << " block!" << endl;
                }
            }
        }
    }

    return backtrack(start, goal, parent);
}

// vector<Point_2> BFS(Point_2 start, Point_2 goal, const vector<Block>& blockList, const Net& net, int ROW, int COL) {
//     queue<Cell> q;
//     map<Point_2, int> dist;
//     map<Point_2, Point_2> parent;

//     q.push(Cell{start.x, start.y, 0});
//     dist[start] = 0;
//     parent[start] = Point_2{-1, -1};

//     while (!q.empty()) {
//         Cell current = q.front();
//         q.pop();
        
//         if (current.x == goal.x && current.y == goal.y) break;
        
//         for (int i = 0; i < 4; i++) {
//             Point_2 next = {current.x + dx[i], current.y + dy[i]};
            
//             if (isValid(next.x, next.y, ROW, COL) && canMove(Point_2{current.x, current.y}, next, blockList, net) && dist.find(next) == dist.end()) {
//                 dist[next] = dist[Point_2{current.x, current.y}] + 1;
//                 parent[next] = Point_2{current.x, current.y};
//                 q.push(Cell{next.x, next.y, dist[next]});
//             }
//         }
//     }

//     return backtrack(start, goal, parent);
// }

vector<Point_2> backtrack(Point_2 start, Point_2 goal, const map<Point_2, Point_2>& parent) {
    vector<Point_2> path;
    for (Point_2 at = goal; at.x != -1; at = parent.at(at)) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

void printPath(const vector<Point_2>& path, ofstream& file) {
    for (const auto& point : path) {
        file << point.x << ";" << point.y << ",";
    }
    file << endl;
}



// original version
// vector<Point_2> BFS(Point_2 start, Point_2 goal, vector<vector<int>> grid) {
//     // initailization
//     int rows = grid.size();
//     int cols = grid[0].size();
//     dist = vector<vector<int>>(rows, vector<int>(cols, INF));
//     parent = vector<vector<Point_2>>(rows, vector<Point_2>(cols, Point_2{-1, -1}));
    
//     priority_queue<Cell> pq;
//     pq.push(Cell(start.x, start.y, 0));
//     dist[start.x][start.y] = 0;

//     while (!pq.empty()) {
//         Cell current = pq.top();
//         pq.pop();
        
//         if (current.x == goal.x && current.y == goal.y) break;
        
//         for (int i = 0; i < 4; i++) {       // four possibility to go
//             int nx = current.x + dx[i];
//             int ny = current.y + dy[i];
            
//             // dist[current.x][current.y] + 1: new move distance
//             // dist[][] is original diatance
//             if (isValid(nx, ny, rows, cols) && grid[ny][nx] == 0 && dist[nx][ny] > dist[current.x][current.y] + 1) {
//                 dist[nx][ny] = dist[current.x][current.y] + 1;
//                 parent[nx][ny] = Point_2{current.x, current.y};
//                 pq.push(Cell(nx, ny, dist[nx][ny]));
//             }
//         }
//     }

//     return backtrack(start, goal);
// }

// vector<Point_2> backtrack(Point_2 start, Point_2 goal) {
//     vector<Point_2> path;

//     for (Point_2 at = goal; at.x != -1; at = parent[at.x][at.y]) {
//         path.push_back(at);
//     }
//     reverse(path.begin(), path.end());
//     return path;
// }
