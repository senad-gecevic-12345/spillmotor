#ifndef ACTOR_H_
#define ACTOR_H_

#include "entt/entt.hpp"
#include "glm/glm.hpp"
#include "BulletDynamics/Character/btCharacterControllerInterface.h"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "btKinematicCharacterController.h"
#include "Utils.h"
#include "Math.h"
#include "Camera.h"
#include <memory>
#include "EditorRegistry.h"

namespace Component{
    struct Position;
    struct Rotation;
}

inline btVector3 to_btVector3(glm::vec3 vec){ return btVector3{btScalar(vec.x),btScalar(vec.y),btScalar(vec.z)}; }


namespace Controller{


    class Character{
    public:
        entt::entity e;
    private:
        BULLETCOPY::btKinematicCharacterController* controller;
        btPairCachingGhostObject* ghost_obj;
        btScalar walk_speed = 8.0;

    public:

        bool valid(){
            return (controller != nullptr && ghost_obj != nullptr);
        }

        void copy_over(entt::registry& registry);

        void update_poll_keyboard(btCollisionWorld* collisionWorld, float dt, glm::vec3 vec_forward, glm::vec3 vec_straife){
            if(controller == nullptr)
                return;
            btVector3 walk_dir(0,0,0);
            PollKeyboard::get().update();

            auto xform = ghost_obj->getWorldTransform();

            vec_forward = glm::normalize(vec_forward);
            vec_straife = glm::normalize(vec_straife);

            btVector3 forward_dir = btVector3(vec_forward.x, vec_forward.y, vec_forward.z);
            btVector3 straife_dir = btVector3(vec_straife.x, vec_straife.y, vec_straife.z);

            if(PollKeyboard::get().w){
                walk_dir += forward_dir;
            }
            if(PollKeyboard::get().a){
                walk_dir += straife_dir;
            }
            if(PollKeyboard::get().s){
                walk_dir -= forward_dir;
            }
            if(PollKeyboard::get().d){
                walk_dir -= straife_dir;
            }
            if(PollKeyboard::get().space){
                if(controller->canJump())
                    controller->jump(btVector3(0, 10, 0));
            }
            
            controller->setWalkDirection(walk_dir * btScalar(0.1));

            controller->updateAction(collisionWorld, btScalar(dt));
        }

        Character(entt::entity e, glm::vec3 _position, glm::vec3 _rotation); 

        Character& operator=(const Character& other)=delete;
        Character(const Character& other)=delete;

        Character& operator=(Character&& other)noexcept{
            if(this == &other)
                return *this;
            walk_speed = (std::move(other.walk_speed));
            e = (std::move(other.e));
            controller = (std::exchange(other.controller, nullptr));
            ghost_obj = std::exchange(other.ghost_obj, nullptr);
            return *this;
        }
        Character():ghost_obj(nullptr), controller(){};
        Character(Character&& other):
            e(std::move(other.e)),
            walk_speed(std::move(other.walk_speed)),
            controller(std::exchange(other.controller, nullptr)),
            ghost_obj(std::exchange(other.ghost_obj, nullptr))
        { 

        }

        ~Character();


    };

    class ControllerSingleton{
    public:
        NewCamera::Camera camera;
        std::unique_ptr<Controller::Character> character;
    public:
        void clear(){
            auto& registry = Registry::get().registry;
            if(character != nullptr)
                registry.destroy(character->e);

            character.reset();
        }

        ControllerSingleton(const ControllerSingleton&)=delete;
        ControllerSingleton& operator=(const ControllerSingleton&)=delete;
        ControllerSingleton():character(nullptr){};
        ~ControllerSingleton()=default;

        void update(entt::registry& registry, btCollisionWorld* world, float dt, float mouse_x, float mouse_y);
        
        static ControllerSingleton& get(){
            static ControllerSingleton instance;
            return instance;
        }
    };
}

#endif // ACTOR_H_
