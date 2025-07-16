#include <iostream>
#include <string>
#include "hashtable.hpp"

struct Elemento
{
    std::string nome;
    std::string categoria;
    std::string campo_extra;

    void exibir() const
    {
        std::cout << "Nome: " << nome << "\n";
        std::cout << "Categoria: " << categoria << "\n";
        std::cout << "Campo Extra: " << campo_extra << "\n";
    }
};

int id = 1;
int gerar_id() { return id++; }

Elemento criarElemento()
{
    Elemento e;
    std::cin.ignore();
    std::cout << "Digite o nome: ";
    std::getline(std::cin, e.nome);
    std::cout << "Digite a categoria: ";
    std::getline(std::cin, e.categoria);
    std::cout << "Digite o campo extra: ";
    std::getline(std::cin, e.campo_extra);
    return e;
}

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
                  << "6 - Editar elemento pelo ID (exceto o ID)\n"
                  << "0 - Sair\n\n";
        std::cin >> n;

        switch (n)
        {
        case 1:
        {
            Elemento e = criarElemento();
            int novo_id = gerar_id();
            tabela.insert(novo_id, e);
            std::cout << "Elemento inserido com ID: " << novo_id << "\n";
            break;
        }
        case 2:
        {
            int id;
            if (tabela.isEmpty())
                std::cout << "A tabela está vazia. Adicione elementos!\n";
            else
            {
                std::cout << "Digite o ID do elemento a remover: ";
                std::cin >> id;
                {
                    if (tabela.erase(id))
                        std::cout << "Elemento removido com sucesso.\n";
                    else
                        std::cout << "Elemento nao encontrado.\n";
                }
            }
            break;
        }
        case 3:
        {
            int id;
            if (tabela.isEmpty())
            {
                std::cout << "A tabela está vazia. Adicione elementos!\n";
                break;
            }
            std::cout << "Digite o ID do elemento a buscar: ";
            std::cin >> id;
            Elemento resultado;
            if (tabela.retrieve(id, resultado))
                resultado.exibir();
            else
                std::cout << "Elemento nao encontrado.\n";
            break;
        }
        case 4:
        {
            if (tabela.isEmpty())
            {
                std::cout << "A tabela está vazia. Adicione elementos!\n";
            }
            else
            {
                std::cout << "Elementos da tabela:\n";
                for (int i = 0; i < 100; ++i)
                {
                    try
                    {
                        Elemento &e = tabela.at(i);
                        std::cout << "ID: " << i << "\n";
                        e.exibir();
                        std::cout << "---------------------\n";
                    }
                    catch (const std::out_of_range &)
                    {
                        continue;
                    }
                }
            }
            break;
        }
        case 5:
        {
            char c;
            if (tabela.isEmpty())
            {
                std::cout << "A tabela já está vazia. Adicione elementos!\n";
                break;
            }
            std::cout << "Tem certeza que deseja apagar toda a tabela? (S/N): ";
            std::cin >> c;
            if (c == 'S' || c == 's')
            {
                tabela.clear();
                std::cout << "Tabela apagada com sucesso.\n";
            }
            break;
        }
        case 6:
        {
            int id;
            if (tabela.isEmpty())
            {
                std::cout << "A tabela está vazia. Adicione elementos!\n";
                break;
            }
            std::cout << "Digite o ID do elemento a editar: ";
            std::cin >> id;

            try
            {
                Elemento &e = tabela.at(id);

                int opcao_edit;
                std::cout << "\nQual campo deseja editar?\n"
                          << "1 - Nome\n"
                          << "2 - Categoria\n"
                          << "3 - Campo Extra\n"
                          << "Digite a opcao: ";
                std::cin >> opcao_edit;
                std::cin.ignore();

                switch (opcao_edit)
                {
                case 1:
                    std::cout << "Digite o novo nome: ";
                    std::getline(std::cin, e.nome);
                    break;
                case 2:
                    std::cout << "Digite a nova categoria: ";
                    std::getline(std::cin, e.categoria);
                    break;
                case 3:
                    std::cout << "Digite o novo campo extra: ";
                    std::getline(std::cin, e.campo_extra);
                    break;
                default:
                    std::cout << "Opcao invalida.\n";
                }

                std::cout << "Elemento atualizado com sucesso.\n";
            }
            catch (const std::out_of_range &)
            {
                std::cout << "Elemento com ID informado nao encontrado.\n";
            }
            break;
        }

        case 0:
        {
            std::cout << "Quantidade de colisões: " << tabela.qtdColisoes() << "\n";
            std::cout << "Encerrando o programa.\n";
            break;
        }
        default:
            std::cout << "Opcao invalida.\n";
        }
    } while (n != 0);

    return 0;
}
