#include <queue>
#include <algorithm> // Include this header for std::reverse

#include "BFS.h"


using namespace std;

const int INF = 10000000;
const int dx[4] = {1, -1, 0, 0}; // Directions: up, down, left, right
const int dy[4] = {0, 0, 1, -1};


void populateEdgeAndBlockMaps(vector<Block> blockList, unordered_map<pair<int, int>, int, pair_hash> &edgeMap, unordered_map<int, Block> &blockMap) {
    for (size_t i = 0; i < blockList.size(); ++i) {
        const Block& block = blockList[i];
        for (const auto& vertex : block.vertices) {
            edgeMap[{vertex.x, vertex.y}] = i;
        }
        blockMap[i] = block;
    }
}

void populateRegionMaps(vector<Region> regions, unordered_map<int, Region> &regionMap) {
    for (size_t i = 0; i < regions.size(); ++i) {
        const Region& reg = regions[i];
        regionMap[i] = reg;
    }
}

Point convertPoint(const Point &point, const vector<float> &coord) {
    return Point{point.x + static_cast<int>(coord[0])*2000, point.y + static_cast<int>(coord[1])*2000};
}

Point getReferencePoint(const string &identifier, unordered_map<int, Block>& blockMap, unordered_map<int, Region>& regionMap) {
    if (identifier.find("BLOCK_") == 0) {
        int blockId = stoi(identifier.substr(6)); // "BLOCK_" 的長度是 6
        if (blockMap.find(blockId) != blockMap.end()) {
            return {blockMap[blockId].position.x, blockMap[blockId].position.y};
        }
    } else if (identifier.find("REGION_") == 0) {
        int regionId = stoi(identifier.substr(7)); // "REGION_" 的長度是 7
        if (regionMap.find(regionId) != regionMap.end()) {
            return {regionMap[regionId].vertices[0].x, regionMap[regionId].vertices[0].y};
        }
    }
    return Point{0, 0}; // Default fallback, in case identifier not found
}

bool isValid(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}


// ray-casting algorithm
bool isPointInsideBlock(const Point& pt, const Block& block) {
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

// bool canMove(const Point& from, const Point& to, const vector<Block>& blockList, const Net& net) {
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

bool canMove(const Point& from, const Point& to, const unordered_map<pair<int, int>, int, pair_hash>& edgeMap, unordered_map<int, Block>& blockMap, const Net& net) {
    if (edgeMap.find({to.x, to.y}) != edgeMap.end()) {
        int blockId = edgeMap.at({to.x, to.y});
        const Block& block = blockMap.at(blockId);
        if (!block.is_feedthroughable || block.through_block_net_num <= 0) {
            return false;
        }
    }
    return true;
}


vector<Point> BFS(Point start, Point goal, const unordered_map<pair<int, int>, int, pair_hash>& edgeMap, unordered_map<int, Block>& blockMap, const Net& net, int ROW, int COL) {
    queue<Cell> q;
    map<Point, int> dist;
    map<Point, Point> parent;

    q.push(Cell{start.x, start.y, 0});
    dist[start] = 0;
    parent[start] = Point{-1, -1};

    while (!q.empty()) {
        Cell current = q.front();
        q.pop();

        if (current.x == goal.x && current.y == goal.y) break;

        for (int i = 0; i < 4; i++) {
            Point next = {current.x + dx[i], current.y + dy[i]};

            if (isValid(next.x, next.y, ROW, COL) && canMove(Point{current.x, current.y}, next, edgeMap, blockMap, net) && dist.find(next) == dist.end()) {
                dist[next] = dist[Point{current.x, current.y}] + 1;
                parent[next] = Point{current.x, current.y};
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

// vector<Point> BFS(Point start, Point goal, const vector<Block>& blockList, const Net& net, int ROW, int COL) {
//     queue<Cell> q;
//     map<Point, int> dist;
//     map<Point, Point> parent;

//     q.push(Cell{start.x, start.y, 0});
//     dist[start] = 0;
//     parent[start] = Point{-1, -1};

//     while (!q.empty()) {
//         Cell current = q.front();
//         q.pop();
        
//         if (current.x == goal.x && current.y == goal.y) break;
        
//         for (int i = 0; i < 4; i++) {
//             Point next = {current.x + dx[i], current.y + dy[i]};
            
//             if (isValid(next.x, next.y, ROW, COL) && canMove(Point{current.x, current.y}, next, blockList, net) && dist.find(next) == dist.end()) {
//                 dist[next] = dist[Point{current.x, current.y}] + 1;
//                 parent[next] = Point{current.x, current.y};
//                 q.push(Cell{next.x, next.y, dist[next]});
//             }
//         }
//     }

//     return backtrack(start, goal, parent);
// }

vector<Point> backtrack(Point start, Point goal, const map<Point, Point>& parent) {
    vector<Point> path;
    for (Point at = goal; at.x != -1; at = parent.at(at)) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());
    return path;
}

void printPath(const vector<Point>& path, ofstream& file) {
    for (const auto& point : path) {
        file << point.x << ";" << point.y << ",";
    }
    file << endl;
}

