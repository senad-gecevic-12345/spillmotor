#include "RenderFunctions.h"
#include <glad/glad.h>



namespace RenderFunctions {
	void texture_model_3d_create_render(Component::Render& render, Component::Mesh& mesh) {
		glGenVertexArrays(1, &render.VAO);
		glGenBuffers(1, &render.VBO);
		glGenBuffers(1, &render.EBO);

		glBindVertexArray(render.VAO);

		glBindBuffer(GL_ARRAY_BUFFER, render.VBO);

		glBufferData(GL_ARRAY_BUFFER, (mesh.vertices.size() * sizeof(glm::vec3)) + ((mesh.normals.size() * sizeof(glm::vec3)) + (mesh.uv.size() * sizeof(glm::vec2))), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, mesh.vertices.size() * sizeof(glm::vec3), &mesh.vertices[0]);
		glBufferSubData(GL_ARRAY_BUFFER,    mesh.vertices.size() * sizeof(glm::vec3), mesh.normals.size() * sizeof(glm::vec3), &mesh.normals[0]);
		glBufferSubData(GL_ARRAY_BUFFER,    (mesh.vertices.size() * sizeof(glm::vec3)) + (mesh.normals.size() * sizeof(glm::vec3)), mesh.uv.size() * sizeof(glm::vec2), &mesh.uv[0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo.size() * sizeof(int), &mesh.ebo[0], GL_STATIC_DRAW);

		render.triangle_count = mesh.triangle_count;

		// problems maybe from reuse of normals?
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)((mesh.vertices.size() * sizeof(glm::vec3))));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)((mesh.vertices.size() * sizeof(glm::vec3) + (mesh.normals.size() * sizeof(glm::vec3)))));

	}


}
