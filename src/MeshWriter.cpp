//------------------------------------------------------------//
// File: MeshWriter.cpp
//
// Description:
//
// Exports a structured mesh to Legacy VTK format.
//
//------------------------------------------------------------//

#include "MeshWriter.h"

#include <fstream>
#include <iostream>

using namespace std;

void MeshWriter::writeVTK
(
    const Mesh& mesh,
    const string& filename,
    bool writejacobianfield
)
{
    ofstream file(filename);

    if(!file.is_open())
    {
        cout << "Error opening " << filename << endl;
        return;
    }

    //--------------------------------------------------------
    // Header
    //--------------------------------------------------------

    file << "# vtk DataFile Version 3.0\n";
    file << "AquaMesh\n";
    file << "ASCII\n";
    file << "DATASET UNSTRUCTURED_GRID\n\n";

    //--------------------------------------------------------
    // Points
    //--------------------------------------------------------

    file << "POINTS "
         << mesh.getNumberOfNodes()
         << " float\n";

    for(const auto& node : mesh.nodes)
    {
        file << node.x
             << " "
             << node.y
             << " "
             << node.z
             << "\n";
    }

    file << "\n";

    //--------------------------------------------------------
    // Cells
    //--------------------------------------------------------

    int nCells = mesh.getNumberOfCells();

    int nNodesPerCell=mesh.isTriangle?3:4;
    int cellType=mesh.isTriangle?5:9;

    file << "CELLS "
         << nCells
         << " "
         << nCells*(nNodesPerCell+1)
         << "\n";

    for(const auto& cell : mesh.cells)
    {
        file << nNodesPerCell<<" ";

        for(int node : cell->nodeIDs)
        {
            file << node << " ";
        }

        file << "\n";
    }

    file << "\n";

    //--------------------------------------------------------
    // Cell Types
    //--------------------------------------------------------

    file << "CELL_TYPES "
         << nCells
         << "\n";

    for(int i=0;i<nCells;i++)
    {
        file << cellType<<std::endl;
    }


    if (writejacobianfield)
    {
	file<<"\n";
	file<<"CELL_DATA "<<nCells<<"\n";
	file<<"SCALARS Jacobian float 1\n";
	file<<"LOOKUP_TABLE default\n";
	for(double jacob:mesh.J)
	{
		file<<jacob<<"\n";
	}
    }
    file.close();

    cout << "\n";
    cout << "Mesh exported to "
         << filename
         << endl;
};
