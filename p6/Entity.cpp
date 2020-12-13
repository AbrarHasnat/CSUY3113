#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    velocity = glm::vec3(0);
    acceleration = glm::vec3(0);
    speed = 0;

    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity* other) {
    if (other == this) return false;
    if (isActive == false || other->isActive == false) return false;
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xdist < 0 && ydist < 0) return true;
    return false;
}
void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{

    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                if (object->entityType == ENEMY) {
                    dead = true;
                    
                }
                if (object->entityType == KEY) {
                    grabbed = true;
                    object->isActive = false;
                    keysGrabbedinLev += 1;
                }
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                if (object->entityType == ENEMY) {
                    object->isActive = false;
                }
                if (object->entityType == KEY) {
                    grabbed = true;
                    object->isActive = false;
                    keysGrabbedinLev += 1;
                }
            }
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
  
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                
                if (object->entityType == KEY) {
                    collidedRight = true;
                    grabbed = true;
                    object->isActive = false;
                    keysGrabbedinLev += 1;
                }
                if (object->entityType == ENEMY) {
                    collidedRight = true;
                    dead = true;
                    
                }
                
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                
                if (object->entityType == ENEMY) {
                    collidedLeft = true;
                    dead = true;
                    
                }
                if (object->entityType == KEY) {
                    collidedLeft = true;
                    grabbed = true;
                    object->isActive = false;
                    keysGrabbedinLev += 1;
                }
            }
        }
    }
}

void Entity::CheckCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
}
void Entity::CheckCollisionsX(Map* map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeft = true;
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
}

void Entity::AIWalker() {

    movement = glm::vec3(-1, 0, 0);

}

void Entity::AIFlyandGo(Entity *player) {
    

    switch (aiState) {
    case IDLE:
        if ((position.x <= 1.97) || (position.x >= 2.03)) {
            movement *= -1.0f;
        }
        if (glm::distance(position.x, player->position.x) > 1.0f) {
            aiState = WALKING;
        }

        break;

    case WALKING:
        
        if (player->position.x < position.x) {
         
            movement = glm::vec3(-1, 0, 0);
       
        }
        else {
            
            movement = glm::vec3(1, 0, 0);
        
        }
        if (player->position.y < position.y) {

            movement = glm::vec3(0, -1, 0);

        }
        else {

            movement = glm::vec3(0, 1, 0);

        }
       
        break;

    case ATTACKING:
        break;
    }

}

void Entity::AIWaitAndGo(Entity* player) {

    switch (aiState) {
    case IDLE:

        if ((glm::distance(position.x, player->position.x) < 3.0f) && (glm::distance(position.y, player->position.y) < 3.0f)) {
            aiState = WALKING;
        }

        break;

    case WALKING:
        if ((glm::distance(position.x, player->position.x) < 2.0f) && (glm::distance(position.y, player->position.y) < 2.0f)) {
            closeEnough = true;
        }
        else {
            closeEnough = false;
        }
        if (player->position.x < position.x) {
            if (player->position.y < position.y) {
                movement = glm::vec3(-1, -1, 0);
            }
            else {
                movement = glm::vec3(-1, 1, 0);
            }

        }
        else {
            if (player->position.y < position.y) {
                movement = glm::vec3(1, -1, 0);
            }
            else {
                movement = glm::vec3(1, 1, 0);
            }

        }

  
        

        break;
    case ATTACKING:
        break;
    }

}



void Entity::AI(Entity* player) {
    switch (aiType) {
    case WALKER:
        AIWalker();
        break;

    case WAITANDGO:
        AIWaitAndGo(player);
        break;

    case FLYANDGO:
        AIFlyandGo(player);
    }
    


}

void Entity::Update(float deltaTime, Entity* player, Entity *enemies, int enemyCount , Entity* objects, int objectCount, Map* map)
{
    if (isActive == false) return;

    if (dead == true) {
        dead = false;


    }
    if (grabbed == true) {
        grabbed = false;


    }

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;

    if (entityType == ENEMY) {
        AI(player);
    }

    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        }
        else {
            animIndex = 0;
        }
    }

    if ((!gameOver) && (!reached)) {
        if (jump) {
            jump = false;
            velocity.y += jumpPower;
        }
        velocity.x = movement.x * speed;
        velocity.y = movement.y * speed;
        velocity += acceleration * deltaTime;
        position.y += velocity.y * deltaTime; // Move on Y
        CheckCollisionsY(map);
        CheckCollisionsY(objects, objectCount);
        CheckCollisionsY(enemies, enemyCount); // Fix if needed
        

        position.x += velocity.x * deltaTime; // Move on X
        CheckCollisionsX(map);
        CheckCollisionsY(objects, objectCount);
        CheckCollisionsX(enemies, enemyCount); // Fix if needed
        
    }
    

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale,scale,1.0f));
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) {
    if (isActive == false) return;
    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}