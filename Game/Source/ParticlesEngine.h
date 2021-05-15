#ifndef _PARTICLESENGINE_H_
#define _PARTICLESENGINE_H_

#include <time.h>
//#include "SDL/include/SDL.h"
//#include "SDL_image/include/SDL_image.h"
//#include "SDL_ttf/include/SDL_ttf.h"
#include "PugiXML/src/pugixml.hpp"
#include "List.h"
#include "Module.h"
#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#define RELEASE(x) { delete x; x = nullptr; }
#define RELEASE_ARRAY(x) { delete[] x; x = nullptr; }

struct Particle
{
	float lifetime, lifespan;
	float x, y;
	float vx, vy;
	float w, h;
};

struct ParticleProperties
{
	unsigned int amount;
	float min_lifespan, max_lifespan;
	float min_vx, max_vx, min_vy, max_vy;
	float gravity_center_x, gravity_center_y, gravity_ax, gravity_ay;
	float min_x, max_x, min_y, max_y, min_w, max_w, min_h, max_h;
	SDL_Texture* texture;
};

enum class EmitterType
{
	SPARKLES,
	BALL,
	FAK,
	SNACK,
	RAIN,
	SNOW,
	RULE,
	LIQUIDPAPER,
	RUBBER,
	CAN,
	COFFE,
	CALCULATOR,
	BOOK,
	FIRE,
	SMOKE,
	FIREWORKS,
};

class Emitter
{
public:

	bool active;
	int center_x, center_y;
	EmitterType type;
	ParticleProperties properties;
	Particle* particles;
	int currentFrame = 0;
	int maxFrames = 0;
	bool pendingToDelete = false;

	Emitter()
	{
		active = false;
	}

	~Emitter()
	{
		RELEASE_ARRAY(particles);
	}

	void Init(EmitterType _type, int _x, int _y, pugi::xml_node config, int maxf)
	{
		active = true;

		type = _type;
		center_x = _x;
		center_y = _y;
		maxFrames = maxf;

		switch (type)
		{
		case EmitterType::SPARKLES: config = config.child("Sparkles"); break;
		case EmitterType::BALL: config = config.child("Ball"); break;
		case EmitterType::RAIN: config = config.child("Rain"); break;
		case EmitterType::SNOW: config = config.child("Snow"); break;
		case EmitterType::FAK: config = config.child("Fak"); break;
		case EmitterType::SNACK: config = config.child("Snack"); break;
		case EmitterType::RULE: config = config.child("Rule"); break;
		case EmitterType::CAN: config = config.child("Can"); break;
		case EmitterType::RUBBER: config = config.child("Rubber"); break;
		case EmitterType::LIQUIDPAPER: config = config.child("LiquidPaper"); break;
		case EmitterType::COFFE: config = config.child("Coffe"); break;
		case EmitterType::CALCULATOR: config = config.child("Calculator"); break;
		case EmitterType::BOOK: config = config.child("Book"); break;
		case EmitterType::FIRE: config = config.child("Fire"); break;
		case EmitterType::SMOKE: config = config.child("Smoke"); break;
		case EmitterType::FIREWORKS: config = config.child("Fireworks"); break;
		}

		properties.amount = config.child("emitter").attribute("amount").as_int();
		properties.min_lifespan = config.child("lifespan").attribute("min").as_float();
		properties.max_lifespan = config.child("lifespan").attribute("max").as_float();
		properties.min_vx = config.child("velocity").attribute("min_vx").as_float();
		properties.max_vx = config.child("velocity").attribute("max_vx").as_float();
		properties.min_vy = config.child("velocity").attribute("min_vy").as_float();
		properties.max_vy = config.child("velocity").attribute("max_vy").as_float();
		properties.gravity_center_x = center_x + config.child("gravity").attribute("center_x").as_float();
		properties.gravity_center_y = center_y + config.child("gravity").attribute("center_y").as_float();
		properties.gravity_ax = config.child("gravity").attribute("ax").as_float();
		properties.gravity_ay = config.child("gravity").attribute("ay").as_float();
		properties.min_x = config.child("position").attribute("min_x").as_float();
		properties.max_x = config.child("position").attribute("max_x").as_float();
		properties.min_y = config.child("position").attribute("min_y").as_float();
		properties.max_y = config.child("position").attribute("max_y").as_float();
		properties.min_w = config.child("draw").attribute("min_w").as_float();
		properties.max_w = config.child("draw").attribute("max_w").as_float();
		properties.min_h = config.child("draw").attribute("min_h").as_float();
		properties.max_h = config.child("draw").attribute("max_h").as_float();
		const char* texture_path = config.child("draw").attribute("texture").as_string();
		properties.texture = app->tex->Load(texture_path);

		particles = new Particle[properties.amount];
		for (int i = 0; i < properties.amount; ++i)
			particles[i] = StartParticle();
	}

