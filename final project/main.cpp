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

float UNITS_DISTANCE_MICRONS = 2000.0;
// int main() {
//     vector<Block> blocks;
//     vector<Net> nets;
//     vector<Component> components;
//     vector<Region> regions;
//     int num_Comp = 0;
//     int UNITS_DISTANCE_MICRONS = 0;
//     DieArea diearea;
//     vector<OnlyBlock> onlyblocks;

    

//     // file path
//     string blockFilePath = "case4/case4_cfg.json";
//     string netFilePath = "case4/case4.json";
//     string defFilePath = "case4/case4_def/chip_top.def";

//     readJsonFiles(blockFilePath, netFilePath, blocks, nets);

//     readDefFile(defFilePath, components, regions, num_Comp, UNITS_DISTANCE_MICRONS, diearea);


//     // file path for individual DEF of component
//     vector<string> compFilePaths(num_Comp);
//     for (int i = 0; i < num_Comp; ++i) {
//         char filePath[50];
//         sprintf(filePath, "case4/case4_def/blk_%d.def", i);
//         compFilePaths[i] = string(filePath); 
//     }

    
//     for (const auto& path : compFilePaths) {
//         readCompFile(path, onlyblocks);
//         // std::cout << "File: " << path << std::endl;
//     }

//     for (const auto& block : onlyblocks) {
//         std::cout << "Block Name: " << block.name << std::endl;
//         for (const auto& pt : block.vertices) {
//             std::cout << "Vertex: (" << pt.x << ", " << pt.y << ")" << std::endl;
//         }
//         std::cout << std::endl;
    
//     }

// //////////////////////////////////////////////////////////////////////////
//     // for input parameters
//     vector<Block> blockList;

//     for (const auto& component : components) {
//         auto it = find_if(onlyblocks.begin(), onlyblocks.end(), [&](const OnlyBlock& ob) { return ob.name == component.name; });
//         if (it != onlyblocks.end()) {
//             Block block;
//             block.name = component.name;
//             block.position = {component.x, component.y};
//             block.vertices = transformVertices(it->vertices, block.position, component.orientation);
//             blockList.push_back(block);
//         }
//     }

//     // Print blockList for verification
//     for (const auto& block : blockList) {
//         cout << "Block: " << block.name << ", Position: (" << block.position.x << ", " << block.position.y << "), Vertices: ";
//         for (const auto& vertex : block.vertices) {
//             cout << "(" << vertex.x << ", " << vertex.y << ") ";
//         }
//         cout << endl;
//     }





//     // for BFS.cpp
//     // grid = vector<vector<int>>(rows, vector<int>(cols, 0));
//     //  1: obstacles, 0: can pass
//     // vector<vector<int>> grid;
//     // grid = {
//     //     {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
//     //     {0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
//     //     {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
//     //     {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
//     //     {1, 1, 0, 0, 1, 0, 0, 0, 0, 0},
//     //     {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
//     //     {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
//     //     {0, 0, 0, 1, 0, 1, 1, 1, 0, 0},
//     //     {1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
//     //     {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}
//     // };
//     // // (1, 0) is downward, (0, 1) is go right 

//     // Point_2 start(0, 0);
//     // Point_2 goal(9, 9);

//     // vector<Point_2> path;
//     // path = BFS(start, goal, grid);
    

//     // cout << "Path: ";
//     // for (auto p : path) {
//     //     cout << "(" << p.x << "," << p.y << ") ";
//     // }
//     // cout << endl;

//     return 0;

// }

// function used in output of plotting csv file
string pointToString(const Point& point) {
    ostringstream oss;
    oss << point.x << ";" << point.y;
    return oss.str();
}


int main(int argc, char *argv[]) {
    // vector<Block> blocks;
    vector<Block> blockList;
    vector<Net> nets;
    vector<Component> components;
    vector<Region> regions;
    int num_Comp = 0;
    // int UNITS_DISTANCE_MICRONS = 0;
    DieArea diearea;
    vector<OnlyBlock> onlyblocks;

    // File path
    // string blockFilePath = "case4/case4_cfg.json";
    // string netFilePath = "case4/case4_small.json";
    // string defFilePath = "case4/case4_def/chip_top.def";
    char *blockFilePath = argv[1];
	char *netFilePath = argv[2];
    char *defFilePath = argv[3];

    readJsonFiles(blockFilePath, netFilePath, blockList, nets);
    readDefFile(defFilePath, components, regions, num_Comp, UNITS_DISTANCE_MICRONS, diearea);

    // File path for individual DEF of component
    vector<string> compFilePaths(num_Comp);
    for (int i = 0; i < num_Comp; ++i) {
        char filePath[50];
        sprintf(filePath, "case4/case4_def/blk_%d.def", i);
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

    float ROW = diearea.x2;
    float COL = diearea.y2;


    // perform BFS
    unordered_map<pair<float, float>, int, pair_hash> edgeMap;
    unordered_map<int, Block> blockMap;
    map<int, vector<vector<Point_2>>> netPaths;

    populateEdgeAndBlockMaps(blockList, edgeMap, blockMap);

    for (const auto& net : nets) {
        Point_2 start = {static_cast<float>(net.TX_COORD[0]), static_cast<float>(net.TX_COORD[1])};
        for (const auto& rx_coord : net.RX_COORD) {
            Point_2 goal = {static_cast<float>(rx_coord[0]), static_cast<float>(rx_coord[1])};
            vector<Point_2> path = BFS(start, goal, edgeMap, blockMap, net, ROW, COL);
            // vector<Point_2> path = AStar(start, goal, blockList, net, ROW, COL);
            netPaths[net.ID].push_back(path);
        }
    }
    // print all stored paths
    ofstream pathfile("path.csv");
    for (const auto& netPath : netPaths) {
        pathfile << "Net ID: " << netPath.first << endl;
        for (const auto& path : netPath.second) {
            printPath(path, pathfile);
        }
        
    }
    pathfile.close();


    // output plotting csv file, called "blocks.csv"
    ofstream file("blocks.csv");
    for (const auto& block : blockList) {
        file << block.block_name << ",";
        for (const auto& pt : block.vertices) {
            file << pointToString(pt);
            file << ",";
        }
        file << "\n";
    }
    file.close();

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


