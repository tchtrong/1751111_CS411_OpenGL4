#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

class Cone {
public:
	Cone() {
		init_vao();
		init_vertex_buffer();
		init_index_buffer();
		bind_buffer();
	}

	void uninitialize() {
		glDeleteBuffers(1, &index_buffer);
		glDeleteBuffers(1, &vertex_buffer);
		glDeleteVertexArrays(1, &vao);
	}

	void render() {
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, edges * 3, GL_UNSIGNED_INT, indices);
		glDrawElements(GL_TRIANGLES, edges * 3, GL_UNSIGNED_INT, bottom_indices);
	}

private:

	void init_vertex_buffer() {
		// Calculate vertices buffer
		static constexpr glm::mat4 identity(1.0f);
		glm::vec4 start{ 0.5f, 0.0f, 0.0f, 1.0f };
		for (int i = 0; i < edges; ++i) {
			if (i > 0) {
				start = glm::rotate(identity, incr_angle, axis) * start;
			}
			vertices[i][0] = start[0];
			vertices[i][1] = start[1];
			vertices[i][2] = start[2];
			vertices[i][3] = start[3];
		}

		vertices[edges][0] = top[0];
		vertices[edges][1] = top[1];
		vertices[edges][2] = top[2];
		vertices[edges][3] = top[3];

		vertices[edges + 1][0] = center[0];
		vertices[edges + 1][1] = center[1];
		vertices[edges + 1][2] = center[2];
		vertices[edges + 1][3] = center[3];

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(vertices), vertices, GL_MAP_READ_BIT);
	}

	void init_index_buffer() {
		//Calculate indices buffer
		for (int i = 0; i < edges * 3; ++i) {
			if (i % 3 == 2) {
				indices[i] = edges;
			}
			else {
				indices[i] = (i % 3 + i / 3) % edges;
			}
		}

		glCreateBuffers(1, &index_buffer);
		glNamedBufferStorage(index_buffer, sizeof(indices), indices, GL_MAP_READ_BIT);

		//Bottom
		for (int i = 0; i < edges * 3; ++i) {
			if (i % 3 == 2) {
				bottom_indices[i] = edges + 1;
			}
			else {
				bottom_indices[i] = (i % 3 + i / 3) % edges;
			}
		}

		glCreateBuffers(1, &bottom_index_buffer);
		glNamedBufferStorage(bottom_index_buffer, sizeof(bottom_indices), bottom_indices, GL_MAP_READ_BIT);
	}

	void init_vao() {
		glCreateVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	void bind_buffer() {
		glVertexArrayVertexBuffer(vao, 0, vertex_buffer, 0, sizeof(float[4]));
		glVertexArrayAttribFormat(vao, 0, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(vao, 0, 0);

		glEnableVertexArrayAttrib(vao, 0);
	}

	glm::vec4 top{ 0.0f, 1.0f, 0.0f, 1.0f };
	glm::vec4 center{ 0.0f, 0.0f, 0.0f, 1.0f };
	static constexpr inline glm::vec3 axis = { 0.0f, 1.0f, 0.0f };
	static constexpr inline int edges = 40;
	static constexpr inline float incr_angle = glm::radians(360 / float(edges));

	//Buffer
	GLfloat vertices[edges + 2][4]{};
	GLuint indices[edges * 3]{};
	GLuint bottom_indices[edges * 3]{};
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint bottom_index_buffer;

	//VAO
	GLuint vao;
};