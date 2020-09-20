#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, playerMatrix, projectionMatrix, starMatrix, moonMatrix; //Object created here by making them matrix
float iRandomValue = 0;
float playerX = -10;
float player_rotate = 0;
float moon_scale = 2.0f;
float scalarVal = 1;
float pscalarVal = 3;
GLuint playerTextureID; //to load textures 
GLuint starTextureID;
GLuint moonTextureID;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}


void Initialize() {

    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Textured", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    playerMatrix = glm::mat4(1.0f);
    starMatrix = glm::mat4(1.0f);
    moonMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-10.0f, 10.0f, -7.5f, 7.5f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_BLEND);
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    playerTextureID = LoadTexture("ctg.png"); //get the file for the image
    starTextureID = LoadTexture("sts.png");
    moonTextureID = LoadTexture("moon.png");
    
}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            gameIsRunning = false;
        }
    }
}

float lastTicks = 0.0f;

void Update() { //update hour objects (tranlate, scale) and then render
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    iRandomValue = (rand() % 20) - 10;


    playerX += pscalarVal * deltaTime;
    if ((playerX >= 10) || (playerX <= -10)) {
        pscalarVal *= -1.0f;
    }

    player_rotate += 90.0f * deltaTime;
    moon_scale += scalarVal * deltaTime;
    if ((moon_scale >= 3.5) || (moon_scale <2 )){
        scalarVal *= -1.0f;
    }
    
    playerMatrix = glm::mat4(1.0f);
    starMatrix = glm::mat4(1.0f);
    moonMatrix = glm::mat4(1.0f);//creates the identity matrix but does nothing HOWEVER THIS CODE CENTERS OUR OBJECT EACH TIME ITS RUN
    //BY commenting out the line above, at every frame we wont re center rather it will just continue the translation from where it was.

    playerMatrix = glm::translate(playerMatrix, glm::vec3(playerX, -6.0f, 0.0f));
    playerMatrix = glm::scale(playerMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
 


    moonMatrix = glm::translate(moonMatrix, glm::vec3(-7.0f, 5.0f, 0.0f));
    moonMatrix = glm::rotate(moonMatrix, glm::radians(player_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    moonMatrix = glm::scale(moonMatrix, glm::vec3(moon_scale, moon_scale, 1.0f));


    starMatrix = glm::translate(starMatrix, glm::vec3(iRandomValue, 5.5f, 0.0f));
    starMatrix = glm::rotate(starMatrix, glm::radians(player_rotate * 10), glm::vec3(0.0f, 0.0f, 1.0f));
    starMatrix = glm::scale(starMatrix, glm::vec3(0.5f, 0.5f, 1.0f));


    //modelMatrix = glm::translate(modelMatrix, glm::vec3(0.03f, 0.0f, 0.0f)); // TRANSLATION EXAMPLE ; move to the right 1 or continusly move in that direction
    //modelMatrix = glm::rotate(modelMatrix, glm::radians(5.0f), glm::vec3(0.0f, 0.0f, 1.0f)); //ROTATION EXAMPLE; moves counterclockwise 
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.9f, 0.9f, 1.0f));//SCALING EXAMPLE; run code self-explanatory rmrbr it is being multiplied by itself over and over to shrink use big decimals less than 1

}

void Render() {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glClear(GL_COLOR_BUFFER_BIT); 
    glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program.positionAttribute);
    glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program.texCoordAttribute);

    program.SetModelMatrix(playerMatrix); //What actually sends out the drawing.
    glBindTexture(GL_TEXTURE_2D, playerTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    for (int i = 0; i < 3; i++) {
        
        program.SetModelMatrix(starMatrix);
        glBindTexture(GL_TEXTURE_2D, starTextureID);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    program.SetModelMatrix(moonMatrix);
    glBindTexture(GL_TEXTURE_2D, moonTextureID);
    glDrawArrays(GL_TRIANGLES, 0, 6);



    glDisableVertexAttribArray(program.positionAttribute);
    glDisableVertexAttribArray(program.texCoordAttribute);

    SDL_GL_SwapWindow(displayWindow);//hello
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}