
template<typename T>
class Vector{
    void* memory{nullptr};
    int counter{0};
pubilc:
    void allocate(std::size_t size){
        if(memory == nullptr){
            memory = malloc(size);
        }
    }
    void emplace_back(T&& t){
               
    }
    ~Vector(){
        delete memory;
    }
};

