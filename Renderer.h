#ifndef RENDERER_H_
#define RENDERER_H_

#include "entt/entt.hpp"
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Renderer{

void bullet_temp_renderer(entt::registry* registry, unsigned int shader_program, unsigned int model_loc);
void bullet_temp_renderer_single(entt::registry* registry, unsigned int shader_program, unsigned int model_loc);
void representation_renderer(entt::registry* registry, unsigned int shader_program, unsigned int model_loc);
void representation_renderer_new_mesh_render(entt::registry* registry, unsigned int shader_program, unsigned int model_loc);

}

namespace BULLET_SHADER_LIGHTS{ 
    inline std::tuple<unsigned int, unsigned int, unsigned int> query_diffuse_uniforms_pos_dir_col(unsigned int shader_program){
        return std::make_tuple(
                glGetUniformLocation(shader_program, "diffuse_pos"),
                glGetUniformLocation(shader_program, "diffuse_dir"),
                glGetUniformLocation(shader_program, "diffuse_col"));
    }

    inline void render_diffuse_light(glm::vec3 pos, glm::vec3 dir, glm::vec3 col, float str, unsigned int diffuse_position_loc, unsigned int diffuse_direction_loc, unsigned int diffuse_color_loc){
        glUniform3f(diffuse_position_loc, pos.x, pos.y, pos.z);
        glUniform3f(diffuse_direction_loc, dir.x, dir.y, dir.z);
        glUniform3f(diffuse_color_loc, col.x, col.y, col.z);
    }

    struct FlashLight{
        glm::vec3 position;
        glm::vec3 direction;
        float cutoff;
        FlashLight(glm::vec3 position, glm::vec3 direction, float cutoff):position(position),direction(direction),cutoff(cutoff){}
    };




};

#endif // RENDERER_H_

