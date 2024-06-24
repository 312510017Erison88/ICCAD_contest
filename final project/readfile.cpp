
#include "nlohmann/json.hpp"
#include "readfile.h"
#include <climits>  // For INT_MIN and INT_MAX

using json = nlohmann::json;
using namespace std;

extern float UNITS_DISTANCE_MICRONS;


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
       block.block_port_region = item["block_port_region"].get<vector<float>>();
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
       net.MUST_THROUGH = item["MUST_THROUGH"].get<map<string, vector<float>>>();
       net.HMFT_MUST_THROUGH = item["HMFT_MUST_THROUGH"].get<map<string, vector<float>>>();
       net.TX_COORD = item["TX_COORD"].get<vector<float>>();
       net.RX_COORD = item["RX_COORD"].get<vector<vector<float>>>();

       nets.push_back(net);
   }
}

void readDefFile(const string& defFilePath, vector<Component>& components, vector<Region>& regions, int& num_Comp, float& UNITS_DISTANCE_MICRONS, DieArea& diearea) {
    ifstream defFile(defFilePath);
    if (!defFile.is_open()) {
        cerr << "Failed to open file: " << defFilePath << std::endl;
        return;
    }

    string line;
    while (getline(defFile, line)) {
        istringstream iss(line);
        string temp;
        if (line.find("UNITS DISTANCE MICRONS") != string::npos) {
            iss >> temp >> temp >> temp >> UNITS_DISTANCE_MICRONS;
        }
        if (line.find("DIEAREA") != string::npos) {
            iss >> temp >> temp >> diearea.x1 >> diearea.y1 >> temp >> temp >> diearea.x2 >> diearea.y2;
        }
        
        if (line.find("COMPONENTS") != string::npos) {
            iss >> temp >> num_Comp;
            break;
        }
    }
    diearea.x1 /= UNITS_DISTANCE_MICRONS;
    diearea.y1 /= UNITS_DISTANCE_MICRONS;
    diearea.x2 /= UNITS_DISTANCE_MICRONS;
    diearea.y2 /= UNITS_DISTANCE_MICRONS;


    while (getline(defFile, line)) {
        if (line.find("END COMPONENTS") != string::npos) {
            break;
        }
        if (line[0] == '-') {
            Component comp;
            istringstream iss(line);
            string temp;
            iss >> temp >> comp.name >> comp.blocktype >> temp >> temp >> temp >> comp.x >> comp.y >> temp >> comp.orientation;
            
            comp.x /= UNITS_DISTANCE_MICRONS;
            comp.y /= UNITS_DISTANCE_MICRONS;
            components.push_back(comp);
        }
    }

    string row;
    while (getline(defFile, row)) {
        if (row.find("REGIONS") != string::npos) {
            break;
        }
    }
    while (getline(defFile, row)) {
        if (row.find("END REGIONS") != string::npos) {
            break;
        }
        if (row[0] == '-') {
            Region reg;
            istringstream iss(row);
            string temp;
            iss >> temp >> reg.name;
            while (iss >> temp) {
                if (temp == "(") {
                    Point pt;
                    iss >> pt.x >> pt.y;
                    pt.x /= UNITS_DISTANCE_MICRONS;
                    pt.y /= UNITS_DISTANCE_MICRONS;
        
                    reg.vertices.push_back(pt);
                } else if (temp == ")") {
                    continue;
                } else {
                    break;
                }
            }
            if (reg.vertices.size() == 2) {
                Point p1 = reg.vertices[0];
                Point p2 = reg.vertices[1];
        
                // Calculate the other two vertices
                Point p3 = {p1.x, p2.y};
                Point p4 = {p2.x, p1.y};
        
                // Clear the existing vertices
                reg.vertices.clear();
        
                // Add the vertices in the order p1 -> p4 -> p2 -> p3
                reg.vertices.push_back(p1);
                reg.vertices.push_back(p4);
                reg.vertices.push_back(p2);
                reg.vertices.push_back(p3);
            }
            regions.push_back(reg);
        }
    }

}

void completeRectangle(OnlyBlock& block) {
    if (block.vertices.size() == 2) {
        Point p1 = block.vertices[0];
        Point p2 = block.vertices[1];

        // Calculate the other two vertices
        Point p3 = {p1.x, p2.y};
        Point p4 = {p2.x, p1.y};

        // Clear the existing vertices
        block.vertices.clear();

        // Add the vertices in the order p1 -> p4 -> p2 -> p3
        block.vertices.push_back(p1);
        block.vertices.push_back(p4);
        block.vertices.push_back(p2);
        block.vertices.push_back(p3);
    }
}

