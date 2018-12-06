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

typedef struct variaveis
{
    int addr;
    string nome;
} variavies;
vector<variavies> variaveis;

int verificaNomesIguais(vector<simbolo> tabelaA, vector<simbolo> tabelaB)
{
    bool duplicado = false;
    for (int i = 0; i < tabelaA.size(); i++)
    {
        for (int j = 0; j < tabelaB.size(); j++)
        {
            if (tabelaA[i].nome == tabelaB[j].nome)
            {
                duplicado = true;
                cout << "ERRO:variavel " << tabelaA[i].nome << " com msm nome usada em dois arquivos" << endl;
            }
        }
    }
    return duplicado;
}

int verificaNaoDeclarados(vector<externa> tabelaA, vector<global> tabelaB)
{
    bool achou = false;
    for (int i = 0; i < tabelaA.size(); i++)
    {
        achou = false;
        for (int j = 0; j < tabelaA.size(); j++)
        {
            if (tabelaA[i].nome == tabelaB[j].nome)
            {
                achou = true;
            }
        }
    }
    if (!achou)
    {
        return true;
    }
    return false;
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

        //verificar as nTabelas
        cout << "numero de tabelas:" << progs.size() << endl;
        bool duplicado = false;
        for (int i = 0; i < progs.size(); i++)
        {
            for (int j = i + 1; j < progs.size(); j++)
            {
                cout << "comparadando prog:" << i << " e prog:" << j << endl;
                if (verificaNomesIguais(progs[i].tabela_simbolos, progs[j].tabela_simbolos))
                {
                    duplicado = true;
                }
            }
        }
        if (duplicado)
        {
            cout << "Não foi possivel gerar arquivo, devido aos erros" << endl;
            return 0;
        }
        //monta tabela 'mestre'
        tabelas tabela_mestre;
        simbolo tmpS;
        int addr = 0;
        for (int i = 0; i < progs.size(); i++)
        {
            for (int j = 0; j < progs[i].tabela_simbolos.size(); j++)
            {
                progs[i].tabela_simbolos[j].addr = addr;
                tabela_mestre.tabela_simbolos.push_back(progs[i].tabela_simbolos[j]);
                addr++;
            }
            for (int k = 0; k < progs[i].tabela_global.size(); k++)
            {
                tmpS.addr = addr;
                tmpS.nome = progs[i].tabela_global[k].nome;
                tabela_mestre.tabela_simbolos.push_back(tmpS);
                addr++;
            }
        }
        cout << "tabela_mestre" << endl;
        mostra_tabelas(tabela_mestre);
    }
    if (argc == 2)
    {
        tabelas tabelas = passo1(argv[1]);
        mostra_tabelas(tabelas);
        passo2(argv[1], arquivoSaida, tabelas);
    }
}