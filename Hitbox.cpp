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
		xCoord += 5;
		return;
	}
	xCoord -= 5;
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
		yCoord -= 1;
	}
	else
	{
		yCoord += 1;
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

void Cannonball::moveCannonball(  )
{

}

bool Cannonball::detectHit()
{
	return true;
}

