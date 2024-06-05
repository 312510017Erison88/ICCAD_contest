#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "readfile.h"
#include <climits>  // For INT_MIN and INT_MAX

using json = nlohmann::json;
using namespace std;


void readJsonFiles(const string& blockFilePath, const string& netFilePath, vector<Block>& blocks, vector<Net>& nets) {
   
   // Read block JSON file
   ifstream blockFile(blockFilePath);
   if (!blockFile.is_open()) {
       cerr << "Could not open the block file." << endl;
       return;
   }

   json blockJson;
   try {
       blockFile >> blockJson;
   } catch (json::parse_error& e) {
       cerr << "Parse error in block file: " << e.what() << endl;
       return;
   }
   blockFile.close();

   if (!blockJson.is_array()) {
       cerr << "Block JSON file is not an array." << endl;
       return;
   }

   for (const auto& item : blockJson) {
       Block block;
       block.block_name = item["block_name"];
       block.through_block_net_num = item["through_block_net_num"];
       block.through_block_edge_net_num = item["through_block_edge_net_num"].get<vector<int>>();
       block.block_port_region = item["block_port_region"].get<vector<int>>();
       block.is_feedthroughable = item["is_feedthroughable"] == "True";
       block.is_tile = item["is_tile"] == "False";

       blocks.push_back(block);  // Add the block to the vector of blocks
   }

   // Read net JSON file
   ifstream netFile(netFilePath);
   if (!netFile.is_open()) {
       cerr << "Could not open the net file." << endl;
       return;
   }

   json netJson;
   try {
       netFile >> netJson;
   } catch (json::parse_error& e) {
       cerr << "Parse error in net file: " << e.what() << endl;
       return;
   }
   netFile.close();

   for (const auto& item : netJson) {
       Net net;
       net.ID = item["ID"];
       net.TX = item["TX"];
       net.RX = item["RX"].get<vector<string>>();
       net.NUM = item["NUM"];
       net.MUST_THROUGH = item["MUST_THROUGH"].get<map<string, vector<int>>>();
       net.HMFT_MUST_THROUGH = item["HMFT_MUST_THROUGH"].get<map<string, vector<int>>>();
       net.TX_COORD = item["TX_COORD"].get<vector<float>>();
       net.RX_COORD = item["RX_COORD"].get<vector<vector<float>>>();

       nets.push_back(net);
   }
}

void readDefFile(const std::string& defFilePath, std::vector<Component>& components, std::vector<Region>& regions, int& num_Comp, int& UNITS_DISTANCE_MICRONS, DieArea& diearea) {
    std::ifstream defFile(defFilePath);
    if (!defFile.is_open()) {
        std::cerr << "Failed to open file: " << defFilePath << std::endl;
        return;
    }

    std::string line;
    while (std::getline(defFile, line)) {
        std::istringstream iss(line);
        std::string temp;
        if (line.find("UNITS DISTANCE MICRONS") != std::string::npos) {
            iss >> temp >> temp >> temp >> UNITS_DISTANCE_MICRONS;
        }
        if (line.find("DIEAREA") != std::string::npos) {
            iss >> temp >> temp >> diearea.x1 >> diearea.y1 >> temp >> temp >> diearea.x2 >> diearea.y2;
        }
        if (line.find("COMPONENTS") != std::string::npos) {
            iss >> temp >> num_Comp;
            break;
        }
    }


    // std::string line;
    // while (std::getline(defFile, line)) {
    //     if (line.find("COMPONENTS") != std::string::npos) {
    //         std::istringstream iss(line);
    //         std::string temp;
    //         iss >> temp >> num_Comp;
    //         break;
    //     }
    // }


    while (std::getline(defFile, line)) {
        if (line.find("END COMPONENTS") != std::string::npos) {
            break;
        }
        if (line[0] == '-') {
            Component comp;
            std::istringstream iss(line);
            std::string temp;
            iss >> temp >> comp.name >> comp.blocktype >> temp >> temp >> temp >> comp.x >> comp.y >> temp >> comp.orientation;
            components.push_back(comp);
        }
    }

    std::string row;
    while (std::getline(defFile, row)) {
        if (row.find("REGIONS") != std::string::npos) {
            break;
        }
    }
    while (std::getline(defFile, row)) {
        if (row.find("END REGIONS") != std::string::npos) {
            break;
        }
        if (row[0] == '-') {
            Region reg;
            std::istringstream iss(row);
            std::string temp;
            iss >> temp >> reg.name >> temp >> reg.x1 >> reg.y1 >> temp >> temp >> reg.x2 >> reg.y2;
            regions.push_back(reg);
        }
    }

}

