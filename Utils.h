#ifndef UTILS_H_
#define UTILS_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "entt/entt.hpp"
#include "Components.h"
#include "Tags.h"
#include "File.h"
#include <tuple>

#include "MeshFunctions2.h"

namespace Utils{
    inline void copy_over_fix(entt::registry& registry){
        static_cast<void>(registry.storage<Tags::TriangleEnemy>());
        static_cast<void>(registry.storage<Tags::Ally>());
        static_cast<void>(registry.storage<Tags::TriangleProjectile >());
        static_cast<void>(registry.storage<Tags::Player >());
        static_cast<void>(registry.storage<Tags::Triangle >());
        static_cast<void>(registry.storage<Tags::Bullet >());
        static_cast<void>(registry.storage<Tags::ManualRender >());
        static_cast<void>(registry.storage<Tags::Model3D>());
        static_cast<void>(registry.storage<Tags::TexModel3D>());
        static_cast<void>(registry.storage<Tags::Sphere>());
        static_cast<void>(registry.storage<Tags::Physics>());
        static_cast<void>(registry.storage<Tags::BulletPhysics>());
        static_cast<void>(registry.storage<Tags::RepresentationRender>());
        static_cast<void>(registry.storage<Tags::RepresentationRenderNewMeshLoader>());

        static_cast<void>(registry.storage<RenderTags::_3D_OBJECT>());
        static_cast<void>(registry.storage<RenderTags::_3D_TEXTURE>());
        static_cast<void>(registry.storage<RenderTags::_2D_COLOR>());
        static_cast<void>(registry.storage<RenderTags::_2D_DEFAULT>());

        static_cast<void>(registry.storage<MeshLoaderNew::MeshRenderId>());
        static_cast<void>(registry.storage<Component::Scale>());
        static_cast<void>(registry.storage<Component::Position>());
        static_cast<void>(registry.storage<Component::Rotation>());
        static_cast<void>(registry.storage<Component::Render>());
        static_cast<void>(registry.storage<Component::Color>());
        static_cast<void>(registry.storage<Component::_3DObjectData>());
        static_cast<void>(registry.storage<Component::SphereCollider>());
        static_cast<void>(registry.storage<Component::SelectedShader>());
        static_cast<void>(registry.storage<Component::Mesh>());
        static_cast<void>(registry.storage<Component::MeshPointers>());
        static_cast<void>(registry.storage<Component::Force>());
        static_cast<void>(registry.storage<Component::LifeTime>());
        static_cast<void>(registry.storage<Component::Texture>());
        static_cast<void>(registry.storage<Component::Render3D>());
        static_cast<void>(registry.storage<Component::ApplyForce>());
        static_cast<void>(registry.storage<Component::ControllVelocity>());
        static_cast<void>(registry.storage<Component::Velocity>());
        static_cast<void>(registry.storage<Component::Name>());

    }

inline void copy_over_to_other_registry(entt::registry& to_registry, entt::registry& from_registry, entt::entity to, entt::entity from){
    for(auto [id, storage]: from_registry.storage()) {
        if(auto *other = to_registry.storage(id); other && storage.contains(from)) {
            other->emplace(to, storage.get(from));
        }
    }
}

}
namespace SHADERS{

    inline unsigned int load_shader(const char* shader_loc_name, int type);


    template<typename ...Args>
    unsigned int link_shaders(Args... args){
        unsigned int shader_program;
        shader_program = glCreateProgram();
        (glAttachShader(shader_program, args), ...);
        glLinkProgram(shader_program);

        int success;
        char log[512];
        glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader_program, 512, NULL, log);
            std::cout << "LINKING FAILED\n" << log << std::endl;
        }

        (glDeleteShader(args), ...);
        return shader_program;
    }

}

namespace SHADERS{
    inline char shader_folder[] = "/home/solidus/Assets/Shaders/\0";

    struct uniform_vec3{
        unsigned int uniform;
    };

    struct FlashLightShaderNew{
        unsigned int shader_program;
        unsigned int u_model, u_view, u_proj, u_view_pos;
        unsigned int u_cutoff;
        unsigned int u_position, u_direction;

        void uniform(glm::mat4 view, glm::mat4 proj, glm::vec3 position, glm::vec3 direction);
        void bind();

        FlashLightShaderNew();

    };

    struct SkyBoxShader{
        unsigned int shader_program;
        unsigned int u_view, u_proj;
        void uniform(glm::mat4 view, glm::mat4 proj);
        void bind();
        SkyBoxShader();
    };

    struct FlashLightShader{
        unsigned int shader_program;
        unsigned int u_model, u_view, u_proj, u_view_pos;
        unsigned int u_constant, u_linear, u_quadratic;
        unsigned int u_cutoff;
        unsigned int u_shininess;
        unsigned int u_position, u_direction;
        void uniform(glm::mat4 view, glm::mat4 proj, glm::vec3 position, glm::vec3 direction);
        void bind();
        FlashLightShader();

    };

    // maybe have lambda thing
    struct BulletShader{
        unsigned int shader_program;
        unsigned int
        u_view, u_proj, u_model,
        u_light_pos, u_light_dir, u_light_col;
        void bind();
        void uniform(glm::mat4 view, glm::mat4 proj);
        BulletShader();
    };
}

namespace CreateRender {
    void create_render_triangle(Component::Render& render);
    void create_render(Component::Render& render, Component::Mesh& mesh);
}

namespace Functions {

    template<typename ...Args>
    void emplace(entt::registry& registry, entt::entity e) {
        ((registry.emplace<Args>(e)), ...);
    }

	template<typename ...Args>
	decltype(auto) get(entt::registry& registry, entt::entity e) {
		return registry.get<Args...>(e);
	}


	void for_each_enemies(entt::registry& registry, std::function<void(entt::registry&, entt::entity)> func);


	void print(Component::Velocity vel);
	void print(Component::Position pos);
	void print_enemy_names(entt::registry& registry); 	
	void print_ally_names(entt::registry& registry);
	
	void spawn_allies(entt::registry& registry, unsigned int count);
	void spawn_enemies(entt::registry& registry, unsigned int count /*some sort of context*/);

}

namespace Background {
	void create_render_background(Component::Render& render);
	void init(entt::registry& registry);
	unsigned int create_shader();
	void draw_background(entt::registry& registry, unsigned int shader_program);
}

void create_triangle(entt::registry& registry, Component::Render& render);
void create_triangle(entt::registry& registry);


class Registry{
public:
	entt::registry registry;
	static Registry& get() {
        static Registry instance;
		return instance;
	}

    Registry& operator=(const Registry&)=delete;
    Registry(const Registry&)=delete;

    Registry()=default;
    ~Registry()=default;
};

#endif // UTILS_H_
