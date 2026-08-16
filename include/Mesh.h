#ifndef MESH_H
#define MESH_H

#include <vector>

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
    std::vector<Cell> cells;
    std::vector<VTKTriangle> triangles;

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

    int getNumberOfTriangles() const;

    void splitIntoTriangles();
    double triangleArea(int n1, int n2, int n3) const;
    //Needed to split cells into triangles 

protected:

    void computeBoundingBox(const Surface& surface);
    void computeBoundingBox();

private:

    void generateNodes();
    void generateCells();
};

#endif
