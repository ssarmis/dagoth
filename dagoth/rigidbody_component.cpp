#include "rigidbody_component.hpp"

#include "game_object.hpp"
#include "physics.hpp"
#include <glm/vec2.hpp>
#include <functional>

void RigidBodyComponent::MakeTrigger() {
    type = TRIGGER;
}

void RigidBodyComponent::SetCollisionCallback(CallbackGameObjectFunction callback){
    onCollisionCallback = std::bind(callback, std::placeholders::_1);
}

void RigidBodyComponent::SetTriggerCallback(CallbackGameObjectFunction callback) {
    onTriggerCallback = std::move(callback);
}

void RigidBodyComponent::AddForce(glm::vec2 force) {
    transform->velocity += force;
    moving = true;
}

void RigidBodyComponent::Tick(r32 delta, void* data) {
    if (moving) {
        glm::vec2 force = transform->velocity * delta;
        switch (type) {
            case NORMAL: {
                if (CanPassThroughTiles(force) && CanPassThroughGameObjects(force)) {
                    scene->RemoveGameObjectFromChunk(object);
                    transform->position += force;
                    scene->MoveGameObjectToChunk(object);
                }
            }
            break;

            case TRIGGER: {
                // NOTE(Sarmis) no need to validate collisions
                // only notify
                CanPassThroughGameObjects(force);
                scene->RemoveGameObjectFromChunk(object);
                transform->position += force;
                scene->MoveGameObjectToChunk(object);
            }
            break;
        }
        
        moving = false;
    }

    transform->velocity = glm::vec2(0, 0);
}

bool RigidBodyComponent::CanPassThroughGameObjects(glm::vec2 force) {
    glm::vec2 futurePosition = transform->position + force;
    Transform futureTransform = {};
    futureTransform.position = futurePosition;
    futureTransform.size = transform->size;

    GameObjectArray* nearbyGameObjects = scene->GetGameObjectsFromChunk(object);
    for (auto& gameObject : *nearbyGameObjects) {
        auto otherRigidBody = gameObject->componentManager.GetComponent<RigidBodyComponent>();
        if (otherRigidBody->object != object) { // don't check myself...
            switch (otherRigidBody->type) {
                case NORMAL: {
                    if (AABB::Intersects(futureTransform, *otherRigidBody->transform)) {
                        otherRigidBody->onCollisionCallback(object); // we also notify them (the game object)
                        onCollisionCallback(gameObject);
                        if (type == TRIGGER) {
                            onTriggerCallback(gameObject); 
                            otherRigidBody->onTriggerCallback(object);
                        }
                        return false;
                    }
                }
                break;

                case TRIGGER: {
                    if (AABB::Intersects(futureTransform, *otherRigidBody->transform)) {
                        onTriggerCallback(gameObject);
                        otherRigidBody->onTriggerCallback(object);
                    }
                }
                break;
            }
        }
    }
    return true;
}

bool RigidBodyComponent::CanPassThroughTiles(glm::vec2 force) {
    glm::vec2 futurePosition = transform->position + force;
    glm::vec2 nextTile = futurePosition / glm::vec2(TILE_DISPLAY_SIZE);
    u32 nx = nextTile.x;
    u32 ny = nextTile.y;
    return !(scene->container.tiles[nx + ny * scene->container.width].properties.solid);
}