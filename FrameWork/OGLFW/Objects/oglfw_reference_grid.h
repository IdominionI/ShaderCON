#pragma once

#include <FrameWork/OGLFW/Geometry/oglfw_entity_object_geometry.h>
#include <FrameWork/OGLFW/Memory/oglfw_EBO_base.h>

#include <FrameWork/AFW/Geometry/afw_vertex.h>
#include <FrameWork/AFW/Geometry/afw_vertex_indices.h>

class oglfw_ref_grid_class : public oglfw_entity_object_geometry_base_class {
public:
    oglfw_ref_grid_class() {}
    ~oglfw_ref_grid_class() { delete_geometry(), delete_buffers(); }

    afw_vertices_base_class<glm::vec3> geometry;
    afw_vertex_indices_base_class   indices;

    size_t number_grid_cells = 10;
    int    grid_limit = 100;

    color_type xy_grid_color = { 0.7843,0.5882,0.5882,1 }, xz_grid_color = { 0.5882,0.7843,0.5882,1 }, yz_grid_color = { 0.1961,0.1961,0.7843,1 };


    bool display_xy_plane = true, display_xz_plane = false, display_yz_plane = false;
    //bool relative_xy_grid = false, relative_xz_grid = false, relative_yz_grid = false;
    //bool relative_xy_grid_absolute = false, relative_xz_grid_absolute = false, relative_yz_grid_absolute = false;

    //float relative_xy_grid_dist = -3.0f, relative_xz_grid_dist = 3.0f, relative_yz_grid_dist = 3.0f;

    //glm::vec3 xy_grid_origin = { 0.0f,0.0f,0.0f }, xz_grid_origin = { 0.0f,0.0f,0.0f }, yz_grid_origin = { 0.0f,0.0f,0.0f };
    glm::vec3 grid_center_location = { 0.0f,0.0f,0.0f };

    //color_type xy_grid_color = { 0.7843f * 255.0f,0.5882f * 255.0f,0.5882f * 255.0f,1.0f * 255.0f };
    //color_type xz_grid_color = { 0.5882f * 255.0f,0.7843f * 255.0f,0.5882f * 255.0f,1.0f * 255.0f };
    //color_type yz_grid_color = { 0.1961f * 255.0f,0.1961f * 255.0f,0.7843f * 255.0f,1.0f * 255.0f };

    bool initialise() override {
        geometry.clear_vertices();

        float gl = float(grid_limit) / float(number_grid_cells);

        glm::vec3 vertex;
        for (float i = -gl; i < gl + 0.1f; i += 1.0f / (float)number_grid_cells) {
            //for (float i = -5.0f; i < 5.1f; i += 1.0f / (float)number_grid_cells) {
            vertex = { i,0.0,0.0 };
            geometry.add_vertex(vertex);
        }

        delete_buffers();
        define_buffers();

        return true;
    }

    bool update_uniforms() override {
        //auto plane_model = glm::mat4(1.0f);
        //plane_model = glm::scale(plane_model, glm::vec3(8.0f, 8.0f, 8.0f));
        //plane_model = glm::translate(plane_model, glm::vec3(0.0f, 0.0f, 0.0f));
        //plane_model = glm::rotate(plane_model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        //shader->set_mat4(shader->get_program(), plane_model, "u_model");

        shader->set_b1(shader->get_program(), display_xy_plane, "display_xy_plane");
        shader->set_b1(shader->get_program(), display_xz_plane, "display_xz_plane");
        shader->set_b1(shader->get_program(), display_yz_plane, "display_yz_plane");

        //shader->set_b1(shader->get_program(), relative_xy_grid, "relative_xy_grid");
        //shader->set_b1(shader->get_program(), relative_yz_grid, "relative_yz_grid");
        //shader->set_b1(shader->get_program(), relative_xz_grid, "relative_xz_grid");

        //shader->set_f1(shader->get_program(), relative_xy_grid_dist, "relative_xy_grid_dist");
        //shader->set_f1(shader->get_program(), relative_xz_grid_dist, "relative_xz_grid_dist");
        //shader->set_f1(shader->get_program(), relative_yz_grid_dist, "relative_yz_grid_dist");

        shader->set_vec4(shader->get_program(), xy_grid_color, "xy_grid_color");
        shader->set_vec4(shader->get_program(), xz_grid_color, "xz_grid_color");
        shader->set_vec4(shader->get_program(), yz_grid_color, "yz_grid_color");

        shader->set_vec3(shader->get_program(), grid_center_location, "grid_center_location");
        shader->set_f1(shader->get_program(), float(grid_limit), "grid_limit");

        return true;
    }

