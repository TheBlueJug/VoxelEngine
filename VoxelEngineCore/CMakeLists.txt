cmake_minimum_required(VERSION 3.12)

set(ENGINE_PROJECT_NAME VoxelEngineCore)


add_library(${ENGINE_PROJECT_NAME} STATIC
    src/Utils/test.cpp
    includes/Utils/test.hpp
    

)

file(GLOB RESOURCES_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/*"
)
target_sources(${ENGINE_PROJECT_NAME}
    PRIVATE
        ${RESOURCES_FILES}
)

set(RESOURCES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/resources" CACHE PATH "Path to resources directory")
target_compile_definitions(${ENGINE_PROJECT_NAME}
    PUBLIC
    RESOURCES_DIR="${RESOURCES_DIR}"
)

# add_custom_command(
#     OUTPUT ${CMAKE_BINARY_DIR}/resources_copied
#     COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/resources
#     COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources
#     DEPENDS ${RESOURCES_FILES}
#     COMMENT "Copying resource files to build directory"
# )
# add_custom_target(copy_resources ALL DEPENDS ${CMAKE_BINARY_DIR}/resources_copied)
# add_dependencies(${ENGINE_PROJECT_NAME} copy_resources)


target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)

add_subdirectory(../external/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glad)



