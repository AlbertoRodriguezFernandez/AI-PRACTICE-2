#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>


// Devuelve si una ubicación en el mapa es transitable para el jugador
bool CasillaTransitable(const ubicacion &x, const vector<vector<unsigned char> > &mapa){
	
	return (mapa[x.f][x.c] != 'P' and mapa[x.f][x.c] != 'M');
}


// Devuelve la ubicación siguiente al avance del agente
ubicacion NextCasilla(const ubicacion &pos){
	
	ubicacion next = pos;
	switch (pos.brujula)
	{
	
	case norte:
		
		next.f = pos.f - 1;
		break;
	
	case noreste:
		
		next.f = pos.f - 1;
		next.c = pos.c + 1;
		break;
	
	case este:
		
		next.c = pos.c + 1;
		break;
	
	case sureste:
		
		next.f = pos.f + 1;
		next.c = pos.c + 1;
		break;
	
	case sur:
		
		next.f = pos.f + 1;
		break;
	
	case suroeste:
		
		next.f = pos.f + 1;
		next.c = pos.c - 1;
		break;
	
	case oeste:
		
		next.c = pos.c - 1;
		break;
	
	case noroeste:
		
		next.f = pos.f - 1;
		next.c = pos.c - 1;
		break;
	}

	return next;
}


// Devuelve el estado que se genera si el agente puede avanzar, si no, se devuelve con el que ya venía
stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char> > mapa){
	
	stateN0 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2; // sig_ubicacion2 es para el caso de actRUN
	
	switch (a)
	{
	
	case actWALK: //si prox casilla es transitable y no está ocupada por el colaborador
		
		sig_ubicacion = NextCasilla(st.jugador);
		
		if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
				
				st_result.jugador = sig_ubicacion;
		}
		
		break;
	
	case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
		
		sig_ubicacion = NextCasilla(st.jugador);
		
		if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){

			sig_ubicacion2 = NextCasilla(sig_ubicacion);

			if (CasillaTransitable(sig_ubicacion2, mapa) and !(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
					
				st_result.jugador = sig_ubicacion2;
			}
		}		
			
		break;

	// En el caso de los giros solo tendremos que cambiar la orientacion del jugador
	case actTURN_L:
		
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
		break;

	case actTURN_SR:
		
		st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
		break;
	}
	
	return st_result;
} 


// Encuentra si el elemento item está en la lista
bool Find(const stateN0 &item, const list<stateN0> &lista){
	
	auto it = lista.begin(); // obtener un iterador que apunta al primer elemento de la lista.
	
	while (it != lista.end() and !((*it) == item)) {

		it++;
	}
		
	return (!(it == lista.end()));
}


// Búsqueda en anchura
bool AnchuraSoloJugador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa) {

	stateN0 current_state = inicio;
	list<stateN0> abiertos; // frontier
	list<stateN0> cerrados; // explored

	abiertos.push_back(current_state);

	bool SolutionFound = (current_state.jugador.f == final.f and current_state.jugador.c == final.c);

	// Proceso de búsqueda
	while (!abiertos.empty() and !SolutionFound) {

		// Si no ha habido solución, sacas el nodo de abiertos y lo introduces en cerrados
		abiertos.pop_front();
		cerrados.push_back(current_state);

		// GENERAR DESCENDIENTES DEL ESTADO ACTUAL

		// Generar hijo actWALK
		stateN0 child_walk = apply(actWALK, current_state, mapa);
		
		// Si el nodo hijo tras andar es solución, se guarda en estado actual
		if (child_walk.jugador.f == final.f and child_walk.jugador.c == final.c){
			
			current_state = child_walk;
			SolutionFound = true;
		
		// Si el nodo hijo tras andar no es solución, y no está ni en abiertos ni en cerrados, se añade como nuevo estado a abiertos
		} else if (!Find(child_walk, abiertos) and !Find(child_walk, cerrados)){
			
			abiertos.push_back(child_walk);
		}

		// Condición para que no se generen estados hijos si ya se encontro la solucion
		if (!SolutionFound){
			
			// Generar hijo actRUN
			stateN0 child_run = apply(actRUN, current_state, mapa);
			
			if (child_run.jugador.f == final.f and child_run.jugador.c == final.c){
				
				current_state = child_run;
				SolutionFound = true;
			
			} else if (!Find(child_run, abiertos) and !Find(child_run, cerrados)){
				
				abiertos.push_back(child_run);
			}
		}

		// Condición para que no se generen estados hijos si ya se encontro la solucion
		// Aqui no se incluye la actualización de current_state, porque sin moverte no se puede alcanzar la casilla destino
		if (!SolutionFound){
			
			// Generar hijo actTURN_L
			stateN0 child_turnl = apply(actTURN_L, current_state, mapa);
			
			if (!Find(child_turnl, abiertos) and !Find(child_turnl, cerrados)){
				
				abiertos.push_back(child_turnl);
			}		
			
			// Generar hijo actTURN_SR
			stateN0 child_turnsr = apply(actTURN_SR, current_state, mapa);
			
			if (!Find(child_turnsr, abiertos) and !Find(child_turnsr, cerrados)){
				
				abiertos.push_back(child_turnsr);
			}		
		}

		// Si no se ha encontrado solución (ninguno de los descendientes es solucion) y sigue habiendo nodos en abiertos, el estado actual será el siguiente nodo de abiertos
		if (!SolutionFound and !abiertos.empty()) {

			current_state = abiertos.front();
		}
			
	}

	return SolutionFound;
}


// Devuelve la siguiente acción a realizar
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	if (plan.size() == 0) {

		cout << "Se completo el plan" << endl;
		hayPlan = false;
	}

	if (!hayPlan) {

		// Método de búsqueda
		cout << "Calculamos nuevo plan" << endl;
		c_state.jugador.f = sensores.posF;
		c_state.jugador.c = sensores.posC;
		c_state.jugador.brujula = sensores.sentido;
		c_state.colaborador.f = sensores.CLBposF;
		c_state.colaborador.c = sensores.CLBposC;
		c_state.colaborador.brujula = sensores.CLBsentido;
		goal.f = sensores.destinoF;
		goal.c = sensores.destinoC;

		hayPlan = AnchuraSoloJugador(c_state, goal, mapaResultado);

		if (hayPlan) {

			cout << "Se encontro un plan" << endl;
		}
	}

	if (hayPlan and plan.size() > 0) {

		accion = plan.front(); // La accion corresponde al primer elemento de la lista
		plan.pop_front(); // Sacar el primer elemento de la lista de acciones plan
	}


	return accion;
}


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}