//
// Created by pbialas on 25.09.2020.
//

#include "app.h"
#include <iostream>
#include <vector>
#include <tuple>
#include "Application/utils.h"

#include <array>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

void SimpleShapeApplication::init() {
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
            {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
             {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
            -0.5f, 0.0f, 0.0f, //upper left
            0.5f, 0.0f, 0.0f, //upper right
            0.0f, 0.5f, 0.0f, //upper center
            -0.5f, -0.5f, 0.0f, //lower left
            0.5f, -0.5f, 0.0f, //lower right
            };

    std::vector<GLushort> indices{0,1,2,0,3,4,0,1,4};

    const std::vector colors = {
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
    };

    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint indices_buffer;
    glGenBuffers(1, &indices_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint colors_buffer;
    glGenBuffers(1, &colors_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, colors_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(colors.size()) * sizeof(GLfloat), colors.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint uniforms_buffer;
    glGenBuffers(1, &uniforms_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, uniforms_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(float), 0, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniforms_buffer);

    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, vertex_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 16 * sizeof(GLfloat), 0, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, vertex_buffer);

    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_buffer);

    // This indicates that the data for attribute 0 should be read from a vertex buffer.
    glEnableVertexAttribArray(0);
    // and this specifies how the data is layout in the buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));

    glBindBuffer(GL_ARRAY_BUFFER, colors_buffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    float strength = 0.9f;
    float color[3] = {0.9f, 0.9f, 0.0f};

    glBindBuffer(GL_UNIFORM_BUFFER, uniforms_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(float), 3 * sizeof(float), color);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glm::mat4 PVM(1.0f);
    glm::mat4 proj = glm::perspective(55.0f, 1.0f, 1.0f, 5.0f);
    glm::mat4 view = glm::lookAt(
            glm::vec3(0.0f, 2.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 model = glm::mat4(1.0f);
    PVM = proj * view * model;

    glBindBuffer(GL_UNIFORM_BUFFER, vertex_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER,0, sizeof(glm::mat4), &PVM);

    GLuint blockIndex = glGetUniformBlockIndex(program, "Transformations");
    glUniformBlockBinding(program, blockIndex, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //end of vao "recording"

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    auto[w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    glUseProgram(program);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES,9,GL_UNSIGNED_SHORT,0);
    glBindVertexArray(0);
}
