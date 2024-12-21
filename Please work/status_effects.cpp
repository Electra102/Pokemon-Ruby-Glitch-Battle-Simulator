#include "main.h"
#include "damage.h"
#include "status_effects_implementation.h"
void addeffect(OnField* onfield, const uint8 temp_status)
{
	int8 index = search(onfield->temp_status_effects, onfield->num_status_effects, NO_STATUS, 1);
	if (index == -1)
	{
		onfield->temp_status_effects = (uint8*)realloc(onfield->temp_status_effects, sizeof(uint8) * (onfield->num_status_effects + 8));
		index = onfield->num_status_effects;
	}
	onfield->num_status_effects++;
	onfield->temp_status_effects[index] = temp_status;
}
void removeeffect(OnField* onfield, const uint8 index)
{
	onfield->temp_status_effects[index] = 0;
	onfield->num_status_effects--;
}
int8 search(const uint8* list, const uint8 listsize, const uint8 searchfor, const uint8 start)
{
	if (start >= listsize)
		return -1;
	for (int i = start; i < listsize; i++)
		if (list[i] == searchfor)
			return i;
	return -1;
}