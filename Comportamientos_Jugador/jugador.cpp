#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FUNCIONES AUXILIARES


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


// Pone los elementos de una matriz a 0
void AnulaMatriz(vector<vector<unsigned char> > &matriz){
	
	for (int i = 0; i < matriz.size(); i++) {

		for (int j = 0; j < matriz[0].size(); j++) {

			matriz[i][j] = 0;
		}
	}
}


// Pintar sobre el mapa el plan del jugador desde el estado inicial
void ComportamientoJugador::VisualizaPlan(const stateN0 &st, const list<Action> &plan)
{
	/*
		stateNO: ubicación agente jugador y ubicación agente colaborador

		mapaConPlan:
			0: casilla NO transitable
			1: casilla transitable por JUGADOR
			2: casilla transitable por COLABORADOR
			3: casilla intermedia en actRUN del jugador 
	*/
	
	AnulaMatriz(mapaConPlan);
	stateN0 cst = st;

	auto it = plan.begin(); // obtener un iterador que apunta al primer elemento de la lista
	
	while (it != plan.end())
	{

		// Si el agente jugador NO le ordeno nada al agente colaborador, este ultimo realizara la ultima accion ordenada por el jugador
		if ((*it != act_CLB_WALK) and (*it != act_CLB_TURN_SR) and (*it != act_CLB_STOP))
		{
			switch (cst.ultimaOrdenColaborador)
			{
			
			case act_CLB_WALK:
				
				cst.colaborador = NextCasilla(cst.colaborador);
				mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
				break;
			
			case act_CLB_TURN_SR:
				
				cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
				break;
			}
		}

		switch (*it)
		{
		
		case actRUN:
			
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 3;
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		
		case actWALK:
			
			cst.jugador = NextCasilla(cst.jugador);
			mapaConPlan[cst.jugador.f][cst.jugador.c] = 1;
			break;
		
		case actTURN_SR:
			
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 1) % 8);
			break;
		
		case actTURN_L:
			
			cst.jugador.brujula = (Orientacion)((cst.jugador.brujula + 6) % 8);
			break;
		
		case act_CLB_WALK:
			
			cst.colaborador = NextCasilla(cst.colaborador);
			cst.ultimaOrdenColaborador = act_CLB_WALK;
			mapaConPlan[cst.colaborador.f][cst.colaborador.c] = 2;
			break;
		
		case act_CLB_TURN_SR:
			
			cst.colaborador.brujula = (Orientacion)((cst.colaborador.brujula + 1) % 8);
			cst.ultimaOrdenColaborador = act_CLB_TURN_SR;
			break;
		
		case act_CLB_STOP:
			
			cst.ultimaOrdenColaborador = act_CLB_STOP;
			break;
		}
		
		it++;
	}
}


// Mostrar en texto secuencia de acciones del plan obtenido
void PintaPlan(const list<Action> &plan)
{
	auto it = plan.begin();
	
	while (it != plan.end())
	{
		if (*it == actWALK){
			
			cout << "W ";
		
		} else if (*it == actRUN){
			
			cout << "R ";
		
		} else if (*it == actTURN_SR){
			
			cout << "r ";
		
		} else if (*it == actTURN_L){
			
			cout << "L ";
		
		} else if (*it == act_CLB_WALK){
			
			cout << "cW ";
		
		} else if (*it == act_CLB_TURN_SR){
			
			cout << "cr ";
		
		} else if (*it == act_CLB_STOP){
			
			cout << "cS ";
		
		} else if (*it == actIDLE){
			
			cout << "I ";
		
		} else{
		
			cout << "-_ ";
		
		}
		
		it++;
	}
	
	cout << " (" << plan.size() << " acciones)\n";
}


// Encuentra si el elemento item está en la lista
bool Find(const stateN0 &item, const list<stateN0> &lista){
	
	auto it = lista.begin(); // obtener un iterador que apunta al primer elemento de la lista
	
	while (it != lista.end() and !((*it) == item)) {

		it++;
	}
		
	return (!(it == lista.end()));
}


// Sobrecarga de la función Find paras buscar en lista de nodeN0
bool Find(const stateN0 &item, const list<nodeN0> &lista){
	
	auto it = lista.begin();
	
	while (it != lista.end() and !(it->st == item)) {

		it++;
	}

	return (!(it == lista.end()));
}




//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// NIVEL 0 --> TUTORIAL


