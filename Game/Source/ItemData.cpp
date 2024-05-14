#include "ItemData.h"
#include "Enemic.h"

ItemData::ItemData(SString _name) : name(_name)
{
}

void ItemData::UseOn(Personatge* p)
{
	p->salutActual += healPow;
	if (p->salutActual > p->salutTotal) p->salutActual = p->salutTotal;
}
