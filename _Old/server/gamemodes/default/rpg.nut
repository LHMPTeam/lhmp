/*
	===================================
	=     REAL-LIFE/RPG MODULE        =
	=          BY ZAKLAUS AND CUKIER  =
	=     VERSION 1.0                 =
	=          LICENCE: GNU/GPLv3     =
	===================================
*/

//----------------------------------------------------------------------------- IMPORTANT STUFF
local rcon = "likopen";

//----------------------------------------------------------------------------- ARRAYS

//Dynamic
local auta = array(serverGetMaxPlayers(),0);
local admin = array(serverGetMaxPlayers(),0);
local user = array(serverGetMaxPlayers(),0);
local money = array(serverGetMaxPlayers(),0);
local pwd = array(serverGetMaxPlayers(),"");
local wanted = array(serverGetMaxPlayers(),0);
local name = array(serverGetMaxPlayers(),"");
local ldriver = array(serverGetMaxPlayers(),0);
local muted = array(serverGetMaxPlayers(),0);
local spy = array(serverGetMaxPlayers(),0);
local god = array(serverGetMaxPlayers(),0);
local lastpm = array(serverGetMaxPlayers(),-1);
local radio = array(serverGetMaxPlayers(),0);

//Static
local peds_blocked = [1,2,5,6,7,9,10,11,12,13,15,17,18,19,22,28,31,42,43,45,62,63,74,77,78,80,82,83,84,85,86,87,89,98,100,109,112,117,123,124,125,132,137,149,150,154,155,156,157,158,159,161,163,164,166,170,176,177,178,179,170,187,193,197,199,214,218,222,224,226,228,234,238,250,252,255,257,259,263,279,278,280,286,289,293,297,298,299,300,301,302,303];
local peds = [3,4,8,14,16,20,21,23,24,25,26,27,29,30,32,33,37,38,39,40,41,44,46,47,48,49,50,51,55,56,57,58,59,60,61,64,65,66,67,68,69,70,71,72,73,75,76,79,81,88,90,91,92,93,94,95,96,97,99,101,102,103,104,105,106,107,108,110,111,113,114,115,116,118,119,120,121,122,126,129,130,131,133,134,135,136,138,139,140,141,142,143,144,145,146,147,148,151,152,153,160,162,165,167,168,169,171,172,173,174,175,181,182,183,184,185,186,188,189,190,191,192,194,195,196,198,200,201,202,203,204,205,206,207,208,209,210,211,212,213,215,216,217,219,220,221,223,225,227,229,230,231,232,233,235,236,237,239,247,248,249,251,253,254,256,258,260,261,262,264,265,266,267,268,269,270,271,272,273,274,275,277,279,281,282,283,284,285,287,288,290,291,292,294,295,296];
local peds_police = [34,35,36,127,128,240,241,242,243,244,245,246];
local peds_detectives = [52,53,54];
local areas = [["name","x1","x2","y1","y2",["x","y","z"]],["Little Italy","1000","2000","0","1337"]];

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

//----------------------------------------------------------------------------- CONSTANTS


//----------------------------------------------------------------------------- EVENTS
function  onServerInit()
{
	serverSetDefaultMap("mise13-zradce");
	print("Starting RPG Module...");
	serverSetGamemodeName("Real-Life");
	print(areas[0][4]);
}
function onPlayerConnect(ID)
{
	playerChangeSkin(ID,peds[rand()%peds.len()]);
	sendPlayerMessage(ID,red + "====================================");
	sendPlayerMessage(ID,yellow + "Welcome to " + white + "RPG" + red + " Module" + yellow + " server!");
	sendPlayerMessage(ID,yellow + "This is DEV server so few things doesn't work");
	sendPlayerMessage(ID,yellow + "Type " + white + "/help " + yellow + "to get commands.");
	sendPlayerMessage(ID,"");
	sendPlayerMessage(ID,yellow + "There are " + white + serverGetOnlinePlayers() + yellow + " players online.");
	sendPlayerMessage(ID,red + "====================================");
	sendPlayerMessage(ID,cyan+"This GM uses dynamic chat system."+red+" /chat"+cyan+" for info.");
	print("Player "+playerGetName(ID)+"["+ID+"] has been connected to the server.");
	//playerLockControlls(ID);
	//playerHideGui(ID);
	money[ID]=500;

	//showLogin(ID);
}

