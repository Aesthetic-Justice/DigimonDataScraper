// DigimonScraper.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "cpr/cpr.h"
#include "gumbo.h"
#include <vector>
#include "DigimonScraper.h"
#include <string>
#include <stdlib.h>
#include <algorithm>
#pragma warning(disable : 4996)

//Step 1.1: Standardizes where I'm outputing to
std::ofstream writeCsv("DigimonDatabaseCSV.csv");
std::ofstream writeMoves("DigimonDatabaseMovelist.csv");

//Step 1.2: Pulls the desired page
std::string extract_html_page(cpr::Url &url)
{
    cpr::Response response = cpr::Get(url);
    return response.text;
}

//Step 2.1: Dig through HTML for start of rows
void Search_for_Links(GumboNode* node)
{
    if (node->type != GUMBO_NODE_ELEMENT)
        return;

    if (node->v.element.tag == GUMBO_TAG_TBODY)
    {
        Catalog_Links(node);
    }

    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; i++)
    {
        Search_for_Links(static_cast<GumboNode*>(children->data[i]));
    }
}

//Step 2.2: Comb through rows for links
void Catalog_Links(GumboNode* node)
{
    //Sanity Check
    if (node->type != GUMBO_NODE_ELEMENT)
        return;

    //Check for tag
    if (node->v.element.tag == GUMBO_TAG_A)
    {
        //Store attribute
        GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
        //Validity Check
        if (href)
        {
            char letter = 'h';
            std::string link = href->value;
            if (link[0] == letter)
            {
                //Add link to list at open spot
                Digimon_List.push_back(href->value);
            }
        }
    }

    //Loop through all potential children
    GumboVector* children = &node->v.element.children;
    for (unsigned int i = 0; i < children->length; i++)
    {
        Catalog_Links(static_cast<GumboNode*>(children->data[i]));
    }
}

//Step 3.1: Digs through the page to find <main>, where the data is stored
void Dig_for_Main(GumboNode* node)
{
    //if node isn't Gumbo compatible, skip to next node
    if (node->type != GUMBO_NODE_ELEMENT)
        return;

    //dig through useless chaff until we find the core data repository
    if (node->v.element.tag == GUMBO_TAG_MAIN)
    {
            //Creates a savepoint for later reference
            HomeNode = static_cast<GumboNode*>(node);
            
            //Dig_Loop(HomeNode);
            //Initiates Step 3.2(for this Digimon)
            Dig_Prep(HomeNode);
            //Initiates Step 3.3(for this Digimon)
            Dig_Targeted(HomeNode);
    }

    //make a note of each of the node's children
    GumboVector* dfm_children = &node->v.element.children;
    //for each of the node's children, loop this function
    for (unsigned int i = 0; i < dfm_children->length; i++)
    {
        Dig_for_Main(static_cast<GumboNode*>(dfm_children->data[i]));
    }
}

//Step 3.2: Checks the page and makes note of exceptions for use in 3.3
void Dig_Prep(GumboNode* node)
{
    //Resets Dig_Prep's check
    isDGVreq = 0;
    isEVf = 0;
    isEVt = 0;

    //Drops down to the Banner Headers and makes a save point
    node = static_cast<GumboNode*>(node->v.element.children.data[3]);
    node = static_cast<GumboNode*>(node->v.element.children.data[5]);
    node = static_cast<GumboNode*>(node->v.element.children.data[3]);
    GumboNode* PrepNode = node;

    //Checks the Banner Headers for specific banners and makes a note of them to be used in 3rd Loop in Step 3.3
    for (unsigned int i = 1; i < PrepNode->v.element.children.length; i++)
    {

        node = static_cast<GumboNode*>(node->v.element.children.data[i]);
        if (node->type != GUMBO_NODE_ELEMENT)
        {
            node = PrepNode;
            continue;
        }
        node = static_cast<GumboNode*>(node->v.element.children.data[0]);
        if (node->type != GUMBO_NODE_ELEMENT)
        {
            node = PrepNode;
            continue;
        }
        node = static_cast<GumboNode*>(node->v.element.children.data[0]);
        
        //Checks banners
        if (std::string(node->v.text.text).find("Digi") != std::string::npos)
            isDGVreq = 1;

        else if (std::string(node->v.text.text).find("from") != std::string::npos)
            isEVf = 1;

        else if (std::string(node->v.text.text).find("to") != std::string::npos)
            isEVt = 1;

        if (i == 5||i==6)
            ;

        else
            i++;

        node = PrepNode;
    }
}

