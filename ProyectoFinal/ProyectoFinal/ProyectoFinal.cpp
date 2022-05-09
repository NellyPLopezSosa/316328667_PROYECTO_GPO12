// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>


// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// Camera
Camera camera(glm::vec3(30.0f, 10.0f, 15.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
//Variables animación puerta principal
float trans = 0.0f;
bool anim = true;
bool anim2 = true;
//Variables animación puerta interior
float trans2 = 0.0f;
bool anim3 = true;
bool anim4 = true;
//Variables animación cajon armario
float trans3 = 0.0f;
bool anim5 = true;
bool anim6 = true;
//Variables animación puertas armario
float trans4 = 0.0f;
bool anim7 = true;
bool anim8 = true;

float tiempo;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");
    Shader lampshader("Shaders/lamp.vs", "Shaders/lamp.frag");
    Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");
    Shader lampshader2("Shaders/lamp2.vs", "Shaders/lamp2.frag");
    Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");




    // Load models
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    GLfloat vertices[] =
    {
        // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 

    };

    GLuint indices[] =
    {  // Note that we start from 0!
        0,1,3,
        1,2,3

    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Load models
    Model Casa((char*)"Models/Casa/Casa.obj");
    Model PuertaEntrada((char*)"Models/Casa/PuertaEntrada.obj");
    Model PuertaInterna((char*)"Models/Casa/PuertaInterna.obj");
    Model Tatami((char*)"Models/Tatami/Tatami.obj");
    Model Jarron((char*)"Models/Jarrón/Jarron.obj");
    Model Bambu((char*)"Models/Bambu/EstructuraBambu.obj");
    Model Cajonera((char*)"Models/Cajonera/CajoneraConTextura.obj");
    Model Lampara((char*)"Models/Lampara/Lampara.obj");
    Model Fogata((char*)"Models/Fogata/Fogata.obj");
    Model Fuego((char*)"Models/Fogata/Fuego.obj");
    Model Armario((char*)"Models/Armario/Armario.obj");
    Model CajonArmario((char*)"Models/Armario/CajonArmario.obj");
    Model PuertasArmario((char*)"Models/Armario/PuertasArmario.obj");

    // Load textures
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    int textureWidth, textureHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* image;
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model

        glm::mat4 model(1);

        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Casa.Draw(shader);

        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, trans));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        PuertaEntrada.Draw(shader);

        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        model = glm::translate(model, glm::vec3(0.09f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, trans2));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        PuertaInterna.Draw(shader);
        
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(8.0f, 2.1f, 8.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Tatami.Draw(shader);

        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(8.0f, 2.1f, -8.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Tatami.Draw(shader);

        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(18.0f, 0.0f, 3.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Jarron.Draw(shader);

        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::translate(model, glm::vec3(3.5f, 2.64f, 17.4f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Cajonera.Draw(shader);

        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(9.5f, 5.5f, 0.0f));
        model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Bambu.Draw(shader);

        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(3.0f, 2.0f, -9.0f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Lampara.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f, 2.2f, -17.6f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Armario.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f, 2.2f, -17.6f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(trans3, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        CajonArmario.Draw(shader);

        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f, 2.2f, -17.6f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, trans4));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        PuertasArmario.Draw(shader);

        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(9.5f, 1.3f, -0.4f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Fogata.Draw(shader);

        glBindVertexArray(0);

        //Animación del fuego
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4(1);
        glBindVertexArray(0);

        // Also draw the lamp object, again binding the appropriate shader
        lampshader.Use();
        Anim.Use();
        tiempo = glfwGetTime();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(Anim.Program, "model");
        viewLoc = glGetUniformLocation(Anim.Program, "view");
        projLoc = glGetUniformLocation(Anim.Program, "projection");
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(9.5f, 1.3f, -0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
        Fuego.Draw(lampshader);
        Fuego.Draw(Anim);
        glBindVertexArray(0);


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        lampshader.Use();
        //glm::mat4 model(1);
        /*glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);*/
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap the buffers
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }


    //Animacion puerta principal
    if (anim)
    {
        if (anim2)
        {
            if (trans < 0.0f)
            {
                trans += 0.0008f;
            }
            else
                anim2 = false;
        }
        else
        {
            if (trans > -1.8f)
            {
                trans -= 0.0008f;
            }
            else
                anim2 = true;
        }

    }

    //Animacion puerta interior
    if (anim3)
    {
        if (anim4)
        {
            if (trans2 < 0.0f)
            {
                trans2 += 0.0008f;
            }
            else
                anim4 = false;
        }
        else
        {
            if (trans2 > -2.45f)
            {
                trans2 -= 0.0008f;
            }
            else
                anim4 = true;
        }

    }

    //Animacion cajon armario
    if (anim5)
    {
        if (anim6)
        {
            if (trans3 < 1.8f)
            {
                trans3 += 0.0008f;
            }
            else
                anim6 = false;
        }
        else
        {
            if (trans3 > 0.0f)
            {
                trans3 -= 0.0008f;
            }
            else
                anim6 = true;
        }

    }

    //Animacion puertas armario
    if (anim7)
    {
        if (anim8)
        {
            if (trans4 < 0.0f)
            {
                trans4 += 0.0008f;
            }
            else
                anim8 = false;
        }
        else
        {
            if (trans4 > -4.0f)
            {
                trans4 -= 0.0008f;
            }
            else
                anim8 = true;
        }

    }

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_O])
    {
        anim = true;
        if (anim2)
        {
            anim2 = false;
        }
        else
            anim2 = true;

    }


}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}
