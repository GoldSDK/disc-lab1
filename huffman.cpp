#include <iostream>
#include <map>
#include <queue>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

struct Node {
    int weight;
    char symbol;
    vector<Node*> children;
    Node(int w, char s = '\0') : weight(w), symbol(s) {}
    ~Node() { for (auto child : children) delete child; }
};

struct CompareNode {
    bool operator()(Node* a, Node* b) { return a->weight > b->weight; }
};

double compute_entropy(const map<char, int>& freq) {
    int total = 0;
    for (const auto& p : freq) total += p.second;
    double entropy = 0;
    for (const auto& p : freq) {
        if (p.second > 0) {
            double p_x = p.second / (double)total;
            entropy -= p_x * log2(p_x);
        }
    }
    return entropy;
}

Node* build_huffman_tree(const map<char, int>& freq, int d) {
    priority_queue<Node*, vector<Node*>, CompareNode> heap;
    for (const auto& p : freq) 
        if (p.second > 0) heap.push(new Node(p.second, p.first));
    
    if (heap.empty()) return nullptr;
    
    int remainder = (d - 1 - (heap.size() - 1) % (d - 1)) % (d - 1);
    while (remainder--) heap.push(new Node(0));
    
    while (heap.size() > 1) {
        int sum = 0;
        vector<Node*> children;
        for (int i = 0; i < d && !heap.empty(); ++i) {
            Node* node = heap.top();
            sum += node->weight;
            children.push_back(node);
            heap.pop();
        }
        Node* internal = new Node(sum);
        internal->children = children;
        heap.push(internal);
    }
    return heap.top();
}

void get_codes(Node* node, string code, map<char, string>& codes) {
    if (node->symbol != '\0') codes[node->symbol] = code.empty() ? "0" : code;
    else for (size_t i = 0; i < node->children.size(); ++i)
        get_codes(node->children[i], code + to_string(i), codes);
}

double compute_avg_length(const map<char, string>& codes, const map<char, int>& freq) {
    int total = 0;
    double avg = 0;
    for (const auto& p : codes) {
        int cnt = freq.at(p.first);
        avg += p.second.length() * cnt;
        total += cnt;
    }
    return total ? avg / total : 0;
}

int main() {
    map<char, int> freq = {
        {' ', 113}, {'А',43}, {'Б',13}, {'В',36}, {'Г',10}, {'Д',14}, {'Е',60},
        {'Ж',8}, {'З',11}, {'И',53}, {'К',19}, {'Л',26}, {'М',22}, {'Н',45},
        {'О',73}, {'П',12}, {'Р',26}, {'С',33}, {'Т',35}, {'У',18}, {'Ф',0},
        {'Х',11}, {'Ц',0}, {'Ч',12}, {'Ш',10}, {'Щ',1}, {'Ъ',0}, {'Ы',11},
        {'Ь',9}, {'Э',0}, {'Ю',2}, {'Я',20}
    };

    double entropy = compute_entropy(freq);
    cout << "Энтропия источника (в битах): " << entropy << "\n\n";

    Node* bin_tree = build_huffman_tree(freq, 2);
    map<char, string> bin_codes;
    if (bin_tree) get_codes(bin_tree, "", bin_codes);
    double bin_avg = compute_avg_length(bin_codes, freq);
    
    cout << "Двоичный код:\n";
    for (const auto& p : bin_codes) cout << p.first << ": " << p.second << '\n';
    cout << "Средняя длина кода: " << bin_avg << "\nИзбыточность: " << bin_avg - entropy << "\n\n";

    Node* quad_tree = build_huffman_tree(freq, 4);
    map<char, string> quad_codes;
    if (quad_tree) get_codes(quad_tree, "", quad_codes);
    double quad_avg = compute_avg_length(quad_codes, freq);
    double quad_bits = quad_avg * log2(4);
    
    cout << "4-ичный код:\n";
    for (const auto& p : quad_codes) cout << p.first << ": " << p.second << '\n';
    cout << "Средняя длина кода (4-ич.): " << quad_avg << "\nСредняя длина (биты): " 
         << quad_bits << "\nИзбыточность: " << quad_bits - entropy << endl;

    delete bin_tree;
    delete quad_tree;
}
