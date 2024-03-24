
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "core/window.h"
#include "core/shader.h"
#include "core/texture.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


int main (int argc, char** argv)
{
    Window window;

    window
        .setName("LisbEngine")
        .setSize(SCR_WIDTH, SCR_HEIGHT)
        .open();


    Shader shader("assets/shaders/shader.vert", "assets/shaders/shader.frag");


    // Set up vertex data and configure vertex attrib
    // ---------------------------------------------------


    float vertices[] = {
            // positions          // colors           // boxTexture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind VAO and VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // First: The position of the vertex attrib, Second: The size of the vertex Attrib, Third> Type of data, Fourth: The stride, where the next attrib starts, the gap. Last: The offset, where the data beggins in the buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // Set the Attribute pointer for the Colors data in the Vertex Array
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof (float ), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof (float), (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);


    // Unbind VAO and VBO
    // The VBO unbind is safe because everything has been saved inside the VAO
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    

    // Load faceTexture
    // -------------------------------------
    Texture faceTexture(1);

    faceTexture
        .setWrapX(WrapParams::Repeat)
        .setWrapY(WrapParams::Repeat)
        .setMinFilter(FilterParams::LinearMipmapLinear)
        .setMagFilter(FilterParams::Linear)
        .loadImage("assets/images/awesomeface.png", GL_RGBA, GL_RGBA);


    // Load boxTexture
    // ----------------------------------------------------
    Texture boxTexture;

    boxTexture
        .setWrapX(WrapParams::Repeat)
        .setWrapY(WrapParams::Repeat)
        .setMinFilter(FilterParams::LinearMipmapLinear)
        .setMagFilter(FilterParams::Linear)
        .loadImage("assets/images/container.jpg", GL_RGB, GL_RGB);

    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);



    while (window.isOpen()) {
        if (glfwGetKey(window.it, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            window.close();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        boxTexture.use();
        faceTexture.use();

        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5, -0.5, 0.0));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));

        shader.setMatrix("transform", trans);

        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window.it);
        glfwPollEvents();
    }

    // Free resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // Terminate cleaning all previous allocate GLFW resources
    glfwTerminate();
    return 0;
}
