//
//  galaxysaviour.c
//  GalaxySavior
//
//  Created by sree on 09/01/14.
//  Copyright (c) 2014 sree. All rights reserved.
//

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "object.h"

const int SCREEN_W = 400;
const int SCREEN_H = 400;
const int x=0;
const int y=0;
const int FPS = 60;
const int num_bullets=5;
const int num_commets = 65;
const bool GameOver= false;
int   x_temp=0;
int   y_temp=10;

enum MKEYS{KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT,KEY_SPACEBAR};
bool keys[5]= {false,false,false,false,false};

void init_ship(SpaceShip *ship);
void draw_ship(SpaceShip *ship);
void move_ship_left(SpaceShip *ship);
void move_ship_right(SpaceShip *ship);
void move_ship_up(SpaceShip *ship);
void move_ship_down(SpaceShip *ship);

void init_bullet(Bullet bullet[]);
void draw_bullet(Bullet bullet[]);
void fire_bullet(Bullet bullet[],SpaceShip *ship);
void update_bullet(Bullet bullet[]);

void init_commet(Commet commet[]);
void draw_commet(Commet commet[]);
void update_commet(Commet commet[]);

void bullet_collision(Bullet bullet[],Commet commet[]);
//void ship_collision(Ship ship[] , Commet commet[]);


int main(void)
{
    bool done =false;
    bool redraw = true;
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_TIMER *timer =NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    SpaceShip ship;
    Bullet bullet[num_bullets];
    Commet commet[num_commets];
    if(!al_init())
    {
        fprintf(stderr,"allegro is not intialized");
        return -1;
    }
    display = al_create_display(SCREEN_W,SCREEN_H);
    if(!display)
    {
        fprintf(stderr,"failed to intialize the display");
        return -1;
    }
    
    timer  = al_create_timer(1.0/FPS);
    if(!timer)
    {
        fprintf(stderr,"failed to intialize timer");
        al_destroy_display(display);
        return -1;
    }
    event_queue = al_create_event_queue();
    if(!event_queue)
    {
        fprintf(stderr,"failed to create event queue");
        al_destroy_display(display);
        al_destroy_timer(timer);
        return -1;
    }
    al_install_keyboard();
    al_register_event_source(event_queue,al_get_timer_event_source(timer));
    al_register_event_source(event_queue,al_get_keyboard_event_source());
    al_register_event_source(event_queue,al_get_display_event_source(display));
    init_ship(&ship);
    init_bullet(&bullet);
    init_commet(&commet);
    al_start_timer(timer);
    while(!done)
    {
        
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue,&ev);
        
        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            if(keys[KEY_UP])
            {
                move_ship_up(&ship);
            }
            if(keys[KEY_DOWN])
            {
                move_ship_down(&ship);
            }if(keys[KEY_LEFT])
            {
                move_ship_left(&ship);
            }
            if(keys[KEY_RIGHT])
            {
                move_ship_right(&ship);
            }
            if(keys[KEY_SPACEBAR])
            {
                fire_bullet(&bullet,&ship);
            }
            if(!GameOver)
            {
                update_bullet(&bullet);
                update_commet(&commet);
                bullet_collision(&bullet,&commet);
            }
            
            redraw = true;
        }
        else if(ev.type==ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {
                case ALLEGRO_KEY_UP:
                {
                    keys[KEY_UP]=true;
                    break;
                }
                case ALLEGRO_KEY_DOWN:
                {
                    keys[KEY_DOWN]=true;
                    break;
                }
                case ALLEGRO_KEY_LEFT:
                {
                    keys[KEY_LEFT]=true;
                    break;
                }
                case ALLEGRO_KEY_RIGHT :
                {
                    keys[KEY_RIGHT]=true;
                    break;
                }
                case ALLEGRO_KEY_SPACE:
                {
                    keys[KEY_SPACEBAR] = true;
                    break;
                }
                    
            }
        }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
                case ALLEGRO_KEY_UP:
                {
                    keys[KEY_UP]=false;
                    break;
                }
                case ALLEGRO_KEY_DOWN:
                {
                    keys[KEY_DOWN]=false;
                    break;
                }
                case ALLEGRO_KEY_LEFT:
                {
                    keys[KEY_LEFT]=false;
                    break;
                }
                case ALLEGRO_KEY_RIGHT :
                {
                    keys[KEY_RIGHT]=false;
                    break;
                }
                case ALLEGRO_KEY_SPACE:
                {
                    keys[KEY_SPACEBAR]=false;
                    break;
                }
                    
            }
        }
        
        if(redraw && al_event_queue_is_empty(event_queue))
        {
            redraw = false ;
            al_clear_to_color(al_map_rgb(255,100,100));
            draw_ship(&ship);
            draw_bullet(bullet);
            draw_commet(commet);
            al_flip_display();
            al_clear_to_color(al_map_rgb(255,100,100));
            
            
        }
    }
    al_destroy_display(display);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    return 0;
}

