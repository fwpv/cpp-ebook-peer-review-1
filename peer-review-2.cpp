#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    Domain(const string& name)
    : data_(name.rbegin(), name.rend()) {
        data_ += "."s;
    }
    bool operator==(const Domain& rhs) const {
        return data_ == rhs.data_;
    }
    bool operator!=(const Domain& rhs) const {
        return !(*this == rhs);
    }
    bool operator<(const Domain& rhs) const {
        return std::lexicographical_compare(data_.begin(), data_.end(),
            rhs.data_.begin(), rhs.data_.end());
    }

    // Возвращает true, если this поддомен other (или домены совпали)
    bool IsSubdomain(const Domain& other) const {
        return data_.size() >= other.data_.size()
            && string_view(data_.data(), other.data_.size()) == string_view(other.data_);
    }
private:
    string data_;
};

class DomainChecker {
public:
    template<typename InputIt>
    DomainChecker(InputIt first, InputIt last)
    : forbidden_domains_(first, last) {
        sort(forbidden_domains_.begin(), forbidden_domains_.end());
        auto new_last = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(),
            [](const Domain& first, const Domain& second){
                return second.IsSubdomain(first);
        });
        forbidden_domains_.erase(new_last, forbidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) {
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain);
        if (it == forbidden_domains_.begin()) {
            return false;
        } else {
            return domain.IsSubdomain(*prev(it));
        }
    }
private:
    vector<Domain> forbidden_domains_;
};

void TestDomain() {
    assert(Domain("gdz.ru"s) == Domain("gdz.ru"s));
    assert(Domain("gdz.ru"s) != Domain("gdz.io"s));
    assert(Domain("gdz.ru"s).IsSubdomain("ru"s));
    assert(!Domain("gdz.io"s).IsSubdomain("ru"s));
    assert(Domain("m.gdz.ru"s).IsSubdomain("gdz.ru"s));
    assert(!Domain("ru"s).IsSubdomain("gdz.ru"s));
}

void TestDomainChecker() {
    const std::vector<Domain> forbidden_domains{"gdz.ru"s, "maps.me"s, "m.gdz.ru"s, "com"s};
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    assert(checker.IsForbidden("gdz.ru"s));
    assert(checker.IsForbidden("gdz.com"s));
    assert(checker.IsForbidden("m.maps.me"s));
    assert(checker.IsForbidden("alg.m.gdz.ru"s));
    assert(checker.IsForbidden("maps.com"s));
    assert(!checker.IsForbidden("maps.ru"s));
    assert(!checker.IsForbidden("gdz.ua"s));
}

std::vector<Domain> ReadDomains(istream& input, size_t n) {
    std::vector<Domain> result;
    result.reserve(n);
    while (n--) {
        string str;
        if (!getline(input, str)) {
            throw runtime_error("error reading the input stream"s);
        }
        result.push_back(str);
    }
    return result;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    if (!input) {
        throw runtime_error("error reading the input stream"s);
    }

    return num;
}

void RunAlltests() {
    TestDomain();
    TestDomainChecker();
}

int main() {
    RunAlltests();

    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}