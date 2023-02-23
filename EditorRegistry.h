#ifndef EditorRegistryAOEUTNH_H_
#define EditorRegistryAOEUTNH_H_

#include <entt/entity/registry.hpp>

class EditorRegistry{
public:
    entt::registry m_registry;

    static EditorRegistry& get(){
        static EditorRegistry instance;
        return instance;
    }
    EditorRegistry& operator=(const EditorRegistry& )=delete;
    EditorRegistry(const EditorRegistry& )=delete;

    EditorRegistry()=default;
    ~EditorRegistry()=default;
};


#endif // EditorRegistry_H_