// Devuelve el estado que se genera si el agente puede avanzar, si no, se devuelve con el que ya venía
stateN0 apply(const Action &a, const stateN0 &st, const vector<vector<unsigned char> > mapa){
	
	stateN0 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2; // sig_ubicacion2 es para el caso de actRUN
	
	// En niveles posteriores habra que tener en cuenta al agente colaborador

	switch (a)
	{

		// CASOS DEL COLABORADOR (modificar el estado del colaborador, pero no el del jugador)
		case act_CLB_WALK:    

			st_result.ultimaOrdenColaborador = act_CLB_WALK;
		
			sig_ubicacion = NextCasilla(st.colaborador);

			if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.jugador.f and sig_ubicacion.c == st.jugador.c)) {

				st_result.colaborador = sig_ubicacion;
			}

			break;


		case act_CLB_TURN_SR:   

			st_result.ultimaOrdenColaborador = act_CLB_TURN_SR;

			st_result.colaborador.brujula = static_cast<Orientacion>((st_result.colaborador.brujula + 1) % 8);
			
			break;


		case act_CLB_STOP:

			st_result.ultimaOrdenColaborador = act_CLB_STOP;

			break;


		// CASOS DEL JUGADOR (modificar ambos estados tanto jugador como colaborador)
		case actWALK: //si prox casilla es transitable y no está ocupada por el colaborador
			
			// Estado colaborador
			st_result = apply(st_result.ultimaOrdenColaborador, st_result, mapa);
			
			// Estado jugador
			sig_ubicacion = NextCasilla(st.jugador);
			
			if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){
					
				st_result.jugador = sig_ubicacion;
			}
			
			break;
		

		case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
			
			// Estado colaborador
			st_result = apply(st_result.ultimaOrdenColaborador, st_result, mapa);
			
			// Estado jugador
			sig_ubicacion = NextCasilla(st.jugador);
			
			if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.colaborador.f and sig_ubicacion.c == st.colaborador.c)){

				sig_ubicacion2 = NextCasilla(sig_ubicacion);

				if (CasillaTransitable(sig_ubicacion2, mapa) and !(sig_ubicacion2.f == st.colaborador.f and sig_ubicacion2.c == st.colaborador.c)){
						
					st_result.jugador = sig_ubicacion2;
				}
			}		
				
			break;

		
		case actTURN_L: // En el caso de los giros solo tendremos que cambiar la orientacion del jugador
			
			// Estado colaborador
			st_result = apply(st_result.ultimaOrdenColaborador, st_result, mapa);
		
			// Estado jugador
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+6)%8);
			break;


		case actTURN_SR:

			// Estado colaborador
			st_result = apply(st_result.ultimaOrdenColaborador, st_result, mapa);
			
			// Estado jugador
			st_result.jugador.brujula = static_cast<Orientacion>((st_result.jugador.brujula+1)%8);
			break;

		
		case actIDLE:

			// Estado colaborador
			st_result = apply(st_result.ultimaOrdenColaborador, st_result, mapa);

			break;
		
	}
	
	return st_result;
}


// Version 1 --> Búsqueda en anchura
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


// Version 2 --> Búsqueda en anchura 2
list<Action> AnchuraSoloJugador_V2(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa) {

	nodeN0 current_node;
	list<nodeN0> abiertos; // frontier
	list<nodeN0> cerrados; // explored
	list<Action> plan;

	current_node.st = inicio;
	
	bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);

	abiertos.push_back(current_node);

	// Proceso de búsqueda
	while (!abiertos.empty() and !SolutionFound) {

		// Si no ha habido solución, sacas el nodo de abiertos y lo introduces en cerrados
		abiertos.pop_front();
		cerrados.push_back(current_node);

		// GENERAR DESCENDIENTES DEL ESTADO ACTUAL

		// Generar hijo actWALK
		nodeN0 child_walk = current_node; 
		child_walk.st = apply(actWALK, current_node.st, mapa);
		
		// Guardar la accion en secuencia
		child_walk.secuencia.push_back(actWALK);

		// Si el nodo hijo tras andar es solución, se guarda en estado actual
		if (child_walk.st.jugador.f == final.f and child_walk.st.jugador.c == final.c){
			
			current_node = child_walk;
			SolutionFound = true;
		
		// Si el nodo hijo tras andar no es solución, y no está ni en abiertos ni en cerrados, se añade como nuevo estado a abiertos
		} else if (!Find(child_walk.st, abiertos) and !Find(child_walk.st, cerrados)){
			
			abiertos.push_back(child_walk);
		}

		// Condición para que no se generen estados hijos si ya se encontro la solucion
		if (!SolutionFound){
			
			// Generar hijo actRUN
			nodeN0 child_run = current_node;
			child_run.st = apply(actRUN, current_node.st, mapa);
			
			// Guardar accion en secuencia
			child_run.secuencia.push_back(actRUN);

			if (child_run.st.jugador.f == final.f and child_run.st.jugador.c == final.c){
				
				current_node = child_run;
				SolutionFound = true;
			
			} else if (!Find(child_run.st, abiertos) and !Find(child_run.st, cerrados)){
				
				abiertos.push_back(child_run);
			}
		}

		// Condición para que no se generen estados hijos si ya se encontro la solucion
		// Aqui no se incluye la actualización de current_state, porque sin moverte no se puede alcanzar la casilla destino
		if (!SolutionFound){
			
			// Generar hijo actTURN_L
			nodeN0 child_turnl = current_node; 
			child_turnl.st = apply(actTURN_L, current_node.st, mapa);

			// Guardar accion en secuencia
			child_turnl.secuencia.push_back(actTURN_L);
			
			if (!Find(child_turnl.st, abiertos) and !Find(child_turnl.st, cerrados)){
				
				abiertos.push_back(child_turnl);
			}		
			
			// Generar hijo actTURN_SR
			nodeN0 child_turnsr = current_node;
			child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
			
			// Guardar accion en secuencia
			child_turnsr.secuencia.push_back(actTURN_SR);

			if (!Find(child_turnsr.st, abiertos) and !Find(child_turnsr.st, cerrados)){
				
				abiertos.push_back(child_turnsr);
			}		
		}

		// Si no se ha encontrado solución (ninguno de los descendientes es solucion) y sigue habiendo nodos en abiertos, el estado actual será el siguiente nodo de abiertos
		if (!SolutionFound and !abiertos.empty()) {

			current_node = abiertos.front();
		}
			
	}

	// Si ha encontrado solución guardar secuencia y pintarlo
	if (SolutionFound) {

		plan = current_node.secuencia;

		cout << "Encontrado un plan" << endl;
		PintaPlan(plan);
	}

	return plan;
}


