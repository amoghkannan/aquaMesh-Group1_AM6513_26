#ifndef VTKTRIANGLE_H
#define VTKTRIANGLE_H

#include<vector>
#include<string.h>

class VTKTriangle{

public:
        int id;
        std::vector<int> nodeIDs;

        VTKTriangle();


        VTKTriangle(
                int id_,
                int n1,
                int n2,
                int n3);

        void print() const;

};
#endif
