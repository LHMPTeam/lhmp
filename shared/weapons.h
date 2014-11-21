//#ifndef SHARED_TOOLS
//#define SHARED_TOOLS
#pragma once

struct DefinedWeapon
{
	bool	hasAmmo;			// e.g. granades doesn't have ammo, they are using itself
	bool	isMultiple;			// e.g. you can have more than one granade 
	int		magazine;			// max. loaded ammo 
};

//DefinedWeapon DefWep[33];

/*void Ahoj()
{
	DefWep[0].
}*/
