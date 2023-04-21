#include "Actor.h"
#include "bullet.h"

#include "Components.h"

namespace Controller{
    void ShootingCharacter::temp_shoot_bullet(const NewCamera::Camera& camera){
        auto& registry = Registry::get().registry;
        auto character_position = registry.get<Component::Position>(e);

        auto xz_direction = -glm::normalize(glm::vec3(camera.m_front.x, 0, camera.m_front.z));

        auto pos = glm::vec3(character_position.x, character_position.y, character_position.z) + (xz_direction*5.f);
        BulletScene::get().temp_throw_ball(pos, xz_direction, 100.f);
    }

    void ControllerSingleton::update(entt::registry& registry, btCollisionWorld* world, float dt, float mouse_x, float mouse_y){
        glm::vec3 out_pos(0,0,0);
        glm::vec3 up(0,1,0);
        glm::vec3 front = camera.get_front();
        front.y = 0;
        front = glm::normalize(front);
        glm::vec3 straife = -glm::cross(front, up);

        FBO::ShadowMapSingleton& shadowmap = FBO::ShadowMapSingleton::get();
        light.update(front, camera.m_follow_pos);

        glm::mat4 glm_proj = glm::perspective(45.f, 1920.f / 1080.f, 0.1f, 500.f);

        float near_plane = 1.0f, far_plane = 7.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

        glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), 
                              glm::vec3( 0.0f, 0.0f,  0.0f), 
                              glm::vec3( 0.0f, 1.0f,  0.0f));

        glm::mat4 lightSpaceMatrix = lightProjection * lightView;         

        shadowmap.update(light.get_light_matrix());

        if(character != nullptr){ 
            character->update_poll_keyboard(world, dt, front, straife); 
            character->copy_over(registry);
            auto position = registry.get<Component::Position>(character->e); 
            out_pos = position.pos; 
            character->update(camera);
        }

        camera.update(mouse_x, mouse_y, out_pos);
    }

    void Character::copy_over(entt::registry& registry){
        btTransform t;
        t = ghost_obj->getWorldTransform();
        btVector3 t_position = t.getOrigin();
        btScalar z, y, x;
        btQuaternion t_rotation = t.getRotation();
        t_rotation.getEulerZYX(z, y, x);

        auto& position = registry.get<Component::Position>(e);
        auto& rotation = registry.get<Component::Rotation>(e);

        rotation.x = Math::rad_to_deg(x);
        rotation.y = Math::rad_to_deg(y);
        rotation.z = Math::rad_to_deg(z);

        position.x = t_position.x();
        position.y = t_position.y();
        position.z = t_position.z();
    }

    Character::Character(entt::entity e, glm::vec3 _position, glm::vec3 _rotation):e(e)
    {
        btVector3 position = to_btVector3(_position);
        btVector3 rotation = to_btVector3(_rotation);
        auto* collision_shape = BulletScene::get().entities.create_capsule(position, rotation, 1.0, 4.0);
        btTransform t;
        t.setIdentity();
        t.setOrigin(position);
        auto set_rotation = btQuaternion();
        set_rotation.setEulerZYX(btScalar(0), btScalar(0), btScalar(0));
        t.setRotation(set_rotation);
        ghost_obj = new btPairCachingGhostObject();
        ghost_obj->setWorldTransform(t);
        ghost_obj->setCollisionShape(collision_shape);
        ghost_obj->setCollisionFlags(btCollisionObject::CF_CHARACTER_OBJECT);
        BulletScene::get().world->addCollisionObject(ghost_obj, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::StaticFilter|btBroadphaseProxy::DefaultFilter);
        auto half_step = btScalar(0.35); 
        auto up = btVector3(0, 1, 0);
        controller = new BULLETCOPY::btKinematicCharacterController(ghost_obj, collision_shape, half_step, up);
    }

    Character::~Character(){
        if(ghost_obj != nullptr)
            BulletScene::get().world->removeCollisionObject(ghost_obj);
        delete controller;
        delete ghost_obj;
    }
}



