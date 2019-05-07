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

