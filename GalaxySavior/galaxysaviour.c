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
enum MKEYS{KEY_UP,KEY_DOWN,KEY_LEFT,KEY_RIGHT};
bool keys[4]= {false,false,false,false};

void init_ship(SpaceShip *ship);
void draw_ship(SpaceShip *ship);
void move_ship_left(SpaceShip *ship);
void move_ship_right(SpaceShip *ship);
void move_ship_up(SpaceShip *ship);
void move_ship_down(SpaceShip *ship);


int main(void)
{
    bool done =false;
    bool redraw = true;
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_TIMER *timer =NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    SpaceShip ship;
    
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
            }if(keys[KEY_RIGHT])
            {
                move_ship_right(&ship);
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
                    
            }
        }
        
        if(redraw && al_event_queue_is_empty(event_queue))
        {
            redraw = false ;
            al_clear_to_color(al_map_rgb(255,100,100));
            draw_ship(&ship);
            al_flip_display();
            
            
            
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
    ship->boundx = 10;
    ship->boundy = 20;
    ship->ID=PLAYER;
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