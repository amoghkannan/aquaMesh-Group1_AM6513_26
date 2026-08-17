//------------------------------------------------------------//
// File: testOversetMesh.cpp
//
// Regression Tests
//
// Overset Mesh
//
//------------------------------------------------------------//

#include <iostream>
#include<stdlib.h>
#include<cmath>
#include<cassert>
#include "MultiBlockMesh.h"
#include "MeshWriter.h"
#include "Block.h"
#include "Surface.h"
#include "Line.h"

#include "Test.h"

using namespace std;

int main()
{
    cout << "=========================================\n";
    cout << " Running Overset Mesh Tests\n";
    cout << "=========================================\n\n";

    Test test;
    
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

    oversetMesh.addBlock(coarseBlock);
    oversetMesh.addBlock(fineBlock);

    oversetMesh.generate();

    OversetConfig config;
    config.overset=true;
    config.fineMeshIdx=1;
    config.motionType="linear";
    config.vx=0.5;
    config.vy=0.2;
    int totalFrames=1;
    config.xc=1.0;
    config.yc=1.0;
    config.omega=5.0;
    double dt=0.1;

    double x1,y1;
    double x2,y2;
    double x3,y3;
    double x4,y4;
    double t;
    double delta1,delta2;

    delta1=sqrt(
           pow(oversetMesh.blocks[1].mesh.nodes[39].x-oversetMesh.blocks[1].mesh.nodes[287].x,2)+
           pow(oversetMesh.blocks[1].mesh.nodes[39].y-oversetMesh.blocks[1].mesh.nodes[287].y,2));

    
    x1=oversetMesh.blocks[1].mesh.nodes[20].x;
    y1=oversetMesh.blocks[1].mesh.nodes[20].y;

    x2=oversetMesh.blocks[0].mesh.nodes[10].x;
    y2=oversetMesh.blocks[0].mesh.nodes[10].y;

    for(int frame=0;frame<totalFrames;++frame){
                oversetMesh.generate(config,dt);
    }

    t=totalFrames*dt;
    x3=oversetMesh.blocks[1].mesh.nodes[20].x;
    y3=oversetMesh.blocks[1].mesh.nodes[20].y;

    x4=oversetMesh.blocks[0].mesh.nodes[10].x;
    y4=oversetMesh.blocks[0].mesh.nodes[10].y;

    test.expectEqual(x2,x4,1E-12,"Background mesh stays stationary");
    test.expectEqual(y2,y4,1E-12,"Background mesh stays stationary");

    test.expectEqual(x3-x1,config.vx*t,1E-12,"Fine mesh moves correctly in line");
    test.expectEqual(y3-y1,config.vy*t,1E-12,"Background mesh moves correctly in line");

    config.motionType="circular";

    x1=oversetMesh.blocks[1].mesh.nodes[40].x;
    y1=oversetMesh.blocks[1].mesh.nodes[40].y;

    x2=oversetMesh.blocks[0].mesh.nodes[30].x;
    y2=oversetMesh.blocks[0].mesh.nodes[30].y;

    for(int frame=0;frame<totalFrames;++frame){
                oversetMesh.generate(config,dt);
    }

    x3=oversetMesh.blocks[1].mesh.nodes[40].x;
    y3=oversetMesh.blocks[1].mesh.nodes[40].y;

    x4=oversetMesh.blocks[0].mesh.nodes[30].x;
    y4=oversetMesh.blocks[0].mesh.nodes[30].y;

    test.expectEqual(x2,x4,1E-12,"Background mesh stays stationary");
    test.expectEqual(y2,y4,1E-12,"Background mesh stays stationary");

    t=totalFrames*dt;

    double r1,r2,dot;

    r1=sqrt(pow(x3-config.xc,2)+pow(y3-config.yc,2));
    r2=sqrt(pow(x1-config.xc,2)+pow(y1-config.yc,2));
    test.expectEqual(r1,r2,1E-12,"Fine mesh moves correctly in circle");
    dot=(x1-config.xc)*(x3-config.xc)+(y1-config.yc)*(y3-config.yc);
    dot=dot/pow(r1,2);
    test.expectEqual(dot,cos(config.omega*M_PI*t/180.0),1E-12,"Fine mesh moves correctly in circle");

    delta2=sqrt(
           pow(oversetMesh.blocks[1].mesh.nodes[39].x-oversetMesh.blocks[1].mesh.nodes[287].x,2)+
           pow(oversetMesh.blocks[1].mesh.nodes[39].y-oversetMesh.blocks[1].mesh.nodes[287].y,2));

    test.expectEqual(delta1,delta2,1E-12,"Rigid motion: distances b/w mesh points does not change");

    test.summary();

    return test.success() ? 0 : 1;
}
