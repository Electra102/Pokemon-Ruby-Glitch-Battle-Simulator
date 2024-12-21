#ifndef MOVE_FUNCTIONALITY
#define MOVE_FUNCTIONALITY

#include "main.h"

uint8 affectstarget(Side* attackingside, Side* defendingside, Move* move);
uint8 attemptstatus(OnField* victim, const uint8 status);
void attemptcurestatus(OnField* victim);
uint8 checkaccuracy(Move* move, OnField* attacker, OnField* defender, const uint8 weather);
void prepareandusemove(Field* field, const uint8 yourselection, const uint8 oppselection);
void heal(PartyMember* toheal, uint16 healby, uint8 healtype);
void secondaryeffect(const uint8 effect, const uint8 effectaccuracy, Side* attackingside, Side* defendingside);
void usemove(Field* field, Side* attackingside, Side* defendingside, Move* move, Move* defendingmove);
void attemptstatchange(uint8 modifiers, const int8 change);

#endif