#include <iostream>
#include "hashtable.hpp"

struct Elemento
{
    std::string nome;
    std::string categoria;
    std::string campo_extra;
};

int id = 0;
int gerar_id() { return id++; }

int main()
{
    ac::HashTable<int, Elemento> tabela;
    int n;

    std::cout << "\nBem-vindo ao gerenciamento de tarefas!\n";
    do
    {
        std::cout << "\nO que deseja fazer?!\n"
                  << "1 - Inserir um novo elemento\n"
                  << "2 - Remover elemento pelo ID\n"
                  << "3 - Buscar elemento por ID\n"
                  << "4 - Exibir todos os elementos da tabela\n"
                  << "5 - Apagar tabela\n"
                  << "0 - Sair\n\n";
        std::cin >> n;

        switch (n)
        {
        case 1:
            tarefas.push_back(criarTarefa());
            break;
        case 2:
        {
            int id;
            std::cout << "Digite o ID da tarefa a remover: ";
            std::cin >> id;
            tarefas.removerPorID(id);
            break;
        }
        case 3:
        {
            int prio;
            std::cout << "Digite a prioridade a remover: ";
            std::cin >> prio;
            tarefas.removerPorPrio(prio);
            break;
        }
        case 4:
        {
            int id;
            std::cout << "Digite o ID da tarefa a buscar: ";
            std::cin >> id;
            tarefas.buscarPorId(id);
            break;
        }
        case 5:
        {
            tarefas.exibirTarefas();
            break;
        }
        case 6:
        {
            char c;
            std::cout << "Tem certeza? Digite 'S' para Sim ou 'N' para Não.\n";
            std::cin >> c;
            if (c == 'S' || c == 's')
                tarefas.clear();
            break;
        }
        case 0:
        {
            std::cout << "Encerrando o programa.\n";
            break;
        }
        default:
            std::cout << "Opção inválida.\n";
        }
    } while (n != 0);
    

        return 0;
}