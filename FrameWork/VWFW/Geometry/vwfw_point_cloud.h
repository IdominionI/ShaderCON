#pragma once

#include <FrameWork/AFW/Geometry/afw_entity_object_geometry_base.h>
#include <FrameWork/OGLFW/Geometry/oglfw_entity_object_geometry.h>

#include <FrameWork/AFW/GL/afw_material.h>

/*
                POINT CLOUD GEOMETRY CLASS

    The geometry class that can be used to store and display
    the geometry data for a point cloud object data type with
    point vertex location data only.
*/

/*
                POINT CLOUD VERTEX GEOMETRY CLASS

    The geometry class that can be used to store and display
    the geometry data for a point data cloud object data type
    with point vertex location and a normal vector value
    corresponding to that vertex location.
*/

typedef glm::vec3 point_cloud_vertex_data_type;

class point_cloud3D_class : public afw_geometry_base_class<point_cloud_vertex_data_type>, public oglfw_entity_object_geometry_base_class {
public:
    using VBC = afw_vertices_base_class<point_cloud_vertex_data_type>;
    using GBC = afw_geometry_base_class<point_cloud_vertex_data_type>;

    point_cloud3D_class() = default;
    virtual ~point_cloud3D_class() {}

    std::vector<float> values;// +++
    ofMaterialSettings material_settings;

    bool initialise() override {
        geometry_type = geometry_type_enum::points;
        generate_test_data();
        set_geometry_draw_mode(draw_mode_enum::UFW_PRIMITIVE_POINTS);// is this needed ????

        define_buffers();

        return true;
    }



    bool generate_test_data(float scale = 0.1f, float att_test = 0.0f) {
        //delete_geometry<glm::vec3>();
        GBC::delete_geometry();

        point_cloud_vertex_data_type vertex;
        //point_cloud_vertex_data_type normal;

        //geometry_attribute_type_struct vertex_attribute_data;
        //vertex_attribute_data.attribute_name        = "test_attr_data";
        //vertex_attribute_data.attribute_description = "test attribute data";

        //for (float i = -0.5f; i < 0.5f; i += 0.1f) {
        //    for (float j = -0.5f; j < 0.5f; j += 0.1f) {
        //        for (float k = -0.5f; k < 0.5f; k += 0.1f) {
        //            vertex = { i,j,k };
        //            normal = { 0.0,0.0,0.0 };

        //            add_vertex(vertex);
        //            add_normal(normal);
        //        }
        //    }
        //}

        for (float i = -100.0f; i < 100.0f; i += 10.0f) {
            for (float j = -100.0f; j < 100.0f; j += 10.0f) {
                for (float k = -100.0f; k < 100.0f; k += 10.0f) {
                    vertex = { i * scale,j * scale,k * scale };
                    //normal = { 0.0,0.0,0.0 };

                    VBC::add_vertex(vertex);
                    //add_normal(normal);
                    values.push_back(att_test);
                }
            }
        }

        //vertex_attributes.attributes.push_back(vertex_attribute_data);

        //return update_geometry();
        return true;
    }

    void add_vertex(const glm::vec3& vertex, int value) {
        VBC::add_vertex(vertex);
        add_value(float(value));
    }

    void add_value(float value) {
        values.push_back(value);
    }

    void delete_values() {
        values.clear();
    }

    void delete_geometry() {
        delete_vertex_data();
        delete_values();
    }

    bool update_buffers() {
        if (get_number_of_vertices() == 0) return false;

        delete_buffers();
        define_buffers();

        return true;
    }

    bool update_uniforms() override {
        update_material();
        return true;
    }

    void update_material() {
        //shader->setUniform4fv("mat_ambient", &material_settings.ambient.r);
        //shader->setUniform4fv("mat_diffuse", &material_settings.diffuse.r);
        //shader->setUniform4fv("mat_specular", &material_settings.specular.r);
        //shader->setUniform1f("mat_shininess", material_settings.shininess);

        shader->set_vec4(shader->program, material_settings.ambient,"mat_ambient");
        shader->set_vec4(shader->program, material_settings.diffuse,"mat_diffuse");
        shader->set_vec4(shader->program, material_settings.specular,"mat_specular");
        shader->set_f1(shader->program, material_settings.shininess, "mat_shininess");
    }

    void render_geometry() override {
        if (shader) {
            update_uniforms();
            
            shader->bind();
            vertex_VAO.Bind();

            glDrawArrays(GL_POINTS, 0, get_number_of_vertices());

//printf(" render_geometry : Error code :: %i \n",err);
            vertex_VAO.Unbind();
            shader->release();
        }
    }

protected:
    unsigned int ofs1 = 0, ofs2 = 0;
private:
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
        //EBO.generate();        ///Generate EBO
    }

    void bind_buffers() {
        vertex_VAO.Bind(); ///First Bind the VAO, so that all the configuration is saved in this VAO
        vertex_VBO.Bind(); ///Bind the VBO to GL_ARRAY_BUFFER
        //EBO.Bind();        ///Bind EBO to GL_ELEMENT_ARRAY_BUFFER
    }

    void unbind_buffers() {
        vertex_VAO.Unbind(); ///First Bind the VAO, so that all the configuration is saved in this VAO
        vertex_VBO.Unbind(); ///Bind the VBO to GL_ARRAY_BUFFER
    }

    void define_buffer_data() {
        vertex_VBO.define_empty_VBO_buffer(GL_ARRAY_BUFFER, vertices.size() * (sizeof(point_cloud_vertex_data_type) + sizeof(float)));

        glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(point_cloud_vertex_data_type), vertices.data());
        glBufferSubData(GL_ARRAY_BUFFER, vertices.size() * sizeof(point_cloud_vertex_data_type), values.size() * sizeof(float), values.data());

        ///Populate VBO with data
        //vertex_VBO.define_VBO_data(GL_ARRAY_BUFFER, get_vertices());
        
        ///Populate EBO with data
        //EBO.define_EBO_data(indices.get_indices());
    }

    void define_VBO_buffer_data_layout() {
        //Set the info of how the VBO must be read

        GLuint position_location = shader->getAttributeLocation("position");
        GLuint values_location   = shader->getAttributeLocation("voxel_values");

        if (position_location < 0) {
            printf("Vertex point size shader input location of position could note be found.\n");
            return;
        }

        if (values_location < 0) {
            printf("Vertex position shader input location of voxel_values could note be found.\n");
            return;
        }

        //printf("point_size_location :: %u\n", point_size_location);

        // position attribute
        vertex_VAO.SetVec3(position_location, sizeof(glm::vec3), (const GLvoid*)(0)); // vertex Layout location 0

        // point values attribute
        // Alternative is to comment out references to vertex point size if not in use and set ofs1 to zero
        ofs1 = vertices.size() * sizeof(float);
        vertex_VAO.SetFloat(values_location, sizeof(float), (const GLvoid*)(ofs1));// vertex Layout location 1


        ///Set the info of how the VBO must be read
        /// position attribute
       // vertex_VAO.SetVec3(0, sizeof(point_cloud_vertex_data_type), 0);
        
        //ertex_VAO.SetVec3(0, sizeof(point_cloud_vertex_data_type), reinterpret_cast<void*>(offsetof(point_cloud_vertex_data_type, position)));
        /// texture attribute
        //vertex_VAO.SetVec2(1, sizeof(point_cloud_vertex_data_type), reinterpret_cast<void*>(offsetof(Vertex, texture_coordinates)));
    }

    void delete_buffers() {
        vertex_VBO.Delete();
        vertex_VAO.Delete();
    }
};
