#include <iostream>
#include <vector>
#include <random>
#include <set>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include <string>
#include <queue>

using std::cout;
using std::endl;



template <typename T> class Balanced_tree{
    void virtual insert(T x){};
    bool virtual find(T x){
        return true;
    };
    void virtual erase(T x){};
};

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

    avl_node<T>* balance(avl_node<T>* node)
    {
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



template <typename T>
class rb_node{
private:
    T key;
    std::string color;
    rb_node* left;
    rb_node* right;
    rb_node* parent;
public:
    rb_node(T key, std::string color, rb_node<T>* parent){
        this->key = key;
        this->parent = parent;
        if(color == "black" or color == "red") {
            this->color = color;
        }
        else{
            this->color = "black";
        }
        this->left = nullptr;
        this->right = nullptr;
    }
    void set_key(T key){
        this->key = key;
    }
    T get_key(){
        return this->key;
    }

    void set_color(std::string color){
        if(color == "black" or color == "red"){
            this->color = color;
        }
    }
    std::string get_color(){
        return this->color;
    }

    rb_node* get_left(){
        return this->left;
    }
    rb_node* get_right(){
        return this->right;
    }

    rb_node* get_parent(){
        return this->parent;
    }
    void set_parent(rb_node* parent){
        this->parent = parent;
    }
    void set_left(rb_node* node){
        this->left = node;
    }
    void set_right(rb_node* node){
        this->right = node;
    }

    ~rb_node(){
        delete left;
        delete right;
    }
};

template <typename T>
class raw_rb_tree:Balanced_tree<T>{
private:
    int tree_size;
    rb_node<T>* root;
    rb_node<T>* get_grandparent(rb_node<T>* cr_node){
        if(cr_node->get_parent() == nullptr or (cr_node->get_parent())->get_parent() == nullptr){
            return nullptr;
        }
        else{
            return (cr_node->get_parent())->get_parent();
        }
    }
    rb_node<T>* get_uncle(rb_node<T>* cr_node){
        if(get_grandparent(cr_node) == nullptr){
            return nullptr;
        }
        if(cr_node->get_parent() == get_grandparent(cr_node)->get_right()){
            return get_grandparent(cr_node)->get_left();
        }
        if(cr_node->get_parent() == get_grandparent(cr_node)->get_left()){
            return get_grandparent(cr_node)->get_right();
        }
        return nullptr;
    }
    void right_rotation(rb_node<T>* cr_node){
        rb_node<T>* tmp = cr_node->get_left();
        tmp->set_parent(cr_node->get_parent());
        if(cr_node->get_parent() != nullptr){
            if(cr_node->get_parent()->get_left() == cr_node){
                cr_node->get_parent()->set_left(tmp);
            }
            else{
                cr_node->get_parent()->set_right(tmp);
            }
        }
        cr_node->set_left(tmp->get_right());
        if(tmp->get_right() != nullptr){
            tmp->get_right()->set_parent(cr_node);
        }
        cr_node->set_parent(tmp);
        tmp->set_right(cr_node);
    }
    void left_rotation(rb_node<T>* cr_node){
        rb_node<T>* tmp = cr_node->get_right();
        tmp->set_parent(cr_node->get_parent());
        if(cr_node->get_parent() != nullptr){
            if(cr_node->get_parent()->get_right() == cr_node){
                cr_node->get_parent()->set_right(tmp);
            }
            else{
                cr_node->get_parent()->set_left(tmp);
            }
        }
        cr_node->set_right(tmp->get_left());
        if(tmp->get_left() != nullptr){
            tmp->get_left()->set_parent(cr_node);
        }
        cr_node->set_parent(tmp);
        tmp->set_left(cr_node);
    }
    void fix_insertion(rb_node<T>* new_node){
        rb_node<T>* tmp = new_node;
        while(tmp->get_parent() != nullptr and tmp->get_parent()->get_color() == "red"){
            rb_node<T>* uncle = get_uncle(tmp);
            if(uncle != nullptr and uncle->get_color() == "red"){
                uncle->set_color("black");
                tmp->get_parent()->set_color("black");
                tmp->get_parent()->get_parent()->set_color("red");
            }
            else{
                if(tmp == tmp->get_parent()->get_right() and tmp->get_parent() == get_grandparent(tmp)->get_left()){
                    left_rotation(tmp->get_parent());
                    tmp = tmp->get_left();
                }
                else if(tmp == tmp->get_parent()->get_left() and tmp->get_parent() == get_grandparent(tmp)->get_right()){
                    right_rotation(tmp->get_parent());
                    tmp = tmp->get_right();
                }
                tmp->get_parent()->set_color("black");
                get_grandparent(tmp)->set_color("red");
                if(tmp == tmp->get_parent()->get_left()){
                    right_rotation(get_grandparent(tmp));
                }
                else{
                    left_rotation(get_grandparent(tmp));
                }
            }
            tmp = get_grandparent(tmp);
        }
        root->set_color("black");
        return;
    }
    void fix_erase(rb_node<T>* new_node){
        return;
    }
public:
    raw_rb_tree(){
        tree_size = 0;
    }
    void insert(T key){
        if(tree_size == 0){
            root = new rb_node<T> (key, "black", nullptr);
        }
        else{
            rb_node<T>* tmp = root;
            while(true){
                if(tmp->get_key() < key) {
                    if (tmp->get_right() != nullptr)
                        tmp = tmp->get_right();
                    else
                        break;

                }
                else if(tmp->get_key() > key) {
                    if (tmp->get_left() != nullptr)
                        tmp = tmp->get_left();
                    else
                        break;
                }
                else{
                    return;
                }
            }
            if(tmp->get_key() > key) {
                rb_node<T>* new_node = new rb_node<T>(key, "red", tmp);
                tmp->set_left(new_node);
                fix_insertion(new_node);
            }
            else {
                rb_node<T>* new_node = new rb_node<T>(key, "red", tmp);
                tmp->set_right(new_node);
                fix_insertion(new_node);
            }

        }
        tree_size++;
    }
    void erase(T key){

    }
    bool find(T key){
        return true;
    }
    void print(){
        std::queue<rb_node<T>* > q;
        q.push(root);
        rb_node<T>* tmp;
        while(q.size() != 0){
            tmp = q.front();
            q.pop();
            if(tmp != nullptr) {
                std::cout << tmp->get_key() << " " << tmp->get_color() << "\n";
                q.push(tmp->get_left());
                q.push(tmp->get_right());
            }
        }
    }
};


double get_time()
{
    //chrono - класс для времени
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
}

int rand_uns(int min, int max)
{
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    static std::default_random_engine e(seed);
    std::uniform_int_distribution<int> d(min, max);
    return d(e);
}

int main() {
    int N = 10000, n = 10000;
    int *push_sequence = new int [N];
    int *test_sequance = new int [n];

    for (int i = 0; i < n; i++){
        test_sequance[i] = rand_uns(1, 1000);
        if (i < N){
            push_sequence[i] = rand_uns(1, 1000);
        }
    }

    cout << "What tree do you want to test?\n 1 - AVL tree \n 2 - Red and Black tree" << endl;
    char flag;
    std::cin >> flag;
    /*if (flag == '1') {
        AVL<int> tree;
    } else if (flag == '2'){
        raw_rb_tree<int> tree;
    } else {
        cout << "WTF have you written?" << endl;
        return 0;
    }*/
    AVL<int> tree;

    std::ofstream fout("data.txt"); // создаём объект класса ofstream для записи и связываем его с файлом
    for (int i = 0; i < N; i++){
        tree.insert(push_sequence[i]);

        double insert_time = 0;
        double find_time = 0;
        double erase_time = 0;

        //testing
        for (int j = 0; j < n; j++){
            double start_insert = get_time();
            tree.insert(test_sequance[0]);
            double end_insert = get_time();
            insert_time += end_insert - start_insert;

            double start_find = get_time();
            tree.find(test_sequance[0]);
            double end_find = get_time();
            find_time += end_find - start_find;

            double start_erase = get_time();
            tree.erase(test_sequance[0]);
            double end_erase = get_time();
            erase_time += end_erase - start_erase;
        }

        fout << i << " " << insert_time/n << " " << find_time/n << " " << erase_time/n << endl; // запись строки в файл
    }
    fout.close(); // закрываем файл

    delete [] push_sequence;
    delete [] test_sequance;

    std::system("python3 plotting.py");

    return 0;
}