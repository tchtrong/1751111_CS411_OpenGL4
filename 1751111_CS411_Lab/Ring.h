#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

class Ring {
public:
	Ring() {
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
		glDrawElements(GL_TRIANGLES, int(indices.size()), GL_UNSIGNED_INT, indices.data());
	}

private:

	void init_vertex_buffer() {
		// Calculate vertices buffer
		glm::mat4 identity{ 1.0f };
		glm::vec4 start{ t_radius, 1.0f };
		glm::mat4 trans = glm::translate(identity, m_radius - t_radius);

		for (int i = 0; i < t_segments; ++i) {
			vertices[i] = trans * glm::rotate(identity, t_angle * i, z_axis) * start;
		}

		glm::mat4 rotate_m = glm::rotate(identity, m_angle, y_axis);

		for (int i = 1; i < m_segments; ++i) {
			for (int j = 0; j < t_segments; ++j) {
				vertices[i * t_segments + j] = rotate_m * vertices[(i - 1) * t_segments + j];
			}
		}

		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(vertices), vertices, GL_MAP_READ_BIT);
	}

	void init_index_buffer() {
		//Calculate indices buffer
		indices.reserve(num_indices);
		for (int i = 0; i < m_segments; ++i) {
			int k1 = i * t_segments;					//begining index
			int k2 = (k1 + t_segments) % num_vertices;	//begining of first index of next m_segment			
			for (int j = 0; j < t_segments; ++j) {
				int tmp = j == t_segments - 1 ? t_segments : 0;
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back((k2 + 1) - tmp);

				indices.push_back(k1);
				indices.push_back((k2 + 1) - tmp);
				indices.push_back((k1 + 1) - tmp);

				++k1;
				++k2;
			}
		}
		glCreateBuffers(1, &index_buffer);
		glNamedBufferStorage(index_buffer, sizeof(GLuint) * indices.size(), indices.data(), GL_MAP_READ_BIT);
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

	static constexpr inline glm::vec3 m_radius{ 1.0f, 0.0f, 0.0f };
	static constexpr inline glm::vec3 t_radius{ 0.25f, 0.0f, 0.0f };
	static constexpr inline glm::vec3 y_axis = { 0.0f, 1.0f, 0.0f };
	static constexpr inline glm::vec3 x_axis = { 1.0f, 0.0f, 0.0f };
	static constexpr inline glm::vec3 z_axis = { 0.0f, 0.0f, 1.0f };
	static constexpr inline int m_segments = 40;
	static constexpr inline int t_segments = 20;
	static constexpr inline int num_vertices = m_segments * t_segments;
	static constexpr inline int num_indices = num_vertices * 2 * 3;
	static constexpr inline float m_angle = glm::radians(360 / float(m_segments));
	static constexpr inline float t_angle = glm::radians(360 / float(t_segments));

	//Buffer
	glm::vec4 vertices[num_vertices]{};
	std::vector<GLuint> indices;
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint bottom_index_buffer;

	//VAO
	GLuint vao;
};