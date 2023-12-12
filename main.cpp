#include <glad/glad.h>
#include <GLFW/glfw3.h>
//imgui
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
//half edge
#include<half_edge.h>

//glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//renderizador
#include<Shader.h>
#include<Camera.h>
#include<mesh.h>


extern Solid* firstSolid;

#define WIDTH   1280
#define HEIGHT  720

bool firstMouse = true;
float lastX = 1024 / 2.0f;
float lastY = 768 / 2.0f;

int state = 0;


void processInput(GLFWwindow *window);

void controleCamera(GLFWwindow* janela, Camera* camera, Shader* shader, float deltaTime);

Camera* camera;

int main() {
    system("cls");
    //pt 1 janela
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW\n";
            return -1;
        }

        GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Simple Window", NULL, NULL);
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

        camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
        ///////////////////////////////////////////
        Solid* s = block(1, glm::vec3(1,1,1));//cone(1, 1, 1, 60);//block(1, glm::vec3(1.0,1.0,1.0));//cone(1, 1, 1, 10);//block(1, glm::vec3(1.0,1.0,1.0));
        
        std::cout << larea(s->s_faces->f_loops) << "\n";
        //ball(1, 1, 20, 60);//MVFS(1, 1, 1, glm::vec4(0, 0, 0, 1));//cyl(1, 0.5, 1, 60);//block(1, glm::vec3(1,1,1));////block(1, glm::vec3(1,1,1));//cyl(1, 0.5, 1, 10);//MVFS(1, 1, 1, glm::vec4(0, 0, 0, 1));/*circle(1, 0,0, 1,1,10); //block(1, glm::vec3(0.5,0.5,0.5));//circle(1, 0, 0, 10, 0, 10);*/
       

        Shader* shad = new Shader("shaders/mVtxShader.glsl", "shaders/mFragShader.glsl");
        Modelo *modelo = new Modelo(s);
        //Mesh_Aresta m = Mesh_Aresta(points[0], points[1]);

        //cria matriz de projecao
        glm::mat4 proj = glm::perspective(glm::radians(45.0f), ((float)WIDTH/(float)HEIGHT), 0.1f, 100.0f);
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        shad->use();
        shad->setMat4("projection", proj);
        shad->setMat4("view", view);
        //funcao de loop
        //loop Janela
        int frames = 0;
        double lastTime = glfwGetTime();
        const int FPS = 60;
        const double frameDuration = 1.0 / FPS;
        double last_frame = glfwGetTime();

    while(!glfwWindowShouldClose(window)){
        double now = glfwGetTime();
        double deltaTime = now - last_frame;
        if(deltaTime >= frameDuration){
            last_frame = now;

            
            frames++;
            if (now - lastTime >= 1.0) {
                std::string frm = "FPS: " + std::to_string(frames);
                glfwSetWindowTitle(window, frm.c_str());
                //std::cout << "FPS: " << frames << std::endl;
                frames = 0;
                lastTime += 1.0;
                }
            glfwPollEvents();
            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::ShowDemoWindow(); // Show demo window! :)
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            //renderizar Modelos
            modelo->Render(shad);
            //controle
            controleCamera(window, camera, shad, deltaTime);
            //swap
            glfwSwapBuffers(window);
        }
    }
    //fim janela
    glfwDestroyWindow(window);
    glfwTerminate();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    
    ImGui::DestroyContext();
    //system("pause");
    return 0;
}



void controleCamera(GLFWwindow* janela, Camera* camera, Shader* shader, float deltaTime){
    if(state == 0){
        glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        //ativando o shader
        shader->use();
        //processamento de mouse
        double xposIn;
        double yposIn;
        glfwGetCursorPos(janela, &xposIn, &yposIn);
        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
        lastX = xpos;
        lastY = ypos;
        camera->ProcessMouseMovement(xoffset, yoffset);

        //manda os valores da camera para o shader
        glm::mat4 view = camera->GetViewMatrix();
        shader->setMat4("view", view);
    }
    

    //processamento de input
    if(glfwGetKey(janela, GLFW_KEY_ESCAPE) == GLFW_PRESS){
            glfwSetWindowShouldClose(janela, true);
    }
    if(state == 0){
        if(glfwGetKey(janela, GLFW_KEY_W) == GLFW_PRESS){
                camera->ProcessKeyboard(FORWARD, deltaTime);
        }else if(glfwGetKey(janela, GLFW_KEY_A) == GLFW_PRESS){
                camera->ProcessKeyboard(LEFT, deltaTime);
        }else if(glfwGetKey(janela, GLFW_KEY_S) == GLFW_PRESS){
                camera->ProcessKeyboard(BACKWARD, deltaTime);
        }else if(glfwGetKey(janela, GLFW_KEY_D) == GLFW_PRESS){
                camera->ProcessKeyboard(RIGHT, deltaTime);
        }else if(glfwGetKey(janela, GLFW_KEY_TAB) == GLFW_PRESS){
                glfwSetInputMode(janela, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                state = 1;
        }
    }else{
        if(glfwGetKey(janela, GLFW_KEY_TAB) == GLFW_PRESS){
                state = 0;
        }
    }
}
