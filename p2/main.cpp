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
#include "Entity.h"



struct GameState {
    Entity* player;
    Entity* player2;
    Entity* ball;
    
};

GameState state;


SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;







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
    displayWindow = SDL_CreateWindow("PONG!!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-10.0f, 10.0f, -7.5f, 7.5f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    //program.SetColor(1.0f, 0.0f, 0.0f, 1.0f);

    glUseProgram(program.programID);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_BLEND);
    // Good setting for transparency
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    
    state.player = new Entity();
    state.player->position = glm::vec3(-9.5f, 0.0f, 0.0f);
    state.player->movement = glm::vec3(0);
    state.player->speed = 3.0f;
    state.player->scale = 3.0f; //can now control the scale
    state.player->textureID = LoadTexture("box.png");
   
    state.player2= new Entity();
    state.player2->position = glm::vec3(9.5f, 0.0f, 0.0f);
    state.player2->movement = glm::vec3(0);
    state.player2->speed = 3.0f;
    state.player2->scale = 3.0f;
    state.player2->textureID = LoadTexture("box.png");
    
    state.ball = new Entity();
    state.ball->position = glm::vec3(0.0f, 0.0f, 0.0f);
    state.ball->movement.y = -1; //must initilize here or else it redraws every frame
    state.ball->movement.x = 1;
    state.ball->speed = 2.0f;
    state.ball->textureID = LoadTexture("box.png");



}

void ProcessInput() {

    state.player->movement = glm::vec3(0);
    state.player2->movement = glm::vec3(0);
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                // Move the player left
                break;

            case SDLK_RIGHT:
                // Move the player right
                //player_movement.x = 1.0f;
                break;

            case SDLK_SPACE:
                // Some sort of action
                break;
            }
            break; // SDL_KEYDOWN
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (keys[SDL_SCANCODE_UP]) {
        if (state.player2->position.y >= 6) {
            state.player2->movement.y = 0.0f;
        }
        else {
            state.player2->movement.y = 1.0f;

        }
        
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        if (state.player2->position.y <= -6) {
            state.player2->movement.y = 0.0f;
        }
        else {
            state.player2->movement.y = -1.0f;

        }
        
    }

    if (glm::length(state.player->movement) > 1.0f) {
        state.player2->movement = glm::normalize(state.player->movement);
        
    }


    if (keys[SDL_SCANCODE_W]) {
        if (state.player->position.y >= 6) {
            state.player->movement.y = 0.0f;
        }
        else {
            state.player->movement.y = 1.0f;

        }

    }
    else if (keys[SDL_SCANCODE_S]) {
        if (state.player->position.y <= -6) {
            state.player->movement.y = 0.0f;
        }
        else {
            state.player->movement.y = -1.0f;

        }

    }
    if (glm::length(state.player->movement) > 1.0f) {
        state.player->movement = glm::normalize(state.player->movement);

    }

}

float lastTicks = 0.0f;

void Update() { //handles multiple objects in a state
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    

    state.player->Update(deltaTime);
    state.player2->Update(deltaTime);

    if ((state.ball->position.x <= -10) || (state.ball->position.x >= 10)) {
        gameIsRunning = false;
        
    }


    //Colliding code 
    float xdist1 = fabs(state.player2->position.x - state.ball->position.x) - ((1+1) / 2.0f); 
    float ydist1 = fabs(state.player2->position.y - state.ball->position.y) - ((1 + 3) / 2.0f);

    float xdist2 = fabs(state.player->position.x - state.ball->position.x) - ((1 + 1) / 2.0f);
    float ydist2 = fabs(state.player->position.y - state.ball->position.y) - ((1 + 3) / 2.0f);

    if ((xdist1 < 0 && ydist1 < 0) || (xdist2 < 0 && ydist2 < 0)) {
        state.ball->movement.x *= -1;
        state.ball->UpdateBall(deltaTime);
    }
    else {
        state.ball->UpdateBall(deltaTime);
    }
   

    
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    
    state.player->Render(&program);
    state.player2->Render(&program);
    state.ball->Render(&program);

    SDL_GL_SwapWindow(displayWindow);
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
