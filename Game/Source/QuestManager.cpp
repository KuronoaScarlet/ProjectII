#include "QuestManager.h"
#include "Module.h"
#include "Point.h"
#include "Log.h"
#include <vector>
#include "App.h"


Quest_Manager::Quest_Manager() {}


Quest_Manager::~Quest_Manager() {

	for (std::list <Quest*>::iterator it = preparedQuests.begin(); it != preparedQuests.end(); it++) {
		preparedQuests.erase(it);
	}		
	for (std::list <Quest*>::iterator it = activatedQuests.begin(); it != activatedQuests.end(); it++) {
		activatedQuests.erase(it);
	}
	for (std::list <Quest*>::iterator it = endedQuests.begin(); it != endedQuests.end(); it++) {
		endedQuests.erase(it);
	}	
}


bool Quest_Manager::Awake(pugi::xml_node& config) {
	LOG("STARTING QUESTMANAGER");
	return true;
}


bool Quest_Manager::Start() {

	bool ret = true;

	pugi::xml_node questNode;
	pugi::xml_parse_result result = xmlfile.load_file("questData.xml");
	if (result == NULL) LOG("Could not load  xml file <loadxmlfunction> pugi error: %s", result.description());
	else questNode = xmlfile.child("quests");

	for (questNode = questNode.child("quest"); questNode; questNode = questNode.next_sibling("quest"))
	{
		Quest* newQuest = new Quest();
		newQuest->DNI = questNode.attribute("DNI").as_int();
		newQuest->Reward = questNode.attribute("Reward").as_int();

		newQuest->activationEvent = createEvent(questNode.child("ActivationEvent"));

		pugi::xml_node subEventNode;
		for (subEventNode = questNode.child("SubEvent"); subEventNode; subEventNode = subEventNode.next_sibling("SubEvent"))
		{
			newQuest->subMissions.push_back(createEvent(subEventNode));
		}
		
		preparedQuests.push_back(newQuest);
	}

	return ret;
}


Event* Quest_Manager::createEvent(pugi::xml_node &xmlReader)
{
	int Type = xmlReader.attribute("Type").as_int();

	if(Type == 1){
		TouchEvent* ret = new TouchEvent();

		ret->Object.x = xmlReader.child("Object").attribute("x").as_int();
		ret->Object.y = xmlReader.child("Object").attribute("y").as_int();
		ret->Object.w = xmlReader.child("Object").attribute("w").as_int();
		ret->Object.h = xmlReader.child("Object").attribute("h").as_int();
		
		return ret;
	}
}