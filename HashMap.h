#ifndef AISDI_MAPS_HASHMAP_H
#define AISDI_MAPS_HASHMAP_H

#include "HashList.h"
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <list>
#include <functional>
#include <iostream>
#include <algorithm>

namespace aisdi
{
template <typename KeyType, typename ValueType>
class HashMap
{
private:
  using size_type = std::size_t;
  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;

  using reference = value_type&;
  using const_reference = const value_type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
  using Hashlist = HashList<const KeyType,  ValueType>;
  const static size_type arraySize = 16000;
  size_type size, firstIndex, lastIndex;
  Hashlist Array[arraySize];


public:






  HashMap():size(0)
  {}

  HashMap(std::initializer_list<value_type> list)
  {
    (void)list; // disables "unused argument" warning, can be removed when method is implemented.
    throw std::runtime_error("TODO");
  }

  HashMap(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  HashMap(HashMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  HashMap& operator=(const HashMap& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  HashMap& operator=(HashMap&& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool isEmpty() const
  {
    return size==0;
  }

  mapped_type& operator[](const key_type& key)
  {
    std::hash<size_type> hash;
    size_type hashIndex = hash(key) % arraySize;
    auto it=find(key);
    if(it!=end()) return it->second;
    else
    {
        Array[hashIndex].append(key, mapped_type{});
        if( hashIndex < firstIndex) firstIndex = hashIndex;
        if( lastIndex < hashIndex ) lastIndex = hashIndex;
        ++size;
        return (--Array[hashIndex].end())->second;
    }
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  mapped_type& valueOf(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  const_iterator find(const key_type& key) const
  {
    if( isEmpty())return end();
    std::hash<size_type> hash;
    size_type hashedIndex = hash(key) % arraySize;
    for ( auto it = Array[hashedIndex].begin(); it != Array[hashedIndex].end(); ++it)
    {
        if( it->first == key )
        return const_iterator(it, hashedIndex, *this);
    }
    return this->end();
  }

  iterator find(const key_type& key)
  {
    if( isEmpty())return end();
    std::hash<size_type> hash;
    size_type hashedIndex = hash(key) % arraySize;
    for ( auto it = Array[hashedIndex].begin(); it != Array[hashedIndex].end(); ++it)
    {
        if( it->first == key )
        return iterator(it, hashedIndex, *this);
    }
    return this->end();
  }

  void remove(const key_type& key)
  {
    (void)key;
    throw std::runtime_error("TODO");
  }

  void remove(const const_iterator& it)
  {
    (void)it;
    throw std::runtime_error("TODO");
  }

  size_type getSize() const
  {
    throw std::runtime_error("TODO");
  }

  bool operator==(const HashMap& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    throw std::runtime_error("TODO");
  }

  iterator end()
  {
    throw std::runtime_error("TODO");
  }

  const_iterator cbegin() const
  {
    throw std::runtime_error("TODO");
  }

  const_iterator cend() const
  {
    throw std::runtime_error("TODO");
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename HashMap::value_type;
  using pointer = const typename HashMap::value_type*;

  explicit ConstIterator()
  {}

  ConstIterator(const ConstIterator& other)
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  ConstIterator& operator++()
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator operator++(int)
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator& operator--()
  {
    throw std::runtime_error("TODO");
  }

  ConstIterator operator--(int)
  {
    throw std::runtime_error("TODO");
  }

  reference operator*() const
  {
    throw std::runtime_error("TODO");
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    (void)other;
    throw std::runtime_error("TODO");
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class HashMap<KeyType, ValueType>::Iterator : public HashMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename HashMap::reference;
  using pointer = typename HashMap::value_type*;

  explicit Iterator()
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif /* AISDI_MAPS_HASHMAP_H */
