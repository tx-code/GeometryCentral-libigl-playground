//
// Created by arya on 2024/2/4.
//

#include <igl/readOFF.h>
#include <igl/opengl/glfw/Viewer.h>

#include "args.hxx"

int main(int argc, char** argv) {
    args::ArgumentParser parser("");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    args::Positional<std::string> inputFilename(parser, "mesh", "A surface mesh file (.OBJ)");

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

    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    igl::readOFF(args::get(inputFilename), V, F);

    // Plot the mesh
    igl::opengl::glfw::Viewer viewer;
    viewer.data().set_mesh(V, F);
    viewer.launch();
}
