add_library(ImGuiExt STATIC
        imguiext/ImGuiHelpers.h
        imguiext/ImGuiTraits.h
        imguiext/ImGuiPlugin.h
        imguiext/ImGuiPlugin.cpp
        imguiext/ImGuiWidget.h
        imguiext/ImGuiMenu.h
        imguiext/ImGuiMenu.cpp
        imguiext/ImGuizmoWidget.h
        imguiext/ImGuizmoWidget.cpp
        imguiext/SelectionWidget.h
        imguiext/SelectionWidget.cpp
)

target_link_libraries(ImGuiExt PUBLIC imgui::imgui igl::igl_glfw imguizmo::imguizmo) # Populate ImGuiExt's LINK_INTERFACE_LIBRARIES

target_include_directories(ImGuiExt PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>) # for headers when building <imgui/...>
