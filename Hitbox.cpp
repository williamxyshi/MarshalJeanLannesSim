#include <stdio.h>
#include <iostream>
#include <string>
#include "Hitbox.h"

Hitbox::Hitbox( int xStartCoord, int yStartCoord )
{
	xCoord = xStartCoord;
	yCoord = yStartCoord;
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
	if( right )
	{
		xCoord += 7;
		return;
	}
	xCoord -= 7;
}

bool Hitbox::moveHitboxVert()
{
	if( jumpTick == 200 )
	{ 
		jumpTick = 0; 
		return false;
	}
	else if( jumpTick < 100 )
	{
		yCoord -= 2;
	}
	else
	{
		yCoord += 2;
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

Cannonball::Cannonball( Hitbox& lannes )
{
	xCoord = 1300;
	yCoord = 0;

	int lannesX = lannes.xCoord;
	int lannesY = lannes.yCoord;

	bFactor = xCoord;
	cFactor = yCoord;

	aFactor = 1*( (double)lannesY - (double)cFactor )/( ((double)lannesX - (double)bFactor)*((double)lannesX - (double)bFactor) );

	minTick = 0;
	currentTick = xCoord;
}

bool Cannonball::calculateTrajectory(  )
{
	currentTick--;
	if( currentTick != minTick )
	{
		xCoord = currentTick;
		yCoord = (aFactor * ( (xCoord - bFactor )*(xCoord - bFactor ) ) + cFactor);
		return true;
	}
	return false;

}

bool Cannonball::detectHit()
{
	return true;
}

