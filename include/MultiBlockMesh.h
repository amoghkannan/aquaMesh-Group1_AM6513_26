#ifndef MULTIBLOCKMESH_H
#define MULTIBLOCKMESH_H

#include <vector>

#include "Block.h"
#include "Mesh.h"
#include "MeshTransformation.h"

struct OversetConfig{
	bool overset=false;
        int fineMeshIdx=-1;
	std::string motionType="linear";
	double vx=1.0,vy=0.0;
	double omega=0.5;
	double xc=0.0,yc=0.0;
};

class MultiBlockMesh : public Mesh
{
public:

    std::vector<Block> blocks;

    MultiBlockMesh();

    void addBlock(const Block& block);

    void generate();
    void generate(const OversetConfig& config,double dt);	

    virtual ~MultiBlockMesh();

};

#endif
