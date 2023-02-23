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

    // TODO: make this virtual and store it as a base for the representations
    // then can easily implement gameplay code
    // virtual destructor
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


        // this down own the camera so not that good
        virtual void update(const NewCamera::Camera& camera){}

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

        virtual ~Character();
    };

    // TODO: probably reading the file or something for the bullets
    // and also fix key repeat
    class ShootingCharacter : public Character{
        void temp_shoot_bullet(const NewCamera::Camera& camera);
        virtual void update(const NewCamera::Camera& camera)override{
            if(PollKeyboard::get().mb_1){
                temp_shoot_bullet(camera);
            }
        }
        public:
        ShootingCharacter(entt::entity e, glm::vec3 _position, glm::vec3 _rotation):Character(e, _position, _rotation){}
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

        // TODO: character should be virtual and this func goes there by update
        // check if bullet has some channels or flags for collision, or perhaps it is just something that shoulb be ignored by custom collision detection in the kinematic character
        
        static ControllerSingleton& get(){
            static ControllerSingleton instance;
            return instance;
        }
    };
}

#endif // ACTOR_H_
