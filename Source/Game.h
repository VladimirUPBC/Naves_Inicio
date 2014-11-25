#pragma once
#include <SDL.h>
#include "Objeto.h"
#include "Nave.h"

class CGame
{
public:

	enum Estado{
		ESTADO_INICIANDO,
		ESTADO_MENU,
		ESTADO_JUGANDO,
		ESTADO_TERMINANDO,
		ESTADO_FINALIZANDO
	};

	bool Start();
	static CGame instanceGame;

	CGame();
	void Finalize();

private:
	void Iniciando();
	void Menu();
	void MoverEnemigo();
	bool EsLimitePantalla(Objeto * objeto, int bandera);
	void CodigoOculto(int codigo);
	
	int opcionSeleccionada;
	Uint8 *keys; // Esta variable nos servirá para ver si determinadas teclas están o no pulsadas
	SDL_Event event; // La variable event de tipo evento de SDL nos servirá para monitorizar el teclado

	SDL_Surface * screen;

	Nave *nave;
	Objeto *enemigoArreglo [10];

	Objeto *menu;//Fondo del Menu
	Objeto *textos;//Textos del juego
	Objeto *fondo;//Fondo del juego

	int tick;
	int tiempoFrameInicial;
	int tiempoFrameFinal;

	Estado estado;
};

