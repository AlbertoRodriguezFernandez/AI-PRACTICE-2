#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>


// Estado del nivel 0
struct stateN0{

  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador;

  bool operator==(const stateN0 &x) const {

    // Uso operator== de la estruct ubicacion 
    if (jugador == x.jugador and colaborador.f == x.colaborador.f and colaborador.c == x.colaborador.c) {

      return true;
    
    } else {

      return false;
    }
  }
};


// Clase ComportamientoJugador
class ComportamientoJugador : public Comportamiento {
  
  public:

    // NIVEL 4
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
    }

    // NIVELES 0,1,2,3
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      
      // Inicializar Variables de Estado
      
      // TODO 
      //plan = ...;
      hayPlan = false;
      //c_state = ...;
      //goal = ...;
      

    }

    // CONSTRUCTOR DE COPIA
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){ 

      plan = comport.plan;
      hayPlan = comport.hayPlan;
      c_state = comport.c_state;
      goal = comport.goal;
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
    stateN0 c_state;   // Estado actual
		ubicacion goal;    // Ubicación de casilla objetivo  

};

#endif
