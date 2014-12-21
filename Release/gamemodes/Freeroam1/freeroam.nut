/*
	===================================
	=     Freeroam                    =
	=      BY ZAKLAUS                 =
	=     VERSION 1.0                 =
	=      LICENCE: Public Domain     =
	===================================
*/

//----------------------------------------------------------------------------- IMPORTANT STUFF
local rcon = "sex";	// DO NOT FORGET TO CHANGE THIS
local adInterval = 120;			// In what interval should server advertisement come.
local ads = ["This is an official public test!","Gamemode is created by ZaKlaus!","Visit www.lh-mp.eu for additional info!"]; // Add here as much ads as you wish.
local carsweep = 20;
//----------------------------------------------------------------------------- ARRAYS

//Dynamic
local admin = array(serverGetMaxPlayers(),0);
local radio = array(serverGetMaxPlayers(),0);
local cars = array(serverGetMaxPlayers(),-1);

//Static
local peds_blocked = [1,2,5,6,7,9,10,11,12,13,15,17,18,19,22,28,31,42,43,45,62,63,74,77,78,80,82,83,84,85,86,87,89,98,100,109,112,117,123,124,125,132,137,149,150,154,155,156,157,158,159,161,163,164,166,170,176,177,178,179,170,187,193,197,199,214,218,222,224,226,228,234,238,250,252,255,257,259,263,279,278,280,286,289,293,297,298,299,300,301,302,303];
local peds = [3,4,8,14,16,20,21,23,24,25,26,27,29,30,32,33,37,38,39,40,41,44,46,47,48,49,50,51,55,56,57,58,59,60,61,64,65,66,67,68,69,70,71,72,73,75,76,79,81,88,90,91,92,93,94,95,96,97,99,101,102,103,104,105,106,107,108,110,111,113,114,115,116,118,119,120,121,122,126,129,130,131,133,134,135,136,138,139,140,141,142,143,144,145,146,147,148,151,152,153,160,162,165,167,168,169,171,172,173,174,175,181,182,183,184,185,186,188,189,190,191,192,194,195,196,198,200,201,202,203,204,205,206,207,208,209,210,211,212,213,215,216,217,219,220,221,223,225,227,229,230,231,232,233,235,236,237,239,247,248,249,251,253,254,256,258,260,261,262,264,265,266,267,268,269,270,271,272,273,274,275,277,279,281,282,283,284,285,287,288,290,291,292,294,295,296];
local peds_police = [34,35,36,127,128,240,241,242,243,244,245,246];
local peds_detectives = [52,53,54];


//----------------------------------------------------------------------------- COLOR CONSTANTS
local red = "#ff0000";
local green = "#00ff00";
local blue = "#0000ff";
local yellow = "#ffff00";
local orange = "#ff7700";
local white = "#ffffff";
local gray25 = "#bfbfbf";
local gray50 = "#808080";
local gray75 = "#404040";
local black = "#000000";
local cyan = "#00FFFF";

//----------------------------------------------------------------------------- VARIABLES
local carcount = 0;
local seconds = 0;
local secondsC = 0;
//----------------------------------------------------------------------------- CONSTANTS
local AUTHOR = "AUTHOR";
local ADMINS = "ADMINS";
local CONTACT = "CONTACT";

//----------------------------------------------------------------------------- EVENTS
function  onServerInit()
{
	print("Starting Freeroam...");
	serverSetGamemodeName("Freeroam");
	iniSetParam("zak.txt","isGod","nope");
	iniSetParam("zak.txt","noob","yea");
	iniRemoveFile("zak.txt");
	if (rcon == "YOURPASSWORD"){
		print("PLAYERS WILL BE KICKED, BECAUSE YOU FORGOT TO CHANGE RCON PASSWORD!");
	}
}
function onPlayerConnect(ID)
{
	if(rcon == "YOURPASSWORD"){
		sendPlayerMessage(ID,red+"RCON VULNERABILITY DETECTED! CONTACT ADMIN TO CHANGE RCON PASSWORD!");
		print("USER HAS BEEN DISCONNECTED BECAUSE OF RCON VULNERABILITY!");
		playerKick(ID);
	}

	playerChangeSkin(ID,peds[rand()%peds.len()]);
	sendPlayerMessage(ID,red + "====================================");
	sendPlayerMessage(ID,yellow + "Welcome to Freeroam server!");
	sendPlayerMessage(ID,yellow + "Type " + white + "/help " + yellow + "to get commands.");
	sendPlayerMessage(ID,"FREEROAM IS UNSTABLE RIGHT NOW, WE SUGGEST PLAYING DM INSTEAD!");
	sendPlayerMessage(ID,yellow + "There are " + white + serverGetOnlinePlayers() + yellow + " players online.");
	sendPlayerMessage(ID,red + "====================================");
	sendPlayerMessage(ID,"/addwep [wepid] [ammo] [ammo2] " + yellow + "- adds a weapon");
	sendPlayerMessage(ID,"/addcar [carid] " + yellow + "- spawns a car");
	playerToggleCityMusic(ID,0);
	print("Player "+playerGetName(ID)+"["+ID+"] has been connected to the server.");
}

