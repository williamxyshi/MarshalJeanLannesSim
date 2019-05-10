#include <stdio.h>
#include <iostream>
#include <string>
#include "Hitbox.h"
#include <map>

Hitbox::Hitbox( int xStartCoord, int yStartCoord, 
				int _horizSpeed, int _vertSpeed,
				int _hitboxXCoord, int _hitboxYCoord )
:
xCoord( xStartCoord ),
yCoord(yStartCoord),
horizSpeed(_horizSpeed),
vertSpeed(_vertSpeed),
hitboxXCoord(_hitboxXCoord),
hitboxYCoord(_hitboxYCoord)
{
}

int Hitbox::getXCoord()
{
	return xCoord;
}

int Hitbox::getYCoord()
{
	return yCoord;
}

void Hitbox::setXCoord( int _xCoord )
{
	xCoord = _xCoord;
}

void Hitbox::setYCoord( int _yCoord )
{
	yCoord = _yCoord;
}

void Hitbox::moveHitboxHoriz( bool right )
{
	//prevents lannes from walking off the screen
	if( (xCoord + horizSpeed >= 1360 && right) 
		 || (xCoord - horizSpeed <= -30 && !right) )
	{
		return;
	}
	else if( right )
	{
		xCoord += horizSpeed;
		return;
	}
	xCoord -= horizSpeed;
	return;
}

bool Hitbox::moveHitboxVert()
{
	if( jumpTick == 40 )
	{ 
		jumpTick = 0; 
		return false;
	}
	else if( jumpTick < 20 )
	{
		yCoord -= vertSpeed;
	}
	else
	{
		yCoord += vertSpeed;
	}
	jumpTick++;
	return true;
}

void Lannes::moveLannes( bool right )
{

}

void Lannes::setXCoord( int _xCoord )
{
	xCoord = _xCoord;
}

void Lannes::setYCoord( int _yCoord )
{
	yCoord = _yCoord;
}

int Lannes::getXCoord()
{
	return xCoord;
}

int Lannes::getYCoord()
{
	return yCoord;
}

void Lannes::jumpingLannes()
{

}

Cannonball::Cannonball( Hitbox& lannes, int _xDimension, int _yDimension )
:
hitboxXDimension(_xDimension),
hitboxYDimension(_yDimension)
{
	xCoord = 1408;
	yCoord = 0;

	int lannesX = lannes.xCoord + 100;
	int lannesY = lannes.yCoord;

	/*uses the vector form of the quadratic formula to model the cannonball's movement
	uses the top right corner as the vertex and the current location of lannes (+100
	in the x direction) for a point.*/
	bFactor = xCoord;
	cFactor = yCoord;

	aFactor = 1*( (double)lannesY - (double)cFactor )/( ((double)lannesX 
				   - (double)bFactor)*((double)lannesX - (double)bFactor) );

	minTick = 0;
	currentTick = xCoord;

	renderMe = true;
}

bool Cannonball::calculateTrajectory(  )
{
	if( !renderMe )
	{
		return false;
	}
	currentTick-=8;
	if( currentTick != minTick )
	{
		xCoord = currentTick;
		//quadratic formula here
		yCoord = (aFactor * ( (xCoord - bFactor )*(xCoord - bFactor ) ) + cFactor);

		if( yCoord > 825 || xCoord < 0 )
		{
			renderMe = false;
			xCoord = -32;
			yCoord = -32;
			return false;
		}
		return true;
	}
	//move the ball offscreen and turn of rendering if it is no longer needed
	xCoord = -32;
	yCoord = -32;
	renderMe = false;
	return false;

}

bool Cannonball::detectHit( Hitbox& lannes )
{
	//because lannes' playermodel and texture size are slightly different, I adjust
	//the hitbox so that ir more accurately reflects the player model rather then just
	//set the hitbox equal to the texture size ( 128x128 )
	const int blankTextureSizeX = (128-lannes.hitboxXCoord)/2;
	const int hitboxXCoordA = lannes.xCoord + blankTextureSizeX; //left bound of hitbox
	const int hitboxXCoordB = lannes.xCoord + blankTextureSizeX + lannes.hitboxXCoord; //right bound of hitbox 

	const int blankTextureSizeY = (128-lannes.hitboxYCoord)/2;
	const int hitboxYCoordA = lannes.yCoord + blankTextureSizeX; //top bound of hitbox
	const int hitboxYCoordB = lannes.yCoord + blankTextureSizeX + lannes.hitboxYCoord; //bottom bound of hitbox


	//establishing the bounds of the cannonball's hitbox
	//probably don't have to use a map, but i logged it with the names in case
	//i change the algorithm in some way
	const int leftBound = xCoord;
	const int rightBound = leftBound + hitboxXDimension;

	const int topBound = yCoord;
	const int bottomBound = yCoord + hitboxYDimension;

	bool horizontalCheck = false;
	bool verticalCheck = false;

	//this algorithm basically just checks if the edges of the cannonball are within the player model
	if( leftBound <= hitboxXCoordB && leftBound >= hitboxXCoordA )
	{
		horizontalCheck = true;
	}
	else if( rightBound <= hitboxXCoordB && rightBound >= hitboxXCoordA )
	{
		horizontalCheck = true; 
	}

	if( topBound >= hitboxYCoordA && topBound <= hitboxYCoordB )
	{
		verticalCheck = true;
	}
	else if( bottomBound >= hitboxYCoordA && bottomBound <= hitboxYCoordB )
	{
		verticalCheck = true;
	}

	if( horizontalCheck && verticalCheck )
	{
		//move it offscreen.
		xCoord = -32;
		yCoord = -32;
		renderMe = false;
		return true;
	}
	return false;
}

