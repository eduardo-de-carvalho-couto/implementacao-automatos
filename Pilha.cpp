#include<iostream>
#include<stack>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

// Pilha usada pelo autômato a pilha (AP)
stack<char> pilhaAP;

// Estado inicial e estado atual
int estadoInicial = 0;
int estadoAtual;

// Estrutura que define uma transição no autômato
struct transicao {
    int origem;    // Estado de origem
    int destino;   // Estado de destino
    char simbolo;  // Símbolo de entrada necessário para a transição
    char topo;     // Símbolo esperado no topo da pilha
    string acao;   // Ação na pilha: empilhar os símbolos especificados
};

// Estrutura que define um estado no autômato, incluindo suas transições
struct estado {
    vector<transicao> transicoes; // Lista de transições do estado
};

// Vetor que armazena todos os estados e as cadeias de entrada
vector<estado> estados;
vector<string> cadeiasDeEntrada;

// Função para imprimir as cadeias de entrada
void imprimirCadeiasDeEntrada(vector<string> c) {
    for(int i = 0; i < c.size(); i++) {
        cout << "Cadeia " << i << ": " << c[i] << endl;
    }
}

// Inicializa a pilha do autômato com o símbolo de fundo 'Z'
void inicializarPilhaAP(stack<char> &pilhaAP) {
    char fundo = 'Z';
    pilhaAP.push(fundo);
}

// Empilha os símbolos especificados na string `acao` na pilha
void empilharSimbolos(string acao) {
    for(int i = 0; i < acao.size(); i++) {
        if(acao[i] != '~') { // Ignora o símbolo '~' (não faz nada)
            char caractere = acao[i];
            pilhaAP.push(caractere);
        }
    }
}

// Imprime o conteúdo atual da pilha
void imprimirPilhaAP(stack<char> pilhaAP) {
    stack<char> aux = pilhaAP; // Copia a pilha original para preservá-la
    vector<char> pilhaAPInvertida;

    // Reverte a pilha para exibir do fundo para o topo
    while(!aux.empty()) {
        pilhaAPInvertida.push_back(aux.top());
        aux.pop();
    }

    for(int j = pilhaAPInvertida.size() - 1; j >= 0; j--) {
        cout << pilhaAPInvertida[j];
    }

    cout << endl;
}

// Esvazia completamente a pilha
void esvaziarPilhaAP() {
    while(!pilhaAP.empty()) {
        pilhaAP.pop();
    }
}

// Verifica se há uma transição definida para o símbolo atual
bool transicaoDefinida(estado e, char c, char t, int &indice) {
    for(int i = 0; i < e.transicoes.size(); i++) {
        if(c == e.transicoes[i].simbolo && e.transicoes[i].topo == t) {
            indice = i; // Salva o índice da transição encontrada
            return true;
        }
    }
    return false;
}

// Retorna a transição no índice especificado
transicao obterTransicao(estado e, int indice) {
    return e.transicoes[indice];
}

// Aplica uma transição, alterando o estado atual e modificando a pilha
void aplicarTransicao(int &estadoAtual, transicao t) {
    estadoAtual = t.destino; // Atualiza o estado atual
    pilhaAP.pop();      // Remove o símbolo do topo da pilha
    empilharSimbolos(t.acao);  // Empilha os novos símbolos definidos pela transição
    imprimirPilhaAP(pilhaAP); // Mostra o estado atual da pilha
}

// Testa uma cadeia de entrada para determinar se é aceita pelo AP
bool testarCadeiaDeEntrada(string cadeiaDeEntrada) {
    int indice;
    estadoAtual = estadoInicial; // Reinicia no estado inicial

    char topoPilhaAP = pilhaAP.top(); // Topo inicial da pilha

    if(cadeiaDeEntrada.size() == 0) { // Verifica se a cadeia é vazia
        return false;
    }

    // Processa cada símbolo da cadeia de entrada
    for(int i = 0; i < cadeiaDeEntrada.size(); i++) {
        // Se existe uma transição válida
        if (transicaoDefinida(estados[estadoAtual], cadeiaDeEntrada[i], topoPilhaAP, indice)) {
            transicao t = obterTransicao(estados[estadoAtual], indice);
            aplicarTransicao(estadoAtual, t); // Aplica a transição
            topoPilhaAP = pilhaAP.top();       // Atualiza o topo da pilha
            if(i == cadeiaDeEntrada.size() - 1) { // Se processou toda a cadeia
                return true; // Aceita
            }
        } else {
            return false; // Transição inválida
        }
    }

    return false; // Rejeita por padrão
}

int main() {
    int C; // Número de cadeias de entrada
    cin >> C;

    int N, T; // Número de estados e transições
    cin >> N >> T;

    // Cria os estados do autômato
    for(int i = 0; i < N; i++) {
        estado e;
        estados.push_back(e);
    }

    // Lê as transições
    for(int i = 0; i < T; i++) {
        int o, f;
        char l, e;
        string d;
        cin >> o >> f >> l >> e >> d;

        transicao t = {o, f, l, e, d};
        estados[o].transicoes.push_back(t); // Adiciona a transição ao estado de origem
    }

    cin.ignore(); // Limpa o buffer

    // Lê as cadeias de entrada
    for(int i = 0; i < C; i++) {
        string cadeiaDeEntrada;
        getline(cin, cadeiaDeEntrada);
        cadeiaDeEntrada += '~'; // Adiciona o marcador de fim
        cadeiasDeEntrada.push_back(cadeiaDeEntrada);
    }

    // Processa cada cadeia de entrada
    for(int i = 0; i < C; i++) {
        inicializarPilhaAP(pilhaAP); // Inicializa a pilha
        cout << "Caso " << i + 1 << ":" << endl;
        imprimirPilhaAP(pilhaAP); // Mostra a pilha inicial
        bool resultado = testarCadeiaDeEntrada(cadeiasDeEntrada[i]);
        if(resultado) {
            cout << "ACEITO" << endl;
        } else {
            cout << "REJEITADO" << endl;
        }
        cout << endl;
        esvaziarPilhaAP(); // Limpa a pilha para o próximo teste
    }

    return 0;
}
