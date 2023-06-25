#pragma once

#include "tile_container.hpp"
#include "camera.hpp"

#include <vector>
#include <memory>
#include <string>

class GameObject;
class RenderingObject;

using GameObjectArray = std::vector<GameObject*>;

constexpr u32 GAME_OBJECT_CHUNK_SIZE = 16;
constexpr u32 GAME_OBJECT_CHUNK_X_TILE = TILE_DISPLAY_SIZE * GAME_OBJECT_CHUNK_SIZE;
constexpr u32 GAME_OBJECT_CHUNK_Y_TILE = TILE_DISPLAY_SIZE * GAME_OBJECT_CHUNK_SIZE;

struct Layer {
    TileContainer container;
};

class Scene {
private:
    std::string name;
    u32 gameObjectChunkTotalWidth;
public:
    Scene(const char* name);
    TileContainer container;
    std::vector<Layer*> layers;
    Camera camera;
    // not GameObjectArray, because that one isn't shared ptr
    // didn't want to waste even more time transforming them into shared ptr, potential fix, 
    // make them shared_ptr while constructing game objects so they can pass that...
    std::vector<std::shared_ptr<GameObject>> gameObjects; 
    std::vector<GameObject*> gameObjectsToBeRemoved;
    std::vector<GameObjectArray> gameObjectsInTiles;

    void AddLayer(Layer* layer);

    GameObjectArray* GetGameObjectsFromChunk(GameObject* object);
    void RemoveGameObjectFromChunk(GameObject* object);
    void MoveGameObjectToChunk(GameObject* object);
    void AllocateGameObjectChunks();

    void TickGameObjects(r32 delta);
    void RenderGameObjects(RenderingObject* renderObject);

    void RemoveGameObject(GameObject* object);

    template<typename Type, typename ...Args>
	auto AddGameObject(Args... arguments) {
		auto object = std::make_shared<Type>(std::forward<Args>(arguments)...);
        gameObjects.push_back(object);
        MoveGameObjectToChunk(object.get()); // put the entity into its chunk so collision works
		return object;
	}
};
