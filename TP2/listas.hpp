#ifndef LISTAS_HPP_INCLUDED
#define LISTAS_HPP_INCLUDED

#include <cassert>

template <typename T> struct Nodo {
	T dato; // valor que contiene el nodo
	Nodo<T>* sig; // puntero al siguiente nodo
};

template <typename T> void mostrar(Nodo<T>* lista)
{ //Presupone que de ser neceario se hizo la sobrecarga de << para dato
	while (lista != nullptr) {
		std::cout << lista->dato << std::endl;
		lista = lista->sig;
	}
}

template <typename T> void push(Nodo<T>* &pila, T valor)
{
	Nodo<T>* nuevo = new Nodo<T>;
	nuevo->dato = valor;
	nuevo->sig = pila;
	pila = nuevo;
}

template <typename T> T pop(Nodo<T>* &pila)
{
	assert(pila != nullptr); //precondición de pop
	T valor = pila->dato;
	Nodo<T>* aux_elim = pila;
	pila = pila->sig;
	delete aux_elim;
	return valor;
}

template <typename T> void agregar(Nodo<T>*& cola, T valor)
{
	Nodo<T>* nuevo = new Nodo<T>;
	nuevo->dato = valor;
	nuevo->sig = nullptr;
	Nodo<T>** pp = &cola;
	while (*pp != nullptr) //avanzo hasta el que no tiene siguiente
		pp = &(*pp)->sig;
	*pp = nuevo; // atrás del último, agrego el nuevo
}

//Lista no ordenada
template <typename T> T* buscar_lno(T clave, Nodo<T>* lista, int (*criterio)(T, T))
{
	while (lista != nullptr) {
		if (criterio(lista->dato, clave) == 0)
			return &lista->dato;
		lista = lista->sig;
	}
	return nullptr;
}

//Lista ordenada
template <typename T> Nodo<T>* buscar(T clave, Nodo<T>* lista, int criterio(T, T))
{
	while (lista != nullptr && criterio(clave, lista->dato) > 0)
		lista = lista->sig;

	return lista != nullptr && criterio(clave, lista->dato) == 0 ? lista : nullptr;
}

template <typename T> void insertar(T valor, Nodo<T>*& lista, int criterio(T, T))
{
	Nodo<T>* nuevo = new Nodo<T>;
	nuevo->dato = valor;

	Nodo<T>** pp = &lista;
	while (*pp != nullptr && criterio(nuevo->dato, (*pp)->dato) >= 0)
		pp = &(*pp)->sig;
	nuevo->sig = *pp;
	*pp = nuevo;
}

template <typename T> Nodo<T>* extraer(T clave, Nodo<T>*& lista, int criterio(T, T))
{
	Nodo<T> *aux;
	Nodo<T> **pp = &lista;

	while (*pp != nullptr && criterio(clave, (*pp)->dato) > 0)
		pp = &(*pp)->sig;

	if (*pp != nullptr && criterio(clave, (*pp)->dato) == 0) {
		aux = *pp;
		*pp = (*pp)->sig;
		return aux;
	} else {
		return nullptr;
	}
}

template <typename T> bool borrar(T clave, Nodo<T>*& lista, int criterio(T, T))
{
	Nodo<T>* extraido = extraer(clave, lista, criterio);
	if (extraido) {
		delete extraido;
		return true;
	} else {
		return false;
	}
}

template <typename T> Nodo<T>* insertar_unico(T valor, Nodo<T>*& lista, int criterio(T, T))
{
	Nodo<T>** pp = &lista;
	while (*pp != nullptr && criterio(valor, (*pp)->dato) > 0)
		pp = &(*pp)->sig;
	if (*pp != nullptr && criterio(valor, (*pp)->dato) == 0) {
		return *pp; //devuelvo el que ya estaba y acabo de encontrar
	} else {
		Nodo<T>* nuevo = new Nodo<T>; //Genero nodo
		nuevo->dato = valor; // y guardo el valor
		//Ahora lo inserto en su lugar
		nuevo->sig = *pp;
		*pp = nuevo;
		return nuevo;
	}
}

#endif // LISTAS_HPP_INCLUDED