//Step 3.3: Used to pull data using a specific child directory
void Dig_Targeted(GumboNode* node)
{
    //Digimon_Count starts at zero, but ingame dex starts at 1, this accounts for that
    Digimon_Database.at(Digimon_Count).Number = Digimon_Count+1;

    //Main Loop, goes through XpathDetails vertically
    for (unsigned int i = 0; i < XpathDetails.size(); i++)
    {
        //Sub Loop, goes through XpathDetails horizontally
        for (unsigned int i2 = 0; i2 < XpathDetails[i].size(); i2++)
        {
            node = static_cast<GumboNode*>(node->v.element.children.data[XpathDetails[i][i2]]);
        }

        switch (i)
        {
        case 0:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).Name = node->v.text.text;
            break;
        case 1:
            //Shoutmon && OmniShoutmon don't have a Stage, so this check circumvents that scenario
            if (node->v.element.children.length == 0)
                Digimon_Database.at(Digimon_Count).Stage = "None";
            else
            {
                node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                Digimon_Database.at(Digimon_Count).Stage = node->v.text.text;
            }
            break;
        case 2:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).Attribute = node->v.text.text;
            break;
        case 3:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).Type = node->v.text.text;
            //Neutral Digimon have their Support Skill Name at 3, 13,.. instead of 3,15. This pulls that manually
            if (Digimon_Database.at(Digimon_Count).Type == "Free")
            {
                node = HomeNode;
                node = static_cast<GumboNode*>(node->v.element.children.data[3]);
                node = static_cast<GumboNode*>(node->v.element.children.data[13]);
                node = static_cast<GumboNode*>(node->v.element.children.data[1]);
                node = static_cast<GumboNode*>(node->v.element.children.data[3]);
                node = static_cast<GumboNode*>(node->v.element.children.data[1]);
                node = static_cast<GumboNode*>(node->v.element.children.data[1]);
                node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                node = static_cast<GumboNode*>(node->v.element.children.data[0]);

                Digimon_Database.at(Digimon_Count).Support_Skill_Name = node->v.text.text;
                i++;
            }

            else if (Digimon_Database.at(Digimon_Count).Name == "Kabuterimon")
            {
                Digimon_Database.at(Digimon_Count).Support_Skill_Name = "Numbing Waves";
                i++;
            }

            break;
        case 4:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).Support_Skill_Name = node->v.text.text;
            break;
        case 5:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).Memory = atoi(node->v.text.text);
            break;
        case 6:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).Equip_Slots = atoi(node->v.text.text);
            break;
        case 7:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).Description = node->v.text.text;
            break;
        }
        node = HomeNode;
    }

    //2nd Loop, functions the same as the previous, but pulls stats instead
    for (unsigned int i = 0; i < XpathStats.size(); i++)
    {
        for (unsigned int i2 = 0; i2 < XpathStats[i].size(); i2++)
        {
            node = static_cast<GumboNode*>(node->v.element.children.data[XpathStats[i][i2]]);
        }

        node = static_cast<GumboNode*>(node->v.element.children.data[0]);

        switch (i)
        {
        case 0:
            Digimon_Database.at(Digimon_Count).HP_base = atof(node->v.text.text);
            break;
        case 1:
            Digimon_Database.at(Digimon_Count).HP_scaling = ((atof(node->v.text.text) - Digimon_Database.at(Digimon_Count).HP_base) / 50);
            break;
        case 2:
            Digimon_Database.at(Digimon_Count).SP_base = atof(node->v.text.text);
            break;
        case 3:
            Digimon_Database.at(Digimon_Count).SP_scaling = ((atof(node->v.text.text) - Digimon_Database.at(Digimon_Count).SP_base) / 50);
            break;
        case 4:
            Digimon_Database.at(Digimon_Count).ATK_base = atof(node->v.text.text);
            break;
        case 5:
            Digimon_Database.at(Digimon_Count).ATK_scaling = ((atof(node->v.text.text) - Digimon_Database.at(Digimon_Count).ATK_base) / 50);
            break;
        case 6:
            Digimon_Database.at(Digimon_Count).INT_base = atof(node->v.text.text);
            break;
        case 7:
            Digimon_Database.at(Digimon_Count).INT_scaling = ((atof(node->v.text.text) - Digimon_Database.at(Digimon_Count).INT_base) / 50);
            break;
        case 8:
            Digimon_Database.at(Digimon_Count).DEF_base = atof(node->v.text.text);
            break;
        case 9:
            Digimon_Database.at(Digimon_Count).DEF_scaling = ((atof(node->v.text.text) - Digimon_Database.at(Digimon_Count).DEF_base) / 50);
            break;
        case 10:
            Digimon_Database.at(Digimon_Count).SPD_base = atof(node->v.text.text);
            break;
        case 11:
            Digimon_Database.at(Digimon_Count).SPD_scaling = ((atof(node->v.text.text) - Digimon_Database.at(Digimon_Count).SPD_base) / 50);
            break;
        }
        node = HomeNode;
    }

    //The 3rd loop's first xpath is variable based on which paths exist. It starts at 7 and increments by 2
    int x = 7;
    //3rd Loop, same song and dance
    for (unsigned int i = 0; i < XpathDGV.size(); i++)
    {
        //Used to skip cases where Digimon don't have that specific stat, otherwise the dig loop would error out
        if (i < 10)
        {
            if (isDGVreq == false)
                continue;
        }
        //If the previous skip didn't happen AND this part isn't skipping, increment by 2
        else if (i == 10)
        {
            if (isEVf == false)
                continue;
            else
                if (isDGVreq == true)
                    x += 2;
        }
        //Same as above
        else if (i == 11)
        {
            if (isEVt == false)
                continue;
            else
                if (isEVf == true)
                    x += 2;

        }
        //Movelist always exists, so always increment 2
        else
        {
                x += 2;
        }
        //Dig the first, variable child node
        node = static_cast<GumboNode*>(node->v.element.children.data[x]);

        //Proceed as normal
        for (unsigned int i2 = 0; i2 < XpathDGV[i].size(); i2++)
        {
            node = static_cast<GumboNode*>(node->v.element.children.data[XpathDGV[i][i2]]);
        }

        GumboVector* dt_children = &node->v.element.children;

        switch (i)
        {
        case 0:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).DGV_LVL = atoi(node->v.text.text);
            break;
        case 1:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).DGV_ATK = atoi(node->v.text.text);
            break;
        case 2:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).DGV_DEF = atoi(node->v.text.text);
            break;
        case 3:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).DGV_HP = atoi(node->v.text.text);
            break;
        case 4:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).DGV_INT = atoi(node->v.text.text);
            break;
        case 5:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).DGV_SP = atoi(node->v.text.text);
            break;
        case 6:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).DGV_SPD = atoi(node->v.text.text);
            break;
        case 7:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).DGV_ABI = atoi(node->v.text.text);
            break;
        case 8:
            node = static_cast<GumboNode*>(node->v.element.children.data[0]);
            Digimon_Database.at(Digimon_Count).DGV_CAM = atoi(node->v.text.text);
            break;
        case 9://DGV_AdReqs
        
            //Condition for Jogress/Fusion, Grabs Both
            if (dt_children->length == 3)
            {
                //Wipe Blank
                Digimon_Database.at(Digimon_Count).DGV_AdReqs.clear();

                //Add the Component Digimon
                for (unsigned int i2 = 0; i2 < dt_children->length + 1; i2++)
                {
                    node = static_cast<GumboNode*>(dt_children->data[i2]);

                    GumboVector* dt_children = &node->v.element.children;
                    GumboNode* dt_first_child = static_cast<GumboNode*>(dt_children->data[0]);

                    Digimon_Database.at(Digimon_Count).DGV_AdReqs.append(dt_first_child->v.text.text);
                    if (i2 == 0)
                        Digimon_Database.at(Digimon_Count).DGV_AdReqs.append("|");


                    i2++;
                }
            }

            else
            {
                node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                if (node->v.text.text == "-")
                {
                    break;
                }

                else
                {
                    Digimon_Database.at(Digimon_Count).DGV_AdReqs = node->v.text.text;
                }
            }

            break;
        case 10://DGV_DOptions
            //Default value is none, this clears that
            Digimon_Database.at(Digimon_Count).DeDigivolve_Options.clear();

            //if there are multiple DeDigivolve options...
            if (dt_children->length > 1)
            {
                //for each child of, dig to bottom of Xpath and insert to vector DeDigivolve Options
                for (unsigned int i = 0; i < dt_children->length; i++)
                {
                    node = static_cast<GumboNode*>(dt_children->data[i]);
                    node = static_cast<GumboNode*>(node->v.element.children.data[1]);
                    node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                    node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                    Digimon_Database.at(Digimon_Count).DeDigivolve_Options.push_back(node->v.text.text);
                }
            }

            //Else, grab the singular
            else
            {
                node = static_cast<GumboNode*>(dt_children->data[0]);
                node = static_cast<GumboNode*>(node->v.element.children.data[1]);
                node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                Digimon_Database.at(Digimon_Count).DeDigivolve_Options.push_back(node->v.text.text);
            }

            break;
        case 11://DGV_Options, functionally identical to case 10
            Digimon_Database.at(Digimon_Count).Digivolve_Options.clear();
            if (dt_children->length > 1)
            {
                for (unsigned int i = 0; i < dt_children->length; i++)
                {
                    node = static_cast<GumboNode*>(dt_children->data[i]);
                    node = static_cast<GumboNode*>(node->v.element.children.data[1]);
                    node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                    node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                    Digimon_Database.at(Digimon_Count).Digivolve_Options.push_back(node->v.text.text);
                }
            }

            else
            {
                node = static_cast<GumboNode*>(dt_children->data[0]);
                node = static_cast<GumboNode*>(node->v.element.children.data[1]);
                node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                Digimon_Database.at(Digimon_Count).Digivolve_Options.push_back(node->v.text.text);
            }
            break;

        case 12://Movelist
            //Default value is "None", this clears that
            Digimon_Database.at(Digimon_Count).MoveList.clear();

            //If multiple moves, copy all to MoveList vector
            if (dt_children->length > 1)
            {
                for (unsigned int i = 0; i < dt_children->length; i++)
                {
                    node = static_cast<GumboNode*>(dt_children->data[i]);
                    node = static_cast<GumboNode*>(node->v.element.children.data[1]);
                    node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                    node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                    Digimon_Database.at(Digimon_Count).MoveList.push_back(node->v.text.text);
                }
            }

            //else, copy the singular
            else
            {
                node = static_cast<GumboNode*>(dt_children->data[0]);
                node = static_cast<GumboNode*>(node->v.element.children.data[1]);
                node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                Digimon_Database.at(Digimon_Count).MoveList.push_back(node->v.text.text);
            }
            break;
        }

        

        node = HomeNode;
    }
}

