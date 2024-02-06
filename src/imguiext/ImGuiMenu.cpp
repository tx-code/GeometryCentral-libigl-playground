#include "ImGuiMenu.h"
#include "ImGuiHelpers.h"

#include <imgui.h>

namespace imguiext {
    void ImGuiMenu::init(igl::opengl::glfw::Viewer* _viewer, ImGuiPlugin* _plugin) {
        m_viewer = _viewer;
        m_plugin = _plugin;
    }

    void ImGuiMenu::shutdown() {
    }

    void ImGuiMenu::draw() {
        // Viewer settings
        if (callback_draw_viewer_window) { callback_draw_viewer_window(); } else draw_viewer_window();

        // Other windows
        if (callback_draw_custom_window) { callback_draw_custom_window(); } else draw_custom_window();
    }

    void ImGuiMenu::draw_viewer_window() {
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(0.0f, 0.0f), ImGuiCond_FirstUseEver);
        bool _viewer_menu_visible = true;
        ImGui::Begin(
            "Viewer", &_viewer_menu_visible,
            ImGuiWindowFlags_NoSavedSettings
            | ImGuiWindowFlags_AlwaysAutoResize
        );
        ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.4f);
        if (callback_draw_viewer_menu) { callback_draw_viewer_menu(); } else { draw_viewer_menu(); }
        ImGui::PopItemWidth();
        ImGui::End();
    }

    void ImGuiMenu::draw_viewer_menu() {
        // Workspace
        if (ImGui::CollapsingHeader("Workspace", ImGuiTreeNodeFlags_DefaultOpen)) {
            float w = ImGui::GetContentRegionAvail().x;
            float p = ImGui::GetStyle().FramePadding.x;
            if (ImGui::Button("Load##Workspace", ImVec2((w - p) / 2.f, 0))) {
                m_viewer->load_scene();
            }
            ImGui::SameLine(0, p);
            if (ImGui::Button("Save##Workspace", ImVec2((w - p) / 2.f, 0))) {
                m_viewer->save_scene();
            }
        }

        // Mesh
        if (ImGui::CollapsingHeader("Mesh", ImGuiTreeNodeFlags_DefaultOpen)) {
            float w = ImGui::GetContentRegionAvail().x;
            float p = ImGui::GetStyle().FramePadding.x;
            if (ImGui::Button("Load##Mesh", ImVec2((w - p) / 2.f, 0))) {
                m_viewer->open_dialog_load_mesh();
            }
            ImGui::SameLine(0, p);
            if (ImGui::Button("Save##Mesh", ImVec2((w - p) / 2.f, 0))) {
                m_viewer->open_dialog_save_mesh();
            }
        }

        // Viewing options
        if (ImGui::CollapsingHeader("Viewing Options", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Button("Center object", ImVec2(-1, 0))) {
                m_viewer->core().align_camera_center(m_viewer->data().V, m_viewer->data().F);
            }
            if (ImGui::Button("Snap canonical view", ImVec2(-1, 0))) {
                m_viewer->snap_to_canonical_quaternion();
            }

            // Zoom
            ImGui::PushItemWidth(80 * menu_scaling());
            ImGui::DragFloat("Zoom", &(m_viewer->core().camera_zoom), 0.05f, 0.1f, 20.0f);

            // Select rotation type
            int rotation_type = static_cast<int>(m_viewer->core().rotation_type);
            static Eigen::Quaternionf trackball_angle = Eigen::Quaternionf::Identity();
            static bool orthographic = true;
            if (ImGui::Combo("Camera Type", &rotation_type, "Trackball\0Two Axes\0002D Mode\0\0")) {
                using RT = igl::opengl::ViewerCore::RotationType;
                auto new_type = static_cast<RT>(rotation_type);
                if (new_type != m_viewer->core().rotation_type) {
                    if (new_type == RT::ROTATION_TYPE_NO_ROTATION) {
                        trackball_angle = m_viewer->core().trackball_angle;
                        orthographic = m_viewer->core().orthographic;
                        m_viewer->core().trackball_angle = Eigen::Quaternionf::Identity();
                        m_viewer->core().orthographic = true;
                    } else if (m_viewer->core().rotation_type == RT::ROTATION_TYPE_NO_ROTATION) {
                        m_viewer->core().trackball_angle = trackball_angle;
                        m_viewer->core().orthographic = orthographic;
                    }
                    m_viewer->core().set_rotation_type(new_type);
                }
            }

            // Orthographic view
            ImGui::Checkbox("Orthographic view", &(m_viewer->core().orthographic));
            ImGui::PopItemWidth();
        }

        // Helper for setting viewport specific mesh options
        auto make_checkbox = [&](const char* label, unsigned int& option) {
            return ImGui::Checkbox(label,
                                   [&]() { return m_viewer->core().is_set(option); },
                                   [&](bool value) { return m_viewer->core().set(option, value); }
            );
        };

        // Draw options
        if (ImGui::CollapsingHeader("Draw Options", ImGuiTreeNodeFlags_DefaultOpen)) {
            if (ImGui::Checkbox("Face-based", &(m_viewer->data().face_based))) {
                m_viewer->data().dirty = igl::opengl::MeshGL::DIRTY_ALL;
            }
            make_checkbox("Show texture", m_viewer->data().show_texture);
            if (ImGui::Checkbox("Invert normals", &(m_viewer->data().invert_normals))) {
                m_viewer->data().dirty |= igl::opengl::MeshGL::DIRTY_NORMAL;
            }
            make_checkbox("Show overlay", m_viewer->data().show_overlay);
            make_checkbox("Show overlay depth", m_viewer->data().show_overlay_depth);
            ImGui::ColorEdit4("Background", m_viewer->core().background_color.data(),
                              ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);
            ImGui::ColorEdit4("Line color", m_viewer->data().line_color.data(),
                              ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_PickerHueWheel);
            ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.3f);
            ImGui::DragFloat("Shininess", &(m_viewer->data().shininess), 0.05f, 0.0f, 100.0f);
            ImGui::PopItemWidth();
        }

        // Overlays
        if (ImGui::CollapsingHeader("Overlays", ImGuiTreeNodeFlags_DefaultOpen)) {
            make_checkbox("Wireframe", m_viewer->data().show_lines);
            make_checkbox("Fill", m_viewer->data().show_faces);
            make_checkbox("Show vertex labels", m_viewer->data().show_vertex_labels);
            make_checkbox("Show faces labels", m_viewer->data().show_face_labels);
            make_checkbox("Show extra labels", m_viewer->data().show_custom_labels);
        }
    }

}
