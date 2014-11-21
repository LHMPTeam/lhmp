/*
	===================================
	=     DeathMatch                  =
	=      BY ZAKLAUS                 =
	=     VERSION 1.0                 =
	=      LICENCE: Public Domain     =
	===================================
*/

//----------------------------------------------------------------------------- IMPORTANT STUFF
local rcon = "sex";//"YOURPASSWORD";	// DO NOT FORGET TO CHANGE THIS
local adInterval = 120;			// In what interval should server advertisement come.
local ads = ["Welcome to Aurora Play!","Gamemode is created by ZaKlaus!","Visit play.aurora-net.eu for additional info!"]; // Add here as much ads as you wish.
local varfreq = 40;				// Player shout frequency per cent. (0-100)
local shoutDelay = 3;			// Delay between last shout and next shout
local killDelay = 60;			// When kill combo expires.
//----------------------------------------------------------------------------- ARRAYS

//Dynamic
local admin = array(serverGetMaxPlayers(),0);
local radio = array(serverGetMaxPlayers(),0);
local killsCombo = array(serverGetMaxPlayers(),0);
local kills = array(serverGetMaxPlayers(),0);
local killTS = array(serverGetMaxPlayers(),0);

//Static
local peds_blocked = [1,2,5,6,7,9,10,11,12,13,15,17,18,19,22,28,31,42,43,45,62,63,74,77,78,80,82,83,84,85,86,87,89,98,100,109,112,117,123,124,125,132,137,149,150,154,155,156,157,158,159,161,163,164,166,170,176,177,178,179,170,187,193,197,199,214,218,222,224,226,228,234,238,250,252,255,257,259,263,279,278,280,286,289,293,297,298,299,300,301,302,303];
local peds = [3,4,8,14,16,20,21,23,24,25,26,27,29,30,32,33,37,38,39,40,41,44,46,47,48,49,50,51,55,56,57,58,59,60,61,64,65,66,67,68,69,70,71,72,73,75,76,79,81,88,90,91,92,93,94,95,96,97,99,101,102,103,104,105,106,107,108,110,111,113,114,115,116,118,119,120,121,122,126,129,130,131,133,134,135,136,138,139,140,141,142,143,144,145,146,147,148,151,152,153,160,162,165,167,168,169,171,172,173,174,175,181,182,183,184,185,186,188,189,190,191,192,194,195,196,198,200,201,202,203,204,205,206,207,208,209,210,211,212,213,215,216,217,219,220,221,223,225,227,229,230,231,232,233,235,236,237,239,247,248,249,251,253,254,256,258,260,261,262,264,265,266,267,268,269,270,271,272,273,274,275,277,279,281,282,283,284,285,287,288,290,291,292,294,295,296,34,35,36,127,128,240,241,242,243,244,245,246,52,53,54];
local spawnpos = [[-40.45,5.05,-0.12],[24.19,3.4,0.002],[7.44,3.41,-0.03],[0.17,0.31,-0.48],[-26.76,6.54,-0.8],[0.26,6.24,-6.4]];
local shoutsKill = ["00940001","01010080","01940012","01940017","01940069","01940074","01990141","01990361","01990421","01990831","01990861","02010040","02010110","02940010","02940014","0294022","02940028","00940105","00940092","00940080","00940077","00940069","00940129","00940133","00940134","00950001","00950004","00950008","00950011","00950013","00950014","00950020","00950026","00950027","00950031","00950032","00950034","00950035","00950039","00950049","00950051","00950056","00950059","00950071","00950072","00950089","00950095","00950096","00950110","00950151","00950154","00950188","00950189","00950200","00950204","00950218","00950249","01010140","01940007","01940012","01940016","01940011","01940019","01940027","01940069","01940070","01940071","01940072","01940075","01940077","01940079","01940081","01940082","01940083","01940089","01940090","01940091","01990071","01990081","01990111","01990131","01990151","01990171","01990181","01990231","01990511","01990521","01990641","01990841","01990861","01990871","02010050","02940019","02940022","02940027","02940030","02940040","02940082","02940096","02950004","02950005","02950010","02950011","02950012","02950024","02950064","02950115","03030010","03030030","03030050","03940010","03940041","03940060","03940065","04040025","04040030","04040050","04940010","04940018","04940025","04940027","05010190","05010350","05030080","05940002","05940015","05940033","05940095","05990051","19990502","19990632","20010140","21940004","21940003","21940005","21940006","21940007","21940008","21940009","21940012","21940021","21940055","21940079","21940022","23940023","23940022","23940021","23940020","23940004","23940001","23940026","23940027","23940036","23940044","23940046","23940048","23940050","23940051","23940069","23940072","23940078","23940091","24940026","2590023 ","25940045","28940059","30940048","30940048","28940004","27940024","27940011","25940038","25940026","25940024","25940023","25940021","25940002","24940063","21940081","20020450","20020160","19992011","19991512","19991255","19990351","19950110","19950060","19950040","18020420","18020360","17990421","17040550","15990212","15990084","15100010","15090070","15070080","15050021","14990251","14010460","14000220","14000120","13990071","12940059","12940039","11990763","11940083","11940040","08990041","08060130","07990231","07990161","07010240","15990801","15990881","16010040","16010090","16010100","17040020","17040461","17940058","17990051","18940059","18940052","18020301","16030060","15991621","15990211","15060030","15050004","15030130","14040050","14010560","14000250","14000230","12940086","12940039","12940006","12010140","10990672","09050210","09050190","08990151","08950015","08950017","08950013","09840008","08020010","07990415","07990371","07990224","07940029","07020190","05940026","04940058","04940039","04060050","03940079","28940001","28940002","28940003","28940004","28940005","28940006","28940007","28940008","28940009","28940010","28940011","28940012","28940013","28940014","28940015","28940016","28940017","28940018","28940019","28940020","28940021","28940022","28940023","28940025","28940026","28940027","28940028","28940029","28940030","28940031","28940032","28940033","28940034","28940035","28940036","28940037","28940038","28940039","28940040","28940041","28940042","28940043","28940044","28940045","28940046","28940047","28940048","28940049","28940042","28940043","28940044","28940045","28940046","28940047","28940048","28940049"];
local shoutsVar = ["01990831","02940001","02940025","01990811","01940016","01930069","01990131","01940069","01990841","01990831","01990211","01010080","01990211","00940105","00940092","00940080","00940077","00940069","00940129","00940133","00940134","00950001","00950004","00950008","00950011","00950013","00950014","00950020","00950026","00950027","00950031","00950032","00950034","00950035","00950039","00950049","00950051","00950056","00950059","00950071","00950072","00950089","00950095","00950096","00950110","00950151","00950154","00950188","00950189","00950200","00950204","00950218","00950249","01010140","01940007","01940012","01940016","01940011","01940019","01940027","01940069","01940070","01940071","01940072","01940075","01940077","01940079","01940081","01940082","01940083","01940089","01940090","01940091","01990071","01990081","01990111","01990131","01990151","01990171","01990181","01990231","01990511","01990521","01990641","01990841","01990861","01990871","02010050","02940019","02940022","02940027","02940030","02940040","02940082","02940096","02950004","02950005","02950010","02950011","02950012","02950024","02950064","02950115","03030010","03030030","03030050","03940010","03940041","03940060","03940065","04040025","04040030","04040050","04940010","04940018","04940025","04940027","05010190","05010350","05030080","05940002","05940015","05940033","05940095","05990051","19990502","19990632","20010140","21940004","21940003","21940005","21940006","21940007","21940008","21940009","21940012","21940021","21940055","21940079","21940022","23940023","23940022","23940021","23940020","23940004","23940001","23940026","23940027","23940036","23940044","23940046","23940048","23940050","23940051","23940069","23940072","23940078","23940091","24940026","2590023 ","25940045","28940059","30940048","30940048","28940004","27940024","27940011","25940038","25940026","25940024","25940023","25940021","25940002","24940063","21940081","20020450","20020160","19992011","19991512","19991255","19990351","19950110","19950060","19950040","18020420","18020360","17990421","17040550","15990212","15990084","15100010","15090070","15070080","15050021","14990251","14010460","14000220","14000120","13990071","12940059","12940039","11990763","11940083","11940040","08990041","08060130","07990231","07990161","07010240","15990801","15990881","16010040","16010090","16010100","17040020","17040461","17940058","17990051","18940059","18940052","18020301","16030060","15991621","15990211","15060030","15050004","15030130","14040050","14010560","14000250","14000230","12940086","12940039","12940006","12010140","10990672","09050210","09050190","08990151","08950015","08950017","08950013","09840008","08020010","07990415","07990371","07990224","07940029","07020190","05940026","04940058","04940039","04060050","03940079","28940001","28940002","28940003","28940004","28940005","28940006","28940007","28940008","28940009","28940010","28940011","28940012","28940013","28940014","28940015","28940016","28940017","28940018","28940019","28940020","28940021","28940022","28940023","28940025","28940026","28940027","28940028","28940029","28940030","28940031","28940032","28940033","28940034","28940035","28940036","28940037","28940038","28940039","28940040","28940041","28940042","28940043","28940044","28940045","28940046","28940047","28940048","28940049","28940042","28940043","28940044","28940045","28940046","28940047","28940048","28940049"];

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
local shoutTS = 0;
local weapon = 0;			// Weapon Mode: 0 - all weps, 1 - knives only, 2 - pistols only, 3 - machineguns only, 4 - shotguns only, 5 - grenades/molotovs only, 6 - snipers only
//----------------------------------------------------------------------------- CONSTANTS
local AUTHOR = "AUTHOR";
local ADMINS = "ADMINS";
local CONTACT = "CONTACT";

