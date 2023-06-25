#include "camera.hpp"

#include "game_object.hpp"
#include "opengl_layer.h"
#include "global_settings.hpp"

void Camera::FollowGameObject(std::shared_ptr<GameObject> target){
    this->target = target;
}

glm::vec3 Lerp(glm::vec3 x, glm::vec3 y, float t) {
  return x * (1.f - t) + y * t;
}

void Camera::ApplyCameraModifiers(RenderingProgram* program, r32 delta){
    if(target){
      glm::vec3 targetToVec3 = glm::vec3(target->transform.position, 0);
      glm::vec3 screenOffset = glm::vec3(Settings::width / 3, Settings::height / 3, 0);
      glm::vec3 otherOffset = glm::vec3(target->transform.size.x / 2, target->transform.size.y / 2, 0);

      glm::vec3 finalOffset = -targetToVec3 + screenOffset - otherOffset;
      
      currentOffset = Lerp(currentOffset, finalOffset, delta);
      //currentOffset = finalOffset;
      openglCameraTranslate(*program, currentOffset);
      //openglCameraRotate(*program, .001, glm::vec3(0, 0, 1));
    }
}