#pragma once

#include <memory>
#include <string>

// Fwd
namespace igl::opengl::glfw {
    class Viewer;
}

namespace imguiext {
    // Fwd
    class ImGuiPlugin;

    /// Abstract class for imgui "widgets". A widget is something that uses imgui,
    /// but doesn't own the entire imgui IO stack:the simge plugin owns that and widgets are registered with it.
    class ImGuiWidget {
    protected:
        igl::opengl::glfw::Viewer* m_viewer = nullptr;
        ImGuiPlugin* m_plugin = nullptr;

    public:
        std::string m_name{"dummy"};

        virtual ~ImGuiWidget() = default;

        virtual void init(igl::opengl::glfw::Viewer* _viewer, ImGuiPlugin* _plugin) {
            m_viewer = _viewer;
            m_plugin = _plugin;
        }

        virtual void shutdown() {
        }

        virtual void draw() {
        }

        virtual bool mouse_down(int, int) { return false; }
        virtual bool mouse_up(int, int) { return false; }
        virtual bool mouse_move(int, int) { return false; }
        virtual bool key_pressed(unsigned int, int) { return false; }
        virtual bool key_down(int, int) { return false; }
        virtual bool key_up(int, int) { return false; }
    };
}