void init_ship(SpaceShip *ship)
{
    ship->x=SCREEN_W/2;
    ship->y=0;
    ship->lives = 3;
    ship->x = SCREEN_H/2;
    ship->y = SCREEN_W/2;
   // ship->ID=PLAYER;
    ship->speed =10 ;
    ship->score = 0;
}

void draw_ship(SpaceShip *ship)
{
    al_draw_filled_triangle(ship->x, ship->y , ship->x-20 , ship->y+20, ship->x+20, ship->y+20, al_map_rgb(0,0, 255));
}


void move_ship_right(SpaceShip *ship)
{
    ship->x = ship->speed + ship->x;
    
    if(ship->x+20>SCREEN_W)
    {
        ship->x = SCREEN_W-20;
    }
}

void move_ship_left(SpaceShip *ship)
{
    ship->x = ship->x-ship->speed;
    if(ship->x<20)
    {
        ship->x = 20;
    }
}

void move_ship_up(SpaceShip *ship)
{
    ship->y =  ship->y-ship->speed;
    if(ship->y<0)
    {
        ship->y = 0;
    }
    
    
}
void move_ship_down(SpaceShip *ship)
{
    
    ship->y = ship->speed + ship->y;
    if(ship->y+20>SCREEN_H)
    {
        ship->y = SCREEN_H-20;
    }
    
}

void init_bullet(Bullet *bullet)
{
    for (int i=0;i<num_bullets;i++)
    {
        
  //bullet[i].ID=BULLET;
	bullet[i].live = false;
	bullet[i].speed=6;
   
    }
}

void fire_bullet(Bullet *bullet,SpaceShip *ship)
{
    for (int i=0;i<num_bullets;i++)
    {
    if(!bullet[i].live)
    {
        bullet[i].x = ship->x;
        bullet[i].y = ship->y-20;
        bullet[i].live = true;
        break;
     }
    }
}

void draw_bullet(Bullet *bullet)
{
    for (int i=0;i<num_bullets;i++)
    {
    if(bullet[i].live)
    {
      //  al_draw_filled_rectangle(bullet[i].x,bullet[i].y,bullet[i].x,bullet[i].y+3,al_map_rgb(0,0,0));
        al_draw_filled_circle(bullet[i].x,bullet[i].y,2,al_map_rgb(100,100,100));
       // fprintf(stderr," %d) the co-ordinates of x %d and the y position %d ",i,bullet[i].x,bullet[i].y);
    }
    }
   // al_draw_filled_circle(40,10,10,al_map_rgb(100,100,100));
    
}

void update_bullet(Bullet *bullet)
{
    for(int i = 0; i < num_bullets; i++)
	{
		if(bullet[i].live)
		{
			bullet[i].y -= bullet[i].speed;
			if(bullet[i].y<0)
				bullet[i].live = false;
		}
	}
}

void init_commet(Commet *commet)
{
    int temp_pos=0;
    int co=1;
    for(int i=0; i<num_commets;i++)
    {
        commet[i].boundx =0 ;
        commet[i].boundy =5 ;
        commet[i].live = true;
        commet[i].speed=1 ;
        if(i!=0)
        {
        commet[i].x =commet[i-1].x+30;
        }
        else
        {
            commet[i].x = 30;
        }
        if((float)i/13==(float)(co))
        {
            temp_pos=-40*co;
            co++;
            commet[i].x = 30;
        }
        
        commet[i].y = temp_pos;
        
      
    }

}

void draw_commet(Commet *commet)
{
    for(int i=0; i<num_commets;i++)
    {
    //    al_draw_filled_ellipse(commet[i].x,commet[i].y,,al_map_rgb(255,100, 0));
        if(commet[i].live == true)
        {
        al_draw_filled_circle(commet[i].x,commet[i].y,10,al_map_rgb(100,100,100));
        }
       //  al_draw_filled_circle(commet[i].x+30,commet[i].y,10,al_map_rgb(100,100,100));
       // fprintf(stderr, "the x position %d y position %d and the comet %d\n",commet[i].x,commet[i].y,i);
        
        
    }
    
}

void update_commet(Commet *commet)
{
    for(int i=0; i<num_commets;i++)
    {
        if(commet[i].live == true)
        commet[i].y += commet[i].speed;
        if(commet[i].y>SCREEN_H)
        {
            commet[i].live =false;
        }
    }
    

}

void bullet_collision(Bullet *bullet,Commet *commet)
{
    for(int i = 0; i <num_bullets; i++)
	{
		if(bullet[i].live)
		{
			for(int j =0; j <num_commets; j++)
			{
				if(commet[j].live)
				{
					if(commet[j].y==bullet[i].y&&bullet[j].x==commet[i].x)
                    {
						bullet[i].live = false;
						commet[j].live = false;
                        
						//ship.score++;
					}
				}
			}
		}
	}
    
}


