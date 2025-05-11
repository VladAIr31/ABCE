#include <random>
#include <chrono>
#include <fstream>

using namespace std;

ifstream in("abce.in");
ofstream out("abce.out");
void nos()
{
    ios_base::sync_with_stdio(false);
    in.tie(NULL);
}

class Node {
    int key;
    int priority;
    Node *left;
    Node *right;
    public:

    Node(int k_val) : key(k_val), left(NULL), right(NULL) {
        static std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
        static std::uniform_int_distribution<int> dist(1, 2'000'000'000);
        priority = dist(rng);
    }
    ~Node(){
          delete left;
          delete right;
     }

    int getKey() const {
      return key;
    }
    int getPriority() const {
      return priority;
    }
    Node* getLeft() const {
      return left;
    }
    Node* getRight() const {
      return right;
    }

    void setLeft(Node* new_left) { left = new_left; }
    void setRight(Node* new_right) { right = new_right; }

};

Node* rotate_right(Node* y) {
  Node* x = y->getLeft();
  Node* T2 = x->getRight();

  // Face rotatia folosind setteri
  x->setRight(y);
  y->setLeft(T2);

  return x;
}

Node* rotate_left(Node* x) {
  Node* y = x->getRight();
  Node* T2 = y->getLeft();

  // Face rotatia folosind setteri
  y->setLeft(x);
  x->setRight(T2);

  return y;
}

Node* insert(Node* root, int key_to_insert) {
  if (!root) {
    return new Node(key_to_insert);
  }

  if (key_to_insert < root->getKey()) {
    root->setLeft(insert(root->getLeft(), key_to_insert));
    if (root->getLeft() && root->getLeft()->getPriority() > root->getPriority()) {
      root = rotate_right(root);
    }
  } else if (key_to_insert > root->getKey()) {
    root->setRight(insert(root->getRight(), key_to_insert));
    if (root->getRight() && root->getRight()->getPriority() > root->getPriority()) {
      root = rotate_left(root);
    }
  }
  // Nu facem nimic dacă key_to_insert == root->getKey() (fara duplicate)

  return root;
}

Node* erase(Node* root, int to_erase) {
  if (!root) {
    return NULL;
  }

  if (to_erase < root->getKey()) {
    root->setLeft(erase(root->getLeft(), to_erase));
  } else if (to_erase > root->getKey()) {
    root->setRight(erase(root->getRight(), to_erase));
  } else { // Am gasit nodul de sters
    if (!root->getLeft()) {
      Node* temp = root->getRight();
      root->setRight(NULL); // Evitam stergerea dubla / facuta de catre destructor
      delete root;
      root = temp;
    } else if (!root->getRight()) {
      Node* temp = root->getLeft();
      root->setLeft(NULL); // Evitam stergerea dubla / facuta de catre destructor
      delete root;
      root = temp;
    } else {
      if (root->getLeft()->getPriority() > root->getRight()->getPriority()) {
        root = rotate_right(root);
        root->setRight(erase(root->getRight(), to_erase)); // Continuam in dreapta
      } else {
        root = rotate_left(root);
        root->setLeft(erase(root->getLeft(), to_erase)); // Continuam in stanga
      }
    }
  }

  return root;
}

bool search(Node* root, int x) {
  if (!root) {
    return false;
  }
  if (root->getKey() == x) {
    return true;
  }
  if (x < root->getKey()) {
    return search(root->getLeft(), x);
  }
  return search(root->getRight(), x);
}

Node* smaller_or_equal(Node* root, int x) {
  Node* current = root;
  Node* predecessor = NULL;

  while (current) {
    if (current->getKey() == x) {
      return current;
    } else if (current->getKey() < x) {
      predecessor = current;
      current = current->getRight();
    } else {
      current = current->getLeft();
    }
  }
  return predecessor;
}

Node* greater_or_equal(Node* root, int x) {
  Node* current = root;
  Node* successor = NULL;

  while (current) {
    if (current->getKey() == x) {
      return current;
    } else if (current->getKey() > x) {
      successor = current;
      current = current->getLeft();
    } else {
      current = current->getRight();
    }
  }
  return successor;
}

void range_query(Node* root, int X, int Y, std::vector<int>& result) {
  if (!root) {
    return;
  }

  if (root->getKey() > X) {
    range_query(root->getLeft(), X, Y, result);
  }

  if (root->getKey() >= X && root->getKey() <= Y) {
    result.push_back(root->getKey());
  }

  if (root->getKey() < Y) {
    range_query(root->getRight(), X, Y, result);
  }
}

Node* merge(Node* t1, Node* t2) {
  if (!t1) {
    return t2;
  }
  if (!t2) {
    return t1;
  }

  if (t1->getPriority() > t2->getPriority()) {
    t1->setRight(merge(t1->getRight(), t2));
    return t1;
  } else {
    t2->setLeft(merge(t1, t2->getLeft()));
    return t2;
  }
}

int main() {

    nos();
  Node* root = NULL;
  int Q;
  in >> Q;

   for (int op_count = 0; op_count < Q; ++op_count) {
        int type;
        in >> type;

        if (type == 1) {
            int X;
            in >> X;
            root = insert(root, X);
        } else if (type == 2) {
            int X;
            in >> X;
            root = erase(root, X);
        } else if (type == 3) {
            int X;
            in >> X;
            if (search(root, X)) {
                out << 1 << "\n";
            } else {
                out << 0 << "\n";
            }
        } else if (type == 4) {
            int X;
            in >> X;
            Node* result_node = smaller_or_equal(root, X);
            if (result_node) {
                out << result_node->getKey() << "\n";
            }
        } else if (type == 5) {
            int X;
            in >> X;
            Node* result_node = greater_or_equal(root, X);
            if (result_node) {
                out << result_node->getKey() << "\n";
            }
        } else if (type == 6) {
            int X, Y;
            in >> X >> Y;
            std::vector<int> result_vector;
            range_query(root, X, Y, result_vector);

            for (size_t j = 0; j < result_vector.size(); ++j) {
                out << result_vector[j] << (j == result_vector.size() - 1 ? "" : " ");
            }
            out << "\n";
        }
   }

  delete root;

  return 0;
}
