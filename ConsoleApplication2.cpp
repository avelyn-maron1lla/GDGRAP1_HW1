#include <glad/glad.h>
#include <GLFW/glfw3.h>
/*
    Quiz 1 - February 1, 2024
    Maronilla, Mary Avelyn B.
    Author's Notes: I think I'm at my limit here. I'm pretty sure all this takes is
                    basic math but WOW do I have absolutely no idea how to implement this
                    properly. 
                    I think it's rotating? Just on the completely wrong axis. And that
                    sucks rip
                    In hindsight I shouldn't have bothered with the 3d rotation since
                    in principle all I need is the while loop for the transformation
                    I think
                    I'll try that right now actually -
*/


//GLM HEADERS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//libraries
#include <string>
#include <iostream>
#include <math.h>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

//INITIALIZE BASE FLOATS
float x = 0.0f;
float y = 0.0f;
float z = -2.5f;

float scale_x = 0.5f;
float scale_y = 0.5f;
float scale_z = 0.5f;

float theta = 5.0f;

float w_height = 600.0f;
float w_width = 600.0f;

//INITIALIZE MOD DOUBLES
float a;
float b;

//INITIALIZE OTHER STUFF
int counter;

//4x4 identity matrix
glm::mat4 identity_matrix = glm::mat4(1.0f);

//perspective matrix
glm::mat4 projectionMatrix = glm::perspective(
    glm::radians(60.0f),    //FOV
    w_height/w_width,       //Aspect Ratio
    0.1f,                   //Znear >= 0
    100.0f                  //Zfar
);

void calcRotation() {

    counter += 0.1;

    //new floats
    float alpha = 0;
    float delta;
    float radius = 1;

    float i = radius * cos(theta);
    float j = 0;
    float k = radius * sin(theta);

    a = x * cos(alpha) - x * sin(alpha);
    b = z * cos(alpha) + z * sin(alpha);

    alpha += counter;

}

//yeah ok that didn't work. unfortunate

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
        calcRotation();

        //first rotation
        glm::mat4 transformation_matrix = glm::scale(
            identity_matrix,
            glm::vec3(scale_x, scale_y, scale_z)
        );

        //glm::mat4
        transformation_matrix = glm::rotate(
            transformation_matrix,
            0.0f,
            glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f))
        );
        
        //second rotation
        transformation_matrix = glm::rotate(
            transformation_matrix,
            glm::radians(theta += 1.0f),
            glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f))
        );
        
        //translation
        transformation_matrix = glm::translate(
            transformation_matrix,
            glm::vec3(a, 0.0f, b)
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