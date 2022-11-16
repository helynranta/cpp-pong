cmake_minimum_required(VERSION 3.21.2)

project(template CXX)

find_package(engine CONFIG REQUIRED)

include(engine_create)

engine_create_plugin(template
        PROJECT_FILE
            "${CMAKE_CURRENT_LIST_DIR}/template.project"

        SOURCE_FILES
            src/plugin.cpp

        INCLUDE_FILES
            src/scenes/load.hpp
            src/tags.hpp
        )

engine_create_launcher(
        SOURCE_DIR "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/engine/launcher"
        PROJECT_FILE "${CMAKE_SOURCE_DIR}/template.project"
)

target_precompile_headers(${PROJECT_NAME} PUBLIC src/engine.hxx)