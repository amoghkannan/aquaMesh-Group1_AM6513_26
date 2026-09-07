#include "CoordinateMapping.h"

#include <cmath>

using namespace std;

//------------------------------------------------------------//
// Identity Mapping
//------------------------------------------------------------//

void CoordinateMapping::identity(Mesh& mesh)
{
    // No change
}

//------------------------------------------------------------//
// Trapezoidal Mapping
//------------------------------------------------------------//

void CoordinateMapping::trapezoidal
(
    Mesh& mesh,
    double topScale
)
{
    if(mesh.isCurvilinear){
        std::cout<<"Warning: Mesh already transformed, no change"<<std::endl;
        return;
    };

    double ymin = mesh.ymin;
    double ymax = mesh.ymax;

    for(auto& node : mesh.nodes)
    {
        double eta = (node.y-ymin)/(ymax-ymin);

        node.x *= (1.0 + eta*(topScale-1.0));
    }

    mesh.isCurvilinear=true;

    mesh.computeJacobians();

}

//------------------------------------------------------------//
// Sinusoidal Mapping
//------------------------------------------------------------//

void CoordinateMapping::sinusoidal
(
    Mesh& mesh,
    double amplitude
)
{
    if(mesh.isCurvilinear){
        std::cout<<"Warning: Mesh already transformed, no change"<<std::endl;
        return;
    };

    const double pi = acos(-1.0);

    double xmin = mesh.xmin;
    double xmax = mesh.xmax;

    double L = xmax-xmin;

    for(auto& node : mesh.nodes)
    {
        node.y += amplitude*
                  sin(2.0*pi*(node.x-xmin)/L);
    }

    mesh.isCurvilinear=true;

    mesh.computeJacobians();

}

//------------------------------------------------------------//
// Polar Mapping
//------------------------------------------------------------//

void CoordinateMapping::polar
(
    Mesh& mesh,
    double rInner,
    double rOuter
)
{
    if(mesh.isCurvilinear){
        std::cout<<"Warning: Mesh already transformed, no change"<<std::endl;
        return;
    };

    double xmin = mesh.xmin;
    double xmax = mesh.xmax;

    double ymin = mesh.ymin;
    double ymax = mesh.ymax;

    for(auto& node : mesh.nodes)
    {
        double xi =
            (node.x-xmin)/(xmax-xmin);

        double eta =
            (node.y-ymin)/(ymax-ymin);

        double r =
            rOuter +
            eta*(-rOuter+rInner);

        double theta =
            xi*2.0*acos(-1.0);

        node.x =
            r*cos(theta);

        node.y =
            r*sin(theta);
    }

    mesh.isCurvilinear=true;

    mesh.computeJacobians();

}

void CoordinateMapping::genericCurvilinear(Mesh& mesh, std::pair<double,double>(*transformation)(double,double)){
   
    if(mesh.isCurvilinear){
        std::cout<<"Warning: Mesh already transformed, no change"<<std::endl;
        return;
    };

    std::pair<double,double> coordNew;
  
    for(auto& node : mesh.nodes)
    {
        double xi = node.x;

        double eta = node.y;

        coordNew=transformation(xi,eta);

        node.x=coordNew.first;
        node.y=coordNew.second;

    }

    mesh.isCurvilinear=true;
    
    mesh.computeJacobians();

};
