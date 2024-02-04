add_executable(load-mesh 101-LoadMesh/main.cpp)
target_link_libraries(load-mesh PUBLIC igl::igl_core geometry-central taywee::args)