#include "stdafx.h"
#include "Header.h"

int InitiationProcess(const byte *bookname)
{
	FILE* booksource = NULL;
	ts_fighter *fighters = { 0 };
	ts_innkeeper the_inkeeper = { 0 };
	int innkeeper_status = 0;
	uint32_t idx = 0;
	uint32_t number_fighters = 0;
	uint16_t age = 0;

	if (NULL == (booksource = (fopen(bookname, "r")))) return 1;

	/* Reading from the book (a .txt file with header <age | name_length | name>) */
	for (idx = 0; ; ++idx)
	{
		if (fscanf(booksource, "%hu | ", &age) != 1) break;

		if (NULL == (fighters = (ts_fighter *)realloc(fighters, sizeof(ts_fighter) * (idx + 1)))) return 3;

		fighters[idx].age = age;

		if ((fscanf(booksource, "%u | ", &fighters[idx].name_len)) != 1) return 6;

		if (NULL == (fighters[idx].name = (byte *)malloc(sizeof(byte) * fighters[idx].name_len))) return 4;

		if (fscanf(booksource, "%s\n", fighters[idx].name) != 1) return 6;

		fighters[idx].id = (idx + 1);
	}
	/* Initializing the innkeeper */
	the_inkeeper.base_properties.name_len = strlen(INNKEEPER_NAME);
	if (NULL == (the_inkeeper.base_properties.name = (byte *)malloc(sizeof(byte) * the_inkeeper.base_properties.name_len))) return 4;
	strcpy(the_inkeeper.base_properties.name, INNKEEPER_NAME);
	the_inkeeper.base_properties.age = INNKEEPER_AGE;

	number_fighters = idx;

	for (; ; )
	{
		if (number_fighters <= 1) break;

		/* Picking opponent can be used to decice when the innkeeper to show */
		/* 100/CHANCE_OF_SPAWN is the CHANCE OF SPAWN in percentage if equal to 1 */
		if (PickingOponents(100 / CHANCE_OF_SPAWN) == 1)
		{
			if (innkeeper_status == 1) continue;
			/* The innkeeper has shown */
			printf("IZDISLAV!");
			the_inkeeper.executeFight = Fight_with_Innkeeper;
			innkeeper_status = the_inkeeper.executeFight(number_fighters, fighters);
			number_fighters--;
		}
		else
		{
			CallingTheFight(fighters, number_fighters);
			number_fighters--;
		}
	}

	ChickenDinner(fighters[0]);
	free(fighters);
	if (fclose(booksource) != 0) return 2;
	return 0;
}

void CallingTheFight(ts_fighter *fighters, uint32_t number_fighters)
{
	uint32_t idx = 0;
	uint32_t fighter1_number = 0;
	uint32_t fighter2_number = 0;
	uint32_t loser_number = 0;

	/* Picking fighter1 and then looping until fighter2 is different from fighter1 */
	fighter1_number = PickingOponents(number_fighters);
	do
	{
		fighter2_number = PickingOponents(number_fighters);
	} while (fighter1_number == fighter2_number);

	/* Finding the winner and the loser */
	loser_number = TheFight(fighter1_number, fighter2_number);

	/* Getting rid of the loser and ordering the array */
	for (idx = loser_number; idx < (number_fighters - 1); ++idx)
	{
		fighters[idx] = fighters[idx + 1];
	}
}

int Fight_with_Innkeeper(uint32_t numberOpponents, ts_fighter *fighters)
{
	uint32_t idx = 0;
	int fighter_HP = FULL_HP;
	int innkeeper_HP = FULL_HP_INNKEEPER;
	uint32_t fighter_id = PickingOponents(numberOpponents);

	/* fighter1 hits first so fighter2 takes the damage first */
	while (1)
	{
		fighter_HP -= FighterHit();
		if (fighter_HP <= 0)
		{
			/* Getting rid of the loser and ordering the array */
			for (idx = fighter_id; idx < (numberOpponents - 1); ++idx)
			{
				fighters[idx] = fighters[idx + 1];
			}
			return 0;
		}
		innkeeper_HP -= FighterHit();
		if (innkeeper_HP <= 0)
		{
			printf("The innkeeper died!");
			return 1;
		}
	}
	return 0;
}

uint32_t PickingOponents(uint32_t numberOpponents)
{
	time_t timer;
	uint32_t generatedNum = 0;

	srand((unsigned)time(&timer));

	generatedNum = (rand() % numberOpponents);

	return generatedNum;
}

uint32_t TheFight(uint32_t id1, uint32_t id2)
{
	int fighter1_HP = FULL_HP;
	int fighter2_HP = FULL_HP;

	/* fighter1 hits first so fighter2 takes the damage first */
	while (1)
	{
		fighter2_HP -= FighterHit();
		if (fighter2_HP <= 0) return id2;
		fighter1_HP -= FighterHit();
		if (fighter1_HP <= 0) return id1;
	}
}

uint16_t FighterHit()
{
	time_t clock;
	uint16_t generatedNum = 0;
	uint16_t generatedNum1 = 0;
	uint16_t generatedNum2 = 0;

	srand((unsigned)time(&clock));

	generatedNum1 = (uint16_t)(rand() % MAX_DMG);
	generatedNum2 = (uint16_t)(rand() % MAX_DMG);

	/* I make sure that most of the time the fit is between
	5 and 15 (the numbers closest to the middle) */
	generatedNum = (uint16_t)((generatedNum1 + generatedNum2) / 2);

	return generatedNum;
}

void ChickenDinner(ts_fighter winner)
{
	printf("Winner winner chicken dinner\n");
	printf("At age of %d, %s won the tournament of the north-western fight club", winner.age, winner.name);
}

void PoliceReport(int errNum)
{
	switch (errNum)
	{
	case 0:
		break;
	case 1:
		printf("Problem with opening the book!");
		break;
	case 2:
		printf("Problem with closing the book!");
		break;
	case 3:
		printf("Problem with making space for the fighters!");
		break;
	case 4:
		printf("Problem with the name of the opponent (suspicious dude)!");
		break;
	case 5:
		printf("Fight not well initiated!");
		break;
	case 6:
		printf("Problem with the spelling of the names!");
		break;
	case 7:
		printf("Other problem!");
		break;
	}
}