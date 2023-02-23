#ifndef SHADERS_H_
#define SHADERS_H_
 
namespace Shaders {
	inline const char* background_fragment_shader_1 =
		"#version 460 core\n"
		"out vec4 frag_color;\n"
		"uniform sampler2D t_;\n"
		"in vec3 color;\n"
		"in vec2 v_tex;\n"
		"void main() {\n"
		"    frag_color = texture(t_, v_tex);\n"
		"}\n\0";

	inline const char* background_vertex_shader_1 =
		"#version 460 core\n"
		"layout (location = 0) in vec3 apos;\n"
		"layout (location = 1) in vec3 acolor;\n"
		"layout (location = 2) in vec2 atex;\n"
		"out vec2 v_tex;\n"
		"out vec3 color;\n"
		"void main() {\n"
		"	 color = acolor;"
		"    gl_Position = vec4(apos, 1.0);\n"
		"    v_tex = vec2(atex.x, atex.y);\n"
		"}\n\0";
	inline const char* fragment_shader_2 =
		"#version 460 core\n"
		"out vec4 frag_color;\n"
		"uniform sampler2D tex;\n"
		"in vec2 texCoord;\n"
		"void main() {\n"
		"	frag_color = texture(tex, texCoord);\n"
		"}\n\0";
	inline const char* vertex_shader_2 =
		"#version 460 core\n"
		"layout (location = 0) in vec3 apos;\n"
		"layout (location = 1) in vec2 atex;\n"
		"uniform mat4 view;\n"
		"uniform mat4 proj;\n"
		"uniform mat4 model;\n"
		"out vec2 texCoord;\n"
		"void main() {\n"
		"   gl_Position = proj * view * model * vec4(apos.x, apos.y, apos.z, 1.0);\n"
		"   texCoord = atex;\n"
		"}\n\0";

	inline const char* fragment_shader_color_select =
		"#version 460 core\n"
		"out vec4 frag_color;\n"
        "uniform vec4 color;\n"
		"void main() {\n"
		"	frag_color = color;\n"
		"}\n\0";
	inline const char* vertex_shader_color_select =
		"#version 460 core\n"
		"layout (location = 0) in vec3 apos;\n"
		"uniform mat4 view;\n"
		"uniform mat4 proj;\n"
		"uniform mat4 model;\n"
		"void main() {\n"
		"   gl_Position = proj * view * model * vec4(apos.x, apos.y, apos.z, 1.0);\n"
		"}\n\0";
	inline const char* fragment_shader_bullet1 =
		"#version 460 core\n"
		"out vec4 frag_color;\n"
		"uniform sampler2D tex;\n"
		"in vec2 texCoord;\n"
		"in vec3 nor;\n"
		"void main() {\n"
		"   vec3 normal = nor * 0.1;\n"
		"   vec3 tex1 = texture(tex, texCoord).xyz;\n"
		"	frag_color = vec4(tex1, 1.0);\n"
		"}\n\0";

	inline const char* vertex_shader_bullet1 =
		"#version 460 core\n"
		"layout (location = 0) in vec3 apos;\n"
		"layout (location = 1) in vec3 anor;\n"
		"layout (location = 2) in vec2 atex;\n"
		"uniform mat4 view;\n"
		"uniform mat4 proj;\n"
		"uniform mat4 model;\n"
		"out vec2 texCoord;\n"
		"out vec3 nor;\n"
		"void main() {\n"
		"   gl_Position = proj * view * model * vec4(apos.x, apos.y, apos.z, 1.0);\n"
		"   texCoord = atex;\n"
		"   nor = anor;\n"
		"}\n\0";

	inline const char* fragment_shader_bullet_ =
	"#version 460 core\n"
	"out vec4 frag_color;\n"
	"uniform sampler2D tex;\n"
	"in vec2 texCoord;\n"
	"in vec3 frag_pos;\n"
	"in vec3 norm;\n"

	"uniform vec3 diffuse_pos;\n"
	"uniform vec3 diffuse_dir;\n"
	"uniform vec3 diffuse_col;\n"

	"void main() {\n"
	"   float ambient_str = 0.1;\n"
        "   vec3 ambient_light_color = vec3(1.0, 1.0, 1.0);\n"
        "   vec3 ambient = ambient_light_color * ambient_str;\n"
        "   vec3 light_norm = normalize(norm);\n"
        "   vec3 light_dir = normalize(diffuse_pos - frag_pos);\n"
        "   float diff = max(dot(norm, light_dir), 0.0);\n"
        "   vec3 diffuse = diff * diffuse_col;\n"
        "   vec3 light = (ambient + diffuse);\n"
        "   vec3 tex1 = texture(tex, texCoord).xyz ;\n"
        "   frag_color = vec4(tex1 * diffuse, 1.0);\n"
	"}\n\0";

	inline const char* vertex_shader_bullet_ =
	"#version 460 core\n"
	"layout (location = 0) in vec3 apos;\n"
	"layout (location = 1) in vec3 anor;\n"
	"layout (location = 2) in vec2 atex;\n"
	"uniform mat4 view;\n"
	"uniform mat4 proj;\n"
	"uniform mat4 model;\n"
	"out vec2 texCoord;\n"
        "out vec3 frag_pos;\n"
        "out vec3 norm;\n"
	"void main() {\n"
        "   frag_pos = vec3(model * vec4(apos, 1.0));\n"
        "   norm = anor;\n"
	"   texCoord = atex;\n"
	"   gl_Position = proj * view * model * vec4(apos.x, apos.y, apos.z, 1.0);\n"
	"}\n\0";

