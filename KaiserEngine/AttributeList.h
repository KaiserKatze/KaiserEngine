#pragma once

class AttributeList
{
public:
    AttributeList();
    ~AttributeList();
    void Put(const int& key, const int& val);
    void Clear();
    const std::vector<int> Make();
private:
    std::map<int, int> entries;
};
