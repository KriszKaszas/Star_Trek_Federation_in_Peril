/**
*@file ui_input.c
*/

#include "ui_input.h"



/**
*@brief user_input
*@details A felhasznalotol erkezo billentyuparancsokat ertelmezi, es egy interface-n keresztel adja
at a program tobbi reszenek
*@param [in out] *isi a jatek InputStateInterface-re mutato pointer. Ezen keresztul kommunikalnak egymassal a vezerlomodulok.
*@param [in] *key_map ez a vezerlo KeyMap interfacen keresztul hasonlitja ossze a bejovo billentyuparancsokat a valid vezerlo gombokkal.
*@param [in] id egy SDL_TimerID tipusu idozito. Feladata, hogy general egy SDL_USEREVENTet, amennyiben az idozito lejartaval nincs beerkezo esemeny/parancs
(enelkul a vezerlo blokkolna a program futasat, nem mukodne a hatter animacio, es semmi nem tortenne, amig nincs felhasznaloi interakcio).
*@return void
*/
void user_input(InputStateInterface *isi, KeyMap *key_map, SDL_TimerID id){
    SDL_Event event;
    SDL_WaitEvent(&event);
    char *key = SDL_GetKeyName(event.key.keysym.sym);
    switch (event.type)
    {
        /* felhasznaloi esemeny: ilyeneket general az idozito fuggveny */
        case SDL_USEREVENT:
            break;
        case SDL_KEYUP:
            key = SDL_GetKeyName(event.key.keysym.sym);
            if(event.key.keysym.sym == SDL_GetKeyFromName(key_map->upkey))
                {
                isi->up = false;
            }
            if(event.key.keysym.sym == SDL_GetKeyFromName(key_map->downkey))
            {
                isi->down = false;
            }
            if(event.key.keysym.sym == SDL_GetKeyFromName(key_map->leftkey))
            {
                isi->left = false;
            }
            if(event.key.keysym.sym == SDL_GetKeyFromName(key_map->rightkey))
            {
                isi->right = false;
            }
            if(event.key.keysym.sym == SDL_GetKeyFromName(key_map->torpedokey))
            {
                isi->torpedo_ready = true;
            }
            if(event.key.keysym.sym == SDLK_y)
            {
                isi->y = false;
            }
            if(event.key.keysym.sym == SDLK_n)
            {
                isi->n = false;
            }
            break;

        case SDL_KEYDOWN:
            key = SDL_GetKeyName(event.key.keysym.sym);
             if(event.key.keysym.sym == SDL_GetKeyFromName(key_map->upkey))
                {
                isi->up = true;
            }
            if(event.key.keysym.sym == SDL_GetKeyFromName(key_map->downkey))
                {
                isi->down = true;
            }
            if(event.key.keysym.sym == SDL_GetKeyFromName(key_map->leftkey))
            {
                isi->left = true;
            }
            if(event.key.keysym.sym == SDL_GetKeyFromName(key_map->rightkey))
            {
                isi->right = true;
            }
            if(event.key.keysym.sym == SDL_GetKeyFromName(key_map->torpedokey) && isi->torpedo_ready)
            {
                isi->torpedo = true;
                isi->torpedo_ready = false;
            }
            if(event.key.keysym.sym == SDLK_y)
            {
                isi->y = true;
            }
            if(event.key.keysym.sym == SDLK_n)
            {
                isi->n = true;
            }
            break;

        case SDL_QUIT:
            isi->quit = true;
            break;
    }
}
