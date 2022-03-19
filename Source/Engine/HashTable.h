#pragma once
#include "Types.h"
#include "HashFunctions.h"

#include <assert.h>
#include <string.h>
#include <initializer_list>
#include <utility>

template<typename K, typename V>
struct HashNode {
    K key;
    V value;
    HashNode* next;

    HashNode(K key, V value): key(key), value(value), next(nullptr) { }
};

template <typename K, typename V>
class HashTable {
private:
    void* m_data = nullptr;
    uint32 m_bucketCount = 0;
    uint32 m_size = 0;

    HashNode<K, V>** m_buckets = nullptr;
    HashNode<K, V>* m_nextEmpty = nullptr;

    void Realloc();
    HashNode<K, V>* GetNextEmpty();
    uint32 GetBucket(K key) const;
    HashNode<K, V>* GetNode(K key) const;

    //friend class HashTableIterator;

public:
    class HashTableIterator {
    private:
        const HashTable<K, V>& data;
        uint32 index;
        int32 bucketIndex;
        HashNode<K, V>* node;

        void FindNextNode() {
            if (node != nullptr && node->next != nullptr) {
                node = node->next;
                return;
            }

            while (bucketIndex < (int32)data.m_bucketCount) {
                // Try next bucket
                ++bucketIndex;
                if (data.m_buckets[bucketIndex] != nullptr) {
                    node = data.m_buckets[bucketIndex];
                    return;
                }
            }
            node = nullptr;
        }

    public:
        HashTableIterator(const HashTable<K, V>& data, uint32 index) :
            data(data),
            index(index)
        {
            assert(index <= data.m_size);
            node = nullptr;

            if (index == data.m_size) {
                bucketIndex = data.m_bucketCount;
                return;
            }

            bucketIndex = -1;
            uint32 currIndex = 0;
            while (currIndex <= index) {
                FindNextNode();
                ++currIndex;
            }
        }

        const HashNode<K,V>& operator*() { return *node; }
        const HashNode<K, V>* operator->() { return node; }

        HashTableIterator& operator++() { ++index; FindNextNode(); return *this; }
        friend bool operator==(const HashTableIterator& a, const HashTableIterator& b)
        {
            return &a.data == &b.data && a.index == b.index;
        }
        friend bool operator!=(const HashTableIterator& a, const HashTableIterator& b)
        {
            return !(a == b);
        }
    };

    HashTable() : m_size(5) { 
        Realloc();
    }
    HashTable(uint32 preallocation) : m_size(preallocation / 2 + 1) {
        Realloc();
    }
    HashTable(std::initializer_list<std::pair<K, V>> list) : m_size(list.size() / 2 + 1) {
        Realloc();
        for (const auto& pair : list) {
            (*this)[pair.first] = pair.second;
        }
    }
    HashTable(HashTable<K, V>&& copy) {
        *this = std::move(copy);
    }
    HashTable(const HashTable<K, V>& copy) {
        *this = copy;
    }
    ~HashTable() {
        delete[] m_data;
    }

    HashTable<K, V>& operator=(const HashTable<K, V>& rhs)
    {
        Clear();
        m_size = rhs.m_bucketCount; // Realloc same size
        Realloc();
        for (const auto& pair : rhs) {
            (*this)[pair.key] = pair.value;
        }
        return *this;
    }

    HashTable<K, V>& operator=(HashTable<K, V>&& rhs)
    {
        m_data = rhs.m_data;
        m_bucketCount = rhs.m_bucketCount;
        m_size = rhs.m_size;
        m_buckets = rhs.m_buckets;
        m_nextEmpty = rhs.m_nextEmpty;

        rhs.m_data = nullptr;
        rhs.m_buckets = nullptr;
        rhs.m_nextEmpty = nullptr;

        return *this;
    }

    HashTableIterator begin() const { return HashTableIterator(*this, 0); }
    HashTableIterator end() const { return HashTableIterator(*this, m_size); }

    uint32 Size() const { return m_size; }
    bool Empty() const { return m_size == 0; }

    bool Contains(K key) const;
    const V& operator[](K key) const;
    V& operator[](K key);
    void Delete(K key);

    void Clear();
};

