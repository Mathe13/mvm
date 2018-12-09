#include <iostream>
#include <stack>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

typedef struct simbolo
{
    int addr;
    string nome;
} simbolo;

typedef struct label
{
    int addr;
    string nome;
} label;

typedef struct global
{
    int addr;
    string nome;
} global;

typedef struct externa
{
    string nome;
} externa;

typedef struct tabelas
{
    //tabelas
    vector<externa> tabela_extern;
    vector<global> tabela_global;
    vector<label> tabela_labels;
    vector<simbolo> tabela_simbolos;
} tabelas;

vector<string> separaLinha(string linha);
int rastreiaSimbolo(string nome_simbolo);
int rastreiaLabel(string nome_label);
tabelas passo1(string fileName);
void passo2(string fileName, string targetName);
int rastreiaGlobal(string nome_global);
int rastreiaExterno(string nome_global);

vector<string> separaLinha(string linha)
{
    vector<string> resultArray;
    bool espace = true;
    int separador = 0, len = linha.size();
    string tmp;

    for (int j = 0, k = 0; j < len; j++)
    {
        if (linha[j] == ' ')
        {
            string ch = linha.substr(k, j - k);
            k = j + 1;
            resultArray.push_back(ch);
        }
        if (j == len - 1)
        {
            string ch = linha.substr(k, j - k + 1);
            resultArray.push_back(ch);
        }
    }
    return resultArray;
}

int rastreiaSimbolo(string nome_simbolo, vector<simbolo> tabela_simbolos)
{
    for (int i = 0; i < tabela_simbolos.size(); i++)
    {
        if (tabela_simbolos[i].nome == nome_simbolo)
        {
            return i;
        }
    }
    return -1;
}

int rastreiaLabel(string nome_label, vector<label> tabela_labels)
{
    for (int i = 0; i < tabela_labels.size(); i++)
    {
        if (tabela_labels[i].nome == nome_label)
        {
            return i;
        }
    }
    return -1;
}

int rastreiaGlobal(string nome_global, vector<global> tabela_global)
{
    for (int i = 0; i < tabela_global.size(); i++)
    {
        if (tabela_global[i].nome == nome_global)
        {
            return i;
        }
    }
    return -1;
}

int rastreiaExterno(string nome_extern, vector<externa> tabela_extern)
{
    for (int i = 0; i < tabela_extern.size(); i++)
    {
        if (tabela_extern[i].nome == nome_extern)
        {
            return i;
        }
    }
    return -1;
}

void declaraLabel(vector<string> comando, int nLinha, vector<label> &tabela_labels)
{
    // cout<<<<endl;
    string labelName;
    labelName = comando[0].substr(0, comando[0].size() - 1);
    int a = rastreiaLabel(labelName, tabela_labels);
    label tmp;
    if (a != -1)
    {
        cout << "label " << comando[0] << " declarada multiplas vezes" << endl;
    }
    else
    {
        tmp.addr = nLinha;
        tmp.nome = labelName;
        tabela_labels.push_back(tmp);
        cout << "adicionando label:" << labelName << endl;
    }
}

void declaraExtern(vector<string> comando, externa tmp, vector<externa> &tabela_extern)
{
    if (comando[1] == "EXTERN")
    {
        if (rastreiaExterno(comando[0], tabela_extern) == -1)
        {
            if (stoi(comando[2]) > 1)
            {
                if (comando[0].find("[") != -1)
                {
                    cout << "ERROR: variavel com nome invalido:" << comando[0] << endl;
                    return;
                }
                for (int i = 0; i < stoi(comando[2]); i++)
                {
                    tmp.nome = comando[0] + "[" + to_string(i) + "]";
                    tabela_extern.push_back(tmp);
                }
            }
            else
            {
                tmp.nome = comando[0];
                tabela_extern.push_back(tmp);
            }
        }
        else
        {
            cout << "ERRO: variavel " << comando[0] << " declarada multiplas vezes" << endl;
            return;
        }
    }
}

void declaraGlobal(vector<string> comando, global tmp, vector<global> &tabela_global)
{
    if (comando[1] == "GLOBAL")
    {
        if (rastreiaGlobal(comando[0], tabela_global) == -1)
        {
            if (stoi(comando[2]) > 1)
            {
                if (comando[0].find("[") != -1)
                {
                    cout << "ERROR: variavel com nome invalido:" << comando[0] << endl;
                    return;
                }
                for (int i = 0; i < stoi(comando[2]); i++)
                {
                    tmp.nome = comando[0] + "[" + to_string(i) + "]";
                    tabela_global.push_back(tmp);
                }
            }
            else
            {
                tmp.nome = comando[0];
                tabela_global.push_back(tmp);
            }
        }
        else
        {
            cout << "ERRO: variavel " << comando[0] << " declarada multiplas vezes" << endl;
            return;
        }
    }
}