// Version 3 --> Búsqueda en anchura 3
list<Action> AnchuraSoloJugador_V3(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa) {

	nodeN0 current_node;
	list<nodeN0> abiertos; // frontier
	set<nodeN0> cerrados; // explored --> la búsqueda de nodos se hace sobre cerrados y la estructura set es más eficiente que la estructura list
	list<Action> plan;

	// De primeras no puedo situar ni a jugador ni a colaborador en una casilla no transitable
	if (CasillaTransitable(inicio.jugador, mapa) and CasillaTransitable(inicio.colaborador, mapa)) {

		current_node.st = inicio;
		abiertos.push_back(current_node);

		bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);

		// Proceso de búsqueda
		while (!abiertos.empty() and !SolutionFound) {

			// Si no ha habido solución, sacas el nodo de abiertos y lo introduces en cerrados
			abiertos.pop_front();
			cerrados.insert(current_node);

			// GENERAR DESCENDIENTES DEL ESTADO ACTUAL

			// Generar hijo actWALK
			nodeN0 child_walk = current_node; 
			child_walk.st = apply(actWALK, current_node.st, mapa);
			
			// Guardar la accion en secuencia
			child_walk.secuencia.push_back(actWALK);

			// Si el nodo hijo tras andar es solución, se guarda en estado actual
			if (child_walk.st.jugador.f == final.f and child_walk.st.jugador.c == final.c){
				
				current_node = child_walk;
				SolutionFound = true;
			
			// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
			} else if (cerrados.find(child_walk) == cerrados.end()){
				
				abiertos.push_back(child_walk);
			}

			// Condición para que no se generen estados hijos si ya se encontro la solucion
			if (!SolutionFound){
				
				// Generar hijo actRUN
				nodeN0 child_run = current_node;
				child_run.st = apply(actRUN, current_node.st, mapa);
				
				// Guardar accion en secuencia
				child_run.secuencia.push_back(actRUN);

				if (child_run.st.jugador.f == final.f and child_run.st.jugador.c == final.c){
					
					current_node = child_run;
					SolutionFound = true;
				
				// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
				} else if (cerrados.find(child_run) == cerrados.end()){
					
					abiertos.push_back(child_run);
				}
			}

			// Condición para que no se generen estados hijos si ya se encontro la solucion
			// Aqui no se incluye la actualización de current_state, porque sin moverte no se puede alcanzar la casilla destino
			if (!SolutionFound){
				
				// Generar hijo actTURN_L
				nodeN0 child_turnl = current_node; 
				child_turnl.st = apply(actTURN_L, current_node.st, mapa);

				// Guardar accion en secuencia
				child_turnl.secuencia.push_back(actTURN_L);
				
				// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
				if (cerrados.find(child_turnl) == cerrados.end()){
					
					abiertos.push_back(child_turnl);
				}		
				
				// Generar hijo actTURN_SR
				nodeN0 child_turnsr = current_node;
				child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
				
				// Guardar accion en secuencia
				child_turnsr.secuencia.push_back(actTURN_SR);

				// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
				if (cerrados.find(child_turnsr) == cerrados.end()){
					
					abiertos.push_back(child_turnsr);
				}		
			}

			// Si no se ha encontrado solución (ninguno de los descendientes es solucion) y sigue habiendo nodos en abiertos, el estado actual será el siguiente nodo de abiertos
			if (!SolutionFound and !abiertos.empty()) {

				current_node = abiertos.front();

				while (!abiertos.empty() and cerrados.find(current_node) != cerrados.end()) {

					abiertos.pop_front();

					if (!abiertos.empty()) {

						current_node = abiertos.front();
					}
				}
			}
		}

		// Si ha encontrado solución guardar secuencia y pintarlo
		if (SolutionFound) {

			plan = current_node.secuencia;

			cout << "Encontrado un plan" << endl;
			PintaPlan(plan);
		}
	
	} else {

		cerr << "Casilla no transitable" << endl;
		exit(EXIT_FAILURE);
	} 

	return plan;
}




//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// NIVEL 1


