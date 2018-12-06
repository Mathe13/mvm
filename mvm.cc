#include <iostream>
#include <stack>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

//strcuct definition
typedef struct
{
    int comando;
    int op;
} mem;

//variables definition
mem mem_p[512];
int CS = 0;
int DS = 0;
int SP = 511; //stack counter
int PC = CS;  //program counter

//function definition
bool loadProg(string filename);
void cpu();
bool push(int op);
void add();
void sub();
void mul();
void div();
void store(int op);
void jmp(int op);
void jeq(int op);
void jgt(int io);
void jlt(int io);
void out(int op);
void in(int op);
void define_CS(int op);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "especifique o nome do codigo fonte" << endl;
        return 0;
    }
    bool carregou = loadProg("saida");
    cout << "aqui chega" << endl;
    if (carregou)
    {
        cpu();
    }
    return 0;
}

void cpu()
{
    bool teste;
    while (1)
    // cout << "lendo linha" << PC + CS << endl;
    // cout << "li:" << mem_p[PC + CS].comando << " " << mem_p[PC + CS].op << endl;
    {
        switch (mem_p[PC + CS].comando)
        {
        case 0:
        {
            teste = push((mem_p[PC + CS].op));
            if (!teste)
            {
                cout << "pilha cheia" << endl;
            }
            break;
        }
        case 1:
        {
            add();
            break;
        }
        case 2:
        {
            sub();
            break;
        }
        case 3:
        {
            mul();
            break;
        }
        case 4:
        {
            div();
            break;
        }
        case 5:
        {
            store((mem_p[PC + CS].op));
            break;
        }
        case 6:
        {
            jmp((mem_p[PC + CS].op));
            break;
        }
        case 7:
        {
            jeq((mem_p[PC + CS].op));
            break;
        }
        case 8:
        {
            jgt((mem_p[PC + CS].op));
            break;
        }
        case 9:
        {
            jlt((mem_p[PC + CS].op));
            break;
        }
        case 10:
        {
            in(mem_p[PC + CS].op);
            break;
        }
        case 11:
        {
            out(mem_p[PC + CS].op);
            break;
        }
        case 12:
        {
            return;
            break;
        }
        case 99:
        {
            cout << "definindo cs como " << mem_p[PC].op << endl;
            define_CS(mem_p[PC].op);
        }
        }

        PC++;
    }
}

void out(int op)
{
    cout << mem_p[op].op << endl;
}

void jeq(int op)
{
    cout << "verificando jeq" << endl;
    if ((mem_p[SP].op) == 0)
    {
        PC = op;
        cout << "fazenndo jeq" << endl;
    }
}

void jgt(int op)
{
    cout << "verificando jgt" << endl;
    if ((mem_p[SP].op) > 0)
    {
        cout << "fazenndo jgt" << endl;
        PC = op;
    }
}

void jlt(int op)
{
    cout << "verificando jlt" << endl;
    if ((mem_p[SP].op) < 0)
    {
        cout << "fazenndo jlt" << endl;
        PC = op;
    }
}

void jmp(int op)
{
    cout << "fazendo jump para a pos " << op + CS << endl;
    PC = op;
}

void store(int op)
{
    cout << "fazenndo store do valor:" << mem_p[SP].op << " para:" << SP << endl;
    mem_p[op].op = mem_p[SP].op;
    // SP--;
}

void mul()
{
    if (SP > 510)
    {
        cout << "carregue dois valores primeiro" << endl;
    }
    cout << "fazendo mul" << endl;
    mem_p[SP - 1].op = mem_p[SP].op * mem_p[SP + 1].op;
    SP--;
}

void div()
{
    if (SP > 510)
    {
        cout << "carregue dois valores primeiro" << endl;
    }
    cout << "fazendo div" << endl;
    mem_p[SP - 1].op = (mem_p[SP].op) / (mem_p[SP + 1].op);
    SP--;
}
void in(int op)
{
    // int a;
    cout << "Digite um numero" << endl;
    cin >> mem_p[op].op;
    cout << "salvando o valor:" << mem_p[op].op << " em " << op << endl;
}
void sub()
{
    if (SP > 510)
    {
        cout << "carregue dois valores primeiro" << endl;
    }
    cout << "fazendo sub de:" << mem_p[SP].op << " e " << mem_p[SP + 1].op << " e salvando em " << (SP - 1) << endl;
    mem_p[SP - 1].op = (mem_p[SP].op) - (mem_p[SP + 1].op);
    SP--;
}

void add()
{
    if (SP > 510)
    {
        cout << "carregue dois valores primeiro" << endl;
    }
    cout << "fazendo add de:" << mem_p[SP].op << " e " << mem_p[SP + 1].op << " e salvando em " << (SP - 1) << endl;
    mem_p[SP - 1].op = (mem_p[SP].op) + (mem_p[SP + 1].op);
    SP--;
}
void define_CS(int op)
{
    CS = op;
}

bool push(int op)
{
    // if(SP >= PC+CS){
    //     return false;
    // }
    SP--;
    mem_p[SP].op = mem_p[op].op;
    cout << "fazendo push de :" << op << " e salvando em:" << SP << endl;
    return true;
}

bool loadProg(string filename)
{
    cout << "aqui" << endl;
    ifstream leitor;
    leitor.open(filename);
    if (leitor.is_open())
    {

        int linha = 0;
        string aux;
        while (!leitor.eof())
        {
            if (linha > 500)
            {
                cout << "erro seu programa excedeu a memoria maxima" << endl;
                return false;
            }
            else
            {
                leitor >> aux;
                mem_p[linha].comando = stoi(aux);
                leitor >> aux;
                mem_p[linha].op = stoi(aux);
                cout << "li:" << mem_p[linha].comando << " " << mem_p[linha].op << endl;
            }

            linha++;
        }
        return true;
    }
    else
    {
        cout << "erro ao abrir executavel" << endl;
    }
}
