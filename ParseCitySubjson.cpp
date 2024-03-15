// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
vector<City> ParseCitySubjson(const Json& json, const string& country_name,
                      const string& country_iso_code, const string& country_phone_code, const string& country_time_zone,
                      const vector<Language>& languages) {
    vector<City> cities;
    for (const auto& city_json : json.AsList()) {
        const auto& city_obj = city_json.AsObject();
        cities.push_back({city_obj["name"s].AsString(), city_obj["iso_code"s].AsString(),
                          country_phone_code + city_obj["phone_code"s].AsString(), country_name, country_iso_code,
                          country_time_zone, languages});
    }
    return cities;
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
vector<Country> ParseCountryJson(const Json& json) {
    vector<Country> countries;
    for (const auto& country_json : json.AsList()) {
        const auto& country_obj = country_json.AsObject();
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
        });
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }
    }
    return countries;
}