#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm> // for find_if 
#include <map>
#include "readfile.h"
// #include "linesearch.h"
#include "BFS.h"
// #include "Astar.h"

using namespace std;


// function used in output of plotting csv file
string pointToString(const Point& point) {
    ostringstream oss;
    oss << point.x << ";" << point.y;
    return oss.str();
}


int main(int argc, char *argv[]) {
    vector<Block> blockList;
    vector<Net> nets;
    vector<Component> components;
    vector<Region> regions;
    int num_Comp = 0;
    int UNITS_DISTANCE_MICRONS = 0;
    DieArea diearea;
    vector<OnlyBlock> onlyblocks;

    // File path
    // string blockFilePath = "case4/case4_cfg.json";
    // string netFilePath = "case4/case4_small.json";
    // string defFilePath = "case4/case4_def/chip_top.def";

    char *blockFilePath = argv[1];
    char *netFilePath = argv[2];
    char *defFilePath = argv[3];
    char *defDirPath = argv[4];

    readJsonFiles(blockFilePath, netFilePath, blockList, nets);
    readDefFile(defFilePath, components, regions, num_Comp, UNITS_DISTANCE_MICRONS, diearea);

    // File path for individual DEF of component
    vector<string> compFilePaths(num_Comp);
    for (int i = 0; i < num_Comp; ++i) {
        char filePath[50];
        sprintf(filePath, "%s/blk_%d.def", defDirPath, i);
        compFilePaths[i] = string(filePath); 
    }

    for (const auto& path : compFilePaths) {
        readCompFile(path, onlyblocks);
    }

    for (const auto& block : onlyblocks) {
        cout << "Block Name: " << block.name << endl;
        for (const auto& pt : block.vertices) {
            cout << "Vertex: (" << pt.x << ", " << pt.y << ")" << endl;
        }
        cout << "(Width, Height): " << block.width << ", "<< block.height << endl;
        cout << endl;
    }

    // Print components for verification
    // cout << "Components:" << endl;
    // for (const auto& component : components) {
    //     cout << "Name: " << component.name 
    //          << ", Blocktype: " << component.blocktype 
    //          << ", X: " << component.x 
    //          << ", Y: " << component.y 
    //          << ", Orientation: " << component.orientation 
    //          << endl;
    // }


    // Build Block
    updateBlocksWithVertices(blockList, components, onlyblocks);

    // Print blockList for verification
    for (const auto& block : blockList) {
        cout << "Block: " << block.block_name << ", Position: (" << block.position.x << ", " << block.position.y << "), Vertices: ";
        for (const auto& vertex : block.vertices) {
            cout << "(" << vertex.x << ", " << vertex.y << ") ";
        }
        cout << "isFeedthroughable: " << block.is_feedthroughable;
        cout << endl;
    }

    // Print onlyblocks for verification
    // cout << "OnlyBlocks:" << endl;
    // for (const auto& onlyblock : onlyblocks) {
    //     cout << "Name: " << onlyblock.name << ", Vertices: ";
    //     for (const auto& vertex : onlyblock.vertices) {
    //         cout << "(" << vertex.x << ", " << vertex.y << ") ";
    //     }
    //     cout << endl;
    // }

    int ROW = diearea.x2;
    int COL = diearea.y2;


    // perform BFS
    unordered_map<pair<int, int>, int, pair_hash> edgeMap;
    unordered_map<int, Block> blockMap;
    unordered_map<int, Region> regionMap;
    map<int, vector<vector<Point>>> netPaths;

    populateEdgeAndBlockMaps(blockList, edgeMap, blockMap);
    populateRegionMaps(regions, regionMap);

    for (const auto& net : nets) {
        // �B�z TX
        Point start = convertPoint(getReferencePoint(net.TX, blockMap, regionMap), net.TX_COORD);
        cout << "Start Point for Net " << net.ID << ": (" << start.x << ", " << start.y << ")" << endl;

        // �B�z�C�� RX
        for (size_t i = 0; i < net.RX.size(); ++i) {
            Point goal = convertPoint(getReferencePoint(net.RX[i], blockMap, regionMap), net.RX_COORD[i]);
            cout << "Goal Point for Net " << net.ID << " RX " << i << ": (" << goal.x << ", " << goal.y << ")" << endl;

            // ���B�i�H�[�J BFS �� A* ���|�d��N�X
            // vector<Point> path = BFS(start, goal, edgeMap, blockMap, net, ROW, COL);
            // vector<Point> path = AStar(start, goal, blockList, net, ROW, COL);
            // netPaths[net.ID].push_back(path);
        }
    }

    // for (const auto& net : nets) {
    //     Point start = {static_cast<int>(net.TX_COORD[0]), static_cast<int>(net.TX_COORD[1])};
    //     for (const auto& rx_coord : net.RX_COORD) {
    //         Point goal = {static_cast<int>(rx_coord[0]), static_cast<int>(rx_coord[1])};
    //         vector<Point> path = BFS(start, goal, edgeMap, blockMap, net, ROW, COL);
    //         // vector<Point> path = AStar(start, goal, blockList, net, ROW, COL);
    //         netPaths[net.ID].push_back(path);
    //     }
    // }

    // print all stored paths
    ofstream pathfile("path.csv");
    for (const auto& netPath : netPaths) {
        pathfile << "Net ID: " << netPath.first << endl;
        for (const auto& path : netPath.second) {
            printPath(path, pathfile);
        }
     
    }
    pathfile.close();


    // // output plotting csv file, called "blocks.csv"
    // ofstream file("blocks.csv");
    // for (const auto& block : blockList) {
    //     file << block.block_name << ",";
    //     for (const auto& pt : block.vertices) {
    //         file << pointToString(pt);
    //         file << ",";
    //     }
    //     file << "\n";
    // }
    // file.close();

    // output plotting csv file, called "blocks.csv"
    ofstream feedthroughfile("blocks_feedthrough.csv");
    ofstream nonfeedthroughfile("blocks_nonfeedthrough.csv");
    for (const auto& block : blockList) {
        if (block.is_feedthroughable == true) {
            feedthroughfile << block.block_name << ",";
            for (const auto& pt : block.vertices) {
                feedthroughfile << pointToString(pt);
                feedthroughfile << ",";
            }
            feedthroughfile << "\n";
        } else {
            nonfeedthroughfile << block.block_name << ",";
            for (const auto& pt : block.vertices) {
                nonfeedthroughfile << pointToString(pt);
                nonfeedthroughfile << ",";
            }
            nonfeedthroughfile << "\n";
        }
    }
    feedthroughfile.close();
    nonfeedthroughfile.close();

    // output plotting csv file, called "regions.csv"
    ofstream regionfile("regions.csv");
    for (const auto& region : regions) {
        regionfile << region.name << ",";
        for (const auto& pt : region.vertices) {
            regionfile << pointToString(pt);
            regionfile << ",";
        }
        regionfile << "\n";
    }
    regionfile.close();


    return 0;
}


