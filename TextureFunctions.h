#ifndef TEXTUREFUNCTIONS_H_
#define TEXTUREFUNCTIONS_H_

#include <string>
#include "Components.h"

namespace SKYBOX{
    struct SkyBox{
        unsigned int texture_id;
		unsigned int VAO, VBO;
		void draw();
    };
    SkyBox create_skybox(const std::array<std::string, 6>& faces);
}

namespace TextureFunctions{
	//void bind_texture(Component::Texture& texture);
	//void unbind_texture(Component::Texture& texture);
    // inline or static maybe?
    void bind(Component::Texture& texture);
	bool load_texture_jpg(std::string texture_file_path, Component::Texture& texture);
	bool load_texture_png(const std::string& texture_file_path, Component::Texture& texture);
}

#endif // TEXTUREFUNCTIONS_H_

