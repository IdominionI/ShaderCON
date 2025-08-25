#pragma once

//#define GLEW_STATIC
//#include <GL/glew.h>
//#define GLFW_INCLUDE_NONE
//#include <GLFW/glfw3.h>

class oglfw_VAO_class {
public:
    oglfw_VAO_class() {
        //glGenVertexArrays(1, &mID);
        //glBindVertexArray(mID);
    }

    void define_VAO() {
        glGenVertexArrays(1, &mID);
        glBindVertexArray(mID);
    }

    void generate() {
        glGenVertexArrays(1, &mID);
    }

    void Bind() const {
        glBindVertexArray(mID);
    }
    void Delete() const {
        glDeleteVertexArrays(1, &mID);
    }

    void Unbind() {
        glBindVertexArray(0);
    }

    void SetFloat(GLuint index, GLsizei stride, const void* pointer) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 1, GL_FLOAT, GL_FALSE, stride, pointer);
    }
    void SetVec2(GLuint index, GLsizei stride, const void* pointer) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride, pointer);
    }
    void SetVec3(GLuint index, GLsizei stride, const void* pointer) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, pointer);
    }
    void SetVec4(GLuint index, GLsizei stride, const void* pointer) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, stride, pointer);
    }

private:
    GLuint mID = 0;
};
/*
oglfw_VAO_class::oglfw_VAO_class() {
    glGenVertexArrays(1, &mID);
    glBindVertexArray(mID);
}

void oglfw_VAO_class::Bind() const {
    glBindVertexArray(mID);
}

void oglfw_VAO_class::Delete() const {
    glDeleteVertexArrays(1, &mID);
}

void oglfw_VAO_class::Unbind() {
    glBindVertexArray(0);
}

void oglfw_VAO_class::SetFloat(const GLuint index, const GLsizei stride, const void* pointer) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, 1, GL_FLOAT, GL_FALSE, stride, pointer);
}

void oglfw_VAO_class::SetVec2(const GLuint index, const GLsizei stride, const void* pointer) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride, pointer);
}

void oglfw_VAO_class::SetVec3(const GLuint index, const GLsizei stride, const void* pointer) {
    glEnableVertexAttribArray(index);
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, pointer);
}
*/