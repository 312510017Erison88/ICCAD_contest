#include <iostream>
#include <string>
#include <vector>
#include "readfile.h"
#include "linesearch.h"

using namespace std;


int main() {
    vector<Block> blocks;
    vector<Net> nets;
    vector<Component> components;
    vector<Region> regions;
    int num_Comp = 0;
    int UNITS_DISTANCE_MICRONS = 0;
    DieArea diearea;

    

    // file path
    string blockFilePath = "case4/case4_cfg.json";
    string netFilePath = "case4/case4.json";
    string defFilePath = "case4/case4_def/chip_top.def";

    readJsonFiles(blockFilePath, netFilePath, blocks, nets);

    readDefFile(defFilePath, components, regions, num_Comp, UNITS_DISTANCE_MICRONS, diearea);

    // readCompFile(compFilePaths, );
    
    // for (const auto& block : blocks) {
    //     cout << "Block Name: " << block.block_name << endl;
    //     cout << "Block through_block_net_num: " << block.through_block_net_num << endl;
    // }
    

    // for (const auto& net : nets) {
    //     cout << "Net ID: " << net.ID << endl;
    // }

    // printf("UNITS_DISTANCE_MICRONS = %d\n", UNITS_DISTANCE_MICRONS);
    // printf("DieArea = (%d, %d) (%d, %d)\n", diearea.x1, diearea.y1, diearea.x2, diearea.y2);
    // printf("Number of Component = %d\n", num_Comp);
    // for (const auto& comp : components) {
    //     cout << "Component Name: " << comp.name << " at (" << comp.x << ", " << comp.y << "), "<< comp.orientation <<"" << endl;
    // }

    // for (const auto& reg : regions) {
    //     cout << "Region Name: " << reg.name << " at (" << reg.x1 << ", " << reg.y1 << "), (" << reg.x2 << ", " << reg.y2 << ")" << endl;
    // }

    // file path for individual DEF of component
    std::vector<std::string> compFilePaths(num_Comp);
    for (int i = 0; i < num_Comp; ++i) {
        char filePath[50];
        std::sprintf(filePath, "case4/case4_def/blk_%d.def", i);
        compFilePaths[i] = std::string(filePath); 
    }

    // print the paths
    // for (const auto& path : compFilePaths) {
    //     std::cout << path << std::endl;
    // }

    std::vector<std::vector<Point>> allVertices;

    for (const auto& path : compFilePaths) {
        std::vector<Point> vertices = readCompFile(path);
        allVertices.push_back(vertices);

        // std::cout << "File: " << path << std::endl;
        // for (const auto& pt : vertices) {
        //     std::cout << "Vertex: (" << pt.x << ", " << pt.y << ")" << std::endl;
        // }
        // std::cout << std::endl;
    
    }



    // Example usage (replace this part with your file reading logic)
    // 1 for block
    vector<vector<int>> grid = {
        {0, 0, 0, 1},
        {1, 1, 0, 1},
        {0, 1, 0, 0}
    };
    Point_2 start(0, 0);
    Point_2 target(3, 2);

    vector<Point_2> path = mikami_tabuchi_line_search(grid, start, target);
    if (path.empty()) {
        cout << "No path found." << endl;
    } 
    else {
        cout << "Path found: ";
        for (const Point_2& p : path) {
            cout << "(" << p.x << ", " << p.y << ") ";
        }
        cout << endl;
    }

    return 0;

}

