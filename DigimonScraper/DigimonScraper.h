#pragma once

//Functions
void Search_for_Links(GumboNode* node);
void Catalog_Links(GumboNode* node);

void Dig_for_Main(GumboNode* node);
void Dig_Prep(GumboNode* node);
void Dig_Loop(GumboNode* node);
void Dig_Targeted(GumboNode* node);

//Stores links
std::vector<std::string>Digimon_List;

//Stores nodes to return to
GumboNode* HomeNode;
GumboNode* ListNode;

//Bools used for HeaderChecks
bool isDGVreq = 0;
bool isEVf = 0;
bool isEVt = 0;
unsigned int Digimon_Count;

//Used to track where the functions find things
std::vector<int>Digimon_Xpath;

//Where all the Data is stored on the page
std::vector<std::vector<int>>Xpathline = {
	{3,	7,	3,	5,	1,	1,	0,	3}, //Name				0
	{3,	7,	3,	5,	1,	1,	2,	3},	//Stage
	{3,	7,	3,	5,	1,	1,	4,	3},	//Attribute
	{3,	7,	3,	5,	1,	1,	6,	3},	//Type
	{3,	7,	3,	5,	1,	1,	8,	3},	//Memory_Usage
	{3,	7,	3,	5,	1,	1,	10,	3},	//Equipment_Slot
	{3,	11,	0},						//Description_Text
	{3,	13,	1,	3,	1,	1,	0},		//SupportSkill_Name 7
	{5,	1,	1,	1,	2,	3},			//HP1
	{5,	1,	1,	1,	2,	7},			//HP50
	{5,	1,	1,	1,	2,	9},			//HP99
	{5,	1,	1,	1,	4,	3},			//SP1
	{5,	1,	1,	1,	4,	7},			//SP50
	{5,	1,	1,	1,	4,	9},			//SP99
	{5,	1,	1,	1,	6,	3},			//ATK1
	{5,	1,	1,	1,	6,	7},			//ATK50
	{5,	1,	1,	1,	6,	9},			//ATK99
	{5,	1,	1,	1,	8,	3},			//INT1
	{5,	1,	1,	1,	8,	7},			//INT50
	{5,	1,	1,	1,	8,	9},			//INT99
	{5,	1,	1,	1,	10,	3},			//DEF1
	{5,	1,	1,	1,	10,	7},			//DEF50
	{5,	1,	1,	1,	10,	9},			//DEF99
	{5,	1,	1,	1,	12,	3},			//SPD1
	{5,	1,	1,	1,	12,	7},			//SPD50
	{5,	1,	1,	1,	12,	9},			//SPD99				25
	{7,	0,	0,	1,	0,	0},			//DGV_LVL
	{7,	0,	0,	1,	0,	1},			//DGV_ATK
	{7,	0,	0,	1,	0,	2},			//DGV_DEF
	{7,	0,	0,	1,	0,	3},			//DGV_HP
	{7,	0,	0,	1,	0,	4},			//DGV_INT
	{7,	0,	0,	1,	0,	5},			//DGV_SP
	{7,	0,	0,	1,	0,	6},			//DGV_SPD
	{7,	0,	0,	1,	0,	7},			//DGV_ABI
	{7,	0,	0,	1,	0,	8},			//DGV_CAM
	{7,	0,	0,	1,	0,	9},			//DGV_AdReqs
	{9,	0,	0,	1},					//DGV_DOptions		36
	{11,0,	0,	1},					//DGV_Options		37
	{13,0,	0,	1}					//Move_List			38
};


std::vector<std::vector<int>>XpathDetails = {
	{3,	7,	3,	5,	1,	1,	0,	3}, //Name				0
	{3,	7,	3,	5,	1,	1,	2,	3},	//Stage				1
	{3,	7,	3,	5,	1,	1,	4,	3},	//Attribute			2
	{3,	7,	3,	5,	1,	1,	6,	3},	//Type				3
	{3,	15,	1,	3,	1,	1,	0},		//SupportSkill_Name 4
	{3,	7,	3,	5,	1,	1,	8,	3},	//Memory_Usage		5
	{3,	7,	3,	5,	1,	1,	10,	3},	//Equipment_Slot	6
	{3,	11,	0},						//Description_Text	7
	
};

