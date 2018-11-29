#include <montador_functions.hpp>

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
        arquivoSaida = argv[2];
    }
    tabelas tabelas = passo1(argv[1]);
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
    passo2(argv[1], arquivoSaida, tabelas);
    return 0;
}
