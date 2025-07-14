#ifndef HASHTBL_HPP
#define HASHTBL_HPP

#include <iostream>
#include <string>
#include <cmath>
#include <iomanip>

namespace ac {

    // Entrada da tabela hash (chave + dados)
    template <class KeyType, class DataType>
    struct HashEntry {
        KeyType m_key;
        DataType m_data;

        HashEntry(KeyType key_type, DataType data_type)
            : m_key{key_type}, m_data{data_type} {}

        friend std::ostream& operator<<(std::ostream &os, const HashEntry &entry) {
            os << entry.m_data;
            return os;
        }
    };

    // Nó da lista encadeada (encadeamento externo)
    template <typename T>
    struct Node {
        T data;
        Node* next;

        Node(const T& d, Node* n = nullptr)
            : data(d), next(n) {}
    };

    template <class KeyType, class DataType>
    class HashTable {
    public:
        using Entry = HashEntry<KeyType, DataType>;
        using size_type = std::size_t;

        explicit HashTable(size_type table_sz = default_size)
            : m_size(find_next_prime(table_sz)), m_count(0) {
            m_table = new Node<Entry>*[m_size]();
        }

        ~HashTable() {
            clear();
            delete[] m_table;
        }

        bool insert(const KeyType &key, const DataType &data) {
            size_t index = hash(key);
            Node<Entry>* current = m_table[index];

            while (current) {
                if (current->data.m_key == key) {
                    current->data.m_data = data; // Atualiza
                    return false;
                }
                current = current->next;
            }

            Node<Entry>* new_node = new Node<Entry>(Entry(key, data), m_table[index]);
            m_table[index] = new_node;
            ++m_count;
            return true;
        }

        bool retrieve(const KeyType &key, DataType &result) {
            size_t index = hash(key);
            Node<Entry>* current = m_table[index];

            while (current) {
                if (current->data.m_key == key) {
                    result = current->data.m_data;
                    return true;
                }
                current = current->next;
            }
            return false;
        }

        bool erase(const KeyType &key) {
            size_t index = hash(key);
            Node<Entry>* current = m_table[index];
            Node<Entry>* prev = nullptr;

            while (current) {
                if (current->data.m_key == key) {
                    if (prev) prev->next = current->next;
                    else m_table[index] = current->next;

                    delete current;
                    --m_count;
                    return true;
                }
                prev = current;
                current = current->next;
            }
            return false;
        }

        DataType& at(const KeyType &key) {
            size_t index = hash(key);
            Node<Entry>* current = m_table[index];

            while (current) {
                if (current->data.m_key == key) {
                    return current->data.m_data;
                }
                current = current->next;
            }
            throw std::out_of_range("Chave não encontrada.");
        }

        void clear() {
            for (size_type i = 0; i < m_size; ++i) {
                Node<Entry>* current = m_table[i];
                while (current) {
                    Node<Entry>* to_delete = current;
                    current = current->next;
                    delete to_delete;
                }
                m_table[i] = nullptr;
            }
            m_count = 0;
        }

        void print() const {
            for (size_type i = 0; i < m_size; ++i) {
                std::cout << "[" << i << "]: ";
                Node<Entry>* current = m_table[i];
                if (!current) {
                    std::cout << "Empty\n";
                } else {
                    while (current) {
                        std::cout << current->data << " -> ";
                        current = current->next;
                    }
                    std::cout << "NULL\n";
                }
            }
        }

        size_type size() const { return m_count; }

    private:
        Node<Entry>** m_table;
        size_type m_size;
        size_type m_count;

        size_type hash(const KeyType& key) const {
            return std::hash<KeyType>{}(key) % m_size;
        }

        static size_type find_next_prime(size_type start) {
            auto is_prime = [](size_type n) {
                if (n < 2) return false;
                for (size_type i = 2; i <= std::sqrt(n); ++i)
                    if (n % i == 0) return false;
                return true;
            };
            while (!is_prime(start)) ++start;
            return start;
        }

        static const size_type default_size = 10;
    };

}

#endif