//----------------------------------------------------------------------------- EVENTS
function  onServerInit()
{
	print("Starting DeathMatch...");
	serverSetGamemodeName("DeathMatch");
	serverSetDefaultMap("MISE14-PARNIK");

	if (rcon == "YOURPASSWORD"){
		print("PLAYERS WILL BE KICKED, BECAUSE YOU FORGOT TO CHANGE RCON PASSWORD!");
	}
}
function onPlayerConnect(ID)
{
	local c = "";
	switch(weapon){			//Weapon Mode
		case 1:
			c = "Knives Only";
			break;
		case 2:
			c = "Pistols Only";
			break;
		case 3:
			c = "Machineguns Only";
			break;
		case 4:
			c = "Shotguns Only";
			break;
		case 5:
			c = "Explosives Only";
			break;
		case 5:
			c = "Sniper Rifles Only";
			break;
		default:
			c = "All Weapons";
	}
	if(rcon == "YOURPASSWORD"){
		sendPlayerMessage(ID,red+"RCON VULNERABILITY DETECTED! CONTACT ADMIN TO CHANGE RCON PASSWORD!");
		print("USER HAS BEEN DISCONNECTED BECAUSE OF RCON VULNERABILITY!");
		playerKick(ID);
	}

	sendPlayerMessage(ID,red + "====================================");
	sendPlayerMessage(ID,yellow + "Welcome to DeathMatch server!");
	sendPlayerMessage(ID,yellow + "Type " + white + "/help " + yellow + "to get commands.");
	sendPlayerMessage(ID,"");
	sendPlayerMessage(ID,yellow + "There are " + white + serverGetOnlinePlayers() + yellow + " players online.");
	sendPlayerMessage(ID,cyan+"Weapon Mode: "+c);
	sendPlayerMessage(ID,red + "====================================");
	
	guiToggleNametag(ID,0);

	playerToggleCityMusic(ID,1);
	print("Player "+playerGetName(ID)+"["+ID+"] has been connected to the server.");
}

