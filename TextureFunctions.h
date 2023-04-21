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
    void bind(Component::Texture& texture);

	bool load_texture_jpg(std::string texture_file_path, Component::Texture& texture);
	bool load_texture_png(const std::string& texture_file_path, Component::Texture& texture);

    bool load_texture_embedded(Component::Texture& texture, unsigned int size, void* data);

    void create_depth_texture(Component::Texture& texture, unsigned int width, unsigned int height);
}

#endif // TEXTUREFUNCTIONS_H_

