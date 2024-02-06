#include "ImGuizmoWidget.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_fonts_droid_sans.h>
#include <GLFW/glfw3.h>

namespace imguiext {
    void ImGuizmoWidget::init(igl::opengl::glfw::Viewer* _viewer, ImGuiPlugin* _plugin) {
        ImGuiWidget::init(_viewer, _plugin);
    }

    void ImGuizmoWidget::draw() {
        if (!visible) return;
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0);
        ImGuizmo::BeginFrame();
        ImGui::PopStyleVar();
        // Don't draw the Viewer's default menu: draw just the ImGuizmo
        Eigen::Matrix4f view = (m_viewer->core().view / m_viewer->core().camera_zoom);
        Eigen::Matrix4f proj = m_viewer->core().proj;
        if (m_viewer->core().orthographic) { view(2, 3) -= 1000;/* Hack depth */ }
        // ImGuizmo doesn't like a lot of scaling in view, shift it to T
        const float z = m_viewer->core().camera_base_zoom;
        const Eigen::Matrix4f S =
                (Eigen::Matrix4f() << z, 0, 0, 0, 0, z, 0, 0, 0, 0, z, 0, 0, 0, 0, 1).finished();
        view = (view * S.inverse()).eval();
        const Eigen::Matrix4f T0 = T;
        T = (S * T).eval();
        ImGuiIO& io = ImGui::GetIO();
        ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);
        Manipulate(
            view.data(), proj.data(), operation, ImGuizmo::LOCAL, T.data(),NULL,NULL);
        // invert scaling that was shifted onto T
        T = (S.inverse() * T).eval();
        // Only call if actually changed; otherwise, triggers on all mouse events
        if (const float diff = (T - T0).array().abs().maxCoeff(); diff > 1e-7) { callback(T); }
    }
}
