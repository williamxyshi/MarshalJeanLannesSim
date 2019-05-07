#include <SDL.h>
#include <stdio.h>
#include <string>
#include "Hitbox.h"

//Screen dimension
const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

bool init();
bool loadMedia( std::string path );
void close();
SDL_Surface* loadSurface( std::string path );

SDL_Window* gWindow = NULL;
    
SDL_Surface* screenSurface = NULL;

SDL_Surface* backgroundSurface = NULL;
SDL_Surface* imageSurface = NULL;
SDL_Surface* ballSurface = NULL;

bool init()
{
    bool success = true;
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window surface
            screenSurface = SDL_GetWindowSurface( gWindow );
        }
    }

    return success;
}

bool loadMedia( std::string path, SDL_Surface* surface )
{
    bool success = true;

    surface = loadSurface( path );
    if( surface == NULL )
    {
        printf( "Failed to load stretching image!\n" );
        success = false;
    }

    return success;
}

void close()
{
    //Free loaded image
    SDL_FreeSurface( backgroundSurface );
    backgroundSurface = NULL;
    SDL_FreeSurface( imageSurface );
    imageSurface = NULL;
    SDL_FreeSurface( ballSurface );
    ballSurface = NULL;

    //Destroy window
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}

SDL_Surface* loadSurface( std::string path )
{
    //The final optimized image
    SDL_Surface* optimizedSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    else
    {
        //Convert surface to screen format
        optimizedSurface = SDL_ConvertSurface( loadedSurface, screenSurface->format, NULL );
        if( optimizedSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return optimizedSurface;
}

int main( int argc, char* args[] )
{
    if( !init() )
    {
        close();
        return 0;
    }
    //Load images
    backgroundSurface = loadSurface( "background.bmp" );

    Hitbox lannes{ 0, 836 };
    /*lannes.setXCoord( 0 );
    lannes.setYCoord( 836 );*/
    imageSurface = loadSurface( "lannes.bmp" );

    //load the player model
    SDL_SetColorKey(imageSurface, SDL_TRUE, SDL_MapRGB(imageSurface->format, 0, 0, 0));
    SDL_Rect offset;
    offset.x = lannes.getXCoord();
    offset.y = lannes.getYCoord();
    SDL_BlitSurface( imageSurface, NULL, screenSurface, &offset );

    //Apply the background stretched to the appropriate resolution
    SDL_Rect stretchRect;
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = SCREEN_WIDTH;
    stretchRect.h = SCREEN_HEIGHT;
    SDL_BlitScaled( backgroundSurface, NULL, screenSurface, &stretchRect );
    SDL_UpdateWindowSurface( gWindow );

    bool quit = false;
    SDL_Event e;

    while( !quit )
    {
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
        }
  
        SDL_BlitSurface( imageSurface, NULL, screenSurface, &offset );
        SDL_UpdateWindowSurface( gWindow );
    }

    close();
    return 0;
}