#include <glad/glad.h>
#include <GLFW/glfw3.h>
//imgui
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
//half edge
#include<half_edge.h>

extern Solid* firstSolid;

int janela(){
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(640, 480, "Simple Window", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
    ImGui_ImplOpenGL3_Init();
    glClearColor(0,0,0,0);
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow(); // Show demo window! :)
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    return 0;
}


int main() {
    system("cls");
    Solid* s = block(1, glm::vec3(10,10,10));//circle(1, 0, 0, 10, 0, 10);//MVFS(1, 1, 1, glm::vec4(0, 0, 0, 1));
    /*SMEV(1, 1, 1, 2, glm::vec4(10, 10, 10, 1));
    SMEV(1, 1, 2, 3, glm::vec4(11, 12, 12, 1));
    SMEV(1, 1, 3, 4, glm::vec4(13, 14, 15, 1));

    SMEF(1,1,1,4,2);
    std::pair maximos = getMaxNames(s->solid_num);
    std::cout << "Vertex m:" << maximos.first << " Face m:" << maximos.second <<"\n";
    std::cout << "\n---------\n";*/
    //printar todas as arestas de s;
    Edge* e = s->s_edges;
    int k = 1;
    while(e != NIL){
        //ponto inicial
        std::cout << "Aresta h1:" << k << " p1: " << e->he1->vtx->vertex_num << " pf: " << e->he1->nextH->vtx->vertex_num <<"\n";
        //std::cout << "Aresta h2:" << k << " p1: " << e->he2->vtx->vertex_num << " pf: " << e->he2->nextH->vtx->vertex_num <<"\n";
        e = e->nextE;
        k++;
        if(k >= 100){break;}
        //ponto final
    }
    std::cout << "\n-------------------------\n";
    /*Vertex* v = s->s_vertex;
    while(v != NIL){
        //ponto inicial
        std::cout         << "v: "
                          << v->vertex_num
                          << ": ("
                          << v->vcood.x
                          << " "
                          << v->vcood.y
                          << " "
                          << v->vcood.z
                          << ")\n";
        v = v->nextV;
    }*/
    //SMEV(s->solid_num, 1, 3, 4, glm::vec4(10,11,12,1));
    //std::cout << "\n---------\n";
    
    //printSolidos(s);
    std::cout << "\n-------------------------\n";
    ListSolidos(firstSolid);
    system("pause");
    return 0;
}