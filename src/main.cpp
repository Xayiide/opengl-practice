#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp> /* mates 3D */

using namespace glm; /* en lugar de glm3::vec3, sólo vec3 */

int main()
{
    GLFWwindow *window;

    if (glfwInit() == GL_FALSE) {
        fprintf(stderr, "Error al inicializar GLFW.\n");
        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_SAMPLES, 4); /* 4x antialiasing */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); /* Queremos OpenGL 3.3 */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); /* MacOS */
    glfwWindowHint(GLFW_OPENGL_PROFILE,
                   GLFW_OPENGL_CORE_PROFILE); /* No queremos el viejo OpenGL */

    /* Abre una ventana y crea su contexto OpenGL */
    window = glfwCreateWindow(1024, 768, "Pruebas", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Error al crear la ventana.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window); /* Inicializa GLEW */
    glewExperimental = true; /* Necesario para perfil core */

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Error al inicializar GLEW.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    do {
        glClear(GL_COLOR_BUFFER_BIT); /* Limpia la pantalla */

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &
             glfwWindowShouldClose(window) == 0);

}

