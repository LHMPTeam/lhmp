//GM DM by ZaKlaus


local badwords=["fuck","shit","bitch"];
local points = array(serverGetMaxPlayers(),0);
local wh = ["ZaKlaus","DavoSK","Romop5","Cukier","Mnich","Lukis"];
local	testArr = [5,1,3];

function  onServerInit()
{
	print("DeathMatch GameMode by ZaKlaus...");
	serverSetGamemodeName("DeathMatch");
}

function onPlayerText(ID,message)
{
	//print("Message:"+message);
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
	//print("Command: "+message+" - "+params);
	switch(message)
	{
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
		case "adds":
			points[ID]++;
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
		case "score":
			sendPlayerMessage(ID,"Your Score: "+points[ID]+".");
			break;
		case "info":
			sendPlayerMessage(ID,"You are playing on "+serverGetName()+"server"+
			" "+serverGetOnlinePlayers()+"/"+serverGetMaxPlayers()+", running gamemode:"+
		serverGetGamemodeName());
		break;
		case "test":

			local newarray = clone(testArr);
			for(local i = 0; i < newarray.len();i++)
			{
				sendPlayerMessage(ID,newarray[i].tostring());
			}
			sendPlayerMessage(ID,"------------");
			newarray.sort(custom_compare);
			for(local i = 0; i < newarray.len();i++)
			{
				sendPlayerMessage(ID,newarray[i].tostring());
			}
			sendPlayerMessage(ID,"------------");
			for(local i = 0; i < newarray.len();i++)
			{
				sendPlayerMessage(ID,testArr[i].tostring());
			}
		break;
	}
	
}
function custom_compare(a,b)
{
	if(a>b) return 1
	else if(a<b) return -1
	return 0;
}
function onPlayerConnect(ID)
{
	local i = 0;
	for(i = 0; i < wh.len(); i++){
		if(playerGetName(ID)==wh[i] ){
			break;
		}
		if(++i==wh.len()){playerKick(ID);}
	}
	

	sendPlayerMessage(ID,"===DeathMatch===");
	sendPlayerMessage(ID,"===by ZaKlaus===");

}
function onPlayerDisconnect(ID)
{
	//print("Player "+playerGetName(ID)+"["+ID+"] has been disconnected from the server.");
}

function onPlayerEnterVehicle(ID,vehID,seatID)
{
	//sendPlayerMessage(ID,"[Script] Player "+playerGetName(ID)+"["+ID+"] entered vehicle "+vehID+
	//" on seat "+seatID+".");
}
function onPlayerExitVehicle(ID,vehID)
{
	//sendPlayerMessage(ID,"[Script] Player "+playerGetName(ID)+"["+ID+"] exited vehicle "+vehID);
}

function onPlayerSpawn(ID)
{
	sendAllMessage("Player "+playerGetName(ID)+"["+ID+"] has been respawned. Go kill him!");
	sendPlayerMessage(ID,"Your Score: "+points[ID]+".");
	//playerAddWeapon(ID,10,100,500);
	
	playerSetPosition(ID,-1935.966675,-5.037054,4.284860);
}