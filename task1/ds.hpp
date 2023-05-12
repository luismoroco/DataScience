/**
 * DS using in program
*/

#include <vector>
#include <functional>
#include <cmath>
#include <utility>
#include <limits>
#include <map>
#include <queue>
#include <set>

using std::priority_queue;
using std::numeric_limits;
using std::vector;
using std::function;
using std::pair;
using std::map;
using std::set;
using std::string;

const float INF = numeric_limits<float>::max();

template <typename T>
struct Node {
  T v;
  int index;
  Node<T> *next;

  Node(T v, int x)
    :v(v), index(x), next(nullptr) {}
  
  bool operator<(const Node<T>& b) const { return v > b.v; }
};

template <typename T>
struct ObjectEngine {
  private:
    set<pair<int, T>> beng;
  
  public:
    T search(int src) {
      auto it = beng.lower_bound({src, T()});
      if (it != beng.end()) {
        return it->second;
      } 
      return T();
    }

    void add(int id, T w) {
      beng.insert({id, w});
    }
};

template <typename T>
struct LinkedList {
  Node<T> *root, *tail;
  T module, sum;
  int size;

  LinkedList()
    : root(nullptr), tail(nullptr), 
      module(0.0), sum(0.0), size(0) {}

  void add(Node<T> *node) {
    if (this->root == nullptr) {
      this->root = node;
      this->tail = node;
      this->size += 1;
      this->module += powf(node->v, 2);
      //this->sum += node->v;
    } else {
      this->tail->next = node;
      this->tail = node;
      this->size += 1;
      this->module += powf(node->v, 2);
      //this->sum += node->v;
    }
  }
};

template <typename T, typename R>
struct MatrixLLBased {
  vector<LinkedList<T>> roots;
  map<pair<int, int>, int> ndp;

  MatrixLLBased(int Len): roots(++Len) {
    for (auto& x : roots) 
      x = LinkedList<T>();
  }

  void add(int src, T v, int u) {
    Node<T> *node = new Node<T>(v, u);
    this->roots[src].add(node); 
  }

  T iterateTwoLL(int src, int to, function<T(T, T)> f, bool count = false) {
    T v = 0.0f;
    Node<T> *F = this->roots[src].root;
    Node<T> *S = this->roots[to].root;
    int n = 0;

    while (F != nullptr && S != nullptr) {
      if (F->index == S->index) {
        //printf("%f %f\n", F->v, S->v);
        v += f(F->v, S->v);
        F = F->next;
        S = S->next;
        ++n; 
      } else if (F->index < S->index) {
        F = F->next;
      } else {
        S = S->next;
      }
    } 

    if (count == true) ndp.insert({{src, to}, n});

    return v;
  } 

  vector<R> differen(int src, int to) {
    vector<R> diff;
    Node<T> *F = this->roots[src].root;
    Node<T> *S = this->roots[to].root;

    while (F != nullptr && S != nullptr) {
      if (F->index == S->index) {
        F = F->next;
        S = S->next;
      } else if (F->index < S->index) {
        diff.push_back(S->value);
        S = S->next;
      } else {
        F = F->next;
      }
    }

    while (S != nullptr) {
        diff.push_back(S->value);
        S = S->next;
    }

    return diff;
  }

  int searchN(int src, int to) {
    pair<int, int> x = {src, to}, y = {to, src};
    auto a = ndp.find(x);
    if (a != ndp.end()) return a->second;
    auto b = ndp.find(y);
    if (b != ndp.end()) return b->second;
    return -1;
  }

  T getModule(int src) { return roots[src].module; }
  T getSum(int src) { return roots[src].sum; }
};

template <typename T, typename R>
struct QueryEngine {
  private:
    MatrixLLBased<T, R> main;
    map<pair<int, int>, T> dp; 
    
    function<T(T, T)> dot = [](T x, T y) -> T { return x * y; };
    function<T(T, T)> manh = [](T x, T y) -> T { return fabs(x - y); };
    function<T(T, T)> eucle = [](T x, T y) -> T { return powf((x - y), 2); };

  public:
    QueryEngine(MatrixLLBased<T, R> v): main(v) {}

    pair<bool, T> search(int src, int to) {
      pair<int, int> x = {src, to}, y = {to, src};
      auto a = dp.find(x);
      if (a != dp.end()) 
        return {true, a->second};
      auto b = dp.find(y);
      if (b != dp.end()) 
        return {true, b->second};
      return {false, -1.0f};
    }

