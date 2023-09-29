#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp> /* GLM TCT Transformación matrices */
#include <glm/glm.hpp>                  /* mates 3D */

#include "shader.hpp"

int main()
{
    GLFWwindow *window;
    GLuint      VertexArrayID;
    GLuint      vertexbuffer;
    GLuint      colorbuffer;
    GLuint      programID;
    GLuint      matrixID;
    glm::mat4   Projection;
    glm::mat4   View;
    glm::mat4   Model;
    glm::mat4   MVP;

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, -1.0f, // triangle 1 : begin
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f, // triangle 1 : end
         1.0f,  1.0f, -1.0f, // triangle 2 : begin
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f, // triangle 2 : end
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,- 1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    static const GLfloat g_color_buffer_data[] = {
        0.583f,  0.771f,  0.014f,
        0.609f,  0.115f,  0.436f,
        0.327f,  0.483f,  0.844f,
        0.822f,  0.569f,  0.201f,
        0.435f,  0.602f,  0.223f,
        0.310f,  0.747f,  0.185f,
        0.597f,  0.770f,  0.761f,
        0.559f,  0.436f,  0.730f,
        0.359f,  0.583f,  0.152f,
        0.483f,  0.596f,  0.789f,
        0.559f,  0.861f,  0.639f,
        0.195f,  0.548f,  0.859f,
        0.014f,  0.184f,  0.576f,
        0.771f,  0.328f,  0.970f,
        0.406f,  0.615f,  0.116f,
        0.676f,  0.977f,  0.133f,
        0.971f,  0.572f,  0.833f,
        0.140f,  0.616f,  0.489f,
        0.997f,  0.513f,  0.064f,
        0.945f,  0.719f,  0.592f,
        0.543f,  0.021f,  0.978f,
        0.279f,  0.317f,  0.505f,
        0.167f,  0.620f,  0.077f,
        0.347f,  0.857f,  0.137f,
        0.055f,  0.953f,  0.042f,
        0.714f,  0.505f,  0.345f,
        0.783f,  0.290f,  0.734f,
        0.722f,  0.645f,  0.174f,
        0.302f,  0.455f,  0.848f,
        0.225f,  0.587f,  0.040f,
        0.517f,  0.713f,  0.338f,
        0.053f,  0.959f,  0.120f,
        0.393f,  0.621f,  0.362f,
        0.673f,  0.211f,  0.457f,
        0.820f,  0.883f,  0.371f,
        0.982f,  0.099f,  0.879f
    };

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

    /* Inicializa GLEW */
    glewExperimental = true; /* Necesario para perfil core */
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Error al inicializar GLEW.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    /* Para poder capturar la pulsación de teclas */
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    /* Color negro de fondo */
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    /* Activa pruebas de profundidad (para renderizar en el orden correcto) */
    glEnable(GL_DEPTH_TEST);
    /* Aceptar fragmentos si está más cerca de la cámara que el anterior */
    glDepthFunc(GL_LESS);

    /* Crea Vertex Array Object (VAO) y ponerlo como primero */
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    programID = LoadShaders("vertexShader.glsl",
                            "fragmentShader.glsl");

    /* Model View Projection */
    matrixID = glGetUniformLocation(programID, "MVP");

    /* Matriz de proyección: 45º campo visión, ratio 4:3, rango: 0.1 - 100 u */
    Projection = glm::perspective(glm::radians(45.0f),
                                  4.0f / 3.0f,
                                  0.1f,
                                  100.0f);
    /* Para cámara ortogonal (en coordenadas mundo): */
    /* glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 0.0f, 100.0f) */

    /* Matriz de la cámara */
    View = glm::lookAt(
        glm::vec3(4, 3, 3), /* Cámara en (4, 3, 3) en espacio Mundo   */
        glm::vec3(0, 0, 0), /* Mirando al origen                      */
        glm::vec3(0, 1, 0)  /* La cabeza está recta. -1 es boac abajo */
    );

    /* Matriz modelo */
    Model = glm::mat4(1.0f);

    /* Model View Projection */
    MVP = Projection * View * Model;

    /* "Damos" el triángulo a OpenGL */
    /* Genera 1 buffer y pone el identificador en vertexbuffer */
    glGenBuffers(1, &vertexbuffer);
    /* Haz que los siguientes comandos se dirigan a vertexbuffer */
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    /* Da nuestros vértices a OpenGL */
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data),
            g_vertex_buffer_data, GL_STATIC_DRAW);

    /* Hacemos lo mismo con el color */
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data),
            g_color_buffer_data, GL_STATIC_DRAW);



    do {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); /* Limpia la pantalla */

        /* Usa nuestro shader */
        glUseProgram(programID);

        /* Manda nuestra transformación al shader actual */
        glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
            0,         /* Tiene que ser igual al layout del shader */
            3,         /* Tamaño                                   */
            GL_FLOAT,  /* Tipo                                     */
            GL_FALSE,  /* ¿normalizado?                            */
            0,         /* Stride                                   */
            (void *) 0 /* Desplazamiento del array buffer          */
        );

        /* Lo mismo pero con el color */
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,
            3,
            GL_FLOAT,
            GL_FALSE,
            0,
            (void *) 0
        );

        /* Dibuja el triángulo */
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3); /* 12 triángulos para el cubo */

        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while ((glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS ) &&
             (glfwWindowShouldClose(window) == 0));

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);


    /* Cierra OpenGL */
    glfwTerminate();
}

