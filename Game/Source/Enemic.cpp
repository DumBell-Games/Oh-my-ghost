#include "Enemic.h"

void Enemic::rebreDanys(int danys) {
    salutActual -= danys;
    if (salutActual < 0) salutActual = 0;
}

void Enemic::aplicarEfecteEstat() {
    if (estat == CREMADA || estat == ENVENENADA) {
        salutActual -= salutActual * 0.05; // Aplicar daños por efecto de estado
        if (salutActual < 0) salutActual = 0;
    }
}

void Enemic::atacar(Enemic& oponent, int indexAtac) {
    if (indexAtac < 0 || indexAtac >= atacs.size()) {
        std::cout << "Índice de ataque no válido." << std::endl;
        return;
    }
    int danys = (atacs[indexAtac].potencia * atac) / oponent.defensa;
   
    oponent.rebreDanys(danys);
}

