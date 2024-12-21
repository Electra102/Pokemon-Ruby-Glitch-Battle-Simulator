#include "move_functionality.h"
#include "damage.h"
#include "abilities.h"
#include "move_categories.h"
#include "move_effects.h"
#include "field_effects.h"
#include "default_moves.h"
#include "item_effects.h"
#include "held_items.h"
#include "non_move.h"
#include "status_effects_implementation.h"

uint8 affectstarget(Side* attackingside, Side* defendingside, Move* move)
{
	if (!typeeffectiveness(move->type, defendingside->onfield->types[0]) * typeeffectiveness(move->type, defendingside->onfield->types[1]))
		return 0;
	else if (typeeffectiveness(move->type, defendingside->onfield->types[0]) * typeeffectiveness(move->type, defendingside->onfield->types[1]) < 16 && defendingside->onfield->ability == ABILITY_WONDER_GUARD && move->effect != EFFECT_FUTURE_SIGHT && move->effect != EFFECT_BEAT_UP && move->index != MOVE_STRUGGLE)
	{
		printf("%s\'s move didn't affect %s because of its ability WONDER GUARD and the move was neither super-effective nor did it ignore WONDER GUARD\n", attackingside->onfield->dataisfor->nickname, defendingside->onfield->dataisfor->nickname);
		return 0;
	}
	else if (move->type == TYPE_GROUND && defendingside->onfield->ability == ABILITY_LEVITATE)
	{
		printf("%s\'s move didn't affect %s because of its ability LEVITATE\n", attackingside->onfield->dataisfor->nickname, defendingside->onfield->dataisfor->nickname);
		return 0;
	}
	else if (move->type == TYPE_WATER && defendingside->onfield->ability == ABILITY_WATER_ABSORB)
	{
		defendingside->onfield->dataisfor->hp_remaining += (defendingside->onfield->dataisfor->max_hp / 4);
		if (defendingside->onfield->dataisfor->hp_remaining > defendingside->onfield->dataisfor->max_hp)
			memcpy(&defendingside->onfield->dataisfor->hp_remaining, (void*)&defendingside->onfield->dataisfor->max_hp, sizeof(uint16));
		printf("%s\'s move healed %s because of its ability WATER ABSORB\n", attackingside->onfield->dataisfor->nickname, defendingside->onfield->dataisfor->nickname);
		return 0;
	}
	else if (move->type == TYPE_ELECTRIC && defendingside->onfield->ability == ABILITY_VOLT_ABSORB)
	{
		defendingside->onfield->dataisfor->hp_remaining += (defendingside->onfield->dataisfor->max_hp / 4);
		if (defendingside->onfield->dataisfor->hp_remaining > defendingside->onfield->dataisfor->max_hp)
			memcpy(&defendingside->onfield->dataisfor->hp_remaining, (void*)&defendingside->onfield->dataisfor->max_hp, sizeof(uint16));
		printf("%s\'s move healed %s because of its ability VOLT ABSORB\n", attackingside->onfield->dataisfor->nickname, defendingside->onfield->dataisfor->nickname);
		return 0;
	}
	else if (move->type == TYPE_FIRE && defendingside->onfield->ability == ABILITY_FLASH_FIRE)
	{
		defendingside->onfield->ability = ACTIVE_ABILITY_FLASH_FIRE;
		printf("%s\'s move activated %s\'s ability FLASH FIRE\n", attackingside->onfield->dataisfor->nickname, defendingside->onfield->dataisfor->nickname);
		return 0;
	}
	else if (move->type == TYPE_ELECTRIC && defendingside->onfield->ability == ABILITY_LIGHTNING_ROD)
	{
		attemptstatchange(defendingside->onfield->stat_modifiers[3], 1);
		printf("%s\'s move activated %s\'s ability LIGHTNING ROD\n", attackingside->onfield->dataisfor->nickname, defendingside->onfield->dataisfor->nickname);
		return 0;
	}
	else
		return 1;
}
uint8 attemptstatus(OnField* victim, const uint8 status, Move* move)
{
	if (victim->dataisfor->lasting_status_effect == 0)
	{
		if (status == STATUS_POISON && victim->types[0] != TYPE_POISON && victim->types[1] != TYPE_POISON && victim->types[0] != TYPE_STEEL && victim->types[1] != TYPE_STEEL)
		{
			if (victim->ability != ABILITY_IMMUNITY)
			{
				victim->dataisfor->lasting_status_effect = STATUS_POISON;
				printf("%s was POISONED\n", victim->dataisfor->nickname);
				return 1;
			}
			else
				printf("%s was not poisoned because of its ability IMMUNITY\n", victim->dataisfor->nickname);
		}
		else if (status == STATUS_TOXIC && victim->types[0] != TYPE_POISON && victim->types[1] != TYPE_POISON && victim->types[0] != TYPE_STEEL && victim->types[1] != TYPE_STEEL)
		{
			if (victim->ability != ABILITY_IMMUNITY)
			{
				victim->dataisfor->lasting_status_effect = STATUS_TOXIC;
				printf("%s was BADLY POISONED\n", victim->dataisfor->nickname);
				return 1;
			}
			else
				printf("%s was not badly poisoned because of its ability IMMUNITY\n", victim->dataisfor->nickname);
		}
		else if (status == STATUS_BURN && victim->types[0] != TYPE_FIRE && victim->types[1] != TYPE_FIRE)
		{
			if (victim->ability != ABILITY_WATER_VEIL)
			{
				victim->dataisfor->lasting_status_effect = STATUS_BURN;
				printf("%s was BURNED\n", victim->dataisfor->nickname);
				return 1;
			}
			else if(move->effect == EFFECT_WILL_O_WISP)
				printf("%s was not burned because of its ability WATER VEIL\n", victim->dataisfor->nickname);
		}
		else if (status == STATUS_FREEZE && victim->types[0] != TYPE_ICE && victim->types[1] != TYPE_ICE && victim->ability != ABILITY_MAGMA_ARMOR)
		{
			victim->dataisfor->lasting_status_effect = STATUS_FREEZE;
			printf("%s was FROZEN\n", victim->dataisfor->nickname);
		}
		else if (status == STATUS_SLEEP)
		{
			if (victim->ability != ABILITY_VITAL_SPIRIT)
			{
				victim->dataisfor->lasting_status_effect = STATUS_SLEEP;
				printf("%s was put to SLEEP\n", victim->dataisfor->nickname);
				return 1;
			}
			else
				printf("%s was not put to sleep because of its ability VITAL SPIRIT\n", victim->dataisfor->nickname);
		}
		else if (status == STATUS_PARALYSIS)
		{
			if (victim->ability != ABILITY_LIMBER)
			{
				victim->dataisfor->lasting_status_effect = STATUS_PARALYSIS;
				printf("%s was PARALYZED\n", victim->dataisfor->nickname);
				return 1;
			}
			else if(move->effect == EFFECT_PARALYZE)
				printf("%s was not paralyzed because of its ability LIMBER\n", victim->dataisfor->nickname);
		}

	}
	return 0;
}
void attemptcurestatus(OnField* victim)
{
	if (victim->dataisfor->lasting_status_effect != 0)
	{
		if (victim->dataisfor->held_item->effect == victim->dataisfor->lasting_status_effect || victim->dataisfor->held_item->effect == HOLD_EFFECT_CURE_STATUS)
		{
			victim->dataisfor->lasting_status_effect = NO_STATUS;
			printf("%s\'s status was cured by its %s\n", victim->dataisfor->nickname, victim->dataisfor->held_item->name);
		}
		else if (victim->ability % 128 == ABILITY_NATURAL_CURE && search(victim->temp_status_effects, victim->num_status_effects, TEMP_STATUS_SWITCHING, 1) != -1)
		{
			victim->dataisfor->lasting_status_effect = NO_STATUS;
			printf("%s\'s status was cured by its ability NATURAL CURE\n", victim->dataisfor->nickname);
		}
		else if (victim->ability == ABILITY_WATER_VEIL && victim->dataisfor->lasting_status_effect == STATUS_BURN)
		{
			victim->dataisfor->lasting_status_effect = NO_STATUS;
			printf("%s\'s burn was cured by its ability WATER VEIL\n", victim->dataisfor->nickname);
		}
		else if (victim->ability == ABILITY_IMMUNITY && victim->dataisfor->lasting_status_effect == STATUS_POISON)
		{
			victim->dataisfor->lasting_status_effect = NO_STATUS;
			printf("%s\'s poison was cured by its ability IMMUNITY\n", victim->dataisfor->nickname);
		}
		else if (victim->ability == ABILITY_IMMUNITY && victim->dataisfor->lasting_status_effect == STATUS_TOXIC)
		{
			victim->dataisfor->lasting_status_effect = NO_STATUS;
			printf("%s\'s bad poison was cured by its ability IMMUNITY\n", victim->dataisfor->nickname);
		}
		else if (victim->ability == ABILITY_VITAL_SPIRIT && victim->dataisfor->lasting_status_effect == STATUS_SLEEP)
		{
			victim->dataisfor->lasting_status_effect = NO_STATUS;
			printf("%s woke up because of its ability VITAL SPIRIT\n", victim->dataisfor->nickname);
		}
		else if (victim->ability == ABILITY_LIMBER && victim->dataisfor->lasting_status_effect == STATUS_PARALYSIS)
		{
			victim->dataisfor->lasting_status_effect = NO_STATUS;
			printf("%s\'s paralysis was cured by its ability LIMBER\n", victim->dataisfor->nickname);
		}
	}
}
uint8 checkaccuracy(Move* move, OnField* attacker, OnField* defender, const uint8 weather)
{
	if (move->effect == EFFECT_ALWAYS_HIT || (move->effect == EFFECT_THUNDER && weather == WEATHER_RAIN))
		return 1;
	uint16 accuracy = move->accuracy;
	if (move->effect == EFFECT_THUNDER && weather == WEATHER_HARSH_SUN)
		accuracy = 50;
	int8 combinedboosts = (attacker->stat_modifiers[3] % 16) - (defender->stat_modifiers[3] / 16);
	//0 = -6, 1 = -5, 2 = -4, 3 = -3, 4 = -2, 5 = -1, 6 = 0, 7 = 1, 8 = 2, 9 = 3, 10 = 4, 11 = 5, 12 = 6.
	//capped at -6 or +6. combinations: -6/0, -5/1, -4/2
	if (combinedboosts < -6)
		combinedboosts = -6;
	else if (combinedboosts > 6)
		combinedboosts = 6;

	switch (combinedboosts)
	{
		case -6:
			accuracy *= 33;
			break;
		case -5:
			accuracy *= 36;
			break;
		case -4:
			accuracy *= 43;
			break;
		case -3:
			accuracy *= 50;
			break;
		case -2:
			accuracy *= 60;
			break;
		case -1:
			accuracy *= 75;
			break;
		case 0:
			accuracy *= 100;
			break;
		case 1:
			accuracy *= 133;
			break;
		case 2:
			accuracy *= 166;
			break;
		case 3:
			accuracy *= 200;
			break;
		case 4:
			accuracy *= 250;
			break;
		case 5:
			accuracy *= 266;
			break;
		case 6:
			accuracy *= 300;
			break;
		default:
			fprintf(stderr, "Somehow, Electra fucked up her math skills and the combined stage for accuracy and evasion is not valid.");
			exit(EXIT_FAILURE);
			break;
	}
	accuracy /= 100;


	uint8 abilitymodifier = 100;
	uint8 movetype = move->type;
	if (((attacker->ability % 128 != ABILITY_CLOUD_NINE && attacker->ability % 128 != ABILITY_AIR_LOCK) || attacker->ability / 128 == 0) && ((defender->ability % 128 != ABILITY_CLOUD_NINE && defender->ability % 128 != ABILITY_AIR_LOCK) || defender->ability == 0))
		movetype = weatherballtype(weather);

	//ability modifier for attacker
	if (attacker->ability % 128 == ABILITY_COMPOUND_EYES)
		abilitymodifier = 130;
	else if (attacker->ability % 128 == ABILITY_HUSTLE && movetype < TYPE_MYSTERY)
		abilitymodifier = 80;
	accuracy = accuracy * abilitymodifier / 100;

	//sand veil
	if (defender->ability % 128 == ABILITY_SAND_VEIL && weather == WEATHER_SANDSTORM)
		accuracy = accuracy * 80 / 100;

	//item accuracy
	uint8 itemmodifier;
	if (defender->dataisfor->held_item->id == ITEM_LAX_INCENSE)
		itemmodifier = 95;
	else if (defender->dataisfor->held_item->id == ITEM_BRIGHT_POWDER)
		itemmodifier = 90;
	if (accuracy >= 100)
		return 1;
	else
		return (Random() % 100 < accuracy);
}
void prepareandusemove(Field* field, const uint8 yourselection, const uint8 oppselection) //need to check if a move is selectable before this is called
{
	//to do:
	//
	//implement move effects
	//implement remaining held item effects
	//
	//done:
	//
	//accuracy and evasion
	//deal damage
	//deduct pp
	
	Side* attackingside = (Side*)malloc(sizeof(Side));
	Side* defendingside = (Side*)malloc(sizeof(Side));
	uint8 attackingslot, defendingslot = 0;
	if (field->turn_weather_spikes / 128 == 0)
	{
		memcpy(attackingside, field->your_side->onfield, sizeof(Side));
		attackingslot = yourselection;
		memcpy(defendingside, field->opp_side->onfield, sizeof(Side));
		defendingslot = oppselection;
	}
	else
	{
		memcpy(defendingside, field->your_side->onfield, sizeof(Side));
		defendingslot = yourselection;
		memcpy(attackingside, field->opp_side->onfield, sizeof(Side));
		attackingslot = oppselection;
	}
	//deal damage

	if (affectstarget(attackingside, defendingside, attackingside->onfield->dataisfor->moveset[attackingslot]))
	{
		if (checkaccuracy(attackingside->onfield->dataisfor->moveset[attackingslot], attackingside->onfield, defendingside->onfield, field->turn_weather_spikes % 128 / 16))
			// 33/100 36/100 43/100 50/100 60/100 75/100
			usemove(field, attackingside, defendingside, attackingside->onfield->dataisfor->moveset[attackingslot], defendingside->onfield->dataisfor->moveset[defendingslot]);
		else
			printf("%s avoided the attack!\n", defendingside->onfield->dataisfor->nickname);
	}
	//deduct pp
	attackingside->onfield->dataisfor->pp[attackingslot] -= (1 + (defendingside->onfield->ability == ABILITY_PRESSURE && attackingside->onfield->dataisfor->pp[attackingslot] > 1));

}
void heal(PartyMember* toheal, uint16 healby, uint8 healtype)
{
	if (healtype == EFFECT_SOFTBOILED && toheal->hp_remaining >= toheal->max_hp)
		printf("%s is already at max HP or above!\n", toheal->nickname);
	else if (toheal->max_hp - healby < toheal->hp_remaining)
		toheal->hp_remaining = toheal->max_hp;
}
void secondaryeffect(const uint8 effect, const uint8 effectaccuracy, Side* attackingside, Side* defendingside)
{
	uint8 randomnum = 0;
	if (effectaccuracy < 100 && !(effectaccuracy >= 50 && attackingside->onfield->ability == ABILITY_SERENE_GRACE))
		randomnum = Random() % 100;
	else if (randomnum >= (uint16)(effectaccuracy * (1 + (attackingside->onfield->ability == ABILITY_SERENE_GRACE))))
		return;
	switch (effect)
	{
		case EFFECT_SLEEP:
			if (attemptstatus(defendingside->onfield, STATUS_SLEEP) && defendingside->onfield->ability == ABILITY_SYNCHRONIZE)
				attemptstatus(attackingside->onfield, STATUS_SLEEP);
			break;
		case EFFECT_POISON_HIT:
			if(attemptstatus(defendingside->onfield, STATUS_POISON) && defendingside->onfield->ability == ABILITY_SYNCHRONIZE)
				attemptstatus(attackingside->onfield, STATUS_POISON);
			break;
		case EFFECT_BURN_HIT:
			if(attemptstatus(defendingside->onfield, STATUS_BURN) && defendingside->onfield->ability == ABILITY_SYNCHRONIZE)
				attemptstatus(attackingside->onfield, STATUS_BURN);
			break;
		case EFFECT_FREEZE_HIT:
			if(attemptstatus(defendingside->onfield, STATUS_FREEZE) && defendingside->onfield->ability == ABILITY_SYNCHRONIZE)
				attemptstatus(attackingside->onfield, STATUS_FREEZE);
			break;
		case EFFECT_PARALYZE_HIT:
			if(attemptstatus(defendingside->onfield, STATUS_PARALYSIS) && defendingside->onfield->ability == ABILITY_SYNCHRONIZE)
				attemptstatus(attackingside->onfield, STATUS_PARALYSIS);
			break;
		case EFFECT_TOXIC:
			if (attemptstatus(defendingside->onfield, STATUS_TOXIC) && defendingside->onfield->ability == ABILITY_SYNCHRONIZE)
				attemptstatus(attackingside->onfield, STATUS_TOXIC);
			break;
		case EFFECT_FLINCH_HIT:
			addeffect(defendingside->onfield, TEMP_STATUS_FLINCH);
			break;

	}
}
void usemove(Field* field, Side* attackingside, Side* defendingside, Move* move, Move* defendingmove)
{
	uint16 damage = 0;
	uint16 recoil = 0;
	int8 index = -1;
	if(checkaccuracy(move, attackingside->onfield, defendingside->onfield, (field->turn_weather_spikes / 16) % 128))
	{
		switch (move->effect)
		{
			case EFFECT_HIT:
				if(affectstarget(attackingside, defendingside, move))
					damage = calculatedamage(field, attackingside, defendingside, move);
				break;
			case EFFECT_SLEEP:
				secondaryeffect(move->effect, move->effect_accuracy, attackingside, defendingside);
				break;
			case EFFECT_POISON_HIT:
				if (affectstarget(attackingside, defendingside, move))
					damage = calculatedamage(field, attackingside, defendingside, move);
				secondaryeffect(move->effect, move->effect_accuracy, attackingside, defendingside);
				break;
			case EFFECT_ABSORB:
				if (affectstarget(attackingside, defendingside, move))
				{
					damage = calculatedamage(field, attackingside, defendingside, move);
					if (defendingside->onfield->ability != ABILITY_LIQUID_OOZE)
						heal(attackingside->onfield->dataisfor, damage / 2, EFFECT_ABSORB);
					else
					{
						recoil = damage / 2;
						printf("%s was hurt by attempting to take some of %s\'s HP, who was the ability LIQUID OOZE", attackingside->onfield->dataisfor->nickname, defendingside->onfield->dataisfor->nickname);
					}
				}
				break;
			case EFFECT_BURN_HIT:
				if (affectstarget(attackingside, defendingside, move))
				{
					damage = calculatedamage(field, attackingside, defendingside, move);
					secondaryeffect(move->effect, move->effect_accuracy, attackingside, defendingside);
				}
				break;
			case EFFECT_FREEZE_HIT:
				if (affectstarget(attackingside, defendingside, move))
				{
					damage = calculatedamage(field, attackingside, defendingside, move);
					secondaryeffect(move->effect, move->effect_accuracy, attackingside, defendingside);
				}
				break;
			case EFFECT_PARALYZE_HIT:
				if (affectstarget(attackingside, defendingside, move))
				{
					damage = calculatedamage(field, attackingside, defendingside, move);
					secondaryeffect(move->effect, move->effect_accuracy, attackingside, defendingside);
				}
				break;
			case EFFECT_EXPLOSION:
				//uint16 olddef = defendingside->onfield->dataisfor->stats[2];
				//defendingside->onfield->dataisfor->stats[2] = defendingside->onfield->dataisfor->stats[2] >> 1;
				if (affectstarget(attackingside, defendingside, move))
					damage = calculatedamage(field, attackingside, defendingside, move);
				//defendingside->onfield->dataisfor->stats[2] = olddef;
				break;
			case EFFECT_DREAM_EATER:
				if (affectstarget(attackingside, defendingside, move))
				{
					if (defendingside->onfield->dataisfor->lasting_status_effect == STATUS_SLEEP)
					{
						damage = calculatedamage(field, attackingside, defendingside, move);
						heal(attackingside->onfield->dataisfor, damage / 2, EFFECT_DREAM_EATER);
					}
					else
						printf("But it failed- %s is not asleep!\n", defendingside->onfield->dataisfor->nickname);
				}
				break;
			case EFFECT_MIRROR_MOVE:
				break;
			case EFFECT_ATTACK_UP:
				attemptstatchange(attackingside->onfield->stat_modifiers[0], 1);
				break;
			case EFFECT_DEFENSE_UP:
				attemptstatchange(attackingside->onfield->stat_modifiers[1], 1);
				break;
			case EFFECT_SPEED_UP:
				attemptstatchange(attackingside->onfield->stat_modifiers[4], 1);
				break;
			case EFFECT_SPECIAL_ATTACK_UP:
				attemptstatchange(attackingside->onfield->stat_modifiers[2], 1);
				break;
			case EFFECT_SPECIAL_DEFENSE_UP:
				attemptstatchange(attackingside->onfield->stat_modifiers[3], 1);
				break;
			case EFFECT_ACCURACY_UP:
				attemptstatchange(attackingside->onfield->stat_modifiers[5], 1);
				break;
			case EFFECT_EVASION_UP:
				attemptstatchange(attackingside->onfield->stat_modifiers[6], 1);
				break;
				//case EFFECT_ALWAYS_HIT: ALREADY IMPLEMENTED
					//break;
			case EFFECT_ATTACK_DOWN:
				attemptstatchange(defendingside->onfield->stat_modifiers[0], -1);
				break;
			case EFFECT_DEFENSE_DOWN:
				attemptstatchange(defendingside->onfield->stat_modifiers[1], -1);
				break;
			case EFFECT_SPEED_DOWN:
				attemptstatchange(defendingside->onfield->stat_modifiers[4], -1);
				break;
			case EFFECT_SPECIAL_ATTACK_DOWN:
				attemptstatchange(defendingside->onfield->stat_modifiers[2], -1);
				break;
			case EFFECT_SPECIAL_DEFENSE_DOWN:
				attemptstatchange(defendingside->onfield->stat_modifiers[3], -1);
				break;
			case EFFECT_ACCURACY_DOWN:
				attemptstatchange(defendingside->onfield->stat_modifiers[5], -1);
				break;
			case EFFECT_EVASION_DOWN:
				attemptstatchange(defendingside->onfield->stat_modifiers[6], -1);
				break;
			case EFFECT_HAZE:
				for (int i = 0; i < 7; i++)
				{
					attackingside->onfield->stat_modifiers[i] = 0;
					defendingside->onfield->stat_modifiers[i] = 0;
				}
				break;
			case EFFECT_BIDE: //how the fuck do i implement this
				index = search(attackingside->onfield->temp_status_effects, attackingside->onfield->num_status_effects, TEMP_STATUS_BIDE, 1);
				if (index == -1)
					damage = attackingside->onfield->stored_damage;
				else
					attackingside->onfield->temp_status_effects[index] = NO_STATUS;
				break;
			case EFFECT_RAMPAGE:
				index = search(attackingside->onfield->temp_status_effects, attackingside->onfield->num_status_effects, TEMP_STATUS_BIDE, 1);
				damage = attackingside->onfield->stored_damage;
				if (index != -1)
					attackingside->onfield->temp_status_effects[index] = NO_STATUS;
				else
					addeffect(attackingside->onfield, TEMP_STATUS_CONFUSION);
				break;
			case EFFECT_ROAR:
				makeswitch(defendingside, NULL, RANDOM);
				break;
			case EFFECT_MULTI_HIT:
				break;
			case EFFECT_CONVERSION:
				break;
			case EFFECT_FLINCH_HIT:
				if(affectstarget(attackingside, defendingside, move))
				{
					damage = calculatedamage(field, attackingside, defendingside, move);
					secondaryeffect(EFFECT_FLINCH_HIT, move->effect_accuracy, attackingside, defendingside);
				}
				break;
			case EFFECT_RESTORE_HP:
				break;
			case EFFECT_TOXIC:
				secondaryeffect(move->effect, move->effect_accuracy, attackingside, defendingside);
				break;
			case EFFECT_PAY_DAY:
				break;
			case EFFECT_LIGHT_SCREEN:
				if (!(attackingside->fieldeffects >> 5))
					attackingside->fieldeffects += 160;
				break;
			case EFFECT_TRI_ATTACK:
				break;
			case EFFECT_REST:
				break;
			case EFFECT_OHKO:
				if (defendingside->onfield->ability != ABILITY_STURDY && affectstarget(attackingside, defendingside, move))
				{
					defendingside->onfield->dataisfor->stats[0] = 0;
					printf("It's a ONE-HIT-KO\n");
				}
				break;
			case EFFECT_RAZOR_WIND:
				break;
			case EFFECT_SUPER_FANG:
				break;
			case EFFECT_DRAGON_RAGE:
				if (affectstarget(attackingside, defendingside, move))
					damage = 40;
				break;
			case EFFECT_TRAP:
				break;
			case EFFECT_HIGH_CRITICAL:
				break;
			case EFFECT_DOUBLE_HIT:
				break;
			case EFFECT_RECOIL_IF_MISS:
				break;
			case EFFECT_MIST:
				break;
			case EFFECT_FOCUS_ENERGY:
				break;
			case EFFECT_RECOIL:
				if(affectstarget(attackingside, defendingside, move))
				{
					damage = calculatedamage(field, attackingside, defendingside, move);
					if (attackingside->onfield->ability != ABILITY_ROCK_HEAD)
					{
						recoil = damage / 4;
						printf("%s is hit with recoil!\n", attackingside->onfield->dataisfor->nickname);
					}
				}
					break;
			case EFFECT_CONFUSE:
				break;
			case EFFECT_ATTACK_UP_2:
				break;
			case EFFECT_DEFENSE_UP_2:
				break;
			case EFFECT_SPEED_UP_2:
				break;
			case EFFECT_SPECIAL_ATTACK_UP_2:
				break;
			case EFFECT_SPECIAL_DEFENSE_UP_2:
				break;
			case EFFECT_ACCURACY_UP_2:
				break;
			case EFFECT_EVASION_UP_2:
				break;
			case EFFECT_TRANSFORM:
				break;
			case EFFECT_ATTACK_DOWN_2:
				break;
			case EFFECT_DEFENSE_DOWN_2:
				break;
			case EFFECT_SPEED_DOWN_2:
				break;
			case EFFECT_SPECIAL_ATTACK_DOWN_2:
				break;
			case EFFECT_SPECIAL_DEFENSE_DOWN_2:
				break;
			case EFFECT_ACCURACY_DOWN_2:
				break;
			case EFFECT_EVASION_DOWN_2:
				break;
			case EFFECT_REFLECT:
				break;
			case EFFECT_POISON:
				break;
			case EFFECT_PARALYZE:
				break;
			case EFFECT_ATTACK_DOWN_HIT:
				break;
			case EFFECT_DEFENSE_DOWN_HIT:
				break;
			case EFFECT_SPEED_DOWN_HIT:
				break;
			case EFFECT_SPECIAL_ATTACK_DOWN_HIT:
				break;
			case EFFECT_SPECIAL_DEFENSE_DOWN_HIT:
				break;
			case EFFECT_ACCURACY_DOWN_HIT:
				break;
			case EFFECT_EVASION_DOWN_HIT:
				break;
			case EFFECT_SKY_ATTACK:
				break;
			case EFFECT_CONFUSE_HIT:
				break;
			case EFFECT_TWINEEDLE:
				break;
			case EFFECT_VITAL_THROW:
				break;
			case EFFECT_SUBSTITUTE:
				break;
			case EFFECT_RECHARGE:
				break;
			case EFFECT_RAGE:
				break;
			case EFFECT_MIMIC:
				break;
			case EFFECT_METRONOME:
				break;
			case EFFECT_LEECH_SEED:
				break;
			case EFFECT_SPLASH:
				break;
			case EFFECT_DISABLE:
				break;
			case EFFECT_LEVEL_DAMAGE:
				break;
			case EFFECT_PSYWAVE:
				break;
			case EFFECT_COUNTER:
				break;
			case EFFECT_ENCORE:
				break;
			case EFFECT_PAIN_SPLIT:
				break;
			case EFFECT_SNORE:
				break;
			case EFFECT_CONVERSION_2:
				break;
			case EFFECT_LOCK_ON:
				break;
			case EFFECT_SKETCH:
				break;
			case EFFECT_UNUSED_60:
				fprintf(stderr, "INVALID MOVE EFFECT- UNUSED 60");
				exit(EXIT_FAILURE);
				break;
			case EFFECT_SLEEP_TALK:
				break;
			case EFFECT_DESTINY_BOND:
				break;
			case EFFECT_FLAIL:
				break;
			case EFFECT_SPITE:
				break;
			case EFFECT_FALSE_SWIPE:
				break;
			case EFFECT_HEAL_BELL:
				break;
			case EFFECT_QUICK_ATTACK:
				break;
			case EFFECT_TRIPLE_KICK:
				break;
			case EFFECT_THIEF:
				break;
			case EFFECT_MEAN_LOOK:
				break;
			case EFFECT_NIGHTMARE:
				break;
			case EFFECT_MINIMIZE:
				break;
			case EFFECT_CURSE:
				break;
			case EFFECT_UNUSED_6E:
				fprintf(stderr, "INVALID MOVE EFFECT- UNUSED 6E");
				exit(EXIT_FAILURE);
				break;
			case EFFECT_PROTECT:
				break;
			case EFFECT_SPIKES:
				break;
			case EFFECT_FORESIGHT:
				break;
			case EFFECT_PERISH_SONG:
				break;
			case EFFECT_SANDSTORM:
				field->turn_weather_spikes = (field->turn_weather_spikes & 143) + WEATHER_HAIL;
				printf("A sandstorm brewed!\n");
				break;
			case EFFECT_ENDURE:
				break;
			case EFFECT_ROLLOUT:
				break;
			case EFFECT_SWAGGER:
				break;
			case EFFECT_FURY_CUTTER:
				break;
			case EFFECT_ATTRACT:
				break;
			case EFFECT_RETURN:
				break;
			case EFFECT_PRESENT:
				break;
			case EFFECT_FRUSTRATION:
				break;
			case EFFECT_SAFEGUARD:
				break;
			case EFFECT_THAW_HIT:
				break;
			case EFFECT_MAGNITUDE:
				break;
			case EFFECT_BATON_PASS:
				break;
			case EFFECT_PURSUIT:
				break;
			case EFFECT_RAPID_SPIN:
				break;
			case EFFECT_SONICBOOM:
				break;
			case EFFECT_UNUSED_83:
				fprintf(stderr, "INVALID MOVE EFFECT- UNUSED 83");
				exit(EXIT_FAILURE);
				break;
			case EFFECT_MORNING_SUN:
				break;
			case EFFECT_SYNTHESIS:
				break;
			case EFFECT_MOONLIGHT:
				break;
			case EFFECT_HIDDEN_POWER:
				break;
			case EFFECT_RAIN_DANCE:
				field->turn_weather_spikes = (field->turn_weather_spikes & 143) + WEATHER_RAIN;
				printf("It started to rain!\n");
				break;
			case EFFECT_SUNNY_DAY:
				field->turn_weather_spikes = (field->turn_weather_spikes & 143) + WEATHER_HARSH_SUN;
				printf("The sunlight got bright!\n");
				break;
			case EFFECT_DEFENSE_UP_HIT:
				break;
			case EFFECT_ATTACK_UP_HIT:
				break;
			case EFFECT_ALL_STATS_UP_HIT:
				break;
			case EFFECT_UNUSED_8D:
				fprintf(stderr, "INVALID MOVE EFFECT- UNUSED 8D");
				exit(EXIT_FAILURE);
				break;
			case EFFECT_BELLY_DRUM:
				break;
			case EFFECT_PSYCH_UP:
				break;
			case EFFECT_MIRROR_COAT:
				break;
			case EFFECT_SKULL_BASH:
				break;
			case EFFECT_TWISTER:
				break;
			case EFFECT_EARTHQUAKE:
				break;
			case EFFECT_FUTURE_SIGHT:
				break;
			case EFFECT_GUST:
				break;
			case EFFECT_FLINCH_HIT_2:
				break;
			case EFFECT_SOLARBEAM:
				index = search(attackingside->onfield->temp_status_effects, attackingside->onfield->num_status_effects, TEMP_STATUS_CHARGING_TURN, 1);
				if(index != -1)
				{
					removeeffect(attackingside->onfield, index);
					if (affectstarget(attackingside, defendingside, move))
					{
						damage = calculatedamage(field, attackingside, defendingside, move);
						if ((field->turn_weather_spikes & 127) != WEATHER_NONE && (field->turn_weather_spikes & 127) != WEATHER_HARSH_SUN)
							damage /= 2;
					}
				}
				else if((field->turn_weather_spikes & 127) == WEATHER_HARSH_SUN)
				{
					if (affectstarget(attackingside, defendingside, move))
						damage = calculatedamage(field, attackingside, defendingside, move);
				}
				else
				{
					printf("%s began taking in sunlight!\n", attackingside->onfield->dataisfor->nickname);
					addeffect(attackingside->onfield, TEMP_STATUS_CHARGING_TURN);
				}
				break;
			case EFFECT_THUNDER:
				if (affectstarget(attackingside, defendingside, move))
					damage = calculatedamage(field, attackingside, defendingside, move);
				break;
			case EFFECT_TELEPORT:
				printf("But it failed!\n");
				break;
			case EFFECT_BEAT_UP:
				break;
			case EFFECT_FLY:
				break;
			case EFFECT_DEFENSE_CURL:
				break;
			case EFFECT_SOFTBOILED:
				break;
			case EFFECT_FAKE_OUT:
				break;
			case EFFECT_UPROAR:
				break;
			case EFFECT_STOCKPILE:
				break;
			case EFFECT_SPIT_UP:
				break;
			case EFFECT_SWALLOW:
				break;
			case EFFECT_UNUSED_A3:
				fprintf(stderr, "INVALID MOVE EFFECT- UNUSED A3");
				exit(EXIT_FAILURE);
				break;
			case EFFECT_HAIL:
				field->turn_weather_spikes = (field->turn_weather_spikes & 143) + WEATHER_HAIL;
				printf("It started to hail!\n");
				break;
			case EFFECT_TORMENT:
				break;
			case EFFECT_FLATTER:
				break;
			case EFFECT_WILL_O_WISP:
				break;
			case EFFECT_MEMENTO:
				break;
			case EFFECT_FACADE:
				break;
			case EFFECT_FOCUS_PUNCH:
				break;
			case EFFECT_SMELLINGSALT:
				break;
			case EFFECT_FOLLOW_ME:
				break;
			case EFFECT_NATURE_POWER:
				break;
			case EFFECT_CHARGE:
				break;
			case EFFECT_TAUNT:
				break;
			case EFFECT_HELPING_HAND:
				break;
			case EFFECT_TRICK:
				break;
			case EFFECT_ROLE_PLAY:
				break;
			case EFFECT_WISH:
				break;
			case EFFECT_ASSIST:
				break;
			case EFFECT_INGRAIN:
				break;
			case EFFECT_SUPERPOWER:
				break;
			case EFFECT_MAGIC_COAT:
				break;
			case EFFECT_RECYCLE:
				break;
			case EFFECT_REVENGE:
				break;
			case EFFECT_BRICK_BREAK:
				break;
			case EFFECT_YAWN:
				break;
			case EFFECT_KNOCK_OFF:
				break;
			case EFFECT_ENDEAVOR:
				if (affectstarget(attackingside, defendingside, move))
				{
					if (attackingside->onfield->dataisfor->stats[0] < defendingside->onfield->dataisfor->stats[0])
						memcpy(&defendingside->onfield->dataisfor->stats[0], (void*)&attackingside->onfield->dataisfor->stats[0], sizeof(uint16));
					else
						printf("But it failed!\n");
				}
				break;
			case EFFECT_ERUPTION:
				break;
			case EFFECT_SKILL_SWAP:
				break;
			case EFFECT_IMPRISON:
				break;
			case EFFECT_REFRESH:
				break;
			case EFFECT_GRUDGE:
				break;
			case EFFECT_SNATCH:
				break;
			case EFFECT_LOW_KICK:
				break;
			case EFFECT_SECRET_POWER:
				break;
			case EFFECT_DOUBLE_EDGE:
				if (affectstarget(attackingside, defendingside, move))
				{
					damage = calculatedamage(field, attackingside, defendingside, move);
					if (attackingside->onfield->ability != ABILITY_ROCK_HEAD)
					{
						recoil = damage / 3;
						printf("%s is hit with recoil!\n", attackingside->onfield->dataisfor->nickname);
					}
				}
				break;
			case EFFECT_TEETER_DANCE:
				break;
			case EFFECT_BLAZE_KICK:
				break;
			case EFFECT_MUD_SPORT:
				break;
			case EFFECT_POISON_FANG:
				break;
			case EFFECT_WEATHER_BALL:
				if (affectstarget(attackingside, defendingside, move))
					damage = calculatedamage(field, attackingside, defendingside, move);
				break;
			case EFFECT_OVERHEAT:
				break;
			case EFFECT_TICKLE:
				break;
			case EFFECT_COSMIC_POWER:
				break;
			case EFFECT_SKY_UPPERCUT:
				break;
			case EFFECT_BULK_UP:
				break;
			case EFFECT_POISON_TAIL:
				break;
			case EFFECT_WATER_SPORT:
				break;
			case EFFECT_CALM_MIND:
				break;
			case EFFECT_DRAGON_DANCE:
				break;
			case EFFECT_CAMOUFLAGE:
				break;
			default:
				fprintf(stderr, "ERROR- invalid move effect. Glitch move effects are not allowed, nor are they implemented!");
				exit(EXIT_FAILURE);
				break;
		}
		if (damage != 0)
			if (defendingside->onfield->dataisfor->hp_remaining > defendingside->onfield->dataisfor->max_hp)
				memcpy(&defendingside->onfield->dataisfor->hp_remaining, (void*)&defendingside->onfield->dataisfor->max_hp, sizeof(uint16));
	}
	else
		printf("But it missed!\n");
}
void attemptstatchange(uint8 modifier, const int8 change)
{
	//attemptstatchange(defendingside->onfield->stat_modifiers[1], LOWER, -1);
	if (abs(modifier + change) > 6)
		return;
	modifier += change;
}