// Función para ver al colaborador
bool VeoColaborador(const stateN0 &st) {

	bool veocolaborador = false;

	// Ver si en las casillas delanteras tengo al colaborador y si es en rango correcto
	if (st.jugador.f >= 0 and st.jugador.f <= 99 and st.jugador.c >= 0 and st.jugador.c <= 99 and 
		st.colaborador.f >= 0 and st.colaborador.f <= 99 and st.colaborador.c >= 0 and st.colaborador.c <= 99 ) {

		switch (st.jugador.brujula)
		{
			
			case norte: // acabado

				if ((st.jugador.f - st.colaborador.f == 1 && st.jugador.c - st.colaborador.c == 1)  ||
				    (st.jugador.f - st.colaborador.f == 1 && st.jugador.c - st.colaborador.c == 0)  ||
					(st.jugador.f - st.colaborador.f == 1 && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == 2)  ||
					(st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == 1)  ||
					(st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == 0)  ||
					(st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == -2) ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == 3)  ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == 2)  ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == 1)  ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == 0)  ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == -2) ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == -3)){

					veocolaborador = true;
				}

				break;

			case noreste: // acabado

				if ((st.jugador.f - st.colaborador.f == 1 && st.jugador.c - st.colaborador.c == 0)  ||
				    (st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == 0)  ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == 0)  ||
					(st.jugador.f - st.colaborador.f == 1 && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == 1 && st.jugador.c - st.colaborador.c == -2) ||
					(st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == -2) ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == -2) ||
					(st.jugador.f - st.colaborador.f == 1 && st.jugador.c - st.colaborador.c == -3) ||
					(st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == -3) ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == -3) ||
					(st.jugador.f - st.colaborador.f == 0 && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == 0 && st.jugador.c - st.colaborador.c == -2) ||
					(st.jugador.f - st.colaborador.f == 0 && st.jugador.c - st.colaborador.c == -3)){

					veocolaborador = true;
				}

				break;

			case este: // acabado

				if ((st.jugador.f - st.colaborador.f == 1  && st.jugador.c - st.colaborador.c == -1)  ||
				    (st.jugador.f - st.colaborador.f == 0  && st.jugador.c - st.colaborador.c == -1)  ||
					(st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == -1)  ||
					(st.jugador.f - st.colaborador.f == 2  && st.jugador.c - st.colaborador.c == -2)  ||
					(st.jugador.f - st.colaborador.f == 1  && st.jugador.c - st.colaborador.c == -2)  ||
					(st.jugador.f - st.colaborador.f == 0  && st.jugador.c - st.colaborador.c == -2)  ||
					(st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == -2)  ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == -2)  ||
					(st.jugador.f - st.colaborador.f == 3  && st.jugador.c - st.colaborador.c == -3)  ||
					(st.jugador.f - st.colaborador.f == 2  && st.jugador.c - st.colaborador.c == -3)  ||
					(st.jugador.f - st.colaborador.f == 1  && st.jugador.c - st.colaborador.c == -3)  ||
					(st.jugador.f - st.colaborador.f == 0  && st.jugador.c - st.colaborador.c == -3)  ||
					(st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == -3)  ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == -3)  ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == -3)){

					veocolaborador = true;
				}

				break;

			case sureste: // acabado

				if ((st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == 0)  ||
				    (st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == 0)  ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == 0)  ||
					(st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == -2) ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == -2) ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == -2) ||
					(st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == -3) ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == -3) ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == -3) ||
					(st.jugador.f - st.colaborador.f == 0  && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == 0  && st.jugador.c - st.colaborador.c == -2) ||
					(st.jugador.f - st.colaborador.f == 0  && st.jugador.c - st.colaborador.c == -3)){

					veocolaborador = true;
				}

				break;

			case sur: // acabado

				if ((st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == -1) ||
				    (st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == 0)  ||
					(st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == 1)  ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == -2) ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == 0)  ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == 1)  ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == 2)  ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == -3) ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == -2) ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == -1) ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == 0)  ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == 1)  ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == 2)  ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == 3)){

					veocolaborador = true;
				}

				break;

			case suroeste: // acabado

				if ((st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == 0) ||
				    (st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == 0) ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == 0) ||
					(st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == 1) ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == 1) ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == 1) ||
					(st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == 2) ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == 2) ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == 2) ||
					(st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == 3) ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == 3) ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == 3) ||
					(st.jugador.f - st.colaborador.f == 0  && st.jugador.c - st.colaborador.c == 1) ||
					(st.jugador.f - st.colaborador.f == 0  && st.jugador.c - st.colaborador.c == 2) ||
					(st.jugador.f - st.colaborador.f == 0  && st.jugador.c - st.colaborador.c == 3)){

					veocolaborador = true;
				}
				
				break;

			case oeste: // acabado

				if ((st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == 1) ||
				    (st.jugador.f - st.colaborador.f == 0  && st.jugador.c - st.colaborador.c == 1) ||
					(st.jugador.f - st.colaborador.f == 1  && st.jugador.c - st.colaborador.c == 1) ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == 2) ||
					(st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == 2) ||
					(st.jugador.f - st.colaborador.f == 0  && st.jugador.c - st.colaborador.c == 2) ||
					(st.jugador.f - st.colaborador.f == 1  && st.jugador.c - st.colaborador.c == 2) ||
					(st.jugador.f - st.colaborador.f == 2  && st.jugador.c - st.colaborador.c == 2) ||
					(st.jugador.f - st.colaborador.f == -3 && st.jugador.c - st.colaborador.c == 3) ||
					(st.jugador.f - st.colaborador.f == -2 && st.jugador.c - st.colaborador.c == 3) ||
					(st.jugador.f - st.colaborador.f == -1 && st.jugador.c - st.colaborador.c == 3) ||
					(st.jugador.f - st.colaborador.f == 0  && st.jugador.c - st.colaborador.c == 3) ||
					(st.jugador.f - st.colaborador.f == 1  && st.jugador.c - st.colaborador.c == 3) ||
					(st.jugador.f - st.colaborador.f == 2  && st.jugador.c - st.colaborador.c == 3) ||
					(st.jugador.f - st.colaborador.f == 3  && st.jugador.c - st.colaborador.c == 3)){

					veocolaborador = true;
				}

				break;

			case noroeste: // acabado

				if ((st.jugador.f - st.colaborador.f == 1 && st.jugador.c - st.colaborador.c == 0)  ||
				    (st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == 0)  ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == 0)  ||
					(st.jugador.f - st.colaborador.f == 1 && st.jugador.c - st.colaborador.c == 1)  ||
					(st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == 1)  ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == 1)  ||
					(st.jugador.f - st.colaborador.f == 1 && st.jugador.c - st.colaborador.c == 2)  ||
					(st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == 2)  ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == 2)  ||
					(st.jugador.f - st.colaborador.f == 1 && st.jugador.c - st.colaborador.c == 3)  ||
					(st.jugador.f - st.colaborador.f == 2 && st.jugador.c - st.colaborador.c == 3)  ||
					(st.jugador.f - st.colaborador.f == 3 && st.jugador.c - st.colaborador.c == 3)  ||
					(st.jugador.f - st.colaborador.f == 0 && st.jugador.c - st.colaborador.c == 1)  ||
					(st.jugador.f - st.colaborador.f == 0 && st.jugador.c - st.colaborador.c == 2)  ||
					(st.jugador.f - st.colaborador.f == 0 && st.jugador.c - st.colaborador.c == 3)){

					veocolaborador = true;
				}
				
				break;
		}
	}

	return veocolaborador;
}


