//
//  object.h
//  GalaxySavior
//
//  Created by sree on 09/01/14.
//  Copyright (c) 2014 sree. All rights reserved.
//

//enum IDS{PLAYER, BULLET, ENEMY};

//Our Player
# include <stdlib.h>
typedef struct
{
	//int ID;
	int x;
	int y;
	int lives;
	int speed;
	int boundx;
	int boundy;
	int score;
}SpaceShip;

typedef struct
{
	//int ID;
	int x;
	int y;
	bool live;
	int speed;
}Bullet;

typedef struct
{
	//int ID;
	int x;
	int y;
	bool live;
	int speed;
	int boundx;
	int boundy;
}Commet;

int random_in_range (unsigned int min, unsigned int max)
{
    int base_random = rand(); /* in [0, RAND_MAX] */
    if (RAND_MAX == base_random) return random_in_range(min, max);
    /* now guaranteed to be in [0, RAND_MAX) */
    int range       = max - min,
    remainder   = RAND_MAX % range,
    bucket      = RAND_MAX / range;
    /* There are range buckets, plus one smaller interval
     within remainder of RAND_MAX */
    if (base_random < RAND_MAX - remainder) {
        return min + base_random/bucket;
    } else {
        return random_in_range (min, max);
    }
}