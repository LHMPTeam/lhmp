local badwords=["fuck","shit","bitch"];
function foo(i, f, s) 
{ 
    print("Called foo(), i="+i+", f="+f+", s='"+s+"'\n");
	testfunc();
}

function  onServerInit()
{
	print("Simple script init...");
	serverSetGamemodeName("RomcoTestMode");
}

function onPlayerText(ID,message)
{
	print("Message:"+message);
	foreach(val in badwords)
    	{
		if(message.find(val) != null)
		{
			sendPlayerMessage(ID,"Don't swear !");
			return false;
		}
	}
	local IP = playerGetIP(ID);
	print("IP: "+IP);
}

function onPlayerCommand(ID,message,params)
{
	print("Command: "+message+" - "+params);
	switch(message)
	{
		case "help":
			sendPlayerMessage(ID,"F7 = hide HUD, F8 = show HUD, F9 = screenshot");
			sendPlayerMessage(ID,"Commands: /cs ID /addwep ID ammo ammo /addcar ID");
			sendPlayerMessage(ID,"/portback /kickme /heal");
		break;
		case "cs":
			if(params == "")
			{
				sendPlayerMessage(ID,"Use /cs skinID");
			} else {
				playerChangeSkin(ID,params.tointeger());
				sendPlayerMessage(ID,"Skin's been changed");
			}
		break;
		case "puttoveh":
			playerPutToVehicle(ID,params.tointeger(),0);
		break;
		case "heal":
			playerSetHealth(ID,200.0);
		break;
		case "spawnikaru":
			vehicleSpawn(80,-1985.966675,0.037054,4.284860,0.0,0.0,0.0);
			break;
		case "portback":	
			playerSetPosition(ID,-1985.966675,-5.037054,4.284860);
			vehicleSetPosition(0,-1965.966675,-5.037054,4.284860);
		break;
		case "kickme":
			playerKick(ID);
		case "status":
			sendPlayerMessage(ID,serverGetOnlinePlayers()+"/"+serverGetMaxPlayers());
	}
	
}

function onPlayerConnect(ID)
{
	print("[Script] Player "+playerGetName(ID)+"["+ID+"] has connected.");
}
function onPlayerDisconnect(ID)
{
	print("[Script] Player "+playerGetName(ID)+"["+ID+"] has disconnected.");
}

function onPlayerEnterVehicle(ID,vehID,seatID)
{
	sendPlayerMessage(ID,"[Script] Player "+playerGetName(ID)+"["+ID+"] entered vehicle "+vehID+
	" on seat "+seatID+".");
}
function onPlayerExitVehicle(ID,vehID)
{
	sendPlayerMessage(ID,"[Script] Player "+playerGetName(ID)+"["+ID+"] exited vehicle "+vehID);
}

function onPlayerSpawn(ID)
{
	playerAddWeapon(ID,10,100,500);
	playerSetPosition(ID,-1935.966675,-5.037054,4.284860);
}