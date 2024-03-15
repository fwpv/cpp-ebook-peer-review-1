#include "LoadPerson.h"

#include <sstream>

using namespace std;

struct LoaderSettings{
    LoaderSettings& SetDBName(string_view db_name) {
        this->db_name = db_name;
        return *this;
    }
    LoaderSettings& SetConnectionTimeout(int db_connection_timeout) {
        this->db_connection_timeout = db_connection_timeout;
        return *this;
    }
    LoaderSettings& SetAllowExceptions(bool db_allow_exceptions) {
        this->db_allow_exceptions = db_allow_exceptions;
        return *this;
    }
    LoaderSettings& SetDBLogLevel(DBLogLevel db_log_level) {
        this->db_log_level = db_log_level;
        return *this;
    }
    LoaderSettings& SetMinAge(int min_age) {
        this->min_age = min_age;
        return *this;
    }
    LoaderSettings& SetMaxAge(int max_age) {
        this->max_age = max_age;
        return *this;
    }
    LoaderSettings& SetAnmeFilter(string_view name_filter) {
        this->name_filter = name_filter;
        return *this;
    }

    string_view db_name = "default_name";
    int db_connection_timeout = 1000;
    bool db_allow_exceptions = false;
    DBLogLevel db_log_level = 0;
    int min_age = 18;
    int max_age = 120;
    string_view name_filter;
};


vector<Person> LoadPersons(const LoaderSettings& ls) {
    DBConnector connector(ls.db_allow_exceptions, ls.db_log_level);
    DBHandler db;
    if (ls.db_name.substr("tmp."s.size()) == "tmp."sv) {
        db = connector.ConnectTmp(ls.db_name, ls.db_connection_timeout);
    } else {
        db = connector.Connect(ls.db_name, ls.db_connection_timeout);
    }
    if (!ls.db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << ls.min_age << " and "s << ls.max_age << " "s
              << "and Name like '%"s << db.Quote(ls.name_filter) << "%'"s;
    DBQuery query(query_str.str());

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}