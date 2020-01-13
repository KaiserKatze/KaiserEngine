#pragma once

class AttributeList
{
public:
    AttributeList();
    ~AttributeList();
    void put(const int& key, const int& val);
    void clear();
    size_t size() const;
    const std::vector<int> make();
private:
    std::map<int, int> entries;
};