// Calculate the width and height of a component
void WidthHeight(OnlyBlock& onlyb) {

    // Initialize the MIN and MAX of X and Y axis
    float minX = LONG_MAX, maxX = LONG_MIN;
    float minY = LONG_MAX, maxY = LONG_MIN;
    // float minX = 10000000.0, maxX = 0.0001;
    // float minY = 10000000.0, maxY = 0.0001;

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



void readCompFile(const string& compFilePath, vector<OnlyBlock>& onlyblocks) {
    ifstream compFile(compFilePath);
    // check if the paths can found
    // if (!compFile.is_open()) {
    //     std::cerr << "Could not open the file: " << compFilePath << std::endl;
    // }

    string line1;
    OnlyBlock onlyb;
    while (getline(compFile, line1)) {
        istringstream iss(line1);
        string temp1;
        if (line1.find("DESIGN") != string::npos) {
            iss >> temp1 >> onlyb.name;
        }
        if (line1.find("DIEAREA") != string::npos) {
            istringstream iss(line1);
            string token;

            // "DIEAREA"
            iss >> token;

            while (iss >> token) {
                if (token == "(") {
                    Point pt;
                    iss >> pt.x >> pt.y;
                    pt.x /= UNITS_DISTANCE_MICRONS;
                    pt.y /= UNITS_DISTANCE_MICRONS;
                    onlyb.vertices.push_back(pt);
                } else if (token == ")") {
                    continue;
                } else {
                    break;
                }
            }
            WidthHeight(onlyb);
            completeRectangle(onlyb);
            onlyblocks.push_back(onlyb);
            break; // Assuming only one DIEAREA line1 per file
        }
    }
}

Point rotatePoint(const Point& pt, int angle, const float& width, const float& height) {
    float x = pt.x;
    float y = pt.y;
    float newX, newY;

    switch (angle) {
        case 90:
            newX = -y + height;
            newY = x;
            break;
        case 180:
            newX = -x + width;
            newY = -y + height;
            break;
        case 270:
            newX = y;
            newY = -x + width;
            break;
        default:
            newX = x;
            newY = y;
            break;
    }

    return {newX, newY};
}

Point reflectPoint(const Point& pt, bool isYAxis, const float& width, const float& height, bool haveRotate) {

    float x = pt.x;
    float y = pt.y;
    float blk_width;
    float blk_height;

    if (haveRotate) {
        blk_width = height;
        blk_height = width;
    } else{
        blk_width = width;
        blk_height = height;
    }

    if (isYAxis) {
        x = -x + blk_width;
    } else {
        y = -y + blk_height;
    }
    return {x, y};

}

vector<Point> transformVertices(const vector<Point>& vertices, const float& width, const float& height, const Point& origin, const string& orientation) {
    vector<Point> newVertices;

    for (const auto& vertex : vertices) {
        Point transformedVertex = vertex;
        Point newVertex = vertex;
        if (orientation == "N") {
            // No rotation
            newVertex.x = origin.x + transformedVertex.x;
            newVertex.y = origin.y + transformedVertex.y;

        } else if (orientation == "S") {
            newVertex = rotatePoint(vertex, 180, width, height);
            newVertex.x = newVertex.x + origin.x;
            newVertex.y = newVertex.y + origin.y;

        } else if (orientation == "W") {
            newVertex = rotatePoint(vertex, 90, width, height);
            newVertex.x = newVertex.x + origin.x;
            newVertex.y = newVertex.y + origin.y;

        } else if (orientation == "E") {
            newVertex = rotatePoint(vertex, 270, width, height);
            newVertex.x = newVertex.x + origin.x;
            newVertex.y = newVertex.y + origin.y;

        } else if (orientation == "FN") {
            newVertex = reflectPoint(vertex, true, width, height, false);
            newVertex.x = newVertex.x + origin.x;
            newVertex.y = newVertex.y + origin.y;

        } else if (orientation == "FS") {
            newVertex = reflectPoint(vertex, false, width, height, false);
            newVertex.x = newVertex.x + origin.x;
            newVertex.y = newVertex.y + origin.y;

        } else if (orientation == "FW") {
            newVertex = reflectPoint(rotatePoint(vertex, 90, width, height), false, width, height, true);
            newVertex.x = newVertex.x + origin.x;
            newVertex.y = newVertex.y + origin.y;

        } else if (orientation == "FE") {
            newVertex = reflectPoint(rotatePoint(vertex, 270, width, height), true, width, height, true);
            newVertex.x = newVertex.x + origin.x;
            newVertex.y = newVertex.y + origin.y;
        }
        newVertices.push_back(newVertex);
    }

    return newVertices;
}



void updateBlocksWithVertices(vector<Block>& blocks, const vector<Component>& components, const vector<OnlyBlock>& onlyblocks) {
    for (const auto& component : components) {
        auto it_block = find_if(blocks.begin(), blocks.end(), [&](const Block& block) { return block.block_name == component.name; });
        if (it_block != blocks.end()) {
            auto it_onlyblock = find_if(onlyblocks.begin(), onlyblocks.end(), [&](const OnlyBlock& ob) { return ob.name == component.blocktype; });
            if (it_onlyblock != onlyblocks.end()) {
                it_block->position = {component.x, component.y};
                it_block->vertices = transformVertices(it_onlyblock->vertices, it_onlyblock->width, it_onlyblock->height, it_block->position, component.orientation);
            }
        }
    }
}



