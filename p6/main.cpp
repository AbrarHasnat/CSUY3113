#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#include <SDL_mixer.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <string>


#include "Util.h"
#include "Effects.h"
#include "Entity.h"
#include "map.h"
#include "Scene.h"
#include "Level1.h"
#include "Level2.h"
#include "Level0.h"




SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

Scene* currentScene;
Scene* sceneList[3];
Effects* effects;
int lives;
int keys;
int fluteUses;
Mix_Music* music;
Mix_Chunk* bounce;
Mix_Chunk* flute;



void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}




void Initialize() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    displayWindow = SDL_CreateWindow("SNAKE MINE!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_lit.glsl", "shaders/fragment_lit.glsl");
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("cave.mp3");
    
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    bounce = Mix_LoadWAV("bounce.wav");
    flute = Mix_LoadWAV("flute.wav");


    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sceneList[0] = new Level0();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    SwitchToScene(sceneList[0]);
    effects = new Effects(projectionMatrix, viewMatrix);
    effects->Start(FADEIN, 0.5f);
    lives = 3;
    keys = 0;
    fluteUses = 4;



}

void ProcessInput() {

    currentScene->state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) { //one time hit
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
                break;

            case SDLK_SPACE:
                if (currentScene->state.player->collidedBottom) { // makes sure were grounded before the jump
                    Mix_PlayChannel(-1, bounce, 0);
                    currentScene->state.player->jump = true;
                }
                // Some sort of action
                break;

            case SDLK_z:
                if (fluteUses > 0) {
                    Mix_PlayChannel(-1, flute, 0);
                    fluteUses -= 1;
                    for (int i = 0; i < sizeof(currentScene->state.enemies); i++) {
                        if (currentScene->state.enemies[i].closeEnough) {
                            currentScene->state.enemies[i].isActive = false;
                        }
                    }
                }

            
            }

            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);
   
    
    if (keys[SDL_SCANCODE_LEFT]) { // hold down
        currentScene->state.player->movement.x = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animLeft;
    }
    else if (keys[SDL_SCANCODE_RIGHT]) {
        currentScene->state.player->movement.x = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animRight;
    }
    if (keys[SDL_SCANCODE_UP]) { // hold down
        
        currentScene->state.player->movement.y = 1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animUp;
    }
    else if (keys[SDL_SCANCODE_DOWN]) {
        
        currentScene->state.player->movement.y = -1.0f;
        currentScene->state.player->animIndices = currentScene->state.player->animDown;
    }


    if (glm::length(currentScene->state.player->movement) > 1.0f) {
        currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
    }

}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;
bool lastCollidedBottom = false;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP) {
        // Update. Notice it's FIXED_TIMESTEP. Not deltaTime
        currentScene->Update(FIXED_TIMESTEP);
        if (currentScene->state.player->dead) {
            lives -= 1;
        }
        if (currentScene->state.player->grabbed) {
            keys += 1;
        }
        if (lives <= 0) {
            currentScene->state.player->gameOver = true;
        }

        if (!currentScene->state.player->reached) {
            program.SetLightPosition(currentScene->state.player->position);
        }
        else {
            
            program.SetLightPosition(glm::vec3(21.5f, -1, 0));
        }
        
        //if (lastCollidedBottom == false && currentScene->state.player->collidedBottom) {
          //  effects->Start(SHAKE, 2.0f);
        //}

        lastCollidedBottom = currentScene->state.player->collidedBottom;
        effects->Update(FIXED_TIMESTEP);


        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;
    viewMatrix = glm::mat4(1.0f);
    if (!currentScene->state.player->inFinalScene) {
        if ((currentScene->state.player->position.x >= 5) && (currentScene->state.player->position.x <= 8)) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
        }
        else if (currentScene->state.player->position.x < 5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        }
        else if (currentScene->state.player->position.x > 8) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-8, 3.75, 0));
        }
    }
    else {
        if ((currentScene->state.player->position.x >= 5) && (currentScene->state.player->position.x <= 22)) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
        }
        else if (currentScene->state.player->position.x < 5) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
        }
        else if (currentScene->state.player->position.x > 22) {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-22, 3.75, 0));
        }
        viewMatrix = glm::translate(viewMatrix, effects->viewOffset);
    }
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT);
    GLuint fontTextureID = Util::LoadTexture("font1.png");
    program.SetViewMatrix(viewMatrix);
    glUseProgram(program.programID);
    

    currentScene->Render(&program);
    effects->Render();

    //DrawText(&program, fontTextureID, "Hello", 1, -0.5f, glm::vec3(-4.25f, 3, 0));
    if (!currentScene->state.player->inMenuScene) {
        Util::DrawText(&program, fontTextureID, "Lives: " + std::to_string(lives), 1, -0.5f, glm::vec3(1, -1, 0));
        Util::DrawText(&program, fontTextureID, "Keys: " + std::to_string(keys) + "/4", 1, -0.5f, glm::vec3(1, -2, 0));
    }

    if (currentScene->state.player->gameOver) {
        Util::DrawText(&program, fontTextureID, "YOU LOSE", 1, -0.5f, glm::vec3(currentScene->state.player->position.x, -3, 0));
    }
    if (currentScene->state.player->reached) {
        Util::DrawText(&program, fontTextureID, "YOU WIN", 1, -0.5f, glm::vec3(20, -1, 0));
    }
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
        if (currentScene->state.nextScene >= 0) SwitchToScene(sceneList[currentScene->state.nextScene]);
        Render();
        
    }
    

    Shutdown();
    return 0;
}