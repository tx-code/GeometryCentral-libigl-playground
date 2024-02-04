list(APPEND GCI_DEP_LIBS igl::igl_core geometry-central taywee::args)

add_executable(load-mesh 101-LoadMesh/main.cpp)
target_link_libraries(load-mesh PUBLIC ${GCI_DEP_LIBS})

add_executable(basic-mutation 102-BasicMutation/main.cpp)
target_link_libraries(basic-mutation PUBLIC ${GCI_DEP_LIBS})

add_executable(direction-fields 103-DirectionFields/main.cpp)
target_link_libraries(direction-fields PUBLIC ${GCI_DEP_LIBS})