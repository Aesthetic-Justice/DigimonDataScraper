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
std::vector<std::string>MoveList_Links;

//Stores nodes to return to
GumboNode* HomeNode;
GumboNode* ListNode;

//Bools used for HeaderChecks
bool isDGVreq = 0;
bool isEVf = 0;
bool isEVt = 0;
unsigned int Digimon_Count;

CURL *image;
CURLcode imgresult;
FILE *fp{NULL};
std::string variable_file_address;

//Used to track where the functions find things
std::vector<int>Digimon_Xpath;

//Where the data is stored
std::vector<std::vector<int>>XpathDetails = {
	{3,	7,	3,	5,	1,	1,	0,	3}, //Name				0
	{3,	7,	3,	5,	1,	1,	2,	3},	//Stage				1
	{3,	7,	3,	5,	1,	1,	4,	3},	//Attribute			2
	{3,	7,	3,	5,	1,	1,	6,	3},	//Type				3
	{3,	15,	1,	3,	1,	1,	0},		//SupportSkill_Name 4
	{3,	7,	3,	5,	1,	1,	8,	3},	//Memory_Usage		5
	{3,	7,	3,	5,	1,	1,	10,	3},	//Equipment_Slot	6
	{3,	11,	0}						//Description_Text	7
	
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
	{5,	1,	1,	1,	12,	7}			//SPD50				11
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
	std::string DGV_AdReqs = "None";

	//Digivolves from
	std::vector<std::string> DeDigivolve_Options{"None"};

	//Digivolves to
	std::vector<std::string> Digivolve_Options{ "None" };

	//Movelist
	std::vector<std::string> MoveList{ "None" };
};

struct MoveArray
{
	std::string Name{};
	int Level{};
};

struct Move
{
	std::string Name{};
	std::string Attribute{};
	std::string Type{};
	int SP_Cost{};
	int Power{};
	bool Inheritable{false};
	std::string Description{};
	std::vector<MoveArray>LearnList;
};

std::vector<Digimon_DataAsset>Digimon_Database;
std::vector<Move>MoveList_Database;

std::vector<std::vector<int>>XpathMovelist = {
	{2,	8,	3,	1,	4,	1,	0},//Name(Remove last 11 characters)
	{2,	8,	3,	1,	3,	7,	1,	5,	1,	1,	0,	3,	0},//Attribute
	{2,	8,	3,	1,	3,	7,	1,	5,	1,	1,	2,	3,	0},//Type
	{2,	8,	3,	1,	3,	7,	1,	5,	1,	1,	4,	3,	0},//SP_Cost
	{2,	8,	3,	1,	3,	7,	1,	5,	1,	1,	6,	3,	0},//Power
	{2,	8,	3,	1,	3,	7,	1,	5,	1,	1,	8,	1,	0},//Inheritable(Either says Special Move or Inherited Skill)
	{2,	8,	3,	1,	3,	11,	1,	0},//Description

	//Learn List
	{2,	8,	3,	1,	5,	1,	1,	3,	1,	3,	0},//Name(increase 3rd last digit by 2)
	{2,	8,	3,	1,	5,	1,	1,	3,	1,	5,	0}//Level(increase 3rd last digit by 2)
};

std::string ImageLink_Digimon{"https://www.grindosaur.com/img/games/digimon-story-cyber-sleuth/icons/"};

