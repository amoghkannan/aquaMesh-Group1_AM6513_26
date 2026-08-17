#ifndef MESH_H
#define MESH_H

#include <vector>
#include<memory>

#include "Point.h"
#include "Cell.h"
#include "VTKTriangle.h"
#include "Surface.h"

class Mesh
{
public:

    //--------------------------------------------------
    // Data
    //--------------------------------------------------

    std::vector<Point> nodes;
    std::vector<Cell*> cells;
    bool isTriangle; //After triangle cell split, will be true

    double xmin;
    double xmax;

    double ymin;
    double ymax;

    int Nx;
    int Ny;

    double dx;
    double dy;

    //--------------------------------------------------
    // Constructor
    //--------------------------------------------------

    Mesh();
    Mesh(const Mesh&);

    //--------------------------------------------------
    // Mesh Generation
    //--------------------------------------------------

    void generateCartesian
    (
        const Surface& surface,
        int Nx_,
        int Ny_
    );

    //--------------------------------------------------
    // Utility Functions
    //--------------------------------------------------

    int getNumberOfNodes() const;

    int getNumberOfCells() const;

    void splitIntoTriangles();
    double triangleArea(int n1, int n2, int n3) const;
    //Needed to split cells into triangles 

    ~Mesh(); //Destructor

protected:

    void computeBoundingBox(const Surface& surface);
    void computeBoundingBox();

private:

    void generateNodes();
    void generateCells();
};

#endif
