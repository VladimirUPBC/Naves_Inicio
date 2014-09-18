#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include <SDL.h>

CGame::CGame(){
	SDL_Surface *screen;
	
	if (SDL_Init( SDL_INIT_VIDEO )){
		printf("Error %s ", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	screen = SDL_SetVideoMode( 640, 480, 24, SDL_SWSURFACE );
	if (screen == NULL){
		printf("Error %s ", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_WM_SetCaption( "Mi primer Juego", NULL );
	
	SDL_Flip(screen);

	estado = Estado::ESTADO_INICIANDO;
}

// Con esta función eliminaremos todos los elementos en pantalla
void CGame::Finalize(){
}

bool CGame::Start()
{
	// Esta variable nos ayudara a controlar la salida del juego...
	int salirJuego = false;
          
	while (salirJuego == false){
		//Maquina de estados
		switch(estado){
		case Estado::ESTADO_INICIANDO:
			break;
		case Estado::ESTADO_MENU:
			break;
		case Estado::ESTADO_JUGANDO:
			break;
		case Estado::ESTADO_TERMINANDO:
			break;
		case Estado::ESTADO_FINALIZANDO:
				salirJuego = true;
			break;
		};
    }
	return true;
}
