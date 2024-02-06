//
// Created by arya on 2024/2/5.
//

#include <igl/read_triangle_mesh.h>
#include <igl/opengl/glfw/Viewer.h>
#include <GLFW/glfw3.h>

#include <args.hxx>

#include "imguiext/ImGuiMenu.h"
#include "imguiext/ImGuiPlugin.h"
#include "imguiext/ImGuizmoWidget.h"
#include <ImGuizmo.h>

int main(int argc, char** argv) {
    args::ArgumentParser parser("");
    args::HelpFlag help(parser, "help", "Display this help menu", {'h', "help"});

    args::Positional<std::string> inputFilename(parser, "mesh", "A surface mesh file (.OFF)");

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

    // Set up viewer
    igl::opengl::glfw::Viewer vr;
    vr.data().set_mesh(V, F);

    imguiext::ImGuiPlugin imgui_plugin;
    vr.plugins.push_back(&imgui_plugin);

    // Add a 3D gizmo plugin
    imguiext::ImGuizmoWidget gizmo;
    imgui_plugin.widgets.push_back(&gizmo);
    // Initialize ImGuizmo at mesh centroid
    gizmo.T.block(0, 3, 3, 1) =
            0.5 * (V.colwise().maxCoeff() + V.colwise().minCoeff()).transpose().cast<float>();
    // Update can be applied relative to this remembered initial transform
    const Eigen::Matrix4f T0 = gizmo.T;
    // Attach callback to apply imguizmo's transform to mesh
    gizmo.callback = [&](const Eigen::Matrix4f& T) {
        const Eigen::Matrix4d TT = (T * T0.inverse()).cast<double>().transpose();
        vr.data().set_vertices(
            (V.rowwise().homogeneous() * TT).rowwise().hnormalized());
        vr.data().compute_normals();
    };
    // Maya-style keyboard shortcuts for operation
    vr.callback_key_pressed = [&](decltype(vr)&, unsigned int key, int mod) {
        switch (key) {
            case ' ': gizmo.visible = !gizmo.visible;
                return true;
            case 'W':
            case 'w': gizmo.operation = ImGuizmo::TRANSLATE;
                return true;
            case 'E':
            case 'e': gizmo.operation = ImGuizmo::ROTATE;
                return true;
            case 'R':
            case 'r': gizmo.operation = ImGuizmo::SCALE;
                return true;
            default: ;
        }
        return false;
    };

    imguiext::ImGuiMenu menu;
    imgui_plugin.widgets.push_back(&menu);

    std::cout << R"(
W,w  Switch to translate operation
E,e  Switch to rotate operation
R,r  Switch to scale operation
)";
    vr.launch();
}
