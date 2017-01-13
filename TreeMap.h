#ifndef AISDI_MAPS_TREEMAP_H
#define AISDI_MAPS_TREEMAP_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>
#include <iterator>
#include <iostream>

namespace aisdi
{

    template <typename KeyType, typename ValueType>
    class Node
    {
    public:
        std::pair<const KeyType,ValueType> data;
        Node *Parent, *Right, *Left;
        Node():Parent(nullptr), Right(nullptr), Left(nullptr){}
        Node(KeyType key, ValueType value):data(key, value), Parent(nullptr), Right(nullptr), Left(nullptr)
        {}

    };
template <typename KeyType, typename ValueType>
 Node<const KeyType,  ValueType>* minNode(Node< const KeyType,  ValueType> *SubTree)
  {
    Node<const KeyType,  ValueType>* Temp = SubTree;
    while(Temp!=nullptr &&Temp->Left!=nullptr)Temp=Temp->Left;
    return Temp;
  }
  template <typename KeyType, typename ValueType>
  Node<const KeyType,  ValueType>* maxNode(Node<const KeyType,  ValueType> *SubTree)
  {
    Node<const KeyType,  ValueType>* Temp = SubTree;
    while(Temp&&Temp->Right!=nullptr)Temp=Temp->Right;
    return Temp;
  }


template <typename KeyType, typename ValueType>
class TreeMap
{

public:

  using key_type = KeyType;
  using mapped_type = ValueType;
  using value_type = std::pair<const key_type, mapped_type>;
  using size_type = std::size_t;
  using reference = value_type&;
  using const_reference = const value_type&;
  using node= Node<const KeyType,  ValueType>;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;


  node *Root;
  size_type size;

  TreeMap():Root(nullptr),size(0)
  {
  }
  TreeMap(std::initializer_list<value_type> list): TreeMap()
  {
    for(auto ite= list.begin(); ite!=list.end();++ite)
    {
        this->operator[]((*ite).first)=(*ite).second;
    }
  }

  node *Copy(node *CpNode)
  {
    if(CpNode==nullptr)return nullptr;
    node *CreatedNode = new node(CpNode->data.first, CpNode->data.second);
    if(CpNode->Left!=nullptr)
    {
        CreatedNode->Left= Copy(CpNode->Left);
        CreatedNode->Left->Parent=CreatedNode;
    }
    if(CpNode->Right!=nullptr)
    {
        CreatedNode->Right= Copy(CpNode->Right);
        CreatedNode->Right->Parent=CreatedNode;
    }
    return CreatedNode;
  }

  TreeMap(const TreeMap& other)
  {
        size=other.size;
        Root= Copy(other.Root);
  }

  TreeMap(TreeMap&& other): TreeMap()
  {
    //if((*this)==other)return;
    std::swap(Root,other.Root);
    std::swap(size,other.size);
  }

  TreeMap& operator=(const TreeMap& other)
  {
    Root=other.Root;
    size=other.size;
    return *this;
  }

  TreeMap& operator=(TreeMap&& other)
  {
    std::swap(Root,other.Root);
    std::swap(size,other.size);
    other.Root=nullptr;
    other.size=0;
    return *this;
  }
  /*void CutTree(node *Cut)
  {
    if( Cut->Left != nullptr )  CutTree(Cut->Left);
    if( Cut->Right != nullptr ) CutTree(Cut->Right);
    delete Cut;
     }
 ~TreeMap()
{
    if(!isEmpty()) CutTree(Root);
}*/
  bool isEmpty() const
  {
    return(Root==nullptr);

  }

  mapped_type& operator[](const key_type& key)
  {
    node *Pointer = Root, *Previous=nullptr;
    bool IsFound=false;
    while(IsFound==false&&Pointer!=nullptr)
    {
        Previous=Pointer;
        if(key==Pointer->data.first) IsFound=true;
        else if(key<Pointer->data.first)Pointer=Pointer->Left;
        else if(key>Pointer->data.first)Pointer=Pointer->Right;
    }

    if(!IsFound)
    {
        ++size;
        Pointer = new node(key,ValueType{});
        Pointer->Parent= Previous;
        if(isEmpty())
        {
            Root=Pointer;
        }
        else
        {
            if(key<Previous->data.first) Previous->Left=Pointer;
            else if(key>Previous->data.first) Previous->Right=Pointer;
        }
    }
    return Pointer->data.second;
  }

  const mapped_type& valueOf(const key_type& key) const
  {
    ConstIterator Iter=find(key);
    if(Iter==end())throw std::out_of_range("There is no Node of given key");
    return Iter.Pointer->data.second;
  }

  mapped_type& valueOf(const key_type& key)
  {
    Iterator Iter=find(key);
    if(Iter==end())throw std::out_of_range("There is no Node of given key");
    return Iter.Pointer->data.second;
  }

  const_iterator find(const key_type& key) const
  {
    node *Pointerro=Root;
    if(isEmpty())return end();
    while (Pointerro!=nullptr && Pointerro->data.first != key)
    {
        if (key < Pointerro->data.first)
        {
            Pointerro = Pointerro->Left;
        }
        else
        {
            Pointerro = Pointerro->Right;
        }
    }
    return ConstIterator(Pointerro,Root);
  }

  iterator find(const key_type& key)
  {
    node *Pointerro=Root;
    if(isEmpty())return end();
    while (Pointerro!=nullptr && Pointerro->data.first != key)
    {
        if (key < Pointerro->data.first)
        {
            Pointerro = Pointerro->Left;
        }
        else
        {
            Pointerro = Pointerro->Right;
        }
    }
    return Iterator(Pointerro,Root);
  }