function onPlayerSpawn(ID)
{
	sendAllMessage("Player "+playerGetName(ID)+"["+ID+"] has been spawned.");
}
function onPlayerDisconnect(ID)
{
	admin[ID]=0;
	radio[ID]=0;
	if(cars[ID]!=-1){
		//vehicleDelete(cars[ID]);
		//cars[ID] = -1;
	}
	print("Player "+playerGetName(ID)+"["+ID+"] has been disconnected from the server.");
}
function onPlayerText(ID,message)
{
	print("["+playerGetName(ID)+"] Message: "+message);	
	//print("IP: "+ playerGetIP(ID));
	sendAllMessage("<"+playerGetName(ID)+"> "+message);

	return false;
}

function onServerTickSecond(ticks){
	seconds++;
	secondsC++;
	if(seconds>=adInterval){	// Shows advertisement every 'adInterval' seconds.
		seconds = 0;
		sendAllMessage(ads[random(0,ads.len())]);
	}


	return;
	if(secondsC >= carsweep){
		secondsC = 0;
		for(local i = 0; i < carcount; i++)
				{
					if(playerInVehicleID(i) == -1){
						vehicleDelete(i);
						carcount--;
					}
				}
	}
}

function onPlayerEnterVehicle(ID,vehID,seatID)
{
	//sendPlayerMessage(ID,"[Script] Player "+playerGetName(ID)+"["+ID+"] entered vehicle "+vehID+
	//" on seat "+seatID+".");
	playerToggleCityMusic(ID,radio[ID]);
}
function onPlayerExitVehicle(ID,vehID)
{
	playerToggleCityMusic(ID,0);
	//if(auta[ID]==1){auta[ID]=0; vehicleDelete(vehID);}
	//sendPlayerMessage(ID,"[Script] Player "+playerGetName(ID)+"["+ID+"] exited vehicle "+vehID);
}

function onPlayerIsKilled(ID,killerID){
	if(ID!=killerID){
		sendAllMessage("Player "+playerGetName(ID)+"'s been killed by "+playerGetName(killerID));
	}
}

//----------------------------------------------------------------------------- HELPER FUNCTIONS
function getDistance(x1, x2, y1, y2){
	return (sqrt( ( (x2-x1)*(x2-x1) ) + ( (y2-y1)*(y2-y1) ) ) );
}

function isInArea(posx,posy,x1,x2,y1,y2){
	if((posx >= x1 && posx <= x2) && (posy >= y1 && posy <= y2)){
		return 1;
	} else {
		return 0;
	}
}

function random(min,max){
	srand(time());
	return ((rand() % (max-min)) + min);
}

function printHelp(ID){
			sendPlayerMessage(ID,red + "====================================");
			sendPlayerMessage(ID,yellow + "List of available commands:");
			sendPlayerMessage(ID,yellow + "/help [id] - prints help page");
			sendPlayerMessage(ID,red + "====================================");
}

function help1(ID){
	sendPlayerMessage(ID,"/cs [skinid] " + yellow + "- Change your skin!");
	//sendPlayerMessage(ID,"/addwep [id clip ammo] " + yellow + "- Adding weapon");
	//sendPlayerMessage(ID,"/addcar [id] " + yellow + "- Spawning car");
	sendPlayerMessage(ID,"/t [message] " + yellow + "- post message!");
	sendPlayerMessage(ID,"/chat " + yellow + "- show info about chat system.");

}

function help2(ID){
			sendPlayerMessage(ID,"/anim [id] " + yellow + "- Let's do some animation :)");
			sendPlayerMessage(ID,"/setlines [amount] " + yellow + "- Sets the amount of chat lines (starts from 6)");
			//sendPlayerMessage(ID,"/tplist " + yellow + "- Shows you list of available teleports");
			sendPlayerMessage(ID,"/kill " + yellow + "- Get yourself killed!");
			sendPlayerMessage(ID,"/info " + yellow + "- Shows you info about server");
			sendPlayerMessage(ID,"/radio " + yellow + "- Toggles car radio");
			sendPlayerMessage(ID,"/quit " + yellow + "- quit's the game");
}

