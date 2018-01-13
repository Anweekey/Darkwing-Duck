#ifndef BULLET_H
#define BULLET_H

#include "Entity.hpp"


class Bullet:public Entity
{
public:

	Bullet(AnimationManager &a, Level &lev,int x,int y,bool dir):Entity(a,x,y)
	{
		option("Bullet", 0.1, 10, "move");
	anim.set("Bullet");
	if (dir)
		dx = -0.1;
		
	
   		obj = lev.GetObjects("Solid");
	}

	void update(float time)
	{
		x+=dx*time;

		for (int i=0;i<obj.size();i++)
			if (getRect().intersects(obj[i].rect))
		        {Health=0;}

		if (Health <= 0) {
			anim.set("Bang"); 
			anim.animList["Bang"].loop = 0; dx = 0;
		                if (anim.isPlaying()==false) life=false;
		                }

		anim.tick(time);
	}

};

#endif BULLET_H
