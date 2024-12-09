#include<iostream>
#include<stack>
#include<vector>
#include<string>
#include<algorithm>
using namespace std;

// Pilha usada pelo autômato a pilha (AP)
stack<char> faStack;

// Estado inicial e estado atual
int initialState = 0;
int currentState;

// Estrutura que define uma transição no autômato
struct transition {
    int O;      // Estado de origem
    int F;      // Estado de destino
    char L;     // Símbolo de entrada necessário para a transição
    char E;     // Símbolo esperado no topo da pilha
    string D;   // Ação na pilha: empilhar os símbolos especificados
};

// Estrutura que define um estado no autômato, incluindo suas transições
struct state {
    vector<transition> transitions; // Lista de transições do estado
};

// Vetor que armazena todos os estados e as cadeias de entrada
vector<state> states;
vector<string> inputStrings;

// Função para imprimir as cadeias de entrada
void printOutputStrings(vector<string> c) {
    for(int i = 0; i < c.size(); i++) {
        cout << "String " << i << ": " << c[i] << endl;
    }
}

// Inicializa a pilha do autômato com o símbolo de fundo 'Z'
void initfaStack(stack<char> &faStack) {
    char bottom = 'Z';
    faStack.push(bottom);
}

// Empilha os símbolos especificados na string `Dstring` na pilha
void stackSymbols(string Dstring) {
    for(int i = 0; i < Dstring.size(); i++) {
        if(Dstring[i] != '~') { // Ignora o símbolo '~' (não faz nada)
            char character = Dstring[i];
            faStack.push(character);
        }
    }
}

// Imprime o conteúdo atual da pilha
void printfaStack(stack<char> faStack) {
    stack<char> aux = faStack; // Copia a pilha original para preservá-la
    vector<char> faStackReversed;

    // Reverte a pilha para exibir do fundo para o topo
    while(!aux.empty()) {
        faStackReversed.push_back(aux.top());
        aux.pop();
    }

    for(int j = faStackReversed.size() - 1; j >= 0; j--) {
        cout << faStackReversed[j];
    }

    cout << endl;
}

// Esvazia completamente a pilha
void emptyfaStack() {
    while(!faStack.empty()) {
        faStack.pop();
    }
}

// Verifica se há uma transição definida para o símbolo atual
bool definedTransition(state s, char c, char t, int &index) {
    for(int i = 0; i < s.transitions.size(); i++) {
        if(c == s.transitions[i].L && s.transitions[i].E == t) {
            index = i; // Salva o índice da transição encontrada
            return true;
        }
    }
    return false;
}

// Retorna a transição no índice especificado
transition gettransition(state s, int index) {
    return s.transitions[index];
}

// Aplica uma transição, alterando o estado atual e modificando a pilha
void applyTransition(int &currentState, transition t) {
    currentState = t.F; // Atualiza o estado atual
    faStack.pop();      // Remove o símbolo do topo da pilha
    stackSymbols(t.D);  // Empilha os novos símbolos definidos pela transição
    printfaStack(faStack); // Mostra o estado atual da pilha
}

// Testa uma cadeia de entrada para determinar se é aceita pelo AP
bool testInputString(string inputString) {
    int index;
    currentState = initialState; // Reinicia no estado inicial

    char faStackTop = faStack.top(); // Topo inicial da pilha

    if(inputString.size() == 0) { // Verifica se a cadeia é vazia
        return false;
    }

    // Processa cada símbolo da cadeia de entrada
    for(int i = 0; i < inputString.size(); i++) {
        // Se existe uma transição válida
        if (definedTransition(states[currentState], inputString[i], faStackTop, index)) {
            transition t = gettransition(states[currentState], index);
            applyTransition(currentState, t); // Aplica a transição
            faStackTop = faStack.top();       // Atualiza o topo da pilha
            if(i == inputString.size() - 1) { // Se processou toda a cadeia
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
        state state;
        states.push_back(state);
    }

    // Lê as transições
    for(int i = 0; i < T; i++) {
        int o, f;
        char l, e;
        string d;
        cin >> o >> f >> l >> e >> d;

        transition t = {o, f, l, e, d};
        states[o].transitions.push_back(t); // Adiciona a transição ao estado de origem
    }

    cin.ignore(); // Limpa o buffer

    // Lê as cadeias de entrada
    for(int i = 0; i < C; i++) {
        string inputString;
        getline(cin, inputString);
        inputString += '~'; // Adiciona o marcador de fim
        inputStrings.push_back(inputString);
    }

    // Processa cada cadeia de entrada
    for(int i = 0; i < C; i++) {
        initfaStack(faStack); // Inicializa a pilha
        cout << "Case " << i + 1 << ":" << endl;
        printfaStack(faStack); // Mostra a pilha inicial
        bool resultado = testInputString(inputStrings[i]);
        if(resultado) {
            cout << "ACCEPTED" << endl;
        } else {
            cout << "REJECTED" << endl;
        }
        cout << endl;
        emptyfaStack(); // Limpa a pilha para o próximo teste
    }

    return 0;
}
