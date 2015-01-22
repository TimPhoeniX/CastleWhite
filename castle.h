#ifndef CASTLE_H
#define CASTLE_H

#define MAP_SIZE 10
#define EV_LIM 90

enum direction_t {UNKNOWN,NORTH,SOUTH,EAST,WEST};

typedef struct player_t
{
	int max_health;
	int health;
	int moved;
	int won;
	int died;
	int immortal;
	int playing;
	int x;
	int y;
} player_t;

typedef struct map_t
{
	unsigned int ID;
	char name[19];	
	int chamber[MAP_SIZE][MAP_SIZE];
} map_t;

int get_player_input(int n, ...);

enum direction_t get_dir(void);

void Continue(void);
void death(void);
void debugger(void);
void debug_map(void);
void events_books(int id);
void events_chests(int id);
void events_main_switch(int id);
void events_potions(int id);
void generate_map(void);
void get_castle(void);
void get_ID(void);
void get_inside_castle(void);
void goodbye(void);
void heal_player(int life);
void hit_player(int damage);
void initialize(void);
void move_player(enum direction_t direction);
void move_player_random(void);
void parse_commands(int argc, char *argv[]);
void player_was_here(void);
void play(void);
void print_map_debug(void);
void print_map(void);
void teleport_player(void);
void victory(void);


#endif