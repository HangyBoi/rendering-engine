#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/texture.h"
#include "core/shader.h"

//#define MAC_CLION
#define VSTUDIO

#ifdef MAC_CLION
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#endif

#ifdef VSTUDIO
// Note: install imgui with:
//     ./vcpkg.exe install imgui[glfw-binding,opengl3-binding]
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#endif

int g_width = 800;
int g_height = 600;

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebufferSizeCallback(GLFWwindow *window,
                             int width, int height) {
    g_width = width;
    g_height = height;
    glViewport(0, 0, width, height);
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(g_width, g_height, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    //Setup platforms
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");

    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
        core::Shader modelShader("shaders/modelVertex.vert", "shaders/fragment.frag");
        core::Shader textureShader("shaders/modelVertex.vert", "shaders/texture.frag");

        if (!modelShader.isValid() || !textureShader.isValid()) {
            printf("Failed to initialize shaders\n");
            return -1;
        }

        // Mesh can't be copied, so move it in: push_back moves the temporary,
        // std::move hands the vector to the model (don't use quadMeshes afterwards).
        std::vector<core::Mesh> quadMeshes;
        quadMeshes.push_back(core::Mesh::generateQuad());
        core::Model quadModel(std::move(quadMeshes));
        quadModel.translate(glm::vec3(0,0,-2.5));
        quadModel.scale(glm::vec3(5, 5, 1));

        core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
        core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");

        glm::vec4 clearColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
        glClearColor(clearColor.r,
                     clearColor.g, clearColor.b, clearColor.a);

        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 10.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
        glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

        //VP
        glm::mat4 view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

        double currentTime = glfwGetTime();
        double finishFrameTime = 0.0;
        float deltaTime = 0.0f;
        float rotationStrength = 100.0f;

        while (!glfwWindowShouldClose(window)) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            ImGui::Begin("Raw Engine v2");
            ImGui::Text("Hello :)");
            ImGui::End();

            processInput(window);
            suzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(rotationStrength) * static_cast<float>(deltaTime));

            textureShader.use();
            textureShader.setMat4("mvpMatrix", (projection * view * quadModel.getModelMatrix()));
            glActiveTexture(GL_TEXTURE0);
            textureShader.setInt("text", 0);
            glBindTexture(GL_TEXTURE_2D, cmgtGatoTexture.getId());
            quadModel.render();
            glBindVertexArray(0);
            glActiveTexture(GL_TEXTURE0);

            modelShader.use();
            modelShader.setMat4("mvpMatrix", (projection * view * suzanne.getModelMatrix()));
            suzanne.render();
            glBindVertexArray(0);

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
            finishFrameTime = glfwGetTime();
            deltaTime = static_cast<float>(finishFrameTime - currentTime);
            currentTime = finishFrameTime;
        }
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}