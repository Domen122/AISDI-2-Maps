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
  using ListIterator = typename HashList<const key_type, mapped_type>
                                    ::Iterator;
  using constListIterator = typename HashList<const key_type, mapped_type>
                                    ::ConstIterator;
  using Hashlist = HashList<const KeyType,  ValueType>;
  const static size_type arraySize = 16000;
  size_type size, firstIndex, lastIndex;
  Hashlist Array[arraySize];

size_type getNextIndex(size_type ListIndex) const
{
    for (unsigned int i = ListIndex + 1; i < arraySize; ++i)
    {
        if( !Array[i].isEmpty() )
        return i;
    }
    return 0;
}
size_type getPrevIndex(size_type ListIndex) const
{
    for (int i = ListIndex - 1; i >= 0; --i)
    {
        if( !Array[i].isEmpty() )
        return i;
    }
    return 0;
}
void swap(HashMap& other)
        {
            std::swap(size, other.size);
            std::swap(firstIndex, other.firstIndex);
            std::swap(lastIndex, other.lastIndex);
            for(unsigned int i=0;i<arraySize;++i)
            {
                Array[i].swap(other.Array[i]);
            }
        }
public:

  HashMap():size(0),firstIndex(15999),lastIndex(0)
  {}

  HashMap(std::initializer_list<value_type> list):HashMap()
  {
    for(auto iter: list)
    {
        this->operator[](iter.first)=iter.second;
    }
  }

  HashMap(const HashMap& other): HashMap()
  {
    for(auto iter=other.begin();iter!=other.end();++iter) this->operator[](iter->first)=iter->second;
  }

  HashMap(HashMap&& other):HashMap()
  {
    swap(other);
  }

  HashMap& operator=( HashMap other)
  {
    swap(other);
    return *this;
  }



  bool isEmpty() const
  {
    return size==0;
  }

  mapped_type& operator[](const key_type &key)
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
    auto iter=find(key);
    if (  iter == end() ) throw std::out_of_range("No such element");
    return iter->second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    auto iter=find(key);
    if (  iter == end() ) throw std::out_of_range("No such element");
    return iter->second;
  }


  const_iterator findKey(const key_type &key) const
  {
    if( isEmpty())return end();
    std::hash<size_type> hash;
    size_type hashedIndex = hash(key) % arraySize;
    for ( auto it = Array[hashedIndex].begin(); it != Array[hashedIndex].end(); ++it)
    {
        if( it->first == key )
        return const_iterator( it, hashedIndex, *this);
    }
    return this->end();
  }

  const_iterator find(const key_type& key) const
  {
        return findKey(key);
  }
  iterator find(const key_type& key)
  {
    return findKey(key);
  }

  void remove(const key_type& key)
  {
    if( isEmpty() ) throw std::out_of_range("Empty Map");
    auto iter=find(key);
    if( iter == end() ) throw std::out_of_range("No node of given key");
    std::hash<size_type> hash;
    size_type hashedIndex = hash(key) % arraySize;
    Array[hashedIndex].remove(key);
    --size;
  }

  void remove(const const_iterator& it)
  {
    remove (it->first);
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const HashMap& other) const
  {
    if(size!=other.size)return false;
    for(auto iter=begin(),otheriter=other.begin();iter!=end();++iter,++otheriter)
    {
        if(iter->first!=otheriter->first || iter->second!=otheriter->second)return false;
    }
    return true;
  }

  bool operator!=(const HashMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    ListIterator Iter= Array[firstIndex].begin();
    return iterator(Iter, firstIndex, *this);
  }

  iterator end()
  {

    if(isEmpty())return begin();
    ListIterator Iter= Array[lastIndex].end();
    return Iterator(Iter, lastIndex, *this);
  }

  const_iterator cbegin() const
  {

    constListIterator Iter= Array[firstIndex].begin();
    return const_iterator(Iter, firstIndex, *this);
  }

  const_iterator cend() const
  {
    if(isEmpty())return cbegin();
    constListIterator Iter =Array[lastIndex].end();
    return const_iterator(Iter, lastIndex, *this);
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
  using iterator =  typename HashList<const key_type, mapped_type>
                                    ::ConstIterator;


  iterator *IterPoint;
  size_type hashIndex;
  const HashMap &Map;

  explicit ConstIterator(iterator otherIter, size_type otherIndex,const HashMap& otherMap):
        IterPoint(new iterator(otherIter)),
        hashIndex(otherIndex),
        Map(otherMap){}

  ConstIterator(const ConstIterator& other):IterPoint(new iterator(*other.IterPoint)),hashIndex(other.hashIndex),Map(other.Map)
  {
  }
    ~ConstIterator()
    {
        delete IterPoint;
    }
  ConstIterator& operator++()
  {
    if(*IterPoint==Map.Array[Map.lastIndex].end()) throw std::out_of_range("Cannot increment end");
    if(*IterPoint==--Map.Array[hashIndex].end() && (hashIndex!=Map.lastIndex))
    {
        auto nextIndex = Map.getNextIndex(hashIndex);
        delete IterPoint;
        IterPoint = new iterator(Map.Array[nextIndex].begin());
    }
    else ++(*IterPoint);
    return *this;
  }

  ConstIterator operator++(int)
  {
    ConstIterator tmp(*this);
    ++(*this);
    return tmp;
  }

  ConstIterator& operator--()
  {
        if ( *IterPoint == Map.Array[Map.firstIndex].begin() )
        throw std::out_of_range("Can't decrement BEGIN");
        if( *IterPoint == Map.Array[hashIndex].begin() )
        {
            auto prevIndex = Map.getPrevIndex(hashIndex);
            delete IterPoint;
            IterPoint = new iterator(Map.Array[prevIndex].end());
        }
        else  --(*IterPoint);
        return *this;
  }

  ConstIterator operator--(int)
  {
    ConstIterator tmp(*this);
    --(*this);
    return tmp;
  }

  reference operator*() const
  {
    if(*IterPoint==Map.Array[Map.lastIndex].end())throw std::out_of_range("Cannot dereference end");
    return **IterPoint;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return *IterPoint==*other.IterPoint;
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
  using iterator =  typename HashList<const key_type, mapped_type>
                                    ::ConstIterator;

  explicit Iterator(iterator& otherIter, size_type otherHashIndex,HashMap& map):ConstIterator(otherIter, otherHashIndex, map)
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