void declaraVariavel(vector<string> comando, simbolo tmp, int &pos, vector<simbolo> &tabela_simbolos)
{
    if (comando[1] == "space")
    {
        if (rastreiaSimbolo(comando[0], tabela_simbolos) == -1)
        {
            if (stoi(comando[2]) > 1)
            {
                if (comando[0].find("[") != -1)
                {
                    cout << "ERROR: variavel com nome invalido:" << comando[0] << endl;
                    return;
                }
                for (int i = 0; i < stoi(comando[2]); i++)
                {
                    tmp.nome = comando[0] + "[" + to_string(i) + "]";
                    tmp.addr = pos + i;
                    tabela_simbolos.push_back(tmp);
                }
            }
            else
            {
                tmp.nome = comando[0];
                tmp.addr = pos;
                tabela_simbolos.push_back(tmp);
            }
            pos = pos + stoi(comando[2]);
        }
        else
        {
            cout << "ERRO: variavel " << comando[0] << " declarada multiplas vezes" << endl;
            return;
        }
    }
}

tabelas passo1(string fileName)
{
    tabelas tabelas;
    string linha;
    int pos = 0, i = 0;
    // int posEx=0,posG=0; //extern e global
    int nLinha = 0; //usado pra labels;
    simbolo tmp;
    vector<string> comando;
    ifstream leitura;
    leitura.open(fileName);
    while (!leitura.eof())
    {
        getline(leitura, linha);
        comando = separaLinha(linha);
        if (comando.size() == 3)
        {
            if (comando[1] == "GLOBAL")
            {
                global tmp;
                declaraGlobal(comando, tmp, tabelas.tabela_global);
            }
            else if (comando[1] == "EXTERN")
            {
                externa tmp;
                declaraExtern(comando, tmp, tabelas.tabela_extern);
            }
            else if (comando[1] == "space")
            {
                declaraVariavel(comando, tmp, pos, tabelas.tabela_simbolos);
            }
        }
        else if ((comando.size() == 1) &&
                 (comando[0].substr(comando[0].size() - 1, comando[0].size()) == ":"))
        {
            declaraLabel(comando, nLinha - 1, tabelas.tabela_labels);
        }
        if (comando.size() != 3 || comando[1] != "space")
        {
            nLinha++;
        }
        i++;
    }
    return tabelas;
}

void passo2(string fileName, string targetName, tabelas tabelas)
{
    ifstream leitor(fileName);
    ofstream escritor(targetName);
    string linha;
    vector<string> comando;
    cout << "vou definir cs como:" << tabelas.tabela_simbolos.size() << endl;
    escritor << "99 " << to_string(tabelas.tabela_simbolos.size() - 1) << endl;
    for (int i = 0; i < tabelas.tabela_simbolos.size(); i++)
    {
        escritor << "00 00" << endl;
    }
    while (!leitor.eof())
    {
        int opAddr;
        getline(leitor, linha);
        cout << "li:" << linha << endl;
        comando = separaLinha(linha);
        string addr;
        if (comando.size() == 2)
        {
            opAddr = rastreiaSimbolo(comando[1], tabelas.tabela_simbolos);
            if (opAddr == -1 && comando[0].substr(0, 1) != "j")
            {
                cout << "ERROR: uso de variavel nao declarada" << comando[1] << endl;
                return;
            }
            addr = to_string((tabelas.tabela_simbolos[opAddr].addr) + 1);
            if (comando[0] == "push")
            {
                cout << "comando push" << addr << endl;
                escritor << "00 " << addr << endl;
            }
            else if (comando[0] == "store")
            {
                escritor << "05 " << addr << endl;
            }
            else if (comando[0] == "jmp")
            {
                int a = rastreiaLabel(comando[1], tabelas.tabela_labels);
                if (a == -1)
                {
                    cout << "label " << comando[1] << " não declarada" << endl;
                    return;
                }
                else
                {
                    cout << "vou escrever jmp para " << tabelas.tabela_labels[a].addr << endl;
                    escritor << "06 " << (tabelas.tabela_labels[a].addr) << endl;
                }
            }
            else if (comando[0] == "jeq")
            {
                int a = rastreiaLabel(comando[1], tabelas.tabela_labels);
                if (a == -1)
                {
                    cout << "label " << comando[1] << " não declarada" << endl;
                    return;
                }
                else
                {
                    escritor << "07 " << tabelas.tabela_labels[a].addr << endl;
                }
            }
            else if (comando[0] == "jgt")
            {
                int a = rastreiaLabel(comando[1], tabelas.tabela_labels);
                if (a == -1)
                {
                    cout << "label " << comando[1] << " não declarada" << endl;
                    return;
                }
                else
                {
                    escritor << "08 " << tabelas.tabela_labels[a].addr << endl;
                }
            }
            else if (comando[0] == "jlt")
            {
                int a = rastreiaLabel(comando[1], tabelas.tabela_labels);
                if (a == -1)
                {
                    cout << "label " << comando[1] << " não declarada" << endl;
                    return;
                }
                else
                {
                    escritor << "09 " << tabelas.tabela_labels[a].addr << endl;
                }
            }
            else if (comando[0] == "in")
            {
                escritor << "10 " << addr << endl;
            }
            else if (comando[0] == "out")
            {
                escritor << "11 " << addr << endl;
            }
        }
        else if (comando.size() == 1)
        {
            if (comando[0] == "add")
            {
                escritor << "01 00" << endl;
            }
            else if (comando[0] == "sub")
            {
                escritor << "02 00" << endl;
            }
            else if (comando[0] == "mul")
            {
                escritor << "03 00" << endl;
            }
            else if (comando[0] == "div")
            {
                escritor << "04 00" << endl;
            }
            else if (comando[0] == "stp")
            {
                escritor << "12 " << endl;
            }
        }
    }
}
