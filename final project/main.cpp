#include <iostream>
#include <string>
#include <vector>
#include "readfile.h"
// #include "linesearch.h"
#include "BFS.h"

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


    // file path for individual DEF of component
    std::vector<std::string> compFilePaths(num_Comp);
    for (int i = 0; i < num_Comp; ++i) {
        char filePath[50];
        std::sprintf(filePath, "case4/case4_def/blk_%d.def", i);
        compFilePaths[i] = std::string(filePath); 
    }

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



    // // Example usage (replace this part with your file reading logic)
    // // 1 for block
    // vector<vector<int>> grid = {
    //     {0, 0, 0, 1},
    //     {1, 1, 0, 1},
    //     {0, 1, 0, 0}
    // };
    // Point_2 start(0, 0);
    // Point_2 target(3, 2);

    // vector<Point_2> path = mikami_tabuchi_line_search(grid, start, target);
    // if (path.empty()) {
    //     cout << "No path found." << endl;
    // } 
    // else {
    //     cout << "Path found: ";
    //     for (const Point_2& p : path) {
    //         cout << "(" << p.x << ", " << p.y << ") ";
    //     }
    //     cout << endl;
    // }



    // for input parameters
    vector<vector<int>> grid;
    int row_num = diearea.x2;
    int col_num = diearea.y2;
    grid = vector<vector<int>>(row_num, vector<int>(col_num, 0));

    /*
        arr=0 : free
        arr=-1 : has block
        arr=n : net number
    */
    for(int i=0; i < row_num; i++){
        for(int j=0; j< col_num; j++){
            cout << grid[i][j] << " " << endl;
        }
        cout << endl;
    }

    // set block
    // for(const auto& b : blockList) {
    //     for(int i = b[0]; i <= b[1]; i++) {
    //         for(int j = b[2]; j <= b[3]; j++) {
    //             arr[j][i] = -1;
    //         }
    //     }
    // }

    // for(int i=1; i < netList.size(); i++) {     // go through netList row size
    //     arr[netList[i][1]][netList[i][0]] = i;
    //     arr[netList[i][3]][netList[i][2]] = i;
    // }






    // for BFS.cpp
    // grid = vector<vector<int>>(rows, vector<int>(cols, 0));
    //  1: obstacles, 0: can pass
    // vector<vector<int>> grid;
    // grid = {
    //     {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    //     {1, 1, 0, 0, 1, 0, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 1, 1, 0, 0, 0, 0},
    //     {0, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    //     {0, 0, 0, 1, 0, 1, 1, 1, 0, 0},
    //     {1, 1, 0, 0, 0, 0, 0, 1, 1, 1},
    //     {0, 1, 0, 0, 0, 1, 0, 0, 0, 0}
    // };
    // // (1, 0) is downward, (0, 1) is go right 

    // Point_2 start(0, 0);
    // Point_2 goal(9, 9);

    // vector<Point_2> path;
    // path = BFS(start, goal, grid);
    

    // cout << "Path: ";
    // for (auto p : path) {
    //     cout << "(" << p.x << "," << p.y << ") ";
    // }
    // cout << endl;

    return 0;

}

