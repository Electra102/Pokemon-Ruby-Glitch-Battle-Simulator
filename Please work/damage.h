#ifndef DAMAGE_H
#define DAMAGE_H

#include "main.h"

#define NORMAL_STATS 0
#define ACCURACY_AND_EVASION 1
#define SPEED_AND_CRITRATE 2

#define FIRST 0
#define SECOND 1

uint8 typeabilityboost(OnField* onfield, const uint8 type);
uint8 statitemboost(OnField* onfield, const uint8 statid);
uint8 typeitemboost(const uint8 movetype, const uint8 effect);
uint16 calculatedamage(Field* field, Side* attackingside, Side* defendingside, Move* move);
uint8 typeeffectiveness(const uint8 attackingtype, const uint8 defendingtype);
uint8 weatherballtype(const uint8 weather);
int8 findmodifier(const uint8 part, const uint8 modifiers, const uint8 typeofstats);

#endif