	Particle StartParticle()
	{
		Particle p;

		p.lifetime = 0.0f;
		p.lifespan = properties.min_lifespan + rand() % (int)(1 + properties.max_lifespan - properties.min_lifespan);
		p.x = center_x + properties.min_x + rand() % (int)(1 + properties.max_x - properties.min_x);
		p.y = center_y + properties.min_y + rand() % (int)(1 + properties.max_y - properties.min_y);
		p.vx = properties.min_vx + rand() % (int)(1 + properties.max_vx - properties.min_vx);
		p.vy = properties.min_vy + rand() % (int)(1 + properties.max_vy - properties.min_vy);
		p.w = properties.min_w + rand() % (int)(1 + properties.max_w - properties.min_w);
		p.h = properties.min_h + rand() % (int)(1 + properties.max_h - properties.min_h);

		return p;
	}

	bool Update(float dt)
	{
		if (currentFrame < maxFrames)
		{
			++currentFrame;
			for (int i = 0; i < properties.amount; ++i)
			{
				if (particles[i].lifetime >= particles[i].lifespan)
					particles[i] = StartParticle();

				++particles[i].lifetime;

				particles[i].x += particles[i].vx;
				particles[i].y += particles[i].vy;
				if (particles[i].x < properties.gravity_center_x) particles[i].vx += properties.gravity_ax;
				if (particles[i].x > properties.gravity_center_x) particles[i].vx -= properties.gravity_ax;
				if (particles[i].y < properties.gravity_center_y) particles[i].vy += properties.gravity_ay;
				if (particles[i].y > properties.gravity_center_y) particles[i].vy -= properties.gravity_ay;

			}
			return true;
		}
		return false;
	}

	void Draw(bool debugDraw)
	{
		for (int i = 0; i < properties.amount; ++i)
		{
			unsigned int alpha = 255 * (1 - (particles[i].lifetime / particles[i].lifespan));
			SDL_Rect particleRect{ particles[i].x - particles[i].w / 2, particles[i].y - particles[i].h / 2, particles[i].w, particles[i].h };
			
			SDL_SetTextureBlendMode(properties.texture, SDL_BLENDMODE_BLEND);
			SDL_SetTextureAlphaMod(properties.texture, alpha);
			app->render->DrawTexture(properties.texture, particleRect.x, particleRect.y);

		}
		if (debugDraw)
		{
			SDL_SetRenderDrawColor(app->render->renderer, 0, 255, 255, 255);
			SDL_RenderDrawLine(app->render->renderer, -app->render->camera.x + center_x - 20, -app->render->camera.y + center_y, -app->render->camera.x + center_x + 20, -app->render->camera.y + center_y);
			SDL_RenderDrawLine(app->render->renderer, -app->render->camera.x + center_x, -app->render->camera.y + center_y - 20, -app->render->camera.x + center_x, -app->render->camera.y + center_y + 20);
			SDL_SetRenderDrawColor(app->render->renderer, 255, 0, 255, 255);
			SDL_RenderDrawLine(app->render->renderer, -app->render->camera.x + properties.gravity_center_x - 10, -app->render->camera.y + properties.gravity_center_y, -app->render->camera.x + properties.gravity_center_x + 10, -app->render->camera.y + properties.gravity_center_y);
			SDL_RenderDrawLine(app->render->renderer, -app->render->camera.x + properties.gravity_center_x, -app->render->camera.y + properties.gravity_center_y - 10, -app->render->camera.x + properties.gravity_center_x, -app->render->camera.y + properties.gravity_center_y + 10);
		}
	}

};

class ParticleSystem : public Module
{
public:

	List<Emitter*>* emitters = new List<Emitter*>;

	bool debugDraw = false;

	pugi::xml_document particles_config;
	pugi::xml_node type_config;

	unsigned int emitters_count = 0;
	unsigned int particles_count = 0;

	ParticleSystem()
	{
		
	}

	bool Start()
	{
		srand(time(0));
		pugi::xml_parse_result result = particles_config.load_file("particles_config.xml");
		if (!result) printf("ERROR while loading particles_config.xml file: %s", result.description());
		type_config = particles_config.child("ParticleProperties");
		SDL_SetRenderDrawBlendMode(app->render->renderer, SDL_BLENDMODE_BLEND);
		return true;
	}

	~ParticleSystem()
	{
		RELEASE(emitters);
	}

	void AddEmitter(EmitterType type, int x, int y, int maxf = 60)
	{
		Emitter* emitter = new Emitter;
		emitter->Init(type, x, y, type_config, maxf);
		emitters->Add(emitter);
		++emitters_count;
		particles_count += emitter->properties.amount;
	}

	bool Update(float dt)
	{
		if (app->input->GetKey(SDL_SCANCODE_P) == KEY_DOWN) debugDraw = !debugDraw;

		if (emitters->start)
			for (ListItem<Emitter*>* emitter = emitters->start; emitter; emitter = emitter->next)
			{
				if (!emitter->data->Update(dt))
				{
					emitters->Del(emitter);
				}
			}

		return true;
	}

	bool PostUpdate()
	{
		if (emitters->start)
			for (ListItem<Emitter*>* emitter = emitters->start; emitter; emitter = emitter->next)
				emitter->data->Draw(debugDraw);
		return true;
	}

};

#endif
