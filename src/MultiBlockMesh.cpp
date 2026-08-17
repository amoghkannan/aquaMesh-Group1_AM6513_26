#include "MultiBlockMesh.h"

#include <iostream>

using namespace std;

MultiBlockMesh::MultiBlockMesh()
{
}

void MultiBlockMesh::addBlock(const Block& block)
{
    blocks.push_back(block);
}

void MultiBlockMesh::generate()
{
    //----------------------------------------------------
    // Clear existing mesh
    //----------------------------------------------------

    nodes.clear();
    cells.clear();

    int globalNodeID = 0;
    int globalCellID = 0;

    //----------------------------------------------------
    // Loop over all blocks
    //----------------------------------------------------

    for(auto& block : blocks)
    {
        //--------------------------------------------
        // Generate block mesh
        //--------------------------------------------

        block.generateMesh();

        //--------------------------------------------
        // Store node offset
        //--------------------------------------------

        int nodeOffset = globalNodeID;

        //--------------------------------------------
        // Copy nodes
        //--------------------------------------------

        for(auto node : block.mesh.nodes)
        {
            node.id = globalNodeID++;

            nodes.push_back(node);
        }

        //--------------------------------------------
        // Copy cells
        //--------------------------------------------

        for(auto cell : block.mesh.cells)
        {
            cell->id = globalCellID++;

            for(auto& nodeID : cell->nodeIDs)
            {
                nodeID += nodeOffset;
            }

            cells.push_back(cell);
        }
    }

    //----------------------------------------------------
    // Compute Global Bounding Box
    //----------------------------------------------------

    computeBoundingBox();

    cout << "\n";
    cout << "=====================================\n";
    cout << "Multi Block Mesh Generated\n";
    cout << "=====================================\n";

    cout << "Blocks : "
         << blocks.size()
         << endl;

    cout << "Nodes : "
         << nodes.size()
         << endl;

    cout << "Cells : "
         << cells.size()
         << endl;
};

MultiBlockMesh::~MultiBlockMesh(){

};

void MultiBlockMesh::generate(const OversetConfig& config, double dt){
        if (config.overset){


                if (config.motionType!="linear"&& config.motionType!="circular")
                {
                        throw std::invalid_argument("Error:Invalid motion type");
		};

                if (config.fineMeshIdx<0 || config.fineMeshIdx > blocks.size()-1)
                {
                        throw std::invalid_argument("Error:Invalid index of fine mesh");
		};

                if(config.motionType=="linear"){
                        MeshTransformation::translate(blocks[config.fineMeshIdx].mesh,config.vx*dt,config.vy*dt);
                };

                if(config.motionType=="circular"){
                        MeshTransformation::rotate(blocks[config.fineMeshIdx].mesh,config.xc,config.yc,config.omega*dt);
                };

                
                int globalNodeOffset=0;

                for(int i=0;i<config.fineMeshIdx;i++){
                        globalNodeOffset=globalNodeOffset+blocks[i].mesh.nodes.size();

                };

                int meshNodeCount=blocks[config.fineMeshIdx].mesh.getNumberOfNodes();

                for(int i=0;i<meshNodeCount;i++){
                        nodes[i+globalNodeOffset].x = blocks[config.fineMeshIdx].mesh.nodes[i].x;
                        nodes[i+globalNodeOffset].y = blocks[config.fineMeshIdx].mesh.nodes[i].y;
                };
        };

};
