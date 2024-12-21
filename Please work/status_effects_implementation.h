#ifndef STATUS_EFFECTS_IMPLEMENTATION_H
#define STATUS_EFFECTS_IMPLEMENTATION_H

void addeffect(OnField* onfield, const uint8 temp_status);
void removeeffect(OnField* onfield, const uint8 index);
int8 search(const uint8* list, const uint8 listsize, const uint8 searchfor, const uint8 start);

#endif