    T manhattan(int src, int to) {
      return main.iterateTwoLL(src, to, manh);
    }

    T eucledian(int src, int to) {
      return main.iterateTwoLL(src, to, eucle);
    } 

    T dotProduct(int src, int to) {
      pair<bool, T> query = search(src, to);
      if (query.first != false) 
        return query.second;
      
      T v = main.iterateTwoLL(src, to, dot, true);
      dp.insert({{src, to}, v});
      return v;
    }

    T pearson(int src, int to) {
      T f = dotProduct(src, to);
      int n = getN(src, to);
      T s = (getSum(src) * getSum(to))/n;
      T t = sqrtf(getModule(src) - ((pow(getSum(src), 2))/n));
      T q = sqrtf(getModule(to) - ((pow(getSum(to), 2))/n));

      //T g = getSum(to);
      //printf("SUMA ============> %f\n", g);

      return (t * q == 0) ? 0.0f : (f - s) / (t * q);
    }

    T cosine(int src, int to) {
      T v = dotProduct(src, to)/(sqrtf(getModule(src)) * sqrtf(getModule(to)));
    }

    T getModule(int src) { return main.getModule(src); }
    T getSum(int src) { return main.getSum(src); }
    int getN(int src, int to) { return main.searchN(src, to); }
};

template <typename T, typename R>
struct KNN {
  private:
    QueryEngine<T, R> qe;
    int LenUsers, K;
    vector<priority_queue<Node<T>>> knear;
  
  protected:
    void push(int src, const Node<T> &x) {
      if (knear[src].size() >= K) 
        knear[src].pop();
      knear[src].push(x);
    }

    void printPQ(int src) {
      while (!knear[src].empty()) {
        auto x = knear[src].top();
        printf("%f - %d\n", x.v, x.index);
        knear[src].pop();
      }
    }

  public:
    KNN(MatrixLLBased<T, R> x, int u, int k)
      : qe(x), LenUsers(++u), K(k), knear(u) {}
    
    pair<T, int> fitManhattan(int src) {
      pair<T, int> fit = {INF, -1};
      T v;
      for (int i = 1; i < LenUsers; ++i) {
        if (src == i) continue;
        v = qe.manhattan(src, i);
        if (std::isnan(v)) continue;
        Node<T> *node = new Node<T>(v, i);
        push(src, *node);
        fit = (v < fit.first) ? (pair<T, int>){v, i} : fit;
      }

      printPQ(src);
      return fit;
    }

    pair<T, int> fitEucledian(int src) {
      pair<T, int> fit = {INF, -1};
      T v;
      for (int i = 1; i < LenUsers; ++i) {
        if (src == i) continue;
        v = sqrtf(qe.eucledian(src, i));
        if (std::isnan(v)) continue;
        Node<T> *node = new Node<T>(v, i);
        push(src, *node);
        fit = (v < fit.first) ? (pair<T, int>){v, i} : fit;
      }

      printPQ(src);
      return fit;
    }

    pair<T, int> fitCosine(int src) {
      pair<T, int> fit = {-1.0f, -1};
      T v;
      for (int i = 1; i < LenUsers; ++i) {
        if (src == i) continue;
        v = qe.dotProduct(src, i)/(sqrtf(qe.getModule(src)) * sqrtf(qe.getModule(i)));
        if (std::isnan(v)) continue;
        Node<T> *node = new Node<T>(v, i);
        push(src, *node);
        fit = (v > fit.first) ? (pair<T, int>){v, i} : fit;
      }

      printPQ(src);
      return fit;
    }

    pair<T, int> fitBestPearson(int src) {
      pair<T, int> fit = {-1.0f, -1};
      T v;
      for (int i = 1; i < LenUsers; ++i) {
        if (src == i) continue;
        v = qe.pearson(src, i);
        std::cout << v << '\n';
        if (std::isnan(v)) continue;
        Node<T> *node = new Node<T>(v, i);
        push(src, *node);
        fit = (v > fit.first) ? (pair<T, int>){v, i} : fit;
      }

      printPQ(src);
      return fit;
    }

    T fitPearson(int src, int to) {
      return qe.pearson(src, to);
    }
};
