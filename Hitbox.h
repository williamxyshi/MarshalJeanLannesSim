#include <stdio.h>
#include <iostream>
#include <string>

class Hitbox
{
	public:
		Hitbox( int xStartCoord, int yStartCoord );

		int getXCoord();
		int getYCoord();

		void setXCoord( int _xCoord );
		void setYCoord( int _yCoord );

		int xCoord;
		int yCoord;

};

class Lannes: public Hitbox
{
	public:
		Lannes( int xStartCoord, int yStartCoord );
		Lannes();

		void moveLannes( bool right );
		void jumpingLannes();
		void setXCoord( int _xCoord );
		void setYCoord( int _yCoord );
		int getXCoord();
		int getYCoord();

	private:
		const int lannesSpeed {80};
};

class Cannonball: public Hitbox
{
	public:
		Cannonball( int xStartCoord, int yStartCoord );
		void moveCannonball( );

		bool detectHit();

	private:
		const int currentTick = 0;
};