//Search for Xpath children link. Used purely for building the code, not used in final production
void Dig_Loop(GumboNode* node)
{
    if (node->type != GUMBO_NODE_ELEMENT)
    {
        return;
    }
    /*
    if (node->v.element.tag == GUMBO_TAG_TD)
    {
        if (node->v.element.children.length != 0) {
            GumboVector* dl_children = &node->v.element.children;
            GumboNode* dl_first_child = static_cast<GumboNode*>(dl_children->data[0]);

            if (std::string(dl_first_child->v.text.text).size() == 0)
            {
                return;
            }

            writeCsv << dl_first_child->v.text.text;
            for (unsigned int i = 0; i < Digimon_Xpath.size();i++)
            {
                writeCsv << "," << Digimon_Xpath.at(i);
            }

            writeCsv << "\n";
        }
    }
    */

    //Check for links
    if (node->v.element.tag == GUMBO_TAG_A)
    {
        //Store attribute
        GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
        //Validity Check
        if (href)
        {
            char letter = 'h';
            std::string link = href->value;
            if (link[0] == letter)
            {
                std::cout << link << "\n";
                writeCsv << '\"';
                writeCsv << link;
                writeCsv << "\",";
                writeCsv << "\n";
            }
        }
    }

    /*
    //Check for tag
    if (node->v.element.tag == GUMBO_TAG_H2)
    {
        GumboVector* dlA_children = &node->v.element.children;
        GumboNode* dlA_first_child = static_cast<GumboNode*>(dlA_children->data[0]);

        writeCsv << dlA_first_child->v.text.text;
        for (unsigned int i = 0; i < Digimon_Xpath.size();i++)
        {
            writeCsv << "," << Digimon_Xpath.at(i);
        }

        writeCsv << "\n";

    }

    //Check for tag
    if (node->v.element.tag == GUMBO_TAG_P)
    {
        GumboVector* dlA_children = &node->v.element.children;
        GumboNode* dlA_first_child = static_cast<GumboNode*>(dlA_children->data[0]);

        writeCsv << dlA_first_child->v.text.text;
        for (unsigned int i = 0; i < Digimon_Xpath.size();i++)
        {
            writeCsv << "," << Digimon_Xpath.at(i);
        }

        writeCsv << "\n";

    }

    //Check for tag
    if (node->v.element.tag == GUMBO_NODE_TEXT)
    {
     
        writeCsv << node->v.text.text;

        for (unsigned int i = 0; i < Digimon_Xpath.size();i++)
        {
            writeCsv << "," << Digimon_Xpath.at(i);
        }

        writeCsv << "\n";

    }*/

    GumboVector* son = &node->v.element.children;
    for (unsigned int i = 0; i < son->length;i++)
    {
        Digimon_Xpath.push_back(i);
        Dig_Loop(static_cast<GumboNode*>(son->data[i]));
        Digimon_Xpath.pop_back();
    }

}

