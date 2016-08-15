function  onServerInit()
{
	print("Romco Script ...");
}

function onPlayerSpawn(ID)
{
	playerAddWeapon(ID,10,100,500);
	playerSetPosition(ID,-1935.966675,-5.037054,4.284860);
	//playerSetPosition(ID,-34.27843,1.629,10.87);
	playerToggleCityMusic(true);
}

function onPlayerCommand(ID,message,params)
{
	//print("Command: "+message+" - "+params);
	switch(message)
	{
		case "changepos":
			playerSetPosition(ID,-34.27843,1.629,10.87);
			break;
		case "heal":
  		 	playerSetHealth(ID, 200.0);
  		 	break;
		case "die":
  		 	playerSetHealth(ID, 0.0);
  		 	break;
		case "a":
  		 	playerPlayAnim(ID, params);
  		 	break;
  		case "s":
   			allPlaySound("sounds\\"+params+".wav");
   		break;
		case "cs":
			playerChangeSkin(ID,params.tointeger());
			sendPlayerMessage(ID,"Skin's been changed");
		break;
		case "wep":
			local paramArray = split(params," ");
			if(paramArray.len() != 3)
			{
				sendPlayerMessage(ID,"BAD Syntax, use: /wep ID ammo1 ammo2");
			} else
			{
				playerAddWeapon(ID,paramArray[0].tointeger(),
				paramArray[1].tointeger(),paramArray[2].tointeger());
				sendPlayerMessage(ID,"You have been given a weapon.");
			}
		break;
		case "kickme":
			playerKick(ID);
		case "animme":
			playerPlayAnim(ID,"Nuda01");
			break;
		case "anim":
			playerPlayAnim(ID,params);
			break;
		case "kurva":
			playerPlaySound(ID,"sounds\\15020060.wav");
			break;
		case "allkurva":
			allPlaySound(ID,"sounds\\15020060.wav");
			break;
		case "camerka":
			playerSetCameraPos(ID,-1788.927612,-4.542368,-9.105090,
			0.982404,0.520000,1.017291);
			break;
		case "camerkaback":
			playerSetCameraToDefault(ID);
		case "addcar":
			local pos = playerGetPosition(ID);
			vehicleSpawn(params.tointeger(),pos[0] +2,pos[1],pos[2],0.0,0.0,0.0);
			break;
		case "deleteveh":
			vehicleDelete(params.tointeger());
			break;
		case "status":
			local newarray = points;
			newarray.sort(custom_compare);
			sendPlayerMessage(ID,"DM Statistics:");
			for(local i = 0; i < newarray.len();i++)
			{
				if(playerGetName(i) == null)
					continue;
				sendPlayerMessage(ID,"ID "+playerGetName(i)+"SCORE+"+newarray[i]);
			}
			break;
		case "info":
			sendPlayerMessage(ID,"You are playing on "+serverGetName()+"server"+
			" "+serverGetOnlinePlayers()+"/"+serverGetMaxPlayers()+", running gamemode:"+
		serverGetGamemodeName());
		break;
	}
	
}