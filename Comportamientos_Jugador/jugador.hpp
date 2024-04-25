#ifndef COMPORTAMIENTOJUGADOR_H
#define COMPORTAMIENTOJUGADOR_H

#include "comportamientos/comportamiento.hpp"

#include <list>


// Estado  
struct stateN0{

  ubicacion jugador;
  ubicacion colaborador;
  Action ultimaOrdenColaborador; // ultima acción que el jugador ordeno al colaborador. Si el jugador no indica nueva acción seguirá haciendo esta última

  bool operator==(const stateN0 &x) const {

    // Uso operator== de la struct ubicacion 
    if (jugador == x.jugador and colaborador.f == x.colaborador.f and colaborador.c == x.colaborador.c and ultimaOrdenColaborador == x.ultimaOrdenColaborador) {

      return true;
    
    } else {

      return false;
    }
  }
};


// Nodo: almacenar en cada nodo la secuencia de acciones hasta el momento
struct nodeN0{
 
  stateN0 st; // Estado actual
  list<Action> secuencia; // Secuencia de acciones

  bool operator==(const nodeN0 &n) const {
    
    return (st == n.st);
  }

  bool operator<(const nodeN0 &b)  const {
    
    // Tener en cuenta al colaborador en niveles posteriores
    return  (st.jugador.f < b.st.jugador.f) || 
            (st.jugador.f == b.st.jugador.f and st.jugador.c < b.st.jugador.c) || 
            (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula < b.st.jugador.brujula) ||
            (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and st.colaborador.f < b.st.colaborador.f) ||
            (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and st.colaborador.f == b.st.colaborador.f and st.colaborador.c < b.st.colaborador.c) ||
            (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and st.colaborador.f == b.st.colaborador.f and st.colaborador.c == b.st.colaborador.c and st.colaborador.brujula < b.st.colaborador.brujula) ||
            (st.jugador.f == b.st.jugador.f and st.jugador.c == b.st.jugador.c and st.jugador.brujula == b.st.jugador.brujula and st.colaborador.f == b.st.colaborador.f and st.colaborador.c == b.st.colaborador.c and st.colaborador.brujula == b.st.colaborador.brujula and st.ultimaOrdenColaborador < b.st.ultimaOrdenColaborador);
  }
};


// Clase ComportamientoJugador
class ComportamientoJugador : public Comportamiento {
  
  public:


    // CONSTRUCTOR NIVEL 4
    ComportamientoJugador(unsigned int size) : Comportamiento(size) {
      // Inicializar Variables de Estado
    }


    // CONSTRUCTOR NIVELES 0,1,2,3
    ComportamientoJugador(std::vector< std::vector< unsigned char> > mapaR) : Comportamiento(mapaR) {
      
      // Inicializar Variables de Estado
      
      //plan = Inicializado en el metodo think;
      hayPlan = false;
      //c_state = Inicializado en el metodo think;
      //goal = Inicializado en el metodo think;
      

    }


    // CONSTRUCTOR DE COPIA
    ComportamientoJugador(const ComportamientoJugador & comport) : Comportamiento(comport){ 

      plan = comport.plan;
      hayPlan = comport.hayPlan;
      c_state = comport.c_state;
      goal = comport.goal;
    }
    

    // DESTRUCTOR --> Al no contar con memoria dinamica nos sirve el destructor por defecto
    ~ComportamientoJugador(){}


    // MÉTODO INTERACT --> No se implementa
    int interact(Action accion, int valor);


    // FUNCIONES A IMPLEMENTAR / UTILIZAR
    void VisualizaPlan(const stateN0 &st, const list<Action> &plan);
    Action think(Sensores sensores);


  private:

    // Declarar Variables de Estado
    list<Action> plan; // Almacena el plan en ejecución
    bool hayPlan;      // Si se esta siguiendo o no un plan
    stateN0 c_state;   // Estado actual
		ubicacion goal;    // Ubicación de casilla objetivo  

};

#endif