    void delete_geometry() {
        geometry.delete_vertex_data();
        indices.clear_indices();
    }

    void render_geometry() override {
        update_uniforms();

        vertex_VAO.Bind();
        //printf(" render_geometry : before glDrawElements\n");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_POINTS, 0, int(geometry.get_number_of_vertices()));
        //printf(" render_geometry : Error code :: %i \n",err);

        vertex_VAO.Unbind();
    }

private:
    oglfw_EBO_class EBO;

    void define_buffers() override {
        // to correctly render an object the defining the VBO, VAO and 
        // textures etc must be performrd in the correct order as here

        generate_buffers();

        bind_buffers();

        define_buffer_data();

        define_VBO_buffer_data_layout();
    }

    void  generate_buffers() {
        vertex_VBO.generate(); ///Generate VBO
        vertex_VAO.generate(); ///Generate VAO
        EBO.generate();        ///Generate EBO
    }

    void bind_buffers() {
        vertex_VAO.Bind(); ///First Bind the VAO, so that all the configuration is saved in this VAO
        vertex_VBO.Bind(); ///Bind the VBO to GL_ARRAY_BUFFER
        EBO.Bind();        ///Bind EBO to GL_ELEMENT_ARRAY_BUFFER
    }

    void define_buffer_data() {
        ///Populate VBO with data
        vertex_VBO.define_VBO_data(GL_ARRAY_BUFFER, geometry.get_vertices());
        ///Populate EBO with data
        EBO.define_EBO_data(indices.get_indices());
    }

    void define_VBO_buffer_data_layout() {
        ///Set the info of how the VBO must be read
        /// position attribute
        vertex_VAO.SetVec3(0, sizeof(glm::vec3), (void*)0);
    }

    void delete_buffers() override {
        vertex_VAO.Delete();
        vertex_VBO.Delete();
        EBO.Delete();
    };
};



