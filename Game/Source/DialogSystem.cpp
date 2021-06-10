#include "DialogSystem.h"

#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Fonts.h"
#include "EntityManager.h"
#include "FadeToBlack.h"
#include "SceneManager.h"

#include "Scene1.h"
#include "Intro.h"
#include "BattleScene.h"

#include "SDL/include/SDL.h"


DialogueSystem::DialogueSystem() {}

DialogueSystem::~DialogueSystem() {}

bool DialogueSystem::Start()
{
	app->dialogueSystem->LoadDialogue("dialogue_tree.xml");
	battleTransition = app->audio->LoadFx("Assets/Audio/FX/battle_transition.wav");
	textBox = app->tex->Load("Assets/Textures/text_box.png");

	holaRay= true;

	return true;
}

bool DialogueSystem::Update(float dt)
{
	GamePad& pad = app->input->pads[0];
	if (app->entityManager->playerData.onDialog == true)
	{
		app->render->DrawTexture(textBox, -app->render->camera.x, -app->render->camera.y + 560);

		if (Id == 0)
		{
			if ((input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || pad.a) && holaRay == true)
			{
				holaRay = false;
				playerInput = 0;
				Id = 0;
				PerformDialogue(Id, playerInput);
				actionChecks++;
			}
			if (actionChecks == dialogueTrees[0]->dialogueNodes.size())
			{
				app->entityManager->playerData.onDialog = false;
			}
		}

		if (Id == 1)
		{
			if ((input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || pad.a) && holaRay == true)
			{
				holaRay = false;
				playerInput = 0;
				Id = 1;
				PerformDialogue(Id, playerInput);
				actionChecks++;
			}
			if (actionChecks == dialogueTrees[1]->dialogueNodes.size())
			{
				app->entityManager->playerData.onDialog = false;
				app->playerPosition = app->entityManager->playerData.position;
				if (app->sceneManager->id == SCENE1) app->entityManager->playerData.scene = 1;
				if (app->sceneManager->id == SCENE12) app->entityManager->playerData.scene = 2;
				app->sceneManager->ChangeScene(BATTLE,1);
			}
		}

		if (Id == 2)
		{
			if ((input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.a || pad.right) && holaRay == true)
			{
				holaRay = false;
				playerInput = 0;
				Id = 2;
				PerformDialogue(Id, playerInput);
				actionChecks++;
			}
			if (actionChecks == dialogueTrees[2]->dialogueNodes.size())
			{
				app->entityManager->playerData.onDialog = false;
			}
		}

		if (Id == 3)
		{
			if ((input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || pad.a) && holaRay == true)
			{
				holaRay = false;
				playerInput = 0;
				Id = 3;
				PerformDialogue(Id, playerInput);
				actionChecks++;
			}
			if ((input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.up) && holaRay == true)
			{
				holaRay = false;
				playerInput = 0;
				Id = 3;
				PerformDialogue(Id, playerInput);
				actionChecks = 0;
			}
			if ((input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || pad.right) && holaRay == true)
			{
				holaRay = false;
				playerInput = 1;
				Id = 3;
				PerformDialogue(Id, playerInput);
				actionChecks = 0;
			}
			if ((input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || pad.down) && holaRay == true)
			{
				holaRay = false;
				playerInput = 2;
				Id = 3;
				PerformDialogue(Id, playerInput);
				actionChecks = 14;
			}
			if (actionChecks == dialogueTrees[3]->dialogueNodes.size())
			{
				app->entityManager->playerData.onDialog = false;
				app->audio->PlayFx(2,battleTransition);
				app->sceneManager->ChangeScene(SCENE1,0);
			}
		}
		if (Id == 4)
		{
			if ((input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || pad.a) && holaRay == true)
			{
				holaRay = false;
				playerInput = 0;
				Id = 4;
				PerformDialogue(Id, playerInput);
				actionChecks++;
			}
			if ((input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.up) && holaRay == true)
			{
				holaRay = false;
				playerInput = 0;
				Id = 4;
				PerformDialogue(Id, playerInput);
				actionChecks = 3;
				enterDungeon = true;
			}
			if ((input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || pad.right) && holaRay == true)
			{
				holaRay = false;
				playerInput = 1;
				Id = 4;
				PerformDialogue(Id, playerInput);
				actionChecks = 3;
				enterDungeon = false;
			}
			if (actionChecks == dialogueTrees[4]->dialogueNodes.size())
			{
				app->entityManager->playerData.onDialog = false;
				if (enterDungeon == true)
				{
				app->sceneManager->ChangeScene(BATH, 0);
				app->sceneManager->CreateQuest(3, "Make it through the dungeon");
				}
				
			}
		}
		if (Id == 5)
		{
			if ((input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.a || pad.up) && holaRay == true)
			{
				holaRay = false;
				playerInput = 0;
				Id = 5;
				PerformDialogue(Id, playerInput);
				actionChecks++;
			}
			if (actionChecks == dialogueTrees[5]->dialogueNodes.size())
			{
				app->entityManager->playerData.onDialog = false;
				app->sceneManager->CreateQuest(2, "Win a combat");
			}
		}
		if (Id == 6)
		{
			if ((input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN || input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || pad.a || pad.up) && holaRay == true)
			{
				holaRay = false;
				playerInput = 0;
				Id = 6;
				PerformDialogue(Id, playerInput);
				actionChecks++;
			}
			if (actionChecks == dialogueTrees[6]->dialogueNodes.size())
			{
				app->entityManager->playerData.onDialog = false;
				app->sceneManager->CreateQuest(1, "Combine one Item!");
			}
		}
	}
	if (!pad.a && !pad.up && !pad.right && !pad.down)
	{
		holaRay = true;
	}

	return true;
}