	inline const char* fragment_shader_bullet =
	"#version 460 core\n"
	"out vec4 frag_color;\n"
	"uniform sampler2D tex;\n"
	"in vec2 texCoord;\n"
        "in vec3 frag_pos;\n"
        "in vec3 norm;\n"

        "uniform vec3 diffuse_pos;\n"
        "uniform vec3 diffuse_dir;\n"
        "uniform vec3 diffuse_col;\n"

	"   struct FlashLight{\n"
	"      vec3 position;\n"
	"      vec3 direction;\n"
	"      float cutoff;\n"
	"    };\n"

	"uniform FlashLight flashlight;\n"
	"void main() {\n"
	"   float ambient_str = 0.1;\n"
        "   vec3 ambient_light_color = vec3(1.0, 1.0, 1.0);\n"
        "   vec3 ambient = ambient_light_color * ambient_str;\n"
	"   vec3 flashlightout = vec4(ambient, 1.0);\n"
	"   float theta = dot((normalize(flashlight.position - frag_pos)), normalize(-flashlight.direction));\n"
	"   if(theta > flashlight.cutoff){\n"

        "   vec3 light_norm = normalize(norm);\n"
        "   vec3 light_dir = normalize(diffuse_pos - frag_pos);\n"
        "   float diff = max(dot(norm, light_dir), 0.0);\n"
        "   vec3 diffuse = diff * diffuse_col;\n"
        "   vec3 light = (ambient + diffuse);\n"

        "   vec3 tex1 = texture(tex, texCoord).xyz ;\n"
	//"   flashlight = vec4(tex1 * light, 1.0);\n"
	"   flashlightout += vec4(tex1 * diffuse, 1.0);\n"
	"}\n"
        //"   frag_color = vec4(tex1 * light, 1.0);\n"
        "   frag_color = flashlight;\n"
	"}\n\0";

	inline const char* vertex_shader_bullet =
	"#version 460 core\n"
	"layout (location = 0) in vec3 apos;\n"
	"layout (location = 1) in vec3 anor;\n"
	"layout (location = 2) in vec2 atex;\n"
	"uniform mat4 view;\n"
	"uniform mat4 proj;\n"
	"uniform mat4 model;\n"
	"out vec2 texCoord;\n"
        "out vec3 frag_pos;\n"
        "out vec3 norm;\n"
	"void main() {\n"
        "   frag_pos = vec3(model * vec4(apos, 1.0));\n"
        "   norm = anor;\n"
	"   texCoord = atex;\n"
	"   gl_Position = proj * view * model * vec4(apos.x, apos.y, apos.z, 1.0);\n"
	"}\n\0";

	inline const char* fragment_shader_selected =
		"#version 460 core\n"
		"out vec4 frag_color;\n"
		"uniform sampler2D tex;\n"
		"in vec4 highlight;"
		"in vec2 texCoord;\n"
		"void main() {\n"
		"	frag_color = texture(tex, texCoord);\n"
		"}\n\0";


	inline const char* vertex_shader_selected =
		"#version 460 core\n"
		"layout (location = 0) in vec3 apos;\n"
		"layout (location = 1) in vec2 atex;\n"
		"uniform mat4 view;\n"
		"uniform mat4 proj;\n"
		"uniform mat4 model;\n"
		"uniform vec4 selected;\n"
		"out vec2 texCoord;\n"
		"out vec4 highlight;\n"
		"void main() {\n"
		"   gl_Position = proj * view * model * vec4(apos.x, apos.y, apos.z, 1.0);\n"
		"	highlight = selected;"
		"   texCoord = atex;\n"
		"}\n\0";


	inline const char* fragment_shader_1_default =
		"#version 460 core\n"
		"out vec4 frag_color;\n"
		"uniform vec3 color;\n"
		"in vec4 vertex_color;\n"
		"void main() {\n"
		"	frag_color = vertex_color;\n"
		"}\n\0";
	inline const char* fragment_shader_1_color =
		"#version 460 core\n"
		"out vec4 frag_color;\n"
		"uniform vec3 color;\n"
		"in vec4 vertex_color;\n"
		"void main() {\n"
		"	frag_color =vec4(color, 1.0);\n"
		"}\n\0";
	inline const char* vertex_shader_1 =
		"#version 460 core\n"
		"layout (location = 0) in vec3 apos;\n"
		"uniform mat4 view;\n"
		"uniform mat4 proj;\n"
		"uniform mat4 model;\n"
		"out vec4 vertex_color;\n"
		"void main() {\n"
		"   gl_Position =  proj * view * model * vec4(apos.x, apos.y, apos.z, 1.0);\n"
		"   vertex_color = vec4(1.0, 0.0, 0.0, 1.0);\n"
		"}\n\0";
	inline const char* vertexShaderSource_2 = "#version 460 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"uniform mat4 view;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = view * vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	inline const char* fragmentShaderSource_2 = "#version 460 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";

	inline const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main()\n"
		"{\n"
		"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";
	inline const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main()\n"
		"{\n"
		"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\n\0";
}

#endif // SHADERS_H_
       
