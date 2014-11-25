#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Config.h"
#include <SDL.h>
#include <SDL_image.h>

CGame::CGame(){
	tiempoFrameInicial = 0;
	estado = ESTADO_INICIANDO;
}

// Con esta función eliminaremos todos los elementos en pantalla
void CGame::Finalize(){
	delete nave;
	//	delete keys;
	delete *enemigoArreglo;
	SDL_FreeSurface(screen);
	SDL_Quit();
}

// Con esta función se iniciara, lo necesario para jugar (Audio, Video, Etc.)
void CGame::Iniciando(){
	if (SDL_Init( SDL_INIT_VIDEO )){
		printf("Error %s ", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	screen = SDL_SetVideoMode( WIDTH_SCREEN, HEIGHT_SCREEN, 24, SDL_HWSURFACE);
	if (screen == NULL){
		printf("Error %s ", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	SDL_WM_SetCaption( "Mi primer Juego", NULL );

	atexit(SDL_Quit);

	nave = new Nave(screen,"../Data/MiNave.bmp",(WIDTH_SCREEN/2),(HEIGHT_SCREEN-80),MODULO_MINAVE_NAVE);
	menu = new Objeto(screen,"../Data/menu.bmp",0,0,MODULO_MENU_FONDO);
	textos = new Objeto(screen,"../Data/Texto.bmp",0,0,-1);
	fondo = new Objeto(screen,"../Data/fondo.bmp",0,0,MODULO_MENU_FONDO);

	for(int i=0;i<10;i++){
		enemigoArreglo[i] = new Objeto(screen,"../Data/enemigo.bmp",i*60,0,MODULO_ENEMIGO_NAVE);
		enemigoArreglo[i]->SetAutoMovimiento(false);
		enemigoArreglo[i]->SetPasoLimite(4);
	}
	tick=0;
	opcionSeleccionada=MODULO_TEXTO_MENU_OPCION1;
}

bool CGame::Start()
{	
	// Esta variable nos ayudara a controlar la salida del juego...
	int salirJuego = false;
    while (salirJuego == false){
		//Maquina de estados
		switch(estado){
		case Estado::ESTADO_INICIANDO:
			Iniciando();
			estado = ESTADO_JUGANDO;
			break;
		case Estado::ESTADO_MENU:
			menu->Pintar();
			textos->Pintar(MODULO_TEXTO_TITULO,100,50);
			textos->Pintar(MODULO_TEXTO_NOMBRE,300,390);
			Menu();
			//estado=ESTADO_JUGANDO;
			break;
		case Estado::ESTADO_JUGANDO:
			for(int i=0;i<10;i++){
				enemigoArreglo[i]->Actualizar();
			}
			MoverEnemigo();
			fondo->Pintar();
			//SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format,0,0,0));
			keys = SDL_GetKeyState(NULL);
			if(keys[SDLK_RIGHT] ) {
				if(!EsLimitePantalla(nave->GetNaveObjeto(), BORDE_DERECHO))
					nave->MoverDerecha();
			}
			CodigoOculto(0);
			nave->Pintar();
			for(int i=0; i<10;i++)
				enemigoArreglo[i]->Pintar();
			break;
		case Estado::ESTADO_TERMINANDO:
			break;
		case Estado::ESTADO_FINALIZANDO:
			salirJuego = true;
			Finalize();
			break;
		};

		while(SDL_PollEvent(&event)) // Aquí SDL creará una lista de eventos ocurridos
        {
            if(event.type == SDL_QUIT) {salirJuego = true;} // Si se detecta una salida de SDL o...
			if(event.type == SDL_KEYDOWN) {  }
        }

		
		//Este codigo estara provicionalmente aqui.
		SDL_Flip(screen);

		//Calculando FPS
		tiempoFrameFinal=SDL_GetTicks();
		while(tiempoFrameFinal < (tiempoFrameInicial + FPS_DELAY)){
			tiempoFrameFinal=SDL_GetTicks();
			SDL_Delay(2);
		}

		printf("Frames:%d Tiempo:%d Tiempo Promedio:%f Tiempo por Frame:%d FPS:%f\n",tick,SDL_GetTicks(), (float)SDL_GetTicks()/(float)tick,tiempoFrameFinal-tiempoFrameInicial,1000.0f / (float)(tiempoFrameFinal-tiempoFrameInicial) );
		tiempoFrameInicial=tiempoFrameFinal;//Marcamos el inicio nuevamente
		tick++;

    }
	return true;
}

bool CGame::EsLimitePantalla(Objeto * objeto, int bandera){
	if(bandera & BORDE_IZQUIERDO)
		if(objeto->ObtenerX() <= 0)
			return true;
	if(bandera & BORDE_SUPERIOR)
		if(objeto->ObtenerY() <= 0)
			return true;
	if(bandera & BORDE_DERECHO)
		if(objeto->ObtenerX() >= WIDTH_SCREEN-objeto->ObtenerW())
			return true;
	if(bandera & BORDE_INFERIOR)
		if(objeto->ObtenerY() >= HEIGHT_SCREEN-objeto->ObtenerH())
			return true;
	return false;
}

void CGame::MoverEnemigo(){
	for(int i=0;i<10;i++){

	//************************//
	///////////PASO 0///////////
	if(enemigoArreglo[i]->ObtenerPasoActual()==0)
		if(!EsLimitePantalla(enemigoArreglo[i],BORDE_DERECHO))
			enemigoArreglo[i]->Mover(10);//DERECHA
		else{
			enemigoArreglo[i]->IncrementarPasoActual();
			enemigoArreglo[i]->IncrementarPasoActual();
		}
	//************************//
	///////////PASO 1///////////
	//if(enemigoArreglo[i]->ObtenerPasoActual()==1)
		//if(!EsLimitePantalla(enemigoArreglo[i],BORDE_INFERIOR))
			//enemigoArreglo[i]->Mover(1);//ABAJO
	//************************//
	///////////PASO 2///////////
	if(enemigoArreglo[i]->ObtenerPasoActual()==2)
		if(!EsLimitePantalla(enemigoArreglo[i],BORDE_IZQUIERDO))
			enemigoArreglo[i]->Mover(-10);//IZQUIRDA
		else{
			enemigoArreglo[i]->IncrementarPasoActual();
			enemigoArreglo[i]->IncrementarPasoActual();
		}
	//************************//
	///////////PASO 3///////////
	//if(enemigoArreglo[i]->ObtenerPasoActual()==3)
		//if(!EsLimitePantalla(enemigoArreglo[i],BORDE_INFERIOR))
			//enemigoArreglo[i]->Mover(1);//ABAJO
	}
}

void CGame::Menu(){
	for(int i = MODULO_TEXTO_MENU_OPCION1, j = 0; i <= MODULO_TEXTO_MENU_OPCION2; i++, j++ ){
		if(i == opcionSeleccionada)
			textos->Pintar(i+2, 200, 180+(j*80));
		else
			textos->Pintar(i, 200, 180+(j*80)); 
	}
}

