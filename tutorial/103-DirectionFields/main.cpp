//
// Created by arya on 2024/2/4.
//


#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/surface_mesh.h"
#include "geometrycentral/surface/direction_fields.h"

#include "args.hxx"

using namespace geometrycentral;
using namespace geometrycentral::surface;

int main(int argc, char** argv) {
    args::ArgumentParser parser("");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    args::Positional<std::string> inputFilename(parser, "mesh", "A surface mesh file");

    try {
        parser.ParseCLI(argc, argv);
    } catch (args::Help&) {
        std::cout << parser;
        return 0;
    } catch (args::ParseError& e) {
        std::cerr << e.what() << std::endl;
        std::cerr << parser;
        return 1;
    }

    if (!inputFilename) {
        std::cerr << "Please specify a mesh file" << std::endl;
        std::cerr << parser;
        return 1;
    }

    auto [mesh, geometry] = readManifoldSurfaceMesh(args::get(inputFilename));

    // Get vertex tangent basis
    geometry->requireVertexTangentBasis();
    VertexData<Vector3> vBasisX(*mesh);
    for(auto v : mesh->vertices()) {
        vBasisX[v] = geometry->vertexTangentBasis[v].front();
    }

    VertexData<Vector2> field = computeSmoothestVertexDirectionField(*geometry);
}