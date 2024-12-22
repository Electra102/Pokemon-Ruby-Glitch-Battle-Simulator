#ifndef STRUCTS_H
#define STRUCTS_H

//done
#include "integer_types.h"
#include "abilities.h"
#include "field_effects.h"
#include "item_effects.h"
#include "status_effects.h"
#include "move_effects.h"
#include "types.h"

typedef struct
{
	uint16 species;
	//uint16_t dex_number;
	//char* name;
	uint8* abilities; //two slots
	uint8* types; //two slots
	uint8* basestats; //six slots
	//uint8_t exp_curve;
	//uint8_t cry;
} PokemonData;

typedef struct
{
	uint8 effect;
	uint8 id;
	char* name;
} Item;

//done
typedef struct
{
	uint16 index;
	//char* name;
	uint8 type;
	uint8 effect;
	uint8 base_power;
	uint8 pp;
	uint8 accuracy;
	uint8 effect_accuracy;
	int8 priority;
} Move;

typedef struct
{
	PokemonData* pokemondata;
	uint16* stats; //6 items. why was it 8? maybe I confused it with stat_modifiers, which used to have 8 slots?
	char* nickname;
	uint8 abilityslot_level; //most significant bit stores the ability slot, remaining 7 bits store the level
	Item* held_item; //might be done
	Move** moveset; //four slots, one for a pointer to each move
	uint8* pp; //four slots, one for each move's pp
	uint8 lasting_status_effect; //0 = none, 1 = burn, 2 = paralysis, 3 = sleep, 4 = freeze, 5 = poison, 6 = toxic
	uint8 num_status_effects;
	uint16 hp_remaining;
	uint16 max_hp;
	uint8 nature; //25 available
} PartyMember;

typedef struct
{
	PartyMember* dataisfor; // pointer to the partymember that is currently on the field and this data corresponds to
	int8* stat_modifiers;//8 members long. 0- atk, 1- def, 2- spa, 3- spd, 4- spe, 5- accuracy, 6- evasion, 7- critrate. 
	uint8 ability; //most significant bit is set when the ability is active. the rest of the bits are for determining the ability
	uint8* temp_status_effects; //position 0 is reserved for stockpile count
	uint8 num_status_effects;
	uint8* types;
	uint16 stored_damage;
} OnField;

typedef struct
{
	uint8 fieldeffects; //format is aaabbbcd, aaa = turns left of light screen (0 if not set), bbb = turns of reflect, c = if mud sport is set, d = if water sport is set
	//uint8_t field_effects_members;
	PartyMember** team;
	OnField* onfield;
	Move* last_move_targeted_with;
} Side;

typedef struct
{
	uint8 turn_weather_spikes; //format is WXXXYYZZ. W = if it is your turn (bit is set if it is), XXX = weather, YY = spikes set on your side, ZZ = spikes set on opponent's side
	//2 bits for your side's spikes count, 2 bits forthe spikes on the opponent's side, 3 bits for weather
	//most-significant bit determines whose turn it is- not set if it is your turn, set if it is the opponent's turn
	//for weather, 000 = No weather, 001 = harsh sun, 010 = rain, 011 = sand, 100 = hail, rest are invalid
	//check for 16, 32, 48, 64 to see if weather is set and valid
	Side* your_side;
	Side* opp_side;
} Field;

#endif