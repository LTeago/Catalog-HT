#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cmath>

namespace ac
{

    template <typename KeyType, typename DataType>
    struct HashEntry
    {
        KeyType key;
        DataType value;

        HashEntry(const KeyType &k, const DataType &v)
            : key(k), value(v) {}
    };

    template <typename T>
    struct Node
    {
        T data;
        Node *next;

        Node(const T &d, Node *n = nullptr)
            : data(d), next(n) {}
    };

    template <typename KeyType, typename DataType>
    class HashTable
    {
    public:
        using Entry = HashEntry<KeyType, DataType>;
        using size_type = std::size_t;

        explicit HashTable(size_type table_size = default_size)
            : m_size(find_next_prime(table_size)), m_count(0)
        {
            m_table = new Node<Entry> *[m_size]();
        }

        ~HashTable()
        {
            clear();
            delete[] m_table;
        }

        bool isEmpty() const
        {
            return m_count == 0;
        }

        bool insert(const KeyType &key, const DataType &value)
        {
            size_t index = hash_function(key);
            Node<Entry> *current = m_table[index];
            while (current)
            {
                if (current->data.key == key)
                {
                    current->data.value = value;
                    return false;
                }
                current = current->next;
            }
            if (m_table[index] != nullptr)
            {
                ++m_colisoes;
            }

            Node<Entry> *new_node = new Node<Entry>(Entry(key, value), m_table[index]);
            m_table[index] = new_node;
            ++m_count;
            return true;
        }

        size_type qtdColisoes() const
        {
            return m_colisoes;
        }

        bool retrieve(const KeyType &key, DataType &result) const
        {
            size_t index = hash_function(key);
            Node<Entry> *current = m_table[index];

            while (current)
            {
                if (current->data.key == key)
                {
                    result = current->data.value;
                    return true;
                }
                current = current->next;
            }

            return false;
        }

        bool erase(const KeyType &key)
        {
            size_t index = hash_function(key);
            Node<Entry> *current = m_table[index];
            Node<Entry> *prev = nullptr;

            while (current)
            {
                if (current->data.key == key)
                {
                    if (prev)
                        prev->next = current->next;
                    else
                        m_table[index] = current->next;

                    delete current;
                    --m_count;
                    return true;
                }
                prev = current;
                current = current->next;
            }

            return false;
        }

        DataType &at(const KeyType &key)
        {
            size_t index = hash_function(key);
            Node<Entry> *current = m_table[index];

            while (current)
            {
                if (current->data.key == key)
                    return current->data.value;
                current = current->next;
            }

            throw std::out_of_range("Chave não encontrada.");
        }

        void clear()
        {
            for (size_type i = 0; i < m_size; ++i)
            {
                Node<Entry> *current = m_table[i];
                while (current)
                {
                    Node<Entry> *next = current->next;
                    delete current;
                    current = next;
                }
                m_table[i] = nullptr;
            }
            m_count = 0;
        }

    private:
        Node<Entry> **m_table;
        size_type m_size;
        size_type m_count;
        size_type m_colisoes = 0;

        static const size_type default_size = 10;

        size_type hash_function(const KeyType &key) const
        {
            return std::hash<KeyType>{}(key) % m_size;
        }

        static size_type find_next_prime(size_type n)
        {
            auto is_prime = [](size_type x)
            {
                if (x < 2)
                    return false;
                for (size_type i = 2; i <= std::sqrt(x); ++i)
                    if (x % i == 0)
                        return false;
                return true;
            };

            while (!is_prime(n))
                ++n;
            return n;
        }
    };

} // namespace ac

#endif // HASHTABLE_HPP