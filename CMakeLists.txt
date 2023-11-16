cmake_minimum_required(VERSION 3.20)
project(Frontier)

#set variables
set(Library_Dir libs)
set(Def_library Definitions)
set(GLM_STATIC_LIBRARY_ENABLE true)


#other CMakes
add_subdirectory(${Library_Dir}/Glad)
add_subdirectory(${Library_Dir}/Glm/gli)
add_subdirectory(${Library_Dir}/Glm/glm)
add_subdirectory(${Library_Dir}/ImGui)


#includes
include_directories(${Library_Dir}/GLFW/GLFW)
include_directories(${Library_Dir}/Glad/include/glad)
include_directories(${Library_Dir}/Glm/glm/..)
include_directories(${Library_Dir}/Glm/gli/..)
include_directories(${Library_Dir}/ImGui)
include_directories(${Def_library})

#create variables
add_executable(${PROJECT_NAME} ${Def_library}/half_edge.h main.cpp)

#glad
target_link_libraries(${PROJECT_NAME} glad)

#glfw
target_link_libraries(${PROJECT_NAME} glfw3)

#gli
target_link_libraries(${PROJECT_NAME} gli_lib)

#glm
target_link_libraries(${PROJECT_NAME} glm_static)

#glm
target_link_libraries(${PROJECT_NAME} imgui_lib)

