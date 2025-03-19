#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <tuple>
#include <numeric>

using namespace std;

const string encrypted_text = 
    "Л аунтис хомолетра б наесокпойн ,уюноко пор юнуейит схии ю, яго м рилазчип тьриев\n"
    "сте,е е дкеало б отер,аег дк ваорялаб, откорыечх рнс ыена,ист пбоодноуа пти ,неневдпоижр\n"
    "ноислаовисань бндлеойре чтебе носнокла.уС «дап вринасти ьтес, оп— ду лмая,з —ав\n"
    "атротвапрлюв сь Гнеелдж.»ик";

vector<vector<int>> generate_permutations(int size) {
    vector<vector<int>> perms;
    vector<int> base(size);
    iota(base.begin(), base.end(), 0);
    do perms.push_back(base);
    while (next_permutation(base.begin(), base.end()));
    return perms;
}

vector<int> invert_perm(const vector<int>& perm) {
    vector<int> inv(perm.size());
    for (int i = 0; i < perm.size(); ++i)
        inv[perm[i]] = i;
    return inv;
}

string decrypt_block(const string& block, const vector<int>& inv) {
    string res;
    for (int i : inv)
        res += block[i];
    return res;
}

int count_bigrams(const string& text, const vector<string>& bigrams) {
    int cnt = 0;
    for (size_t i = 0; i < text.size() - 1; ++i)
        for (const auto& bg : bigrams)
            if (text.substr(i, 2) == bg) {
                ++cnt;
                break;
            }
    return cnt;
}

int main() {
    const int block_size = 6;
    vector<string> blocks;
    for (size_t i = 0; i < encrypted_text.size(); i += block_size) {
        string block = encrypted_text.substr(i, block_size);
        if (block.size() < block_size)
            block += string(block_size - block.size(), ' ');
        blocks.push_back(block);
    }

    vector<string> bigrams = {"ст","но","то","на","пр","по","ра","ко","ни","не","ен","ov","го","во","ре","ta"};
    auto perms = generate_permutations(block_size);
    vector<tuple<int, vector<int>, string>> results;

    for (const auto& perm : perms) {
        auto inv = invert_perm(perm);
        string decrypted;
        for (const auto& block : blocks)
            decrypted += decrypt_block(block, inv);
        results.emplace_back(count_bigrams(decrypted, bigrams), perm, decrypted);
    }

    sort(results.begin(), results.end(), [](const auto& a, const auto& b) {
        return get<0>(a) > get<0>(b);
    });

    auto& best = results.front();
    cout << "Лучшая перестановка:";
    for (int n : get<1>(best)) cout << ' ' << n;
    cout << "\nБиграмм: " << get<0>(best) << "\nТекст:\n" << get<2>(best) << endl;
}
