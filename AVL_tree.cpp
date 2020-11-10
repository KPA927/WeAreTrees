#include <iostream>

template<typename T> class AVL;

template <typename T> class avl_node{
    T value;
    uint64_t height = 1;
    avl_node<T>* left = nullptr;
    avl_node<T>* right = nullptr;

    avl_node(T val){
        value = val;
    }
    int diff(){
        int lheight = 0, rheight = 0;
        if(left){
            lheight = left->height;
        }
        if(right){
            rheight = right->height;
        }
        return lheight - rheight;
    }
    friend  class AVL<T>;
};

template<typename  T> class AVL{
private:
    int size = 0;
    avl_node<T>* head = nullptr;

    avl_node<T>* insert(avl_node<T>* node, T value){
        if(node == nullptr)
        {
            size++;
            return new avl_node<T> (value);
        }
        if(value < node->value)
        {
            node->left = insert(node->left, value);
        }
        else
        {
            node->right = insert(node->right, value);
        }
        return balance(node);
    }
    avl_node<T>* min_node(avl_node<T>* current)
    {
        if(current->left == nullptr){
            return current;
        }else{
            return min_node(current->left);
        }
    }

    avl_node<T>* remove_min(avl_node<T>* current)
    {
        if(current->left == nullptr)
            return current->right;
        current->left = remove_min(current->left);
        return balance(current);
    }

    avl_node<T>* erase(avl_node<T>* node, T value)
    {
        if(node == nullptr)
            return  node;


        if(value > node->value)
            node->right = erase(node->right, value);
        else if(value < node->value)
            node->left = erase(node->left, value);
        else
        {
            size--;
            if(node->right == nullptr)
            {
                return node->left;
            }
            avl_node<T>* min = min_node(node->right);
            min->right = remove_min(node->right);
            min->left = node->left;
            delete node;
            return balance(min);
        }
        return balance(node);
    }

    void fix(avl_node<T>* node)
    {
        uint64_t lheight = 0, rheight = 0;
        if(node->left)
            lheight = node->left->height;
        if(node->right)
            rheight = node->right->height;
        node->height = std::max(lheight, rheight) +1;
    }

    avl_node<T>* rot_left(avl_node<T>* node)
    {
        avl_node<T>* root = node->right;
        node->right = root->left;
        root->left = node;

        fix(node);
        fix(root);
        return root;
    }

    avl_node<T>* rot_right(avl_node<T>* node)
    {
        avl_node<T>* root = node->left;
        node->left = root->right;
        root->right = node;

        fix(node);
        fix(root);
        return root;
    }


    avl_node<T>* balance(avl_node<T>* node){
        fix(node);

        if (node->diff() == -2) {
            if (node->right->diff() == 1)
                node->right = rot_right(node->right); //большой левый поворот

            return rot_left(node);
        }

        if (node->diff() == 2) {
            if (node->left->diff() == -1)
                node->left = rot_left(node->left); //большой правый поворот

            return rot_right(node);
        }
        return node;
    }



public:
    void insert(T value)
    {
        if(!find(value))
            head = insert(head, value);
    }
    void erase(T value)
    {
        head = erase(head, value);
    }

    bool find(T value)
    {
        avl_node<T>* current = head;
        while(current and current->value != value){
            if(value < current->value)
                current = current->left;
            else
                current = current->right;

        }
        if(current)
            return true;
        else
            return false;

    }

    bool empty()
    {
        if(size==0)
            return true;
        else
            return false;
    }

};