#ifndef HASHTBL_HPP
#define HASHTBL_HPP

#include <forward_list> // forward_list
#include <initializer_list>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>

namespace ac
{
    template <class KeyType, class DataType>
    struct HashEntry
    {
        KeyType m_key;
        DataType m_data;

        HashEntry(KeyType key_type, DataType data_type)
            : m_key{key_type}, m_data{data_type} {}

        friend std::ostream &operator<<(std::ostream &os, const HashEntry &hash_entry)
        {
            // os_ << "{" << he_.m_key << "," << he_.m_data << "}";
            os << hash_entry.m_data;
            return os;
        }
    };

    

    template <class KeyType,
              class DataType,
              class KeyHash = std::hash<KeyType>,
              class KeyEqual = std::equal_to<KeyType>>
    class HashTable
    {
    public:
        // Aliases
        using entry_type = HashEntry<KeyType, DataType>;
        using list_type = std::forward_list<entry_type>;
        using size_type = std::size_t;

        explicit HashTable(size_type table_sz = default_size)
            : m_size(find_next_prime(table_sz)), m_count(0), m_max_load_factor(1.0),
              m_table(std::make_unique<list_type[]>(m_size)) {}

        HashTable(const HashTable &source)
            : m_size(source.m_size), m_count(source.m_count), m_max_load_factor(source.m_max_load_factor),
              m_table(std::make_unique<list_type[]>(source.m_size))
        {
            // Copia os elementos da tabela hash
            for (size_type i{0}; i < m_size; ++i)
            {
                m_table[i] = source.m_table[i]; // Copia as listas da tabela
            }
        }

        ~HashTable() { m_table.reset(); }

        [[nodiscard]] size_type size() { return m_count; }

        bool insert(const KeyType &key, const DataType &new_data)
        {
            if (float(m_count) / m_size > m_max_load_factor)
            {
                rehash();
            }
            size_t index = KeyHash{}(key) % m_size;
            for (auto &entry : m_table[index])
            {
                if (KeyEqual{}(entry.m_key, key))
                {
                    entry.m_data = new_data; // Atualiza o valor associado à chave.
                    return false;            // Foi uma atualização, não uma nova inserção.
                }
            }
            m_table[index].emplace_front(key, new_data);
            ++m_count;
            return true; // Nova inserção.
        }

        bool retrieve(const KeyType &key, DataType &data_item)
        {
            size_t index = KeyHash{}(key) % m_size;

            for (auto &entry : m_table[index])
            {
                if (KeyEqual{}(entry.m_key, key))
                {
                    data_item = entry.m_data;
                    return true; // Retorna o valor da chave.
                }
            }
            return false; // Quando nao encontra a chave
        }

        void rehash()
        {
            size_t new_size = find_next_prime(m_size * 2);
            auto new_table = std::make_unique<list_type[]>(new_size);
            // Rehashing: reinserir todos os elementos nas novas listas.
            for (size_type i{0}; i < m_size; ++i)
            {
                for (auto &entry : m_table[i])
                {
                    size_t new_index = KeyHash{}(entry.m_key) % new_size;
                    new_table[new_index].emplace_front(entry.m_key, entry.m_data);
                }
            }
            // Atualiza a tabela com a nova tabela.
            m_table = std::move(new_table);
            m_size = new_size;
        }

        bool erase(const KeyType &key)
        {
            size_t index = KeyHash{}(key) % m_size;
            auto &bucket = m_table[index];

            if (bucket.empty())
                return false;

            auto prev = bucket.before_begin();
            for (auto it = bucket.begin(); it != bucket.end(); ++it)
            {
                if (KeyEqual{}(it->m_key, key))
                {
                    bucket.erase_after(prev); // Remove o item.
                    --m_count;                // Decrementa o contador de elementos.
                    return true;
                }
                ++prev; // Atualiza o iterador anterior.
            }
            return false; // Chave não encontrada.
        }

        DataType &at(const KeyType &key)
        {
            size_t index = KeyHash{}(key) % m_size;

            for (auto &entry : m_table[index])
            {
                if (KeyEqual{}(entry.m_key, key))
                {
                    return entry.m_data; // Retorna o valor associado à chave.
                }
            }
            throw std::out_of_range("Tá fora do range");
        }

    private:
        static std::size_t find_next_prime(size_type starting_value)
        {
            auto is_prime = [](size_type n)
            {
                if (n < 2)
                    return false;
                for (size_type i{2}; i <= std::sqrt(n); ++i)
                {
                    if (n % i == 0)
                        return false;
                }
                return true;
            };

            while (!is_prime(starting_value))
            {
                ++starting_value;
            }

            return starting_value;
        }
        size_type m_size;           //!< Tamanho da tabela.
        size_type m_count;          //!< Numero de elementos na tabel.
        float m_max_load_factor{1}; //!< Load factor.
                                    // std::unique_ptr<std::forward_list<entry_type>[]> m_table;
        std::unique_ptr<list_type[]> m_table;
        // std::list< entry_type > *mp_Table; //!< Tabela de listas para entradas de tabela.
        static const short default_size{10};
    };
}

#endif