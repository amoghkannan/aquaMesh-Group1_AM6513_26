/*
Compiling Instruction: g++ 08_CurvilinearMesh.cpp src/*.cpp -Iinclude -std=c++17 -o curvilinearMesh && ./curvilinearMesh
*/
#define _USE_MATH_DEFINES
#include <iostream>
#include<cmath>

#include "Point.h"
#include "Line.h"
#include "Surface.h"
#include "CoordinateMapping.h"


#include "Mesh.h"
#include "MeshStatistics.h"
#include "MeshWriter.h"

double A=1.0;
double B=1.0;
double C=1.0;

std::pair<double,double>transform1(double xi, double eta){
        std::pair<double,double> ans={xi,eta*(1.0+A*sin(M_PI*xi))};
        return ans;
};

std::pair<double,double>transform2(double xi, double eta){
        std::pair<double,double> ans={xi+B*eta*(1.0-eta),eta+C*sin(M_PI*xi)*eta};
        return ans;
};

void runCase
(
	const std::string& caseName,
	std::pair<double,double>(*transform)(double,double),
	const std::string& outputFile
)

{
    std::cout << "==========================================" << std::endl;
    std::cout << " "<< caseName << std::endl;
    std::cout << "==========================================" << std::endl;

    //------------------------------------------------------------
    // Step 1 : Create Geometry
    //------------------------------------------------------------

    Point p0(0,0.0,0.0);
    Point p1(1,1.0,0.0);
    Point p2(2,1.0,1.0);
    Point p3(3,0.0,1.0);

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
    // Print Geometry
    //------------------------------------------------------------

    std::cout << "\n";
    std::cout << "Geometry Created" << std::endl;
    std::cout << "----------------" << std::endl;

    rectangle.print();

    //------------------------------------------------------------
    // Step 2 : Generate Cartesian Mesh
    //------------------------------------------------------------

    Mesh mesh;

    mesh.generateCartesian
    (
        rectangle,
        200,     // Nx
        100      // Ny
    );

    try
    {
         CoordinateMapping::genericCurvilinear(mesh,transform);
	 std::cout<<"Mapping is valid: all cells have positive Jacobian.\n";
	 MeshStatistics::print(mesh);
	 MeshWriter::writeVTK(mesh,outputFile,true);
	 std::cout<<"Mesh exported to"<<outputFile<<std::endl;
    }
    catch(const std::exception& e)
    {
	std::cout<<"\nMesh generatio failed:"<<e.what()<<std::endl;
	MeshWriter::writeVTK(mesh,"invalidRegion_"+outputFile,true);
	std::cout<<"Problematic region written to invalidRegion_"<< outputFile <<"(colour by 	'Jacobian')"<<std::endl;
    }
}

int main()
{
	runCase("Transform 1 : y = eta*(1+A*sin(pi*xi))", transform1, "curvilinearMesh_t1.vtk");
        runCase("Transform 2 : x = xi+B*eta*(1-eta), y = eta+C*sin(pi*xi)*eta", transform2, "curvilinearMesh_t2.vtk");
	return 0;
}
