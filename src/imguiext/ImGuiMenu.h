#pragma once

#include "ImGuiPlugin.h"
#include "ImGuiWidget.h"

namespace imguiext {
    class ImGuiMenu : public ImGuiWidget {
    public:
        void init(igl::opengl::glfw::Viewer* _viewer, ImGuiPlugin* _plugin) override;

        void shutdown() override;

        void draw() override;

        // Can be overwritten by `callback_draw_viewer_window`
        virtual void draw_viewer_window();

        // Can be overwritten by `callback_draw_viewer_menu`
        virtual void draw_viewer_menu();

        // Can be overwritten by `callback_draw_custom_window`
        virtual void draw_custom_window() {};

        // Customizable callbacks
        std::function<void(void)> callback_draw_viewer_window;
        std::function<void(void)> callback_draw_viewer_menu;
        std::function<void(void)> callback_draw_custom_window;

        static float menu_scaling() {
            return ImGuiPlugin::hidpi_scaling() / ImGuiPlugin::pixel_ratio();
        }
    };
}
