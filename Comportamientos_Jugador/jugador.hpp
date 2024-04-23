#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>


class ComportamientoJugador : public Comportamiento {
  
  public:

    // NIVEL 4
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
    }

    // NIVELES 0,1,2,3
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      
      // Inicializar Variables de Estado
      // plan = ...;
      hayPlan = false;
      

    }

    // CONSTRUCTOR DE COPIA
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){ 

      plan = comport.plan;
      hayPlan = comport.hayPlan;
    }
    
    // DESTRUCTOR
    ~ComportamientoJugador(){}

    // MÉTODO A IMPLEMENTAR
    Action think(Sensores sensores);
    int interact(Action accion, int valor);


  private:

    // Declarar Variables de Estado
    list<Action> plan; // Almacena el plan en ejecución
    bool hayPlan;      // Si se esta siguiendo o no un plan





};

#endif
