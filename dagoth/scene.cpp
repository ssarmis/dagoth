#include "scene.hpp"
#include "opengl_layer.h"
#include "game_object.hpp"
#include <glm/vec2.hpp>

Scene::Scene(const char* name):
name(name){
}

void Scene::AddLayer(Layer* layer){
    layers.push_back(layer);
}

GameObjectArray* Scene::GetGameObjectsFromChunk(GameObject* object) {
    glm::vec2 goChunkSize = glm::vec2(GAME_OBJECT_CHUNK_X_TILE, GAME_OBJECT_CHUNK_Y_TILE);
    glm::vec2 chunk = object->transform.position / goChunkSize;
    u32 xx = chunk.x;
    u32 yy = chunk.y;

    return &gameObjectsInTiles[xx + yy * gameObjectChunkTotalWidth];
}

void Scene::RemoveGameObjectFromChunk(GameObject* object) {
    glm::vec2 goChunkSize = glm::vec2(GAME_OBJECT_CHUNK_X_TILE, GAME_OBJECT_CHUNK_Y_TILE);
    glm::vec2 chunk = object->transform.position / goChunkSize;
    u32 xx = chunk.x;
    u32 yy = chunk.y;

    // man this will be slow AF
    auto vector = &gameObjectsInTiles[xx + yy * gameObjectChunkTotalWidth];
    auto it = std::find(vector->begin(), vector->end(), object);
    if (it != vector->end()) {
        vector->erase(it);
    }
}

void Scene::MoveGameObjectToChunk(GameObject* object) {
    glm::vec2 goChunkSize = glm::vec2(GAME_OBJECT_CHUNK_X_TILE, GAME_OBJECT_CHUNK_Y_TILE);
    glm::vec2 chunk = object->transform.position / goChunkSize;
    u32 xx = chunk.x;
    u32 yy = chunk.y;

    // man this will be slow AF
    gameObjectsInTiles[xx + yy * gameObjectChunkTotalWidth].push_back(object);
}

void Scene::AllocateGameObjectChunks() {
    if (!container.tilesAmount) {
        return;
    }
    gameObjectChunkTotalWidth = container.width / GAME_OBJECT_CHUNK_SIZE;
    u32 gameObjectInTilesSize = container.tilesAmount / (GAME_OBJECT_CHUNK_SIZE * GAME_OBJECT_CHUNK_SIZE); // should be rounded up
    gameObjectsInTiles.reserve(gameObjectInTilesSize);
    for (int i = 0; i < gameObjectInTilesSize; ++i) {
        gameObjectsInTiles.push_back(GameObjectArray());
    }
}

void Scene::RemoveGameObject(GameObject* object){
    object->toBeRemoved = true;
    gameObjectsToBeRemoved.push_back(object);
    RemoveGameObjectFromChunk(object);
}

void Scene::TickGameObjects(r32 delta){
    // NOTE(Sarmis) using a classic for loop because adding/removing gameObjects from
    // the vector while iterating will make the iterators use the wrong bounds
    for (int i = 0; i < gameObjects.size(); ++i) {
        gameObjects[i]->currentIndex = i;
        gameObjects[i]->Tick(delta);
    }

    if (gameObjectsToBeRemoved.size()) {
        for (int i = gameObjectsToBeRemoved.size() - 1; i >= 0; --i) {
            gameObjects.erase(gameObjects.begin() + gameObjectsToBeRemoved[i]->currentIndex);
        }
        gameObjectsToBeRemoved.clear();
    }
}

void Scene::RenderGameObjects(RenderingObject* renderObject){
    for(auto& gameObject : gameObjects){
        gameObject->Render(renderObject);
    }
}