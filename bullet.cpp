#include "bullet.h"
#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

// need no return something maybe. no
bool BulletScene::stop_manage_representation(RepresentationKey key){
    auto& manager = RepresentationManager<BulletRepresentation>::get();
    auto& rep = manager.get_representation(key);
    auto e = rep.get_entity_id();
    for(auto& x : entities.bullet_entity_list){
        if(x.e == e){
            world->removeRigidBody(x.rb);
            delete x.collision_shape;
            delete x.motion_state;
            delete x.rb;
            break;
        }
    }

    return 1;
}

bool BulletScene::manage_representation(RepresentationKey key, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale){
    auto& manager = RepresentationManager<BulletRepresentation>::get();
    auto& rep = manager.get_representation(key);
    auto e = rep.get_entity_id();
    btRigidBody* rb{nullptr};
    switch(rep.representation_type){
        case RepresentationType::SPHERE:{
            float radius{0};
            for(int i = 0; i < 3; ++i)
                if(scale[i] > radius)
                    radius = scale[i];
            rb = entities.create_ball(e, position, rotation, radius);
            world->addRigidBody(rb);
            break;
        }
        case RepresentationType::BOX:{
            rb = entities.create_box(e, position, rotation, scale);
            world->addRigidBody(rb);
            break;
        }
        default:{
            return 0;
        }
    }
    key.registry_owner = RegistryOwner::BULLETREGISTRY;
    return 1;
}


void BulletScene::other_copy_over(){
    for(auto& entity : entities.bullet_entity_list){
        btTransform t;
        entity.rb->getMotionState()->getWorldTransform(t);
        btVector3 t_position = t.getOrigin();
        btScalar z, y, x;
        btQuaternion t_rotation = t.getRotation();
        t_rotation.getEulerZYX(z, y, x);
        auto& registry = Registry::get().registry;
        auto& position = registry.get<Component::Position>(entity.e);
        auto& rotation = registry.get<Component::Rotation>(entity.e);
        rotation.x = Math::rad_to_deg(x);
        rotation.y = Math::rad_to_deg(y);
        rotation.z = Math::rad_to_deg(z);
        position.x = t_position.x();
        position.y = t_position.y();
        position.z = t_position.z();   
    }
}

void BulletScene::init() {
    btVector3 worldMin(-1000, -1000, -1000);
    btVector3 worldMax(1000, 1000, 1000);
    btAxisSweep3* sweepBP = new btAxisSweep3(worldMin, worldMax);
    sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());

    config =        new btDefaultCollisionConfiguration();
    dispatcher =    new btCollisionDispatcher(config);
    solver =        new btSequentialImpulseConstraintSolver();
    world =         new btDiscreteDynamicsWorld(dispatcher, sweepBP, solver, config);
    world->setGravity(btVector3(0, -10, 0));
    auto& registry = Registry::get().registry;
    entities.spawn_entites(
        [&](btRigidBody* body) { world->addRigidBody(body); },
        registry
    );
}

BulletScene& BulletScene::get(){
    static BulletScene instance;
    return instance;
}


/*
std::vector<entt::entity>& BulletScene::get_entities() {
    return entities.entity_list;
}
*/

/*
const std::unordered_map<entt::entity, btRigidBody*>& BulletScene::get_rb_map(){
    return entities.rb_map;
}
*/

void BulletScene::world_rem_rigid_body(btRigidBody* rb) {
    world->removeRigidBody(rb);
}

void BulletScene::world_add_rigid_body(btRigidBody* rb){
    world->addRigidBody(rb);
}

void BulletScene::world_add_rigid_body(btRigidBody* rb, btTransform t){
    world->addRigidBody(rb);
}

void BulletScene::step(float delta_time) {
    btScalar dt = btScalar(delta_time);
    btScalar fixed_time_step = btScalar(1.0) / btScalar(60.0);
    world->stepSimulation(dt, 1, fixed_time_step);
}

// move this to math
 /*
btVector3 to_btVector3(glm::vec3 vec){ return btVector3{btScalar(vec.x),btScalar(vec.y),btScalar(vec.z)}; }
*/
void BulletScene::temp_throw_ball(glm::vec3 position, glm::vec3 direction, float velocity){
    auto uniform_scale = 0.2f;
    // type is not in scope and this function needs to be declared
    auto e = RepresentationManager<BulletRepresentation>::get().create_bullet_object(RepresentationType::SPHERE_WITH_NORMAL, position, glm::vec3(0, 0, 0), glm::vec3(uniform_scale, uniform_scale, uniform_scale));
    auto* rb = entities.create_ball(e, position, glm::vec3(0, 0, 0), uniform_scale);
    auto linear_velocity = to_btVector3(direction * velocity);
    world->addRigidBody(rb);
    rb->setLinearVelocity(linear_velocity);
}

BulletScene::BulletScene()
{
}


