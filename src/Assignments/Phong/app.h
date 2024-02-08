//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "Application/application.h"
#include "Application/utils.h"

#include "glad/gl.h"
#include "camera.h"
#include "camera_controler.h"

#include "Engine/Mesh.h"
#include "Engine/Material.h"
#include "Engine/lights.h"

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    void scroll_callback(double xoffset, double yoffset) override {
        Application::scroll_callback(xoffset, yoffset);
        camera()->zoom(yoffset / 30.0f);
    }

    void set_controler(CameraControler *controler) { controler_ = controler; }

private:
    GLuint vao_;
    GLuint vertex_buffer;
    GLuint v_buffer_handle;
    Camera *camera_;
    CameraControler *controler_;
    std::vector<GLushort> indices;
    glm::mat4 model;

    GLuint PVM;

    GLuint Matrices;
    GLuint Lights;

    std::vector<xe::Mesh*> meshes_;

    glm::vec3 ambient_;
    std::vector<PointLight> p_lights_;

    void cursor_position_callback(double x, double y);

    void mouse_button_callback(int button, int action, int mods);

    void add_submesh(xe::Mesh *mesh) {
    meshes_.push_back(mesh);
    }

    void add_light(const PointLight &p_light) {
    p_lights_.push_back(p_light); 
}

    void add_ambient(glm::vec3 ambient) {
            ambient_ = ambient;
}
};