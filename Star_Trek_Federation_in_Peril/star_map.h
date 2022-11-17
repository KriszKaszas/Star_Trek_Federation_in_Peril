#ifndef STAR_MAP_H_INCLUDED
#define STAR_MAP_H_INCLUDED

#include "random_number_in_interval.h"

#include "debugmalloc.h"

/**
*@brief StarColor
*@details  Ez az adatstruktura tarolja a hatter csillaganak szinet. Ertekei a csillag RGBA-ban
meghatarozott szinertekei. Ez az adatterolo a fuggvenyhivaskor megadando parameterlistak leroviditeset
szolgalja.
*/
typedef struct starcolor{
    int r;  ///< A csillag RGBA piros erteke.
    int g;  ///< A csillag RGBA zold erteke.
    int b;  ///< A csillag RGBA kek erteke.
    int a;  ///< A csillag RGBA alfa erteke (ez hatarozza meg a csillag attetszoseget.
}StarColor;

/**
*@brief Star
*@details Ez az adatstruktura tarolja a hatter egy csillagat. Ertekei a csillag kirajzolasahoz szukseges koordinatak
es sugar. Ez az adattarolo a fuggvenyhivaskor megadando parameterlistak leroviditeset, illetve az osszetartozo adatok egy
helyen tartasat szolgalja.
*/
typedef struct star{
    int y_coor; ///< A csillagot jelkepzo kor y koordinataja.
    int x_coor; ///< A csillagot jelkepzo kor x koordinataja.
    int radius; ///< A csillagot jelkepzo kor sugara.
}Star;

/**
*@brief StarMap
*@details Ez az adatstruktura tarolja a hatter osszes csillagat. Ertekei a lista hossza, a csillagokat tartalmazo lista,
illetve azok szine. Ez az adattarolo a hatter csillagainak konnyu letrehozasat, tarolasat es felszabaditasat szolgalja.
*/
typedef struct starmap{
    int length;         ///< A lista hossza.
    Star *stars;        ///< A csillagokat tarolo lista.
    StarColor color;    ///< A csillagok szinet tarolo struktura.
}StarMap;


StarMap *starmap_init(int width, int height);


void advance_starmap_frame(StarMap *sm, int width, int height);


void free_starmap(StarMap *sm);

#endif // STAR_MAP_H_INCLUDED
