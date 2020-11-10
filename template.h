template <typename T>
class Balanced_tree(T){
public:
    virtual void insert(T x){};
    virtual bool find(T x){};
    virtual void erase(T x){};
}