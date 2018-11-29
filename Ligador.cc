#include <montador_functions.hpp>

void mostra_tabelas()
{
    cout << "variaveis" << endl;
    for (int i = 0; i < tabela_simbolos.size(); i++)
    {
        cout << "addr:" << tabela_simbolos[i].addr << " nome:" << tabela_simbolos[i].nome << endl;
    }
    cout << "externas" << endl;
    for (int i = 0; i < tabela_extern.size(); i++)
    {
        cout << "nome:" << tabela_extern[i].nome << endl;
    }
    cout << "globais" << endl;
    for (int i = 0; i < tabela_global.size(); i++)
    {
        cout << "nome:" << tabela_global[i].nome << endl;
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
        arquivoSaida = argv[2];
    }
    passo1(argv[1]);
    mostra_tabelas();
}