#include "Greedy.h"

using namespace std;

bool isValid(int x, int y, int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols;
}

bool canMove(const Point& from, const Point& to, const EdgeMap& edgeMap, BlockMap& blockMap, const Net& net) {
    if (edgeMap.find({to.x, to.y}) != edgeMap.end()) {
        int blockId = edgeMap.at({to.x, to.y});
        const Block& block = blockMap.at(blockId);
        if (!block.is_feedthroughable || block.through_block_net_num <= 0) {
            return false;
        }
    }
    return true;
}

// int manhattanDistance(const Point& a, const Point& b) {
//     return abs(a.x - b.x) + abs(a.y - b.y);
// }

vector<Point> Greedy(Point start, Point goal, const EdgeMap& edgeMap, BlockMap& blockMap, const Net& net, int ROW, int COL) {
    vector<Point> path;
    Point current = start;

    while (current != goal) {
        path.push_back(current);
        Point nextStep = current;
        double minDist = numeric_limits<double>::max();

        // // new
        // for (const auto& [dx, dy] : vector<pair<int, int>>{{1, 0}, {-1, 0}, {0, 1}, {0, -1}}) {
        //     Point next = {current.x + dx, current.y + dy};
        //     if (isValid(next.x, next.y, ROW, COL) && canMove(current, next, edgeMap, blockMap, net)) {
        //         double dist = sqrt(pow(next.x - goal.x, 2) + pow(next.y - goal.y, 2));
        //         if (dist < minDist) {
        //             minDist = dist;
        //             nextStep = next;
        //         }
        //     }
        // }

        for (int dx : {1, -1, 0, 0}) {
            for (int dy : {0, 0, 1, -1}) {
                Point next = {current.x + dx, current.y + dy};
                if (isValid(next.x, next.y, ROW, COL) && canMove(current, next, edgeMap, blockMap, net)) {
                    double dist = pow(next.x - goal.x, 2) + pow(next.y - goal.y, 2);
                    if (dist < minDist) {
                        minDist = dist;
                        nextStep = next;
                    }
                }
            }
        }

        if (nextStep == current) {
            cout << "No valid moves available. Stuck at: (" << current.x << ", " << current.y << ")" << endl;
            break;
        }

        current = nextStep;

        if (edgeMap.count({current.x, current.y}) > 0) {
            int blockId = edgeMap.at({current.x, current.y});
            blockMap[blockId].through_block_net_num -= 1;
        }
    }

    path.push_back(goal);
    return path;
}

void printPath(const vector<Point>& path, ofstream& file) {
    for (const auto& point : path) {
        file << point.x << ";" << point.y << ",";
    }
    file << endl;
}

void populateEdgeAndBlockMaps(const vector<Block>& blockList, EdgeMap& edgeMap, BlockMap& blockMap) {
    for (size_t i = 0; i < blockList.size(); ++i) {
        const Block& block = blockList[i];
        for (const auto& vertex : block.vertices) {
            edgeMap[{vertex.x, vertex.y}] = i;
        }
        blockMap[i] = block;
    }
}


void populateRegionMaps(vector<Region> regions, RegionMap& regionMap) {
    for (size_t i = 0; i < regions.size(); ++i) {
        const Region& reg = regions[i];
        regionMap[i] = reg;
    }
}

Point convertPoint(const Point &point, const vector<float> &coord) {
    return Point{point.x + static_cast<int>(coord[0])*UNITS_DISTANCE_MICRONS, point.y + static_cast<int>(coord[1])*UNITS_DISTANCE_MICRONS};
}

Point getReferencePoint(const string &identifier, BlockMap& blockMap, RegionMap& regionMap) {
    if (identifier.find("BLOCK_") == 0) {
        int blockId = stoi(identifier.substr(6)); // "BLOCK_"  6
        if (blockMap.find(blockId) != blockMap.end()) {
            return {blockMap[blockId].position.x, blockMap[blockId].position.y};
        }
    } else if (identifier.find("REGION_") == 0) {
        int regionId = stoi(identifier.substr(7)); // "REGION_"  7
        if (regionMap.find(regionId) != regionMap.end()) {
            return {regionMap[regionId].vertices[0].x, regionMap[regionId].vertices[0].y};
        }
    }
    return Point{0, 0}; // Default fallback, in case identifier not found
}