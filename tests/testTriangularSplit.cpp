/*
Compiler instruction: g++ 05_TriangularMeshExport.cpp src/*.cpp -Iinclude -std=c++17 -o TriangularMeshExport && ./TriangularMeshExport
*/

#include <iostream>

#include"Test.h"

#include "Point.h"
#include "Line.h"
#include "Surface.h"

#include "Mesh.h"
#include "MeshStatistics.h"
#include "MeshWriter.h"

int main()
{

    Test test;

    Point p0(0,0.0,0.0);
    Point p1(1,4.0,0.0);
    Point p2(2,4.0,2.0);
    Point p3(3,0.0,2.0);

    Line l0(0,&p0,&p1);
    Line l1(1,&p1,&p2);
    Line l2(2,&p2,&p3);
    Line l3(3,&p3,&p0);

    Surface rectangle(0);

    rectangle.addBoundary(&l0);
    rectangle.addBoundary(&l1);
    rectangle.addBoundary(&l2);
    rectangle.addBoundary(&l3);

    //------------------------------------------------------------
    // Step 2 : Generate Cartesian Mesh
    //------------------------------------------------------------

    Mesh mesh;

    mesh.generateCartesian
    (
        rectangle,
        20,     // Nx
        10      // Ny
    );

    int numberOfCells=mesh.getNumberOfCells();

    int temp;
    //Mess up node ordering, to check whether triangles still made properly
    temp=mesh.cells[79]->nodeIDs[2];
    mesh.cells[79]->nodeIDs[2]=mesh.cells[79]->nodeIDs[1];
    mesh.cells[79]->nodeIDs[1]=temp;
    int n1,n2,n3,n4;
    n1=mesh.cells[79]->nodeIDs[0];
    n2=mesh.cells[79]->nodeIDs[1];
    n3=mesh.cells[79]->nodeIDs[2];
    n4=mesh.cells[79]->nodeIDs[3];

    mesh.splitIntoTriangles();
   
    int numberOfTriangles=mesh.getNumberOfCells();

    //For cell with messed up ordering, are correct triangles still found?
    test.expectTrue(mesh.cells[79]->nodeIDs[0]==n1 &&
                    mesh.cells[79]->nodeIDs[1]==n2 &&
                    mesh.cells[79]->nodeIDs[2]==n3,
                     "Correct triangles with mangled node ordering");

    test.expectTrue(mesh.cells[79+numberOfCells]->nodeIDs[0]==n1 &&
                    mesh.cells[79+numberOfCells]->nodeIDs[1]==n2 &&
                    mesh.cells[79+numberOfCells]->nodeIDs[2]==n4,
                     "Correct triangles with mangled node ordering");

    test.expectEqual(2*numberOfCells,
                     numberOfTriangles,
                     1E-12,
                     "2 triangles per cell");

    int wrong_triangles=0;
    
    for(auto triangle:mesh.cells){
                if(triangle->nodeIDs[0]==triangle->nodeIDs[1] ||
                   triangle->nodeIDs[0]==triangle->nodeIDs[2] ||
                   triangle->nodeIDs[1]==triangle->nodeIDs[2])
                   wrong_triangles=wrong_triangles+1;
    };

    test.expectEqual(wrong_triangles,
                     0,
                     1E-12,
                     "Unique nodes for a triangle");

    wrong_triangles=0;
    
    for(auto triangle:mesh.cells){
                if(triangle->nodeIDs.size()!=3)
                   wrong_triangles=wrong_triangles+1;
    };

    test.expectEqual(wrong_triangles,
                     0,
                     1E-12,
                     "3 nodes for a triangle");


    test.summary();

    return test.success() ? 0 : 1;


return 0;





};