  void remove(const key_type& key)
  {
    if(isEmpty())throw std::out_of_range("The tree is empty!");
    Iterator Iter=find(key);
    if(Iter==end())throw std::out_of_range("No Node of given key");
    node *x=nullptr, *y=nullptr, *DeleteNode=Iter.Pointer;
    if (DeleteNode->Left==nullptr || DeleteNode->Right==nullptr)
		y=DeleteNode;
	else
		y=minNode(DeleteNode->Right);
	if (y->Left != nullptr)
		x=y->Left;
	else
		x=y->Right;
	if (x!=nullptr)  x->Parent = y->Parent;
	if (y->Parent == nullptr)  Root = x;
	else
    {
        if (y == y->Parent->Left)  y->Parent->Left = x;
		else y->Parent->Right = x;
    }
	if (y != DeleteNode)
    {
        if (DeleteNode==Root)
        {
            if(DeleteNode->Left!=nullptr)
            {
                DeleteNode->Left->Parent=y;
                y->Left=DeleteNode->Left;
            }
            if(DeleteNode->Right!=nullptr)
            {
                DeleteNode->Right->Parent=y;
                y->Right=DeleteNode->Right;
            }
            Root=y;
        }
        else
        {
        if(DeleteNode==DeleteNode->Parent->Left) DeleteNode->Parent->Left=y;
        else DeleteNode->Parent->Right=y;
        if(DeleteNode->Left!=nullptr)
        {
            y->Left=DeleteNode->Left;
            y->Left->Parent=y;
        }
        if(DeleteNode->Right!=nullptr)
        {
            y->Right=DeleteNode->Right;
            y->Right->Parent=y;
        }
        y->Parent=DeleteNode->Parent;
        }
    }
    DeleteNode=nullptr;
    --size;
  }
    void Nodecount()
    {
        int i=1;
        auto iter = Iterator(maxNode(Root),Root);
        auto it=begin();
        for(;it!=iter;++it,++i)
        {
        }
    }
  void remove(const const_iterator& it)
  {
    const KeyType &key = it->first;
    remove(key);
  }

  size_type getSize() const
  {
    return size;
  }

  bool operator==(const TreeMap& other) const
  {
    if(getSize()!=other.getSize())return false;
    for(auto iter=begin(),otheriter=other.begin();iter!=end();++iter,++otheriter)
    {
        if(*iter!=*otheriter)return false;
    }
    return true;
  }

  bool operator!=(const TreeMap& other) const
  {
    return !(*this == other);
  }

  iterator begin()
  {
    return Iterator(minNode(Root),Root);
  }

  iterator end()
  {
    if(isEmpty())return begin();
    return Iterator(nullptr,Root);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(minNode(Root),Root);
  }

  const_iterator cend() const
  {
    if(isEmpty()) return cbegin();
    return ConstIterator(nullptr,Root);
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
class TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::const_reference;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename TreeMap::value_type;
  using pointer = const typename TreeMap::value_type*;
  node *Pointer, *Root;
  explicit ConstIterator()
  {}
  ConstIterator(node *Pointerro, node *Rooterro):Pointer(Pointerro),Root(Rooterro)
  {}
  ConstIterator(const ConstIterator& other):Pointer(other.Pointer),Root(other.Root)
  {
  }

  ConstIterator& operator++()
  {
    if(Pointer==nullptr) throw std::out_of_range("Cannot increment end");
    if(Pointer->Right!=nullptr)
    {
        Pointer=Pointer->Right;
        while(Pointer->Left!=nullptr) Pointer=Pointer->Left;
        return(*this);
    }
    node* TmParent=Pointer->Parent;
    while(TmParent && TmParent->Left!=Pointer)
    {
        Pointer=TmParent;
        TmParent=TmParent->Parent;
    }
    Pointer= TmParent;
    return(*this);
  }

  ConstIterator operator++(int)
  {
    auto Prev= *this;
    ++(*this);
    return Prev;
  }

  ConstIterator& operator--()
  {
    if(Pointer == minNode(Root)) throw std::out_of_range("Cannot decrement begin");
    if(Pointer==nullptr)
    {
        Pointer= maxNode(Root);
        return *this;
    }

    if(Pointer->Left!=nullptr) Pointer=maxNode(Pointer->Left);
    else
    {
        node *Tmp= Pointer->Parent;
        while (Tmp && Tmp->Right != Pointer)
        {
            Pointer = Tmp;
            Tmp = Tmp->Parent;
        }
        Pointer=Tmp;
    }
    return *this;
  }

  ConstIterator operator--(int)
  {
    auto Prev = *this;
    --(*this);
    return Prev;
  }

  reference operator*() const
  {
    if(Pointer==nullptr) throw std::out_of_range("Cannot dereference end");
    return Pointer->data;
  }

  pointer operator->() const
  {
    return &this->operator*();
  }

  bool operator==(const ConstIterator& other) const
  {
    return(Pointer==other.Pointer);
  }

  bool operator!=(const ConstIterator& other) const
  {
    return !(*this == other);
  }
};

template <typename KeyType, typename ValueType>
class TreeMap<KeyType, ValueType>::Iterator : public TreeMap<KeyType, ValueType>::ConstIterator
{
public:
  using reference = typename TreeMap::reference;
  using pointer = typename TreeMap::value_type*;

  explicit Iterator()
  {}

  Iterator(node *Pointerro, node *Rooterro):ConstIterator(Pointerro,Rooterro){}

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

#endif /* AISDI_MAPS_MAP_H */
