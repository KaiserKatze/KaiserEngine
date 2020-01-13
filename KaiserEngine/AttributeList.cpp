#include "stdafx.h"
#include "AttributeList.h"

AttributeListFactory::
AttributeListFactory()
{
}

AttributeListFactory::
~AttributeListFactory()
{
}

void
AttributeListFactory::
put(const int& key, const int& val)
{
    entries[key] = val;
}

void
AttributeListFactory::
clear()
{
    entries.clear();
}

size_t
AttributeListFactory::
size() const
{
    return entries.size() * 2 + 1;
}

const std::vector<int>
AttributeListFactory::
make()
{
    std::vector<int> attribVector;

    for (auto itr = entries.begin();
        itr != entries.end();
        itr++)
    {
        attribVector.push_back(itr->first);
        attribVector.push_back(itr->second);
    }

    attribVector.push_back(0); // push 0 as AttributeList ending

    return attribVector;
}
