#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <list>
#include <map>
#include "testef.hpp"

using namespace std;

class Carro {
private:
    string nome;
    int idade;
public:
    string getNome() {
        return this->nome;
    }
    void setNome(string nome) {
        this->nome = nome;
    }
    int getIdade() {
        return this->idade;
    }
    void setIdade(int idade) {
        this->idade = idade;
    }
};

int main(int argc, char** argv) {

    Carro ca;
    Carro cb;

    ca.setNome("ca");
    ca.setIdade(1);

    cb.setNome("cb");
    cb.setIdade(2);

    Carro cc = cb;
    cb = cc;
    
    cc.setNome("cc");
    cc.getNome().data()[0] = 'x';
    cc.getNome().data()[1] = 'x';
    cc.setIdade(3);

    string x = cc.getNome();
    x.data()[0] = 'x';
    printf("x: %s\n", x.data());
    x.data()[1] = 'x';
    printf("x: %s\n", x.data());

    printf("ca.nome: %s, ca.idade: %d \n", ca.getNome().data(), ca.getIdade());
    printf("cb.nome: %s, cb.idade: %d \n", cb.getNome().data(), cb.getIdade());
    printf("cc.nome: %s, cc.idade: %d \n", cc.getNome().data(), cc.getIdade());

    string a("abcdefgh");
    string b = a;
    printf("%s, %s, %p, %p, %p, %p\n", a.data(), b.data(), a.data(), b.data(), a.data(), b.data());

    b[0] = 'i';
    b[1] = 'j';
    b[2] = 'k';
    string c = b.substr(0, 4);
    string d = b.substr(0, 4);
    char *e = c.data();
    char *f = d.data();
    e[0] = 'z';
    f[0] = 'y';
    
    printf("%s, %s, %p, %p, %s, %s, %p, %p, %s, %s\n", a.c_str(), b.c_str(), &a, &b, c.c_str(), d.data(), e, f, e, f);

    return sum(0, 0);
}