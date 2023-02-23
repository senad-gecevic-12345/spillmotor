#include "Renderer.h"
#include "entt/entt.hpp"
#include "glm/glm.hpp"
#include <glad/glad.h>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "Tags.h"
#include "Math.h"
#include "Components.h"
#include "MeshFunctions2.h"
#include "TextureFunctions.h"


namespace Renderer{

    void bullet_temp_renderer(entt::registry* registry, unsigned int shader_program, unsigned int model_loc){
		auto group = registry->group<>(entt::get<Tags::BulletPhysics, Component::Position, Component::Render, Component::Mesh, Component::Texture, Component::Rotation, Component::Scale>, entt::exclude<Tags::RepresentationRenderNewMeshLoader>);
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

	void representation_renderer(entt::registry* registry, unsigned int shader_program, unsigned int model_loc){
		auto group = registry->group<>(entt::get<Tags::RepresentationRender, Component::Position, Component::Render, Component::Mesh, Component::Texture, Component::Rotation, Component::Scale>);
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
		auto group = registry->group<>(entt::get<Tags::RepresentationRenderNewMeshLoader, Component::Position, MeshLoaderNew::MeshRenderId, Component::Rotation, Component::Scale>);
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
