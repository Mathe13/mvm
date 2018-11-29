#include <montador_functions.hpp>

void mostra_tabelas(tabelas tabelas)
{
    cout << "variaveis" << endl;
    for (int i = 0; i < tabelas.tabela_simbolos.size(); i++)
    {
        cout << "addr:" << tabelas.tabela_simbolos[i].addr << " nome:" << tabelas.tabela_simbolos[i].nome << endl;
    }
    cout << "externas" << endl;
    for (int i = 0; i < tabelas.tabela_extern.size(); i++)
    {
        cout << "nome:" << tabelas.tabela_extern[i].nome << endl;
    }
    cout << "globais" << endl;
    for (int i = 0; i < tabelas.tabela_global.size(); i++)
    {
        cout << "nome:" << tabelas.tabela_global[i].nome << endl;
    }
}
int main(int argc, char *argv[])
{
    string arquivoSaida = "saida";
    if (argc < 2)
    {
        cout << "Digite o nome de um arquivo texto para monta-lo" << endl;
        return 0;
    }
    if (argc >= 3)
    {
        tabelas tmp;
        vector<tabelas> progs;
        for (int i = 1; i < argc; i++)
        {
            tmp = passo1(argv[i]);
            progs.push_back(tmp);
        }
        for (int i = 0; i < progs.size(); i++)
        {
            cout << "tabela:" << i << endl;
            mostra_tabelas(progs[i]);
        }
    }
    if (argc == 2)
    {
        tabelas tabelas = passo1(argv[1]);
        mostra_tabelas(tabelas);
        passo2(argv[1], arquivoSaida, tabelas);
    }
}