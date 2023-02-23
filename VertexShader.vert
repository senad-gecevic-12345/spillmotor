#version 460 

uniform mat4 view;
uniform mat4 proj;
uniform mat4 model;

out vec4 vertex_color;

void main() {
   gl_Position = proj * view * model * vec4(1.0, 0.0, 0.0, 0.0);
   vertex_color = vec4(0.5, 0.0, 0.0, 1.0);
}
