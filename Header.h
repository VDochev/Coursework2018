#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef unsigned int uint32_t;
typedef unsigned int size_t;
typedef unsigned short uint16_t;
typedef char byte;

#define FULL_HP ((uint16_t) 100)
#define FULL_HP_INNKEEPER ((uint16_t) 120)
#define MAX_DMG ((uint16_t) 20)
#define INNKEEPER_AGE ((uint16_t) 30)
#define INNKEEPER_NAME ((const byte *) "Desislav Andreev")
#define	CHANCE_OF_SPAWN ((uint16_t) 20)

/* Reads from the file the name length, the name and the id  */
typedef struct fighter
{
	uint32_t id;
	uint16_t age;
	size_t name_len;
	byte *name;
} ts_fighter;

/* Structure of the inkeeper */
typedef struct innkeeper
{
	ts_fighter base_properties;
	int(*executeFight) (uint32_t, ts_fighter *);
} ts_innkeeper;

/* Reads the names of the fighters before the fight */
int InitiationProcess(const byte *bookname);

/* The generator of the random fighter number */
uint32_t PickingOponents(uint32_t numberOpponents);

/* Matching two opponents in a fight, takes the two fighters, call the fight and declare a winner */
void CallingTheFight(ts_fighter *fighters, uint32_t number_fighters);

/* Matching the inkeeper with opponent in a fight */
int Fight_with_Innkeeper(uint32_t numberOpponents, ts_fighter *fighters);

/* The fight: takes the id of the opponents, returns the id of the defeated fighter */
uint32_t TheFight(uint32_t id1, uint32_t id2);

/* Returns the inflicted damage: random number between 1 and 20 */
uint16_t FighterHit();

/* Calling the winner of the fight, takes the structure of the winner */
void ChickenDinner(ts_fighter winner);

/* The error logger of the file */
void PoliceReport(int errNum);