bool DialogueSystem::CleanUp()
{
	for (int i = 0; i < dialogueTrees.size(); i++)
	{
		for (int j = 0; j < dialogueTrees[i]->dialogueNodes.size(); j++) delete dialogueTrees[i]->dialogueNodes[j];

		dialogueTrees[i]->dialogueNodes.clear();
		delete dialogueTrees[i];
	}
	dialogueTrees.clear();

	return true;
}

void DialogueSystem::PerformDialogue(int treeId, int playerInput)
{
	if (playerInput >= 0 && playerInput < currentNode->dialogueOptions.size())
	{
		for (int i = 0; i < dialogueTrees[treeId]->dialogueNodes.size(); i++)
			if (currentNode->dialogueOptions[playerInput]->nextNode == dialogueTrees[treeId]->dialogueNodes[i]->nodeId)
			{
				currentNode = dialogueTrees[treeId]->dialogueNodes[i];
				break;
			}
	}


}

bool DialogueSystem::LoadDialogue(const char* file)
{
	pugi::xml_parse_result result = dialogues.load_file(file);

	bool ret = true;

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file, result.description());
		ret = false;
	}
	else
	{
		pugi::xml_node n = dialogues.first_child().child("dialogueExample");

		for (n; n != NULL; n = n.next_sibling("dialogueExample"))
		{
			DialogueTree* tree = new DialogueTree;
			tree->treeId = n.attribute("Id").as_int();
			LoadNodes(n, tree);
			dialogueTrees.push_back(tree);
		}
	}
	return true;
}

bool DialogueSystem::LoadNodes(pugi::xml_node& trees, DialogueTree* example)
{
	for (pugi::xml_node n = trees.child("node"); n != NULL; n = n.next_sibling("node"))
	{
		DialogueNode* node = new DialogueNode;
		node->text.assign(n.attribute("text").as_string());
		node->nodeId = n.attribute("id").as_int();
		LoadOptions(n, node);
		example->dialogueNodes.push_back(node);
	}

	return true;
}

bool DialogueSystem::LoadOptions(pugi::xml_node& response, DialogueNode* answers)
{
	for (pugi::xml_node option = response.child("dialogue"); option != NULL; option = option.next_sibling("dialogue"))
	{
		DialogueOption* selection = new DialogueOption;
		selection->text.assign(option.attribute("option").as_string());
		selection->nextNode = option.attribute("nextNode").as_int();
		answers->dialogueOptions.push_back(selection);
		answers->answersList.Add((option.attribute("option").as_string()));
	}

	return true;
}