void Download_Image(int Digimon_Count)
{

//creates a version of the name sanitized for the URL
std::string TempNameStorage = Digimon_Database.at(Digimon_Count).Name;
std::replace(TempNameStorage.begin(), TempNameStorage.end(), ' ', '-');
TempNameStorage.erase(std::remove(TempNameStorage.begin(), TempNameStorage.end(), '('), TempNameStorage.end());
TempNameStorage.erase(std::remove(TempNameStorage.begin(), TempNameStorage.end(), ')'), TempNameStorage.end());
TempNameStorage.erase(std::remove(TempNameStorage.begin(), TempNameStorage.end(), '.'), TempNameStorage.end());
for (int i = 0; i < TempNameStorage.size(); i++)
{
    TempNameStorage[i] = std::tolower(TempNameStorage[i]);
}

int TempNumber = Digimon_Count + 1;
std::string TempNumberStep2 = std::to_string(TempNumber);

//creates variable file address
variable_file_address.append("C:\\Dev\\Repos\\DigimonScraper\\DigimonScraper\\Images\\img_");
if (Digimon_Count + 1 < 10)
    variable_file_address.append("00");
else if (Digimon_Count + 1 < 100)
    variable_file_address.append("0");
variable_file_address.append(TempNumberStep2);
variable_file_address.append(".png");

const char* useable_file_address = variable_file_address.c_str();
const char* useable_image_address = Digimon_WebLinks.at(Digimon_Count).c_str();//Old code used to pull Digimon wiki images

std::cout << useable_image_address << "\n";

image = curl_easy_init();
if (image) {
    char errbuffer[CURL_ERROR_SIZE];
    curl_easy_setopt(image, CURLOPT_ERRORBUFFER, errbuffer);
    errbuffer[0] = 0;

    // Open file 
    fp = fopen(useable_file_address, "wb");
    if (fp == NULL)
        std::cout << "File(fp) cannot be opened";

    curl_easy_setopt(image, CURLOPT_URL, useable_image_address);
    curl_easy_setopt(image, CURLOPT_WRITEFUNCTION, NULL);
    curl_easy_setopt(image, CURLOPT_WRITEDATA, fp);

    // Grab image 
    imgresult = curl_easy_perform(image);
    if (imgresult)
        std::cout << "Cannot grab the image!\n";

    if (imgresult != CURLE_OK)
    {
        size_t len = strlen(errbuffer);
        fprintf(stderr, "\nlibcurl: (%d) ", imgresult);
        if (len)
            fprintf(stderr, "%s%s", errbuffer, ((errbuffer[len - 1] != '\n') ? "\n" : ""));
        else
            fprintf(stderr, "%s\n", curl_easy_strerror(imgresult));
    }
}


// Clean up the resources 
curl_easy_cleanup(image);
// Close the file 
fclose(fp);


variable_file_address.clear();
}

