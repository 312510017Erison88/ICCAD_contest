#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "nlohmann/json.hpp"
#include "readfile.h"

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

// void readDefFile(const std::string& defFilePath, std::vector<Component>& components) {
//     std::ifstream defFile(defFilePath);
//     std::string line;

//     while (std::getline(defFile, line)) {
//         if (line.find("COMPONENTS") != std::string::npos) {
//             break;
//         }
//     }

//     while (std::getline(defFile, line)) {
//         if (line.find("END COMPONENTS") != std::string::npos) {
//             break;
//         }

//         if (line[0] == '-') {
//             Component comp;
//             std::istringstream iss(line);
//             std::string temp;
//             iss >> temp >> comp.name >> comp.macro >> temp >> temp >> comp.x >> comp.y >> comp.orientation;
//             components.push_back(comp);
//         }
//     }
// }

