function  onServerInit()
{
	serverSetDefaultMap("mise13-zradce");
}

function onPlayerSpawn(ID)
{
	sendAllMessage("Player "+playerGetName(ID)+"["+ID+"] has been respawned.");
	playerSetPosition(ID,-36.08815765,1.421900392,43.2922554);

}