// Búsqueda en anchura colaborador
list<Action> AnchuraSoloColaborador(const stateN0 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa) {

	nodeN0 current_node;
	list<nodeN0> abiertos; // frontier
	set<nodeN0> cerrados; // explored --> la búsqueda de nodos se hace sobre cerrados y la estructura set es más eficiente que la estructura list
	list<Action> plan;

	// De primeras no puedo situar ni a jugador ni a colaborador en una casilla no transitable
	if (CasillaTransitable(inicio.jugador, mapa) and CasillaTransitable(inicio.colaborador, mapa)) {

		current_node.st = inicio;
		abiertos.push_back(current_node);

		bool SolutionFound = (current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c);

		// Proceso de búsqueda
		while (!abiertos.empty() and !SolutionFound) {

			// Si no ha habido solución, sacas el nodo de abiertos y lo introduces en cerrados
			abiertos.pop_front();
			cerrados.insert(current_node);


			// GENERAR DESCENDIENTES DEL ESTADO ACTUAL

			/*
					*Si jugador ve a colaborador, el numero de nodos descendientes aumentara
					* Funcion que te diga si ve al colaborador o no
					* Modificar la estructura estado y nodo con respecto ultima accion colaborador
					* Modificar la correcta inicialización de ultimaOrdenColaborador --> De primeras esta parado
					* Modificar la función apply porque tenemos mas opciones
					* Modificar el operator <
			*/

			// if(!SolutionFound) --> Condición para que no se generen estados hijos si ya se encontro la solucion
	

			// GENERAR HIJOS COLABORADOR
			if (VeoColaborador(current_node.st)) {

				// Generar hijo act_CLB_WALK
				nodeN0 child_clb_walk = current_node;
				child_clb_walk.st = apply(act_CLB_WALK, current_node.st, mapa);

				// Guardar la acción en secuencia
				child_clb_walk.secuencia.push_back(act_CLB_WALK);

				// Si el nodo hijo tras andar es solución, se guarda en estado actual
				if (child_clb_walk.st.colaborador.f == final.f and child_clb_walk.st.colaborador.c == final.c){
					
					current_node = child_clb_walk;
					SolutionFound = true;
				
				// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
				} else if (cerrados.find(child_clb_walk) == cerrados.end()){
					
					abiertos.push_back(child_clb_walk);
				}


				if (!SolutionFound) {

					// Generar hijo act_CLB_TURN_SR
					nodeN0 child_clb_turnsr = current_node;
					child_clb_turnsr.st = apply(act_CLB_TURN_SR, current_node.st, mapa);
				
					// Guardar accion en secuencia
					child_clb_turnsr.secuencia.push_back(act_CLB_TURN_SR);

					// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
					if (cerrados.find(child_clb_turnsr) == cerrados.end()){
						
						abiertos.push_back(child_clb_turnsr);
					}
				}


				if (!SolutionFound and (current_node.st.ultimaOrdenColaborador == act_CLB_WALK or current_node.st.ultimaOrdenColaborador == act_CLB_TURN_SR)) {

					// Generar hijo act_CLB_STOP si la ultima fue andar o girar
					nodeN0 child_clb_stop= current_node;
					child_clb_stop.st = apply(act_CLB_STOP, current_node.st, mapa);
					
					// Guardar accion en secuencia
					child_clb_stop.secuencia.push_back(act_CLB_STOP);

					// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
					if (cerrados.find(child_clb_stop) == cerrados.end()){
						
						abiertos.push_back(child_clb_stop);
					}
				}
			}


			// GENERAR HIJOS JUGADOR
			if (!SolutionFound) {

				// Generar hijo actWALK
				nodeN0 child_walk = current_node; 
				child_walk.st = apply(actWALK, current_node.st, mapa);
				
				// Guardar la accion en secuencia
				child_walk.secuencia.push_back(actWALK);

				// Comprobar si el colaborador ha llegado a la casilla destino
				if (current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c) {

					SolutionFound = true;
				}

				if (cerrados.find(child_walk) == cerrados.end()){
					
					abiertos.push_back(child_walk);
				}
			}
			
			
			if (!SolutionFound){
				
				// Generar hijo actRUN
				nodeN0 child_run = current_node;
				child_run.st = apply(actRUN, current_node.st, mapa);
				
				// Guardar accion en secuencia
				child_run.secuencia.push_back(actRUN);

				// Comprobar si el colaborador ha llegado a la casilla destino
				if (current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c) {

					SolutionFound = true;
				}

				if (cerrados.find(child_run) == cerrados.end()){
					
					abiertos.push_back(child_run);
				}
			}

			
			if (!SolutionFound){
				
				// Generar hijo actTURN_L
				nodeN0 child_turnl = current_node; 
				child_turnl.st = apply(actTURN_L, current_node.st, mapa);

				// Guardar accion en secuencia
				child_turnl.secuencia.push_back(actTURN_L);

				// Comprobar si el colaborador ha llegado a la casilla destino
				if (current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c) {

					SolutionFound = true;
				}
				
				// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
				if (cerrados.find(child_turnl) == cerrados.end()){
					
					abiertos.push_back(child_turnl);
				}		
				
				// Generar hijo actTURN_SR
				nodeN0 child_turnsr = current_node;
				child_turnsr.st = apply(actTURN_SR, current_node.st, mapa);
				
				// Guardar accion en secuencia
				child_turnsr.secuencia.push_back(actTURN_SR);

				// Comprobar si el colaborador ha llegado a la casilla destino
				if (current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c) {

					SolutionFound = true;
				}

				// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
				if (cerrados.find(child_turnsr) == cerrados.end()){
					
					abiertos.push_back(child_turnsr);
				}		
			}


			if (!SolutionFound) {

				// Generar hijo actIDLE
				nodeN0 child_idle = current_node; 
				child_idle.st = apply(actIDLE, current_node.st, mapa);

				// Guardar accion en secuencia
				child_idle.secuencia.push_back(actIDLE);

				// Comprobar si el colaborador ha llegado a la casilla destino
				if (current_node.st.colaborador.f == final.f and current_node.st.colaborador.c == final.c) {

					SolutionFound = true;
				}
				
				// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
				if (cerrados.find(child_idle) == cerrados.end()){
					
					abiertos.push_back(child_idle);
				}	
			}


			// Si no se ha encontrado solución (ninguno de los descendientes es solucion) y sigue habiendo nodos en abiertos, el estado actual será el siguiente nodo de abiertos
			if (!SolutionFound and !abiertos.empty()) {

				current_node = abiertos.front();

				while (!abiertos.empty() and cerrados.find(current_node) != cerrados.end()) {

					abiertos.pop_front();

					if (!abiertos.empty()) {

						current_node = abiertos.front();
					}
				}
			}
		}

		// Si ha encontrado solución guardar secuencia y pintarlo
		if (SolutionFound) {

			plan = current_node.secuencia;

			cout << "Encontrado un plan" << endl;
			PintaPlan(plan);
		}


	} else {

		cerr << "Casilla no transitable" << endl;
		exit(EXIT_FAILURE);
	}



	return plan;
}