void Output_CSV()
{
    writeCsv << "---,Number,Name,Stage,Attribute,Type,Memory_Usage,Equip_Slots,Description,Support_Skill_Name,Icon_Digimon,Material_Digimon,HP_Base,HP_Scaling,SP_Base,SP_Scaling,ATK_Base,ATK_Scaling,DEF_Base,DEF_Scaling,INT_Base,INT_Scaling,SPD_Base,SPD_Scaling,DGV_LVL,DGV_HP,DGV_SP,DGV_ATK,DGV_DEF,DGV_INT,DGV_SPD,DGV_ABI,DGV_CAM,DGV_AdReqs,DeDGV_Options,DGV_Options,Move_List\n";//CSV Header
    for (Digimon_Count = 0; Digimon_Count < Digimon_List.size(); Digimon_Count++)
    {
        writeCsv << "\"" << Digimon_Database.at(Digimon_Count).Name << "\",";
        writeCsv << "\"" << Digimon_Database.at(Digimon_Count).Number << "\",";
        writeCsv << "\"" << Digimon_Database.at(Digimon_Count).Name << "\",";
        writeCsv << "\"" << Digimon_Database.at(Digimon_Count).Stage << "\",";//the "\"" is used to quote the entry, to protect special characters like " "
        writeCsv << Digimon_Database.at(Digimon_Count).Attribute << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).Type << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).Memory << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).Equip_Slots << ",";
        writeCsv << "\"" << Digimon_Database.at(Digimon_Count).Description << "\",";
        writeCsv << "\"" << Digimon_Database.at(Digimon_Count).Support_Skill_Name << "\",";
        writeCsv << "Texture2D'/Game/MyContent/ArtAssets/img_";
        if (Digimon_Count + 1 < 10)
            writeCsv << "00";
        else if (Digimon_Count + 1 < 100)
            writeCsv << "0";
        writeCsv << (Digimon_Count + 1) << ".img_";
        if (Digimon_Count + 1 < 10)
            writeCsv << "00";
        else if (Digimon_Count + 1 < 100)
            writeCsv << "0";
        writeCsv << (Digimon_Count + 1) << "',";
        writeCsv << "Material'/Game/MyContent/ArtAssets/img_";
        if (Digimon_Count + 1 < 10)
            writeCsv << "00";
        else if (Digimon_Count + 1 < 100)
            writeCsv << "0";
        writeCsv << (Digimon_Count + 1) << "_Mat.img_";
        if (Digimon_Count + 1 < 10)
            writeCsv << "00";
        else if (Digimon_Count + 1 < 100)
            writeCsv << "0";
        writeCsv << (Digimon_Count + 1) << "_Mat',";
        writeCsv << Digimon_Database.at(Digimon_Count).HP_base << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).HP_scaling << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).SP_base << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).SP_scaling << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).ATK_base << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).ATK_scaling << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).DEF_base << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).DEF_scaling << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).INT_base << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).INT_scaling << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).SPD_base << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).SPD_scaling << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_LVL << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_HP << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_SP << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_ATK << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_DEF << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_INT << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_SPD << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_ABI << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_CAM << ",";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_AdReqs << ",";
        writeCsv << "\"(\"";
        for (unsigned int i = 0; i < Digimon_Database.at(Digimon_Count).DeDigivolve_Options.size();i++)
        {
            writeCsv << '\"' << Digimon_Database.at(Digimon_Count).DeDigivolve_Options.at(i) << '\"';
            if (i + 1 < Digimon_Database.at(Digimon_Count).DeDigivolve_Options.size())
                writeCsv << "\",\"";
        }
        writeCsv << "\")\",";

        writeCsv << "\"(\"";
        for (unsigned int i = 0; i < Digimon_Database.at(Digimon_Count).Digivolve_Options.size();i++)
        {
            writeCsv << '\"' << Digimon_Database.at(Digimon_Count).Digivolve_Options.at(i) << '\"';
            if (i + 1 < Digimon_Database.at(Digimon_Count).Digivolve_Options.size())
                writeCsv << "\",\"";
        }
        writeCsv << "\")\",";

        writeCsv << "\"(\"";
        for (unsigned int i = 0; i < Digimon_Database.at(Digimon_Count).MoveList.size();i++)
        {
            writeCsv << '\"' << Digimon_Database.at(Digimon_Count).MoveList.at(i) << '\"';
            if (i + 1 < Digimon_Database.at(Digimon_Count).MoveList.size())
                writeCsv << "\",\"";
        }
        writeCsv << "\")\"\n";

        //prints to console so I can keep track of progress
        std::cout << Digimon_Count + 1 << "...\n";

    }

    //Close CSV
    writeCsv.close();
}

