#ifndef UPDATE_H
#define UPDATE_H
template <typename T>
class Balanced_tree{
public:
    virtual void insert(T x) = 0;
    virtual bool find(T x) = 0;
    virtual void erase(T x) = 0;
};

#endif