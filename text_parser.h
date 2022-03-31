#include <map>
#include <vector>
#include <fstream>
#include <cstring>

bool is_txt_format(char* file) {
    int i = 0;
    for(; i < strlen(file) and file[i] != '.'; i++) {}
    if(i == strlen(file)) return false;
    std::string format;
    for(;i < strlen(file); i++) {
        format += file[i];
    }
    if(format != ".txt") return false;
    return true;
}

struct Node {
    Node() : len(0), link(-1), is_clone(false) {}
    long long len, link, first_entry;
    bool is_clone;
    std::map<char, long long> to;
    std::vector<int> inv_link;
};

class text_searcher {
public:
    explicit text_searcher(const std::string& s) : last(0) {
        build_structure(s);
    }
    explicit text_searcher(char* way_file);
    std::vector<std::vector<long long>> get_all_occurrences(const std::vector<std::string>& all_lines);
    bool is_valid() {
        return valid;
    }
private:
    void add(char c);
    std::vector<long long> dfs(const std::string& s, long long v, long long i, long long sz);
    void output_all_occurences (long long v, long long str_size, std::vector<long long>& all_occur) const;
    void build_structure(const std::string& s);

    std::vector<Node> t;
    long long last;
    bool valid;
};

text_searcher::text_searcher(char *way_file)  : last(0), valid(true) {
    std::fstream in(way_file);
    std::string t;
    if (in.is_open()) {
        std::string line;
        while (getline(in, line)) {
            t += line + ' ';
        }
    } else {
        valid = false;
        return;
    }
    in.close();
    build_structure(t);
}

std::vector<std::vector<long long>> text_searcher::get_all_occurrences(const std::vector<std::string>& all_lines) {
    std::vector<std::vector<long long>> all_occurr;
    for(size_t i = 0; i < all_lines.size(); i++) {
        all_occurr.push_back(dfs(all_lines[i], 0, 0, all_lines[i].size()));
    }
    return all_occurr;
}

void text_searcher::add(char c) {
    t.push_back(Node());
    long long curr = t.size() - 1;
    t[curr].len = t[last].len + 1;
    t[curr].first_entry = t[curr].len - 1;
    t[curr].is_clone = false;
    long long p = last;
    while(p != -1 and !t[p].to.count(c)) {
        t[p].to[c] = curr;
        p = t[p].link;
    }
    if (p == -1) {
        t[curr].link = 0;
        last = curr;
        return;
    }
    long long q = t[p].to[c];
    if (t[p].len + 1 == t[q].len) {
        t[curr].link = q;
        last = curr;
        return;
    }
    t.push_back(Node());
    long long clone = t.size() - 1;
    t[clone].is_clone = true;
    t[clone].len = t[p].len + 1;
    t[clone].to = t[q].to;
    t[clone].link = t[q].link;
    t[clone].first_entry = t[q].first_entry;
    while(p != -1 and t[p].to[c] == q) {
        t[p].to[c] = clone;
        p = t[p].link;
    }
    t[curr].link = clone;
    t[q].link = clone;
    last = curr;
}

std::vector<long long> text_searcher::dfs(const std::string& s, long long v, long long i, long long sz) {
    if(i == sz) {
        std::vector<long long> all_occur;
        output_all_occurences(v, s.size(), all_occur);
        return all_occur;
    }
    if(!t[v].to.count(s[i])) {
        return std::vector<long long>();
    }
    i += 1;
    return dfs(s, t[v].to[s[i - 1]], i, sz);
}

void text_searcher::output_all_occurences (long long v, long long str_size, std::vector<long long>& all_occur) const {
    if (!t[v].is_clone)
        all_occur.push_back(t[v].first_entry - str_size + 1);
    for (size_t i = 0; i < t[v].inv_link.size(); ++i)
        output_all_occurences (t[v].inv_link[i], str_size, all_occur);
}

void text_searcher::build_structure(const std::string& s) {
    t.push_back(Node());
    for(size_t i = 0; i < s.size(); i++) {
        add(s[i]);
    }
    for(size_t v = 1; v < t.size(); v++) {
        t[t[v].link].inv_link.push_back(v);
    }
}
