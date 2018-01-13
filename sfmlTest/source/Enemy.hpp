#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.hpp"


class ENEMY : public Entity
{
public:
	enum { stay, walk, duck, jump, climb, sit } STATE;
	bool onLadder, shoot, hit,raz=true;
	
	ENEMY(AnimationManager &a, Level &lev, int x, int y) :Entity(a, x, y)
	{
		option("EnemyDuck1", -0.05, 2, "Duck1");
		obj = lev.GetAllObjects();
		anim.flip();
	}

	void update(float time)
	{
		/*if (raz)
		{
			anim.flip();
			dx *= -1;
			raz = false;
		}*/
		x += dx*time;
		y += dy*time;
		
		timer += time;
		/*if (timer>10000) { dx *= -1; timer = 0; }*/


		if (Health <= 0) {
		/*	anim.set("dead"); dx = 0;*/
			timer_end += time;
			life = false;
		}
		dy += 0.0005*time;
		
		Collision(1);
		
		
		anim.tick(time);
	}
	
	void Collision(int num)
	{
		for (int i = 0; i<obj.size(); i++)
			if (getRect().intersects(obj[i].rect))
			{
				if (obj[i].name == "Solid")
				{
					if (dy>0 && num == 1) { y = obj[i].rect.top - h;  dy = 0;   STATE = stay; }
					if (dy<0 && num == 1) { y = obj[i].rect.top + obj[i].rect.height;   dy = 0; }
					if (dx>0 && num == 0) { x = obj[i].rect.left - w; }
					if (dx<0 && num == 0) { x = obj[i].rect.left + obj[i].rect.width; }
				}
				
				else if (obj[i].name == "Graund")
				{

					dx *= -1;
					if (dir == 1)
					{
						
						anim.flip(dir);
						dir = 0;
						
					}
					else if (dir == 0)
					{
						
						anim.flip(dir);
						dir = 1;
					}

					
					
					




				}
			

			}
	}
};

#endif ENEMY_H
