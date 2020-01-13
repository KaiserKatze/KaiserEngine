#include "stdafx.h"
#include "AttributeList.h"

AttributeList::
AttributeList()
{
}

AttributeList::
~AttributeList()
{
}

void
AttributeList::
Put(const int& key, const int& val)
{
    entries[key] = val;
}

void
AttributeList::
Clear()
{
    entries.clear();
}

const std::vector<int>
AttributeList::
Make()
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
