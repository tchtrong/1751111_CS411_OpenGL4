#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <iostream>
#include "Program.h"
#include "Cube.h"
#include "Cone.h"
#include "Sphere.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    GLenum err = glewInit();
    if (GLEW_OK != err) {

    }

    Program program;

    program.add_shader("vertex.glsl", GL_VERTEX_SHADER);
    program.add_shader("fragment.glsl", GL_FRAGMENT_SHADER);

    program.link_program();

    program.use_program();

    GLint mv_location = glGetUniformLocation(program.get_id(), "mv_matrix");
    GLint proj_location = glGetUniformLocation(program.get_id(), "proj_matrix");

    static const GLushort vertex_indices[] =
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

    static const GLfloat vertex_positions[] =
    {
        -0.25f, -0.25f, -0.25f,
        -0.25f,  0.25f, -0.25f,
         0.25f, -0.25f, -0.25f,
         0.25f,  0.25f, -0.25f,
         0.25f, -0.25f,  0.25f,
         0.25f,  0.25f,  0.25f,
        -0.25f, -0.25f,  0.25f,
        -0.25f,  0.25f,  0.25f,
    };

    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CW);

    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glm::mat4 proj_matrix = glm::perspective(50.0f,
        (float)640 / (float)480,
        0.01f,
        1000.0f);

    glUniformMatrix4fv(proj_location, 1, GL_FALSE, &proj_matrix[0][0]);

    glViewport(0, 0, 640, 480);

    double currentTime = glfwGetTime();

    Cube a;

    Cone b;

    Shpere c;

    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        currentTime = (glfwGetTime() - currentTime) / 60;

        static const GLfloat green[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        static const GLfloat one = 1.0f;

        glClearBufferfv(GL_COLOR, 0, green);
        glClearBufferfv(GL_DEPTH, 0, &one);
        glClearBufferfi(GL_DEPTH_STENCIL, 0, 1.0f, 0);

        float f = (float)currentTime * 0.3f * 60;
        glm::mat4 mv_matrix =
            glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f)) *
            glm::translate(glm::mat4(1.0f),
                           glm::vec3(sinf(2.1f * f) * 0.5f,
                                     cosf(1.7f * f) * 0.5f,
                                     sinf(1.3f * f) * cosf(1.5f * f) * 2.0f)) *
            glm::rotate(glm::mat4(1.0f), (float)currentTime * 45.0f, glm::vec3(0.0f, 1.0f, 0.0f)) *
            glm::rotate(glm::mat4(1.0f), (float)currentTime * 81.0f, glm::vec3(1.0f, 0.0f, 0.0f));

        glUniformMatrix4fv(mv_location, 1, GL_FALSE, &mv_matrix[0][0]);

        glEnable(GL_CULL_FACE);
        //a.render();

        glDisable(GL_CULL_FACE);
        //b.render();
        c.render();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    /*glDeleteBuffers(1, &buffer);
    glDeleteBuffers(1, &index_buffer);
    glDeleteVertexArrays(1, &vao);*/

    a.uninitialize();
    b.uninitialize();
    c.uninitialize();
    program.delete_program();

    glfwTerminate();
    return 0;
}