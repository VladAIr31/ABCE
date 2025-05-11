#include <bits/stdc++.h>
using namespace std;

ifstream fin("abce.in");
ofstream fout("abce.out");

struct Nod {
    int key;
    vector<Nod*> forward;
    Nod(int k, int level) : key(k), forward(level+1, nullptr) {}
};

class SkipList {
    int maxLevel;
    float p;
    int level;
    Nod* header;
    mt19937 gen;
    uniform_real_distribution<> dis;

public:
    SkipList(int maxLevel=16, float p=0.5)
        : maxLevel(maxLevel), p(p), level(0), gen(random_device{}()), dis(0.0, 1.0) {
        header = new Nod(INT_MIN, maxLevel);
    }

    int randomLevel() {
        int lvl = 0;
        while (dis(gen) < p && lvl < maxLevel) {
            lvl++;
        }
        return lvl;
    }

    void insert(int key) {
        vector<Nod*> update(maxLevel+1);
        Nod* current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }
        current = current->forward[0];
        if (!current || current->key != key) {
            int rndLevel = randomLevel();
            if (rndLevel > level) {
                for (int i = level+1; i <= rndLevel; i++)
                    update[i] = header;
                level = rndLevel;
            }
            Nod* newNode = new Nod(key, rndLevel);
            for (int i = 0; i <= rndLevel; i++) {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        }
    }

    void erase(int key) {
        vector<Nod*> update(maxLevel+1);
        Nod* current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
            update[i] = current;
        }
        current = current->forward[0];
        if (current && current->key == key) {
            for (int i = 0; i <= level; i++) {
                if (update[i]->forward[i] != current) break;
                update[i]->forward[i] = current->forward[i];
            }
            delete current;
            while (level > 0 && !header->forward[level])
                level--;
        }
    }

    bool exists(int key) const {
        Nod* current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
        }
        current = current->forward[0];
        return current && current->key == key;
    }

    int predecessor(int key) const {
        Nod* current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key <= key)
                current = current->forward[i];
        }
        return (current->key != INT_MIN ? current->key : INT_MIN);
    }

    int successor(int key) const {
        Nod* current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < key)
                current = current->forward[i];
        }
        current = current->forward[0];
        return (current ? current->key : INT_MAX);
    }

    vector<int> range_query(int x, int y) const {
        vector<int> res;
        Nod* current = header;
        for (int i = level; i >= 0; i--) {
            while (current->forward[i] && current->forward[i]->key < x)
                current = current->forward[i];
        }
        current = current->forward[0];
        while (current && current->key <= y) {
            res.push_back(current->key);
            current = current->forward[0];
        }
        return res;
    }
};

int main() {
    ios::sync_with_stdio(false);
    fin.tie(nullptr);

    int Q;
    fin >> Q;
    SkipList sl;
    while (Q--) {
        int t;
        fin >> t;
        if (t == 1) {
            int x; fin >> x;
            sl.insert(x);
        } else if (t == 2) {
            int x; fin >> x;
            sl.erase(x);
        } else if (t == 3) {
            int x; fin >> x;
            fout << (sl.exists(x) ? 1 : 0) << '\n';
        } else if (t == 4) {
            int x; fin >> x;
            int pred = sl.predecessor(x);
            if (pred != INT_MIN) fout << pred;
            else fout << "NU EXISTA"; // nu ar trebui sa intre aici decat pe teste custom
            fout << '\n';
        } else if (t == 5) {
            int x; fin >> x;
            int succ = sl.successor(x);
            if (succ != INT_MAX) fout << succ;
            else fout << "NU EXISTA";
            fout << '\n';
        } else if (t == 6) {
            int x, y;
            fin >> x >> y;
            auto res = sl.range_query(x, y);
            for (size_t i = 0; i < res.size(); ++i) {
                if (i) fout << ' ';
                fout << res[i];
            }
            fout << '\n';
        }
    }
    return 0;
}
