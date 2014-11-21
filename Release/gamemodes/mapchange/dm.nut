function  onServerInit()
{
	print("Starting DeathMatch...");
	serverSetGamemodeName("DeathMatch");
	serverSetDefaultMap("MISE10-LETISTE");
	local result = testfunc();
	/*for (local i = 0; i < result.len(); i++) {
    		print(result[i] + "\n");
	}*/
	print(result);
}

function onPlayerSpawn(ID)
{
	sendAllMessage("Player "+playerGetName(ID)+"["+ID+"] has been respawned.");
	
	//playerChangeSkin(ID,peds[rand()%peds.len()]);

	////pozicovanie hraca
	//local rnd = random(0,spawnpos.len());
	//playerSetPosition(ID,spawnpos[rnd][0].tofloat(),spawnpos[rnd][1].tofloat(),spawnpos[rnd][2].tofloat());
	playerSetPosition(ID,103.0389,0.6649229,-346.4018);
	playerAddWeapon(ID,10,100,100);
}

function onPlayerCommand(ID,message,params)
{
	switch(message) // plrsw
	{
		case "addcar":
			local pos = playerGetPosition(ID);
			if (params.tointeger() > 169)
			{
				sendPlayerMessage(ID,red + "Wrong ID!");
			} else {
				vehicleSpawn(params.tointeger(),pos[0] +2,pos[1],pos[2],0.0,0.0,-1.0);
			}
			break;
	}
}