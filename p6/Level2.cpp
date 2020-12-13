#include "Level2.h"
#define LEVEL2_WIDTH 28
#define LEVEL2_HEIGHT 8
#define LEVEL2_ENEMY_COUNT 4
#define LEVEL2_KEY_COUNT 3
unsigned int level2_data[] =

{
 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 0, 42, 42,
 42, 43, 43, 0, 0, 0, 0, 0, 0, 0, 0, 42, 0, 0, 43, 43, 0, 0, 0, 35, 0, 0, 0, 0, 42, 0, 42, 42,
 42, 43, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 35, 42, 0, 0, 42,
 42, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 35, 35, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 42, 35, 0, 42,
 42, 0, 0, 0, 0, 35, 0, 0, 0, 0, 35, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42,
 42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 0, 0, 0, 0, 0, 0, 42,
 42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 0, 35, 43, 42,
 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42

};


void Level2::Initialize() {
    state.nextScene = -1;
    GLuint mapTextureID = Util::LoadTexture("sheet1.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 8, 8);
    // Move over all of the player and enemy code from initialization.
    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(2, -6, 0);
    state.player->movement = glm::vec3(0);
    //state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 1.95f;
    state.player->textureID = Util::LoadTexture("redalt.png");

    state.player->animRight = new int[4]{ 8, 9, 10, 11 };
    state.player->animLeft = new int[4]{ 4, 5, 6, 7 };
    state.player->animUp = new int[4]{ 12, 13, 14, 15 };
    state.player->animDown = new int[4]{ 0, 1, 2, 3 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;
    state.player->height = 0.8f;
    state.player->width = 0.8f;
    state.player->inFinalScene = true;


    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("snake2.png");

    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(4.5, -1, 0);
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 0.2f;
    state.enemies[0].width = 0.7f;
    state.enemies[0].height = 0.7f;
    state.enemies[0].scale = 0.5f;

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(11.5, -2, 0);
    state.enemies[1].aiType = WAITANDGO;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].speed = 0.2f;
    state.enemies[1].width = 0.7f;
    state.enemies[1].height = 0.7f;
    state.enemies[1].scale = 0.5f;

    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(7.5, -3.5, 0);
    state.enemies[2].aiType = WAITANDGO;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].speed = 0.2f;
    state.enemies[2].width = 0.7f;
    state.enemies[2].height = 0.7f;
    state.enemies[2].scale = 0.5f;

    state.enemies[3].entityType = ENEMY;
    state.enemies[3].textureID = enemyTextureID;
    state.enemies[3].position = glm::vec3(19.5, -4, 0);
    state.enemies[3].aiType = WAITANDGO;
    state.enemies[3].aiState = IDLE;
    state.enemies[3].speed = 0.2f;
    state.enemies[3].width = 0.7f;
    state.enemies[3].height = 0.7f;
    state.enemies[3].scale = 0.5f;

    

    state.objects = new Entity[LEVEL2_KEY_COUNT];
    GLuint keyTextureID = Util::LoadTexture("key.png");
    state.objects[0].entityType = KEY;
    state.objects[0].textureID = keyTextureID;
    state.objects[0].position = glm::vec3(3, -1, 0);
    state.objects[0].scale = 0.5f;
    state.objects[0].width = 0.4f;
    state.objects[0].height = 0.4f;

    state.objects[1].entityType = KEY;
    state.objects[1].textureID = keyTextureID;
    state.objects[1].position = glm::vec3(13, -1, 0);
    state.objects[1].scale = 0.5f;
    state.objects[1].width = 0.4f;
    state.objects[1].height = 0.4f;


    state.objects[2].entityType = KEY;
    state.objects[2].textureID = keyTextureID;
    state.objects[2].position = glm::vec3(24, -6, 0);
    state.objects[2].scale = 0.5f;
    state.objects[2].width = 0.4f;
    state.objects[2].height = 0.4f;

    


}
void Level2::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.objects, LEVEL2_KEY_COUNT, state.map);
    if (state.player->dead == true) {
        state.player->position = glm::vec3(2, -6, 0);
    }
    if ((state.player->position.x >= 25) && (state.player->position.y >= -1) && (state.player->keysGrabbedinLev >= 3)) {
        state.player->reached = true;

    }
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, NULL, NULL, state.map);
    }
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, NULL, NULL, state.map);
    }

    for (int i = 0; i < LEVEL2_KEY_COUNT; i++) {
        state.objects[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, NULL, NULL, state.map);
    }
}
void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
    for (int i = 0; i < LEVEL2_KEY_COUNT; i++) {
        state.objects[i].Render(program);
    }
}