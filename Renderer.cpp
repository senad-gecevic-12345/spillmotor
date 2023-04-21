#include "Renderer.h"
#include "entt/entt.hpp"
#include "glm/glm.hpp"
#include <glad/glad.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "Tags.h"
#include "Math.h"
#include "Components.h"
#include "MeshFunctions2.h"
#include "Utils.h"
#include "World.h"

namespace Renderer{

    void bullet_temp_renderer(entt::registry* registry, unsigned int shader_program, unsigned int model_loc){
		auto group = registry->group<>(entt::get<Tags::BulletPhysics, Component::Position, MeshLoaderNew::MeshRenderId, Component::OpenGLRotation, Component::Scale>, entt::exclude<Tags::NormalMap, RenderTags::BonesRendering>);
		for (auto e : group) {
			auto [pos, render,  rotate, scale] = 
				registry->get<Component::Position, MeshLoaderNew::MeshRenderId, Component::OpenGLRotation, Component::Scale>(e);

            glm::mat4 model(1);
			model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
			model = model * rotate.rotation;
			model = glm::scale(model, scale.scale);

			glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(render.VAO);
			for(int i = 0; i < render.meshes.size(); ++i){
				auto mat_index = render.meshes[i].material_index;

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, render.materials[mat_index].diffuse.id);
				glUniform1i(glGetUniformLocation(shader_program, "tex"), 0);
				glDrawElementsBaseVertex(
					GL_TRIANGLES,
					render.meshes[i].no_indices,
					GL_UNSIGNED_INT,
					(void*)(sizeof(unsigned int) * render.meshes[i].base_index),
					render.meshes[i].base_vertex);
				glBindTexture(GL_TEXTURE_2D, 0);

			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
    }

	void representation_renderer(entt::registry* registry, unsigned int shader_program, unsigned int model_loc){
		auto group = registry->group<>(entt::get<Tags::RepresentationRender, Component::Position, Component::Render, Component::Mesh, Component::Texture, Component::Rotation, Component::Scale>, entt::exclude<Tags::NormalMap, RenderTags::BonesRendering>);
		for (auto e : group) {
			auto [pos, render,  texture, rotate, scale] =
				registry->get<Component::Position, Component::Render, Component::Texture, Component::Rotation, Component::Scale>(e);
			glm::mat4 rotate_matrix(1.f);
			rotate_matrix = glm::rotate(rotate_matrix, Math::deg_to_rad(rotate.y), glm::vec3(0, 1, 0));
			rotate_matrix = glm::rotate(rotate_matrix, Math::deg_to_rad(rotate.x), glm::vec3(1, 0, 0));
			rotate_matrix = glm::rotate(rotate_matrix, Math::deg_to_rad(rotate.z), glm::vec3(0, 0, 1));
			glm::mat4 model(1.f);
			model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
			model = model * rotate_matrix;
			model = glm::scale(model, scale.scale);
			glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(render.VAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture.id);
			glUniform1i(glGetUniformLocation(shader_program, "tex"), 0);
			glDrawElements(GL_TRIANGLES, render.triangle_count, GL_UNSIGNED_INT, 0);
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	void representation_renderer_new_mesh_render(entt::registry* registry, unsigned int shader_program, unsigned int model_loc){
		auto group = registry->group<>(entt::get<Tags::RepresentationRenderNewMeshLoader, Component::Position, MeshLoaderNew::MeshRenderId, Component::Rotation, Component::Scale>, entt::exclude<Tags::NormalMap, RenderTags::BonesRendering>);
		for(auto e : group){
			auto [pos, render, rotate, scale] =
				registry->get<Component::Position, MeshLoaderNew::MeshRenderId, Component::Rotation, Component::Scale>(e);
			glm::mat4 rotate_matrix(1.f);
			rotate_matrix = glm::rotate(rotate_matrix, Math::deg_to_rad(rotate.y), glm::vec3(0, 1, 0));
			rotate_matrix = glm::rotate(rotate_matrix, Math::deg_to_rad(rotate.x), glm::vec3(1, 0, 0));
			rotate_matrix = glm::rotate(rotate_matrix, Math::deg_to_rad(rotate.z), glm::vec3(0, 0, 1));
			glm::mat4 model(1.f);

			model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
			model = model * rotate_matrix;
			model = glm::scale(model, scale.scale);
			glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(render.VAO);
			for(int i = 0; i < render.meshes.size(); ++i){
				auto mat_index = render.meshes[i].material_index;

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, render.materials[mat_index].diffuse.id);
				glUniform1i(glGetUniformLocation(shader_program, "tex"), 0);

                glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, render.materials[mat_index].normal.id);
				glUniform1i(glGetUniformLocation(shader_program, "normalmap"), 1);

				glDrawElementsBaseVertex(
					GL_TRIANGLES,
					render.meshes[i].no_indices,
					GL_UNSIGNED_INT,
					(void*)(sizeof(unsigned int) * render.meshes[i].base_index),
					render.meshes[i].base_vertex);
				glBindTexture(GL_TEXTURE_2D, 0);

			}
		}
	}



    // TODO: the light is not interacting with that memes. probably because normal map or something
	void representation_renderer_normalmap(entt::registry* registry, unsigned int shader_program, unsigned int model_loc){
		auto group = registry->group<>(entt::get<Tags::NormalMap, Tags::RepresentationRenderNewMeshLoader, Component::Position, MeshLoaderNew::MeshRenderId, Component::Rotation, Component::Scale>, entt::exclude<RenderTags::BonesRendering>);
		for(auto e : group){
			auto [pos, render, rotate, scale] =
				registry->get<Component::Position, MeshLoaderNew::MeshRenderId, Component::Rotation, Component::Scale>(e);
			glm::mat4 rotate_matrix(1.f);
			rotate_matrix = glm::rotate(rotate_matrix, Math::deg_to_rad(rotate.y), glm::vec3(0, 1, 0));
			rotate_matrix = glm::rotate(rotate_matrix, Math::deg_to_rad(rotate.x), glm::vec3(1, 0, 0));
			rotate_matrix = glm::rotate(rotate_matrix, Math::deg_to_rad(rotate.z), glm::vec3(0, 0, 1));
			glm::mat4 model(1.f);

			model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
			model = model * rotate_matrix;
			model = glm::scale(model, scale.scale);
			glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(render.VAO);
			for(int i = 0; i < render.meshes.size(); ++i){
				auto mat_index = render.meshes[i].material_index;

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, render.materials[mat_index].diffuse.id);
				glUniform1i(glGetUniformLocation(shader_program, "tex"), 0);

                glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, render.materials[mat_index].normal.id);
				glUniform1i(glGetUniformLocation(shader_program, "normalmap"), 1);

				// maybe the id is wrong
				glDrawElementsBaseVertex(
					GL_TRIANGLES,
					render.meshes[i].no_indices,
					GL_UNSIGNED_INT,
					(void*)(sizeof(unsigned int) * render.meshes[i].base_index),
					render.meshes[i].base_vertex);
				glBindTexture(GL_TEXTURE_2D, 0);

			}
		}
	}

	void representation_renderer_normalmap_bullet_fbo(entt::registry* registry, unsigned int shader_program, unsigned int model_loc){
		auto group = registry->group<>(entt::get<Tags::NormalMap, Tags::RepresentationRenderNewMeshLoader, Component::Position, MeshLoaderNew::MeshRenderId, Component::OpenGLRotation, Component::Scale>, entt::exclude<RenderTags::BonesRendering>);
		for(auto e : group){
			auto [pos, render, rotate, scale] =
				registry->get<Component::Position, MeshLoaderNew::MeshRenderId, Component::OpenGLRotation, Component::Scale>(e);

            glm::mat4 model(1);
			model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));

			model = model * rotate.rotation;
			model = glm::scale(model, scale.scale);

			glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(render.VAO);
			for(int i = 0; i < render.meshes.size(); ++i){
				auto mat_index = render.meshes[i].material_index;

				glDrawElementsBaseVertex(
					GL_TRIANGLES,
					render.meshes[i].no_indices,
					GL_UNSIGNED_INT,
					(void*)(sizeof(unsigned int) * render.meshes[i].base_index),
					render.meshes[i].base_vertex);
				glBindTexture(GL_TEXTURE_2D, 0);

			}
		}
	}

    // TODO: add baseballs without this to bullet and see if render
	void representation_renderer_normalmap_bullet(entt::registry* registry, unsigned int shader_program, unsigned int model_loc){
		auto group = registry->group<>(entt::get<Tags::NormalMap, Tags::RepresentationRenderNewMeshLoader, Component::Position, MeshLoaderNew::MeshRenderId, Component::OpenGLRotation, Component::Scale>, entt::exclude<RenderTags::BonesRendering>);
		for(auto e : group){
			auto [pos, render, rotate, scale] =
				registry->get<Component::Position, MeshLoaderNew::MeshRenderId, Component::OpenGLRotation, Component::Scale>(e);

            glm::mat4 model(1);
			model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
            auto rotation_matrix = glm::mat4(1);
            
			model = model * rotate.rotation;
			model = glm::scale(model, scale.scale);

			glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

			glBindVertexArray(render.VAO);
			for(int i = 0; i < render.meshes.size(); ++i){
				auto mat_index = render.meshes[i].material_index;

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, render.materials[mat_index].diffuse.id);
				glUniform1i(glGetUniformLocation(shader_program, "tex"), 0);

                glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, render.materials[mat_index].normal.id);
				glUniform1i(glGetUniformLocation(shader_program, "normalmap"), 1);

                assert(i < render.meshes.size());
				glDrawElementsBaseVertex(
					GL_TRIANGLES,
					render.meshes[i].no_indices,
					GL_UNSIGNED_INT,
					(void*)(sizeof(unsigned int) * render.meshes[i].base_index),
					render.meshes[i].base_vertex);
				glBindTexture(GL_TEXTURE_2D, 0);

			}
		}
	}

    void bones_render(entt::registry* registry, const SHADERS::BonesShader& shader){
        auto group = registry->group<>(entt::get<RenderTags::BonesRendering, Component::Position, MeshLoaderNew::MeshRenderId, Component::OpenGLRotation, Component::Scale, Component::AnimationController>);
        for(auto e : group){
            auto [pos, render, rotate, scale, anim_loc] =
                    registry->get<Component::Position, MeshLoaderNew::MeshRenderId, Component::OpenGLRotation, Component::Scale, Component::AnimationController>(e);

            glm::mat4 model(1);
            model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));

            model = model * rotate.rotation;
            model = glm::scale(model, scale.scale);
            glUniformMatrix4fv(shader.u_model, 1, GL_FALSE, glm::value_ptr(model));

            auto* anim = MeshLoaderNew::get_bone_transform(World::Clock::get().get_time_since_start(), anim_loc);
            if(anim == nullptr)
                continue;

            for(int i = 0; i < anim->transforms.size(); ++i){
                shader.bones_uniform(anim->transforms[i], i);
            }

            glBindVertexArray(render.VAO);
			for(int i = 0; i < render.meshes.size(); ++i){
				auto mat_index = render.meshes[i].material_index;

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, render.materials[mat_index].diffuse.id);
				glUniform1i(glGetUniformLocation(shader.shader_program, "diffuse"), 0);

				glDrawElementsBaseVertex(
					GL_TRIANGLES,
					render.meshes[i].no_indices,
					GL_UNSIGNED_INT,
					(void*)(sizeof(unsigned int) * render.meshes[i].base_index),
					render.meshes[i].base_vertex);
				glBindTexture(GL_TEXTURE_2D, 0);
			}
        }
    }
}
