#include <stdio.h>
#include <stdlib.h> /* rand */
#include <time.h>   /* time */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp> /* GLM TCT Transformación matrices */
#include <glm/glm.hpp>                  /* mates 3D */

#include "shader.hpp"

static void randomize_color(GLfloat buffer[]);

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
    int         frames = 1;

    srand((unsigned int) time(NULL));

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

    static GLfloat g_color_buffer_data[12 * 3 * 4];
    randomize_color(g_color_buffer_data);

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


        frames++;
        if (frames % 24 == 0) {
            randomize_color(g_color_buffer_data);
            glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data),
                    g_color_buffer_data, GL_STATIC_DRAW);
            frames = 0;
        }


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


static void randomize_color(GLfloat buffer_data[])
{
    int i;

    for (i = 0; i < 12 * 3; i++) {
        buffer_data[3 * i + 0] = (float) (rand() / (float) RAND_MAX);
        buffer_data[3 * i + 1] = (float) (rand() / (float) RAND_MAX);
        buffer_data[3 * i + 2] = (float) (rand() / (float) RAND_MAX);
    }
}
