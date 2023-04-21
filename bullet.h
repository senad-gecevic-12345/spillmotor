#ifndef BULLET_H_
#define BULLET_H_

#include "Window.h"
#include "Bullet3Collision/BroadPhaseCollision/b3BroadphaseCallback.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "entt/fwd.hpp"
#include "BulletCollision/CollisionShapes/btCapsuleShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "LinearMath/btDefaultMotionState.h"
#include "Components.h"
#include "TextureFunctions.h"
#include <unordered_map>
#include "RenderFunctions.h"
#include "Tags.h"
#include "Utils.h"
#include <bitset>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include "Renderer.h"
#include "Representation.h"


// TODO: check if this still holds bullet. if stack sphere and reset one under top, then top floats unless something move the one on top
namespace BulletInit{
    struct BulletConfigData{
        float mass;
        float friction;
        float restitution;
        float rolling_friction;
        float spinning_friction;
        BulletConfigData& operator=(const BulletConfigData& other){
            mass = (other.mass);
            friction = (other.friction);
            restitution = (other.restitution);
            rolling_friction = (other.rolling_friction);
            spinning_friction = (other.spinning_friction);
            return *this;
        }

        BulletConfigData(const BulletConfigData& other):mass(other.mass), friction(other.friction), restitution(other.restitution), rolling_friction(other.rolling_friction), spinning_friction(other.spinning_friction){}
        BulletConfigData():mass(0), friction(0), restitution(0), rolling_friction(0), spinning_friction(0){};
        BulletConfigData(float mass, float friction, float restitution, float rolling_friction, float spinning_friction):
            mass(mass), friction(friction), restitution(restitution), rolling_friction(rolling_friction), spinning_friction(spinning_friction){}
    };

    struct BulletInitOut{
        btCollisionShape* collision_shape;
        btRigidBody* rb;
        btDefaultMotionState* motion_state;
        BulletInitOut(btCollisionShape* collision_shape, btRigidBody* rb, btDefaultMotionState* motion_state):collision_shape(collision_shape), rb(rb), motion_state(motion_state){}
    };



    // TODO: this should be managed by bullet not representation manager which is for the editor. since here dont have a entity id. should push back to entity list for bulletsystems
    // may not be part of copy over
    // or this is called when the representation key is polled from the system and then it takes the entity id from the bullet representation and copies it here, and on that it will construct the bullet memes
    inline BulletInitOut init_from_config_and_collision_shape(const BulletConfigData& data, btCollisionShape* collision_shape, btTransform t, bool is_dynamic){
        float mass = (is_dynamic) ? data.mass : 0;

        btVector3 local_inertia(0, 0, 0);

        if(is_dynamic)
            collision_shape->calculateLocalInertia(mass, local_inertia);

        btDefaultMotionState* motion_state = new btDefaultMotionState(t);
        btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, collision_shape, local_inertia);
        btRigidBody* body = new btRigidBody(rb_info);

        body->setRestitution(data.restitution);
        body->setFriction(data.friction);

        if(is_dynamic){
            body->setRollingFriction(data.rolling_friction);
            body->setSpinningFriction(data.spinning_friction);
        }

        return BulletInitOut(collision_shape, body, motion_state);
    }

    inline BulletInitOut temp_init_from_collision_shape(btCollisionShape* collision_shape, btTransform t, btScalar mass){
        bool is_dynamic = (mass != 0.f);
        btVector3 local_inertia(0, 0, 0);
        if(is_dynamic)
            collision_shape->calculateLocalInertia(mass, local_inertia);
        btDefaultMotionState* motion_state;
        motion_state = new btDefaultMotionState(t);

        btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, collision_shape, local_inertia);
        btRigidBody* body = new btRigidBody(rb_info);
        if(is_dynamic){
            body->setRestitution(0.9);
            body->setFriction(0.2);
            body->setRollingFriction(0.1);
            body->setSpinningFriction(0.1);
            body->setAnisotropicFriction(collision_shape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
        }

        return BulletInitOut(collision_shape, body, motion_state);
    }



}

namespace {
	inline std::string unit_shapes_folder{ "/home/solidus/Assets/UnitShapes/" };
    inline std::string sphere_with_normal{"/home/solidus/Assets/Normal/sphere_with_normal_map.obj"};
	inline std::string unit_sphere_obj{unit_shapes_folder + "sphere.obj"};
	inline std::string unit_sphere_mtl{unit_shapes_folder + "sphere.mtl"};
	inline std::string unit_cone_obj{ unit_shapes_folder + "cone.obj"};
	inline std::string unit_cone_mtl{ unit_shapes_folder + "cone.mtl"};
    inline std::string unit_box_ground_shape{"/home/solidus/Downloads/aerial_rocks_01_2k.blend/aerial_rocks_01_2k_ground.obj"};
	inline std::string unit_box_obj{ unit_shapes_folder + "box.obj"};
	inline std::string unit_box_mtl{ unit_shapes_folder + "box.mtl"};
	inline std::string unit_cylinder_obj{ unit_shapes_folder + "cylinder.obj"};
	inline std::string unit_cylinder_mtl{ unit_shapes_folder + "cylinder.mtl"};
};