//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// NIVEL 2


// TODO: Calcula el coste acumulado de un nodo
int actualizarCosteNodo(const Action &a, nodeN2 &nodo, const vector<vector<unsigned char> > mapa) {

	// Consumo influye el tipo de terreno, la acción aplicada y si se está en posesión o no del objeto que permite reducir el consumo

	// Comprobar en que terreno estoy y si dispongo de objetos: B, A, S, T, K, D, X
	// Comprobar los costes de cada accion
	// Comprobar la accion a realizar
	// Actualizar estado zapatillas y bikini: solo puedo tener una de ellas.

	switch (a)
	{

		case actWALK:

			if (mapa[nodo.st.jugador.f][nodo.st.jugador.c] == 'B') {

				if (nodo.st.tengo_zapatillas) {

					nodo.coste += 15;
				
				} else {

					nodo.coste += 50;
				}
			
			} else if (mapa[nodo.st.jugador.f][nodo.st.jugador.c] == 'A') {

				if (nodo.st.tengo_bikini) {

					nodo.coste += 10;
				
				} else {

					nodo.coste += 100;
				}
			
			} else if (mapa[nodo.st.jugador.f][nodo.st.jugador.c] == 'T') {

				nodo.coste += 2;
			
			} else {

				nodo.coste += 1;
			}

			break;


		case actRUN:

			if (mapa[nodo.st.jugador.f][nodo.st.jugador.c] == 'B') {

				if (nodo.st.tengo_zapatillas) {

					nodo.coste += 25;
				
				} else {

					nodo.coste += 75;
				}
			
			} else if (mapa[nodo.st.jugador.f][nodo.st.jugador.c] == 'A') {

				if (nodo.st.tengo_bikini) {

					nodo.coste += 15;
				
				} else {

					nodo.coste += 150;
				}
			
			} else if (mapa[nodo.st.jugador.f][nodo.st.jugador.c] == 'T') {

				nodo.coste += 3;
			
			} else {

				nodo.coste += 1;
			}

			break;


		case actTURN_L:

			if (mapa[nodo.st.jugador.f][nodo.st.jugador.c] == 'B') {

				if (nodo.st.tengo_zapatillas) {

					nodo.coste += 1;
				
				} else {

					nodo.coste += 7;
				}
			
			} else if (mapa[nodo.st.jugador.f][nodo.st.jugador.c] == 'A') {

				if (nodo.st.tengo_bikini) {

					nodo.coste += 5;
				
				} else {

					nodo.coste += 30;
				}
			
			} else if (mapa[nodo.st.jugador.f][nodo.st.jugador.c] == 'T') {

				nodo.coste += 2;
			
			} else {

				nodo.coste += 1;
			}

			break;


		case actTURN_SR:

			if (mapa[nodo.st.jugador.f][nodo.st.jugador.c] == 'B') {

				if (nodo.st.tengo_zapatillas) {

					nodo.coste += 1;
				
				} else {

					nodo.coste += 5;
				}
			
			} else if (mapa[nodo.st.jugador.f][nodo.st.jugador.c] == 'A') {

				if (nodo.st.tengo_bikini) {

					nodo.coste += 2;
				
				} else {

					nodo.coste += 10;
				}
			
			} else if (mapa[nodo.st.jugador.f][nodo.st.jugador.c] == 'T') {

				nodo.coste += 1;
			
			} else {

				nodo.coste += 1;
			}
			
			break;


		case actIDLE:

			nodo.coste += 0;

			break;
	}


	return nodo.coste;

}


