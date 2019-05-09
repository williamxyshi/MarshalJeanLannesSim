#include <SDL.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <vector>
#include <utility>
#include <iostream>
#include "Hitbox.h"

//Screen dimension
const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

//global declarations (yikes)
bool init();
bool loadMedia( std::string path );
void close();
SDL_Surface* loadSurface( std::string path );

static SDL_Window* window = NULL;
static SDL_Surface* screenSurface = NULL;
static SDL_Surface* backgroundSurface = NULL;
static SDL_Surface* imageSurface = NULL;
static SDL_Surface* ballSurface = NULL;

class Textures
{
    public:
        typedef std::vector<SDL_Surface*> textureVect;

        virtual initializeTexture( std::string path )
        {
            SDL_Surface* surface = loadSurface( path );
            if( !surface )
            {
                printf( "could not load texture! error: %s\n", SDL_GetError() );
            }
            textures.push_back( surface );
        }

        virtual textureVect getTextures()
        {
            return textures;
        }

        virtual SDL_Surface* getTexture( int index )
        {
            SDL_Surface* surface = textures.at(index);
            if( !surface ){ printf("no texture!"); return nullptr; }
            return surface;
        }

    protected:
        textureVect textures;
};

class ObjectTextures : public Textures
{
    public:
        typedef std::pair<int, int> textureLoc;
        typedef std::vector<textureLoc> textureLocVect;

        void initializeTexture( std::string path, int x, int y )
        {
            SDL_Surface* surface = loadSurface( path );
            if( !surface )
            {
                printf( "could not load texture! error: %s\n", SDL_GetError() );
            }
            textures.push_back( surface );
            textureLoc locationPair { x, y };
            locationVector.push_back( locationPair );
        }

        void handleObjectMotion( bool lannesMoved )
        {
            SDL_Rect objectPos;
            SDL_Surface* surface;
            if(lannesMoved)
            {
                for( int i = 0; i < textures.size(); ++i )
                {
                    surface = textures.at(i);
                    textureLoc locationPair = locationVector.at(i);

                    objectPos.x = locationPair.first - 200;
                    objectPos.y = locationPair.second;
                }
            }
            else
            {
                for( int i = 0; i < textures.size(); ++i )
                {
                    surface = textures.at(i);
                    textureLoc locationPair = locationVector.at(i);

                    objectPos.x = locationPair.first;
                    objectPos.y = locationPair.second;
                }
            }

            SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 0));
            SDL_BlitSurface( surface, NULL, screenSurface, &objectPos );
        }

    private:
        textureLocVect locationVector;
};

class LannesTextures : public Textures
{
    public:
        void handleLannesMotion( bool lannesJumping, Hitbox lannes, bool lannesMoved )
        {
            SDL_Surface* surface;
            //walking animation
            if(!lannesJumping)
            {
                if( lannesWalkTick < 3 )
                {
                    surface = textures.at(0);
                }
                else if( lannesWalkTick <= 6 )
                {
                    if( lannesWalkTick == 6 ){ lannesWalkTick = 0; }
                    surface = textures.at(1);
                }

                if( lannesMoved ){ lannesWalkTick++; }
            }
            //lannes jumping loop
            else if( lannesJumping )
            {
                surface = textures.at(2);
                lannesWalkTick = 0;
            }

            SDL_Rect lannesPos;
            lannesPos.x = lannes.getXCoord();
            lannesPos.y = lannes.getYCoord();

            SDL_SetColorKey( surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 0));
            SDL_BlitSurface( surface, NULL, screenSurface, &lannesPos );

            return;
        }
    private:
        int lannesWalkTick = 0;
};