void PullDigimon()
{
    //Part 1: Grab the original database
    //Grab the HTML from the page containing all the links, then convert it to a format Gumbo can use
    cpr::Url Original_URL = cpr::Url{ "https://www.grindosaur.com/en/games/digimon/digimon-story-cyber-sleuth/digimon" };
    std::string page_content = extract_html_page(Original_URL);
    GumboOutput* parsed_response = gumbo_parse(page_content.c_str());

    //Part 2: Grab links to each digimon, creating a list of digimon
    Search_for_Links(parsed_response->root);

    //Prints to console so I can visually progress
    std::cout << "Writing to Database: \n";

    //Part 3: Take the list of digimon URLS and pull from each page and write it to a Database
    for (Digimon_Count = 0; Digimon_Count < Digimon_List.size(); Digimon_Count++)
    {
        //Pull and Parse the HTML, then...
        cpr::Url New_URL = cpr::Url{ Digimon_List[Digimon_Count] };
        page_content = extract_html_page(New_URL);
        GumboOutput* parsed_response = gumbo_parse(page_content.c_str());

        //...Increase Database size by 1, then
        Digimon_Database.push_back(Digimon_DataAsset());

        //...Fill that Digimon Database entry
        Dig_for_Main(parsed_response->root);

        //Prints to console so I can visually progress
        std::cout << Digimon_Count << "...\n";
    }

    //Prints to console so I can visually progress
    std::cout << "Writing to Database Complete \n";

    //SideQuest: Download images
    /*
    for (Digimon_Count = 0; Digimon_Count < Digimon_List.size(); Digimon_Count++)
    {
        Download_Image(Digimon_Count);
    }
    std::cout << "Downloading images complete \n";
    */

    //Part 4: Sanitize the Database for CSV by removing quotation marks from text
    for (Digimon_Count = 0; Digimon_Count < Digimon_List.size(); Digimon_Count++)
    {
        Digimon_Database.at(Digimon_Count).Description.erase(std::remove(Digimon_Database.at(Digimon_Count).Description.begin(), Digimon_Database.at(Digimon_Count).Description.end(), '\"'), Digimon_Database.at(Digimon_Count).Description.end());

        Digimon_Database.at(Digimon_Count).DGV_AdReqs.erase(std::remove(Digimon_Database.at(Digimon_Count).DGV_AdReqs.begin(), Digimon_Database.at(Digimon_Count).DGV_AdReqs.end(), '\"'), Digimon_Database.at(Digimon_Count).DGV_AdReqs.end());
    }

    //Prints to console so I can visually progress
    std::cout << "Printing to CSV: \n";

    //Part 5: Write the Database to CSV
    Output_CSV(); //This outputs to usual CSV

    // free the allocated memory
    gumbo_destroy_output(&kGumboDefaultOptions, parsed_response);
}

void GrabChildren(std::vector<GumboNode*>& Parent)
{
    //Adds children of current node to the end of the list
    GumboVector* Children = &Parent.front()->v.element.children;
    for (int it = 0; it < Children->length;it++) {
        Parent.emplace_back(static_cast<GumboNode*>(Children->data[it]));
    }

    //Remove current node from the list to prevent an infinite loop
    Parent.erase(Parent.begin());
}

std::vector<int> GetXpath(GumboNode*& Root, GumboNode*& Start) {
    GumboNode* CurrentNode = Start;
    std::vector<int> Xpath;
    do {
        for (int it = 0; it < CurrentNode->parent->v.element.children.length;it++) {
            if (CurrentNode->parent->v.element.children.data[it] == CurrentNode) {
                Xpath.emplace(Xpath.begin(), it);
                break;
            }
        }
        CurrentNode = CurrentNode->parent;

    } while (CurrentNode != Root);

    return Xpath;
}

