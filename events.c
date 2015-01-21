#include<stdio.h>
#include<unistd.h>
#include "castle.h"
#include "mybanner.h"
extern int investigate,input;
extern map_t castle_map;
extern player_t player;

void events_books(int id)
{
	puts("You found a strange spellbook\n"
		"Do you read it or leave it? (read/leave)");
	while(!choice(3,"investigate","read","leave"))
		if(investigate)
		{
			puts("You look at the cover.\n"
				"The title of this speelbook seems to be:");
			switch(id%4)
			{
				case 0: puts("\"Book of Death\"."); break;
				case 1: puts("\"Book of Teleportation\"."); break;
				case 2: puts("\"Book of Locomotion\"."); break;
				case 3: puts("\"Book of Alchemy\"."); break;
				default: puts("This ain't supposed to happen, contact provider of this game immediately!"); break; /*This obviously should never happen*/
			}
		}
		else puts("(read/leave)");
	if(input==1)
	{
		switch(id%4)
		{
			case 0:
				puts("You pick up the book\n"
					"And you can feel it draining your soul!");
				hit_player(player.health);
				break;
			case 1:
				puts("You pick up the book and start reading.\n"
					"Suddenly, you are blinded by a flash of light.\n"
					"You are now in a different chamber.");
				teleport_player();
				events_main_switch(castle_map.chamber[player.x][player.y]);
				break;
			case 2:
				puts("You pick up the book and start reading.\n"
					"Suddenly, you are lifted into the air.");
				move_player_random();
				break;
			case 3:
				puts("You pick up the book and start reading.");
				if(player.health>10)
				{
					player.immortal=1;
					puts("You sense some strange energy invigorating your body.\n"
						"According to the book you might\n"
						"have just obtained immortality.");
				}
				else heal_player(player.health/2);
				break;
			default:
				puts("This totally ain't supposed to happen, contact provider of this game immediately!");
				break;
		}
	}
	else puts("As you decide to leave it, the book disappears.");
} /*End of events_books*/
void events_potions(int id)
{
	puts("You found a strange potion!\n"
		"Do you drink it or leave it? (drink/leave)");
	while(!choice(3,"investigate","drink","leave"))
		if(investigate)
		{
			puts("You look at the label.\n"
				"It says:");
			switch(id%4)
			{
				case 0: puts("\"Essence of Death\"."); break;
				case 1: puts("\"Unstable Plasmid\"."); break;
				case 2: puts("\"Vigor of Valtor\"."); break;
				case 3: puts("\"The property of Nicholas Flamel\"."); break;
				default: puts("This ain't supposed to happen, contact provider of this game immediately!."); break;
			}
		}
		else puts("(drink/leave)");
	if(input==1)
	{
		switch(id%4)
		{
			case 0:
				puts("You decided to drink it,\n"
					"But as soon as you uncork it\n"
					"It's horrible smell causes you to faint");
				sleep(1);
				puts("Luckily, you wake up unharmed");
				break;
			case 1:
				puts("You drink the potion.\n"
					"You can feel the liquid burning your intestines.\n"
					"That horrible pain forces you to close your eyes.\n"
					"As soon as the pain ends you open your eyes\n"
					"And discover that you are in another chamber.");
				hit_player(2);
				if(player.died) return;
				teleport_player();
				events_main_switch(castle_map.chamber[player.x][player.y]);
				break;
			case 2:
				puts("You drink the potion.\n"
					"It invigorates your body!");
				heal_player(player.health);
				break;
			case 3:
				puts("You drink the potion.");
				if(player.health>10)
				{
					player.immortal=1;
					puts("You can feel the liquid burning your intestines\n"
						"But as soon as the pain stops, you feel much better.");
				}
				else puts("Nothing of interest happened.");
				break;
			default:
				puts("This totally ain't supposed to happen. contact provider of this game immediately!");
				break;
		}
	}
	else
	{
		puts("As you decide to leave it, the potion explodes\n"
			"You are hurt by glass shard from the vial");
		hit_player(1);
	}
} /*End of events_potions*/
void events_chests(int id)
{
	puts("You found a strange chest!\n"
		"Do you open it or leave it? (open/leave)");
	while(!choice(3,"investigate","open","leave"))
		if(investigate)
		{
			puts("You take a closer look at it.\n"
				"You can see");
			switch(id%4)
			{
				case 0: puts("a skull carved on it's side."); break;
				case 1: puts("a weird creature carved on it's side."); break;
				case 2: puts("that this chest is made of pure gold."); break;
				case 3: puts("that it's a perfectly ordinary chest."); break;
				default: puts("This ain't supposed to happen, contact provider of this game immediately!"); break;
			}
		}
		else puts("(open/leave)");
	if(input==1)
	{
		switch(id%4)
		{
			case 0:
				puts("You open the chest.\n"
					"There are some bones inside\n"
					"Luckily, nothing dangerous happens");
				break;
			case 1:
				puts("You open the chest\n"
					"And a strange creature inside bites you, and shuts chest's lid.");
				hit_player(player.health/4);
				if(!player.died)
					puts("You quickly apply the ignition spell to the chest.");
				break;
			case 2:
				puts("As soon as you touch the chest in order to open it\n"
					"You start turning into gold");
				hit_player(player.health);
				break;
			case 3:
				puts("The chest attacks you! It's a Mimic");
				if(player.health<5)
				{
					puts("You barely avoid some it's attacks");
					hit_player(1);
				}
				else
					puts("You easily dodge it's attacks");
				if(!player.died)
					puts("And you quickly apply the ignition spell to it.");
				break;
			default:
				puts("This totally ain't supposed to happen, contact provider of this game immediately!");
				break;
		}
	}
	else
		puts("As you decide to leave it, the chest disappears in flames.");
} /*End of events_chests*/
void event_42(int id)
{
	puts("You encounter a talking mouse.\n"
		"It asks you a single question:\n"
		"What is the Answer to the Ultimate Question\n" 
		"of Life, The Universe, and Everything?");
	while(!choice(8,"investigate\n","42\n","attack\n","fight\n","ignite\n","ignition\n","spell\n","igni")) /*An average player should figure one of those, Also, short input will not work correctly.*/
		if(investigate)
			puts("If you don't know the answer you may take a different approach");
		else
			puts("That mouse is getting impatient");
	if(input==1) 
	{
		advanced_banner("Correct!");
		puts("The mouse disappears in a bright flash.");
		if(id%5)
		{
			puts("It even left you a healing potion. You drink it immediately.");
			heal_player(id%5);
		}
	}
	else if(input>1)
	{
		puts("You decide to quickly apply the ignition spell to the mouse.\n"
			"Before you finish casting, it jumps at you and bites you");
		if(id%5) hit_player(id%5);\
		else puts("Lucilky, your clothes protect you from it's bite");
		puts("You decide to leave before any more mice appear");
	}
}
void events_main_switch(int id)
{
	switch(id)
	{
		case 0:
			puts("You've been here already");
			break;
		case 1:
			victory();
			break;
		case 2: case 3: case 4: case 5: case 6: /* -pedantic warns about case ranges GCC extension so can't do case 2 ... 6: */
			puts("You encounter a strange anomaly. It's a tornado!");
			player_was_here();
			move_player_random();
			if(player.moved)
			{
				sleep(1);
				events_main_switch(castle_map.chamber[player.x][player.y]);
			}
			break;
		case  7: case  8: case  9: case 10:  /*It's not as bad as it looks*/
		case 11: case 12: case 13: case 14:
		case 15: case 16: case 17: case 18:
		case 19: case 20: case 21: case 22:
			events_books(id);
			break;
		case 23: case 24: case 25: case 26:
		case 27: case 28: case 29: case 30:
		case 31: case 32: case 33: case 34:
		case 35: case 36: case 37: case 38:
			events_potions(id);
			break;
		case 39: case 40: case 41: case 42:
		case 43: case 44: case 45: case 46:
		case 47: case 48: case 49: case 50:
		case 51: case 52: case 53: case 54:
			events_chests(id);
			break;
		case 55: case 56: case 57: case 58: case 59:
			puts("You find a rift to the abbys\n"
				"You gaze into it, and the abyss gazes into you\n"
				"You decide to leave before anything else happens");
			break;
		case 60: case 61: case 62: case 63: case 64:
			puts("You find a painting on the wall.\n"
				"It has a three golden triangles on it.\n"
				"You decide to continue the search.");
			break;
		case 65: case 66: case 67: case 68: case 69:
			puts("You find an altar with a whale bone rune laying on it.\n"
				"before you decide what to do\n"
				"A man in a mask blinks in, takes the rune and blinks out");
			break;
		case 70: case 71: case 72: case 73: case 74:
			puts("You find an armless knight in a black armor.\n"
				"He wants to fight despite his wound.\n"
				"You quickly apply the ignition spell to him.\n"
				"You decide to leave and let him burn in peace.");
			break;
		case 75: case 76: case 77: case 78: case 79:
			event_42(id);
			break;
		default:
			puts("You found an empty chamber!"); 
			break;
	}
	player_was_here();
	player.moved=0;
} /*End of events_main_switch*/