function onPlayerSpawn(ID)
{
	//sendAllMessage("Player "+playerGetName(ID)+"["+ID+"] has been respawned.");
	playerSetMoney(ID,money[ID]);
	playerToggleCityMusic(ID,1);
	playerSetPosition(ID,-36.08815765,1.421900392,43.2922554);

}
function onPlayerKeyPressed(ID,key)
{
	if(key == KEY_A)
	{
		print("KEY_A pressed");
	}
	//print("Key: "+ID+" - "+key+"\n");

}
function onPlayerDisconnect(ID)
{
	user[ID]=0;
	admin[ID]=0;
	ldriver[ID]=0;
	muted[ID]=0;
	spy[ID]=0;
	wanted[ID]=0;
	god[ID]=0;
	lastpm[ID]=-1;
	radio[ID]=0;
	money[ID]=0;
	print("Player "+playerGetName(ID)+"["+ID+"] has been disconnected from the server.");
}
function onPlayerText(ID,message)
{
	print("["+playerGetName(ID)+"] Message: "+message);	
	//print("IP: "+ playerGetIP(ID));
	local myPos = playerGetPosition(ID);
	for(local i = 0; i<serverGetMaxPlayers();i++){
		if(playerIsConnected(i)){
		local iPos = playerGetPosition(i);
		if(getDistance(iPos[0],myPos[0],iPos[2],myPos[2])<50 && ID!=i && getDistance(iPos[0],myPos[0],iPos[2],myPos[2])!=0){
			local hex = "";
			if(getDistance(iPos[0],myPos[0],iPos[2],myPos[2])<5){hex="ffffff";}
			else if(getDistance(iPos[0],myPos[0],iPos[2],myPos[2])<20){hex="dddddd";}
			else if(getDistance(iPos[0],myPos[0],iPos[2],myPos[2])<35){hex="bbbbbb";}
			else if(getDistance(iPos[0],myPos[0],iPos[2],myPos[2])<45){hex="777777";}
			else if(getDistance(iPos[0],myPos[0],iPos[2],myPos[2])<50){hex="333333";}
			if(admin[ID] == 1) 
			{
				sendPlayerMessage(i,"#"+hex+"[" + red + playerGetName(ID) +"#" + hex + "] #" + hex + message + white);
			}
			else
			{
				sendPlayerMessage(i,"#"+hex+"[" + playerGetName(ID) + "] #" + hex + message + white);
			}
		}
	}
	}
	if(admin[ID] == 1) 
	{
		sendPlayerMessage(ID,white+"[" + red + playerGetName(ID) + white + "] " + message + white);
	}
	else
	{
		sendPlayerMessage(ID,white+"[" + playerGetName(ID) + "] " + message + white);
	}
	return false;
}

function onServerTick(ticks){
	if(serverGetOnlinePlayers()==0){return;}
	for (local i = 0; i < serverGetMaxPlayers(); ++i)
	{
		if(!playerIsConnected(i)){continue;}
		srand(time());
		//playerSetMoney(i,money[i]);
		if(god[i]){
			playerSetHealth(i,200.0f);
		}
		
		/*if(playerInVehicleID(i)!=-1){
			local xyz = vehicleGetPosition(playerInVehicleID(i));
			//playerSetPosition(i,xyz[0],xyz[1],xyz[2]);
			//playerSetMoney(i,vehicleGetSpeed(playerInVehicleID(i)),"");
		}*/
		
	}
}

