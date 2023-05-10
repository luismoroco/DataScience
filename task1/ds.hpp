/**
 * DS using in program
*/

#include <vector>
#include <functional>
#include <cmath>
#include <utility>

using std::vector;
using std::function;
using std::pair;

template <typename T>
struct Node {
  T value;
  int index;
  Node<T> *next;

  Node(T v, int x):
    value(v), index(x),
    next(nullptr) {}
};

template <typename T>
struct LinkedList {
  Node<T> *root, *tail;
  T module, sum;
  int size;

  LinkedList():
    root(nullptr), tail(nullptr),
    module(0.0), sum(0.0), size(0) {}

  void add(Node<T> *node) {
    if (this->root == nullptr) {
      this->root = node;
      this->tail = node;
      this->size += 1;
      this->module += node->value * node->value;
    } else {
      this->tail->next = node;
      this->tail = node;
      this->size += 1;
      this->module += node->value * node->value;
    }
  }
};

template <typename T>
struct MatrixLLBased {
  vector<LinkedList<T>> roots;

  MatrixLLBased(int Len): roots(++Len) {
    for (auto& x : roots) 
      x = LinkedList<T>();
  }

  void add(int src, T v, int u) {
    Node<T> *node = new Node<T>(v, u);
    this->roots[src].add(node); 
  }

  T iterateTwoLL(int src, int to, function<T(T, T)> f) {
    T v = 0.0f;
    Node<T> *F = this->roots[src].root;
    Node<T> *S = this->roots[to].root;

    while (F != nullptr && S != nullptr) {
      if (F->index == S->index) {
        v += f(F->value, S->value);
        F = F->next;
        S = S->next; 
      } else if (F->index < S->index) {
        F = F->next;
      } else {
        S = S->next;
      }
    } 

    return v;
  } 

  T getModule(int src) { return roots[src].module; }
};

template <typename T>
struct QueryEngine {
  private:
    MatrixLLBased<T> main;
    
    function<T(T, T)> dot = [](T x, T y) -> T { return x * y; };
    function<T(T, T)> manh = [](T x, T y) -> T { return fabs(x - y); };
    function<T(T, T)> eucle = [](T x, T y) -> T { return powf((x - y), 2); };

  public:
    QueryEngine(MatrixLLBased<T> v): main(v) {}

    T manhattan(int src, int to) {
      return main.iterateTwoLL(src, to, manh);
    }

    T eucledian(int src, int to) {
      return main.iterateTwoLL(src, to, eucle);
    } 

    T dotProduct(int src, int to) {
      return main.iterateTwoLL(src, to, dot);
    }

    T getModule(int src) { return main.getModule(src); }
};

template <typename T>
struct Main {
  private:
    QueryEngine<T> qe;
    int LenUsers;

  public:
    Main(MatrixLLBased<T> x, int u): qe(x), LenUsers(++u) {}
    
    pair<T, int> fitManhattan(int src) {
      pair<T, int> fit = {99999.0f, -1};
      T v;
      for (int i = 1; i <= LenUsers; ++i) {
        if (src == i) continue;
        v = qe.manhattan(src, i);
        fit = (v < fit.first) ? (pair<T, int>){v, i} : fit;
      }

      return fit;
    }

    pair<T, int> fitEucledian(int src) {
      pair<T, int> fit = {99999.0f, -1};
      T v;
      for (int i = 1; i <= LenUsers; ++i) {
        if (src == i) continue;
        v = qe.eucledian(src, i);
        fit = (v < fit.first) ? (pair<T, int>){v, i} : fit;
      }

      return fit;
    }

    pair<T, int> fitCosine(int src) {
      printf("%d\n", src);
      pair<T, int> fit = {-1.0f, -1};
      T v;
      for (int i = 1; i <= LenUsers; ++i) {
        if (src == i) continue;
        v = qe.dotProduct(src, i)/(sqrtf(qe.getModule(src)) * sqrtf(qe.getModule(i)));
        fit = (v > fit.first) ? (pair<T, int>){v, i} : fit;
      }

      return fit;
    }
    
};
