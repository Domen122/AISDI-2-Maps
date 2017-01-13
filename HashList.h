
#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <stdexcept>
#include <utility>

namespace aisdi {
    template <typename KeyType, typename ValueType>
    class HashNode
    {
        using key_type = KeyType;
        using mapped_type = ValueType;
        using value_type = std::pair<const key_type, mapped_type>;

    public:
        value_type data;
        HashNode *next, *prev;

        HashNode(): data(key_type(), mapped_type())
        {}

        HashNode(key_type key, mapped_type value): data(key, value)
        {}

        HashNode(value_type otherData): data(otherData.first, otherData.second)
        {}
    };

    template <typename KeyType, typename ValueType>
    class HashList
    {
    private:
    public:
    void swap(HashList &other)
    {
        std::swap(size,other.size);
        std::swap(Head, other.Head);
        std::swap(Tail,other.Tail);
    }


        using key_type = KeyType;
        using mapped_type = ValueType;
        using value_type = std::pair<const key_type, mapped_type>;
        using size_type = std::size_t;
        using reference = value_type&;
        using const_reference = const value_type&;

        class ConstIterator;
        class Iterator;
        using const_iterator = ConstIterator;
        using iterator = Iterator;
        using Hashnode = HashNode<const key_type,  mapped_type>;

        size_type size;
        Hashnode *Head, *Tail;


        HashList():size(0),Head(nullptr),Tail(nullptr)
        {}

        HashList(const HashList& other) : HashList()
        {
            if ( !isEmpty() )
            {
                Hashnode *copied = other.Head;
                while( copied != nullptr )
                {
                    append(copied->data.first, copied->data.second);
                    copied = copied->next;
                }
            }
        }

        ~HashList()
        {
            if(!isEmpty())
            {
                Hashnode *ite=Head;
                while(ite!=nullptr)
                {
                    Hashnode *temp=ite->next;
                    delete ite;
                    ite=temp;
                }
            }
        }

        void append(const key_type &key, mapped_type value)
        {
            Hashnode *Pointer = new Hashnode(key,value);
            if(isEmpty())
            {
                Head = Tail = Pointer;
                Pointer->next=nullptr;
                Pointer->prev=nullptr;
            }
            else
            {
                Tail->next=Pointer;
                Pointer->prev=Tail;
                Tail=Pointer;
                Tail->next=nullptr;
            }
            ++size;
        }

        bool isEmpty() const
        {
            return size==0;
        }

        const_iterator begin() const
        {
            return const_iterator(Head, *this);
        }

        const_iterator end() const
        {
            if( isEmpty() )
                return begin();
            return const_iterator(nullptr, *this);
        }

        iterator begin()
        {
            return iterator(Head, *this);
        }

        iterator end()
        {
            if( isEmpty() )
                return begin();
            return iterator(nullptr, *this);
        }

        Hashnode *findNode(const key_type &key) const
        {
            Hashnode *Pointer = Head;
            while(Pointer!=nullptr)
            {
                if( Pointer->data.first == key ) return Pointer;
                Pointer = Pointer->next;
            }
            return nullptr;
        }
        void remove(const key_type &key)
        {
            Hashnode *Pointer=findNode(key);
            if(Pointer==nullptr)throw std::out_of_range("There is no Node of given key");
            if(Pointer==Tail) Tail=Tail->prev;
            if(Pointer==Head) Head=Head->next;
            if(Pointer->next!=nullptr)Pointer->next->prev=Pointer->prev;
            if(Pointer->prev!=nullptr)Pointer->prev->next=Pointer->next;
            delete Pointer;
            --size;
        }

        HashList& operator=(HashList other)
        {
            swap(other);
            return *this;
        }

        size_type getSize()
        {
            return size;
        }

    };
    template <typename KeyType, typename ValueType>
    class HashList<KeyType, ValueType>::ConstIterator
    {
        using reference = typename HashList::const_reference;
        using pointer = const typename HashList::value_type*;

        Hashnode *Pointer;
        const HashList& list;

    public:
        ConstIterator( Hashnode *otherPointer, const HashList& otherList ):
        Pointer(otherPointer),
        list(otherList)
        {}

        ConstIterator(const ConstIterator& other):
        Pointer(other.Pointer),
        list(other.list)
        {}

        ConstIterator& operator++()
        {
            if (Pointer == nullptr)
                throw std::out_of_range("Cannot increment end");
            Pointer = Pointer->next;

            return *this;
        }

        ConstIterator operator++(int)
        {
            ConstIterator temp = *this;
            ++(*this);

            return temp;
        }

        ConstIterator& operator--()
        {
            if( Pointer == list.Head ) throw std::out_of_range("Cannot decrement end");
            if(Pointer==nullptr)//Pointer points to an end
            {
                Pointer=list.Tail;
            }
            else    Pointer = Pointer->prev;
            return *this;
        }

        ConstIterator operator--(int)
        {
            Iterator temp = *this;
            --(*this);

            return temp;
        }

        pointer operator->() const
        {
            return &this->operator*();
        }

        reference operator*() const
        {
            if( Pointer == nullptr )
                throw std::out_of_range("Cannot dereference nullptr");

            return Pointer->data;
        }

        bool operator==(const ConstIterator& other) const
        {
            return Pointer == other.Pointer;
        }

        bool operator!=(const ConstIterator& other) const
        {
            return !(*this == other);
        }
    };
    template <typename KeyType, typename ValueType>
    class HashList<KeyType, ValueType>::Iterator : public HashList<KeyType, ValueType>::ConstIterator
    {
        using reference = typename HashList::reference;
        using pointer = typename HashList::value_type*;

    public:
        Iterator( Hashnode *otherNode, HashList& otherList ):
        ConstIterator(otherNode, otherList)
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

        reference operator*() const
        {
            // ugly cast, yet reduces code duplication.
            return const_cast<reference>(ConstIterator::operator*());
        }

        pointer operator->() const
        {
            return &this->operator*();
        }

    };
}


#endif

