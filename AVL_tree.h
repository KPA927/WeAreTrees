#include <iostream>
#include "template.h"

template<typename T> class AVL;

template <typename T> class avl_node{
    T value;
    uint64_t height = 1;
    avl_node<T>* left = nullptr;
    avl_node<T>* right = nullptr;

    avl_node(T val){
        value = val;
    }
    ~avl_node(){
        delete left;
        delete right;
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

template<typename  T> class AVL:Balanced_tree<T>{
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

    avl_node<T>* erase(avl_node<T>* node, T value)
    {
        if (!node)
            return node;

        if (value < node->value)
            node->left = erase(node->left, value);

        else if (value > node->value)
            node->right = erase(node->right, value);

        else {
            //std::cout<<node->value<<'\n';
            size--;
            if (!node->right)
                return node->left;

            node = remove_rmin(node);
        }
        return balance(node);
    }

    T get_min(avl_node<T>* curr_node)
    {
        if (curr_node->left == nullptr)
            return curr_node->value;
        return get_min(curr_node->left);
    }

    avl_node<T>* remove_rmin(avl_node<T>* curr_node)
    {
        avl_node<T>*parent = curr_node, *root = curr_node;
        curr_node = parent->right;
        while (curr_node->left) {
            parent = curr_node;
            curr_node = curr_node->left;
        }

        root->value = curr_node->value;

        if (curr_node == root->right)
            root->right = root->right->right;
        else
            parent->left = curr_node->right;

        delete (curr_node);

        return root;
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
            if (node->right->diff() == 1){
                node->right = rot_right(node->right); //большой левый поворот
                //std::cout <<" big";

            }
            //std::cout <<"left "<<node->value <<'\n';

            return rot_left(node);
        }

        if (node->diff() == 2) {
            if (node->left->diff() == -1){
                node->left = rot_left(node->left); //большой правый поворот
                //std::cout <<" big";
            }
            //std::cout <<"right "<<node->value <<'\n';
            return rot_right(node);
        }
        return node;
    }

    void print(avl_node<T>* node)
    {
        if (!node)
            return;
        print(node->left);
        std::cout << node->value << ' ';
        print(node->right);
    }

    void deletetree(avl_node<T>* node){
        if(!node->left){
            deletetree(node->left);
        }
        if(!node->right){
            deletetree(node->right);
        }
        delete node;
    }


public:

    ~AVL<T>(){
        deletetree(head);
    }

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

    void print()
    {
        print(head);
        std::cout << "\n";
    }

};