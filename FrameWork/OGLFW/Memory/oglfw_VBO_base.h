#pragma once

//#define GLEW_STATIC
//#include <GL/glew.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

#include <vector>

/*
   buffer_target
GL_ARRAY_BUFFER 	            Vertex attributes
GL_ATOMIC_COUNTER_BUFFER 	    Atomic counter storage
GL_COPY_READ_BUFFER 	        Buffer copy source
GL_COPY_WRITE_BUFFER 	        Buffer copy destination
GL_DISPATCH_INDIRECT_BUFFER 	Indirect compute dispatch commands
GL_DRAW_INDIRECT_BUFFER 	    Indirect command arguments
GL_ELEMENT_ARRAY_BUFFER 	    Vertex array indices
GL_PIXEL_PACK_BUFFER 	        Pixel read target
GL_PIXEL_UNPACK_BUFFER 	        Texture data source
GL_QUERY_BUFFER 	            Query result buffer
GL_SHADER_STORAGE_BUFFER 	    Read-write storage for shaders
GL_TEXTURE_BUFFER 	            Texture data buffer
GL_TRANSFORM_FEEDBACK_BUFFER 	Transform feedback buffer
GL_UNIFORM_BUFFER 	            Uniform block storage

usage :
GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, GL_STATIC_DRAW, 
GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, GL_DYNAMIC_READ,
GL_DYNAMIC_COPY

*/


//template<typename T>
class oglfw_VBO_class {
public:
    oglfw_VBO_class() {}
    //explicit oglfw_VBO_class(const std::vector<T> &vertices);
    ~oglfw_VBO_class() { Delete(); }

    template<typename T>
    void define_VBO_data(GLuint buffer_target, std::vector<T> &vertices, GLenum usage = GL_STATIC_DRAW) {
        //glGenBuffers(1, &mID);
        //glBindBuffer(buffer_target, mID);
        glBufferData(buffer_target, static_cast<GLsizeiptr>(vertices.size() * sizeof(T)), vertices.data(), usage);
        target = buffer_target;
    }

    void define_empty_VBO_buffer(GLuint buffer_target, unsigned int buffer_size, GLenum usage = GL_STATIC_DRAW) {
        glBufferData(buffer_target, buffer_size, NULL, usage); //GCE;
        target = buffer_target;
    }

    //void setAttributeData(int location, const float* attrib0x, int numCoords, int total, int usage, int stride) {
    void setAttributeData(int location,void *attrib0x, int numCoords, int total, int usage, int stride) {
        //if (ofIsGLProgrammableRenderer() && location == ofShader::POSITION_ATTRIBUTE) {
        //    totalVerts = total;
        //}

        //bool normalize = false;
        //if (ofIsGLProgrammableRenderer() && !hasAttribute(location)) {
        //    vaoChanged = true;
        //    bUsingVerts |= (location == ofShader::POSITION_ATTRIBUTE);
        //    bUsingColors |= (location == ofShader::COLOR_ATTRIBUTE);
        //    bUsingNormals |= (location == ofShader::NORMAL_ATTRIBUTE);
        //    bUsingTexCoords |= (location == ofShader::TEXCOORD_ATTRIBUTE);
        //}

        //getOrCreateAttr(location).setData(attrib0x, numCoords, total, usage, stride, normalize);

        //setData(attrib0x, numCoords, total, usage, stride, normalize);

        //bind(this->data->lastTarget);
        //glBufferData(this->data->lastTarget, bytes, data, usage);
        //unbind(this->data->lastTarget);

        glBindBuffer(target,location);// bind 
        glBufferData(target, numCoords * total *stride, attrib0x, usage);
        glBindBuffer(target, 0);// unbind
    }

/*
    //--------------------------------------------------------------
    //VertexAttribute& getOrCreateAttr(int location) {
    //    VertexAttribute* attr = nullptr;
    //    if (ofIsGLProgrammableRenderer()) {
    //        switch (location) {
    //        case ofShader::POSITION_ATTRIBUTE:
    //            attr = &positionAttribute;
    //            break;
    //        case oglfw_shader_class::COLOR_ATTRIBUTE:
    //            attr = &colorAttribute;
    //            break;
    //        case oglfw_shader_class::NORMAL_ATTRIBUTE:
    //            attr = &normalAttribute;
    //            break;
    //        case oglfw_shader_class::TEXCOORD_ATTRIBUTE:
    //            attr = &texCoordAttribute;
    //            break;
    //        default:
    //            customAttributes[location].location = location;
    //            attr = &customAttributes[location];
    //            vaoChanged = true;
    //            break;
    //        }
    //    }
    //    else {
    //        customAttributes[location].location = location;
    //        attr = &customAttributes[location];
    //        vaoChanged = true;
    //    }
    //    return *attr;
    //}

    void setData(const float* attrib0x, int numCoords, int total, int usage, int stride, bool normalize) {
        //if (!isAllocated()) {
        //    allocate();
        //}
        GLsizeiptr size = (stride == 0) ? numCoords * sizeof(float) : stride;
        this->stride = size;
        this->numCoords = numCoords;
        this->offset = 0;
        this->normalize = normalize;
        setData(total * size, attrib0x, usage);
    };

    GLsizei stride;
    GLsizeiptr offset;
    int numCoords;
    GLuint location;
    bool normalize;
    int divisor;

    // -------------
    void setData(GLsizeiptr bytes, const void* data, GLenum usage) {
        //if (!this->data) return;
        //this->data->size = bytes;

//#ifdef GLEW_VERSION_4_5
//        if (this->data->isDSA) {
//            glNamedBufferData(this->data->id, bytes, data, usage);
//            return;
//        }
//#endif

        /// --------| invariant: direct state access is not available
        bind(this->data->lastTarget);
        glBufferData(this->data->lastTarget, bytes, data, usage);
        unbind(this->data->lastTarget);
    }
*/
    // =================================

    void generate() {
        glGenBuffers(1, &mID);
    }

    void Bind() {
        //glBindBuffer(GL_ARRAY_BUFFER, mID);
        glBindBuffer(target, mID);
    }

    void Unbind() {
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(target, 0);
    }

    void Delete() {
        glDeleteBuffers(1, &mID);
    }

    GLuint get_target() { return target; }
    GLuint get_id()     { return mID; }
    
//private:
    GLuint mID    = 0;
    GLenum target = GL_ARRAY_BUFFER;
};

/*
template<typename T>
oglfw_VBO_class<T>::oglfw_VBO_class(const std::vector<T> &vertices) {
    glGenBuffers(1, &mID);
    glBindBuffer(GL_ARRAY_BUFFER, mID);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(vertices.size() * sizeof(T)), vertices.data(), GL_STATIC_DRAW);
}

template<typename T>
void oglfw_VBO_class<T>::Bind() const {
    //glBindBuffer(GL_ARRAY_BUFFER, mID);
    glBindBuffer(target, mID);
}

template<typename T>
void oglfw_VBO_class<T>::Delete() const {
    glDeleteBuffers(1, &mID);
}

template<typename T>
void oglfw_VBO_class<T>::Unbind() {
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(target, 0);
}
*/