// TODO: Devuelve el estado que se genera si el agente puede avanzar, si no, se devuelve con el que ya venía
nodeN2 apply2(const Action &a, const nodeN2 &st, const vector<vector<unsigned char> > mapa){
	
	nodeN2 st_result = st;
	ubicacion sig_ubicacion, sig_ubicacion2; // sig_ubicacion2 es para el caso de actRUN
	
	switch (a)
	{

		// ACTUALIZAR COSTE DE CADA NODO

		case actWALK: //si prox casilla es transitable y no está ocupada por el colaborador
			
			// Actualizar nodo jugador anda

			// Coste
			st_result.coste = actualizarCosteNodo(actWALK, st_result, mapa);

			// Estado (ubicacion)
			sig_ubicacion = NextCasilla(st.st.jugador);
			
			if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.st.colaborador.f and sig_ubicacion.c == st.st.colaborador.c)){
					
				st_result.st.jugador = sig_ubicacion;
			}

			// Estado (bikini y zapatillas)
			if (mapa[st_result.st.jugador.f][st_result.st.jugador.c] == 'K') {

				st_result.st.tengo_zapatillas = false;
				st_result.st.tengo_bikini = true;
			}

			if (mapa[st_result.st.jugador.f][st_result.st.jugador.c] == 'D') {

				st_result.st.tengo_zapatillas = true;
				st_result.st.tengo_bikini = false;
			}

			break;
		

		case actRUN: //si prox 2 casillas son transitables y no está ocupada por el colaborador
			
			// Actualizar nodo jugador corre

			// Coste
			st_result.coste = actualizarCosteNodo(actRUN, st_result, mapa);

			// Estado (ubicacion)
			sig_ubicacion = NextCasilla(st.st.jugador);
			
			if (CasillaTransitable(sig_ubicacion, mapa) and !(sig_ubicacion.f == st.st.colaborador.f and sig_ubicacion.c == st.st.colaborador.c)){

				sig_ubicacion2 = NextCasilla(sig_ubicacion);

				if (CasillaTransitable(sig_ubicacion2, mapa) and !(sig_ubicacion2.f == st.st.colaborador.f and sig_ubicacion2.c == st.st.colaborador.c)){
						
					st_result.st.jugador = sig_ubicacion2;
				}
			}	

			// Estado (bikini y zapatillas)
			if (mapa[st_result.st.jugador.f][st_result.st.jugador.c] == 'K') {

				st_result.st.tengo_zapatillas = false;
				st_result.st.tengo_bikini = true;
			}

			if (mapa[st_result.st.jugador.f][st_result.st.jugador.c] == 'D') {

				st_result.st.tengo_zapatillas = true;
				st_result.st.tengo_bikini = false;
			}
				
			break;

		
		case actTURN_L: // En el caso de los giros solo tendremos que cambiar la orientacion del jugador
			
			// Actualizar nodo jugador gira izquierda

			// Coste
			st_result.coste = actualizarCosteNodo(actTURN_L, st_result, mapa);

			// Estado (ubicacion)
			st_result.st.jugador.brujula = static_cast<Orientacion>((st_result.st.jugador.brujula+6)%8);

			break;


		case actTURN_SR:
			
			// Actualizar nodo jugador gira derecha

			// Coste
			st_result.coste = actualizarCosteNodo(actTURN_SR, st_result, mapa);

			// Estado (ubicacion)
			st_result.st.jugador.brujula = static_cast<Orientacion>((st_result.st.jugador.brujula+1)%8);

			break;

		
		case actIDLE:

			// Actualizar nodo jugador no hace nada

			// Coste
			st_result.coste = actualizarCosteNodo(actIDLE, st_result, mapa);

			break;
		
	}
	
	return st_result;
} 


