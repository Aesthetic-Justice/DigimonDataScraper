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


//Step 1.1: Creates csv file to store pulled data
std::ofstream writeCsv("DigimonDatabase2.csv");

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
            //Add link to list at open spot
            Digimon_List.push_back(href->value);
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

                //Add the Component Digimon with a comma after each
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

                //Remove the last comma
                Digimon_Database.at(Digimon_Count).DGV_AdReqs.pop_back();
            }

            else
            {
                node = static_cast<GumboNode*>(node->v.element.children.data[0]);
                Digimon_Database.at(Digimon_Count).DGV_AdReqs = node->v.text.text;
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
                writeCsv << " " << Digimon_Xpath.at(i);
            }

            writeCsv << "\n";
        }
    }

    //Check for tag
    if (node->v.element.tag == GUMBO_TAG_A)
    {
        if (node->v.element.children.length == 0)
            return;

        GumboVector* dlA_children = &node->v.element.children;
        GumboNode* dlA_first_child = static_cast<GumboNode*>(dlA_children->data[0]);

        writeCsv << dlA_first_child->v.text.text;
        for (unsigned int i = 0; i < Digimon_Xpath.size();i++)
        {
            writeCsv << " " << Digimon_Xpath.at(i);
        }

        writeCsv << "\n";

    }

    //Check for tag
    if (node->v.element.tag == GUMBO_TAG_H2)
    {
        GumboVector* dlA_children = &node->v.element.children;
        GumboNode* dlA_first_child = static_cast<GumboNode*>(dlA_children->data[0]);

        writeCsv << dlA_first_child->v.text.text;
        for (unsigned int i = 0; i < Digimon_Xpath.size();i++)
        {
            writeCsv << " " << Digimon_Xpath.at(i);
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
            writeCsv << " " << Digimon_Xpath.at(i);
        }

        writeCsv << "\n";

    }

    //Check for tag
    if (node->v.element.tag == GUMBO_NODE_TEXT)
    {
     
        writeCsv << node->v.text.text;

        for (unsigned int i = 0; i < Digimon_Xpath.size();i++)
        {
            writeCsv << " " << Digimon_Xpath.at(i);
        }

        writeCsv << "\n";

    }

    GumboVector* son = &node->v.element.children;
    for (unsigned int i = 0; i < son->length;i++)
    {
        Digimon_Xpath.push_back(i);
        Dig_Loop(static_cast<GumboNode*>(son->data[i]));
        Digimon_Xpath.pop_back();
    }

}

int main()
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
        cpr::Url New_URL = cpr::Url{Digimon_List[Digimon_Count]};
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

    //Part 4: Sanitize the Database for CSV by removing quotation marks from text
    for (Digimon_Count = 0; Digimon_Count < Digimon_List.size(); Digimon_Count++)
    {
        Digimon_Database.at(Digimon_Count).Description.erase(std::remove(Digimon_Database.at(Digimon_Count).Description.begin(), Digimon_Database.at(Digimon_Count).Description.end(), '"'), Digimon_Database.at(Digimon_Count).Description.end());

        Digimon_Database.at(Digimon_Count).DGV_AdReqs.erase(std::remove(Digimon_Database.at(Digimon_Count).DGV_AdReqs.begin(), Digimon_Database.at(Digimon_Count).DGV_AdReqs.end(), '\"'), Digimon_Database.at(Digimon_Count).DGV_AdReqs.end());
    }

    //Prints to console so I can visually progress
    std::cout << "Printing to CSV: \n";

    //Part 5: Write the Database to CSV
    writeCsv << "Number Name Stage Attribute Type Memory_Usage Equip_Slots Description Support_Skill_Name HP_Base HP_Scaling SP_Base SP_Scaling ATK_Base ATK_Scaling DEF_Base DEF_Scaling INT_Base INT_Scaling SPD_Base SPD_Scaling DGV_LVL DGV_HP DGV_SP DGV_ATK DGV_DEF DGV_INT DGV_SPD DGV_ABI DGV_CAM DGV_AdReqs DeDGV_Options DGV_Options MoveList";//CSV Header
    for (Digimon_Count = 0; Digimon_Count < Digimon_List.size(); Digimon_Count++)
    {
        writeCsv << "\n";
        writeCsv << Digimon_Database.at(Digimon_Count).Number << " ";
        writeCsv << "\"" << Digimon_Database.at(Digimon_Count).Name << "\" ";
        writeCsv << "\"" << Digimon_Database.at(Digimon_Count).Stage << "\" ";//the "\"" is used to quote the entry, to protect special characters like " "
        writeCsv << Digimon_Database.at(Digimon_Count).Attribute << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).Type << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).Memory << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).Equip_Slots << " ";
        writeCsv << "\"" << Digimon_Database.at(Digimon_Count).Description << "\" ";
        writeCsv << "\"" << Digimon_Database.at(Digimon_Count).Support_Skill_Name << "\" ";
        writeCsv << Digimon_Database.at(Digimon_Count).HP_base << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).HP_scaling << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).SP_base << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).SP_scaling << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).ATK_base << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).ATK_scaling << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).DEF_base << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).DEF_scaling << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).INT_base << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).INT_scaling << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).SPD_base << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).SPD_scaling << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_LVL << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_HP << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_SP << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_ATK << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_DEF << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_INT << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_SPD << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_ABI << " ";
        writeCsv << Digimon_Database.at(Digimon_Count).DGV_CAM << " ";
        writeCsv << "\"" << Digimon_Database.at(Digimon_Count).DGV_AdReqs << "\" ";

        writeCsv << "\"";
        for (unsigned int i = 0; i < Digimon_Database.at(Digimon_Count).DeDigivolve_Options.size();i++)
        {
            writeCsv << Digimon_Database.at(Digimon_Count).DeDigivolve_Options.at(i) << "|";
        }
        writeCsv << "\" ";

        writeCsv << "\"";
        for (unsigned int i = 0; i < Digimon_Database.at(Digimon_Count).Digivolve_Options.size();i++)
        {
            writeCsv << Digimon_Database.at(Digimon_Count).Digivolve_Options.at(i) << "|";
        }
        writeCsv << "\" ";

        writeCsv << "\"";
        for (unsigned int i = 0; i < Digimon_Database.at(Digimon_Count).MoveList.size();i++)
        {
            writeCsv << Digimon_Database.at(Digimon_Count).MoveList.at(i) << "|";
        }
        writeCsv << "\"";

        //prints to console so I can keep track of progress
        std::cout << Digimon_Count+1 << "...\n";

    }
    
    //Close CSV
    writeCsv.close();

    // free the allocated memory
    gumbo_destroy_output(&kGumboDefaultOptions, parsed_response);
    
}