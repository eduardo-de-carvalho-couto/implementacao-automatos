#include <iostream>
#include <string>
#include <set>
#include <map>
#include <sstream>

class AFD {
private:
    std::set<std::string> estados;
    std::set<std::string> alfabeto;
    std::map<std::pair<std::string, std::string>, std::string> transicoes;
    std::string estado_inicial;
    std::set<std::string> estados_aceitacao;

public:
    AFD(std::set<std::string> e, std::set<std::string> a, std::map<std::pair<std::string, std::string>, std::string> t, std::string ei, std::set<std::string> ea)
        : estados(e), alfabeto(a), transicoes(t), estado_inicial(ei), estados_aceitacao(ea) {}

    bool aceita(const std::string& cadeia) {
        std::string estado_atual = estado_inicial;

        for (char simbolo : cadeia) {
            std::string simbolo_str(1, simbolo);
            auto transicao = transicoes.find({estado_atual, simbolo_str});
            if (transicao != transicoes.end()) {
                estado_atual = transicao->second;
            } else {
                return false;
            }
        }

        return estados_aceitacao.find(estado_atual) != estados_aceitacao.end();
    }
};

// Função para capturar os parâmetros do AFD
AFD capturar_afd() {
    std::cout << "Definição do AFD:" << std::endl;
    std::set<std::string> estados;
    std::set<std::string> alfabeto;

    std::string input;
    std::cout << "Digite os estados separados por espaço (ex: q0 q1 q2): ";
    std::getline(std::cin, input);
    std::istringstream estados_stream(input);
    std::string estado;
    while (estados_stream >> estado) {
        estados.insert(estado);
    }

    std::cout << "Digite os símbolos do alfabeto separados por espaço (ex: 0 1): ";
    std::getline(std::cin, input);
    std::istringstream alfabeto_stream(input);
    std::string simbolo;
    while (alfabeto_stream >> simbolo) {
        alfabeto.insert(simbolo);
    }

    std::cout << "Digite as transições no formato 'estado_atual simbolo estado_destino' (uma por linha)." << std::endl;
    std::cout << "Digite 'fim' para encerrar as transições:" << std::endl;
    std::map<std::pair<std::string, std::string>, std::string> transicoes;
    while (true) {
        std::getline(std::cin, input);
        if (input == "fim") {
            break;
        }
        std::istringstream transicao_stream(input);
        std::string estado_atual, simbolo, estado_destino;
        transicao_stream >> estado_atual >> simbolo >> estado_destino;
        if (transicao_stream.fail() || estado_atual.empty() || simbolo.empty() || estado_destino.empty()) {
            std::cout << "Formato inválido. Use: estado_atual simbolo estado_destino" << std::endl;
            continue;
        }
        transicoes[{estado_atual, simbolo}] = estado_destino;
    }

    std::string estado_inicial;
    std::cout << "Digite o estado inicial: ";
    std::getline(std::cin, estado_inicial);

    std::set<std::string> estados_aceitacao;
    std::cout << "Digite os estados de aceitação separados por espaço: ";
    std::getline(std::cin, input);
    std::istringstream estados_aceitacao_stream(input);
    while (estados_aceitacao_stream >> estado) {
        estados_aceitacao.insert(estado);
    }

    return AFD(estados, alfabeto, transicoes, estado_inicial, estados_aceitacao);
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        std::cout << "Modo interativo ativado." << std::endl;
        AFD afd = capturar_afd();
        std::cout << "\nAFD definido com sucesso!\n" << std::endl;

        while (true) {
            std::string cadeia;
            std::cout << "Digite uma cadeia para testar (ou 'sair' para encerrar): ";
            std::getline(std::cin, cadeia);
            if (cadeia == "sair") {
                std::cout << "Encerrando o programa." << std::endl;
                break;
            }
            if (afd.aceita(cadeia)) {
                std::cout << "A cadeia '" << cadeia << "' foi aceita pelo AFD." << std::endl;
            } else {
                std::cout << "A cadeia '" << cadeia << "' foi rejeitada pelo AFD." << std::endl;
            }
        }
    } else {
        std::cout << "Modo de uso:" << std::endl;
        std::cout << "Para definir o AFD de forma interativa, execute o script sem argumentos." << std::endl;
        std::cout << "Exemplo: ./AFD" << std::endl;
    }

    return 0;
}


// q0 0 q1
// q0 1 q0
// q1 0 q2
// q1 1 q0
// q2 0 q2
// q2 1 q2

// 00
// 01
// 000
// 0011