function onPlayerSpawn(ID)
{
	//sendAllMessage("Player "+playerGetName(ID)+"["+ID+"] has been respawned.");
	
	playerChangeSkin(ID,peds[rand()%peds.len()]);
	playerEnableMoney(ID,1);
	playerSetMoney(ID,kills[ID]);
	//pozicovanie hraca
	local rnd = random(0,spawnpos.len());
	playerSetPosition(ID,spawnpos[rnd][0].tofloat(),spawnpos[rnd][1].tofloat(),spawnpos[rnd][2].tofloat());
	switch(weapon){			//Weapon Mode
		case 1:
			playerAddWeapon(ID,3,1,1);
			break;
		case 2:
			playerAddWeapon(ID,6,12,1000);
			break;
		case 3:
			playerAddWeapon(ID,10,50,1000);
			break;
		case 4:
			playerAddWeapon(ID,11,20,1000);
			break;
		case 5:
			playerAddWeapon(ID,15,1,1);
			playerAddWeapon(ID,5,1,1);
			break;
		case 5:
			playerAddWeapon(ID,14,5,1000);
			break;
		default:
			playerAddWeapon(ID,10,100,100);
	}
}
function onPlayerDisconnect(ID)
{
	admin[ID]=0;
	radio[ID]=0;
	killsCombo[ID] = 0;
	killTS[ID] = 0;
	kills[ID] = 0;
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
	srand(time());
	seconds++;
	if(seconds>=adInterval){	// Shows advertisement every 'adInterval' seconds.
		seconds = 0;
		sendAllMessage(ads[random(0,ads.len())]);
	}
}



function onPlayerShoot(ID,wepID){
	if((rand() % 100) < (varfreq) && ((time() - shoutTS) > shoutDelay)){
		allPlaySound("sounds\\"+shoutsVar[random(0,shoutsVar.len())]+".wav");
		shoutTS = time();
	}
	if(weapon == 5){
		playerAddWeapon(ID,15,1,1);
		playerAddWeapon(ID,5,1,1);
	}
}

function onPlayerThrowGranade(ID,wepID){
	playerAddWeapon(ID,15,1,1);
	playerAddWeapon(ID,5,1,1);
}

