#include"VTKTriangle.h"

#include<iostream>
#include<fstream>

VTKTriangle::VTKTriangle(){

        id=-1;
};

VTKTriangle::VTKTriangle
(
        int id_,
        int n1,
        int n2,
        int n3
)
{
        id=id_;

        nodeIDs.push_back(n1);
        nodeIDs.push_back(n2);
        nodeIDs.push_back(n3);
};

void VTKTriangle::print() const
{
    std::cout << "Triangle " << id << " : ";

    for (int node : nodeIDs)
    {
        std::cout << node << " ";
    }

    std::cout << std::endl;
}


