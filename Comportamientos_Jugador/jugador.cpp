#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>


// Este es el método principal que se piden en la practica. Tiene como entrada la información de los sensores y devuelve la acción a realizar.
Action ComportamientoJugador::think(Sensores sensores)
{
	Action accion = actIDLE;

	if (plan.size() == 0) {

		hayPlan = false;
	}

	if (!hayPlan) {

		// Método de búsqueda
		hayPlan = true;
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