bool init()
{
    bool success = true;
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL machine broken.. SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Create window
        window = SDL_CreateWindow( "LannesSimulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "The window could not be drawn SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Get window's surface
            screenSurface = SDL_GetWindowSurface( window );
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
    SDL_DestroyWindow( window );
    window = NULL;

    //Quits SDL
    SDL_Quit();
}

SDL_Surface* loadSurface( std::string path )
{
    SDL_Surface* returnSurface = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = SDL_LoadBMP( path.c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
    }
    else
    {
        returnSurface = SDL_ConvertSurface( loadedSurface, screenSurface->format, NULL );
        if( returnSurface == NULL )
        {
            printf( "Unable to optimize image %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
        }
        SDL_FreeSurface( loadedSurface );
    }
    return returnSurface;
}

LannesTextures loadLannesTextures()
{
    LannesTextures lannesTextures;
    lannesTextures.initializeTexture( "textures/lannesstep1.bmp" );
    lannesTextures.initializeTexture( "textures/lannesstep2.bmp" );
    lannesTextures.initializeTexture( "textures/lannesjump.bmp" );
    return lannesTextures;
}

ObjectTextures loadObjectTextures()
{
    ObjectTextures objectTextures;
    objectTextures.initializeTexture( "textures/tree1.bmp", 1440, 569 );
    //objectTextures.initializeTexture( "textures/tree2.bmp", 1440, 569 );
    //objectTextures.initializeTexture( "textures/tree3.bmp", 1440, 569 );
   // objectTextures.initializeTexture( "textures/tree4.bmp", 1440, 569 );
    //objectTextures.initializeTexture( "textures/tree5.bmp", 1440, 569 );
    return objectTextures;
}

int main( int argc, char* args[] )
{
    if( !init() )
    {
        close();
        return 0;
    }
    //Load background textures
    backgroundSurface = loadSurface( "textures/background.bmp" );
    ObjectTextures objectTextures = loadObjectTextures();

    objectTextures.handleObjectMotion( false );

    //player model
    Hitbox lannes{ 0, 700 };
    LannesTextures lannesTextures = loadLannesTextures();

    //cannonball texture
    ballSurface = loadSurface("textures/cannonball.bmp");
    SDL_SetColorKey(ballSurface, SDL_TRUE, SDL_MapRGB(ballSurface->format, 255, 0, 0));

    //load the player model for launch
    SDL_Rect lannesPos;
    lannesPos.x = lannes.getXCoord();
    lannesPos.y = lannes.getYCoord();
    lannesTextures.handleLannesMotion( false, lannes, false );

    //Apply the background stretched to the appropriate resolution
    SDL_Rect stretchRect;
    stretchRect.x = 0;
    stretchRect.y = 0;
    stretchRect.w = SCREEN_WIDTH;
    stretchRect.h = SCREEN_HEIGHT;
    SDL_BlitScaled( backgroundSurface, NULL, screenSurface, &stretchRect );
    SDL_UpdateWindowSurface( window );

    bool quit = false;
    bool lannesJumping = false;
    std::vector<Cannonball> cannonballList; 

    Cannonball cannonball{ lannes };

    SDL_Event e;

    while( !quit )
    {
        bool lannesMoved = false;
        //resets the window
        SDL_BlitScaled( backgroundSurface, NULL, screenSurface, &stretchRect );
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
            else if( e.type == SDL_KEYDOWN )
            {
                lannesMoved = true;
                //Select surfaces based on key press
                switch( e.key.keysym.sym )
                {
                    case SDLK_UP:
                    lannesJumping = true;
                    break;

                    case SDLK_DOWN:
                    break;

                    case SDLK_LEFT:
                    lannes.moveHitboxHoriz( false );
                    break;

                    case SDLK_RIGHT:
                    lannes.moveHitboxHoriz( true );
                    break;

                    default:
                    break;
                }
            }
        }

        lannesTextures.handleLannesMotion( lannesJumping, lannes, lannesMoved );
        objectTextures.handleObjectMotion( lannesMoved );

        //moves hitbox if lannes is jumping 
        if( lannesJumping )
        {
            lannesJumping = lannes.moveHitboxVert();
        }
  
        cannonball.calculateTrajectory();
        SDL_Rect ballPos;
        ballPos.x = cannonball.xCoord;
        ballPos.y = cannonball.yCoord;
        SDL_BlitSurface( ballSurface, NULL, screenSurface, &ballPos );








        //cannonball spawns
        //int random = rand() % 200 + 1;     // v2 in the range 1 to 200

        /*if( random = 100 )
        {
            Cannonball cannonball{ lannes };
            cannonballList.push_back( cannonball );
        }

        if( !cannonballList.empty() )
        {
            std::vector<Cannonball>::iterator ballIt = cannonballList.begin();
            std::vector<Cannonball>::iterator ballItEnd = cannonballList.end();
            for( ; ballIt != ballItEnd; ++ballIt )
            {
                Cannonball ball = *ballIt;
                ball.calculateTrajectory();

                ball.detectHit();

                SDL_Rect ballPos;
                ballPos.x = ball.xCoord;
                ballPos.y = ball.yCoord;
                SDL_BlitSurface( ballSurface, NULL, screenSurface, &ballPos );

                std::cout<<ball.xCoord<<" "<< ball.yCoord << " ";


            }
        }*/

        SDL_UpdateWindowSurface( window );
    }

    close();
    return 0;
}