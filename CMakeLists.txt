cmake_minimum_required(VERSION 3.20)
project(Frontier)


#set variables
set(Library_Dir libs)
set(Def_library Definitions)
set(Render Renderizador)
set(GLM_STATIC_LIBRARY_ENABLE true)

#create execultable
add_executable(${PROJECT_NAME} 
                ${Def_library}/half_edge.h 
                ${Render}/mesh.h 
                main.cpp
                )


#other CMakes
add_subdirectory(external/glfw)
add_subdirectory(${Library_Dir}/Glad)
add_subdirectory(${Library_Dir}/Glm/gli)
add_subdirectory(${Library_Dir}/Glm/glm)
add_subdirectory(${Library_Dir}/ImGui)


#includes
target_include_directories(${PROJECT_NAME}
                            PUBLIC external/glfw/include
                            PUBLIC ${Library_Dir}/Glad/include/glad
                            PUBLIC ${Library_Dir}/Glm/glm/..
                            PUBLIC ${Library_Dir}/Glm/gli/..
                            PUBLIC ${Library_Dir}/ImGui
                            PUBLIC ${Def_library}
                            ${Render}
                            )


#links de diretorios
link_directories(${PROJECT_NAME}
    PRIVATE external/glfw/src
    )

#link libraries 
target_link_libraries(${PROJECT_NAME}
    glfw 
    glad 
    gli_lib 
    glm_static 
    imgui_lib
    )


