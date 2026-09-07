#ifndef MESHTRANSFORMATION_H
#define MESHTRANSFORMATION_H

#include "Mesh.h"
#define _USE_MATH_DEFINES

class MeshTransformation
{
public:

    //--------------------------------------------------
    // Rigid Body Transformations
    //--------------------------------------------------

    static void translate
    (
        Mesh& mesh,
        double dx,
        double dy
    );

    static void rotate
    (
        Mesh& mesh,
        double xc,
        double yc,
        double angleDeg
    );

    //--------------------------------------------------
    // Affine Transformations
    //--------------------------------------------------

    static void scale
    (
        Mesh& mesh,
        double sx,
        double sy
    );

    static void shear
    (
        Mesh& mesh,
        double kx,
        double ky
    );

    //--------------------------------------------------
    // Mesh Morphing
    //--------------------------------------------------

    static void sinusoidal
    (
        Mesh& mesh,
        double amplitude,
        double wavelength
    );

};

#endif
