#include <iostream>
#include <string>
#include <queue>

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
class raw_rb_tree{
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
    rb_node<T>* get_sibling(rb_node<T>* cr_node){
        if(cr_node == cr_node->get_parent()->get_left()){
            return cr_node->get_parent()->get_right();
        }
        else{
            return  cr_node->get_parent()->get_left();
        }
    }
    void replace_node(rb_node<T>* N, rb_node<T>* child){
        if(child != nullptr){
            child->set_parent(N->get_parent());
        }
        if(N == N->get_parent()->get_left()){
            N->get_parent()->set_left(child);
        }
        else{
            N->get_parent()->set_right(child);
        }
        return;
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
        else{
            root = tmp;
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
        else{
            root = tmp;
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
        while(tmp != nullptr and tmp->get_parent() != nullptr and tmp->get_parent()->get_color() == "red"){
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
public:
    raw_rb_tree(){
        root = nullptr;
        tree_size = 0;
    }
    unsigned int get_size(){
        return tree_size;
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
        tree_size--;
        rb_node<T>* node_to_erase = root;
        while(node_to_erase->get_key() != key){
            if(node_to_erase->get_key() > key){
                if(node_to_erase->get_left() != nullptr){
                    node_to_erase = node_to_erase->get_left();
                }
                else{
                    return;
                }
            }
            else if(node_to_erase->get_key() < key){
                if(node_to_erase->get_right() != nullptr){
                    node_to_erase = node_to_erase->get_right();
                }
                else{
                    return;
                }
            }
        }
        rb_node<T>* N;
        if(node_to_erase->get_left() != nullptr)
            N = node_to_erase->get_left();
        else{
            N = node_to_erase;
        }
        while(N->get_right() != nullptr){
            N = N->get_right();
        }
        node_to_erase->set_key(N->get_key());
        rb_node<T>* child;
        if(N->get_right() != nullptr){
            child = N->get_right();
        }
        else{
            child = N->get_left();
        }
        if(N->get_parent() == nullptr){
            delete root;
            return;
        }
        if(N->get_color() == "red")
            replace_node(N, child);
        else if(N->get_color() == "black") {
            if (child != nullptr and child->get_color() == "red") {
                child->set_color("black");
            }
            else {
                rb_node<T>* tmp = N;
                while(true){
                    if(tmp->get_parent() == nullptr){ // case 1
                        break;
                    }
                    rb_node<T>* s = get_sibling(tmp);
                    if(s != nullptr and s->get_color() == "red"){ // case 2
                        tmp->get_parent()->set_color("red");
                        s->set_color("black");
                        if(tmp == tmp->get_parent()->get_left()){
                            left_rotation(tmp->get_parent());
                        }
                        else{
                            right_rotation(tmp->get_parent());
                        }
                    }
                    s = get_sibling(tmp);// case 3
                    if(tmp->get_parent()->get_color() == "black" and s->get_color() == "black" and (s->get_left() ==
                    nullptr or s->get_left()->get_color() == "black") and (s->get_right() == nullptr or s->get_right()->get_color() == "black")){
                        s->set_color("red");
                        tmp = tmp->get_parent();
                        continue;
                    } // case 4
                    else if(tmp->get_parent()->get_color() == "red" and s->get_color() == "black" and (s->get_left() ==
                    nullptr or s->get_left()->get_color() == "black") and (s->get_right() == nullptr or s->get_right()->get_color() == "black")){
                        s->set_color("red");
                        tmp->get_parent()->set_color("black");
                        break;
                    }
                    else if(s->get_color() == "black") {
                        if (tmp == tmp->get_parent()->get_left() and (s->get_left() !=
                                                                      nullptr and
                                                                      s->get_left()->get_color() == "red") and
                            (s->get_right() == nullptr or s->get_right()->get_color() == "black")) {
                            s->set_color("red");
                            s->get_left()->set_color("black");
                            right_rotation(s);
                        }
                        else if (tmp == tmp->get_parent()->get_right() and (s->get_right() !=
                                                                             nullptr and
                                                                             s->get_right()->get_color() == "red") and
                                   (s->get_left() == nullptr or s->get_left()->get_color() == "black")) {
                            s->set_color("red");
                            s->get_right()->set_color("black");
                            left_rotation(s);
                        }
                    }
                    s = get_sibling(tmp);
                    s->set_color(tmp->get_parent()->get_color());
                    tmp->get_parent()->set_color("black");
                    if(tmp == tmp->get_parent()->get_left()){
                        s->get_right()->set_color("black");
                        left_rotation(tmp->get_parent());
                    }
                    else{
                        s->get_left()->set_color("black");
                        right_rotation(tmp->get_parent());
                    }
                    break;
                }
            }
            replace_node(N, child);
        }

        delete N;
        return;
    }
    void print(){
        if(tree_size == 0)
            return;
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
        std::cout << '\n';
    }
    bool find(T key){
        rb_node<T>* tmp = root;
        while(tmp != nullptr){
            if(tmp->get_key() == key){
                return true;
            }
            else{
                if(tmp->get_key() < key){
                    tmp = tmp->get_right();
                }
                else{
                    tmp = tmp->get_left();
                }
            }
        }
        return false;
    }
    ~raw_rb_tree(){
        if(tree_size != 0)
        delete root;
    }
};


template <typename T>
class rb_set{
private:
    raw_rb_tree<T> tree;
public:
    void insert(T value){
        tree.insert(value);
    }
    void erase(T value){
        tree.erase(value);
    }
    bool find(T value){
       return tree.find(value);
    }
    unsigned int size(){
        return tree.get_size();
    }

};
int main() {
    rb_set<int> s;
    s.insert(1);
    s.insert(2);
    s.insert(3);
    s.insert(4);
    s.insert(5);
    s.erase(2);
    s.insert(2);
    std::cout << s.size();
    std::cout << s.find(2) << " " << s.find(8) << '\n';
    s.erase(2);
    std::cout << s.find(2) << " " << s.find(8);
//    tr.print();
//    tr.erase(5);
//    tr.erase(1);
//    tr.erase(3);
//    tr.erase(2);
//    tr.print();

    //tr.insert(6);
//    tr.insert(4);
//    tr.insert(5);
//    tr.insert(6);
//    tr.insert(7);
//    tr.insert(8);
//    tr.insert(2);
//    tr.insert(9);
//    tr.insert(10);
//    tr.insert(-1);
//    tr.erase(10);

    return 0;
}
