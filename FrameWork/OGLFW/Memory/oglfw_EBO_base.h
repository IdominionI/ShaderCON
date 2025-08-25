#pragma once

//#define GLEW_STATIC
//#include <GL/glew.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

#include <vector>

/* 
    Define vertx indices data buffer object

   This is a version of VBO 
   may not be needed as target GL_ELEMENT_ARRAY_BUFFER
   can be defined for a oglfw_VBO_class object
*/

//template<typename T>
class oglfw_EBO_class {
public:
    //explicit oglfw_EBO_class(const std::vector<T>& elements);

    template<typename T>
    void define_EBO_data(std::vector<T>& indices, GLenum usage = GL_STATIC_DRAW) {
        //glGenBuffers(1, &mID);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(T)), indices.data(), usage);
        //target = buffer_target
    }

    void generate() {
        glGenBuffers(1, &mID);
    }

    void Bind() {
        //glBindBuffer(GL_ARRAY_BUFFER, mID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
    }

    void Unbind() {
        //glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void Delete() {
        glDeleteBuffers(1, &mID);
    }

    //GLuint get_target() { return GL_ELEMENT_ARRAY_BUFFER; }
    GLuint get_id() { return mID; }

//private:
    GLuint mID = 0;
    //GLenum target = GL_ELEMENT_ARRAY_BUFFER;
};
/*
template<typename T>
oglfw_EBO_class<T>::oglfw_EBO_class(const std::vector<T>& elements) {
    glGenBuffers(1, &mID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(elements.size() * sizeof(T)), elements.data(), GL_STATIC_DRAW);
}

template<typename T>
void oglfw_EBO_class<T>::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mID);
}

template<typename T>
void oglfw_EBO_class<T>::Delete() const {
    glDeleteBuffers(1, &mID);
}

template<typename T>
void oglfw_EBO_class<T>::Unbind() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
*/