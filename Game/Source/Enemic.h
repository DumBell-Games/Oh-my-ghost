#pragma once
#include "StatusEffect.h"
#include <memory>

#include <string>
#include <vector>
#include <iostream>

enum Estat { NORMAL, CREMADA, ENVENENADA };
enum Objectiu { RIVAL, PROPI, TOTS };

// Declaración de la clase Atac
class Atac {
public:
    std::string nom;
    int potencia;
    Estat estat = NORMAL;
    std::unique_ptr<StatusEffect> status;
    int probEstat = 0; // % de probabilitat
    Objectiu objectiu = RIVAL;

    Atac(std::string _nom, int _potencia) : nom(_nom), potencia(_potencia){}
};


class Personatge {
public:
    std::string nom;
    int nivell;
    int salutTotal;
    int salutActual;
    int atac;
    int defensa;
    int velocitat;
    Estat estat;
    std::unique_ptr<StatusEffect> activeStatus;
    std::vector<Atac> atacs;

    Personatge(std::string _nom, int _nivell, int _atac, int _defensa, int _velocitat)
        : nom(_nom), nivell(_nivell), estat(NORMAL), atac(_atac), defensa(_defensa), velocitat(_velocitat) {
        salutTotal = 100; // Inicializamos la salud en 100 por defecto
        salutActual = 100;
    }

    void rebreDanys(int danys);

    void aplicarEfecteEstat();

    void atacar(Personatge& oponent, int indexAtac);
};
