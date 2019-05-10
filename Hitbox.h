#include <stdio.h>
#include <iostream>
#include <string>

class Hitbox
{
	public:
		Hitbox( int xStartCoord, int yStartCoord,
				int _horizSpeed, int _vertSpeed,
				const int _hitboxXCoord, 
				const int _hitboxYCoord );

		int getXCoord();
		int getYCoord();

		void moveHitboxHoriz( bool right );
		bool moveHitboxVert();

		void setXCoord( int _xCoord );
		void setYCoord( int _yCoord );

		int xCoord;
		int yCoord;

		const int hitboxXCoord, hitboxYCoord;
		
		int jumpTick = 0;
		const int horizSpeed, vertSpeed;

};

//currently unused. FIX_ME: make lannes model use this class instead
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
//FIX_ME inherit from hitbox
class Cannonball
{
	public:
		Cannonball( Hitbox& lannes, 
					const int _xDimension,
					const int _yDimension );
		bool calculateTrajectory();

		bool detectHit( Hitbox& hitbox);

		int xCoord;
		int yCoord;

		bool renderMe; //if it is offscreen, do not render/blit

	private:
		double aFactor;
		int bFactor, cFactor;
		int currentTick, minTick;
		const int hitboxXDimension, hitboxYDimension;
};