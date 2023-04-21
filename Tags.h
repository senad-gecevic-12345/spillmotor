#ifndef TAGS_H_
#define TAGS_H_

namespace Tags {
	struct TriangleEnemy{};
	struct Ally {};
	struct TriangleProjectile {};
	struct Player {};
	struct Triangle {};
	struct Bullet {};
	struct ManualRender {};
	struct Model3D{};
	struct TexModel3D {};
	struct Sphere {};
	struct Physics {};
	struct BulletPhysics {};
	struct RepresentationRender {};
	struct RepresentationRenderNewMeshLoader {};
    struct NormalMap{};
}

namespace RenderTags {
    struct _3D_OBJECT{};
	struct _3D_TEXTURE{};
	struct _2D_COLOR{};
	struct _2D_DEFAULT{};
    struct BonesRendering{};
}

#endif // TAGS_H_
