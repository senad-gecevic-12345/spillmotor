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
#include "Renderer.h"
#include "Representation.h"



//bullet. if stack sphere and reset one under top, then top floats unless something move the one on top

// lighting code

// need then to not use entt directly but if not using somewhere can exclude
// no for assign bullet entities or something. just use where is useful

struct BulletActorInfo {
	btRigidBody* rb;
	btRigidBody::btRigidBodyConstructionInfo info;
    BulletActorInfo(btRigidBody* rb, btRigidBody::btRigidBodyConstructionInfo info):rb(rb), info(info){}
};

struct DebugComponentBitset {
	std::vector<std::tuple<entt::entity, std::bitset<64>>> entities;
};

// TODO: copy over from windows and change dir
namespace {
	inline std::string unit_shapes_folder{ "/home/solidus/Assets/UnitShapes/" };
	inline std::string unit_sphere_obj{unit_shapes_folder + "sphere.obj"};
	inline std::string unit_sphere_mtl{unit_shapes_folder + "sphere.mtl"};
	inline std::string unit_cone_obj{ unit_shapes_folder + "cone.obj"};
	inline std::string unit_cone_mtl{ unit_shapes_folder + "cone.mtl"};
	inline std::string unit_box_obj{ unit_shapes_folder + "box.obj"};
	inline std::string unit_box_mtl{ unit_shapes_folder + "box.mtl"};
	inline std::string unit_cylinder_obj{ unit_shapes_folder + "cylinder.obj"};
	inline std::string unit_cylinder_mtl{ unit_shapes_folder + "cylinder.mtl"};
};



class BulletScene;

// something something double delete
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

// should be just a struct
class BulletEntities {
#define NO_SHAPES 5
public:
	std::vector<BulletActorInfo> entity_info_list;
    std::vector<btRigidBody*> rb_list;
    std::vector<BulletEntity> bullet_entity_list;

	void spawn_entites(std::function<void(btRigidBody*)> add/*, entt::registry& registry = nullptr */, entt::registry& registry) {

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
			//entity_map[body] = e;
            //rb_map[e] = body;

			registry.emplace<Tags::BulletPhysics>(e);
            registry.emplace<Tags::RepresentationRenderNewMeshLoader>(e);
            registry.emplace<MeshLoaderNew::MeshRenderId>(e, MeshLoaderNew::load_mesh(unit_box_obj));
			registry.emplace<Component::Position>(e, t_position.x(), t_position.y(), t_position.z());
			registry.emplace<Component::Rotation>(e, Math::rad_to_deg((float)x), Math::rad_to_deg((float)y), Math::rad_to_deg((float)z));
			registry.emplace<Component::Scale>(e, glm::vec3(100.f, 10.f, 100.f));
            bullet_entity_list.emplace_back(BulletEntity(e, body, ground_shape, motion_state));

			//entity_list.push_back(e);
            entity_info_list.push_back(BulletActorInfo(body, rb_info));
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

				//entity_map[body] = e;
                //rb_map[e] = body;
				registry.emplace<Tags::BulletPhysics>(e);
                registry.emplace<Tags::RepresentationRenderNewMeshLoader>(e);

                registry.emplace<MeshLoaderNew::MeshRenderId>(e, MeshLoaderNew::load_mesh(unit_sphere_obj));
				registry.emplace<Component::Position>(e, t_position.x(), t_position.y(), t_position.z());
				registry.emplace<Component::Rotation>(e, (float)rotation_x, (float)rotation_y, (float)rotation_z);
				registry.emplace<Component::Scale>(e, glm::vec3(radius, radius, radius));
				//entity_list.push_back(e);
                bullet_entity_list.emplace_back(BulletEntity(e, body, sphere_shape, motion_state));
                entity_info_list.push_back(BulletActorInfo(body, rb_info));
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
        rb_list.push_back(body);

        bullet_entity_list.emplace_back(BulletEntity(e, body, collision_shape, motion_state));
        //world->addRigidBody(body);
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
        rb_list.push_back(body);
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


public:
    bool manage_representation(RepresentationKey key, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
    bool stop_manage_representation(RepresentationKey key);

public:
    static BulletScene& get();

public:
	BulletEntities entities;

	bool last_inactive;

public:
	std::vector<entt::entity>& get_entities() ;
    void other_copy_over();

public:
	void init();

public:
	void world_rem_rigid_body(btRigidBody* rb);
    void world_add_rigid_body(btRigidBody* rb);
    void world_add_rigid_body(btRigidBody* rb, btTransform t);

	void step(float delta_time);

	BulletScene();

};
#endif // BULLET_H_

