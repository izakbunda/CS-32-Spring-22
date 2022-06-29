#include "NameTable.h"
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

// %% ALL WORK IS BELOW THIS LINE %% //

struct dataBucket
{
    dataBucket( string id, int num, int scope ) : m_id(id), m_num(num), m_scope(scope), next(nullptr) {}
    string m_id;
    int m_num;
    int m_scope;
    dataBucket* next;
};

class NameTableImpl
{
public:
    NameTableImpl();
    ~NameTableImpl();
    
    void enterScope();
    bool exitScope();
    
    bool declare(const string& id, int num);
    int find(const string& id) const;
    
private:
    void insert(const string& id, int num, int scope, dataBucket* &record);
    unsigned int hashFunction(const string& id) const;
    
    int scopeNum = 0;
    vector<int> hashNum;
    
    dataBucket* m_buckets[19997];
};

NameTableImpl::NameTableImpl()
{
    for(int i = 0; i < 19996; i++)
    {
        m_buckets[i] = nullptr;
    }
}

unsigned int NameTableImpl::hashFunction(const string& id) const
{
    auto hash = 147;
    auto idSize = id.size();
    
    if ( idSize > 3 ) {
        for (int i = 0; i < 3; i++)
        {
            hash += id[i];
            hash *= 7;
        }
    }
    else {
        for (int i = 0; i < id.size(); i++)
        {
            hash += id[i];
            hash *= 7;
        }
    }
   
    hash = hash % 19997;
    return hash;
}

NameTableImpl::~NameTableImpl()
{
    hashNum.erase(hashNum.begin(),hashNum.end());
}

void NameTableImpl::insert(const string& id, int num, int scope, dataBucket* &record)
{
    int x = hashFunction(id);
    dataBucket* p = m_buckets[x]; // point p to location of bucket
    m_buckets[x] = new dataBucket(id, num , scope); // actually create the bucket with data
    m_buckets[x]->next = p; // point it back to 
}

void NameTableImpl::enterScope()
{
    scopeNum++;
    hashNum.push_back(-1); // entering a scope is represented by a -1
}

bool NameTableImpl::exitScope()
{
    if ( scopeNum == 0 ) {
        return false;
    }
    
    int x = hashNum.back();
    
    while (x != -1)
    {
        dataBucket *p = m_buckets[x];
        
        if (m_buckets[x] != nullptr)
        {
            m_buckets[x] = m_buckets[x]->next;
            delete p;
        }
        
        hashNum.pop_back();
        x = hashNum.back();
    }
    
    hashNum.pop_back();
    scopeNum--;
    return true;
}

bool NameTableImpl::declare(const string& id, int num)
{
    if ( id == "" ) {
        return false;
    }
    
    int currScopeNum = scopeNum;
    dataBucket* p = m_buckets[hashFunction(id)];
    
    // if found in the same scope:
    if ( (p != nullptr) && (p->m_scope == scopeNum) && (p->m_id == id) ) {
        return false;
    }
    // otherwise, declare it
    else {
        dataBucket *q;
        insert(id, num, currScopeNum, q);
        hashNum.push_back( hashFunction(id) );
        return true;
    }
}

int NameTableImpl::find(const string& id) const
{
    if ( id.empty() ) {
        return -1;
    }
    
    int scope = 0, num = -1, x = hashFunction(id);
    dataBucket* p = m_buckets[x];
    
    while ( p != nullptr ) {
        
        if ( p->m_id == id ) { // found it
            num = p->m_num;
            scope = p->m_scope;
            break;
        }
        
        p = p->next;
    }
    return num;
}

// %% ALL WORK IS ABOVE THIS LINE %% //

//*********** NameTable functions **************

// For the most part, these functions simply delegate to NameTableImpl's
// functions.

NameTable::NameTable()
{
    m_impl = new NameTableImpl;
}

NameTable::~NameTable()
{
    delete m_impl;
}

void NameTable::enterScope()
{
    m_impl->enterScope();
}

bool NameTable::exitScope()
{
    return m_impl->exitScope();
}

bool NameTable::declare(const string& id, int lineNum)
{
    return m_impl->declare(id, lineNum);
}

int NameTable::find(const string& id) const
{
    return m_impl->find(id);
}