/*
#include <FrameWork/OGLFW/Geometry/oglfw_entity_object_geometry.h>
#include <FrameWork/OGLFW/Memory/oglfw_EBO_base.h>

#include <FrameWork/AFW/Geometry/afw_vertex.h>
#include <FrameWork/AFW/Geometry/afw_vertex_indices.h>

class oglfw_ref_grid_class : public oglfw_entity_object_geometry_base_class {
public:
    oglfw_ref_grid_class() {}
    ~oglfw_ref_grid_class() { delete_geometry(), delete_buffers(); }

    afw_vertices_base_class<glm::vec3> geometry;
    afw_vertex_indices_base_class   indices;

    size_t    number_grid_cells = 20;

    color_type xy_grid_color = { 0.7843,0.5882,0.5882,1 }, xz_grid_color = { 0.5882,0.7843,0.5882,1 }, yz_grid_color = { 0.1961,0.1961,0.7843,1 };


    bool display_xy_plane = true, display_xz_plane = false, display_yz_plane = false;
    bool relative_xy_grid = false, relative_xz_grid = false, relative_yz_grid = false;
    bool relative_xy_grid_absolute = false, relative_xz_grid_absolute = false, relative_yz_grid_absolute = false;

    float relative_xy_grid_dist = -3.0f, relative_xz_grid_dist = 3.0f, relative_yz_grid_dist = 3.0f;

    glm::vec3 xy_grid_origin = { 0.0f,0.0f,0.0f }, xz_grid_origin = { 0.0f,0.0f,0.0f }, yz_grid_origin = { 0.0f,0.0f,0.0f };

    //color_type xy_grid_color = { 0.7843f * 255.0f,0.5882f * 255.0f,0.5882f * 255.0f,1.0f * 255.0f };
    //color_type xz_grid_color = { 0.5882f * 255.0f,0.7843f * 255.0f,0.5882f * 255.0f,1.0f * 255.0f };
    //color_type yz_grid_color = { 0.1961f * 255.0f,0.1961f * 255.0f,0.7843f * 255.0f,1.0f * 255.0f };

    bool initialise() override {
        //geometry.add_vertices(mVertices);
        //indices.add_indices(mElements);

        glm::vec3 vertex;
        for (float i = -1.0f; i < 1.1f; i += 2.0f / (float)number_grid_cells) {
            vertex = { i,0.0,0.0 };
            geometry.add_vertex(vertex);
        }

        define_buffers();

        return true;
    }

    bool update_uniforms() override {
        auto plane_model = glm::mat4(1.0f);
        plane_model = glm::scale(plane_model, glm::vec3(8.0f, 8.0f, 8.0f));
        plane_model = glm::translate(plane_model, glm::vec3(0.0f, 0.0f, 0.0f));
        plane_model = glm::rotate(plane_model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        shader->set_mat4(shader->get_program(), plane_model, "u_model");// +++++++++++++++

        shader->set_b1(shader->get_program(), display_xy_plane, "display_xy_plane");
        shader->set_b1(shader->get_program(), display_xz_plane, "display_xz_plane");
        shader->set_b1(shader->get_program(), display_yz_plane, "display_yz_plane");

        shader->set_b1(shader->get_program(), relative_xy_grid, "relative_xy_grid");
        shader->set_b1(shader->get_program(), relative_yz_grid, "relative_yz_grid");
        shader->set_b1(shader->get_program(), relative_xz_grid, "relative_xz_grid");

        shader->set_f1(shader->get_program(), relative_xy_grid_dist, "relative_xy_grid_dist");
        shader->set_f1(shader->get_program(), relative_xz_grid_dist, "relative_xz_grid_dist");
        shader->set_f1(shader->get_program(), relative_yz_grid_dist, "relative_yz_grid_dist");

        shader->set_vec4(shader->get_program(), xy_grid_color, "xy_grid_color");
        shader->set_vec4(shader->get_program(), xz_grid_color, "xz_grid_color");
        shader->set_vec4(shader->get_program(), yz_grid_color, "yz_grid_color");

        return true;
    }

    void delete_geometry() {
        geometry.delete_vertex_data();
        indices.clear_indices();
    }

    void render_geometry() override {
        update_uniforms();

        vertex_VAO.Bind();
        //printf(" render_geometry : before glDrawElements\n");
        //glDrawElements(GL_POINTS, static_cast<GLsizei>(indices.get_indices().size()), GL_UNSIGNED_INT, static_cast<void*>(nullptr));
        glDrawArrays(GL_POINTS, 0, GLsizei(geometry.get_number_of_vertices()));
        //printf(" render_geometry : Error code :: %i \n",err);

        vertex_VAO.Unbind();
    }

private:
    oglfw_EBO_class EBO;

    //const std::vector<Vertex> mVertices{
    //        {{-1.0f, +0.0f, -1.0f}},// 0
    //        {{+1.0f, +0.0f, -1.0f}},// 1
    //        {{-1.0f, +0.0f, +1.0f}},// 2
    //        {{+1.0f, +0.0f, +1.0f}},// 3
    //};

    //const std::vector<GLuint> mElements{
    //        0,// 0
    //        1,// 1
    //        2,// 2
    //        2,// 3
    //        1,// 4
    //        3,// 5
    //};

    void define_buffers() override {
        // to correctly render an object the defining the VBO, VAO and 
        // textures etc must be performrd in the correct order as here

        generate_buffers();

        bind_buffers();

        define_buffer_data();

        define_VBO_buffer_data_layout();
    }

    void  generate_buffers() {
        vertex_VBO.generate(); ///Generate VBO
        vertex_VAO.generate(); ///Generate VAO
        EBO.generate();        ///Generate EBO
    }

    void bind_buffers() {
        vertex_VAO.Bind(); ///First Bind the VAO, so that all the configuration is saved in this VAO
        vertex_VBO.Bind(); ///Bind the VBO to GL_ARRAY_BUFFER
        EBO.Bind();        ///Bind EBO to GL_ELEMENT_ARRAY_BUFFER
    }

    void define_buffer_data() {
        ///Populate VBO with data
        vertex_VBO.define_VBO_data(GL_ARRAY_BUFFER, geometry.get_vertices());
        ///Populate EBO with data
        EBO.define_EBO_data(indices.get_indices());
    }

    void define_VBO_buffer_data_layout() {
        ///Set the info of how the VBO must be read
        /// position attribute
       // vertex_VAO.SetVec3(0, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
        vertex_VAO.SetVec3(0, sizeof(glm::vec3), (void*)0);
    }

    void delete_buffers() override {
        vertex_VAO.Delete();
        vertex_VBO.Delete();
        EBO.Delete();
    };
};
*/