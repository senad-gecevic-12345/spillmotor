#ifndef COLLISIONWRAPPERS_H_
#define COLLISIONWRAPPERS_H_

namespace CollisionWrappers{
    class CollisionShape{
    public:
        virtual void config()=0;
        virtual void reset()=0;
        virtual btCollisionShape* create()=0;
        CollisionShape(){}
    };

    class SphereCollisionShape : public CollisionShape{
    public:
        float radius{1};
        virtual void config()override{
            ImGui::InputScalar("radius",   ImGuiDataType_Float,  &radius);
        }
        virtual void reset()override{
            radius = 1;
        }
        virtual btCollisionShape* create()override{
            return new btSphereShape(btScalar(radius));
        }
        explicit SphereCollisionShape(float radius):radius(radius){}
        SphereCollisionShape(){}
    };
    class BoxCollisionShape : public CollisionShape{
    public:
        float x{1}, y{1}, z{1};
        virtual void config(){
            ImGui::InputFloat3("xyz",  &x);
        }
        virtual void reset()override{
            x = 1;
            y = 1;
            z = 1;
        }

        virtual btCollisionShape* create()override{
            return new btBoxShape(btVector3(btScalar(x), btScalar(y), btScalar(z)));
        }

        BoxCollisionShape(float x, float y, float z):x(x), y(y), z(z){}
        BoxCollisionShape(){}
    };
    class CapsuleCollisionShape : public CollisionShape{
    public:
        float radius{1};
        float height{1};
        virtual void config(){
            ImGui::InputScalar("radius",   ImGuiDataType_Float,  &radius);
            ImGui::InputScalar("height",   ImGuiDataType_Float,  &height);

        }
        virtual void reset()override{
            radius = 1;
            height = 1;
        }
        virtual btCollisionShape* create()override{
            return new btCapsuleShape(btScalar(radius), height);
        }

        CapsuleCollisionShape(float radius, float height):radius(radius), height(height){}
        CapsuleCollisionShape(){}
    };
    class TriangleMeshCollisionShape : public CollisionShape{
    public:
        std::string file_loc_name;
        void set_path(const std::string& str){
            file_loc_name = std::string(str);
        }
        virtual void config()override{

        }
        virtual void reset()override{
            file_loc_name = "";
        }
        virtual btCollisionShape* create()override{
            btTriangleMesh* mesh = new btTriangleMesh();
            MeshLoaderNew::load_create_triangle_mesh(file_loc_name, *mesh);

            return new btBvhTriangleMeshShape(mesh, true, true);
        }

        TriangleMeshCollisionShape(){}
    };
}


#endif // COLLISIONWRAPPERS_H_
