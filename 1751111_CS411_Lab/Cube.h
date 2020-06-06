#pragma once

#include <GL/glew.h>

class Cube {
public:
    Cube() {
        glCreateVertexArrays(1, &vertex_array_object);
        glBindVertexArray(vertex_array_object);

        glCreateBuffers(1, &vertex_buffer);
        glNamedBufferStorage(vertex_buffer, sizeof(vertex_positions), vertex_positions, GL_MAP_READ_BIT);

        glCreateBuffers(1, &index_buffer);
        glNamedBufferStorage(index_buffer, sizeof(vertex_indices), vertex_indices, GL_MAP_READ_BIT);

        glVertexArrayVertexBuffer(vertex_array_object, 0, vertex_buffer, 0, sizeof(float[4]));
        glVertexArrayAttribFormat(vertex_array_object, 0, 4, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayAttribBinding(vertex_array_object, 0, 0);

        glEnableVertexArrayAttrib(vertex_array_object, 0);
    }
	void render() {
        glBindVertexArray(vertex_array_object);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, vertex_indices);
	}

    void uninitialize() {
        glDeleteBuffers(1, &index_buffer);
        glDeleteBuffers(1, &vertex_buffer);
        glDeleteVertexArrays(1, &vertex_array_object);
    }

private:
	GLuint vertex_array_object;
	GLuint index_buffer;
	GLuint vertex_buffer;
    static constexpr inline GLushort vertex_indices[] =
    {
        0, 1, 2,
        2, 1, 3,
        2, 3, 4,
        4, 3, 5,
        4, 5, 6,
        6, 5, 7,
        6, 7, 0,
        0, 7, 1,
        6, 0, 2,
        2, 4, 6,
        7, 5, 3,
        7, 3, 1
    };

    static constexpr inline GLfloat vertex_positions[] =
    {
        -0.25f, -0.25f, -0.25f, 1.0f,
        -0.25f,  0.25f, -0.25f, 1.0f,
         0.25f, -0.25f, -0.25f, 1.0f,
         0.25f,  0.25f, -0.25f, 1.0f,
         0.25f, -0.25f,  0.25f, 1.0f,
         0.25f,  0.25f,  0.25f, 1.0f,
        -0.25f, -0.25f,  0.25f, 1.0f,
        -0.25f,  0.25f,  0.25f, 1.0f,
    };
};