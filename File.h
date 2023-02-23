#ifndef FILE_H_
#define FILE_H_

#include <fstream>
#include <tuple>
#include <string>
#include "Bullet3Collision/BroadPhaseCollision/b3BroadphaseCallback.h"
#include "BulletDynamics/Dynamics/btRigidBody.h"
#include "BulletCollision/CollisionDispatch/btCollisionObject.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseInterface.h"
#include "BulletCollision/BroadphaseCollision/btBroadphaseProxy.h"
#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionDispatch/btCollisionDispatcher.h"
#include "BulletCollision/BroadphaseCollision/btDbvtBroadphase.h"
#include "BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h"
#include "BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h"
#include "BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h"
#include "entt/entt.hpp"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "glm/glm.hpp"

// load token instructions for further parsing

namespace ParserFunctions{

int first_non_space(const std::string& str, int index);

std::string float_to_str(float fl);

std::string glm_vec3_to_str(const glm::vec3 vec);

std::string bullet_quaternion_to_str(const btQuaternion quaternion);

std::tuple<int, std::string> get_word(const std::string& str, int index);

std::tuple<int, float> get_float(const std::string& str, int index);

std::tuple<int, btQuaternion> get_quaternion(const std::string& str, int index);

std::tuple<int, glm::vec3> get_vec3(const std::string& str, int index);

template<typename T>
std::vector<T> read_parse_file(std::string file_loc_name, std::function<T(std::string)> item_parse_func){
    std::ifstream file;
    file.open(file_loc_name);
    std::string line;
    std::vector<T> out;

    if(file.is_open()){
        while(getline(file, line))
            out.push_back(T(line));
    }

    return out;
};

std::vector<std::string> read_file(std::string file_loc_name);

// return next pos or something
// create a custom stream or stream operators for level file thingy
void write_file(std::string file_loc_name, const std::vector<std::string>& vec);

}

#endif // FILE_H_