class BulletScene;

class BulletEntity{
    entt::entity e;
    btRigidBody* rb;
    btCollisionShape* collision_shape;
    btDefaultMotionState* motion_state;
public:
    BulletEntity(BulletEntity&& other)noexcept:
        e(std::move(other.e)), rb(std::exchange(other.rb, nullptr)), collision_shape(std::exchange(other.collision_shape, nullptr)), motion_state(std::exchange(other.motion_state, nullptr)){}

    BulletEntity& operator=(BulletEntity&& other)noexcept
    {
        if(this == &other)
            return *this;
        e = (std::move(other.e)); 
        rb = (std::exchange(other.rb, nullptr)); 
        collision_shape = (std::exchange(other.collision_shape, nullptr)); 
        motion_state = (std::exchange(other.motion_state, nullptr));
        return *this;
    }
    BulletEntity& operator=(const BulletEntity& bullet_entity)=delete;
    BulletEntity(const BulletEntity& bullet_entity)=delete;
    BulletEntity(entt::entity e, btRigidBody* rb, btCollisionShape* collision_shape, btDefaultMotionState* motion_state):
        e(e), rb(rb), collision_shape(collision_shape), motion_state(motion_state)
    {
    }
    friend class BulletEntities;
    friend class BulletScene;
};

class BulletEntities {
#define NO_SHAPES 5
public:
    std::vector<BulletEntity> bullet_entity_list;

	void spawn_entites(std::function<void(btRigidBody*)> add, entt::registry& registry) {
		{
			btScalar mass = (0.);
			btCollisionShape* ground_shape = new btBoxShape(btVector3(btScalar(100.), btScalar(10.), btScalar(100.)));
			btTransform t;
			btVector3 rotation_axis(1, 0, 0);
			auto set_rotation = btQuaternion();
			set_rotation.setRotation(rotation_axis, 0.17);
			t.setIdentity();
			//t.setRotation(set_rotation);
            t.setOrigin(btVector3(0, -20, 0));
			bool is_dynamic = (mass != 0.f);
			btVector3 local_inertia(0, 0, 0);
			if (is_dynamic)
				ground_shape->calculateLocalInertia(mass, local_inertia);
			btDefaultMotionState* motion_state = new btDefaultMotionState(t);
			btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, ground_shape, local_inertia);
			btRigidBody* body = new btRigidBody(rb_info);
			body->setFriction(.5);
			body->setRestitution(0.4);
			add(body);

			auto t_position = t.getOrigin();
			btScalar rotation_x, rotation_y, rotation_z;
			t.getRotation().getEulerZYX(rotation_z, rotation_y, rotation_x);
			auto _rotation = t.getRotation();
			float x, y, z;
			_rotation.getEulerZYX(z, y, x);
			auto e = registry.create();

			registry.emplace<Tags::BulletPhysics>(e);
            registry.emplace<Tags::RepresentationRenderNewMeshLoader>(e);
            auto[mesh, flags] = MeshLoaderNew::load_mesh(unit_box_ground_shape);
            registry.emplace<MeshLoaderNew::MeshRenderId>(e, mesh);

			registry.emplace<Component::Position>(e, t_position.x(), t_position.y(), t_position.z());
			registry.emplace<Component::OpenGLRotation>(e);
			registry.emplace<Component::Scale>(e, glm::vec3(100.f, 10.f, 100.f));

            bullet_entity_list.emplace_back(BulletEntity(e, body, ground_shape, motion_state));
		}