function onPlayerEnterVehicle(ID,vehID,seatID)
{
	//sendPlayerMessage(ID,"[Script] Player "+playerGetName(ID)+"["+ID+"] entered vehicle "+vehID+
	//" on seat "+seatID+".");
	playerToggleCityMusic(ID,radio[ID]);
	if(ldriver[ID]==0 && seatID==0){
		sendPlayerMessage(ID,red+"Be warned! You are driving without a drive licence!");
	}
}
function onPlayerExitVehicle(ID,vehID)
{
	playerToggleCityMusic(ID,1);
	//if(auta[ID]==1){auta[ID]=0; vehicleDelete(vehID);}
	//sendPlayerMessage(ID,"[Script] Player "+playerGetName(ID)+"["+ID+"] exited vehicle "+vehID);
}

function onPlayerIsKilled(ID,killerID){
	if(ID!=killerID){
		//playerPlaySound(ID,"13940056.wav");
		//Steal money!
		if(wanted[ID]<wanted[killerID]){
			if(wanted[killerID]>3){return;}
			wanted[killerID]++;
			sendPlayerMessage(killerID,red+"Your wanted level has been increased!");
		}

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
function casinoHelp(ID){
	sendPlayerMessage(ID,red + "====================================");
	sendPlayerMessage(ID,yellow + "List of available commands for /casino :");
	sendPlayerMessage(ID,"/casino roll [amount] " + yellow + "- roll a dice! Get Double!");
	sendPlayerMessage(ID,red + "====================================");
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
			sendPlayerMessage(ID,"/balance " + yellow + "- your money balance!");
			sendPlayerMessage(ID,"/pm [id] [message] " + yellow + "- send SMS!");
			sendPlayerMessage(ID,"/anim [id] " + yellow + "- Let's do some animation :)");
			sendPlayerMessage(ID,"/setlines [amount] " + yellow + "- Sets the amount of chat lines (starts from 6)");
			//sendPlayerMessage(ID,"/tplist " + yellow + "- Shows you list of available teleports");
			sendPlayerMessage(ID,"/kill " + yellow + "- Get yourself killed!");
			sendPlayerMessage(ID,"/info " + yellow + "- Shows you info about server");
			sendPlayerMessage(ID,"/radio " + yellow + "- Toggles car radio");
			sendPlayerMessage(ID,"/quit " + yellow + "- quit's the game");
}

function showLogin(ID){
	if(playerGetName(ID)==ID){
		sendPlayerMessage(ID,red+"Type in /log [password] to login.");
	}
	else{
		sendPlayerMessage(ID,red+"Type in /reg [password] to register.");	
	}
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
	user[ID]=1;
	print("["+playerGetName(ID)+"] Command: "+message+" - "+params);
	if(!user[ID] && message != "reg"){
		sendPlayerMessage(ID,red+"You must be logged in to perform this command!");
		showLogin();
		return;
	} else if(!user[ID] && message == "reg"){
		user[ID]=1;
		money[ID]=500;
		name[ID]=playerGetName(ID);
		pwd[ID]=params;
		//playerUnlockControlls(ID);
		sendPlayerMessage(ID,green+"Successfully registered!");
		return;
	} else if(!user[ID] && message == "log"){
		if(params==pwd[ID] && playerGetName(ID)==name[ID]){
			
			user[ID]=1;
		//	playerUnlockControlls(ID);
			sendPlayerMessage(ID,green+"Successfully logged in!");
		}
	}
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
			if(radio[ID]==1){
				radio[ID]=0;
				sendPlayerMessage(ID,green+"Radio turned on.");
			}else{
				radio[ID]=1;
				sendPlayerMessage(ID,green+"Radio turned off.");
			}

			if(playerInVehicleID(ID)!=-1){playerToggleCityMusic(ID,radio[ID]);}
			break;
		case "balance":
			sendPlayerMessage(ID,green+"Balance: "+money[ID]+"$.");
			//playerSetMoney(ID,money[ID],"$");
			break;
		case "peat":
			playerSetPosition(ID, 51.34225, 5.124118, 112.0351);
			break;
		case "ahelp":
			if(admin[ID]==1){
			sendPlayerMessage(ID,red + "====================================");
			sendPlayerMessage(ID,yellow + "List of available admin commands:");
			sendPlayerMessage(ID,"/delcars " + yellow + "- Removing all cars from map");
			sendPlayerMessage(ID,"/spy " + yellow + "- Toggles Spy mode (can see PMs).");
			sendPlayerMessage(ID,"/kick [id] " + yellow + "- Kicking player by ID");
			sendPlayerMessage(ID,"/getip [id] " + yellow + "- Get player IP by ID");
			sendPlayerMessage(ID,red + "====================================");}
			else{sendPlayerMessage(ID,red + "No privileges!");}
			break;
		case "me":
			sendAllMessage("#663399*" + playerGetName(ID) + " " + params);
			break;
		case "skusam":
			//playerSetMoney(ID,2500,"$");
			sendAllMessage(playerGetMoney(ID).tostring());
			break;
		case "pm":
			if(money[ID]<1){sendPlayerMessage(ID,red+"You don't have enough money for sending SMS!"); break;}
			local args = split(params," ");
			if(ID==args[0].tointeger()){break;}
			
			local msg = "";
			for(local i = 1; i<args.len();i++){
				msg = msg + " " + args[i];
			}
			sendPlayerMessage(args[0].tointeger(),orange+"["+playerGetName(ID)+"]->Me: "+msg);
			sendPlayerMessage(ID,orange+"Me->["+playerGetName(args[0].tointeger())+"]: "+msg);
			for(local i = 0; i<serverGetMaxPlayers();i++){
				if(spy[i]==1 && ID!=i && admin[args[0].tointeger()] == 0){
					sendPlayerMessage(ID,orange+"["+playerGetName(ID)+"]["+playerGetName(args[0].tointeger())+"]: "+msg);
				}
			}
			money[ID]--;
			//playerSetMoney(ID,money[ID],"$");
			lastpm[args[0].tointeger()]=ID;
			break;
		case "r":
			if(money[ID]<1){sendPlayerMessage(ID,red+"You don't have enough money for sending SMS!"); break;}
			
			if(lastpm[ID]==-1){break;}
			local msg = params;
			sendPlayerMessage(lastpm[ID],orange+"["+playerGetName(ID)+"]->Me: "+msg);
			sendPlayerMessage(ID,orange+"Me->["+playerGetName(lastpm[ID])+"]: "+msg);
			for(local i = 0; i<serverGetMaxPlayers();i++){
				if(spy[i]==1 && ID!=i && admin[lastpm[ID]] == 0){
					sendPlayerMessage(ID,orange+"["+playerGetName(ID)+"]["+playerGetName(args[0].tointeger())+"]: "+msg);
				}
			}
			money[ID]--;
			//playerSetMoney(ID,money[ID],"$");
			lastpm[lastpm[ID]]=ID;
			break;
		case "spy":
			if(admin[ID]==0){break;}
			if(spy[ID]==0){
				spy[ID]=1;
				sendPlayerMessage(ID,yellow+"Spy mode enabled!");
			} else {
				spy[ID]=0;
				sendPlayerMessage(ID,yellow+"Spy mode disabled!");
			}
			break;
		case "mute":
			if(admin[ID]==1){
				muted[params.tointeger()]=1;
				sendPlayerMessage(ID,"Phone blocked!");
				sendPlayerMessage(params.tointeger(),red+"Your phone has been blocked by ISP!");
			}
			break;
		case "unmute":
			if(admin[ID]==1){
				muted[params.tointeger()]=0;
				sendPlayerMessage(ID,"Phone unblocked!");
				sendPlayerMessage(params.tointeger(),green+"Your phone has been unblocked by ISP!");
			}
			break;
		case "nametag":
			guiToggleNametag(ID,params.tointeger());
			break;
		case "t":
			if(money[ID]<10){sendPlayerMessage(ID,red+"You don't have enough money for posting a message!"); break;}
			if(muted[ID]==1){sendPlayerMessage(ID,red+"You can't message from blocked phone!"); break;}
			if(admin[ID] == 1) 
			{
				sendAllMessage(cyan+"[" + red + playerGetName(ID) + cyan + "] posts: " + cyan + params + white);
			}
			else
			{
				sendAllMessage(cyan+"[" + playerGetName(ID) + "] posts: " + cyan + params + white);
			}
			money[ID]-=10;
			break;
		case "range":
			local myPos = playerGetPosition(ID);
			sendPlayerMessage(ID,"People in range:")
			for(local i = 0; i<serverGetOnlinePlayers();i++){
				local iPos = playerGetPosition(i);
				if(getDistance(iPos[0],myPos[0],iPos[2],myPos[2])<50 && ID!=i){
					sendPlayerMessage(ID,"+ "+playerGetName(i)+" -- Distance: "+getDistance(iPos[0],myPos[0],iPos[2],myPos[2]));
				}
			}
			break;
		case "rand":
			sendPlayerMessage(ID,"Random: "+rand()%params.tointeger());
			break;
		case "casino":
			local args = split(params," ");
			if(args.len()==0){casinoHelp(ID);break;}
			switch(args[0]){
				case "roll":
				if(args[1]=="0" || args[1].tointeger()>money[ID] || args[1].tointeger() < 0){sendPlayerMessage(ID,red+"You don't have much money!");}
					local rnd = rand() % 9;
					if(rnd>5){
						money[ID]+=args[1].tointeger()*2;
						sendPlayerMessage(ID,green+"You won "+args[1].tointeger()*2+"$!");
						if(args[1].tointeger()*2 >= 5000){sendAllMessage(green+"Player "+playerGetName(ID)+" just won "+args[1].tointeger()*2+"$ in casino!");}
					}
					else{
						money[ID]+=-args[1].tointeger();
						sendPlayerMessage(ID,red+"You lose "+args[1].tointeger()+"$!");
					}
					break;
					default:
						casinoHelp(ID);
						break;
			}
			break;
		case "chat":
			sendPlayerMessage(ID,red + "====================================");
			sendPlayerMessage(ID,yellow + "CHAT Overview");
			sendPlayerMessage(ID,yellow+"Your chat message will be displayed only");
			sendPlayerMessage(ID,yellow+"for people within range of 50 meters.");
			sendPlayerMessage(ID,yellow+"You can also send global message by typing:");			
			sendPlayerMessage(ID,cyan+"/t [message]"+yellow+" - it will cost 10$.");
			sendPlayerMessage(ID,cyan+"/pm [ID] [message]"+yellow+" - sends SMS to citizen. Costs: 1$.");
			sendPlayerMessage(ID,red + "====================================");
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
		case "god":
			if(admin[ID]==1){
				god[ID] = !god[ID];
				sendPlayerMessage(ID,"God mode toggled!");
			}
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
			if(admin[ID]==0){break;}
			if (params.tointeger() > 303){sendPlayerMessage(ID,red + "Wrong ID!");}
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
			if(admin[ID]==0){break;}
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
			
		case "roof":
			local test = playerInVehicleID(ID);
			vehicleToggleRoof(test, params.tointeger());
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
		case "addcar":
			//if(admin[ID]==0){break;} // Admin Only
			local pos = playerGetPosition(ID);
			if (params.tointeger() > 169){sendPlayerMessage(ID,red + "Wrong ID!");}
			else{carcount++;
			vehicleSpawn(params.tointeger(),pos[0] +2,pos[1],pos[2],-0.006,0.003,-0.99);}
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
			sendPlayerMessage(ID,yellow + "Author: "+white+"ZaKlaus and Cukier");
			sendPlayerMessage(ID,yellow + "Admin: " + white + "Cukier, ZaKlaus");
			sendPlayerMessage(ID,yellow + "Contact: " + white + "lordcukier@gmail.com or zaklaus@aurora-net.eu");
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