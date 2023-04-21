#include "bullet.h"
#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"
#include "World.h"

#include "Math.h"

// this needs its own lifetimesystem and event system

bool BulletScene::stop_manage_representation(RepresentationKey key){
    //auto& manager = RepresentationManager<BulletRepresentation>::get();
    auto& manager = BulletRepresentationManager::get();
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

bool BulletScene::manage_representation(RepresentationKey key, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, BulletInit::BulletConfigData config, btCollisionShape* collision_shape){
    bool is_dynamic = config.mass != 0;
    auto& manager = BulletRepresentationManager::get();

    auto& rep = manager.get_representation(key);
    auto e = rep.get_entity_id();

    auto quat = btQuaternion(btRadians(rotation.y), btRadians(rotation.x), btRadians(rotation.z));


    btTransform t;
    t.setIdentity();
    t.setOrigin(btVector3(btScalar(position.x), btScalar(position.y), btScalar(position.z)));
    t.setRotation( quat );

    auto out = init_from_config_and_collision_shape(config, collision_shape, t, is_dynamic);

    if(is_dynamic)
        out.rb->setAnisotropicFriction(collision_shape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);


    world->addRigidBody(out.rb);

    entities.bullet_entity_list.emplace_back(BulletEntity(e, out.rb, out.collision_shape, out.motion_state));
    return 1;
}

void BulletScene::other_copy_over(){
    for(auto& entity : entities.bullet_entity_list){
        btTransform t;
        entity.rb->getMotionState()->getWorldTransform(t);
        btVector3 t_position = t.getOrigin();
        btScalar z, y, x;
        btQuaternion t_rotation = t.getRotation();
        auto& registry = Registry::get().registry;
        auto& position = registry.get<Component::Position>(entity.e);
        auto& rotation = registry.get<Component::OpenGLRotation>(entity.e);
        btMatrix3x3 m;
        m.setRotation(t_rotation);

        rotation.rotation = glm::mat4(1.0);
        for(int i = 0; i < 3; ++i){
             for(int j = 0; j < 3; ++j){
                rotation.rotation[i][j] = m[j][i];
             }
        }

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

    /*
    entities.spawn_entites(
        [&](btRigidBody* body) { world->addRigidBody(body); },
        registry

    );
     */
}


BulletScene& BulletScene::get(){
    static BulletScene instance;
    return instance;
}



void BulletScene::step(float delta_time) {
    btScalar dt = btScalar(delta_time);
    btScalar fixed_time_step = btScalar(1.0) / btScalar(60.0);
    world->stepSimulation(dt, 1, fixed_time_step);
}


void BulletScene::temp_throw_ball(glm::vec3 position, glm::vec3 direction, float velocity){
    static float last = World::Clock::get().get_time();
    if(World::Clock::get().get_time() - last < 0.1)
        return;
    last = World::Clock::get().get_time();
    auto uniform_scale = 0.2f;
    auto e = BulletRepresentationManager::get().create_bullet_object("/home/solidus/Assets/Normal/sphere_with_normal_map.obj", position, glm::vec3(0, 0, 0), glm::vec3(uniform_scale, uniform_scale, uniform_scale));

    auto* rb = entities.create_ball(e, position, glm::vec3(0, 0, 0), uniform_scale);
    auto linear_velocity = to_btVector3(direction * velocity);
    world->addRigidBody(rb);
    rb->setLinearVelocity(linear_velocity);

}


