#include <iostream>
#include <algorithm>
#include <random>
#include <limits>
#include <chrono>
#include <fstream>
#include <utility>
#include <set>

using namespace std;

class SetOfPoints{ // родительский класс - некоторое множество рациональных точек
public:
    virtual bool contains(int p) { // метод, проверяющий, есть ли точка в данном множестве
        return false;
    }
};

class Points: public SetOfPoints{ // дочерний класс - конечный набор точек
private:
    int number; // число элементов во множестве
    set <int> plenty; // множество
public:
    Points(): Points(0, {}) {} // делигирующие конструкторы
    Points(int old_number, set <int> old_array){
        number = old_number;
        plenty = move(old_array);
    }
    void append(int new_last){ // метод, добавляющий во множество элемент new_last, если его там нет
        if (plenty.find(new_last)== plenty.end()){
            number++;
        }
        plenty.insert(new_last);
    }
    int get_number(){ // метод, возвращающий число элементов во множестве
        return number;
    }
    bool contains(int p) override { // перезаписанный метод, проверяющий, есть ли точка в данном множестве
        if (plenty.find(p) == plenty.end()) {
            return false;
        }
        return true;
    }
};


class Segment: public SetOfPoints{ // дочерний класс - отрезок
private:
    int end, begin; // границы отрезка
public:
    Segment(int begin, int end): end(end), begin (begin){}; // конструктор
    bool contains(int p) override { // перезаписанный метод, проверяющий, есть ли точка в данном множестве
        return (p <= end && p >= begin);
    }
};


class SegmentNoPoints: public Segment{ // дочерний класс - отрезок без конечного набора точек
private:
    Points points; // множество выколотых точек
public:
    SegmentNoPoints(int begin, int end, int old_number, set <int> old_array) : Segment(begin, end), points(old_number, move(old_array)) {} // перегруженные конструкторы
    SegmentNoPoints(int begin, int end, Points points) : Segment(begin, end), points(move(points)) {}
    bool contains(int p) override { // перезаписанный метод, проверяющий, есть ли точка в данном множестве
        return (Segment::contains(p) && !points.contains(p));
    }
};


class Interval: public SetOfPoints{ //дочерний класс - интервал
private:
    int end, begin; // границы интервала
public:
    Interval(int begin, int end): end(end), begin (begin){};
    bool contains(int p) override { // перезаписанный метод, проверяющий, есть ли точка в данном множестве
        return (p < end && p > begin);
    }
};


class IntervalNoPoints: public Interval{ // дочерний класс - интервал без конечного набора точек
private:
    Points points; // множество выколотых точек
public:
    IntervalNoPoints(int begin, int end, int old_number, set <int> old_array) : Interval(begin, end), points(old_number, move(old_array)) {} // перегруженные конструкторы
    IntervalNoPoints(int begin, int end, Points points) : Interval(begin, end), points(move(points)) {}
    bool contains(int p) override { // перезаписанный метод, проверяющий, есть ли точка в данном множестве
        return (Interval::contains(p) && !points.contains(p));
    }
};

float probability(SetOfPoints &set, int min, int max, int tests){ // функция для подсчета вероятности выбора точки из заданного множества set, tests - число испытаний
    auto x = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();
    float luck = 0;
    default_random_engine rng(x);
    uniform_int_distribution<int> dstr(min, max);
    for (int i = 0; i < tests; i++){
        if (set.contains(dstr(rng))){
            luck += 1;
        }
    }
    return luck / tests;
}




int main() {
    auto points = Points();
    while (points.get_number() < 18){
        auto y = chrono::duration_cast<chrono::seconds>(chrono::system_clock::now().time_since_epoch()).count();
        default_random_engine rng(y);
        uniform_int_distribution<int> dstrb(0, 39);
        points.append(dstrb(rng));
    }
    auto set_of_points = SegmentNoPoints(0,39,points);
    for (int t = 1; t < 1e5; t++){
        fstream inOut;
        inOut.open("file.txt",ios::app);
        inOut << t;
        inOut << ' ';
        inOut << probability(set_of_points,0,99,t) << endl;
        inOut.close();
    }
    return 0;
}