// Calculate the width and height of a component
void WidthHeight(OnlyBlock& onlyb) {

    // Initialize the MIN and MAX of X and Y axis
    int minX = INT_MAX, maxX = INT_MIN;
    int minY = INT_MAX, maxY = INT_MIN;

    //for (auto& block : onlyb) {
        for (const auto& vertex : onlyb.vertices) {
            if (vertex.x < minX) minX = vertex.x;
            if (vertex.x > maxX) maxX = vertex.x;
            if (vertex.y < minY) minY = vertex.y;
            if (vertex.y > maxY) maxY = vertex.y;
        }
        onlyb.width = maxX - minX;
        onlyb.height = maxY - minY;
    //}

    // Calculate the lengths
    //width = maxX - minX;
    //height = maxY - minY;

    // print
    //std::cout << "width: " << width << std::endl;
    //std::cout << "height: " << height << std::endl;
}



void readCompFile(const std::string& compFilePath, std::vector<OnlyBlock>& onlyblocks) {
    std::ifstream compFile(compFilePath);
    // check if the paths can found
    // if (!compFile.is_open()) {
    //     std::cerr << "Could not open the file: " << compFilePath << std::endl;
    // }

    std::string line1;
    OnlyBlock onlyb;
    while (std::getline(compFile, line1)) {
        std::istringstream iss(line1);
        std::string temp1;
        if (line1.find("DESIGN") != std::string::npos) {
            iss >> temp1 >> onlyb.name;
        }
        if (line1.find("DIEAREA") != std::string::npos) {
            std::istringstream iss(line1);
            std::string token;

            // ��?"DIEAREA"
            iss >> token;

            // ?��??��?
            while (iss >> token) {
                if (token == "(") {
                    Point pt;
                    iss >> pt.x >> pt.y;
                    onlyb.vertices.push_back(pt);
                } else if (token == ")") {
                    continue;
                } else {
                    break;
                }
            }
            WidthHeight(onlyb);
            onlyblocks.push_back(onlyb);
            break; // Assuming only one DIEAREA line1 per file
        }
    }
}

Point rotatePoint(const Point& pt, const Point& origin, int angle) {
    int x = pt.x - origin.x;
    int y = pt.y - origin.y;
    int newX, newY;

    switch (angle) {
        case 90:
            newX = -y;
            newY = x;
            break;
        case 180:
            newX = -x;
            newY = -y;
            break;
        case 270:
            newX = y;
            newY = -x;
            break;
        default:
            newX = x;
            newY = y;
            break;
    }

    return {newX + origin.x, newY + origin.y};
}

Point reflectPoint(const Point& pt, const Point& origin, bool isYAxis) {
    int x = pt.x - origin.x;
    int y = pt.y - origin.y;
    if (isYAxis) {
        x = -x;
    } else {
        y = -y;
    }
    return {x + origin.x, y + origin.y};
}

vector<Point> transformVertices(const vector<Point>& vertices, const Point& origin, const string& orientation) {
    vector<Point> newVertices;

    for (const auto& vertex : vertices) {
        Point transformedVertex = vertex;
        Point newVertex = vertex;
        if (orientation == "N") {
            // No rotation
            newVertex.x = origin.x + transformedVertex.x;
            newVertex.y = origin.y + transformedVertex.y;

        } else if (orientation == "S") {
            // transformedVertex = rotatePoint(vertex, origin, 180);
            newVertex.x = origin.x - transformedVertex.x;
            newVertex.y = origin.y - transformedVertex.y;
        } else if (orientation == "W") {
            // transformedVertex = rotatePoint(vertex, origin, 90);
            newVertex.x = origin.x - transformedVertex.y;
            newVertex.y = origin.y + transformedVertex.x;
        } else if (orientation == "E") {
            // transformedVertex = rotatePoint(vertex, origin, 270);
            newVertex.x = origin.x + transformedVertex.y;
            newVertex.y = origin.y - transformedVertex.x;
        } else if (orientation == "FN") {
            newVertex = reflectPoint(vertex, origin, true);
        } else if (orientation == "FS") {
            newVertex = reflectPoint(vertex, origin, false);
        } else if (orientation == "FW") {
            newVertex = reflectPoint(rotatePoint(vertex, origin, 90), origin, false);
        } else if (orientation == "FE") {
            newVertex = reflectPoint(rotatePoint(vertex, origin, 270), origin, true);
        }
        newVertices.push_back(newVertex);
    }

    return newVertices;
}



