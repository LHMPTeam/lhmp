/* Skin Selector by ZaKlaus */
/* LOAD ORDER: LAST */
/* Make sure you load this plugin as last. */

//Variables
local skin = array(serverGetMaxPlayers(),-1);
local skinch = array(serverGetMaxPlayers(),0);
local cyan = "#00FFFF";

//Character DB -- Change as you wish | set to 0 if you want to use internal DB
local peds = [3,4,8,14,16,20,21,23,24,25,26,27,29,30,32,33,37,38,39,40,41,44,46,47,48,49,50,51,55,56,57,58,59,60,61,64,65,66,67,68,69,70,71,72,73,75,76,79,81,88,90,91,92,93,94,95,96,97,99,101,102,103,104,105,106,107,108,110,111,113,114,115,116,118,119,120,121,122,126,129,130,131,133,134,135,136,138,139,140,141,142,143,144,145,146,147,148,151,152,153,160,162,165,167,168,169,171,172,173,174,175,181,182,183,184,185,186,188,189,190,191,192,194,195,196,198,200,201,202,203,204,205,206,207,208,209,210,211,212,213,215,216,217,219,220,221,223,225,227,229,230,231,232,233,235,236,237,239,247,248,249,251,253,254,256,258,260,261,262,264,265,266,267,268,269,270,271,272,273,274,275,277,279,281,282,283,284,285,287,288,290,291,292,294,295,296,34,35,36,127,128,240,241,242,243,244,245,246,52,53,54];

//Events
function onServerInit(){
	print("[PLUGIN] Skin Selector loaded.")
}

function onPlayerKeyPressed(ID,key)
{
	if(key == KEY_A && skinch[ID]!=0){
		if(skin[ID]>0){
			skin[ID]--;
			if(peds.len()>1)
				playerChangeSkin(ID,peds[skin[ID]]);
			else playerChangeSkin(ID,skin[ID]);
		}
	} else if(key == KEY_D  && skinch[ID]!=0){
		if(skin[ID]<peds.len()){
			skin[ID]++;
			if(peds.len()>1)
				playerChangeSkin(ID,peds[skin[ID]]);
			else playerChangeSkin(ID,skin[ID]);
		}
	} else if(key == KEY_F && skinch[ID]!=0){
		skinch[ID] = 0;
		//playerToggleControls(ID,1);
		playerSetCameraToDefault(ID);
		sendPlayerMessage(ID,cyan+"SKIN SELECTOR DISABLED!");
	}
}

function onPlayerCommand(ID,cmd,param){
	if(cmd=="skin"){
		skinch[ID] = 1;
		sendPlayerMessage(ID,cyan+"SKIN SELECTOR ENABLED!");
		sendPlayerMessage(ID,cyan+"Keys: A,D - browse skins; F - accept skin");
		//playerToggleControls(ID,0);
		skin[ID] = playerGetSkin(ID);

		local pos = playerGetPosition(ID);
		local roty = playerGetRotation(ID)[1];
		local x = pos[0] + cos(roty*(3.14/180)) + 2;
		local z = pos[2] + sin(roty*(3.14/180)) + 2;
		playerSetCameraPos(ID,x.tofloat(),pos[1].tofloat(),z.tofloat(),0.0,-roty.tofloat(),0.0);
	} else if(cmd == "noskin"){
		skin[ID]=-1;
		sendPlayerMessage(ID,cyan+"CUSTOM SKIN DISABLED!");
	}
}

function onPlayerSpawn(ID){
	if(skin[ID]!=-1){
		if(peds.len()>1)
			playerChangeSkin(ID,peds[skin[ID]]);
		else playerChangeSkin(ID,skin[ID]);
	}
}

function onPlayerDisconnect(ID)
{
	skin[ID]=-1;
	skinch[ID]=0;
}