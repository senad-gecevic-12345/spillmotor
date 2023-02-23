#ifndef GUIGLOBALVARIABLESSINGLETON_H_
#define GUIGLOBALVARIABLESSINGLETON_H_

#include <vector>

namespace MeshLoaderNew{
    struct MeshGlobalVariable;
};


template<typename T>
class GuiGlobalVariablesSingleton{
public:
    std::vector<T> assets;
    
    static GuiGlobalVariablesSingleton<T>& get(){
        static GuiGlobalVariablesSingleton instance;
        return instance;
    }

    friend class ComponentSelectionBase;
    friend class NewGui;
};

class MeshSingleton{
public:
    std::vector<MeshLoaderNew::MeshGlobalVariable> meshes;
    static MeshSingleton& get(){
        static MeshSingleton instance;
        return instance;
    }
};

#endif // GUIGLOBALVARIABLESSINGLETON_H_

