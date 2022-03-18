#pragma once
#include "Types.h"
#include <assert.h>
#include <string.h>

template <typename T>
uint32 HashFunc(T value) {
    assert(false && "HashFunc not implemented");
    return 0;
}

template <>
inline uint32 HashFunc<uint32>(uint32 value) {
    return value;
}

// https://stackoverflow.com/questions/8317508/hash-function-for-a-string
template <>
inline uint32 HashFunc<const char *>(const char* value) {
    uint32 hash = 37, index = 0;
    while (value[index] != 0) {
        hash = (hash * 54059) ^ (value[index] * 76963);
        ++index;
    }
    return hash;
}

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

public:
    HashTable() : m_size(5) { 
        Realloc();
    }
    HashTable(uint32 preallocation) : m_size(preallocation / 2) {
        Realloc();
    }

    uint32 Size() const { return m_size; }

    bool Contains(K key) const;
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