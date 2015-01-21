#include<stdlib.h>
#include<stdio.h>
#include<stdarg.h>
#include<string.h>
#include<time.h>
#include<limits.h>
#include<unistd.h>
#include "mybanner.h"
#include "castle.h" /*prototypes, types and macros*/
	
int debug=0,DarkSoulsMode=0,input=0,investigate=0;
player_t player={0};
map_t castle_map={0};

void goodbye(void)
{
	puts("Have a nice day");
}
void initialize(void)
{
	system("clear");
	memset(&castle_map,0,sizeof(map_t));
	memset(&player,0,sizeof(player_t));
	player.max_health=(DarkSoulsMode?2:10); /*1 was way too low*/
	player.health=player.max_health;
	player.playing=1;
	puts("Welcome to Castle White."); sleep(1);
}
void print_map(void)
{
	FILE *fmap;
	int x=0,y=0;
	time_t time_buffer;
	if(!castle_map.ID) return;
	time(&time_buffer);
	fmap=fopen(castle_map.name,"a"); 
	fprintf(fmap,"Map id=%u, %s\n",castle_map.ID,ctime(&time_buffer));
	fprintf(fmap,"  #####################\n");
	for(y=MAP_SIZE-1;y>=0;y--)
	{
		if(y<MAP_SIZE-1) fprintf(fmap,"  # # # # # # # # # # #\n");
		fprintf(fmap,"%2d",y+1);
		for(x=0;x<MAP_SIZE;x++)
		{
			fprintf(fmap,"%c%c",(x==0?'#':' '),(castle_map.chamber[x][y]==1?'X':(castle_map.chamber[x][y]==0?'+':' ')));
		}
		fprintf(fmap,"#\n");
	}
	fprintf(fmap,"  #####################\n  ");
	for(x=1;x<=MAP_SIZE;x++) fprintf(fmap,"|%d",x);
	fprintf(fmap,"\nRooms you've explored:+, Treasure:X\n");
	fclose(fmap);
}
void print_map_debug(void)
{
	FILE *fmap;
	char debug_filename[21];
	int x=0,y=0;
	time_t time_buffer;
	time(&time_buffer);
	sprintf(debug_filename,"d_%s",castle_map.name);
	fmap=fopen(debug_filename,"w"); 
	fprintf(fmap,"Map id=%u, %s\n",castle_map.ID,ctime(&time_buffer));
	for(y=MAP_SIZE-1;y>=0;y--)
	{
		for(x=0;x<MAP_SIZE;x++)
		{
			fprintf(fmap,"[%2d,%2d]=%3d, ",x+1,y+1,castle_map.chamber[x][y]);
		}
		fprintf(fmap,"\n");
	}
	fclose(fmap);
}	
int choice(int number, ...)
{
	va_list opts;
	int i=0;
	char player_input[64]={0},input_string[32]={0}, short_input[3]=" \n\0";/*Possible Segfaults here, 32 should be enough.*/
	va_start(opts,number);
	input=0;
	investigate=0;
	fgets(player_input,sizeof(player_input),stdin);
	for(i=0;i<number;i++)
	{
		strcpy(input_string,va_arg(opts,char*));
		short_input[0]=input_string[0];
		if(!strcmp(player_input,input_string) || !strcmp(player_input,short_input))
		{	
			if(!i) investigate=1;
			else input=i;
			break;
		}
	}
	va_end(opts);
	return input;
}
void Continue(void)
{
	if(!player.playing) return;
	puts("Continue? (yes/no)");
	while(!choice(3,"","no\n","yes\n")) puts("(yes/no)");
	if(input==1) player.playing=0;
}
void death(void)
{
	player.died=1;
	advanced_banner("You died!");
}
void hit_player(int damage)
{
	if(!player.immortal)
	{
			player.health-=damage;
			sleep(1);
			if(player.health<=0) death();
	}
	else
		puts("As an immortal you are unharmed");
} 
void heal_player(int life)
{
	player.health+=life;
	puts("You feel a bit better");
}	
enum direction_t get_dir(void)
{
	enum direction_t dir=UNKNOWN;
	char player_input[64]={0};
	while(1)
	{
		puts("#####\nWhere do you want to go\n(north/south/east/west)(suicide to quit)");
		fputs("-go ",stdout);
		fgets(player_input,sizeof(player_input),stdin);
		if(!strcmp(player_input,"suicide\n"))
		{
			puts("Feeling suicidal, you decide to pass away");
			player.immortal=0;
			hit_player(player.health);
			break;
		}
		else if(!strcmp(player_input,"north\n") || !strcmp(player_input,"n\n"))
		{
			dir=NORTH;
			break;
		}
		else if(!strcmp(player_input,"south\n") || !strcmp(player_input,"s\n"))
		{
			dir=SOUTH;
			break;
		}
		else if(!strcmp(player_input,"east\n") || !strcmp(player_input,"e\n"))
		{
			dir=EAST;
			break;
		}
		else if(!strcmp(player_input,"west\n") || !strcmp(player_input,"w\n"))
		{
			dir=WEST;
			break;
		}
		else
		{
			puts("I can't understand you.\nTry again.");
			continue;
		}
		system("clear");
	}
	return dir;
}
void move_player(enum direction_t direction)
{
	player.moved=0;
	switch(direction)
	{
		case NORTH:
			if(player.y==(MAP_SIZE-1))
			{
				puts("You hit the wall");
				hit_player(1);
			} 
			else
			{
				player.y++;
				player.moved=1;
			}
			break;
		case SOUTH:
			if(player.y==0)
			{
				puts("You hit the wall");
				hit_player(1);
			}
			else 
			{
				player.y--; 
				player.moved=1;	
			}
			break;
		case EAST:
			if(player.x==(MAP_SIZE-1))
			{
				puts("You hit the wall");
				hit_player(1);
			}
			else
			{
				player.x++;
				player.moved=1;
			}
			break;
		case WEST:
			if(player.x==0)
			{
				puts("You hit the wall");
				hit_player(1);
			}
			else
			{
				player.x--;
				player.moved=1;
			}
			break;
		default:
			break;
	}
}
void move_player_random(void)
{
	puts("A tremendous force throws you into the wall");
	player_was_here();
	move_player(rand()%4+1);
	if(player.moved)
	{
		puts("Luckily, there were doors that opened right before you hit them.\n"
			"You enter another castle chamber.");
		events_main_switch(castle_map.chamber[player.x][player.y]);
	}
}
void teleport_player(void)
{
	player.x=rand()%MAP_SIZE;
	player.y=rand()%MAP_SIZE;
}
void player_was_here(void) /* Marks current chamber as visited */
{
	castle_map.chamber[player.x][player.y]=0;
}
void victory(void)
{
	puts("You've found the treasure!");
	advanced_banner("Congratulations!");
	player.won=1; sleep(1);
	puts("Thank you for playing.\n"
		"As a reward, I will tell you a secret.\n"
		"Use 'investigate' to get more information about your surrounding.\n"
		"And/or add -DarkSoulsMode flag when launching\n"
		"to make game more difficult\n"
		"A map of your castle will be saved to a text file.\n");
	print_map();
}
void get_ID(void)
{
	char player_input[64]={0};
	puts("Please, Initialize your castle by entering");
	printf("A number ranging from 1 to %u\n",UINT_MAX);
	while(!castle_map.ID)
	{
		fgets(player_input,sizeof(player_input),stdin);
		sscanf(player_input,"%u",&castle_map.ID);
		if(!castle_map.ID) puts("Try again");
	}
	sprintf(castle_map.name,"map%u.txt",castle_map.ID);
}
void generate_map(void)
{
	int x=0,y=0;
	srand(castle_map.ID);
	for(y=MAP_SIZE-1;y>=0;y--)
		for(x=0;x<MAP_SIZE;x++)
			castle_map.chamber[x][y]=rand()%(EV_LIM-1)+2;
	x=rand()%MAP_SIZE;
	y=rand()%MAP_SIZE;
	castle_map.chamber[x][y]=1;
}
void debug_map(void)
{
	get_ID();
	generate_map();
	print_map();
	print_map_debug();
}
void debugger(void)
{
	int event_id=0;
	char player_input[64]={0};
	puts("debug (map/events/banner)");
	while(!choice(4,"","events\n","map\n","banner\n")) puts("(map/events/banner)");
	switch(input)
	{
	case 1:
		while(event_id>=0)
		{
			if(player.health<=0) initialize();
			printf("Test events 0-%d, negative to quit\n",EV_LIM);
			fgets(player_input,sizeof(player_input),stdin);
			sscanf(player_input,"%d",&event_id);
			events_main_switch(event_id);
		}
		break;
	case 2:
		debug_map();
		break;
	case 3:
		fgets(player_input,sizeof(player_input),stdin);
		advanced_banner(player_input);
		break;
	default:
		break;
	}
	player.playing=0;
} /*End of debugger*/
void parse_commands(int argc, char *argv[])
{
	int i=0;
	if(argc>1) for(i=1;i<argc;i++)
	{
		if(!strcmp(argv[i],"-DarkSoulsMode")) DarkSoulsMode=1;
		else if(!strcmp(argv[i],"-debug")) debug=1;
	}
}
void get_castle(void)
{
	while(player.playing)
	{
		puts("Your castle appears to be uninitialized"); sleep(1);
		puts("Would you like to initialize it? (yes/no)");
		while(!choice(3,"investigate\n","yes\n","no\n"))
			if(investigate) 
				puts("You need to initialize your castle\n"
					"before you can explore it\n"
					"Would you like to initialize it? (yes/no)");
			else puts("(yes/no)");
		if(input==1) break;
		else
		{
			puts("Your refusal to initialize the Castle White had angered the gods."); sleep(1);
			puts("Do you want to apologize them? (yes/no)");
			while(!choice(3,"investigate\n","yes\n","no\n")) 
				if(investigate) puts("You better apologize the gods or something bad will happen\n"
							"Do you want to apologize them? (yes/no)");
				else puts("Do you want to apologize them? (yes/no)");
			if(input==1) puts("Gods have accepted the apology.");
			else
			{
				puts("You have been struck by a lighting.");
				hit_player(player.health);
			}
		}
		if(player.died) return;
	}
	get_ID();
	if(player.died) return;
	generate_map();
	puts("You stand in awe as your castle rises from the ground");
} /*End of get_castle*/
void get_inside_castle(void)
{
	teleport_player();
	printf("You have been teleported into the castle\nat [%d,%d] coordinates\n",player.x+1,player.y+1); sleep(1);
	puts("Your castle apears to have a treasure inside!\n"
		"Would you kindly find it?\n");
	sleep(1);
	events_main_switch(castle_map.chamber[player.x][player.y]);
}
void play(void)
{
	while(!player.died && !player.won)
	{
		while(!player.moved)
		{
			sleep(1);
			move_player(get_dir());
			if(player.died) return;
		}
		system("clear");
		events_main_switch(castle_map.chamber[player.x][player.y]);
	}
}
int main(int argc, char *argv[])
{
	atexit(goodbye);
	parse_commands(argc,argv);
	if(debug) debugger();
	else player.playing=1;
	while(player.playing)
	{
		initialize();
		get_castle();
		if(!player.died && !player.won)
		{
			srand(time(NULL));
			get_inside_castle();
			play();
		}
		Continue();
	}
	return 0;
}