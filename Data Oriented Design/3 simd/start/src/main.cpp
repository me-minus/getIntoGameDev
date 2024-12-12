#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <chrono>
#include <random>
#include <cstdlib>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

int random_int_in_range(int range) {
    unsigned int seed = static_cast<uint32_t>(std::chrono::steady_clock::now().time_since_epoch().count());
    std::default_random_engine generator(seed);

    return generator() % range;
}

float random_float() {

    unsigned int seed = static_cast<uint32_t>(std::chrono::steady_clock::now().time_since_epoch().count());
    std::default_random_engine generator(seed);

    return static_cast<float>(generator()) / static_cast<float>(generator.max());
}

uint32_t make_module(const char* filepath, uint32_t moduleType) {

    std::ifstream file;
    std::stringstream bufferedLines;
    std::string line;

    file.open(filepath);
    while (std::getline(file, line)) {
        //std::cout << line << std::endl;
        bufferedLines << line << '\n';
    }
    std::string shaderSource = bufferedLines.str();
    const char* shaderSrc = shaderSource.c_str();
    bufferedLines.str("");
    file.close();

    unsigned int shaderModule = glCreateShader(moduleType);
    glShaderSource(shaderModule, 1, &shaderSrc, NULL);
    glCompileShader(shaderModule);

    int success;
    glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &success);
    if (!success) {
        char errorLog[1024];
        glGetShaderInfoLog(shaderModule, 1024, NULL, errorLog);
        std::cout << "Shader Module compilation error:\n" << errorLog << std::endl;
    }

    return shaderModule;
}

uint32_t create_shader(const char* vertexFilepath, const char* fragmentFilepath) {

    //To store all the shader modules
    std::vector<uint32_t> modules;

    //Add a vertex shader module
    modules.push_back(make_module(vertexFilepath, GL_VERTEX_SHADER));

    //Add a fragment shader module
    modules.push_back(make_module(fragmentFilepath, GL_FRAGMENT_SHADER));

    //Attach all the modules then link the program
    unsigned int shader = glCreateProgram();
    for (uint32_t shaderModule : modules) {
        glAttachShader(shader, shaderModule);
    }
    glLinkProgram(shader);

    //Check the linking worked
    int success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        char errorLog[1024];
        glGetProgramInfoLog(shader, 1024, NULL, errorLog);
        std::cout << "Shader linking error:\n" << errorLog << '\n';
    }

    //Modules are now unneeded and can be freed
    for (uint32_t shaderModule : modules) {
        glDeleteShader(shaderModule);
    }

    return shader;

}

class Cube {
public:

    glm::vec3 position;
    glm::vec3 eulers;
    glm::vec3 eulerVelocity;

    Cube(glm::vec3 position, glm::vec3 eulers, glm::vec3 eulerVelocity):
    position(position),
    eulers(eulers),
    eulerVelocity(eulerVelocity){
    }
};

class World {
public:
    uint32_t cubeCount = 100000;
    Cube* cubes;

    World() {
        
        cubes = (Cube*)malloc(cubeCount * sizeof(Cube));

        for (uint32_t i = 0; i < cubeCount; ++i) {
            glm::vec3 pos = { 40.0f * (1.0f - 2.0f * random_float()), 
                40.0f * (1.0f - 2.0f * random_float()), 
                40.0f * (1.0f - 2.0f * random_float()) };
            glm::vec3 eulers = { 360.0f * random_float(),
                360.0f * random_float(),
                360.0f * random_float() };
            glm::vec3 eulerVelocity = { 0.4f * (1.0f - 2.0f * random_float()),
                0.4f * (1.0f - 2.0f * random_float()),
                0.4f * (1.0f - 2.0f * random_float()) };
            cubes[i] = Cube(pos, eulers, eulerVelocity);
        }
    }

    void update(float dt) {

        for (uint32_t i = 0; i < cubeCount; ++i) {
            cubes[i].eulers = glm::mod(cubes[i].eulers + dt * (1.0f / 17.0f) * cubes[i].eulerVelocity, glm::vec3(360.0f));
        }
    }

    ~World() {
        free(cubes);
    }
    
};

class CubeMesh {

public:
    uint32_t vertexCount = 36;
    uint32_t VAO, VBO;

    CubeMesh() {

        //x, y, z, s, t
        std::vector<float> vertices = { {
            -0.5, -0.5, -0.5, 0, 0,
             0.5, -0.5, -0.5, 1, 0,
             0.5,  0.5, -0.5, 1, 1,

             0.5,  0.5, -0.5, 1, 1,
            -0.5,  0.5, -0.5, 0, 1,
            -0.5, -0.5, -0.5, 0, 0,

            -0.5, -0.5,  0.5, 0, 0,
             0.5, -0.5,  0.5, 1, 0,
             0.5,  0.5,  0.5, 1, 1,

             0.5,  0.5,  0.5, 1, 1,
            -0.5,  0.5,  0.5, 0, 1,
            -0.5, -0.5,  0.5, 0, 0,

            -0.5,  0.5,  0.5, 1, 0,
            -0.5,  0.5, -0.5, 1, 1,
            -0.5, -0.5, -0.5, 0, 1,

            -0.5, -0.5, -0.5, 0, 1,
            -0.5, -0.5,  0.5, 0, 0,
            -0.5,  0.5,  0.5, 1, 0,

             0.5,  0.5,  0.5, 1, 0,
             0.5,  0.5, -0.5, 1, 1,
             0.5, -0.5, -0.5, 0, 1,

             0.5, -0.5, -0.5, 0, 1,
             0.5, -0.5,  0.5, 0, 0,
             0.5,  0.5,  0.5, 1, 0,

            -0.5, -0.5, -0.5, 0, 1,
             0.5, -0.5, -0.5, 1, 1,
             0.5, -0.5,  0.5, 1, 0,

             0.5, -0.5,  0.5, 1, 0,
            -0.5, -0.5,  0.5, 0, 0,
            -0.5, -0.5, -0.5, 0, 1,

            -0.5,  0.5, -0.5, 0, 1,
             0.5,  0.5, -0.5, 1, 1,
             0.5,  0.5,  0.5, 1, 0,

             0.5,  0.5,  0.5, 1, 0,
            -0.5,  0.5,  0.5, 0, 0,
            -0.5,  0.5, -0.5, 0, 1 } };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 20, (void*)0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 20, (void*)12);
    }

    ~CubeMesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
};

