#pragma once

#include <FrameWork/OGLFW/Geometry/oglfw_entity_object_geometry.h>
#include <FrameWork/OGLFW/Memory/oglfw_EBO_base.h>
#include <FrameWork/OGLFW/Memory/oglfw_texture.hpp>

#include <FrameWork/AFW/Geometry/afw_vertex.h>
#include <FrameWork/AFW/Geometry/afw_vertex_indices.h>

class oglfw_plane_class : public oglfw_entity_object_geometry_base_class {
public:
    oglfw_plane_class() {}
    ~oglfw_plane_class() { delete_geometry(), delete_buffers(); }

    struct Vertex {
        glm::vec3 position = { 0.0f,0.0f,0.0f };
        glm::vec2 texture_coordinates;
    };

    afw_vertices_base_class<Vertex> geometry;
    afw_vertex_indices_base_class   indices;

    bool initialise() override {
        geometry.add_vertices(mVertices);
        indices.add_indices(mElements);

        define_buffers();

        return true;
    }

    bool update_uniforms() override { 
        auto plane_model = glm::mat4(1.0f);
        plane_model = glm::scale(plane_model, glm::vec3(8.0f, 8.0f, 8.0f));
        plane_model = glm::translate(plane_model, glm::vec3(0.0f, 0.0f, 0.0f));
        plane_model = glm::rotate(plane_model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

        //shader.SetMat4("u_model", plane_model);
        shader->set_mat4(shader->get_program(), plane_model, "u_model");// +++++++++++++++    
        return true; 
    }

    void delete_geometry() {
        geometry.delete_vertex_data();
        indices.clear_indices();
    }

    void render_geometry() override {
        update_uniforms();

        vertex_VAO.Bind();
        mTexture.Bind();
        //printf(" render_geometry : before glDrawElements\n");
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.get_indices().size()), GL_UNSIGNED_INT, static_cast<void*>(nullptr));
        //printf(" render_geometry : Error code :: %i \n",err);

        mTexture.Unbind();
        vertex_VAO.Unbind();
    }

private:
    oglfw_EBO_class EBO;
    //Texture mTexture{ "assets/textures/texture.png" };
    oglfw_texture_class mTexture;

    const std::vector<Vertex> mVertices{
            {{-0.5f, +0.0f, -0.5f}, {+0.0f, +0.0f}},// 0
            {{+0.5f, +0.0f, -0.5f}, {+1.0f, +0.0f}},// 1
            {{-0.5f, +0.0f, +0.5f}, {+0.0f, +1.0f}},// 2
            {{+0.5f, +0.0f, +0.5f}, {+1.0f, +1.0f}},// 3
    };

    const std::vector<GLuint> mElements{
            0,// 0
            1,// 1
            2,// 2
            2,// 3
            1,// 4
            3,// 5
    };

    void define_buffers() override {
        // to correctly render an object the defining the VBO, VAO and 
        // textures etc must be performrd in the correct order as here

        generate_buffers();

        bind_buffers();

        define_buffer_data();

        define_VBO_buffer_data_layout();

        load_texture_data();
    }

    void  generate_buffers()  {
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
        vertex_VAO.SetVec3(0, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
        /// texture attribute
        vertex_VAO.SetVec2(1, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, texture_coordinates)));
    }

    void load_texture_data() {
        ///Load Textures
        mTexture.define_texture("assets/textures/texture.png");
    }


    void delete_buffers() override {
        vertex_VAO.Delete();
        vertex_VBO.Delete();
        EBO.Delete();
        mTexture.Delete();
    };
};

/*

    // To correctly render an object the defining the VBO, VAO and
    // textures etc must be performrd in the correct order as here

   ///Generate VBO
    glGenBuffers(1, &VBO);

    ///Generate VAO
    glGenVertexArrays(1, &VAO);

    ///Generate EBO
    glGenBuffers(1, &EBO);

    ///First Bind the VAO, so that all the configuration is saved in this VAO
    glBindVertexArray(VAO);

    ///Bind the VBO to GL_ARRAY_BUFFER
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    ///Bind EBO to GL_ELEMENT_ARRAY_BUFFER
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    ///Populate VBO with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices,GL_STATIC_DRAW);

    ///Populate EBO with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    ///Set the info of how the VBO must be read
    /// position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    /// color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
    /// texture attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)(6* sizeof(float)));
    glEnableVertexAttribArray(2);
*/