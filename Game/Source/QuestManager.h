#ifndef QUEST_MANAGER_H_
#define QUEST_MANAGER_H_

#include "Module.h"
#include "Point.h"
#include <vector>
#include <list>
#include "SDL/include/SDL_rect.h"

enum  EVENT_TYPE {
	TOUCH_EVENT = 0,
};

class  Event
{
public:
	Event(EVENT_TYPE type_) :type(type_) {};
	~Event() ;
	EVENT_TYPE type;
	int orderNumber; 
	SDL_Rect Object;
	bool completed = false;
};  

class TouchEvent: public Event {
public: 
	TouchEvent():Event(EVENT_TYPE::TOUCH_EVENT){}
	~TouchEvent(){}
};


class Quest
{
public:
	Quest() {};
	~Quest() ;
	
	int DNI; 
	int Reward;
	Event* activationEvent;
	std::list <Event*> subMissions;
};


class Quest_Manager : public Module {
public:
	Quest_Manager();
	~Quest_Manager();

	bool Awake(pugi::xml_node& file);
	bool Start();
	Event* createEvent(pugi::xml_node&);

	void ActivateNextQuest(){}
	pugi::xml_document xmlfile;

	std::list<Quest*> preparedQuests;
	std::list<Quest*> activatedQuests;
	std::list<Quest*> endedQuests;
};

#endif
