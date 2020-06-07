#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

class Shpere {
public:
	Shpere() {
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
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, indices.data());
	}

private:

	void init_vertex_buffer() {
		// Calculate vertices buffer
		glm::vec4 top{ 0.0f, 0.35f, 0.0f, 1.0f };
		glm::mat4 identity{ 1.0f };
		glm::mat4 matrix = glm::translate(identity, glm::vec3(0.0f, -1.0f, 0.0f));
		for (int i = 0; i < vertical + 1; ++i) {			
			if (!i) {
				vertices[i] = matrix * top;
			}
			else {
				top = glm::rotate(identity, vtc_angle, x_axis) * top;
				if (i == vertical) {
					vertices[num_vertices - 1] = matrix * top;
				}
				else {
					int index = (i - 1) * horizontal + 1;
					for (int j = 0; j < horizontal; ++j) {
						if (j == 0) {
							vertices[index] = matrix * top;
						}
						else {
							top = glm::rotate(identity, hrz_angle, y_axis) * top;
							vertices[index + j] = matrix * top;
						}
					}
					top = glm::rotate(identity, hrz_angle, y_axis) * top;
				}
			}
		}
		glCreateBuffers(1, &vertex_buffer);
		glNamedBufferStorage(vertex_buffer, sizeof(vertices), vertices, GL_MAP_READ_BIT);
	}

	void init_index_buffer() {
		//Calculate indices buffer
		indices.reserve(num_indices);

		for (int i = 0, k1 = 0, k2 = 0; i < vertical; ++i) {
			if (!i) {
				k1 = 0;
				k2 = 1;
			}
			else {
				k1 = (i - 1) * horizontal + 1;
				k2 = k1 + horizontal;
			}
			bool not_zero = i;
			bool not_last = !(i == vertical - 1);
			for (int j = 0; j < horizontal; ++j) {
				if (not_last) {
					indices.push_back(k1);
					indices.push_back(k2);
					indices.push_back(k2 + 1 - horizontal * bool(j == horizontal - 1));
				}

				if (not_zero) {
					indices.push_back(k1 + 1 - horizontal * bool(j == horizontal - 1));
					indices.push_back(k1);
					indices.push_back(k2 + not_last - horizontal * bool(j == horizontal - 1));
				}

				k1 += not_zero;
				k2 += not_last;
			}
		}
		indices.back() = num_vertices - 1;

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

	static constexpr inline glm::vec3 y_axis = { 0.0f, 1.0f, 0.0f };
	static constexpr inline glm::vec3 x_axis = { 1.0f, 0.0f, 0.0f };
	static constexpr inline int horizontal = 40;
	static constexpr inline int vertical = 20;
	static constexpr inline int num_vertices = horizontal * (vertical - 1) + 2;
	static constexpr inline int num_indices = horizontal * vertical * 3;
	static constexpr inline float hrz_angle = glm::radians(360 / float(horizontal));
	static constexpr inline float vtc_angle = glm::radians(180 / float(vertical));

	//Buffer
	glm::vec4 vertices[num_vertices]{};
	std::vector<GLuint> indices;
	GLuint vertex_buffer;
	GLuint index_buffer;
	GLuint bottom_index_buffer;

	//VAO
	GLuint vao;
};