function cmp(a,b)
{
	if(a>b) return 1
	else if(a<b) return -1
	return 0;
}

//----------------------------------------------------------------------------- PLAYER COMMANDS
function onPlayerCommand(ID,message,params)
{
	switch(message) // plrsw
	{
		case "help":
			switch(params){
				case "1":
					help1(ID);
					break;
				case "2":
					help2(ID);
					break;
				default:
					printHelp(ID);
					break;
			}
			break;
		case "radio":
			if(radio[ID]==0){
				radio[ID]=1;
				sendPlayerMessage(ID,green+"Radio turned on.");
			}else{
				radio[ID]=0;
				sendPlayerMessage(ID,green+"Radio turned off.");
			}

			if(playerInVehicleID(ID)!=-1){playerToggleCityMusic(ID,radio[ID]);}
			break;
		case "ahelp":
			if(admin[ID]==1){
			sendPlayerMessage(ID,red + "====================================");
			sendPlayerMessage(ID,yellow + "List of available admin commands:");
			sendPlayerMessage(ID,"/delcars " + yellow + "- Removing all cars from map");
			sendPlayerMessage(ID,"/kick [id] " + yellow + "- Kicking player by ID");
			sendPlayerMessage(ID,"/getip [id] " + yellow + "- Get player IP by ID");
			sendPlayerMessage(ID,red + "====================================");}
			else{sendPlayerMessage(ID,red + "No privileges!");}
			break;
		case "me":
			sendAllMessage("#663399*" + playerGetName(ID) + " " + params);
			break;
		case "neo":
			if(admin[ID]==0){break;}
			playerChangeSkin(ID,303);
			sendPlayerMessage(ID,"Welcome back, agent "+playerGetName(ID)+"!");
			playerAddWeapon(ID,12,1000,1000);
			break;
		case "nametag":
			guiToggleNametag(ID,params.tointeger());
			break;
		case "rand":
			sendPlayerMessage(ID,"Random: "+rand()%params.tointeger());
			break;
		case "rcon":
			if(rcon == params)
			{
				admin[ID]=1;
				print("Player [" + playerGetName(ID) + "] has successfully login as administrator");
				sendPlayerMessage(ID,cyan + "You are logged as Admin!");
				sendPlayerMessage(ID,cyan + "Type " + white + "/ahelp" + cyan + " for admin commands.");
			}
			else
			{sendPlayerMessage(ID,red + "Wrong rcon password!");}
			break;
		case "kick":
			local args = split(params," ");
			if(admin[ID]==1)
			{
			sendPlayerMessage(ID,red + "Player " + white + playerNameIP(args[0].tointeger()) + red + " was kicked for " + args[1]);
			kickPlayer(args[0].tointeger());
			}
			else
			{
			sendPlayerMessage(ID,red + "No privileges!");
			}
			break;
		case "getip":
			if(admin[ID]==1){
				sendPlayerMessage(ID,red + "Player " + white + playerGetName(params.tointeger()) + red + " IP: " + white + playerGetIP(params.tointeger()));
			} 
			else {
				sendPlayerMessage(ID,red + "No privileges!");
			}
			break;
		case "cs":
			//if(admin[ID]==0){break;} ALLOW FOR EVERYONE
			if (params.tointeger() > 302){sendPlayerMessage(ID,red + "Wrong ID!");}
			else {playerChangeSkin(ID,params.tointeger());}
			//sendPlayerMessage(ID,Skin's been changed");
			break;
		case "kill":
			playerSetHealth(ID,0.0);
			break;
		case "a":
			playerPlayAnim(ID, params);
			break;
		case "s":
			allPlaySound("sounds\\"+params+".wav");
			break;
		case "as":
			if(admin[ID]==1){
				allPlaySound("sounds\\"+params);}
			else {
				sendPlayerMessage(ID,red + "No privileges!");}
			break;
		case "addwep":
			//if(admin[ID]==0){break;}
			local args = split(params," ");
			local wepID = 0;
			local ammo = 1;
			local ammo2 = 0;
			if(args.len() > 0)
			{
				wepID = args[0].tointeger();
				if(args.len() > 1)
				{
					ammo = args[1].tointeger();
					if(args.len() > 2){
					ammo2 = args[2].tointeger();}
				}
			playerAddWeapon (ID,wepID,ammo,ammo2);
			//??
			}
			else {
				sendPlayerMessage(ID,red + "Wrong syntax!");
			}
			break;
		case "getpos":
			local xyz = playerGetPosition(ID);
			sendPlayerMessage(ID,"X: "+yellow+xyz[0].tostring() + white + " Y: "+yellow+xyz[1].tostring()+ white + " Z: "+yellow+xyz[2].tostring());
			break;
		case "setpos":
			if(admin[ID]==0){break;}
			local xyz = split(params," ");
			playerSetPosition(ID,xyz[0].tointeger(),xyz[1].tointeger(),xyz[2].tointeger());
			break;
		case "mov":
			if(admin[ID]==0){break;}
			local args = split(params," ");
			local oldpos = playerGetPosition(ID);
			if(args[0]=="x"){
				playerSetPosition(ID,oldpos[0]+args[1].tointeger(),oldpos[1],oldpos[2]);
			}
			else if(args[0]=="y"){
				playerSetPosition(ID,oldpos[0],oldpos[1]+args[1].tointeger(),oldpos[2]);
			}
			else if(args[0]=="z"){
				playerSetPosition(ID,oldpos[0],oldpos[1],oldpos[2]+args[1].tointeger());
			}
			break;
		case "addcar":
			//if(admin[ID]==0){break;} // Admin Only
			local pos = playerGetPosition(ID);
			if (params.tointeger() > 169){sendPlayerMessage(ID,red + "Wrong ID!");}
			else{
				if(carcount> 16){sendPlayerMessage(ID,red + "Car capacity's been reached! Grab some car on street!"); return;}
				carcount++;
				if(cars[ID]!=-1){
					//vehicleDelete(cars[ID]);

				}
			vehicleSpawn(params.tointeger(),pos[0] +2,pos[1],pos[2],1.0,0.0,0.0);}
			cars[ID] = carcount-1;
			break;
		case "delcars": break;
			if(admin[ID]==1) 
			{
				for(local i = 0; i < carcount; i++)
				{
					/*for (local j = 0; j < serverGetMaxPlayers(); ++j)
					{*/
						vehicleDelete(i);
						carcount = 0;
					// }
					
				}
			}
			else
				{
				sendPlayerMessage(ID,red + "No privileges!");
				}
			break;
		//case "fun":
		//local pos = playerGetPosition(ID);
		//local currentx = pos[0];
		
		//for(local i = 0; i <169; i++)
		//{
		//	vehicleSpawn(i,currentx + 5, pos[1], pos[2],0.0,0.0,0.0);
		//	currentx += 5;
		//}
		
		//break;
		case "info":
			sendPlayerMessage(ID,red + "====================================");
			sendPlayerMessage(ID,yellow + "You are on " + white + serverGetName() + yellow + " server.");
			sendPlayerMessage(ID,yellow + "There are " + white + serverGetOnlinePlayers() + yellow + " players online.");
			sendPlayerMessage(ID,yellow + "Gamemode: " + white + serverGetGamemodeName());
			sendPlayerMessage(ID,yellow + "Author: "+white+AUTHPR);
			sendPlayerMessage(ID,yellow + "Admin: " + white + ADMINS);
			sendPlayerMessage(ID,yellow + "Contact: " + white + CONTACT);
			sendPlayerMessage(ID,red + "====================================");
			break;
		case "tp":
			if(!admin[ID]){break;}
			local iPos = playerGetPosition(params.tointeger());
			playerSetPosition(ID,iPos[0],iPos[1],iPos[2]);
			break;
		case "tphere":
			if(!admin[ID]){break;}
			local iPos = playerGetPosition(ID);
			playerSetPosition(params.tointeger(),iPos[0],iPos[1],iPos[2]);
			break;
		case "matrix":
		if(!admin[ID]){break;}
		playerSetPosition(ID,-3318.581299, 24.137, -776.702393);
		playerSetRotation(ID,0.0, 0.0, 180.0);
		vehicleSetPosition(ID,-3318.581299, 24.137, -776.702393, 0.0, 0.0, 0.0);
		break;
	}
	//playerSetMoney(ID,money[ID],"$");
}


//=========================================================================
/*
	HELPER SECTION
	USEFUL SNIPPETS
*/


//Section delimiter
//-----------------------------------------------------------------------------

//=========================================================================