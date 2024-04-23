#pragma once

#include <string>
#include <vector>
#include <iostream>

enum Estat { NORMAL, CREMADA, ENVENENADA };

// Declaración de la clase Atac
class Atac {
public:
    std::string nom;
    int potencia;

    Atac(std::string _nom, int _potencia) : nom(_nom), potencia(_potencia){}
};


class Enemic {
public:
    std::string nom;
    int nivell;
    int salut;
    int atac;
    int defensa;
    int velocitat;
    Estat estat;
    std::vector<Atac> atacs;

    Enemic(std::string _nom, int _nivell, int _atac, int _defensa, int _velocitat)
        : nom(_nom), nivell(_nivell), estat(NORMAL), atac(_atac), defensa(_defensa), velocitat(_velocitat) {
        salut = 100; // Inicializamos la salud en 100 por defecto
    }

    void rebreDanys(int danys);

    void aplicarEfecteEstat();

    void atacar(Enemic& oponent, int indexAtac);
};
