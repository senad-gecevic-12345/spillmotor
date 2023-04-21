#ifndef GUIHELPERS_H_
#define GUIHELPERS_H_

#include "Representation.h"

// TODO: delete this
// no selection data have just single template
template<typename T>
struct SelectionList{
    int selected_id{-1};
    std::vector<T> representations;

    std::size_t size(){return representations.size();}

    typename std::vector<T>::iterator begin(){return representations.begin();};
    typename std::vector<T>::iterator end(){return representations.end();};

    void add_representation(T data){representations.push_back(data);}
    bool has_selection(){return selected_id != -1;}
    void deselect(){selected_id = -1;}

    void toggle_selected(int i){
        if(i == selected_id)
            selected_id = -1;
        else 
            selected_id = i;
    }

    T& get_selection(){return representations[selected_id];}
    T& operator[](int i){return representations[i];}
};


// stupid just move it
template<typename T>
struct ReferenceSelectionList{
    int selected_id{-1};
    std::vector<T>* representations;

    std::size_t size(){return representations->size();}

    typename std::vector<T>::iterator begin(){return representations->begin();};
    typename std::vector<T>::iterator end(){return representations->end();};

    bool has_selection(){return selected_id != -1;}
    void deselect(){selected_id = -1;}

    void toggle_selected(int i){
        if(i == selected_id)
            selected_id = -1;
        else
            selected_id = i;
    }

    ReferenceSelectionList(std::vector<T>* ptr):representations(ptr){}

    T& get_selection(){return (*representations)[selected_id];}
    T& operator[](int i){return (*representations)[i];}
};

struct RepresentationSelectionList{
    int selected_id{-1}; 
    std::vector<RepresentationKey> representations;
    using RepType = std::vector<RepresentationKey>;
    std::size_t size(){return representations.size();}
    RepType::iterator begin(){
        return representations.begin();
    };
    RepType::iterator end(){
        return representations.end();
    };
    void add_representation(RepresentationKey key){
        representations.push_back(key);
    }
    void deselect(){
        selected_id = -1;
    }
    void throw_away(int i){
        assert(i < representations.size());
        assert(i > -1);
        std::swap(representations[i], representations[representations.size()-1]);
        representations.pop_back();
        selected_id = -1;
    }
    bool has_selection(){
        return selected_id != -1;
    }
    RepresentationKey& get_selection(){
        assert(selected_id > -1);
        assert(selected_id < representations.size());
        return representations[selected_id];
    }
    
    void toggle_selected(int i){
        if(i == selected_id)
            selected_id = -1;
        else 
            selected_id = i;
    }
    RepresentationKey& operator[](int i){
        return representations[i];
    }
};

template<typename Data>
struct RepresentationSelectionListWithAdditionalData{
    int selected_id{-1};
    std::vector<RepresentationKey> representations;
    std::vector<Data> additional_data;
    using RepType = std::vector<RepresentationKey>;
    std::size_t size(){return representations.size();}
    RepType::iterator begin(){
        return representations.begin();
    };
    RepType::iterator end(){
        return representations.end();
    };
    void add_representation(const RepresentationKey& key, const Data& data){
        representations.push_back(RepresentationKey(key));
        additional_data.push_back(Data(data));
    }
    void deselect(){
        selected_id = -1;
    }
    void throw_away(int i){
        assert(i < representations.size());
        assert(i > -1);
        int x = i;
        int y = representations.size()-1;
        std::swap(representations[x], representations[y]);
        std::swap(additional_data[x], additional_data[y]);

        representations.pop_back();
        additional_data.pop_back();

        selected_id = -1;
    }
    bool has_selection(){
        return selected_id != -1;
    }
    RepresentationKey& get_selection(){
        assert(selected_id > -1);
        assert(selected_id < representations.size());
        return representations[selected_id];
    }
    Data& get_selection_data(){
        return additional_data[selected_id];
    }

    void toggle_selected(int i){
        if(i == selected_id)
            selected_id = -1;
        else
            selected_id = i;
    }
    RepresentationKey& operator[](int i){
        return representations[i];
    }
};

#endif // GUIHELPERS_H_