void PullMoveList()
{
    //Part 1: Grab the original database
    //Grab the HTML from the page containing all the links, then convert it to a format Gumbo can use
    cpr::Url Original_URL = cpr::Url{ "https://www.grindosaur.com/en/games/digimon/digimon-story-cyber-sleuth/moves" };
    std::string page_content = extract_html_page(Original_URL);
    GumboOutput* parsed_response = gumbo_parse(page_content.c_str());

    //Part 2: Grab links to each move, and combining them into a container(MoveList_Links)
    {
        //Create a container that'll temporarily store working memory of nodes, starting with the root
        std::vector<GumboNode*>NodeList{parsed_response->root};

        do {
            //Sanity Check
            if (NodeList.front()->type != GUMBO_NODE_ELEMENT) {
                NodeList.erase(NodeList.begin());
                continue;
            }

            //If I hit a hyperlink, store it in 
            if (NodeList.front()->v.element.tag == GUMBO_TAG_MAIN) {
                NodeList.erase(NodeList.begin() + 1,NodeList.end());
                break;
            }

            //then, add the children of the current loop to the list to be checked
            GrabChildren(NodeList);

            //This will loop until all children of all nodes are checked
        } while (!NodeList.empty());

        do {
            //Sanity Check
            if (NodeList.front()->type != GUMBO_NODE_ELEMENT){
                NodeList.erase(NodeList.begin());
                continue;
            }

            //If I hit a hyperlink, store it in 
            if (NodeList.front()->v.element.tag == GUMBO_TAG_A) {
                GumboAttribute* href = gumbo_get_attribute(&NodeList.front()->v.element.attributes, "href");
                if (href) {
                    std::string link = href->value;
                    if (link[0] == 'h'&&link.length()>70)//all links start wtih http and relevant links are longer than 70 char
                    {
                        MoveList_Links.push_back(href->value);
                    }
                }
            }
            
            //then, add the children of the current loop to the list to be checked
            GrabChildren(NodeList);

         //This will loop until all children of all nodes are checked
        } while (!NodeList.empty());
    }

    //Prints to console so I can visually progress
    std::cout << "Writing to Database: \n";

    //Part 3: Take the list of URLs to each move and pull relevant data from each page and print it to a CSV
    /*{
        for(int MoveIterator = 0; MoveIterator < MoveList_Links.size(); MoveIterator++) {
            cpr::Url New_URL = cpr::Url{ MoveList_Links[MoveIterator] };
            std::cout << New_URL << "\n";
            page_content = extract_html_page(New_URL);
            GumboOutput* parsed_response = gumbo_parse(page_content.c_str());

            std::vector<GumboNode*>NodeList{ parsed_response->root };
            MoveList_Database.push_back(Move());
            //std::map<GumboNode*, int>NodeMap;
            //NodeMap.insert(std::pair<GumboNode*,int>(NodeList[0], 1));
            std::vector<int>MoveList_Xpath;

            //DigforMain
            do {
                //Sanity Check
                if (NodeList.front()->type != GUMBO_NODE_ELEMENT)
                    if (NodeList.front()->type != GUMBO_NODE_ELEMENT) {
                        NodeList.erase(NodeList.begin());
                        continue;
                    }

                //Prints out the NodePath to each desired element
                if (NodeList.front()->v.element.tag == GUMBO_TAG_TD || NodeList.front()->v.element.tag == GUMBO_TAG_H2 || NodeList.front()->v.element.tag == GUMBO_TAG_P) {
                    if (NodeList.front()->v.element.children.length != 0) {
                        GumboVector* dl_children = &NodeList.front()->v.element.children;
                        GumboNode* dl_first_child = static_cast<GumboNode*>(dl_children->data[0]);

                        //Guard Clause if blank text
                        if (NodeList.front()->v.element.tag == GUMBO_TAG_TD && std::string(dl_first_child->v.text.text).size() == 0) {
                            GrabChildren(NodeList);
                            continue;
                        }

                        {
                            MoveList_Xpath = GetXpath(parsed_response->root, dl_first_child);

                            writeMoves << dl_first_child->v.text.text;

                            for (int i = 0; i < MoveList_Xpath.size();i++)
                            {
                                writeMoves << "," << MoveList_Xpath.at(i);
                            }

                            writeMoves << "\n";
                        }
                    }
                }

                //Prints out the NodePath to any blocks of text
                if (NodeList.front()->v.element.tag == GUMBO_NODE_TEXT) {
                    writeMoves << NodeList.front()->v.text.text;

                    MoveList_Xpath = GetXpath(parsed_response->root, NodeList.front());
                    for (int i = 0; i < MoveList_Xpath.size();i++) {
                        writeMoves << "," << MoveList_Xpath.at(i);
                    }
                    writeMoves << "\n";
                }

                //then, add the children of the current loop to the list to be checked
                GrabChildren(NodeList);
            }while (!NodeList.empty());
        }

        writeMoves.close();
    }*/
    //Prints to console so I can visually progress
    
    //Part 3(Step 2): Take the list of URLS and pulled relevant data Xpaths to create an in-memory database
    {
        MoveList_Database.resize(MoveList_Links.size());
        for(int MoveIterator = 0; MoveIterator < MoveList_Links.size(); MoveIterator++) {
            //Pull and Parse the HTML, then...
            cpr::Url New_URL = cpr::Url{ MoveList_Links[MoveIterator] };
            page_content = extract_html_page(New_URL);
            GumboOutput* parsed_response = gumbo_parse(page_content.c_str());

            //Prints to console so I can visually progress
            std::cout << "Move Number: " << MoveIterator + 1 << ", ";

            //...Fill that Digimon Database entry
            for (int y = 0; y < XpathMovelist.size()-2;y++) {
                GumboNode* ptr_Node = parsed_response->root;//Set current node to root
                for (int x = 0; x < XpathMovelist[y].size();x++) {
                    ptr_Node = static_cast<GumboNode*>(ptr_Node->v.element.children.data[XpathMovelist[y].at(x)]);
                }//follow XPath from root to relevant data. Y == Row, X == Column

                switch (y)
                {
                case 0://Name
                    MoveList_Database[MoveIterator].Name = ptr_Node->v.text.text;
                    MoveList_Database[MoveIterator].Name.erase(MoveList_Database[MoveIterator].Name.end() - 11, MoveList_Database[MoveIterator].Name.end());//Actually pulls a bit of text which says "X learned by". This line removes the learned by bit, leaving just the name.
                    std::cout << MoveList_Database[MoveIterator].Name << '\n' ;//Prints out the name for visual confirmation and for spotting errors.
                    break;

                case 1://Attribute
                    MoveList_Database[MoveIterator].Attribute = ptr_Node->v.text.text;
                    break;

                case 2://Type
                    MoveList_Database[MoveIterator].Type = ptr_Node->v.text.text;
                    break;

                case 3://SP Cost
                    MoveList_Database[MoveIterator].SP_Cost = std::stoi(ptr_Node->v.text.text);
                    break;

                case 4://Power
                    MoveList_Database[MoveIterator].Power = std::stoi(ptr_Node->v.text.text);
                    break;

                case 5://Inheritable
                    if (ptr_Node->v.text.text[0] != 'S')
                        MoveList_Database[MoveIterator].Inheritable = true;
                    break;

                case 6://Description
                    MoveList_Database[MoveIterator].Description = ptr_Node->v.text.text;

                //Technically case 7, Learned List; 
                    // This one is a little bit different(it needs to stop 2 digits prior and loop). Rather than adding a wasteful if statement further up, I'll manually handle it down where its intended to trigger, after Description.
                    ptr_Node = parsed_response->root;
                    for (int x = 0; x < XpathMovelist[y + 1].size()-3;x++) {//this digs to the start of where the loop should go
                        ptr_Node = static_cast<GumboNode*>(ptr_Node->v.element.children.data[XpathMovelist[y + 1].at(x)]);
                    }

                    GumboNode* ptr_LearnedListStart = ptr_Node;//make a note of the loop start point
                    bool ticktock{ 0 };
                    int LearnListSize = ptr_LearnedListStart->v.element.children.length/2;

                    for (int z = 0; z < LearnListSize;) {
                        ptr_Node = ptr_LearnedListStart;
                        ptr_Node = static_cast<GumboNode*>(ptr_Node->v.element.children.data[1 + (z * 2)]);

                            //These two plug the information into the relevant spot
                            if (ticktock == 0) {
                                //Creates an empty move slot every loop, then fills it
                                MoveList_Database[MoveIterator].LearnList.push_back(MoveArray());
                                ptr_Node = static_cast<GumboNode*>(ptr_Node->v.element.children.data[3]);
                                ptr_Node = static_cast<GumboNode*>(ptr_Node->v.element.children.data[0]);

                                ptr_Node = static_cast<GumboNode*>(ptr_Node->v.element.children.data[0]);
                                MoveList_Database[MoveIterator].LearnList[z].Name = ptr_Node->v.text.text;
                                ticktock = 1;
                                continue;
                            }

                            else {
                                ptr_Node = static_cast<GumboNode*>(ptr_Node->v.element.children.data[5]);
                                ptr_Node = static_cast<GumboNode*>(ptr_Node->v.element.children.data[0]);

                                MoveList_Database[MoveIterator].LearnList[z].Level = std::stoi(ptr_Node->v.text.text);
                                ticktock = 0;
                                z++;
                            }
                    }
                    break;
                }
            }
        }
    }
    std::cout << "Writing to Database Complete \n";

    //Step 4: Print to CSV
    {
        std::cout << "Printing Database to CSV \n";
        writeMoves << "---,Name,Attribute,Type,SP_Cost,Power,Inheritable,Description,LearnNames,LearnLevels\n";//CSV header
        for (int MoveIterator = 0;MoveIterator < MoveList_Database.size();MoveIterator++) {
            //Print to console for visual feedback of progross
            std::cout << "Move #:" << MoveIterator+1 << " " << MoveList_Database[MoveIterator].Name << '\n';

            writeMoves << MoveIterator+1 << ',';//Number/Key
            writeMoves << "\"" << MoveList_Database[MoveIterator].Name << "\",";//Name
            writeMoves << "\"" << MoveList_Database[MoveIterator].Attribute << "\",";//Attribute
            writeMoves << "\"" << MoveList_Database[MoveIterator].Type << "\",";//Type
            writeMoves << MoveList_Database[MoveIterator].SP_Cost << ',';//SP_Cost
            writeMoves << MoveList_Database[MoveIterator].Power << ',';//Power
            
            if (MoveList_Database[MoveIterator].Inheritable == true)//Inheritable
                writeMoves << "\"Inheritable\",";
            else
                writeMoves << "\"Special Move\",";
            
            writeMoves << "\"" << MoveList_Database[MoveIterator].Description << "\"";//Description

            if (MoveList_Database[MoveIterator].LearnList.empty()) {
                writeMoves << '\n';
                continue;
            }

            int LLIterator = 0;
            writeMoves << ",\"(\"";
            do {//Names of all Digimon that learn this move. In a single cell, seperated by | for later conversion into an Array
                writeMoves << '\"' << MoveList_Database[MoveIterator].LearnList[LLIterator].Name << '\"';
                LLIterator++;
                if (LLIterator < MoveList_Database[MoveIterator].LearnList.size())
                    writeMoves << "\",\"";
            } while (LLIterator < MoveList_Database[MoveIterator].LearnList.size());
            writeMoves << "\")\",";

            LLIterator = 0;
            writeMoves << "\"(\"";
            do {
                writeMoves << '\"' << MoveList_Database[MoveIterator].LearnList[LLIterator].Level << '\"';
                LLIterator++;
                if (LLIterator < MoveList_Database[MoveIterator].LearnList.size())
                    writeMoves << "\",\"";
            } while (LLIterator < MoveList_Database[MoveIterator].LearnList.size());
            writeMoves << "\")\"\n";
        }
    }
    std::cout << "Printing to Database Complete";//Step 4 complete
}

int main()
{
    PullMoveList();

    
}