		{
			btScalar arr_radius[NO_SHAPES] = {
				0.5, 1.5, 0.5, 1.5, 0.5
			};
			btCollisionShape* shapes[NO_SHAPES] = {
				new btSphereShape(btScalar(arr_radius[0])),
				new btSphereShape(btScalar(arr_radius[1])),
				new btSphereShape(btScalar(arr_radius[2])),
				new btSphereShape(btScalar(arr_radius[3])),
				new btSphereShape(btScalar(arr_radius[4])),
			};


			for (int i = 0; i < NO_SHAPES; ++i) {
				btTransform t;
				t.setIdentity();
				t.setOrigin(btVector3(
					btScalar(i * 5.5),
					btScalar(0.0),
					btScalar(20.0)
				));

				btScalar mass(1.f);
				btCollisionShape* sphere_shape = shapes[i];
				float radius = arr_radius[i];
				bool is_dynamic = (mass != 0.f);
				btVector3 local_inertia(0, 0, 0);
				if (is_dynamic)
					sphere_shape->calculateLocalInertia(mass, local_inertia);
				btDefaultMotionState* motion_state = new btDefaultMotionState(t);
				btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, sphere_shape, local_inertia);
				btRigidBody* body = new btRigidBody(rb_info);

				body->setRestitution(0.9);
				body->setFriction(0.2);
				body->setRollingFriction(0.1);
				body->setSpinningFriction(0.1);
				body->setAnisotropicFriction(sphere_shape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
				add(body);

				auto t_position = t.getOrigin();
				btScalar rotation_x, rotation_y, rotation_z;
				t.getRotation().getEulerZYX(rotation_z, rotation_y, rotation_x);
				auto e = registry.create();

				registry.emplace<Tags::BulletPhysics>(e);
                registry.emplace<Tags::NormalMap>(e);
                registry.emplace<Tags::RepresentationRenderNewMeshLoader>(e);

                // yes bug when shooting temp bullet also seems like normal not loading
                // normal not loading form the editor
                // no bug now
                // FIXME: 
                // use mesh loader new and check for the relevant stuff. also what about tangents. that is probably only for normal map
                
                auto [mesh, flags] = MeshLoaderNew::load_mesh(sphere_with_normal);
                registry.emplace<MeshLoaderNew::MeshRenderId>(e, mesh);
				registry.emplace<Component::Position>(e, t_position.x(), t_position.y(), t_position.z());
				registry.emplace<Component::OpenGLRotation>(e);
				registry.emplace<Component::Scale>(e, glm::vec3(radius, radius, radius));

                bullet_entity_list.emplace_back(BulletEntity(e, body, sphere_shape, motion_state));
			}
		}
	}


	btRigidBody* create_ball(entt::entity e, glm::vec3 position, glm::vec3 rotation, float radius) {
        // delete this
		btCollisionShape* collision_shape = new btSphereShape(btScalar(radius));
        btTransform t;
        t.setIdentity();
        t.setOrigin(btVector3(btScalar(position.x), btScalar(position.y), btScalar(position.z)));
        auto set_rotation = btQuaternion();
        set_rotation.setEulerZYX(rotation.z, rotation.y, rotation.x);
        t.setRotation(set_rotation);
        btScalar mass(1.f);
        bool is_dynamic = (mass != 0.f);
        btVector3 local_inertia(0, 0, 0);
        if(is_dynamic)
            collision_shape->calculateLocalInertia(mass, local_inertia);
        // delete this 
        btDefaultMotionState* motion_state = new btDefaultMotionState(t);
        btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, collision_shape, local_inertia);
        btRigidBody* body = new btRigidBody(rb_info);
        body->setRestitution(0.9);
        body->setFriction(0.2);
        body->setRollingFriction(0.1);
        body->setSpinningFriction(0.1);
        body->setAnisotropicFriction(collision_shape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

        bullet_entity_list.emplace_back(BulletEntity(e, body, collision_shape, motion_state));
        return body;
    }


    // probably should be kinematic
    // may need different configuration as is most likely for a contoller
    // probably delete the rigid body
    // need to disable rotation or something
    btConvexShape* create_capsule(btVector3 position, btVector3 rotation, float radius, float total_height) {
        float height = total_height - (2 * radius);
		btConvexShape* collision_shape = new btCapsuleShape(btScalar(radius), height);
        return collision_shape;
    }

    btRigidBody* create_box(entt::entity e, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
        auto* collision_shape = new btBoxShape(btVector3(btScalar(scale.x),btScalar(scale.y),btScalar(scale.z)));
        btTransform t;
        t.setIdentity();
        t.setOrigin(btVector3(btScalar(position.x), btScalar(position.y), btScalar(position.z)));
        btScalar mass(1.f);
        bool is_dynamic = (mass != 0.f);
        btVector3 local_inertia(0, 0, 0);
        if (is_dynamic)
            collision_shape->calculateLocalInertia(mass, local_inertia);
        btDefaultMotionState* motion_state = new btDefaultMotionState(t);
        btRigidBody::btRigidBodyConstructionInfo rb_info(mass, motion_state, collision_shape, local_inertia);
        btRigidBody* body = new btRigidBody(rb_info);

        body->setFriction(.5);
        body->setRestitution(0.4);
        bullet_entity_list.emplace_back(BulletEntity(e, body, collision_shape, motion_state));
        return body;
    }




};

class BulletScene {
public:
    btCollisionDispatcher* dispatcher;
    btDbvtBroadphase* broadphase;
    btDefaultCollisionConfiguration* config;
    btDiscreteDynamicsWorld* world;
    btSequentialImpulseConstraintSolver* sol;
    btConstraintSolver* solver;


    bool manage_representation(RepresentationKey key, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, BulletInit::BulletConfigData config, btCollisionShape* collision_shape);

    bool stop_manage_representation(RepresentationKey key);

    static BulletScene& get();

	BulletEntities entities;

	std::vector<entt::entity>& get_entities();
    void other_copy_over();

	void init();

    // TODO: temporary memes
    // also can be buggy if spawn the ball inside something so may need some checks first
    // need to create a representation first because of rendering    
    // here a bit problematic to generate a key since this should maybe not manage such things, or they do at bullet entities
    void temp_throw_ball(glm::vec3 position, glm::vec3 direction, float velocity);
	void step(float delta_time);

	BulletScene(){};

};

#endif // BULLET_H_
