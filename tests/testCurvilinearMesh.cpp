#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <fstream>
#include <string>

#include "Point.h"
#include "Line.h"
#include "Surface.h"
#include "Mesh.h"
#include "CoordinateMapping.h"
#include "MeshWriter.h"
#include "Test.h"

using namespace std;

double A_val;
std::pair<double,double> transform1(double xi, double eta)
{
    return { xi, eta*(1.0+A_val*sin(M_PI*xi)) };
}

double B_val, C_val;
std::pair<double,double> transform2(double xi, double eta)
{
    return { xi+B_val*eta*(1.0-eta), eta+C_val*sin(M_PI*xi)*eta };
}

int main()
{
	Test test;
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

	Mesh mesh1;
	mesh1.generateCartesian(rectangle,40,40);
	A_val=0.5;
	bool threw1=false;
	try{CoordinateMapping::genericCurvilinear(mesh1,transform1);}
	catch(const std::exception&){threw1=true;}
	test.expectTrue(!threw1,"Transform1: A=0.5 is valid");
	MeshWriter::writeVTK(mesh1,"case1_transform1_valid.vtk",true);


        Mesh mesh2;
	mesh2.generateCartesian(rectangle,40,40);
	A_val=-2.0;
	bool threw2=false;
	try{CoordinateMapping::genericCurvilinear(mesh2,transform1);}
	catch(const std::exception&){threw2=true;}
	test.expectTrue(threw2,"Transform1: A=-2.0 is invalid");
	MeshWriter::writeVTK(mesh2,"case2_transform1_invalid.vtk",true);


	Mesh mesh3;
	mesh3.generateCartesian(rectangle,60,60);
	B_val=0.5;C_val=0.5;
	bool threw3=false;
	try{CoordinateMapping::genericCurvilinear(mesh3,transform2);}
	catch(const std::exception&){threw3=true;}
	test.expectTrue(!threw3,"Transform2: BC=0.25 is valid");
	MeshWriter::writeVTK(mesh3,"case3_transform2_valid.vtk",true);

	Mesh mesh4;
	mesh4.generateCartesian(rectangle,60,60);
	B_val=0.65;C_val=0.65;
	bool threw4=false;
	try{CoordinateMapping::genericCurvilinear(mesh4,transform2);}
	catch(const std::exception&){threw4=true;}
	test.expectTrue(threw4,"Transform2: BC=0.4225 is invalid");
	MeshWriter::writeVTK(mesh4,"case4_transform2_invalid.vtk",true);

	ifstream vtkFile("case4_transform2_invalid.vtk");
	string content((istreambuf_iterator<char>(vtkFile)),istreambuf_iterator<char>());
	test.expectTrue
	(
		content.find("SCALARS Jacobian")!=string::npos,"Visualization file contains jacobian field"
	);
	test.summary();
	return test.success()? 0 :1;
}






