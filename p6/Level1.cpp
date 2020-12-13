#include "Level1.h"
#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8
#define LEVEL1_ENEMY_COUNT 3
#define LEVEL1_KEY_COUNT 1
unsigned int level1_data[] =

{
 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42,
 42, 43, 0, 0, 0, 0, 0, 0, 43, 43, 42, 0, 42, 42,
 42, 43, 0, 35, 0, 0, 0, 0, 0, 0, 42, 0, 0, 42,
 42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 35, 35, 0, 42,
 42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42,
 42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42,
 42, 0, 0, 0, 0, 43, 35, 0, 35, 0, 0, 43, 43, 42,
42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42, 42
};


void Level1::Initialize() {
    state.nextScene = -1;
	GLuint mapTextureID = Util::LoadTexture("sheet1.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 8, 8);
	// Move over all of the player and enemy code from initialization.
    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1.25f, -5, 0);
    state.player->movement = glm::vec3(0);
    //state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 1.5f;
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
    state.player->jumpPower = 4.8f;



    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    GLuint enemyTextureID = Util::LoadTexture("snake2.png");
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = enemyTextureID;
    state.enemies[0].position = glm::vec3(6, -1.75, 0);
    //state.enemies[0].movement = glm::vec3(-1, 0, 0);
    //state.enemies[0].acceleration = glm::vec3(0, -1.81f, 0);
    state.enemies[0].aiType = WAITANDGO;
    state.enemies[0].aiState = IDLE;
    state.enemies[0].speed = 0.2f;
    state.enemies[0].width = 0.7f;
    state.enemies[0].height = 0.7f;
    state.enemies[0].scale = 0.5f;

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = enemyTextureID;
    state.enemies[1].position = glm::vec3(5, -4, 0);
    //state.enemie1[0].movement = glm::vec3(-1, 0, 0);
    //state.enemie1[0].acceleration = glm::vec3(0, -1.81f, 0);
    state.enemies[1].aiType = WAITANDGO;
    state.enemies[1].aiState = IDLE;
    state.enemies[1].speed = 0.2f;
    state.enemies[1].width = 0.7f;
    state.enemies[1].height = 0.7f;
    state.enemies[1].scale = 0.5f;


    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = enemyTextureID;
    state.enemies[2].position = glm::vec3(11, -3, 0);
    //state.enemie2[0].movement = glm::vec3(-1, 0, 0);
    //state.enemie2[0].acceleration = glm::vec3(0, -1.81f, 0);
    state.enemies[2].aiType = WAITANDGO;
    state.enemies[2].aiState = IDLE;
    state.enemies[2].speed = 0.2f;
    state.enemies[2].width = 0.7f;
    state.enemies[2].height = 0.7f;
    state.enemies[2].scale = 0.5f;

    state.objects = new Entity[LEVEL1_KEY_COUNT];
    GLuint keyTextureID = Util::LoadTexture("key.png");
    state.objects[0].entityType = KEY;
    state.objects[0].textureID = keyTextureID;
    state.objects[0].position = glm::vec3(7, -6, 0);
    state.objects[0].scale = 0.5f;
    state.objects[0].width = 0.4f;
    state.objects[0].height = 0.4f;


    
    
}
void Level1::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.objects, LEVEL1_KEY_COUNT, state.map);
	
    if (state.player->dead == true) {
        state.player->position = glm::vec3(1.25f, -5, 0);
    }
    if ((state.player->position.x >= 11) && (state.player->position.y >= -1) && (state.player->keysGrabbedinLev >= 1)){
        state.nextScene = 2;
    }
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, NULL,NULL, state.map);
    }

    for (int i = 0; i < LEVEL1_KEY_COUNT; i++) {
        state.objects[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, NULL, NULL, state.map);
    }
}
void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);

    
	state.player->Render(program);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }
    for (int i = 0; i < LEVEL1_KEY_COUNT; i++) {
        state.objects[i].Render(program);
    }
}