#include <iostream>
#include <stack>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

typedef struct simbolo{
    int addr;
    string nome;
}simbolo;
vector <simbolo> tabela_simbolos;

typedef struct label{
    int addr;
    string nome;
}label;
vector <label> tabela_labels;

typedef struct global{
    int addr;
    string nome;
}global;
vector <global> tabela_global;

typedef struct externa{
    int addr;
    string nome;
}externa;
vector <externa> tabela_extern;


vector <string> separaLinha(string linha);
int rastreiaSimbolo(string nome_simbolo);
int rastreiaLabel(string nome_label);
void passo1(string fileName);
void passo2(string fileName,string targetName);

int main(int argc, char* argv[]){
    string arquivoSaida="saida";
    if(argc<2){
        cout<<"Digite o nome de um arquivo texto para monta-lo"<<endl;
        return 0;
    }
    if(argc>=3){
        arquivoSaida=argv[2];
    }
    passo1(argv[1]);
    for(int i=0;i<tabela_simbolos.size();i++){
        cout<<"addr:"<<tabela_simbolos[i].addr<<" nome:"<<tabela_simbolos[i].nome<<endl;
    }
    passo2(argv[1],arquivoSaida);
    return 0;
}


vector <string> separaLinha(string linha){
    vector<string> resultArray;
    bool espace=true;
    int separador=0,len=linha.size();
    string tmp;

    for (int j = 0, k = 0; j < len; j++) {
        if (linha[j] == ' ') {
            string ch = linha.substr(k, j - k);
            k = j+1;
            resultArray.push_back(ch);
        }
        if (j == len - 1) {
            string ch = linha.substr(k, j - k+1);
            resultArray.push_back(ch);
        }
    }
    return resultArray;
}

int rastreiaSimbolo(string nome_simbolo){
    for(int i=0;i<tabela_simbolos.size();i++){
        if(tabela_simbolos[i].nome==nome_simbolo){
            return i;
        }
    }
    return -1;
}

int rastreiaLabel(string nome_label){
    for(int i=0;i<tabela_labels.size();i++){
        if(tabela_labels[i].nome==nome_label){
            return i;
        }
    }
    return -1;
}

void declaraLabel(vector <string> comando,int nLinha){
    // cout<<<<endl;
    string labelName;
    labelName = comando[0].substr(0,comando[0].size()-1);
    int a = rastreiaLabel(labelName);
    label tmp;
    if(a!=-1){
        cout<<"label "<<comando[0]<<" declarada multiplas vezes"<<endl;
    }
    else{
        tmp.addr=nLinha;
        tmp.nome=labelName;
        tabela_labels.push_back(tmp);
        cout<<"adicionando label:"<<labelName<<endl;
    }
}

void declaraVariavel(vector <string> comando,simbolo tmp,int pos){
    if(comando[1]=="space"){
        if(rastreiaSimbolo(comando[0])==-1){
            if(stoi(comando[2])>1){
                if(comando[0].find("[")!=-1){
                    cout<<"ERROR: variavel com nome invalido:"<<comando[0]<<endl;
                    return;
                }
                for(int i =0;i<stoi(comando[2]);i++){
                    tmp.nome=comando[0]+"["+to_string(i)+"]";
                    tmp.addr=pos+i;
                    tabela_simbolos.push_back(tmp);
                }
            }else{
                tmp.nome=comando[0];
                tmp.addr=pos;
                tabela_simbolos.push_back(tmp);
            }
            pos=pos+stoi(comando[2]);
        }else{
            cout<<"ERRO: variavel "<<comando[0]<<" declarada multiplas vezes"<<endl;
            return;
        }
    }
}

void passo1(string fileName){
    string linha;
    int pos = 0,i=0;
    int nLinha=0;//usado pra labels;
    simbolo tmp;
    vector <string> comando;
    ifstream leitura;
    leitura.open(fileName);
    while(!leitura.eof()){
        getline(leitura,linha);
        comando = separaLinha(linha);
        if(comando.size()==3){
            declaraVariavel(comando, tmp, pos);
        }else if((comando.size()==1) &&
            (comando[0].substr(comando[0].size()-1,comando[0].size())==":")){
            declaraLabel(comando,nLinha);
        }
        if(comando.size()!=3 || comando[1]!="space"){
            cout<<"aqui chega"<<endl;
            nLinha++;
        }
        i++;
    }
}

void passo2(string fileName,string targetName){
    ifstream leitor(fileName);
    ofstream escritor(targetName);
    string linha;
    vector <string> comando;
    escritor<<"99 "<<to_string(tabela_simbolos.size())<<endl;
    for(int i=0;i<tabela_simbolos.size();i++){
        escritor<<"00 00"<<endl;
    }
    while(!leitor.eof()){
        int opAddr;
        getline(leitor,linha);
        comando = separaLinha(linha);
        string addr;
        if(comando.size()==2){
            cout<<"simbolo:"<<comando[1]<<endl;
            opAddr=rastreiaSimbolo(comando[1]);
            cout<<"op addr:"<<opAddr<<endl;
            if(opAddr==-1 && comando[0].substr(0,1)!="j"){
                cout<<"ERROR: uso de variavel nao declarada"<<comando[1]<<endl;
                return;
            }
            addr = to_string((tabela_simbolos[opAddr].addr)+1);
            if(comando[0]=="push"){
                cout<<"comando push"<<endl;
                escritor<<"00 "<<addr<<endl;
            }else if(comando[0]=="store"){
                escritor<<"05 "<<addr<<endl;
            }else if(comando[0]=="jmp"){
                // cout<<"mostrando labels"<<endl;
                // for(int i = 0;i<tabela_labels.size();i++){
                //     cout<<"label:"<<tabela_labels[i].nome<<endl;
                // }
                int a = rastreiaLabel(comando[1]);
                if(a==-1){
                    cout<<"label "<<comando[1]<<" não declarada"<<endl;
                    return;
                }else{
                    escritor<<"06 "<<tabela_labels[a].addr<<endl;
                }
            }else if(comando[0]=="jeq"){
                int a = rastreiaLabel(comando[1]);
                if(a==-1){
                    cout<<"label "<<comando[1]<<" não declarada"<<endl;
                    return;
                }else{
                    escritor<<"07 "<<tabela_labels[a].addr<<endl;
                }
            }else if(comando[0]=="jgt"){
                int a = rastreiaLabel(comando[1]);
                if(a==-1){
                    cout<<"label "<<comando[1]<<" não declarada"<<endl;
                    return;
                }else{
                    escritor<<"08 "<<tabela_labels[a].addr<<endl;
                }
            }else if(comando[0]=="jlt"){
                int a = rastreiaLabel(comando[1]);
                if(a==-1){
                    cout<<"label "<<comando[1]<<" não declarada"<<endl;
                    return;
                }else{
                    escritor<<"09 "<<tabela_labels[a].addr<<endl;
                }
            }else if(comando[0]=="in"){
                escritor<<"10 "<<addr<<endl;
            }else if(comando[0]=="out"){
                escritor<<"11 "<<addr<<endl;
            }

        }else if(comando.size()==1){
            if(comando[0]=="add"){
                escritor<<"01 00"<<endl;
            }else if(comando[0]=="sub"){
                escritor<<"02 00"<<endl;
            }else if(comando[0]=="mul"){
                escritor<<"03 00"<<endl;
            }else if(comando[0]=="div"){
                escritor<<"04 00"<<endl;
            }else if(comando[0]=="stp"){
                escritor<<"12 "<<endl;
            }
        }

    }

}