// TODO: Búsqueda en Dijkstra jugador
list<Action> DijkstraSoloJugador(const stateN2 &inicio, const ubicacion &final, const vector<vector<unsigned char>> &mapa) {

	nodeN2 current_node;
	priority_queue<nodeN2> abiertos; // frontier
	set<stateN2> cerrados; // explored --> la búsqueda de nodos se hace sobre cerrados y la estructura set es más eficiente que la estructura list
	list<Action> plan;

	// De primeras no puedo situar ni a jugador ni a colaborador en una casilla no transitable
	if (CasillaTransitable(inicio.jugador, mapa) and CasillaTransitable(inicio.colaborador, mapa)) {

		current_node.st = inicio;
		current_node.coste = 0;

		abiertos.push(current_node);

		bool SolutionFound = (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c);

		// Proceso de búsqueda
		while (!abiertos.empty() and !SolutionFound) {

			// DIJKSTRA: solo cuando se saca un nuevo nodo de abierto o antes de entrar en cerrados hay solucion

			// Si no ha habido solución, sacas el nodo de abiertos y antes de entrar en cerrados compruebas si hay solucion
			abiertos.pop();
			
			if (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c) {

				SolutionFound = true;
			}
			
			cerrados.insert(current_node.st);

			// GENERAR DESCENDIENTES DEL ESTADO ACTUAL
			// if(!SolutionFound) --> Condición para que no se generen estados hijos si ya se encontro la solucion

			/* OPERACIONES A REALIZAR

				* Añadir nueva estructura y estado e inicializarlos bien
				* Modificar apply convenientemente
				* Modificar dijstra convenientemente
			*/

	
			// GENERAR HIJOS JUGADOR
			if (!SolutionFound) {

				// Generar hijo actWALK
				nodeN2 child_walk = current_node; 
				child_walk = apply2(actWALK, current_node, mapa);
				
				// Guardar la accion en secuencia
				child_walk.secuencia.push_back(actWALK);

				if (cerrados.find(child_walk.st) == cerrados.end()){
					
					abiertos.push(child_walk);
				}
			}
			
			
			if (!SolutionFound){
				
				// Generar hijo actRUN
				nodeN2 child_run = current_node;
				child_run = apply2(actRUN, current_node, mapa);
				
				// Guardar accion en secuencia
				child_run.secuencia.push_back(actRUN);

				if (cerrados.find(child_run.st) == cerrados.end()){
					
					abiertos.push(child_run);
				}
			}

			
			if (!SolutionFound){
				
				// Generar hijo actTURN_L
				nodeN2 child_turnl = current_node; 
				child_turnl = apply2(actTURN_L, current_node, mapa);

				// Guardar accion en secuencia
				child_turnl.secuencia.push_back(actTURN_L);

				// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
				if (cerrados.find(child_turnl.st) == cerrados.end()){
					
					abiertos.push(child_turnl);
				}		
				
				// Generar hijo actTURN_SR
				nodeN2 child_turnsr = current_node;
				child_turnsr = apply2(actTURN_SR, current_node, mapa);
				
				// Guardar accion en secuencia
				child_turnsr.secuencia.push_back(actTURN_SR);

				// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
				if (cerrados.find(child_turnsr.st) == cerrados.end()){
					
					abiertos.push(child_turnsr);
				}		
			}


			if (!SolutionFound) {

				// Generar hijo actIDLE
				nodeN2 child_idle = current_node; 
				child_idle = apply2(actIDLE, current_node, mapa);

				// Guardar accion en secuencia
				child_idle.secuencia.push_back(actIDLE);
				
				// Si no lo encuentra en cerrados el find devuelve end, por eso lo introduce en abiertos
				if (cerrados.find(child_idle.st) == cerrados.end()){
					
					abiertos.push(child_idle);
				}	
			}


			// Si no se ha encontrado solución (ninguno de los descendientes es solucion) y sigue habiendo nodos en abiertos, el estado actual será el siguiente nodo de abiertos
			if (!SolutionFound and !abiertos.empty()) {

				current_node = abiertos.top();

				while (!abiertos.empty() and cerrados.find(current_node.st) != cerrados.end()) {

					abiertos.pop();

					if (current_node.st.jugador.f == final.f and current_node.st.jugador.c == final.c) {

						SolutionFound = true;
					}

					if (!abiertos.empty()) {

						current_node = abiertos.top();
					}
				}
			}
		}

		// Si ha encontrado solución guardar secuencia y pintarlo
		if (SolutionFound) {

			plan = current_node.secuencia;

			cout << "Encontrado un plan" << endl;
			PintaPlan(plan);
		}

	} else {

		cerr << "Casilla no transitable" << endl;
		exit(EXIT_FAILURE);
	}

	return plan;
}




//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FUNCIÓN PRINCIPAL THINK


// Devuelve la siguiente acción a realizar
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	// Niveles 0,1,2,3
	if (sensores.nivel != 4) {

		if (!hayPlan) {

			// Método de búsqueda
			cout << "Calculamos nuevo plan" << endl;
			
			// Inicialización variable de estado c_state (niveles 0 e 1)
			c_state.jugador.f = sensores.posF;
			c_state.jugador.c = sensores.posC;
			c_state.jugador.brujula = sensores.sentido;
			c_state.colaborador.f = sensores.CLBposF;
			c_state.colaborador.c = sensores.CLBposC;
			c_state.colaborador.brujula = sensores.CLBsentido;
			c_state.ultimaOrdenColaborador = act_CLB_STOP; 

			// Inicialización variable de estado c_state2 (niveles 2 e 3)
			c_state2.jugador.f = sensores.posF;
			c_state2.jugador.c = sensores.posC;
			c_state2.jugador.brujula = sensores.sentido;
			c_state2.colaborador.f = sensores.CLBposF;
			c_state2.colaborador.c = sensores.CLBposC;
			c_state2.colaborador.brujula = sensores.CLBsentido;
			c_state2.ultimaOrdenColaborador = act_CLB_STOP; 
			c_state2.tengo_bikini = false;
			c_state2.tengo_zapatillas = false;
			
			// Inicialización variable de estado goal
			goal.f = sensores.destinoF;
			goal.c = sensores.destinoC;

			
			switch (sensores.nivel)
			{
				case 0: plan = AnchuraSoloJugador_V3(c_state, goal, mapaResultado);
						break;

				case 1: plan = AnchuraSoloColaborador(c_state, goal, mapaResultado);
						break;

				case 2: plan = DijkstraSoloJugador(c_state2, goal, mapaResultado);
						break;

				case 3: cout << "Pendiente 3" << endl;
						break;

				case 4: cout << "Pendiente 4" << endl;
						break;
			}

			if (plan.size() > 0) {

				VisualizaPlan(c_state, plan);
				hayPlan = true;
			}
			
		}

		if (hayPlan and plan.size() > 0) {

			accion = plan.front(); // La accion corresponde al primer elemento de la lista
			plan.pop_front(); // Quitar el primer elemento de la lista de acciones plan
		}

		if (plan.size() == 0) {

			cout << "Se completo el plan" << endl;
			hayPlan = false;
		}

	// Nivel 4
	} else {

		cout << "Nivel 4" << endl;
	}

	return accion;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
// FUNCIÓN INTERACT


int ComportamientoJugador::interact(Action accion, int valor)
{
	return false;
}