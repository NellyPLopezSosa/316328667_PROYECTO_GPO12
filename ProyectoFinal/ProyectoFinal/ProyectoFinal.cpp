/*
=================================
        PROYECTO FINAL
        316328667
        GRUPO:12
=================================
*/

/*
>> Blibliotecas
>> Libraries
*/

// Std. Includes
#include <string>
#include <iostream>
#include <cmath>

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


// Constantes de la ventana
// Properties
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Funciones de movimientos
// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Propiedades de la cámara
// Camera properties
Camera camera(glm::vec3(30.0f, 10.0f, 15.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

/*
>> Variables globales
>> Global variables
*/

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;

/*
>> Variables para animación
>> Animation variables
*/

//Variables animación puerta principal
//Main door
float trans = 0.0f;
bool anim = true;
bool anim2 = true;

//Variables animación puerta interior
//Room's door
float trans2 = 0.0f;
bool anim3 = true;
bool anim4 = true;

//Variables animación cajon armario
//Drawer
float trans3 = 0.0f;
bool anim5 = true;
bool anim6 = true;

//Variables animación puertas armario
//Closet door
float trans4 = 0.0f;
bool anim7 = true;
bool anim8 = true;

float tiempo;

int main()
{

/*
>> Configuración inicial de la ventana
>> Initial configuration of window 
*/
    // Init GLFW
    glfwInit();
    // Verificación de la compatibilidad, se trabaja con la versión 3.3
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Creación de la ventana con los valores definidos
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);


    // Verificación de errores de creación de ventana
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

    // Verificación de errores de inicialización de glew
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define las dimensiones del viwport
    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

/*
>> Configuración y compilación de shaders
>> Setup and compile of shaders
*/
    Shader shader("Shaders/modelLoading.vs", "Shaders/modelLoading.frag");
    Shader lampshader("Shaders/lamp.vs", "Shaders/lamp.frag");
    Shader Anim("Shaders/anim.vs", "Shaders/anim.frag");
    Shader Anim2("Shaders/anim2.vs", "Shaders/anim2.frag");
    Shader Anim3("Shaders/anim3.vs", "Shaders/anim3.frag");
    Shader lampshader2("Shaders/lamp2.vs", "Shaders/lamp2.frag");
    Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");

    // Load models
    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    //Definición de vértices
    GLfloat vertices[] =
    {
        // positions          // colors           // texture coords
     0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
     0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
    -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
    };

    //Definición de índices
    GLuint indices[] =
    {  // Note that we start from 0!
        0,1,3,
        1,2,3
    };

/*
>> Configuración de VBO, VAO y EBO
>> Setup of VBO, VAO and EBO

VBO: Contiene la información acerca de los vértices
VAO: Objeto que contiene uno o más VBO
*/
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

/*
>> Carga de los modelos
>> Load models

Define el nombre de los modelos, y se localiza los objetos con extención .obj dentro de su carpeta correspondiente.
*/

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
    Model Olla((char*)"Models/Olla/Olla.obj");
    Model Caldo((char*)"Models/Olla/Caldo.obj");
    Model Ingredientes((char*)"Models/Olla/Ingredientes.obj");

    //Objetos segunda habitación
    Model Escritorio((char*)"Models/Escritorio/Escritorio.obj");
    Model Futon((char*)"Models/Futon/Futon.obj");
    Model Katana((char*)"Models/Katana/KatanaCompleta.obj");
    Model Cuadernos((char*)"Models/Cuadernos/Cuadernos.obj");
    Model Cuaderno((char*)"Models/Cuadernos/Cuaderno.obj");
    Model LamparaHabitacion((char*)"Models/LamparaHabitacion/Lampara.obj");


/*
>> Carga de texturas
>> Load textures
*/
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

        // Color de la ventana (recibe colores normalizados)
        // Clear the colorbuffer
        glClearColor(0.172f, 0.235f, 0.164f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


/*
>> Se dibujan los modelos cargados
>> Draw the loaded model

Estos objetos emplean a shader
*/
        // Seteo de la matriz
        glm::mat4 model(1);

        // Se dibuja el objeto Casa
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Casa.Draw(shader);

        // Se dibuja el objeto PuertaEntrada
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        model = glm::translate(model, glm::vec3(0.15f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, trans));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        PuertaEntrada.Draw(shader);

        // Se dibuja el objeto PuertaInterna
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
        model = glm::translate(model, glm::vec3(0.09f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, trans2));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        PuertaInterna.Draw(shader);
        
        // Se dibuja el objeto Tatami
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(8.0f, 2.1f, 8.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Tatami.Draw(shader);

        // Se dibuja el objeto Tatami
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(8.0f, 2.1f, -8.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Tatami.Draw(shader);

        // Se dibuja el objeto Jarron
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(18.0f, 0.0f, 3.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Jarron.Draw(shader);

        // Se dibuja el objeto Cajonera
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::translate(model, glm::vec3(3.5f, 2.64f, 17.4f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Cajonera.Draw(shader);

        // Se dibuja el objeto Bambu
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(9.5f, 6.8f, 0.0f));
        model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Bambu.Draw(shader);

        // Se dibuja el objeto Olla
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(9.44f, 3.54f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.7f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Olla.Draw(shader);

        // Se dibuja el objeto Lampara
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(3.0f, 2.0f, -9.0f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Lampara.Draw(shader);

        // Se dibuja el objeto Armario
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f, 2.2f, -17.6f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Armario.Draw(shader);

        // Se dibuja el objeto CajonArmario
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f, 2.2f, -17.6f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(trans3, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        CajonArmario.Draw(shader);

        // Se dibuja el objeto PuertasArmario
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f, 2.2f, -17.6f));
        model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, trans4));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        PuertasArmario.Draw(shader);

        // Se dibuja el objeto Fogata
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(9.5f, 1.3f, -0.4f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Fogata.Draw(shader);

        //Objetos segunda habitación
        // Se dibuja el objeto Escritorio
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::translate(model, glm::vec3(-13.0f, 2.0f, -6.0f));
        model = glm::scale(model, glm::vec3(1.5f, 1.5f, 1.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Escritorio.Draw(shader);

        // Se dibuja el objeto Futon
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-7.0f, 2.0f, 5.0f));
        model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Futon.Draw(shader);

        // Se dibuja el objeto Katana
        model = glm::mat4(1);        
        model = glm::translate(model, glm::vec3(-9.0f, 2.5f, 5.0f));
        model = glm::rotate(model, glm::radians(315.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(3.7f, 3.7f, 3.7f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Katana.Draw(shader);

        // Se dibuja el objeto LamparaHabitacion
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::translate(model, glm::vec3(-13.0f, 2.0f, -2.0f));
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        LamparaHabitacion.Draw(shader);

        // Se dibuja el objeto Cuadernos
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::translate(model, glm::vec3(-13.0f, 5.1f, -7.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Cuadernos.Draw(shader);

        // Se dibuja el objeto Cuaderno
        model = glm::mat4(1);
        model = glm::rotate(model, glm::radians(-rot), glm::vec3(0.0f, 0.5f, 0.0f));
        model = glm::translate(model, glm::vec3(-13.0f, 5.1f, -5.5f));
        model = glm::scale(model, glm::vec3(0.4f, 0.5f, 0.5f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        Cuaderno.Draw(shader);

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


/*
>> Generación de animaciones complejas con uso de los shaders "lightingShader" y "Anim" correspondientes a cada objeto
>> Generation of complex animations using the shaders "lightingShader" and "Anim" corresponding to each object
*/

        // Configuración del lightingShader
        
        // Obtener las uniform locations
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pasar las matrices al shader
        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        model = glm::mat4(1);
        glBindVertexArray(0);

        // Animación del fuego
        // Contiene sus funciones en el shader Anim

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

        // Animación del caldo
        // Contiene sus funciones en el shader Anim2
        
        // Also draw the lamp object, again binding the appropriate shader
        lampshader.Use();
        Anim2.Use();
        tiempo = glfwGetTime();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(Anim2.Program, "model");
        viewLoc = glGetUniformLocation(Anim2.Program, "view");
        projLoc = glGetUniformLocation(Anim2.Program, "projection");
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(9.44f, 3.54f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.7f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(Anim2.Program, "time"), tiempo);
        Caldo.Draw(lampshader);
        Caldo.Draw(Anim2);
        glBindVertexArray(0);


        //Animación de ingredientes
        // Contiene sus funciones en el shader Anim3
        
        // Also draw the lamp object, again binding the appropriate shader
        lampshader.Use();
        Anim3.Use();
        tiempo = glfwGetTime();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(Anim3.Program, "model");
        viewLoc = glGetUniformLocation(Anim3.Program, "view");
        projLoc = glGetUniformLocation(Anim3.Program, "projection");
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(9.44f, 3.54f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.7f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(Anim3.Program, "time"), tiempo);
        Ingredientes.Draw(lampshader);
        Ingredientes.Draw(Anim3);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        lampshader.Use();

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
    // Controles de la camara
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

/*
>> Implementación de las animaciones sencillas
>> Implementation of simple animations
*/

    // Animacion puerta principal
    // Consiste en abrir y cerrar la puerta, la cual se desliza de un lado a otro
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

    // Animacion puerta interior
    // Consiste en abrir y cerrar la puerta, la cual se desliza de un lado a otro
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
    // Consiste en abrir y cerrar el cajon, la cual se desliza de un adentro hacia afuera
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

    // Animacion puertas armario
    // Consiste en abrir y cerrar las puertas, las cuales se deslizan de un lado a otro
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
