#ifndef VTKTRIANGLE_H
#define VTKTRIANGLE_H

#include"Cell.h"

class VTKTriangle: public Cell{

public:

        VTKTriangle();

        VTKTriangle(
                int id_,
                int n1,
                int n2,
                int n3
                );

        virtual void print() const;
        virtual ~VTKTriangle() {};

};
#endif
