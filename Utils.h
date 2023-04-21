#ifndef UTILS_H_
#define UTILS_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "entt/entt.hpp"
#include "Components.h"
#include "Tags.h"
//#include "File.h"
#include <tuple>
#include "Renderer.h"
#include "Window.h"
#include "MeshFunctions2.h"

namespace Debug{
    void create_render_rectangle_top_right(Component::Render& render);
}

namespace Utils{
    void copy_over_fix(entt::registry& registry);
    void copy_over_to_other_registry(entt::registry& to_registry, entt::registry& from_registry, entt::entity to, entt::entity from);
}

namespace SHADERS{

    unsigned int load_shader(const char* shader_loc_name, int type);

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
            std::cout << "LINKING FAILED\n";
            ((std::cout << args << std::endl), ...);
            std::cout << log << std::endl;
        }

        (glDeleteShader(args), ...);
        return shader_program;
    }

}

// now have both shader and shaders
namespace Shader{

    class Shader{

    protected:
    public:
        unsigned int shader_program;
        void bind(){glUseProgram(shader_program);}
        Shader(const std::string vertex_shader, const std::string fragment_shader){
            shader_program = SHADERS::link_shaders(
                SHADERS::load_shader(fragment_shader.c_str(), GL_FRAGMENT_SHADER),
                SHADERS::load_shader(vertex_shader.c_str(), GL_VERTEX_SHADER));
        }
    };

    class ShadowmapShader: public Shader{
    public:
        unsigned int u_light_matrix, u_model;
    public:
        void light_matrix_uniform(const glm::mat4& light_matrix){
            glUniformMatrix4fv(u_light_matrix,  1, GL_FALSE, glm::value_ptr(light_matrix));
        }
        void model_uniform(const glm::mat4& model){
            glUniformMatrix4fv(u_model,  1, GL_FALSE, glm::value_ptr(model));
        }
        ShadowmapShader();
    };
    struct ShadowmapDebugShader : public Shader{
        unsigned int u_depth_map;
        unsigned int u_near, u_far;
        // render
        Component::Render render; 
        void bind(){ glUseProgram(shader_program); }
        void update(float near, float far, unsigned int depth_map_texture_id){
            bind();
            glUniform1f(u_near, 1);
            glUniform1f(u_far, 50);

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, depth_map_texture_id);
            glBindVertexArray(render.VAO);

        }
        ShadowmapDebugShader();
    };

}



namespace SHADERS{
    inline char shader_folder[] = "/home/solidus/Assets/Shaders/\0";

    struct uniform_vec3{
        unsigned int uniform;
    };

    struct FlashLightShaderNormalMap{
        unsigned int shader_program;
        unsigned int u_model, u_view, u_proj, u_view_pos;
        unsigned int u_cutoff;
        unsigned int u_position, u_direction;
        unsigned int u_fov, u_col, u_len;
        unsigned int u_light_projection;

        void uniform(glm::mat4 view, glm::mat4 proj, glm::vec3 position, glm::vec3 direction);
        void uniform(glm::mat4 view, glm::mat4 proj, glm::vec3 position, glm::vec3 direction, float fov, float len, glm::vec3 col);
        void uniform(glm::mat4 view, glm::mat4 proj, glm::vec3 position, glm::vec3 direction, float fov, float len, glm::vec3 col, glm::mat4 light_projection);

        void bind();

        FlashLightShaderNormalMap();

    };

    struct FlashLightShaderNew{
        unsigned int shader_program;
        unsigned int u_model, u_view, u_proj, u_view_pos;
        unsigned int u_cutoff;
        unsigned int u_position, u_direction;
        unsigned int u_light_projection;

        void uniform(glm::mat4 view, glm::mat4 proj, glm::vec3 position, glm::vec3 direction);
        void uniform(glm::mat4 view, glm::mat4 proj, glm::vec3 position, glm::vec3 direction, glm::mat4 light_projection);

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

    struct BonesShader{
        static constexpr int MAX_BONES{200};
        unsigned int shader_program;
        unsigned int u_model, u_view, u_proj;
        unsigned int u_bones[200];
        void uniform(const glm::mat4& view, const glm::mat4& proj)const;
        void bones_uniform(const glm::mat4& bone_transform, unsigned int location)const;
        void bind()const;
        BonesShader();
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

namespace FBO{
    class ShadowmapFBO{
    public:
        unsigned int m_fbo;
        Component::Texture m_texture;
        const int m_height, m_width;
        Shader::ShadowmapShader m_shader;
        // need to set right viewport after
        void write_bind()const{
            glEnable(GL_DEPTH_TEST);
            glViewport(0, 0, m_width, m_height);
            glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
            glClear(GL_DEPTH_BUFFER_BIT);
        }
        void read_bind(){

        }
    public:
        ShadowmapFBO();
        ~ShadowmapFBO(){}
    };

    class ShadowMapSingleton{
        public:
        Shader::ShadowmapShader shadowmap_shader;
        Shader::ShadowmapDebugShader debug_shader;
        ShadowmapFBO fbo;
        //FBO::ShadowMap shadowmap_fbo;
        static ShadowMapSingleton& get(){
            static ShadowMapSingleton instance;
            return instance;
        }
        void update(glm::mat4 light_projection);

        void bind_for_gui_render(){
            //debug_shader.bind();
            //shadowmap_fbo.read_bind(GL_TEXTURE0);
            debug_shader.update(0.1f, 50.f, fbo.m_texture.id);

        }

    public:
        ShadowMapSingleton();
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

namespace Light{
    struct PerspectiveLight{
        float m_fov;
        float m_light_len;
        glm::vec3 m_front;
        glm::vec3 m_position;
        glm::mat4 m_projection;
        glm::mat4 m_view;
        glm::vec3 m_col;

        std::tuple<float, float, glm::vec3, glm::vec3, glm::vec3>
            get_uniform_variables_radfov_len_front_pos_col()
        {
            return std::make_tuple(Math::deg_to_rad(m_fov), m_light_len, m_front, m_position, m_col);
        }

        void update(glm::vec3 front, glm::vec3 pos){
            m_front = front;
            m_position = pos;
            m_projection = glm::perspective(m_fov, 1.f, 1.f, 20.f);
            m_view = glm::lookAt (m_position, m_front + m_position, glm::vec3(0, 1, 0));
        }

        glm::mat4 get_light_matrix(){
            return m_projection * m_view;
        }

        PerspectiveLight(float fov, glm::vec3 position, glm::vec3 front):
            m_fov(fov), m_position(position), m_front(front), m_light_len(20.f), m_col(1, 1, 1)
        {
            m_front = front;
            m_projection = glm::perspective(m_fov, 1.f, 1.f, 20.f);
            m_view = glm::lookAt (m_position, m_front + m_position, glm::vec3(0, 1, 0));
        }

    };
}

#endif // UTILS_H_
