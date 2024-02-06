#pragma once

#include "ImGuiMenu.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include <Eigen/Dense>

namespace imguiext {
    /// Widget for a guizmo (3D tranform manipulator)
    class ImGuizmoWidget : public ImGuiWidget {
    public:
        /// Called when the stored transform T changes
        std::function<void(const Eigen::Matrix4f&)> callback{nullptr};

        bool visible{true}; //!< Whether to display

        ImGuizmo::OPERATION operation{ImGuizmo::ROTATE}; //!< Whether rotating, translating or scaling

        Eigen::Matrix4f T{Eigen::Matrix4f::Identity()}; //!< Stored transformation

        void init(igl::opengl::glfw::Viewer* _viewer, ImGuiPlugin* _plugin) override;

        void draw() override;
    };
}
