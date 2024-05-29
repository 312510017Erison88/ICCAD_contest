#include <iostream>
#include <string>
#include <vector>
#include "readfile.h"

using namespace std;


int main() {
    vector<Block> blocks;
    vector<Net> nets;
    vector<Component> components;

    // file path
    string blockFilePath = "case4/case4_cfg.json";
    string netFilePath = "case4/case4.json";
    string defFilePath = "chip_top.def";

    readJsonFiles(blockFilePath, netFilePath, blocks, nets);

    // readDefFile(defFilePath, components);
    
    // for (const auto& block : blocks) {
    //     cout << "Block Name: " << block.block_name << endl;
    //     cout << "Block through_block_net_num: " << block.through_block_net_num << endl;
    //     for(const auto& index : block.block_port_region){
    //         cout << "Block_port_region: " << index << endl;
    //     }
    // }
    

    for (const auto& net : nets) {
        cout << "Net ID: " << net.ID << endl;
        cout << "TX: " << net.TX << endl;

        // Print all RX values
        cout << "RX:";
        for (const auto& rxValue : net.RX) {
            cout << " " << rxValue;
        }
        cout << endl;

        cout << "NUM: " << net.NUM << endl;

        // Print MUST_THROUGH
        cout << "MUST_THROUGH:" << endl;
        for (const auto& pair : net.MUST_THROUGH) {
            cout << pair.first << ":";
            for (const auto& value : pair.second) {
                cout << " " << value;
            }
            cout << endl;
        }

        // Print HMFT_MUST_THROUGH
        cout << "HMFT_MUST_THROUGH:" << endl;
        for (const auto& pair : net.HMFT_MUST_THROUGH) {
            cout << pair.first << ":";
            for (const auto& value : pair.second) {
                cout << " " << value;
            }
            cout << endl;
        }

        // Print TX_COORD
        cout << "TX_COORD:";
        for (const auto& coord : net.TX_COORD) {
            cout << " " << coord;
        }
        cout << endl;

        // Print RX_COORD
        cout << "RX_COORD:" << endl;
        for (const auto& coordList : net.RX_COORD) {
            cout << "[";
            for (const auto& coord : coordList) {
                cout << " " << coord;
            }
            cout << "]" << endl;
        }

        // Add a separator between Net objects
        cout << "---------------------------------------" << endl;
    }

    // for (const auto& comp : components) {
    //     cout << "Component Name: " << comp.name << " at (" << comp.x << ", " << comp.y << ")" << endl;
    // }

    return 0;
}

