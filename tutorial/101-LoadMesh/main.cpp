//
// Created by arya on 2024/2/4.
//

#include "geometrycentral/surface/meshio.h"
#include "geometrycentral/surface/manifold_surface_mesh.h"
#include "geometrycentral/surface/surface_mesh.h"

#include "args.hxx"

using namespace geometrycentral;
using namespace geometrycentral::surface;

int main(int argc, char** argv) {
    // Configure a simple argument parser
    args::ArgumentParser parser("");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    args::Positional<std::string> inputFilename(parser, "mesh", "A surface mesh file");

    // Parse args
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

    // load a general surface mesh from file
    auto [mesh, geometry] = readSurfaceMesh(args::get(inputFilename));

    // Print some information about the mesh.
    mesh->printStatistics();

    // Iterate through the vertices of the mesh, printing the degree of each and the incident faces
    for(auto v : mesh->vertices()) {
        std::cout << "Vertex " << v << " has degree " << v.degree() << "\n";
        for(auto f : v.adjacentFaces()) {
            std::cout << " incident on face " << f << "\n";
        }
    }

    return EXIT_SUCCESS;
}
