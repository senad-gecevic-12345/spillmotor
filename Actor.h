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
#include <array>

namespace Component{
    struct Position;
    struct Rotation;
}

inline btVector3 to_btVector3(glm::vec3 vec){ return btVector3{btScalar(vec.x),btScalar(vec.y),btScalar(vec.z)}; }

struct Timer{
    std::array<float, 4> expr;
    std::array<float, 4> accumulator;

    void set_expr(int index, float expr_time){
        expr[index] = expr_time;
        accumulator[index] = expr_time;
    }

    void update(float dt){
        for(int i = 0; i < 4; ++i){
            accumulator[i] -= dt;
        }
    }

    bool is_expired(int index){
        if(accumulator[index] <= 0.f){
            accumulator[index] = expr[index];
            return 1;
        }
        return 0;
    }

};

namespace Controller{

    class Character{
    public:
        // used for a destroy for the once that is controlled
        entt::entity e;
        Timer timer;
    private:
        BULLETCOPY::btKinematicCharacterController* controller;
        btPairCachingGhostObject* ghost_obj;

    public:

        bool valid(){
            return (controller != nullptr && ghost_obj != nullptr);
        }

        void copy_over(entt::registry& registry);

        // this dont own the camera so not that good
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
            // should update all
            //

            timer.update(dt);

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
                if(controller->canJump() && timer.is_expired(0))
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
            e = (std::move(other.e));
            controller = (std::exchange(other.controller, nullptr));
            ghost_obj = std::exchange(other.ghost_obj, nullptr);
            return *this;
        }
        Character():ghost_obj(nullptr), controller(){};
        Character(Character&& other):
            e(std::move(other.e)),
            controller(std::exchange(other.controller, nullptr)),
            ghost_obj(std::exchange(other.ghost_obj, nullptr)){}
        // which registry does this belong to. probably bullet registry

        // is this called
        virtual ~Character();
    };

    // TODO: a character should own its own camera
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
        ShootingCharacter(entt::entity e, glm::vec3 _position, glm::vec3 _rotation):Character(e, _position, _rotation){
            timer.set_expr(0, 0.7);
        }
    };

    class ControllerSingleton{
    public:
        NewCamera::Camera camera;
        std::unique_ptr<Controller::Character> character;
        Light::PerspectiveLight light;
    public:
        bool active(){return character != nullptr;}
        void clear(){
            auto& registry = Registry::get().registry;
            if(character != nullptr)
                registry.destroy(character->e);
            character.reset();
        }

        ControllerSingleton(const ControllerSingleton&)=delete;
        ControllerSingleton& operator=(const ControllerSingleton&)=delete;
        ControllerSingleton():character(nullptr), light(20.f, camera.m_follow_pos, camera.m_front){};
        ~ControllerSingleton()=default;

        void update(entt::registry& registry, btCollisionWorld* world, float dt, float mouse_x, float mouse_y);

        static ControllerSingleton& get(){
            static ControllerSingleton instance;
            return instance;
        }
    };
}

#endif // ACTOR_H_