function onPlayerIsKilled(ID,killerID){
	if(ID!=killerID){
		kills[killerID]++;
		if(((time() - killTS[killerID]) < (killDelay)) || killsCombo[killerID]==0){
			killTS[killerID] = time();
			killsCombo[killerID]++;
			local i = killsCombo[killerID];
			local m = red+"Player "+playerGetName(killerID) + " achieved ";
			local n = " Total kills: "+cyan+kills[killerID]+red+".";
			if(i==2){ //Double kill
				sendAllMessage(m+"Double-Kill!"+n);
			} else if(i==3){
				sendAllMessage(m+"Multi-Kill!"+n);
			} else if(i==4){
				sendAllMessage(m+"Ultra-Kill!"+n);
			} else if(i==5){
				sendAllMessage(m+"MONSTER-Kill!"+n);
			} else if(i==6){
				sendAllMessage(m+"LUDICROUS-Kill!"+n);
			} else if(i>6){
				sendAllMessage(m+"HOLY SHIT!"+n);
			}

		} else {
			killsCombo[killerID] = 0;
			sendPlayerMessage(killerID,red+"Combo chance lost!")
		}
		sendAllMessage(green+"Player "+red+playerGetName(ID)+green+"'s been killed by "+red+playerGetName(killerID));
	} else {
		sendAllMessage(green+"Player "+red+playerGetName(ID)+green+" commited suicide.");
	}
	allPlaySound("sounds\\"+shoutsKill[random(0,shoutsKill.len())]+".wav");
	playerSetMoney(killerID,kills[killerID]);
	killsCombo[ID] = 0;
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
	sendPlayerMessage(ID,"/skin " + yellow + "- Enables skin selector.");
	sendPlayerMessage(ID,"/wep [id] [clip] [ammo] " + yellow + "- Adding weapon");
	//sendPlayerMessage(ID,"/addcar [id] " + yellow + "- Spawning car");
	//sendPlayerMessage(ID,"/t [message] " + yellow + "- post message!");
	//sendPlayerMessage(ID,"/chat " + yellow + "- show info about chat system.");

}

function help2(ID){
	sendPlayerMessage(ID,"/anim [id] " + yellow + "- Let's do some animation :)");
	sendPlayerMessage(ID,"/setlines [amount] " + yellow + "- Sets the amount of chat lines (starts from 6)");
	//sendPlayerMessage(ID,"/tplist " + yellow + "- Shows you list of available teleports");
	sendPlayerMessage(ID,"/kill " + yellow + "- Get yourself killed!");
	sendPlayerMessage(ID,"/info " + yellow + "- Shows you info about server");
	sendPlayerMessage(ID,"/music " + yellow + "- Toggles music");
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
		case "stats":
			sendPlayerMessage(ID,"Your Score: "+kills[ID]);
			break;
		case "music":
			if(radio[ID]==0){
				radio[ID]=1;
				sendPlayerMessage(ID,green+"Music turned on.");
			}else{
				radio[ID]=0;
				sendPlayerMessage(ID,green+"Music turned off.");
			}

			playerToggleCityMusic(ID,radio[ID]);
			break;
		case "wepmode":		//Weapon Mode Selector
			if(admin[ID]==0)break;
			weapon = params.tointeger();
			local c = "";
			switch(weapon){			//Weapon Mode
				case 1:
					c = "Knives Only";
					break;
				case 2:
					c = "Pistols Only";
					break;
				case 3:
					c = "Machineguns Only";
					break;
				case 4:
					c = "Shotguns Only";
					break;
				case 5:
					c = "Explosives Only";
					break;
				case 5:
					c = "Sniper Rifles Only";
					break;
				default:
					c = "All Weapons";
			}
			sendAllMessage(cyan+"Weapon Mode: "+c);
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
		case "nametag":
			//guiToggleNametag(ID,params.tointeger());
			break;
		case "rand":
			//sendPlayerMessage(ID,"Random: "+rand()%params.tointeger());
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
		case "kill":
			playerSetHealth(ID,0.0);
			break;
		case "a":
			playerPlayAnim(ID, params);
			break;
		case "s":
			//allPlaySound("sounds\\"+params+".wav");
			break;
		case "as":
			if(admin[ID]==1){
				allPlaySound("sounds\\"+params);}
			else {
				sendPlayerMessage(ID,red + "No privileges!");}
			break;
		case "wep":
			if(weapon!=0){break;}
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
				sendPlayerMessage(ID,red + "Wrong syntax! '/wep [wepid] [bullets] [clips]'");
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
				playerSetPosition(ID,oldpos[0],oldpos[1],oldpos[2]args[1].tointeger());
			}
			break;
		case "delcars":
			if(admin[ID]==1) 
			{
				for(local i = 0; i < carcount; i++)
				{
					for (local j = 0; j < serverGetMaxPlayers(); ++j)
					{
						vehicleDelete(i);
					
					}
					
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