std::vector<std::vector<int>>XpathStats = {
	{5,	1,	1,	1,	2,	3},			//HP1				0
	{5,	1,	1,	1,	2,	7},			//HP50				1
	{5,	1,	1,	1,	4,	3},			//SP1				2
	{5,	1,	1,	1,	4,	7},			//SP50				3
	{5,	1,	1,	1,	6,	3},			//ATK1				4
	{5,	1,	1,	1,	6,	7},			//ATK50				5
	{5,	1,	1,	1,	8,	3},			//INT1				6
	{5,	1,	1,	1,	8,	7},			//INT50				7
	{5,	1,	1,	1,	10,	3},			//DEF1				8
	{5,	1,	1,	1,	10,	7},			//DEF50				9
	{5,	1,	1,	1,	12,	3},			//SPD1				10
	{5,	1,	1,	1,	12,	7},			//SPD50				11
};

std::vector<std::vector<int>>XpathDGV = {
	{0,	0,	1,	0,	0},			//DGV_LVL			0
	{0,	0,	1,	0,	1},			//DGV_ATK			1
	{0,	0,	1,	0,	2},			//DGV_DEF			2
	{0,	0,	1,	0,	3},			//DGV_HP			3
	{0,	0,	1,	0,	4},			//DGV_INT			4
	{0,	0,	1,	0,	5},			//DGV_SP			5
	{0,	0,	1,	0,	6},			//DGV_SPD			6
	{0,	0,	1,	0,	7},			//DGV_ABI			7
	{0,	0,	1,	0,	8},			//DGV_CAM			8
	{0,	0,	1,	0,	9},			//DGV_AdReqs		9
	{0,	0,	1},					//DGV_DOptions		10
	{0,	0,	1},					//DGV_Options		11
	{0,	0,	1}					//Move_List			12
};


/* Exception List;
- Training 1 Digimon do not have Digivolution requirements, incrementing proceeding addresses by 2
- Free Type Digimon Digimon don't have weaknesses, icrementing Description address down by 2(from 15)
- Shoutmon line has no Stage




	0,0,0 = N/A
	0,0,1 = Poyomon						3,5,0,0,7,9
	0,1,0 = N/A
	0,1,1 = N/A
	1,0,0 = Gallantmon NX				3,5,7,0,0,9
	1,0,1 = N/A
	1,1,0 = Imperialdramon				3,5,7,9,0,11
	1,1,1 = ExVeemon					3,5,7,9,11,13
*/

//The Actual Database Stuff
struct Digimon_DataAsset
{
	int Number{};
	std::string Name{};
	std::string Stage{};
	std::string Attribute{};
	std::string Type{};
	int Memory{};
	int Equip_Slots{};
	std::string Description{};
	std::string Support_Skill_Name{};

	//Stats(Base and Scaling)
	double HP_base = 0;
	double HP_scaling = 0;
	double SP_base = 0;
	double SP_scaling = 0;
	double ATK_base = 0;
	double ATK_scaling = 0;
	double DEF_base = 0;
	double DEF_scaling = 0;
	double INT_base = 0;
	double INT_scaling = 0;
	double SPD_base = 0;
	double SPD_scaling = 0;

	//Digivolve Conditions
	int DGV_LVL{};
	int DGV_ATK{};
	int DGV_DEF{};
	int DGV_HP{};
	int DGV_INT{};
	int DGV_SP{};
	int DGV_SPD{};
	int DGV_ABI{};
	int DGV_CAM{};
	std::string DGV_AdReqs = "-";

	//Digivolves from
	std::vector<std::string> DeDigivolve_Options{"None"};

	//Digivolves to
	std::vector<std::string> Digivolve_Options{ "None" };

	//Movelist
	std::vector<std::string> MoveList{ "None" };
};

std::vector<Digimon_DataAsset>Digimon_Database;