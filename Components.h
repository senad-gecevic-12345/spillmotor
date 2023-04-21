#ifndef COMPONENTS_H_
#define COMPONENTS_H_

#include <vector>
#include <string>
#include "tiny_obj_loader.h"
#include <chrono>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Math.h"

namespace ApplyForceFunctions{
	class ApplyForceKey;
}


namespace Component {


    struct AnimationController{
        int animation_location={-1};
        int playing_animation={-1};
        int no_animations={0};
    };
    struct _3DObjectData{
        bool has_bullet_representation;
        bool can_dynamic_edit;
    };

	struct SphereCollider {
		float radius;
	};

	struct Scale {
		glm::vec3 scale;
	};

	struct Color {
		glm::vec3 color;
	};

	struct ActivateTime {
		double time;
	};

    struct OpenGLRotation{
        glm::mat4 rotation;
    };

struct Rotation {
    union{
        struct{
            float x, y, z;
        };
        glm::vec3 rotation;
    };
};


struct SelectedShader{
	glm::vec4 color = glm::vec4(0,0,0,0);
};

struct Mesh {
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uv;
	std::vector<int> indices;
	std::vector<int> ebo;
    unsigned int triangle_count;
};
/*
struct MeshSingle {
    unsigned int triangle_count;
    float* vertex;
    int* ebo;
    int vertex_size;
    int ebo_size;
};
*/

// is vao ebo etc
/*
struct MeshIndex{

};
*/

// multiple meshes 
// probably mesh index or something and then can order the indexes per render group for better caching
// see render thing
struct MeshPointers{
    std::vector<Mesh*> meshes;
    std::vector<glm::vec3> offset;
};

struct Force {
	glm::vec3 vel;
};

struct LifeTime {
	float seconds;
	int condition; // for example if goes out of screen space
	float spawn_time;
	LifeTime(float seconds, float spawn_time):seconds(seconds),spawn_time(spawn_time) {}
	LifeTime() {}
};

struct Texture{
	unsigned int id;
	unsigned char* data;
	int width;
	int height;
	int bits_per_pixel;
};
struct Render {
	unsigned int EBO;
	unsigned int VAO;
	unsigned int VBO;
    unsigned int triangle_count;
};
struct Render3D {
	unsigned int EBO;
	unsigned int VAO;
	unsigned int VBO;
};

// may need id for which applies
// perhaps make key default
// or add sender class
// or add them... yeah
class ApplyForce {
	friend class ApplyForceFunctions::ApplyForceKey;
public:
	std::vector<Force> force;
};

struct Name { 
	std::string str;
	Name(std::string name):str(name){}
	Name() {}
};


struct ControllVelocity {
	float x, y, z;
	bool is_empty() {
		return(x == 0 && y == 0 && z == 0);
	}
	ControllVelocity(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}



	ControllVelocity():x(0.f),y(0.f),z(0.f){}

	ControllVelocity operator *(float val){
		return ControllVelocity(x * val, y * val, z * val);
	}
	ControllVelocity operator /(float val) {
		return ControllVelocity(x / val, y / val, z / val);
	}

};

struct Velocity{
	union{
	struct{
		float x, y, z;
		};
		glm::vec3 velocity;
	};

	Velocity(float x, float y, float z){
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void operator +=(ControllVelocity vel) {
		x += vel.x;
		y += vel.y;
		z += vel.z;
	}

	Velocity():x(0.f),y(0.f),z(0.f){}

	Velocity operator *(float val){
		return Velocity(x * val, y * val, z * val);
	}
	Velocity operator /(float val) {
		return Velocity(x / val, y / val, z / val);
	}
};

struct Position{
	union {
		struct
		{
			float x, y, z;
		};
		glm::vec3 pos;
	};
	/*
	Position(float x, float y, float z):x(x), y(y), z(z){}

	Position():x(1.f), y(1.f), z(1.f){}
	Position& operator+=(Force force) {
		x += force.vel.x;
		y += force.vel.y;
		z += force.vel.z;
		return *this;
	}
	Position& operator+=(Velocity vel){
		x += vel.x;
		y += vel.y;
		z += vel.z;
		return *this;
	}
	*/
};

}


#endif // COMPONENTS_H_