template <typename K, typename V>
void HashTable<K, V>::Realloc() {
    uint32 newSize = m_size * 2;
    void* oldData = m_data;
    HashNode<K, V>* oldNodes = nullptr;
    for (uint32 i = 0; i < m_bucketCount; ++i) {
        while (m_buckets[i] != nullptr) {
            auto node = m_buckets[i];
            m_buckets[i] = node->next;
            node->next = oldNodes;
            oldNodes = node;
        }
    }

    // Store buckets first and then nodes
    uint32 byteSize = newSize * sizeof(HashNode<K, V>*) + newSize * sizeof(HashNode<K, V>);
    void* m_data = new void* [byteSize];
    memset(m_data, 0, byteSize);
    m_size = 0;
    m_bucketCount = newSize;
    m_buckets = static_cast<HashNode<K, V>**>(m_data);
    m_nextEmpty = nullptr;
    //blerg
    HashNode<K, V>* nodeStart = static_cast<HashNode<K, V>*>(static_cast<void*>(m_buckets + newSize));

    // Alloc new nodes
    for (uint32 i = 0; i < newSize; ++i) {
        auto node = nodeStart + i;
        node->next = m_nextEmpty;
        m_nextEmpty = node;
    }

    while (oldNodes != nullptr) {
        auto node = oldNodes;
        oldNodes = oldNodes->next;
        (*this)[node->key] = node->value;
    }

    delete[] oldData;
}

template <typename K, typename V>
HashNode<K, V>* HashTable<K, V>::GetNextEmpty() {
    if (m_nextEmpty == nullptr) {
        Realloc();
    }

    auto node = m_nextEmpty;
    m_nextEmpty = node->next;
    
    ++m_size;

    return node;
}

template <typename K, typename V>
uint32 HashTable<K, V>::GetBucket(K key) const {
    return HashFunc(key) % m_bucketCount;
}

template <typename K, typename V>
HashNode<K, V>* HashTable<K, V>::GetNode(K key) const {
    uint32 bucketIndex = GetBucket(key);
    auto node = m_buckets[bucketIndex];
    while (node != nullptr && node->key != key) {
        node = node->next;
    }

    return node;
}

template <typename K, typename V>
bool HashTable<K, V>::Contains(K key) const {
    return GetNode(key) != nullptr;
}

template <typename K, typename V>
const V& HashTable<K, V>::operator[](K key) const
{
    assert(Contains(key));
    auto node = GetNode(key);
    return node->value;
}

template <typename K, typename V>
V& HashTable<K, V>::operator[](K key)
{
    auto node = GetNode(key);
    if (node == nullptr) {
        node = GetNextEmpty();
        // Do after GetNextEmpty, may have resized
        uint32 bucketIndex = GetBucket(key);
        node->key = key;
        node->next = m_buckets[bucketIndex];
        m_buckets[bucketIndex] = node;
    }

    return node->value;
}

template <typename K, typename V>
void HashTable<K, V>::Delete(K key) {
    uint32 bucketIndex = GetBucket(key);
    auto node = m_buckets[bucketIndex];

    if (node == nullptr) {
        // Not found
        return;
    }

    // If node is head, set the new head
    if (node != nullptr && node->key == key) {
        m_buckets[bucketIndex] = node->next;

        node->next = m_nextEmpty;
        m_nextEmpty = node;
        node->value.~V();

        --m_size;
        return;
    }

    // Find the node in the list
    auto prev = node;
    node = node->next;
    while (node != nullptr && node->key != key) {
        node = node->next;
        prev = prev->next;
    }
    if (node == nullptr) {
        // Not found
        return;
    }

    prev->next = node->next;
    node->next = m_nextEmpty;
    m_nextEmpty = node;
    node->value.~V();

    --m_size;
}

template <typename K, typename V>
void HashTable<K, V>::Clear() {
    for (uint32 i = 0; i < m_bucketCount; ++i) {
        while (m_buckets[i] != nullptr) {
            auto node = m_buckets[i];
            m_buckets[i] = node->next;

            node->next = m_nextEmpty;
            m_nextEmpty = node;
            node->value.~V();
        }
    }

    m_size = 0;
}