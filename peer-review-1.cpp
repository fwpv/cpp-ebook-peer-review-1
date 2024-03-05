#include <numeric>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ReadMotivator {
public:
    void Read(int user, int page) {
        AdjustSizeOfArrays(user, page);
        int prev_page = page_of_user[user];
        if (prev_page != -1) {
            users_on_page[prev_page]--;
        } else {
            users_count++;
        }
        page_of_user[user] = page;
        users_on_page[page]++;
    }

    double Cheer(int user) const {
        if (user >= static_cast<int>(page_of_user.size())) {
            return 0.0;
        }
        int cur_page = page_of_user[user];
        if (cur_page == -1) {
            return 0.0;
        }
        int total_less = accumulate(users_on_page.begin(), users_on_page.begin() + cur_page, 0);
        int total_more_or_equal = users_count - 1 - total_less;
        if (total_more_or_equal == 0) {
            return 1.0;
        } else {
            return static_cast<double>(total_less) / (total_more_or_equal + total_less);
        } 
    }

private:
    vector<int> page_of_user; // index - номер пользователя, value - до какой страницы дочитал
    vector<int> users_on_page; // index - номер страницы, value - сколько пользователей дочитало
    int users_count = 0; // всего пользователей читают книгу

    void AdjustSizeOfArrays(int user, int page) {
        if (user >= static_cast<int>(page_of_user.size())) {
            page_of_user.resize(user + 1, -1);
        }
        if (page >= static_cast<int>(users_on_page.size())) {
            users_on_page.resize(page + 1, 0);
        }
    }
};

void TestReadMotivator() {
    ReadMotivator motivator;
    assert(motivator.Cheer(5) == 0.0);
    motivator.Read(1, 10);
    assert(motivator.Cheer(1) == 1.0);
    motivator.Read(2, 5);
    motivator.Read(3, 7);
    assert(motivator.Cheer(2) == 0.0);
    assert(motivator.Cheer(3) == 0.5);
    motivator.Read(3, 10);
    assert(motivator.Cheer(3) == 0.5);
    motivator.Read(3, 11);
    assert(motivator.Cheer(3) == 1.0);
    assert(motivator.Cheer(1) == 0.5);
}

int main() {
    TestReadMotivator();
  
    ReadMotivator motivator;

    int commands;
    cin >> commands;
    while (commands--) {
        string command;
        cin >> command;

        if (command == "READ"s) {
            int user, page;
            cin >> user >> page;
            motivator.Read(user, page);
        } else { // command == "CHEER"s
            int user;
            cin >> user;
            cout << motivator.Cheer(user) << endl;
        }
    }
}

// для ревью