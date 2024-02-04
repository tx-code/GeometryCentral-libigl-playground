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

    std::cout << "Before split and flip: \n";
    mesh->printStatistics();

    VertexData<bool> isOrigVert(*mesh, true);
    EdgeData<bool> isOrigEdge(*mesh, true);

    // Split all edges
    std::vector<Edge> toFlip;
    for (auto e: mesh->edges()) {
        if (!isOrigEdge[e]) continue;

        auto oldA = e.halfedge().tipVertex();
        auto oldB = e.halfedge().tailVertex();
        auto oldAPos = geometry->inputVertexPositions[oldA];
        auto oldBPos = geometry->inputVertexPositions[oldB];

        // Split the edge
        auto newV = mesh->splitEdgeTriangular(e).vertex();
        isOrigVert[newV] = false;

        // position the new vertex
        auto newPos = 0.5 * (oldAPos + oldBPos);
        geometry->inputVertexPositions[newV] = newPos;

        // make the new edges
        for (auto edge: newV.adjacentEdges()) {
            isOrigEdge[edge] = false;
            if (auto otherV = edge.otherVertex(newV); isOrigVert[otherV] && otherV != oldA && otherV != oldB) {
                toFlip.push_back(edge);
            }
        }
    }

    for (auto e: toFlip) {
        mesh->flip(e);
    }

    std::cout << "\n\nAfter split and flip: \n";
    mesh->printStatistics();
}
