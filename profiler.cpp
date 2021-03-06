#include "AVL_tree.h"
#include <iostream>
#include <random>
#include <set>
#include <chrono>
#include <fstream>
#include "rb_tree.cpp"

using std::cout;
using std::endl;

template<class T>
class profiler {
private:
    int N = 1000, n = 10000;
    int *push_sequence = new int[N];
    int *test_sequence = new int[n];
    T tree;

    double get_time(){
        //chrono - класс для времени
        return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    int rand_uns(int min, int max){
        unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
        static std::default_random_engine e(seed);
        std::uniform_int_distribution<int> d(min, max);
        return d(e);
    }

public:
    profiler() {
        for (int i = 0; i < n; i++) {
            test_sequence[i] = rand_uns(1, 1000000);
        }
        for (int i = 0; i < N; i++) {
            push_sequence[i] = rand_uns(1, 1000000);
        }
    }

    void test() {
        std::ofstream fout("data.txt"); // создаём объект класса ofstream для записи и связываем его с файлом
        for (int i = 0; i < N; i++) {
            tree.insert(push_sequence[i]);

            double insert_time = 0;
            double find_time = 0;
            double erase_time = 0;

            //testing
            for (int j = 0; j < n; j++) {
                double start_insert = get_time();
                tree.insert(test_sequence[j]);
                double end_insert = get_time();
                insert_time += end_insert - start_insert;

                double start_find = get_time();
                tree.find(test_sequence[j]);
                double end_find = get_time();
                find_time += end_find - start_find;

                double start_erase = get_time();
                tree.erase(test_sequence[j]);
                double end_erase = get_time();
                erase_time += end_erase - start_erase;
            }

            fout << i << " " << insert_time / n << " " << find_time / n << " " << erase_time / n
                 << endl; // запись строки в файл
        }
        fout.close(); // закрываем файл

        std::system("python3 plotting.py");
    }

    ~profiler(){
        delete [] push_sequence;
        delete [] test_sequence;
    }
};

int main(){
    profiler <rb_set<int> > pf;
    pf.test();
}

