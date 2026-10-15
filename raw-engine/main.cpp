#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <cstdio>
#include <vector>
#include "core/mesh.h"
#include "core/assimpLoader.h"
#include "core/texture.h"
#include "core/shader.h"
#include "core/camera.h"

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

// ---------------------------------------------------------------------------
// Window size (updated by framebufferSizeCallback)
// ---------------------------------------------------------------------------
int g_width = 800;
int g_height = 600;

// ---------------------------------------------------------------------------
// Input
// ---------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Fly camera input, like the Unreal viewport: hold the right mouse button,
// then move the mouse to look and use W/A/S/D to fly (E = up, Q = down).
struct LookState {
    bool active = false;                // right mouse button held, we own the cursor
    double lastX = 0.0, lastY = 0.0;    // cursor position last frame
};
LookState g_look;

void updateCameraInput(GLFWwindow *window, core::Camera &camera, float deltaTime) {
    const float moveSpeed = 5.0f;           // units per second
    const float mouseSensitivity = 0.1f;    // degrees per pixel

    const bool rightHeld = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

    if (rightHeld && !g_look.active && !ImGui::GetIO().WantCaptureMouse) {
        // Just pressed (and not on an ImGui window): hide the cursor and start looking
        g_look.active = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwGetCursorPos(window, &g_look.lastX, &g_look.lastY);
    } else if (!rightHeld && g_look.active) {
        // Just released: give the cursor back
        g_look.active = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
    if (!g_look.active) return;

    // Mouse look: how far the cursor moved since last frame
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    const float dx = static_cast<float>(x - g_look.lastX);
    const float dy = static_cast<float>(g_look.lastY - y);  // screen y grows downwards, so flip it
    g_look.lastX = x;
    g_look.lastY = y;
    camera.rotate(dx * mouseSensitivity, dy * mouseSensitivity);

    // Keys. Multiplying by deltaTime keeps the speed the same on fast and slow computers.
    auto down = [&](int key) { return glfwGetKey(window, key) == GLFW_PRESS; };
    const float step = moveSpeed * deltaTime;
    if (down(GLFW_KEY_W)) camera.moveForward(step);
    if (down(GLFW_KEY_S)) camera.moveForward(-step);
    if (down(GLFW_KEY_D)) camera.moveRight(step);
    if (down(GLFW_KEY_A)) camera.moveRight(-step);
    if (down(GLFW_KEY_E)) camera.moveUp(step);
    if (down(GLFW_KEY_Q)) camera.moveUp(-step);
}

// ---------------------------------------------------------------------------
// Setup: window, OpenGL state, ImGui
// ---------------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow *, int width, int height) {
    g_width = width;
    g_height = height;
    glViewport(0, 0, width, height);
}

// Creates the window with an OpenGL 3.3 core context and loads the GL functions (glad).
// Returns nullptr if anything fails.
GLFWwindow *createWindow() {
    if (!glfwInit()) {
        printf("Failed to initialize GLFW\n");
        return nullptr;
    }
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(g_width, g_height, "Raw Engine", NULL, NULL);
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return nullptr;
    }
    return window;
}

void setupRenderState() {
    glEnable(GL_DEPTH_TEST);
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void initImGui(GLFWwindow *window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 400");
}

void shutdownImGui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

// ---------------------------------------------------------------------------
// Scene: creates everything we draw, then runs the main loop. Returns the exit code.
// Every GL object (shaders, meshes, texture) is a local variable here, so it is destroyed
// when this function returns: before ImGui shutdown and glfwTerminate(), while the
// GL context still exists.
// ---------------------------------------------------------------------------
int runScene(GLFWwindow *window) {
    // --- Shaders ---
    core::Shader modelShader("shaders/modelVertex.vert", "shaders/fragment.frag");
    core::Shader textureShader("shaders/modelVertex.vert", "shaders/texture.frag");
    if (!modelShader.isValid() || !textureShader.isValid()) {
        printf("Failed to initialize shaders\n");
        return -1;
    }

    // --- Scene objects ---
    // Mesh can't be copied, so move it in: push_back moves the temporary,
    // std::move hands the vector to the model (don't use quadMeshes afterwards).
    std::vector<core::Mesh> quadMeshes;
    quadMeshes.push_back(core::Mesh::generateQuad());
    core::Model quadModel(std::move(quadMeshes));
    quadModel.translate(glm::vec3(0, 0, -2.5));
    quadModel.scale(glm::vec3(5, 5, 1));

    core::Model suzanne = core::AssimpLoader::loadModel("models/nonormalmonkey.obj");
    core::Texture cmgtGatoTexture("textures/CMGaTo_crop.png");

    // --- Camera: starts at z = 10, looking down -Z ---
    core::Camera camera(glm::vec3(0.0f, 0.0f, 10.0f), 45.0f,
                        static_cast<float>(g_width) / static_cast<float>(g_height), 0.1f, 100.0f);

    const glm::vec4 clearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

    // --- Main loop ---
    const float suzanneSpinDegreesPerSecond = 100.0f;
    double lastFrameTime = glfwGetTime();
    float deltaTime = 0.0f;  // seconds the last frame took

    while (!glfwWindowShouldClose(window)) {
        // 1. Start the frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::Begin("Raw Engine v2");
        ImGui::Text("Hold right mouse button + W/A/S/D (E/Q: up/down) to fly");
        ImGui::End();

        // 2. Input and update
        processInput(window);
        updateCameraInput(window, camera, deltaTime);
        // Keep the picture undistorted when the window is resized (height is 0 when minimized)
        if (g_height > 0) camera.setAspectRatio(static_cast<float>(g_width) / static_cast<float>(g_height));
        suzanne.rotate(glm::vec3(0.0f, 1.0f, 0.0f), glm::radians(suzanneSpinDegreesPerSecond) * deltaTime);

        // 3. Draw the scene
        const glm::mat4 viewProjection = camera.getProjectionMatrix() * camera.getViewMatrix();

        // use() first: the setters write to the active program
        textureShader.use();
        textureShader.setMat4("mvpMatrix", viewProjection * quadModel.getModelMatrix());
        glActiveTexture(GL_TEXTURE0);
        textureShader.setInt("diffuseTexture", 0);  // sampler reads texture unit 0
        glBindTexture(GL_TEXTURE_2D, cmgtGatoTexture.getId());
        quadModel.render();
        glBindVertexArray(0);
        glActiveTexture(GL_TEXTURE0);

        modelShader.use();
        modelShader.setMat4("mvpMatrix", viewProjection * suzanne.getModelMatrix());
        suzanne.render();
        glBindVertexArray(0);

        // 4. Draw the UI on top
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // 5. Show the frame and measure how long it took
        glfwSwapBuffers(window);
        glfwPollEvents();
        const double now = glfwGetTime();
        deltaTime = static_cast<float>(now - lastFrameTime);
        lastFrameTime = now;
    }
    return 0;
}

// ---------------------------------------------------------------------------
// Entry point: set up, run, tear down (in reverse order)
// ---------------------------------------------------------------------------
int main() {
    GLFWwindow *window = createWindow();
    if (window == nullptr) {
        glfwTerminate();
        return -1;
    }
    initImGui(window);
    setupRenderState();

    const int exitCode = runScene(window);

    shutdownImGui();
    glfwTerminate();
    return exitCode;
}
