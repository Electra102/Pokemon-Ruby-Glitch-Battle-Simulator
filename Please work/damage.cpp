#include "damage.h"
#include "move_categories.h"
#include "default_moves.h"
#include "status_effects_implementation.h"
#include "pokemon_species.h"
#include "move_functionality.h"
//times I needed to fix this because I messed with how another file works: 8 (at least 4 fixes/changes for other reasons)
uint8 typeabilityboost(OnField* onfield, const uint8 type)
{
    if (onfield->dataisfor->stats[0] * 3 <= onfield->dataisfor->max_hp)
    {
        if (type == TYPE_GRASS && onfield->ability == ABILITY_OVERGROW)
            return 15;
        else if (type == TYPE_FIRE && onfield->ability == ABILITY_BLAZE)
            return 15;
        else if (type == TYPE_WATER && onfield->ability == ABILITY_TORRENT)
            return 15;
        else if (type == TYPE_BUG && onfield->ability == ABILITY_SWARM)
            return 15;
    }
    return 10;
}
uint8 abilitystatboost(OnField* onfield, const uint8 statid)
{
    switch (onfield->ability)
    {
        case ABILITY_HUSTLE:
            if (statid == 1)
                return 15;
            break;
        case ABILITY_GUTS:
            if (statid == 1 && (onfield->dataisfor->lasting_status_effect == STATUS_BURN || onfield->dataisfor->lasting_status_effect == STATUS_PARALYSIS || onfield->dataisfor->lasting_status_effect == STATUS_POISON || onfield->dataisfor->lasting_status_effect == STATUS_TOXIC))
                return 15;
            break;
        case ABILITY_HUGE_POWER:
            if (statid == 1)
                return 20;
            break;
        case ABILITY_PURE_POWER:
            if (statid == 1)
                return 20;
            break;
        case ABILITY_MARVEL_SCALE:
            if(statid == 2 && (onfield->dataisfor->lasting_status_effect > 1 && (onfield->dataisfor->lasting_status_effect == STATUS_TOXIC || onfield->dataisfor->lasting_status_effect < 7)))
                return 15;
            break;
        default:
            return 10;
            break;
    }
}
uint8 itemstatboost(OnField* onfield, const uint8 statid)
{
    switch (onfield->dataisfor->held_item->effect)
    {
        case HOLD_EFFECT_THICK_CLUB:
            if (statid == 1 && (onfield->dataisfor->pokemondata->species == SPECIES_CUBONE || onfield->dataisfor->pokemondata->species == SPECIES_MAROWAK))
                return 20;
            break;
        case HOLD_EFFECT_DEEP_SEA_TOOTH:
            if (statid == 3 && onfield->dataisfor->pokemondata->species == SPECIES_CLAMPERL)
                return 20;
        break;
        case HOLD_EFFECT_DEEP_SEA_SCALE:
            if (statid == 4 && onfield->dataisfor->pokemondata->species == SPECIES_CLAMPERL)
                return 20;
            break;
        case HOLD_EFFECT_CHOICE_BAND:
            if (statid == 1)
                return 15;
            break;
        case HOLD_EFFECT_LIGHT_BALL:
            if ((statid == 1 || statid == 3) && onfield->dataisfor->pokemondata->species == SPECIES_PIKACHU)
                return 20;
            break;
        case HOLD_EFFECT_SOUL_DEW:
            if ((onfield->dataisfor->pokemondata->species == SPECIES_LATIOS || onfield->dataisfor->pokemondata->species == SPECIES_LATIAS) && (statid == 3 || statid == 4))
                return 15;
            break;
        case HOLD_EFFECT_METAL_POWDER:
            if (onfield->dataisfor->pokemondata->species == SPECIES_DITTO && statid == 2 && search(onfield->temp_status_effects, onfield->num_status_effects, TEMP_STATUS_TRANSFORMED, 1) != -1)
                return 20;
            break;
        default:
            return 10;
            break;
    }
}
uint8 typeitemboost(const uint8 movetype, const uint8 effect)
{
    uint8 type2;
    switch (effect)
    {
        case HOLD_EFFECT_NORMAL_POWER:
            type2 = TYPE_NORMAL;
            break;
        case HOLD_EFFECT_FIGHTING_POWER:
            type2 = TYPE_FIGHTING;
            break;
        case HOLD_EFFECT_FLYING_POWER:
            type2 = TYPE_FLYING;
            break;
        case HOLD_EFFECT_POISON_POWER:
            type2 = TYPE_POISON;
            break;
        case HOLD_EFFECT_GROUND_POWER:
            type2 = TYPE_GROUND;
            break;
        case HOLD_EFFECT_ROCK_POWER:
            type2 = TYPE_ROCK;
            break;
        case HOLD_EFFECT_BUG_POWER:
            type2 = TYPE_BUG;
            break;
        case HOLD_EFFECT_GHOST_POWER:
            type2 = TYPE_GHOST;
            break;
        case HOLD_EFFECT_STEEL_POWER:
            type2 = TYPE_STEEL;
            break;
        case HOLD_EFFECT_FIRE_POWER:
            type2 = TYPE_FIRE;
            break;
        case HOLD_EFFECT_WATER_POWER:
            type2 = TYPE_WATER;
            break;
        case HOLD_EFFECT_GRASS_POWER:
            type2 = TYPE_GRASS;
            break;
        case HOLD_EFFECT_ELECTRIC_POWER:
           type2 = TYPE_ELECTRIC;
           break;
        case HOLD_EFFECT_PSYCHIC_POWER:
            type2 = TYPE_PSYCHIC;
            break;
        case HOLD_EFFECT_ICE_POWER:
            type2 = TYPE_ICE;
            break;
        case HOLD_EFFECT_DRAGON_POWER:
            type2 = TYPE_DRAGON;
            break;
        case HOLD_EFFECT_DARK_POWER:
            type2 = TYPE_DARK;
            break;
        default:
            return 100;
            break;
    }
    if(type2 == movetype)
        return 110;
    return 100;
}
uint8 typeeffectiveness(const uint8 attackingtype, const uint8 defendingtype)
{
    switch (attackingtype)
    {
        case TYPE_NORMAL:
            switch(defendingtype)
            {
                case TYPE_STEEL:
                    return 1;
                    break;
                case TYPE_GHOST:
                    return 0;
                    break;
                case TYPE_ROCK:
                    return 1;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_FIGHTING:
            switch (defendingtype)
            {
                case TYPE_NORMAL:
                    return 4;
                    break;
                case TYPE_ROCK:
                    return 4;
                    break;
                case TYPE_ICE:
                    return 4;
                    break;
                case TYPE_STEEL:
                    return 4;
                    break;
                case TYPE_DARK:
                    return 4;
                    break;
                case TYPE_FLYING:
                    return 1;
                    break;
                case TYPE_PSYCHIC:
                    return 1;
                    break;
                case TYPE_BUG:
                    return 1;
                    break;
                case TYPE_POISON:
                    return 1;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_FLYING:
            switch (defendingtype)
            {
                case TYPE_FIGHTING:
                    return 4;
                    break;
                case TYPE_BUG:
                    return 4;
                    break;
                case TYPE_GRASS:
                    return 4;
                    break;
                case TYPE_ROCK:
                    return 1;
                    break;
                case TYPE_STEEL:
                    return 1;
                    break;
                case TYPE_ELECTRIC:
                    return 1;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_POISON:
            switch (defendingtype)
            {
                case TYPE_GRASS:
                    return 4;
                    break;
                case TYPE_POISON:
                    return 1;
                    break;
                case TYPE_GROUND:
                    return 1;
                    break;
                case TYPE_ROCK:
                    return 1;
                    break;
                case TYPE_GHOST:
                    return 1;
                    break;
                case TYPE_STEEL:
                    return 0;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_GROUND:
            switch (defendingtype)
            {
                case TYPE_POISON:
                    return 4;
                    break;
                case TYPE_ROCK:
                    return 4;
                    break;
                case TYPE_STEEL:
                    return 4;
                    break;
                case TYPE_FIRE:
                    return 4;
                    break;
                case TYPE_ELECTRIC:
                    return 4;
                    break;
                case TYPE_BUG:
                    return 1;
                    break;
                case TYPE_GRASS:
                    return 1;
                    break;
                case TYPE_FLYING:
                    return 0;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_ROCK:
            switch (defendingtype)
            {
                case TYPE_FLYING:
                    return 4;
                    break;
                case TYPE_BUG:
                    return 4;
                    break;
                case TYPE_FIRE:
                    return 4;
                    break;
                case TYPE_ICE:
                    return 4;
                    break;
                case TYPE_FIGHTING:
                    return 1;
                    break;
                case TYPE_GROUND:
                    return 1;
                    break;
                case TYPE_STEEL:
                    return 1;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_BUG:
            switch (defendingtype)
            {
                case TYPE_GRASS:
                    return 4;
                    break;
                case TYPE_PSYCHIC:
                    return 4;
                    break;
                case TYPE_DARK:
                    return 4;
                    break;
                case TYPE_FIGHTING:
                    return 1;
                    break;
                case TYPE_FLYING:
                    return 1;
                    break;
                case TYPE_POISON:
                    return 1;
                    break;
                case TYPE_GHOST:
                    return 1;
                    break;
                case TYPE_STEEL:
                    return 1;
                    break;
                case TYPE_FIRE:
                    return 1;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_GHOST:
            switch (defendingtype)
            {
                case TYPE_GHOST:
                    return 4;
                    break;
                case TYPE_PSYCHIC:
                    return 4;
                    break;
                case TYPE_STEEL:
                    return 1;
                    break;
                case TYPE_DARK:
                    return 1;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_STEEL:
            switch (defendingtype)
            {
                case TYPE_ROCK:
                    return 4;
                    break;
                case TYPE_ICE:
                    return 4;
                    break;
                case TYPE_STEEL:
                    return 1;
                    break;
                case TYPE_FIRE:
                    return 1;
                    break;
                case TYPE_WATER:
                    return 1;
                    break;
                case TYPE_ELECTRIC:
                    return 1;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_FIRE:
            switch (defendingtype)
            {
                case TYPE_BUG:
                    return 4;
                    break;
                case TYPE_STEEL:
                    return 4;
                    break;
                case TYPE_GRASS:
                    return 4;
                    break;
                case TYPE_ICE:
                    return 4;
                    break;
                case TYPE_ROCK:
                    return 1;
                    break;
                case TYPE_FIRE:
                    return 1;
                    break;
                case TYPE_WATER:
                    return 1;
                    break;
                case TYPE_DRAGON:
                    return 1;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_WATER:
            switch(defendingtype)
            {
                case TYPE_GROUND:
                    return 4;
                    break;
                case TYPE_ROCK:
                    return 4;
                    break;
                case TYPE_FIRE:
                    return 4;
                    break;
                case TYPE_WATER:
                    return 1;
                    break;
                case TYPE_GRASS:
                    return 1;
                    break;
                case TYPE_DRAGON:
                    return 1;
                    break;
            }
            break;
        case TYPE_GRASS:
            switch (defendingtype)
            {
                case TYPE_GROUND:
                    return 4;
                    break;
                case TYPE_ROCK:
                    return 4;
                    break;
                case TYPE_WATER:
                    return 4;
                    break;
                case TYPE_FLYING:
                    return 1;
                    break;
                case TYPE_POISON:
                    return 1;
                    break;
                case TYPE_BUG:
                    return 1;
                    break;
                case TYPE_STEEL:
                    return 1;
                    break;
                case TYPE_FIRE:
                    return 1;
                    break;
                case TYPE_GRASS:
                    return 1;
                    break;
                case TYPE_DRAGON:
                    return 1;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_ELECTRIC:
            switch (defendingtype)
            {
                case TYPE_FLYING:
                    return 4;
                    break;
                case TYPE_WATER:
                    return 4;
                    break;
                case TYPE_GRASS:
                    return 1;
                    break;
                case TYPE_ELECTRIC:
                    return 1;
                    break;
                case TYPE_DRAGON:
                    return 1;
                    break;
                case TYPE_GROUND:
                    return 0;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_PSYCHIC:
            switch (defendingtype)
            {
                case TYPE_FIGHTING:
                    return 4;
                    break;
                case TYPE_POISON:
                    return 4;
                    break;
                case TYPE_STEEL:
                    return 1;
                    break;
                case TYPE_PSYCHIC:
                    return 1;
                    break;
                case TYPE_DARK:
                    return 0;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_ICE:
            switch (defendingtype)
            {
                case TYPE_FLYING:
                    return 4;
                    break;
                case TYPE_GROUND:
                    return 4;
                    break;
                case TYPE_GRASS:
                    return 4;
                    break;
                case TYPE_DRAGON:
                    return 4;
                    break;
                case TYPE_STEEL:
                    return 1;
                    break;
                case TYPE_FIRE:
                    return 1;
                    break;
                case TYPE_WATER:
                    return 1;
                    break;
                case TYPE_ICE:
                    return 1;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_DRAGON:
            switch (defendingtype)
            {
                case TYPE_DRAGON:
                    return 4;
                    break;
                case TYPE_STEEL:
                    return 1;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        case TYPE_DARK:
            switch (defendingtype)
            {
                case TYPE_GHOST:
                    return 4;
                    break;
                case TYPE_PSYCHIC:
                    return 4;
                    break;
                case TYPE_FIGHTING:
                    return 1;
                    break;
                case TYPE_STEEL:
                    return 1;
                    break;
                case TYPE_DARK:
                    return 1;
                    break;
                default:
                    return 2;
                    break;
            }
            break;
        default:
            return 2;
            break;
    }
    return NULL;
}
#define SEARCH_START 1 //used when searching temp_status_effects
//#define SEARCH_START_2 0 //used when searching field_effects
uint16 calculatedamage(Field* field, Side* attackingside, Side* defendingside, Move* move)
{
    uint8 type = move->type;
    uint8 weatherpasses = (attackingside->onfield->ability != ABILITY_CLOUD_NINE && attackingside->onfield->ability != ABILITY_AIR_LOCK  && defendingside->onfield->ability != ABILITY_CLOUD_NINE && defendingside->onfield->ability != ABILITY_AIR_LOCK);
    if (weatherpasses)
        type = weatherballtype(field->turn_weather_spikes);
    uint8 category;
    if (type < TYPE_MYSTERY)
        category = PHYSICAL;
    else if (type > TYPE_MYSTERY)
        category = SPECIAL;
    else
        return 0;

    uint16 damage = ((2 * (attackingside->onfield->dataisfor->abilityslot_level & 127) / 5) + 2) * move->base_power / 10 * typeitemboost(type, attackingside->onfield->dataisfor->held_item->effect) / 10 * typeabilityboost(attackingside->onfield, type) / 10;
    //behold, the longest line of code i have in this project so far
    //accounts for: stat modifiers, stat boosts from ability and items, attacking and defending stats, dividing by 50 at the end, and explosion halving the target's defense
    damage = (damage * (attackingside->onfield->dataisfor->stats[(2 * category) + 1] * findmodifier(FIRST, attackingside->onfield->stat_modifiers[2 * category], NORMAL_STATS) / findmodifier(SECOND, attackingside->onfield->stat_modifiers[2 * category], NORMAL_STATS) * abilitystatboost(attackingside->onfield, (2 * category) + 1) * statitemboost(attackingside->onfield, (2 * category) + 1) / 100 / (defendingside->onfield->dataisfor->stats[(2 * category) + 2] * findmodifier(FIRST, defendingside->onfield->stat_modifiers[(2 * category) + 1], NORMAL_STATS) / findmodifier(SECOND, defendingside->onfield->stat_modifiers[(2 * category) + 1], NORMAL_STATS) * abilitystatboost(attackingside->onfield, (2 * category) + 1) * statitemboost(defendingside->onfield, (2 * category) + 1) / 100 / (2 * (move->effect == EFFECT_EXPLOSION))))) / 50;
    if (category == PHYSICAL && attackingside->onfield->dataisfor->lasting_status_effect == STATUS_BURN && move->effect != EFFECT_FACADE && attackingside->onfield->ability != ABILITY_GUTS)
        damage /= 2;
    //need to find if the attacker crits now, because crits ignore screens
    uint8 critrate = attackingside->onfield->stat_modifiers[7];
    uint8 critresult = 0;
    if ((attackingside->onfield->dataisfor->pokemondata->species == 113 && attackingside->onfield->dataisfor->held_item->effect == HOLD_EFFECT_LUCKY_PUNCH) || (attackingside->onfield->dataisfor->pokemondata->species == 83 && attackingside->onfield->dataisfor->held_item->effect == HOLD_EFFECT_STICK))
        critrate += 2;
    else if (attackingside->onfield->dataisfor->held_item->effect == HOLD_EFFECT_SCOPE_LENS)
        critrate++;
    if (move->effect == EFFECT_HIGH_CRITICAL)
        critrate++;
    if (move->effect != EFFECT_FUTURE_SIGHT && move->effect != EFFECT_SPIT_UP && defendingside->onfield->ability != ABILITY_SHELL_ARMOR && defendingside->onfield->ability != ABILITY_BATTLE_ARMOR)
        critresult *= (1 + (0 == Random() % (16 - ((8 * (critrate >= 1)) + (4 * (critrate >= 2)) + (critrate >= 3) + (critrate >= 4)))));
    //need to confirm brick break deals damage after removing the screen
    if (critresult == 1 && !(move->effect == EFFECT_BRICK_BREAK) && (((defendingside->fieldeffects & 224) != 0 && category == SPECIAL) || ((defendingside->fieldeffects & 28) != 0 && category == PHYSICAL)))
        damage /= 2;
    if (weatherpasses)
    {
        if ((type == TYPE_FIRE && (field->turn_weather_spikes & 112) == WEATHER_HARSH_SUN) || (type == TYPE_WATER && (field->turn_weather_spikes & 112) == WEATHER_RAIN))
            damage = damage * 15 / 10;
        else if ((type == TYPE_WATER && (field->turn_weather_spikes & 112) == WEATHER_HARSH_SUN) || (type == TYPE_FIRE && (field->turn_weather_spikes & 112) == WEATHER_RAIN))
            damage /= 2;
    }
    if (attackingside->onfield->ability == ACTIVE_ABILITY_FLASH_FIRE && move->type == TYPE_FIRE)
        damage = damage * 15 / 10;
    damage += 2;
    if (move->effect == EFFECT_SPIT_UP && attackingside->onfield->temp_status_effects[0])
        damage *= attackingside->onfield->temp_status_effects[0];
    //apply the critical hit, if present
    damage *= critresult;
    //now for DoubleDmg
    if ((move->effect == EFFECT_GUST || move->effect == EFFECT_TWISTER) && (search(defendingside->onfield->temp_status_effects, defendingside->onfield->num_status_effects, TEMP_STATUS_FLY, SEARCH_START) + search(defendingside->onfield->temp_status_effects, defendingside->onfield->num_status_effects, TEMP_STATUS_BOUNCE, SEARCH_START) != -2))
        damage *= 2;
    if (search(defendingside->onfield->temp_status_effects, defendingside->onfield->num_status_effects, TEMP_STATUS_MINIMIZE, 1) != -1 && move->effect == EFFECT_FLINCH_HIT_2)
        damage *= 2;
    if (search(defendingside->onfield->temp_status_effects, defendingside->onfield->num_status_effects, TEMP_STATUS_DIG, 1) != -1 && (move->effect == EFFECT_EARTHQUAKE || move->effect == EFFECT_MAGNITUDE))
        damage *= 2;
    if (search(defendingside->onfield->temp_status_effects, defendingside->onfield->num_status_effects, TEMP_STATUS_DIVE, 1) != -1 && (move->index == MOVE_SURF || move->index == MOVE_WHIRLPOOL))
        damage *= 2;
    if (defendingside->onfield->dataisfor->lasting_status_effect == STATUS_PARALYSIS && move->effect == EFFECT_SMELLINGSALT)
    {
        damage *= 2;
        defendingside->onfield->dataisfor->lasting_status_effect == NO_STATUS;
    }
    if (search(defendingside->onfield->temp_status_effects, defendingside->onfield->num_status_effects, TEMP_STATUS_SWITCHING, 1) != -1 && move->effect == EFFECT_PURSUIT)
        damage *= 2;
    if (move->effect == EFFECT_FACADE && (attackingside->onfield->dataisfor->lasting_status_effect == STATUS_BURN || attackingside->onfield->dataisfor->lasting_status_effect == STATUS_PARALYSIS || attackingside->onfield->dataisfor->lasting_status_effect == STATUS_POISON || attackingside->onfield->dataisfor->lasting_status_effect == STATUS_TOXIC))
        damage *= 2;
    if (search(attackingside->onfield->temp_status_effects, attackingside->onfield->num_status_effects, TEMP_STATUS_DAMAGED_THIS_TURN, 1) != -1 && move->effect == EFFECT_REVENGE)
        damage *= 2;
    if (weatherpasses && move->effect == EFFECT_WEATHER_BALL && (field->turn_weather_spikes % 128 / 16 > 0 && field->turn_weather_spikes % 128 / 16 < 5))
        damage *= 2;
    if (field->turn_weather_spikes % 128 / 16 > 5)
    {
        fprintf(stderr, "Invalid weather value is stored");
        exit(EXIT_FAILURE);
    }
    //finished DoubleDmg
    if (type == TYPE_ELECTRIC && search(attackingside->onfield->temp_status_effects, attackingside->onfield->num_status_effects, TEMP_STATUS_CHARGED, SEARCH_START) != -1)
        damage *= 2;
    if (type == attackingside->onfield->types[0] || type == attackingside->onfield->types[1])
        damage = (15 * damage) / 10;
    damage = damage * typeeffectiveness(type, defendingside->onfield->types[0]) * typeeffectiveness(type, defendingside->onfield->types[1]) / 4;
    damage = damage * (100 - (Random() % 16)) / 100;

    return damage;
}
uint8 weatherballtype(const uint8 weather)
{
    switch (weather % 128 / 16)
    {
        case 1:
            return TYPE_FIRE;
            break;
        case 2:
            return TYPE_WATER;
            break;
        case 3:
            return TYPE_ROCK;
            break;
        case 4:
            return TYPE_ICE;
            break;
        default:
            fprintf(stderr, "weather stored was valid but looking up the weather failed."); //add variables
            exit(EXIT_FAILURE);
           break;
    }
    return 0;
}
int8 findmodifier(const uint8 part, const uint8 modifiers, const uint8 typeofstats)
{
    //could maybe fit this in a single line and not have a function, but this is cleaner
    if ((part && modifiers >= 0) || (!part && modifiers <= 0)) //in other words, return 2 if either looking for the second part and modifier is 0 or above, OR if looking for the first part and modifier is 0 or below.
        return 2;
    else
        return 8 - modifiers;
    
}