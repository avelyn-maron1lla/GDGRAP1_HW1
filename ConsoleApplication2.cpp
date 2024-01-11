#include <GLFW/glfw3.h>
//#include <math.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /*
    //Polygon Vertices
    float v1X = 0;
    float v1Y = 0.5;

    float v2X = v1X * (cos(18));
    float v2Y = v1Y * (sin(18));

    float v3X = v1X * (cos(54));
    float v3Y = v1Y * (sin(54));
    */

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 640, "MA Maronilla", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_POLYGON);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.4755f, 0.1045f);
        glVertex2f(0.2940f, -0.4045f);
        glVertex2f(-0.2940f, -0.4045f);
        glVertex2f(-0.4755f, 0.1045f);
        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}