#include <glad/glad.h>
#include <GLFW/glfw3.h>

//GLM HEADERS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//libraries
#include <string>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//INITIALIZE
float x = 0.0f;
float y = 0.0f;
float z = -2.5f;

float scale_x = 1.0f;
float scale_y = 1.0f;
float scale_z = 1.0f;

float axis_x = 0.0f;
float axis_y = 1.0f;
float axis_z = 0.0f;

float theta = 90.0f;

float w_height = 600.0f;
float w_width = 600.0f;

//4x4 identity matrix
glm::mat4 identity_matrix = glm::mat4(1.0f);

//orthographic matrix
/*glm::mat4 projectionMatrix = glm::ortho(
    -2.0f,   //left
    2.0f,    //right
    -2.0f,   //bottom
    2.0f,    //top
    -1.0f,   //near
    1.0f     //far
);*/

//perspective matrix
glm::mat4 projectionMatrix = glm::perspective(
    glm::radians(60.0f),    //FOV
    w_height/w_width,       //Aspect Ratio
    0.1f,                   //Znear >= 0
    100.0f                  //Zfar
);

void Key_Callback(GLFWwindow* window,   //Pointer to the window
    int key,                            //Keycode of the press
    int scancode,                       //Physical Position of the Press
    int action,                         //Press/Release 
    int mods)                           //Which keys are held down
{
    //Then there's this entire rodeo
    //add && action == GLFW_PRESS to argument if needed

    //WASD
    if (key == GLFW_KEY_D) {
        //move bun bun to right
        x += 0.01f;
    }
    if (key == GLFW_KEY_A) {
        //move bun bun to left
        x += -0.01f;
    }
    if (key == GLFW_KEY_W) {
        //move bun bun up
        y += 0.01f;
    }
    if (key == GLFW_KEY_S) {
        //move bun bun down
        y += -0.01f;
    }
    
    //ROTATION
    if (key == GLFW_KEY_LEFT) {
        theta += 1.5f;
    }
    if (key == GLFW_KEY_RIGHT) {
        theta += -1.5f;
    }
    //how the frick do I do this with y what -

    //SCALE
    if (key == GLFW_KEY_Q) {
        scale_x += 0.25f;
        scale_y += 0.25f;
    }
    if (key == GLFW_KEY_E) {
        scale_x += -0.25f;
        scale_y += -0.25f;
    }

    //ZOOM
    if (key == GLFW_KEY_Z) {
        //move bun bun closer
        z += 0.1f;
    }
    else if (key == GLFW_KEY_X) {
        //move bun bun further
        z += -0.1f;
    }

}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(w_width, w_height, "MA Maronilla", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Initialize glad */
    gladLoadGL();

    //glViewport(0, 0, w_width, w_height);

    /* CALLBACK MOVE */
    glfwSetKeyCallback(window, Key_Callback);
    
    /* VERTEX SHADER */
    //Load shader file into a string stream
    std::fstream vertSrc("Shaders/sample.vert");
    std::stringstream vertBuff;
    //Add file stream to the string stream
    vertBuff << vertSrc.rdbuf();
    //Convert the stream to a character array
    std::string vertS = vertBuff.str();
    const char* v = vertS.c_str();

    /* FRAGMENT SHADER */
    //Load shader file into a string stream
    std::fstream fragSrc("Shaders/sample.frag");
    std::stringstream fragBuff;
    //Add file stream to the string stream
    fragBuff << fragSrc.rdbuf();
    //Convert the stream to a character array
    std::string fragS = fragBuff.str();
    const char* f = fragS.c_str();

    //VERTEX SHADER BUT AGAIN
    //Create a Vertex Shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //Assign Source to Vertex Shader
    glShaderSource(vertexShader, 1, &v, NULL);
    //Compile the Vertex Shader
    glCompileShader(vertexShader);

    //FRAGMENT SHADER BUT AGAIN
    //Create a Framgent Shader
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    //Assign Source to Vertex Shader
    glShaderSource(fragShader, 1, &f, NULL);
    //Compile the Vertex Shader
    glCompileShader(fragShader);

    /* CREATE SHADER PROGRAM */
    GLuint shaderProg = glCreateProgram();
    //Attach compiled Vertex Shader
    glAttachShader(shaderProg, vertexShader);
    //Attach compiled Fragment Shader
    glAttachShader(shaderProg, fragShader);

    //FINALIZE COMPILING SHADERS
    glLinkProgram(shaderProg);
    
    /* A BUNCH OF THIS */
    std::string path = "3D/bunny.obj";  //mesh relative path
    std::vector<tinyobj::shape_t> shapes;   //mesh shapes
    std::vector<tinyobj::material_t> material;  //mesh materials
    std::string warning, error; //error messages

    tinyobj::attrib_t attributes;   //mesh attributes

    bool success = tinyobj::LoadObj(
        &attributes,
        &shapes,
        &material,
        &warning,
        &error,
        path.c_str()    
    );

    /* Getting EBO indices array of OBJ */
    std::vector<GLuint> mesh_indices;
    for (int i = 0; i < shapes[0].mesh.indices.size(); i++) {
        mesh_indices.push_back(
            shapes[0].mesh.indices[i].vertex_index
        );
    }

    /* VBO, VAO, and EBO set-up */
    GLfloat vertices[]{
        //initial vertices
        0.f, 0.5f, 0.f,     //0
        -0.5f, -0.5f, 0.f,  //1
        0.5f, -0.5f, 0.f    //2

    };

    // EBO index array
    GLuint indices[]{
        0, 1, 2
    };

    // Initialize
    GLuint VAO, VBO, EBO;    //unsigned variables
    glGenVertexArrays(1, &VAO); //assigns ID to VAO
    glGenBuffers(1, &VBO); //assigns ID to VBO
    glGenBuffers(1, &EBO); //assigns ID to EBO

    //Currently editing VAO = VAO
    glBindVertexArray(VAO);
    //Currently editing VBO = VBO
    //VAO <- VBO : VBO assigned to VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //provide array
    glBufferData(GL_ARRAY_BUFFER,   //array type
        sizeof(GL_FLOAT) * attributes.vertices.size(),   //size in bytes
        attributes.vertices.data(),       // == &attributes.vertices[0]
        GL_STATIC_DRAW);

    //interpretation of array
    glVertexAttribPointer(
        0,
        3,  //data is in xyz
        GL_FLOAT,
        GL_FALSE,
        3 * sizeof(float),
        (void*)0
    );

    //Currently editing EBO = EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //VAO <- VBO, EBO : EBO assigned to VAO after VBO

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                    sizeof(GLuint) * mesh_indices.size(),
                    mesh_indices.data(), // == &mesh_indices.data[0] (??)
                    GL_STATIC_DRAW);

    //enable array
    glEnableVertexAttribArray(0);
    
    //currently editing no VBOs
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //currently editing no VAOs
    glBindVertexArray(0);
    //currently editing no EBOs
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* MOVE BUNNY */
        //translation matrix
        glm::mat4 transformation_matrix = glm::translate(
            identity_matrix,
            glm::vec3(x, y, z)
        );

        //scalar matrix
        transformation_matrix = glm::scale(
            transformation_matrix,
            glm::vec3(scale_x, scale_y, scale_z)
        );

        //rotation matrix
        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta),
            glm::normalize(glm::vec3(axis_x, axis_y, axis_z))
        );

        //projection matrix
        unsigned int projectionLoc = glGetUniformLocation(shaderProg, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
        
        //location of transform variable
        unsigned int transformLoc = glGetUniformLocation(shaderProg, "transform"); 
        glUniformMatrix4fv(transformLoc,    //address of transform variable
            1,                              //how many matrices to assign
            GL_FALSE,                       //transpose?
            glm::value_ptr(transformation_matrix));
        
        //SHADERS
        glUseProgram(shaderProg);
        glBindVertexArray(VAO);

        /* Render triangle with VAOs */
        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 
                        mesh_indices.size(), 
                        GL_UNSIGNED_INT, 
                        0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}