#include <iostream>
#include <vector>
#include <random>
#include <set>
#include <chrono>
#include <fstream>
#include <cstdlib>
#include "AVL_tree"
#include "RedBlack_tree"

using std::cout;
using std::endl;

class Balanced_tree<typename T>{
    void virtual insert(T x){};
    void virtual find(T x){};
    void virtual erase(T x){};
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
    cin >> flag;
    if (flag == '1') {
        AVL_tree tree;
    } else if (flag == '2'){
        RedBlack_tree tree;
    } else {
        cout << "WTF have you written?" << endl;
        return 0;
    }

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