std::vector<std::string>Digimon_WebLinks{
"https://static.wikia.nocookie.net/digimon/images/a/a5/Kuramon_dl.png/revision/latest?cb=20200920035928",
"https://static.wikia.nocookie.net/digimon/images/c/ca/Pabumon_dl.png/revision/latest?cb=20200920040014",
"https://static.wikia.nocookie.net/digimon/images/d/de/Punimon_dl.png/revision/latest?cb=20180823193449",
"https://static.wikia.nocookie.net/digimon/images/d/dc/Botamon_dl.png/revision/latest?cb=20200920040046",
"https://static.wikia.nocookie.net/digimon/images/b/bd/Poyomon_dl.png/revision/latest?cb=20200920040112",
"https://static.wikia.nocookie.net/digimon/images/2/2e/Arcadiamon_%28In-Training%29_dscshm.png/revision/latest?cb=20180207171309",
"https://static.wikia.nocookie.net/digimon/images/3/3e/Koromon_dl.png/revision/latest?cb=20180829063022",
"https://static.wikia.nocookie.net/digimon/images/c/c7/Tanemon_dl.png/revision/latest?cb=20210715000303",
"https://static.wikia.nocookie.net/digimon/images/e/eb/Tsunomon_dl.png/revision/latest?cb=20210715005744",
"https://static.wikia.nocookie.net/digimon/images/2/20/Tsumemon_dl.png/revision/latest?cb=20210714230207",
"https://static.wikia.nocookie.net/digimon/images/9/9d/Tokomon_dl.png/revision/latest?cb=20210714233736",
"https://static.wikia.nocookie.net/digimon/images/8/84/Nyaromon_dl.png/revision/latest?cb=20210715004455",
"https://static.wikia.nocookie.net/digimon/images/8/8a/Pagumon_dl.png/revision/latest?cb=20210714231515",
"https://static.wikia.nocookie.net/digimon/images/0/0c/Yokomon_dl.png/revision/latest?cb=20210714224933",
"https://static.wikia.nocookie.net/digimon/images/4/4f/Bukamon_dl.png/revision/latest?cb=20210714220803",
"https://static.wikia.nocookie.net/digimon/images/6/6c/Motimon_dl.png/revision/latest?cb=20210714234844",
"https://static.wikia.nocookie.net/digimon/images/4/4a/Wanyamon_dl.png/revision/latest?cb=20210714221906",
"https://static.wikia.nocookie.net/digimon/images/d/d6/Agumon_dl.png/revision/latest?cb=20190524001133",
"https://static.wikia.nocookie.net/digimon/images/9/9c/Agumon_%28Black%29_dl.png/revision/latest?cb=20190601092101",
"https://static.wikia.nocookie.net/digimon/images/8/81/Arcadiamon_%28Rookie%29_dscshm.png/revision/latest?cb=20180207171622",
"https://static.wikia.nocookie.net/digimon/images/d/d4/Armadillomon_dl.png/revision/latest?cb=20190528130926",
"https://static.wikia.nocookie.net/digimon/images/f/f9/Impmon_dl.png/revision/latest?cb=20180830071659",
"https://static.wikia.nocookie.net/digimon/images/5/53/Elecmon_dl.png/revision/latest?cb=20180829054435",
"https://static.wikia.nocookie.net/digimon/images/5/57/Otamamon_dl.png/revision/latest?cb=20180829064029",
"https://static.wikia.nocookie.net/digimon/images/0/0d/Gaomon_dl.png/revision/latest?cb=20190523220410",
"https://static.wikia.nocookie.net/digimon/images/3/3e/Gazimon_dl.png/revision/latest?cb=20190524001240",
"https://static.wikia.nocookie.net/digimon/images/6/6b/Gabumon_dl.png/revision/latest?cb=20190524001208",
"https://static.wikia.nocookie.net/digimon/images/5/53/Gabumon_%28Black%29_dl.png/revision/latest?cb=20190601092304",
"https://static.wikia.nocookie.net/digimon/images/b/ba/Guilmon_dl.png/revision/latest?cb=20190523213459",
"https://static.wikia.nocookie.net/digimon/images/1/1c/Kudamon_dl.png/revision/latest?cb=20180824014103",
"https://static.wikia.nocookie.net/digimon/images/c/c6/Keramon_dl.png/revision/latest?cb=20180829062626",
"https://static.wikia.nocookie.net/digimon/images/8/88/Gotsumon_dl.png/revision/latest?cb=20180829061226",
"https://static.wikia.nocookie.net/digimon/images/b/b1/Goblimon_dl.png/revision/latest?cb=20190523220535",
"https://static.wikia.nocookie.net/digimon/images/2/2b/Gomamon_dl.png/revision/latest?cb=20190523220652",
"https://static.wikia.nocookie.net/digimon/images/a/ad/Syakomon_dl.png/revision/latest?cb=20190528213353",
"https://static.wikia.nocookie.net/digimon/images/4/43/Zubamon_dl.png/revision/latest?cb=20180824010112",
"https://static.wikia.nocookie.net/digimon/images/4/4b/Solarmon_dl.png/revision/latest?cb=20190523220504",
"https://static.wikia.nocookie.net/digimon/images/d/d8/Chuumon_dl.png/revision/latest?cb=20201108024903",
"https://static.wikia.nocookie.net/digimon/images/c/cd/Terriermon_dl.png/revision/latest?cb=20180831025141",
"https://static.wikia.nocookie.net/digimon/images/0/0b/Tentomon_dl.png/revision/latest?cb=20180829065145",
"https://static.wikia.nocookie.net/digimon/images/4/4c/ToyAgumon_dl.png/revision/latest?cb=20190522231847",
"https://static.wikia.nocookie.net/digimon/images/0/02/Dracmon_dl.png/revision/latest?cb=20201108025714",
"https://static.wikia.nocookie.net/digimon/images/0/0e/Dracomon_dl.png/revision/latest?cb=20180824005734",
"https://static.wikia.nocookie.net/digimon/images/3/31/Dorumon_dl.png/revision/latest?cb=20190523222009",
"https://static.wikia.nocookie.net/digimon/images/e/eb/Hagurumon_dl.png/revision/latest?cb=20190528212302",
"https://static.wikia.nocookie.net/digimon/images/4/4c/Patamon_dl.png/revision/latest?cb=20190523220733",
"https://static.wikia.nocookie.net/digimon/images/6/65/Hackmon_dl.png/revision/latest?cb=20180829061741",
"https://static.wikia.nocookie.net/digimon/images/c/cd/Palmon_dl.png/revision/latest?cb=20200919100610",
"https://static.wikia.nocookie.net/digimon/images/e/e2/DemiDevimon_dl.png/revision/latest?cb=20190523220608",
"https://static.wikia.nocookie.net/digimon/images/7/7c/Biyomon_dl.png/revision/latest?cb=20190523221409",
"https://static.wikia.nocookie.net/digimon/images/d/d1/Falcomon_dl.png/revision/latest?cb=20210715012913",
"https://static.wikia.nocookie.net/digimon/images/5/56/FanBeemon_dl.png/revision/latest?cb=20190916201549",
"https://static.wikia.nocookie.net/digimon/images/2/22/Veemon_dl.png/revision/latest?cb=20190523220318",
"https://static.wikia.nocookie.net/digimon/images/6/68/Salamon_dl.png/revision/latest?cb=20190522231729",
"https://static.wikia.nocookie.net/digimon/images/1/1c/Betamon_dl.png/revision/latest?cb=20180825193240",
"https://static.wikia.nocookie.net/digimon/images/2/25/Hawkmon_dl.png/revision/latest?cb=20190528130854",
"https://static.wikia.nocookie.net/digimon/images/c/c9/Mushroomon_dl.png/revision/latest?cb=20201108032927",
"https://static.wikia.nocookie.net/digimon/images/6/62/Monodramon_dl.png/revision/latest?cb=20201108032026",
"https://static.wikia.nocookie.net/digimon/images/2/21/Lalamon_dl.png/revision/latest?cb=20190523220437",
"https://static.wikia.nocookie.net/digimon/images/e/e8/Lucemon_dl.png/revision/latest?cb=20190601084947",
"https://static.wikia.nocookie.net/digimon/images/9/9e/Lunamon_dl.png/revision/latest?cb=20201108031628",
"https://static.wikia.nocookie.net/digimon/images/b/bb/Renamon_dl.png/revision/latest?cb=20190521113502",
"https://static.wikia.nocookie.net/digimon/images/d/d2/Lopmon_dl.png/revision/latest?cb=20190528214337",
"https://static.wikia.nocookie.net/digimon/images/3/3c/Wormmon_dl.png/revision/latest?cb=20190520191809",
"https://static.wikia.nocookie.net/digimon/images/c/c9/IceDevimon_dl.png/revision/latest?cb=20210715022109",
"https://static.wikia.nocookie.net/digimon/images/6/60/Icemon_dl.png/revision/latest?cb=20201013170515",
"https://static.wikia.nocookie.net/digimon/images/2/25/Aquilamon_dl.png/revision/latest?cb=20210715023738",
"https://static.wikia.nocookie.net/digimon/images/4/48/Agunimon_dl.png/revision/latest?cb=20190917223745",
"https://static.wikia.nocookie.net/digimon/images/5/5a/Arcadiamon_%28Champion%29_dscshm.png/revision/latest?cb=20180207172526",
"https://static.wikia.nocookie.net/digimon/images/1/14/Ankylomon_dl.png/revision/latest?cb=20180829052031",
"https://static.wikia.nocookie.net/digimon/images/a/a9/Ikkakumon_dl.png/revision/latest?cb=20190524173709",
"https://static.wikia.nocookie.net/digimon/images/2/29/Wizardmon_dl.png/revision/latest?cb=20210715044507",
"https://static.wikia.nocookie.net/digimon/images/4/43/Lobomon_dl.png/revision/latest?cb=20190423235657",
"https://static.wikia.nocookie.net/digimon/images/6/6c/Woodmon_dl.png/revision/latest?cb=20210715050216",
"https://static.wikia.nocookie.net/digimon/images/9/98/Airdramon_dl.png/revision/latest?cb=20200501144044",
"https://static.wikia.nocookie.net/digimon/images/8/81/ExVeemon_dl.png/revision/latest?cb=20190521113447",
"https://static.wikia.nocookie.net/digimon/images/e/e2/Angemon_dl.png/revision/latest?cb=20190522231646",
"https://static.wikia.nocookie.net/digimon/images/c/ce/Ogremon_dl.png/revision/latest?cb=20200920035844",
"https://static.wikia.nocookie.net/digimon/images/d/d5/Guardromon_dl.png/revision/latest?cb=20190224085746",
"https://static.wikia.nocookie.net/digimon/images/d/da/Guardromon_%28Gold%29_dl.png/revision/latest?cb=20190225022248",
"https://static.wikia.nocookie.net/digimon/images/2/24/GaoGamon_dl.png/revision/latest?cb=20190524174227",
"https://static.wikia.nocookie.net/digimon/images/5/56/Kabuterimon_dl.png/revision/latest?cb=20180829062430",
"https://static.wikia.nocookie.net/digimon/images/a/ae/ShellNumemon_dl.png/revision/latest?cb=20210715051050",
"https://static.wikia.nocookie.net/digimon/images/c/c2/Gargomon_dl.png/revision/latest?cb=20180826191055",
"https://static.wikia.nocookie.net/digimon/images/0/02/Garurumon_dl.png/revision/latest?cb=20190524173558",
"https://static.wikia.nocookie.net/digimon/images/1/1c/Garurumon_%28Black%29_dl.png/revision/latest?cb=20210715053306",
"https://static.wikia.nocookie.net/digimon/images/b/b3/Kyubimon_dl.png/revision/latest?cb=20210715055721",
"https://static.wikia.nocookie.net/digimon/images/b/b5/Growlmon_dl.png/revision/latest?cb=20190524234947",
"https://static.wikia.nocookie.net/digimon/images/8/8e/Kurisarimon_dl.png/revision/latest?cb=20180907024259",
"https://static.wikia.nocookie.net/digimon/images/9/91/Greymon_dl.png/revision/latest?cb=20190524173502",
"https://static.wikia.nocookie.net/digimon/images/e/e3/Greymon_%28Blue%29_dl.png/revision/latest?cb=20210715063814",
"https://static.wikia.nocookie.net/digimon/images/b/b3/Clockmon_dl.png/revision/latest?cb=20210715070856",
"https://static.wikia.nocookie.net/digimon/images/6/6a/Kuwagamon_dl.png/revision/latest?cb=20210715080346",
"https://static.wikia.nocookie.net/digimon/images/3/3e/Gekomon_dl.png/revision/latest?cb=20210715081450",
"https://static.wikia.nocookie.net/digimon/images/2/2d/Geremon_dl.png/revision/latest?cb=20210715083909",
"https://static.wikia.nocookie.net/digimon/images/f/f9/Coredramon_%28Blue%29_dl.png/revision/latest?cb=20190225032745",
"https://static.wikia.nocookie.net/digimon/images/4/4f/Coredramon_%28Green%29_dl.png/revision/latest?cb=20190225032747",
"https://static.wikia.nocookie.net/digimon/images/0/0a/GoldNumemon_dl.png/revision/latest?cb=20210715095005",
"https://static.wikia.nocookie.net/digimon/images/c/c5/Golemon_dl.png/revision/latest?cb=20201108030101",
"https://static.wikia.nocookie.net/digimon/images/0/0e/Cyclonemon_dl.png/revision/latest?cb=20200920035908",
"https://static.wikia.nocookie.net/digimon/images/0/02/Sangloupmon_dl.png/revision/latest?cb=20210304040310",
"https://static.wikia.nocookie.net/digimon/images/7/71/Sunflowmon_dl.png/revision/latest?cb=20190524173535",
"https://static.wikia.nocookie.net/digimon/images/9/91/Seadramon_dl.png/revision/latest?cb=20190525000741",
"https://static.wikia.nocookie.net/digimon/images/a/ae/Coelamon_dl.png/revision/latest?cb=20201108025054",
"https://static.wikia.nocookie.net/digimon/images/8/80/GeoGreymon_dl.png/revision/latest?cb=20190524173442",
"https://static.wikia.nocookie.net/digimon/images/6/61/Sukamon_dl.png/revision/latest?cb=20190224082023",
"https://static.wikia.nocookie.net/digimon/images/3/3e/Starmon_dl.png/revision/latest?cb=20210715100859",
"https://static.wikia.nocookie.net/digimon/images/9/9d/Stingmon_dl.png/revision/latest?cb=20200116235136",
"https://static.wikia.nocookie.net/digimon/images/a/ae/Strikedramon_dl.png/revision/latest?cb=20190625213017",
"https://static.wikia.nocookie.net/digimon/images/6/6e/ZubaEagermon_dl.png/revision/latest?cb=20180824222807",
"https://static.wikia.nocookie.net/digimon/images/f/f2/Sorcermon_dl.png/revision/latest?cb=20210715192549",
"https://static.wikia.nocookie.net/digimon/images/6/69/Tankmon_dl.png/revision/latest?cb=20180829065021",
"https://static.wikia.nocookie.net/digimon/images/a/a6/MudFrigimon_dl.png/revision/latest?cb=20190311013418",
"https://static.wikia.nocookie.net/digimon/images/4/42/Tyrannomon_dl.png/revision/latest?cb=20190523214334",
"https://static.wikia.nocookie.net/digimon/images/c/cd/Gatomon_dl.png/revision/latest?cb=20180824025531",
"https://static.wikia.nocookie.net/digimon/images/c/c2/Devimon_dl.png/revision/latest?cb=20210715020700",
"https://static.wikia.nocookie.net/digimon/images/6/64/Turuiemon_dl.png/revision/latest?cb=20190917224314",
"https://static.wikia.nocookie.net/digimon/images/c/c9/Togemon_dl.png/revision/latest?cb=20210715215749",
"https://static.wikia.nocookie.net/digimon/images/8/84/Dorugamon_dl.png/revision/latest?cb=20190524210230",
"https://static.wikia.nocookie.net/digimon/images/f/f9/Nanimon_dl.png/revision/latest?cb=20210715221723",
"https://static.wikia.nocookie.net/digimon/images/1/11/Numemon_dl.png/revision/latest?cb=20210715083816",
"https://static.wikia.nocookie.net/digimon/images/c/cb/Birdramon_dl.png/revision/latest?cb=20190524173620",
"https://static.wikia.nocookie.net/digimon/images/1/1c/BaoHuckmon_dl.png/revision/latest?cb=20191031005418",
"https://static.wikia.nocookie.net/digimon/images/b/bf/Bakemon_dl.png/revision/latest?cb=20210715223330",
"https://static.wikia.nocookie.net/digimon/images/3/39/Veedramon_dl.png/revision/latest?cb=20190523215419",
"https://static.wikia.nocookie.net/digimon/images/8/86/Hudiemon_dl.png/revision/latest?cb=20201108031037",
"https://static.wikia.nocookie.net/digimon/images/0/02/PlatinumSukamon_dl.png/revision/latest?cb=20190224081954",
"https://static.wikia.nocookie.net/digimon/images/6/6d/BlackGatomon_dl.png/revision/latest?cb=20180824025703",
"https://static.wikia.nocookie.net/digimon/images/1/14/Vegiemon_dl.png/revision/latest?cb=20210715233720",
"https://static.wikia.nocookie.net/digimon/images/d/de/Peckmon_dl.png/revision/latest?cb=20210715235942",
"https://static.wikia.nocookie.net/digimon/images/6/64/Meramon_dl.png/revision/latest?cb=20210716010039",
"https://static.wikia.nocookie.net/digimon/images/f/f0/Monochromon_dl.png/revision/latest?cb=20200625224513",
"https://static.wikia.nocookie.net/digimon/images/e/ec/Frigimon_dl.png/revision/latest?cb=20210716021546",
"https://static.wikia.nocookie.net/digimon/images/f/fb/Unimon_dl.png/revision/latest?cb=20201108032431",
"https://static.wikia.nocookie.net/digimon/images/0/0a/Raptordramon_dl.png/revision/latest?cb=20201108032150",
"https://static.wikia.nocookie.net/digimon/images/4/42/Raremon_dl.png/revision/latest?cb=20190917224116",
"https://static.wikia.nocookie.net/digimon/images/9/93/Leomon_dl.png/revision/latest?cb=20190529192502",
"https://static.wikia.nocookie.net/digimon/images/1/1a/Lekismon_dl.png/revision/latest?cb=20200304151242",
"https://static.wikia.nocookie.net/digimon/images/7/71/Reppamon_dl.png/revision/latest?cb=20210716034600",
"https://static.wikia.nocookie.net/digimon/images/4/47/Waspmon_dl.png/revision/latest?cb=20210716040238",
"https://static.wikia.nocookie.net/digimon/images/8/84/MegaKabuterimon_dl.png/revision/latest?cb=20210716042146",
"https://static.wikia.nocookie.net/digimon/images/6/64/Arcadiamon_%28Ultimate%29_dscshm.png/revision/latest?cb=20180207174909",
"https://static.wikia.nocookie.net/digimon/images/f/fa/Antylamon_dl.png/revision/latest?cb=20210618160837",
"https://static.wikia.nocookie.net/digimon/images/b/be/Andromon_dl.png/revision/latest?cb=20190520191232",
"https://static.wikia.nocookie.net/digimon/images/c/c6/Meteormon_dl.png/revision/latest?cb=20201013170934",
"https://static.wikia.nocookie.net/digimon/images/b/b4/Infermon_dl.png/revision/latest?cb=20180824204309",
"https://static.wikia.nocookie.net/digimon/images/6/61/Myotismon_dl.png/revision/latest?cb=20180829063910",
"https://static.wikia.nocookie.net/digimon/images/7/76/Wingdramon_dl.png/revision/latest?cb=20180812003836",
"https://static.wikia.nocookie.net/digimon/images/c/cf/BurningGreymon_dl.png/revision/latest?cb=20190917223916",
"https://static.wikia.nocookie.net/digimon/images/b/be/AeroVeedramon_dl.png/revision/latest?cb=20210716044710",
"https://static.wikia.nocookie.net/digimon/images/0/00/Etemon_dl.png/revision/latest?cb=20190524225301",
"https://static.wikia.nocookie.net/digimon/images/3/35/Angewomon_dl.png/revision/latest?cb=20190521113534",
"https://static.wikia.nocookie.net/digimon/images/2/2f/Okuwamon_dl.png/revision/latest?cb=20210716052828",
"https://static.wikia.nocookie.net/digimon/images/f/fa/Garudamon_dl.png/revision/latest?cb=20190521113421",
"https://static.wikia.nocookie.net/digimon/images/c/c8/KendoGarurumon_dl.png/revision/latest?cb=20190423235739",
"https://static.wikia.nocookie.net/digimon/images/b/b1/Gigadramon_dl.png/revision/latest?cb=20210716065423",
"https://static.wikia.nocookie.net/digimon/images/e/e1/CatchMamemon_dl.png/revision/latest?cb=20201108024244",
"https://static.wikia.nocookie.net/digimon/images/f/fa/CannonBeemon_dl.png/revision/latest?cb=20180831025430",
"https://static.wikia.nocookie.net/digimon/images/a/a4/Groundramon_dl.png/revision/latest?cb=20180824222517",
"https://static.wikia.nocookie.net/digimon/images/8/88/GrapLeomon_dl.png/revision/latest?cb=20190529190943",
"https://static.wikia.nocookie.net/digimon/images/7/70/Grademon_dl.png/revision/latest?cb=20201108030206",
"https://static.wikia.nocookie.net/digimon/images/e/eb/Crescemon_dl.png/revision/latest?cb=20200304151311",
"https://static.wikia.nocookie.net/digimon/images/9/95/Cyberdramon_dl.png/revision/latest?cb=20180829053745",
"https://static.wikia.nocookie.net/digimon/images/3/3e/Shakkoumon_dl.png/revision/latest?cb=20180829064813",
"https://static.wikia.nocookie.net/digimon/images/2/21/Cherrymon_dl.png/revision/latest?cb=20210716214215",
"https://static.wikia.nocookie.net/digimon/images/7/71/Silphymon_dl.png/revision/latest?cb=20180829064637",
"https://static.wikia.nocookie.net/digimon/images/e/e9/SuperStarmon_dl.png/revision/latest?cb=20210624113739",
"https://static.wikia.nocookie.net/digimon/images/d/d5/SkullGreymon_dl.png/revision/latest?cb=20190524234227",
"https://static.wikia.nocookie.net/digimon/images/1/14/SkullSatamon_dl.png/revision/latest?cb=20190917224222",
"https://static.wikia.nocookie.net/digimon/images/8/89/Zudomon_dl.png/revision/latest?cb=20190527055314",
"https://static.wikia.nocookie.net/digimon/images/d/da/SaviorHuckmon_dl.png/revision/latest?cb=20191031005553",
"https://static.wikia.nocookie.net/digimon/images/9/98/Taomon_dl.png/revision/latest?cb=20210716220425",
"https://static.wikia.nocookie.net/digimon/images/5/50/Dragomon_dl.png/revision/latest?cb=20201108025832",
"https://static.wikia.nocookie.net/digimon/images/4/45/Chirinmon_dl.png/revision/latest?cb=20200824130916",
"https://static.wikia.nocookie.net/digimon/images/3/30/Dinobeemon_dl.png/revision/latest?cb=20201108025510",
"https://static.wikia.nocookie.net/digimon/images/c/c1/Digitamamon_dl.png/revision/latest?cb=20190224082807",
"https://static.wikia.nocookie.net/digimon/images/5/5c/SkullMeramon_dl.png/revision/latest?cb=20190530005521",
"https://static.wikia.nocookie.net/digimon/images/c/ce/Duramon_dl.png/revision/latest?cb=20180824222938",
"https://static.wikia.nocookie.net/digimon/images/f/fa/ShogunGekomon_dl.png/revision/latest?cb=20190526051915",
"https://static.wikia.nocookie.net/digimon/images/3/35/Triceramon_dl.png/revision/latest?cb=20200525185042",
"https://static.wikia.nocookie.net/digimon/images/6/66/DoruGreymon_dl.png/revision/latest?cb=20190524210307",
"https://static.wikia.nocookie.net/digimon/images/7/7c/Knightmon_dl.png/revision/latest?cb=20180829062903",
"https://static.wikia.nocookie.net/digimon/images/2/24/Datamon_dl.png/revision/latest?cb=20180829053917",
"https://static.wikia.nocookie.net/digimon/images/9/98/Paildramon_dl.png/revision/latest?cb=20190526190748",
"https://static.wikia.nocookie.net/digimon/images/b/b0/IceLeomon_dl.png/revision/latest?cb=20190529193423",
"https://static.wikia.nocookie.net/digimon/images/6/61/Pandamon_dl.png/revision/latest?cb=20190524075442",
"https://static.wikia.nocookie.net/digimon/images/1/17/Pumpkinmon_dl.png/revision/latest?cb=20180829064338",
"https://static.wikia.nocookie.net/digimon/images/2/2f/Piximon_dl.png/revision/latest?cb=20180825022424",
"https://static.wikia.nocookie.net/digimon/images/d/d2/HippoGryphonmon_dl.png/revision/latest?cb=20200625215930",
"https://static.wikia.nocookie.net/digimon/images/1/18/Phantomon_dl.png/revision/latest?cb=20200117212855",
"https://static.wikia.nocookie.net/digimon/images/b/b3/BlackKingNumemon_dl.png/revision/latest?cb=20210715095203",
"https://static.wikia.nocookie.net/digimon/images/d/d4/BlueMeramon_dl.png/revision/latest?cb=20210716010130",
"https://static.wikia.nocookie.net/digimon/images/b/bc/Vademon_dl.png/revision/latest?cb=20210716222207",
"https://static.wikia.nocookie.net/digimon/images/5/5c/Whamon_dl.png/revision/latest?cb=20210716235211",
"https://static.wikia.nocookie.net/digimon/images/2/26/MagnaAngemon_dl.png/revision/latest?cb=20190529185503",
"https://static.wikia.nocookie.net/digimon/images/e/e8/Volcanomon_dl.png/revision/latest?cb=20201108032716",
"https://static.wikia.nocookie.net/digimon/images/b/b7/Matadormon_dl.png/revision/latest?cb=20210304040437",
"https://static.wikia.nocookie.net/digimon/images/3/34/MachGaogamon_dl.png/revision/latest?cb=20181228080901",
"https://static.wikia.nocookie.net/digimon/images/3/34/Mamemon_dl.png/revision/latest?cb=20200919095916",
"https://static.wikia.nocookie.net/digimon/images/f/f9/MegaSeadramon_dl.png/revision/latest?cb=20210717003302",
"https://static.wikia.nocookie.net/digimon/images/d/de/Megadramon_dl.png/revision/latest?cb=20200824130715",
"https://static.wikia.nocookie.net/digimon/images/a/aa/WarGrowlmon_dl.png/revision/latest?cb=20190526051851",
"https://static.wikia.nocookie.net/digimon/images/4/48/MetalGreymon_dl.png/revision/latest?cb=20210717022847",
"https://static.wikia.nocookie.net/digimon/images/d/d8/MetalGreymon_%28Blue%29_dl.png/revision/latest?cb=20210717022945",
"https://static.wikia.nocookie.net/digimon/images/6/63/MetalTyrannomon_dl.png/revision/latest?cb=20210717032915",
"https://static.wikia.nocookie.net/digimon/images/f/fa/MetalMamemon_dl.png/revision/latest?cb=20200919100022",
"https://static.wikia.nocookie.net/digimon/images/f/fb/Monzaemon_dl.png/revision/latest?cb=20190524225235",
"https://static.wikia.nocookie.net/digimon/images/0/0e/Crowmon_dl.png/revision/latest?cb=20180824233938",
"https://static.wikia.nocookie.net/digimon/images/7/70/RizeGreymon_dl.png/revision/latest?cb=20190524075312",
"https://static.wikia.nocookie.net/digimon/images/b/bc/Lilamon_dl.png/revision/latest?cb=20190524075156",
"https://static.wikia.nocookie.net/digimon/images/e/ec/Rapidmon_dl.png/revision/latest?cb=20190524234208",
"https://static.wikia.nocookie.net/digimon/images/1/11/Lillymon_dl.png/revision/latest?cb=20210716000136",
"https://static.wikia.nocookie.net/digimon/images/f/f7/Lucemon_FM_dl.png/revision/latest?cb=20210717035020",
"https://static.wikia.nocookie.net/digimon/images/f/f3/LadyDevimon_dl.png/revision/latest?cb=20210717074053",
"https://static.wikia.nocookie.net/digimon/images/8/83/WereGarurumon_dl.png/revision/latest?cb=20180829065630",
"https://static.wikia.nocookie.net/digimon/images/0/06/WereGarurumon_%28Black%29_dl.png/revision/latest?cb=20180829065615",
"https://static.wikia.nocookie.net/digimon/images/6/6a/Wisemon_dl.png/revision/latest?cb=20180829065751",
"https://static.wikia.nocookie.net/digimon/images/0/02/Arcadiamon_%28Mega%29_dscshm.png/revision/latest?cb=20180207172908",
"https://static.wikia.nocookie.net/digimon/images/5/5b/Alphamon_dl.png/revision/latest?cb=20180829070009",
"https://static.wikia.nocookie.net/digimon/images/6/6e/UlforceVeedramon_dl.png/revision/latest?cb=20180813002210",
"https://static.wikia.nocookie.net/digimon/images/2/25/Ebemon_dl.png/revision/latest?cb=20210717082500",
"https://static.wikia.nocookie.net/digimon/images/1/14/Imperialdramon_DM_dl.png/revision/latest?cb=20180829062925",
"https://static.wikia.nocookie.net/digimon/images/1/1b/Imperialdramon_FM_dl.png/revision/latest?cb=20180825181757",
"https://static.wikia.nocookie.net/digimon/images/e/ea/Vikemon_dl.png/revision/latest?cb=20200919100132",
"https://static.wikia.nocookie.net/digimon/images/7/7d/Valkyrimon_dl.png/revision/latest?cb=20200911203324",
"https://static.wikia.nocookie.net/digimon/images/9/9e/Varodurumon_dl.png/revision/latest?cb=20210120002612",
"https://static.wikia.nocookie.net/digimon/images/a/a3/VenomMyotismon_dl.png/revision/latest?cb=20180829065315",
"https://static.wikia.nocookie.net/digimon/images/5/5d/WarGreymon_dl.png/revision/latest?cb=20180811192633",
"https://static.wikia.nocookie.net/digimon/images/5/5c/Ophanimon_dl.png/revision/latest?cb=20190524072615",
"https://static.wikia.nocookie.net/digimon/images/7/75/Gaiomon_dl.png/revision/latest?cb=20180829054921",
"https://static.wikia.nocookie.net/digimon/images/2/2e/KaiserGreymon_dl.png/revision/latest?cb=20190917224015",
"https://static.wikia.nocookie.net/digimon/images/a/a4/ChaosGallantmon_dl.png/revision/latest?cb=20190927013739",
"https://static.wikia.nocookie.net/digimon/images/4/4c/Chaosdramon_dl.png/revision/latest?cb=20180812004403",
"https://static.wikia.nocookie.net/digimon/images/3/31/Gankoomon_dl.png/revision/latest?cb=20200919100542",
"https://static.wikia.nocookie.net/digimon/images/f/fb/KingEtemon_dl.png/revision/latest?cb=20180824223444",
"https://static.wikia.nocookie.net/digimon/images/7/75/Kuzuhamon_dl.png/revision/latest?cb=20210717090709",
"https://static.wikia.nocookie.net/digimon/images/9/98/GranKuwagamon_dl.png/revision/latest?cb=20210717145444",
"https://static.wikia.nocookie.net/digimon/images/5/55/GranDracmon_dl.png/revision/latest?cb=20210304040645",
"https://static.wikia.nocookie.net/digimon/images/9/9d/GroundLocomon_dl.png/revision/latest?cb=20180828013920",
"https://static.wikia.nocookie.net/digimon/images/2/23/Gryphonmon_dl.png/revision/latest?cb=20201108030534",
"https://static.wikia.nocookie.net/digimon/images/6/6c/Craniamon_dl.png/revision/latest?cb=20180829053509",
"https://static.wikia.nocookie.net/digimon/images/6/6d/Kerpymon_%28Black%29_dl.png/revision/latest?cb=20210921025205",
"https://static.wikia.nocookie.net/digimon/images/f/f0/Cherubimon_%28Good%29_dl.png/revision/latest?cb=20181104041548",
"https://static.wikia.nocookie.net/digimon/images/e/e7/Goldramon_dl.png/revision/latest?cb=20200501145428",
"https://static.wikia.nocookie.net/digimon/images/5/56/SaberLeomon_dl.png/revision/latest?cb=20190529195939",
"https://static.wikia.nocookie.net/digimon/images/3/3b/Sakuyamon_dl.png/revision/latest?cb=20180829004413",
"https://static.wikia.nocookie.net/digimon/images/c/cc/Jesmon_dl.png/revision/latest?cb=20191031005909",
"https://static.wikia.nocookie.net/digimon/images/f/f9/ShineGreymon_dl.png/revision/latest?cb=20180813025829",
"https://static.wikia.nocookie.net/digimon/images/4/44/ShineGreymon_BM_dl.png/revision/latest?cb=20180812010128",
"https://static.wikia.nocookie.net/digimon/images/4/42/Justimon_dl.png/revision/latest?cb=20180829062205",
"https://static.wikia.nocookie.net/digimon/images/8/8c/Kentaurosmon_dl.png/revision/latest?cb=20180831025759",
"https://static.wikia.nocookie.net/digimon/images/8/80/Slayerdramon_dl.png/revision/latest?cb=20180824221523",
"https://static.wikia.nocookie.net/digimon/images/4/4a/Seraphimon_dl.png/revision/latest?cb=20180828235347",
"https://static.wikia.nocookie.net/digimon/images/2/26/MegaGargomon_dl.png/revision/latest?cb=20180825184014",
"https://static.wikia.nocookie.net/digimon/images/e/e7/Darkdramon_dl.png/revision/latest?cb=20191111011511",
"https://static.wikia.nocookie.net/digimon/images/c/c6/TigerVespamon_dl.png/revision/latest?cb=20180826020811",
"https://static.wikia.nocookie.net/digimon/images/9/90/Titamon_dl.png/revision/latest?cb=20180828235816",
"https://static.wikia.nocookie.net/digimon/images/d/dd/TyrantKabuterimon_dl.png/revision/latest?cb=20201108014714",
"https://static.wikia.nocookie.net/digimon/images/c/c3/Dianamon_dl.png/revision/latest?cb=20200304151351",
"https://static.wikia.nocookie.net/digimon/images/6/60/Diaboromon_dl.png/revision/latest?cb=20180811193711",
"https://static.wikia.nocookie.net/digimon/images/a/a6/Creepymon_dl.png/revision/latest?cb=20210717152842",
"https://static.wikia.nocookie.net/digimon/images/5/55/Gallantmon_dl.png/revision/latest?cb=20210717154913",
"https://static.wikia.nocookie.net/digimon/images/6/69/Dynasmon_dl.png/revision/latest?cb=20211125032540",
"https://static.wikia.nocookie.net/digimon/images/3/3b/Durandamon_dl.png/revision/latest?cb=20180812223124",
"https://static.wikia.nocookie.net/digimon/images/2/2c/Leopardmon_dl.png/revision/latest?cb=20200513224657",
"https://static.wikia.nocookie.net/digimon/images/5/55/Leopardmon_LM_dl.png/revision/latest?cb=20201106085256",
"https://static.wikia.nocookie.net/digimon/images/5/59/Dorugoramon_dl.png/revision/latest?cb=20200312005839",
"https://static.wikia.nocookie.net/digimon/images/6/64/Neptunemon_dl.png/revision/latest?cb=20200805122521",
"https://static.wikia.nocookie.net/digimon/images/8/8b/HiAndromon_dl.png/revision/latest?cb=20190520191525",
"https://static.wikia.nocookie.net/digimon/images/d/d3/PileVolcamon_dl.png/revision/latest?cb=20201108031444",
"https://static.wikia.nocookie.net/digimon/images/2/2a/Barbamon_dl.png/revision/latest?cb=20210717164145",
"https://static.wikia.nocookie.net/digimon/images/2/25/BanchoLeomon_dl.png/revision/latest?cb=20210717225028",
"https://static.wikia.nocookie.net/digimon/images/d/d9/Piedmon_dl.png/revision/latest?cb=20210717232658",
"https://static.wikia.nocookie.net/digimon/images/a/a1/Puppetmon_dl.png/revision/latest?cb=20210718001424",
"https://static.wikia.nocookie.net/digimon/images/a/a4/PlatinumNumemon_dl.png/revision/latest?cb=20210715095056",
"https://static.wikia.nocookie.net/digimon/images/7/75/BlackWarGreymon_dl.png/revision/latest?cb=20210718004747",
"https://static.wikia.nocookie.net/digimon/images/d/d0/PrinceMamemon_dl.png/revision/latest?cb=20200919100051",
"https://static.wikia.nocookie.net/digimon/images/4/4e/Brakedramon_dl.png/revision/latest?cb=20180812003542",
"https://static.wikia.nocookie.net/digimon/images/6/6d/Plesiomon_dl.png/revision/latest?cb=20180813024439",
"https://static.wikia.nocookie.net/digimon/images/8/82/HerculesKabuterimon_dl.png/revision/latest?cb=20210718011803",
"https://static.wikia.nocookie.net/digimon/images/4/49/Beelzemon_dl.png/revision/latest?cb=20181228081020",
"https://static.wikia.nocookie.net/digimon/images/f/fa/Beelzemon_BM_dl.png/revision/latest?cb=20181228003928",
"https://static.wikia.nocookie.net/digimon/images/9/9e/Belphemon_SM_dl.png/revision/latest?cb=20180828005533",
"https://static.wikia.nocookie.net/digimon/images/2/21/Phoenixmon_dl.png/revision/latest?cb=20210120015820",
"https://static.wikia.nocookie.net/digimon/images/a/ae/Magnadramon_dl.png/revision/latest?cb=20190527193604",
"https://static.wikia.nocookie.net/digimon/images/3/3d/Boltmon_dl.png/revision/latest?cb=20180828005954",
"https://static.wikia.nocookie.net/digimon/images/1/16/MagnaGarurumon_dl.png/revision/latest?cb=20190423235812",
"https://static.wikia.nocookie.net/digimon/images/1/14/MagnaGarurumon_%28SV%29_dl.png/revision/latest?cb=20201108015642",
"https://static.wikia.nocookie.net/digimon/images/1/18/Mastemon_dl.png/revision/latest?cb=20210718042845",
"https://static.wikia.nocookie.net/digimon/images/c/cf/MarineAngemon_dl.png/revision/latest?cb=20210718050457",
"https://static.wikia.nocookie.net/digimon/images/b/bd/Minervamon_dl.png/revision/latest?cb=20180828230509",
"https://static.wikia.nocookie.net/digimon/images/6/65/MirageGaogamon_dl.png/revision/latest?cb=20190524072653",
"https://static.wikia.nocookie.net/digimon/images/6/6b/MirageGaogamon_BM_dl.png/revision/latest?cb=20180824221057",
"https://static.wikia.nocookie.net/digimon/images/9/9d/Machinedramon_dl.png/revision/latest?cb=20210718070302",
"https://static.wikia.nocookie.net/digimon/images/c/cd/Megidramon_dl.png/revision/latest?cb=20200911204018",
"https://static.wikia.nocookie.net/digimon/images/7/77/MetalEtemon_dl.png/revision/latest?cb=20210718070338",
"https://static.wikia.nocookie.net/digimon/images/c/c7/MetalGarurumon_dl.png/revision/latest?cb=20180812223425",
"https://static.wikia.nocookie.net/digimon/images/a/a8/MetalGarurumon_%28Black%29_dl.png/revision/latest?cb=20180826054704",
"https://static.wikia.nocookie.net/digimon/images/6/63/MetalSeadramon_dl.png/revision/latest?cb=20190525001857",
"https://static.wikia.nocookie.net/digimon/images/1/18/Merukimon_dl.png/revision/latest?cb=20201002190017",
"https://static.wikia.nocookie.net/digimon/images/e/ed/RustTyranomon_dl.png/revision/latest?cb=20180828234906",
"https://static.wikia.nocookie.net/digimon/images/2/2b/Leviamon_dl.png/revision/latest?cb=20210718073730",
"https://static.wikia.nocookie.net/digimon/images/5/5d/Lilithmon_dl.png/revision/latest?cb=20210718144143",
"https://static.wikia.nocookie.net/digimon/images/4/4a/Ravemon_dl.png/revision/latest?cb=20180829064505",
"https://static.wikia.nocookie.net/digimon/images/3/3b/Ravemon_BM_dl.png/revision/latest?cb=20180822071948",
"https://static.wikia.nocookie.net/digimon/images/7/7b/LordKnightmon_dl.png/revision/latest?cb=20190524072507",
"https://static.wikia.nocookie.net/digimon/images/3/38/Rosemon_dl.png/revision/latest?cb=20190524072721",
"https://static.wikia.nocookie.net/digimon/images/7/7a/Rosemon_BM_dl.png/revision/latest?cb=20180812223259",
"https://static.wikia.nocookie.net/digimon/images/b/b6/Lotosmon_dl.png/revision/latest?cb=20180829063355",
"https://static.wikia.nocookie.net/digimon/images/9/94/Armageddemon_dl.png/revision/latest?cb=20180811210002",
"https://static.wikia.nocookie.net/digimon/images/9/96/Arcadiamon_%28Ultra%29_dscshm.png/revision/latest?cb=20180207172947",
"https://static.wikia.nocookie.net/digimon/images/7/77/Alphamon_Ouryuken_dl.png/revision/latest?cb=20191229011920",
"https://static.wikia.nocookie.net/digimon/images/9/9d/Imperialdramon_PM_dl.png/revision/latest?cb=20180813044311",
"https://static.wikia.nocookie.net/digimon/images/0/06/Examon_dl.png/revision/latest?cb=20201116043036",
"https://static.wikia.nocookie.net/digimon/images/0/0a/Omegamon_dl.png/revision/latest?cb=20180813035916",
"https://static.wikia.nocookie.net/digimon/images/b/b5/Omegamon_Zwart_dl.png/revision/latest?cb=20181014231053",
"https://static.wikia.nocookie.net/digimon/images/7/70/Chaosmon_dl.png/revision/latest?cb=20200612000243",
"https://static.wikia.nocookie.net/digimon/images/1/1c/Chaosmon_VA_dl.png/revision/latest?cb=20201108024657",
"https://static.wikia.nocookie.net/digimon/images/9/99/Susanoomon_dl.png/revision/latest?cb=20181014231355",
"https://static.wikia.nocookie.net/digimon/images/7/70/Gallantmon_CM_dl.png/revision/latest?cb=20181228081153",
"https://static.wikia.nocookie.net/digimon/images/7/7e/Belphemon_RM_dl.png/revision/latest?cb=20180825150051",
"https://static.wikia.nocookie.net/digimon/images/c/c3/Lucemon_SM_dl.png/revision/latest?cb=20180829062510",
"https://static.wikia.nocookie.net/digimon/images/e/e9/Flamedramon_dl.png/revision/latest?cb=20210618160507",
"https://static.wikia.nocookie.net/digimon/images/a/a7/Magnamon_dl.png/revision/latest?cb=20190525190555",
"https://static.wikia.nocookie.net/digimon/images/6/69/Rapidmon_%28Gold%29_dl.png/revision/latest?cb=20210718213821",
"https://static.wikia.nocookie.net/digimon/images/9/90/Shoutmon_dl.png/revision/latest?cb=20180824220251",
"https://static.wikia.nocookie.net/digimon/images/a/a8/OmniShoutmon_dl.png/revision/latest?cb=20180824215450",
"https://static.wikia.nocookie.net/digimon/images/9/9e/Ryudamon_dl.png/revision/latest?cb=20190127084013",
"https://static.wikia.nocookie.net/digimon/images/2/2b/Ginryumon_dl.png/revision/latest?cb=20190127084043",
"https://static.wikia.nocookie.net/digimon/images/e/e4/Hisyaryumon_dl.png/revision/latest?cb=20190127084110",
"https://static.wikia.nocookie.net/digimon/images/a/ac/Ouryumon_dl.png/revision/latest?cb=20190127084138",
"https://static.wikia.nocookie.net/digimon/images/8/84/Apocalymon_dl.png/revision/latest?cb=20180927203832",
"https://static.wikia.nocookie.net/digimon/images/8/86/Sistermon_Blanc_dl.png/revision/latest?cb=20201108012945",
"https://static.wikia.nocookie.net/digimon/images/3/33/Sistermon_Blanc_%28Awakened%29_dl.png/revision/latest?cb=20201108013712",
"https://static.wikia.nocookie.net/digimon/images/e/e9/Sistermon_Noir_dl.png/revision/latest?cb=20201108021547",
"https://static.wikia.nocookie.net/digimon/images/7/7a/Sistermon_Noir_%28Awakened%29_dl.png/revision/latest?cb=20201108003456",
"https://static.wikia.nocookie.net/digimon/images/1/1c/Sistermon_Ciel_dl.png/revision/latest?cb=20201108012758",
"https://static.wikia.nocookie.net/digimon/images/0/01/Sistermon_Ciel_%28Awakened%29_dl.png/revision/latest?cb=20201108013934",
"https://static.wikia.nocookie.net/digimon/images/e/ed/Alphamon_NX_dscshm.png/revision/latest?cb=20180823235423",
"https://static.wikia.nocookie.net/digimon/images/1/15/Crusadermon_NX_dscshm.png/revision/latest?cb=20180824000154",
"https://static.wikia.nocookie.net/digimon/images/6/64/Leopardmon_NX_dscshm.png/revision/latest?cb=20180823235759",
"https://static.wikia.nocookie.net/digimon/images/0/07/Omnimon_NX_dscshm.png/revision/latest?cb=20180823235937",
"https://static.wikia.nocookie.net/digimon/images/1/1d/Gallantmon_NX_dscshm.png/revision/latest?cb=20180823235539",

};