class Material {

private:

    uint32_t texture;

public:

    Material(const char* filepath) {

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, channels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(filepath, &width, &height, &channels, STBI_rgb_alpha);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        free(data);
    }

    void use() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
    }

    ~Material() {
        glDeleteTextures(1, &texture);
    }
};

class App {

private:

    GLFWwindow* window;
    float width = 800.0f, height = 600.0f;

    double lastTime, lastFrameTime, currentTime;
    int numFrames;
    float frameTime;

    World* world;
    Material* woodTexture;
    CubeMesh* cubeMesh;
    glm::mat4* modelTransforms;

    uint32_t shader, modelTransformVBO;

public:

    App() {
        glfwInit();
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);


        window = glfwCreateWindow(static_cast<int>(width), static_cast<int>(height), "Hello Window!", NULL, NULL);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(0);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cout << "Couldn't load opengl" << std::endl;
            glfwTerminate();
        }

        lastTime = glfwGetTime();
        lastFrameTime = glfwGetTime();
        numFrames = 0;
        frameTime = 0.0f;

        world = new World();

        // initialise opengl
        glClearColor(0.1, 0.2, 0.2, 1);
        shader = create_shader("src/shaders/vertex.txt", "src/shaders/fragment.txt");
        glUseProgram(shader);
        glUniform1i(glGetUniformLocation(shader, "imageTexture"), 0);
        glEnable(GL_DEPTH_TEST);

        woodTexture = new Material("img/wood.png");
        cubeMesh = new CubeMesh();

        constexpr float fovY = glm::radians(45.0f);
        float aspect = width / height;
        float near = 0.1f;
        float far = 400.0f;
        glm::mat4 projection = glm::perspective(fovY, aspect, near, far);
        glUniformMatrix4fv(
            glGetUniformLocation(shader, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));

        glm::vec3 eye = { -150, 0, 0 };
        glm::vec3 target = { 0,0,0 };
        glm::vec3 up = { 0,0,1 };
        glm::mat4 view = glm::lookAt(eye, target, up);
        glUniformMatrix4fv(
            glGetUniformLocation(shader, "view"),
            1, GL_FALSE, glm::value_ptr(view));

        modelTransforms = (glm::mat4*)malloc(world->cubeCount * sizeof(glm::mat4));
        for (size_t i = 0; i < world->cubeCount; ++i) {
            modelTransforms[i] = glm::mat4(1.0f);
        }

        glGenBuffers(1, &modelTransformVBO);
        glBindBuffer(GL_ARRAY_BUFFER, modelTransformVBO);
        glBufferData(GL_ARRAY_BUFFER, world->cubeCount * sizeof(glm::mat4), modelTransforms, GL_STATIC_DRAW);

        glBindVertexArray(cubeMesh->VAO);
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 64, (void*)0);
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 64, (void*)16);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 64, (void*)32);
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 64, (void*)48);
        // 0: per shader call, 1 : per instance
        glVertexAttribDivisor(2, 1);
        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
    }

    void run() {

        while (!glfwWindowShouldClose(window)) {

            // check events
            if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
                break;
            }
            glfwPollEvents();

            world->update(frameTime);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glUseProgram(shader);

            /*
            for (size_t i = 0; i < world->cubeCount; ++i) {
                Cube cube = world->cubes[i];
                glm::mat4 pitch = glm::rotate(glm::mat4(1.0f), glm::radians(cube.eulers.y), { 0, 1, 0 });
                glm::mat4 yaw = glm::rotate(glm::mat4(1.0f), glm::radians(cube.eulers.z), { 0, 0, 1 });
                glm::mat4 roll = glm::rotate(glm::mat4(1.0f), glm::radians(cube.eulers.x), { 1, 0, 0 });
                glm::mat4 translate = glm::translate(glm::mat4(1.0f), cube.position);
                modelTransforms[i] = translate * roll * pitch * yaw;
            }
            glBufferData(GL_ARRAY_BUFFER, world->cubeCount * sizeof(glm::mat4), modelTransforms, GL_STATIC_DRAW);


            woodTexture->use();
            glBindVertexArray(cubeMesh->VAO);
            glDrawArraysInstanced(GL_TRIANGLES, 0, cubeMesh->vertexCount, world->cubeCount);
            */
            glFlush();
            //glfwSwapBuffers(window);

            currentTime = glfwGetTime();
            double delta = currentTime - lastTime;
            double frameDelta = currentTime - lastFrameTime;
            frameTime = float(1000.0f * frameDelta);
            lastFrameTime = currentTime;

            if (delta >= 1) {
                int framerate{ std::max(1, int(numFrames / delta)) };
                std::stringstream title;
                title << "Running at " << framerate << " fps.";
                glfwSetWindowTitle(window, title.str().c_str());
                lastTime = currentTime;
                numFrames = -1;
            }

            ++numFrames;
        }
    }

    ~App() {
        delete world;
        delete cubeMesh;
        delete woodTexture;
        glDeleteProgram(shader);
        glfwTerminate();
    }
};

int main() {
    App* myApp = new App();
    myApp->run();
    delete myApp;
}