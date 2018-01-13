#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <list>
#include "level.hpp"
#include "Animation.hpp"
#include "Entity.hpp"
#include "Bullet.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "MovingPlatform.hpp"
#include "HealthBar.hpp"
using namespace sf;


void RunGame()
{
	///////////// инициализация ///////////////////////////
	RenderWindow window(VideoMode(800, 600), "The Game!");

	View view( FloatRect(0, 0, 400, 300) );

	Level lvl;
	lvl.LoadFromFile("files/Level1.tmx");

	Texture enemy_t, moveplatform_t, megaman_t, bullet_t, bg;
	bg.loadFromFile("files/images/bg.png");
	enemy_t.loadFromFile("files/images/67163.png");
	moveplatform_t.loadFromFile("files/images/movingPlatform.png");
	megaman_t.loadFromFile("files/images/Duckii.png");
	bullet_t.loadFromFile("files/images/bullet.png");

	

	AnimationManager anim;
	anim.loadFromXML("files/Hodit.xml",megaman_t);
	anim.animList["Jump"].loop = 0;
	anim.animList["Sit"].loop = 0;
	anim.animList["Damage"].loop = 0;

	AnimationManager anim2;
	anim2.create("move",bullet_t,7,10,8,8,50,12,0,true);
	anim2.create("explode",bullet_t,27,7,18,18,4,0.01,29,false);
	
	anim2.loadFromXML("files/Bullet.xml", bullet_t);
	
	





	AnimationManager anim3;
	


	anim3.loadFromXML("files/Enemy.xml", enemy_t);


	AnimationManager anim4;
	anim4.create("move",moveplatform_t,0,0,95,22,1,0);

    Sprite background(bg);
    background.setOrigin(bg.getSize().x/2,bg.getSize().y/2);

	std::list<Entity*>  entities;
	std::list<Entity*>::iterator it;

	std::vector<Object> e = lvl.GetObjects("EnemyDuck1");
	for (int i=0;i < e.size();i++)
		entities.push_back(new ENEMY(anim3, lvl, e[i].rect.left, e[i].rect.top) );

	e = lvl.GetObjects("MovingPlatform");
	for (int i=0;i < e.size();i++)
		entities.push_back(new MovingPlatform(anim4, lvl, e[i].rect.left, e[i].rect.top) );

	Object pl = lvl.GetObject("Player");
	PLAYER Mario(anim, lvl, pl.rect.left, pl.rect.top);

	HealthBar healthBar;

	Clock clock;

	/////////////////// основной цикл  /////////////////////
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();

		time = time/500;  // здесь регулируем скорость игры

		if (time > 40) time = 40;

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::KeyPressed)
				if (event.key.code==Keyboard::X)
		        	entities.push_back(new Bullet(anim2,lvl,Mario.x+15,Mario.y+6,Mario.dir) );
		}


		if (Keyboard::isKeyPressed(Keyboard::Left)) Mario.key["L"]=true;
		if (Keyboard::isKeyPressed(Keyboard::Right)) Mario.key["R"]=true;
		if (Keyboard::isKeyPressed(Keyboard::Up)) Mario.key["Up"]=true;
		if (Keyboard::isKeyPressed(Keyboard::Down)) Mario.key["Down"]=true;
		if (Keyboard::isKeyPressed(Keyboard::Space)) Mario.key["Space"]=true;
		if (Keyboard::isKeyPressed(Keyboard::X)) Mario.key["X"] = true;
		if (Keyboard::isKeyPressed(Keyboard::P)) Mario.key["P"] = true;


		for(it=entities.begin();it!=entities.end();)
		{
			Entity *b = *it;
			b->update(time);
			if (b->life==false)	{ it  = entities.erase(it); delete b;}
			else it++;
		}


		Mario.update(time);
		
		healthBar.update(Mario.Health);
		

		for (it = entities.begin(); it != entities.end(); it++)
		{
			if ((*it)->Name == "EnemyDuck1")
			{

				Entity *enemy = *it;
				if (enemy->Health <= 0) continue;
				if (Mario.getRect().intersects(enemy->getRect()))
				{
					if (!Mario.hit) {
						Mario.Health -= 1; Mario.hit = true;
						
					}
					

				}
						for (std::list<Entity*>::iterator it2=entities.begin(); it2!=entities.end(); it2++)
						{
							Entity *bullet = *it2;
							if (bullet->Name=="Bullet")
								if ( bullet->Health>0)
									if  (bullet->getRect().intersects( enemy->getRect() ) )
									 { bullet->Health=0; enemy->Health-=1;}
							
							
						}

			}


		}

		//for(it=entities.begin();it!=entities.end();it++)
		//{
		//	//1. враги
		//	if ((*it)->Name=="EnemyDuck1")
		//	{
		//		Entity *enemy = *it;
		//		

		//		if (enemy->Health<=0) continue;

		//		if  (Mario.getRect().intersects( enemy->getRect() ))
		//		    if (Mario.dy>0) { enemy->dx=0; Mario.dy=-0.2; enemy->Health=0;}
		//		    else if (!Mario.hit) { Mario.Health-=5; Mario.hit=true;
		//		                         if (Mario.dir) Mario.x+=10; else Mario.x-=10;}


		//		for (std::list<Entity*>::iterator it2=entities.begin(); it2!=entities.end(); it2++)
		//		{
		//			Entity *bullet = *it2;
		//			if (bullet->Name=="Bullet")
		//				if ( bullet->Health>0)
		//					if  (bullet->getRect().intersects( enemy->getRect() ) )
		//					 { bullet->Health=0; enemy->Health-=5;}
		//		}
		//	}

		//	//2. движущиеся платформы
		//	if ((*it)->Name=="MovingPlatform")
		//	{
		//	    Entity *movPlat = *it;
		//		if  (Mario.getRect().intersects( movPlat->getRect() ) )
		//		   if (Mario.dy>0)
		//			  if (Mario.y+Mario.h<movPlat->y+movPlat->h)
		//			  {Mario.y=movPlat->y - Mario.h + 3; Mario.x+=movPlat->dx*time; Mario.dy=0; Mario.STATE=PLAYER::stay;}
		//	}

		//	//3.. и т.д.
		//}


		/////////////////////отображаем на экран/////////////////////
	
		
			view.setCenter( Mario.x,Mario.y);
		

		window.setView(view);

		background.setPosition(view.getCenter());
		window.draw(background);

		lvl.Draw(window);

		for(it=entities.begin();it!=entities.end();it++)
			(*it)->draw(window);

		Mario.draw(window);
		healthBar.draw(window);

		window.display();
	}


}

#endif GAME_H

