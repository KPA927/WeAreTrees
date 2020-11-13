#ifndef UPDATE_H
#define UPDATE_H
template <typename T>
class Balanced_tree{
public:
    virtual void insert(T x){};
    virtual bool find(T x){};
    virtual void erase(T x){};
};

#endif