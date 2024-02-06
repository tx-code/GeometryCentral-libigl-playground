#pragma once

#include <igl/opengl/glfw/Viewer.h>
#include <igl/opengl/glfw/ViewerPlugin.h>

struct ImGuiContext;

namespace imguiext {
    class ImGuiWidget;

    class ImGuiPlugin : public igl::opengl::glfw::ViewerPlugin {
    protected:
        float m_hidpi_scaling{0.f};
        float m_pixel_ratio{0.f};
        ImGuiContext* m_context{nullptr};

    public:
        // List of registered widgets
        std::vector<ImGuiWidget *> widgets;

    public:
        void init(igl::opengl::glfw::Viewer* _viewer) override;

        void init_widgets();

        virtual void reload_font(int font_size);

        void shutdown() override;

        bool pre_draw() override;

        bool post_draw() override;

        void post_resize(int, int) override;

        bool mouse_down(int, int) override;

        bool mouse_up(int, int) override;

        bool mouse_move(int, int) override;

        bool mouse_scroll(float) override;

        bool key_pressed(unsigned int, int) override;

        bool key_down(int, int) override;

        bool key_up(int, int) override;

        static float pixel_ratio();

        static float hidpi_scaling();
    };
}
