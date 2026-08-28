/*
Compiler instruction: g++ 07_OversetMesh.cpp src/*.cpp -Iinclude -std=c++17 -o OversetMesh && ./OversetMesh
*/

#include<iostream>
#include<stdlib.h>
#include<cassert>
#include "MultiBlockMesh.h"
#include "MeshWriter.h"
#include "Block.h"
#include "Surface.h"
#include "Line.h"

int main(){
	std::cout<<"starting overset and animation\n";
	MultiBlockMesh oversetMesh;
	Point p0(0, -10.0, -10.0);
	Point p1(1,  10.0, -10.0);
	Point p2(2,  10.0,  10.0);
	Point p3(3, -10.0,  10.0);
    	Line l0(0, &p0, &p1);
    	Line l1(1, &p1, &p2);
    	Line l2(2, &p2, &p3);
    	Line l3(3, &p3, &p0);

    	Surface coarseSurface(0);
    	coarseSurface.addBoundary(&l0);
    	coarseSurface.addBoundary(&l1);
    	coarseSurface.addBoundary(&l2);
    	coarseSurface.addBoundary(&l3);
	Block coarseBlock(0, coarseSurface, 10, 10);

        Point p4(4,0.0,0.0);
        Point p5(5,5.0,0.0);
        Point p6(6,5.0,5.0);
        Point p7(7,0.0,5.0);
        Line l4(4,&p4,&p5);
        Line l5(5,&p5,&p6);
        Line l6(6,&p6,&p7);
        Line l7(7,&p7,&p4);
    
        Surface fineSurface(1);
    
        fineSurface.addBoundary(&l4);
        fineSurface.addBoundary(&l5);
        fineSurface.addBoundary(&l6);
        fineSurface.addBoundary(&l7);

        Block fineBlock(1,fineSurface,50,50);


	oversetMesh.addBlock(fineBlock);
	oversetMesh.addBlock(coarseBlock);

        oversetMesh.generate();

	OversetConfig config;
	config.overset=true;
        config.fineMeshIdx=0;
	config.motionType="circular";
	config.vx=0.5;
	config.vy=0.2;
	int totalFrames=200;
        config.xc=1.0;
        config.yc=1.0;
        config.omega=5.0;
	double dt=0.1;

        system("mkdir oversetMesh_frames");

	for(int frame=0;frame<totalFrames;++frame){
		double t=frame*dt;
                oversetMesh.generate(config,dt);
		std::string filename="oversetMesh_frames/output_frame_"+std::to_string(frame)+".vtk";
		MeshWriter::writeVTK(oversetMesh,filename);
		std::cout<<"Generated and saved:"<<filename<<" at t="<<t<<"\n";

      }
      std::cout<<"Animation files generated\n";
      return 0;
}
