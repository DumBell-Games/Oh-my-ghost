#pragma once
#include "GuiControl.h"
#include "List.h"
class GuiGroup : public GuiControl
{
public:

	GuiGroup();

	~GuiGroup();


public:

	List<GuiControl*> children;
};

