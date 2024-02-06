list(APPEND GCI_DEP_LIBS igl::igl_core geometry-central taywee::args)

add_executable(load-mesh 101-LoadMesh/main.cpp)
target_link_libraries(load-mesh PUBLIC ${GCI_DEP_LIBS})

add_executable(basic-mutation 102-BasicMutation/main.cpp)
target_link_libraries(basic-mutation PUBLIC ${GCI_DEP_LIBS})

add_executable(direction-fields 103-DirectionFields/main.cpp)
target_link_libraries(direction-fields PUBLIC ${GCI_DEP_LIBS})

add_executable(draw-mesh 104-DrawMesh/main.cpp)
target_link_libraries(draw-mesh PUBLIC ${GCI_DEP_LIBS} igl::igl_glfw)

add_executable(viewer-menu 105-ViewerMenu/main.cpp)
target_link_libraries(viewer-menu PUBLIC ${GCI_DEP_LIBS} ImGuiExt)

add_executable(imguizmo-demo 106-ImGuizmo/main.cpp)
target_link_libraries(imguizmo-demo PUBLIC ${GCI_DEP_LIBS} ImGuiExt)

add_executable(events 107-Events/main.cpp)
target_link_libraries(events PUBLIC ${GCI_DEP_LIBS} igl::igl_glfw)

add_executable(selection 108-Selection/main.cpp)
target_link_libraries(selection PUBLIC ${GCI_DEP_LIBS} ImGuiExt)