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
#include <Engine/ColorMaterial.h>

void SimpleShapeApplication::init() {
    set_camera(new Camera);
    set_controler(new CameraControler(camera()));
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
            {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
             {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }

    xe::ColorMaterial::init();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLfloat> vertices = {
        0.5f, 0.0f, -0.5f,
        -0.5f, 0.0f, -0.5f,
        -0.5f, 0.0f, 0.5f,

        0.5f, 0.0f, -0.5f,
        -0.5f, 0.0f, 0.5f,
        0.5f, 0.0f, 0.5f,

        0.5f, 0.0f, -0.5f,
        0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, -0.5f,

        -0.5f, 0.0f, -0.5f,
        0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f,

        -0.5f, 0.0f, 0.5f,
        0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, 0.5f,

        0.5f, 0.0f, 0.5f,
        0.0f, 1.0f, 0.0f,
        0.5f, 0.0f, -0.5f,
        };

    indices = {
        0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17
    };

    auto pyramid = new xe::Mesh;
    pyramid->allocate_vertex_buffer(vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid->load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());
    pyramid->vertex_attrib_pointer(0, 3, GL_FLOAT, 3 * sizeof(GLfloat), 0);

    pyramid->allocate_index_buffer(indices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid->load_indices(0, indices.size() * sizeof(GLfloat), indices.data());

    pyramid->add_submesh(0, 6, new xe::ColorMaterial({0.0f, 1.0f, 1.0f, 1.0f}) ); 
    pyramid->add_submesh(6, 9, new xe::ColorMaterial({0.0f, 0.0f, 1.0f, 1.0f}) );
    pyramid->add_submesh(9, 12, new xe::ColorMaterial({1.0f, 1.0f, 0.0f, 1.0f}) );
    pyramid->add_submesh(12, 15, new xe::ColorMaterial({1.0f, 0.0f, 0.0f, 1.0f}) );
    pyramid->add_submesh(15, 18, new xe::ColorMaterial({0.0f, 1.0f, 0.0f, 1.0f}) ); 
    add_submesh(pyramid);

    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

    // This indicates that the data for attribute 0 should be read from a vertex buffer.
    glEnableVertexAttribArray(0);
    // and this specifies how the data is layout in the buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));

    int w, h;
    std::tie(w, h) = frame_buffer_size();
    auto aspect_ = (float)w/h;
    auto fov_ = glm::pi<float>()/4.0;
    auto near_ = 0.1f;
    auto far_ = 100.0f;

    camera_->look_at(glm::vec3(2.0f, -1.f, 2.0f),
                     glm::vec3(0.0f, 0.0f, 0.0f),
                     glm::vec3(0.0f, 1.0f, 0.0f));

    camera_->perspective(fov_, aspect_, near_, far_);

    GLuint blockIndex = glGetUniformBlockIndex(program, "Transformations");
    glUniformBlockBinding(program, blockIndex, 1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    //end of vao "recording"

    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    glViewport(0, 0, w, h);

    glUseProgram(program);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    auto PVM = camera_->projection() * camera_->view();
    glBindBuffer(GL_UNIFORM_BUFFER, vertex_buffer);
    glBufferSubData(GL_UNIFORM_BUFFER,0, sizeof(glm::mat4), &PVM[0]);

    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    glDrawArrays(GL_TRIANGLES, 0, 18);
    glBindVertexArray(0);

     for (auto m: meshes_)
        m->draw();
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    camera_->set_aspect((float) w / h);
}

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
    Application::cursor_position_callback(x, y);
    if (controler_) {
        controler_->mouse_moved(x, y);
    }
}

void SimpleShapeApplication::mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);
    if (controler_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controler_->LMB_pressed(x, y);
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controler_->LMB_released(x, y);
    }
}
