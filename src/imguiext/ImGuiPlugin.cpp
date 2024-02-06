#include "ImGuiPlugin.h"
#include "ImGuiHelpers.h"
#include "ImGuiWidget.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>
#include <imgui_fonts_droid_sans.h>
#include <GLFW/glfw3.h>

namespace imguiext {
    void ImGuiPlugin::init(igl::opengl::glfw::Viewer* _viewer) {
        ViewerPlugin::init(_viewer);
        // Setup ImGui binding
        if (_viewer) {
            IMGUI_CHECKVERSION();
            if (!m_context) {
                // Single global context by default, but can be overridden by the user
                static ImGuiContext* _global_context = ImGui::CreateContext();
                m_context = _global_context;
            }
            const char* glsl_version = "#version 150";
            ImGui_ImplGlfw_InitForOpenGL(viewer->window, false);
            ImGui_ImplOpenGL3_Init(glsl_version);
            ImGui::GetIO().IniFilename = nullptr;
            ImGui::StyleColorsDark();
            ImGuiStyle& style = ImGui::GetStyle();
            style.FrameRounding = 5.0f;
            reload_font(13);
        }
        init_widgets();
    }

    void ImGuiPlugin::init_widgets() {
        // Init all widgets
        for (auto& widget: widgets) { widget->init(viewer, this); }
    }

    void ImGuiPlugin::reload_font(int font_size) {
        m_hidpi_scaling = hidpi_scaling();
        m_pixel_ratio = pixel_ratio();
        ImGuiIO& io = ImGui::GetIO();
        io.Fonts->Clear();
        io.Fonts->AddFontFromMemoryCompressedTTF(droid_sans_compressed_data,
                                                 droid_sans_compressed_size, font_size * m_hidpi_scaling);
        io.FontGlobalScale = 1.0 / m_pixel_ratio;
    }

    void ImGuiPlugin::shutdown() {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        // User is responsible for destroying context if a custom context is given
        // ImGui::DestroyContext(*context_);
    }

    bool ImGuiPlugin::pre_draw() {
        glfwPollEvents();

        // Check whether window dpi has changed
        float scaling = hidpi_scaling();
        if (std::abs(scaling - m_hidpi_scaling) > 1e-5) {
            reload_font(13);
            ImGui_ImplOpenGL3_DestroyDeviceObjects();
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        return false;
    }

    bool ImGuiPlugin::post_draw() {
        for (auto& widget: widgets) { widget->draw(); }
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        return false;
    }

    void ImGuiPlugin::post_resize(int width, int height) {
        if (m_context) {
            ImGui::GetIO().DisplaySize.x = float(width);
            ImGui::GetIO().DisplaySize.y = float(height);
        }
    }

    // Mouse IO
    bool ImGuiPlugin::mouse_down(int button, int modifier) {
        ImGui_ImplGlfw_MouseButtonCallback(viewer->window, button, GLFW_PRESS, modifier);
        if (ImGui::GetIO().WantCaptureMouse) { return true; }
        for (auto& widget: widgets) {
            if (widget->mouse_down(button, modifier)) { return true; }
        }
        return false;
    }

    bool ImGuiPlugin::mouse_up(int button, int modifier) {
        ImGui_ImplGlfw_MouseButtonCallback(viewer->window, button, GLFW_RELEASE, modifier);
        if(ImGui::GetIO().WantCaptureMouse) { return true; }
        //return ImGui::GetIO().WantCaptureMouse;
        // !! Should not steal mouse up
        for (auto& widget: widgets) {
            widget->mouse_up(button, modifier);
        }
        return false;
    }

    bool ImGuiPlugin::mouse_move(int mouse_x, int mouse_y) {
        if (ImGui::GetIO().WantCaptureMouse) { return true; }
        for (auto& widget: widgets) {
            if (widget->mouse_move(mouse_x, mouse_y)) { return true; }
        }
        return false;
    }

    bool ImGuiPlugin::mouse_scroll(float delta_y) {
        ImGui_ImplGlfw_ScrollCallback(viewer->window, 0.f, delta_y);
        return ImGui::GetIO().WantCaptureMouse;
    }

    // Keyboard IO
    bool ImGuiPlugin::key_pressed(unsigned int key, int modifiers) {
        ImGui_ImplGlfw_CharCallback(nullptr, key);
        if (ImGui::GetIO().WantCaptureKeyboard) { return true; }
        for (auto& widget: widgets) {
            if (widget->key_pressed(key, modifiers)) { return true; }
        }
        return false;
    }

    bool ImGuiPlugin::key_down(int key, int modifiers) {
        ImGui_ImplGlfw_KeyCallback(viewer->window, key, 0, GLFW_PRESS, modifiers);
        if (ImGui::GetIO().WantCaptureKeyboard) { return true; }
        for (auto& widget: widgets) {
            if (widget->key_down(key, modifiers)) { return true; }
        }
        return false;
    }

    bool ImGuiPlugin::key_up(int key, int modifiers) {
        ImGui_ImplGlfw_KeyCallback(viewer->window, key, 0, GLFW_RELEASE, modifiers);
        if (ImGui::GetIO().WantCaptureKeyboard) { return true; }
        for (auto& widget: widgets) {
            if (widget->key_up(key, modifiers)) { return true; }
        }
        return false;
    }

    float ImGuiPlugin::pixel_ratio() {
        // Computes pixel ratio for hidpi devices
        int buf_size[2];
        int win_size[2];
        GLFWwindow* window = glfwGetCurrentContext();
        glfwGetFramebufferSize(window, &buf_size[0], &buf_size[1]);
        glfwGetWindowSize(window, &win_size[0], &win_size[1]);
        return (float) buf_size[0] / (float) win_size[0];
    }

    float ImGuiPlugin::hidpi_scaling() {
        // Computes scaling factor for hidpi devices
        float xscale, yscale;
        GLFWwindow* window = glfwGetCurrentContext();
        glfwGetWindowContentScale(window, &xscale, &yscale);
        return 0.5 * (xscale + yscale);
    }
}
