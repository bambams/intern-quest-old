/*
 * Intern's Quest is an adventure game.
 * Copyright (C) 2009 Brandon McCaig, Samuel Henderson
 *
 * This file is part of Intern's Quest.
 *
 * Intern's Quest is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * Intern's Quest is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Intern's Quest.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <app.hpp>

namespace iq
{
	bool app::close_button_pressed = false;
	const std::string app::DEFAULT_FILE = "config/app.config";

	app::app(const char * const title, int argc, char *argv[]):
		m_file(DEFAULT_FILE),
		fts(0),
		ms(0),
		total_frames(0)
	{
		IQ_APP_TRACE("iq::app::app(int, char *[]) {");

		iq::entity_ptr entity;

		this->sem.reset(new sem_t(), app::sem_destroy);
		this->target_fps = 30;
		this->verbose = false;

		try
		{
			this->parse_args(argc, argv);
			this->initialize();

			if(title)
				set_window_title(title);

			this->load();

			/*
			 * For now we'll just copy ALL entities into the drawn vector.
			 * We'll begin by reserving enough slots.
			 */
			this->drawn_entities.reserve(this->entities.size());

			/*
			 * Now we'll transform the std::map's values into std::vector values.
			 * URL: http://www.velocityreviews.com/forums/showpost.php?p=2512190&postcount=6
			 */
			std::transform(
				this->entities.begin(),
				this->entities.end(),
				std::back_inserter(this->drawn_entities),
				iq::select_value< std::pair<std::string, iq::entity_ptr> >());
		}
		catch(...)
		{
			this->deinitialize();
			throw;
		}

		this->map.reset(new iq::tilemap("map/example.map", this->tiles));

/*
 * h4x to get the entities on the map to start...
 */
this->entities["boss"]->m_x = 200;
this->entities["boss"]->m_y = 100;

this->entities["intern"]->m_x = 164; // 100/32=3.125 && 124/32=3.875 therefore player is located in 3rd tile across (extends into 3rd)
this->entities["intern"]->m_y = 164; // 100/32=3.125 && 132/32=4.125 therefore player is located in 3rd tile down extending to 4th

this->entities["security"]->m_x = 600;
this->entities["security"]->m_y = 220;

		this->timer.reset(new iq::timer());

		/*
		 * This will eventually start at SETUP, but there currently is no
		 * setup. :P
		 */
		this->set_state(GAMEPLAY);

		IQ_APP_TRACE("} //iq::app::app(int, char *[])");
	}

	app::~app(void)
	{
		IQ_APP_TRACE("iq::app::~app() {");

		this->deinitialize();

		IQ_APP_TRACE("} //iq::app::~app()");
	}

	void app::add_frame(void *app)
	{
		IQ_APP_TRACE("iq::app::add_frame(void) {");

		iq::app *self = (iq::app *)app;

		sem_post(self->sem.get());
		self->fts++;
		self->total_frames++;

		self->ms = self->ms + (timer::MSECS_PER_SEC / self->target_fps);

		IQ_APP_TRACE("} //iq::app::add_frame(void)");
	}
	END_OF_FUNCTION(app::add_frame)

	void app::close_button_handler(void)
	{
		IQ_APP_TRACE("iq::app::close_button_handler(void) {");

		iq::app::close_button_pressed = true;

		IQ_APP_TRACE("} //iq::app::close_button_handler(void)");
	}
	END_OF_FUNCTION(app::close_button_handler)

// h4x for demo: Should probably be moved to game_logic or something
bool app::horizontal_collision(int x, int y, int w, int &tilecoordy)
{
	int tilexpixels = x-(x%32);	//calculate the x position (pixels!) of the tiles we check against
	int testend = x + w;		//calculate the end of testing (just to save the x+w calculation each for loop)

	tilecoordy = y/32;		//calculate the y position (map coordinates!) of the tiles we want to test

	int tilecoordx = tilexpixels/32;	//calculate map x coordinate for first tile

//	textprintf_ex(this->scrbuf.get(), font, 20, 70, LIGHTGREEN, -1,	"hc: tilecoordx: %d tilecoordy: %d", tilecoordx, tilecoordy);
	//loop while the start point (pixels!) of the test tile is inside the players bounding box
	while(tilexpixels <= testend){
		if(!this->map->is_passable(tilecoordx, tilecoordy)){	//is a solid tile is found at tilecoordx, tilecoordy?
//			printf("app::horizontal_colision:: pixel: %d,%d tile: %d,%d\n", x, y, tilecoordx, tilecoordy);
			return true;
		}

		tilecoordx++;		//increase tile x map coordinate
		tilexpixels+=32;	//increase tile x pixel coordinate
	}

	return false;
}

bool app::vertical_collision(int x, int y, int h, int &tilecoordx)
{
	int tileypixels = y-(y%32);
	int testend = y + h;

	tilecoordx = x/32;

	int tilecoordy = tileypixels/32;
//	textprintf_ex(this->scrbuf.get(), font, 20, 60, LIGHTGREEN, -1,	"vc: tilecoordx: %d tilecoordy: %d", tilecoordx, tilecoordy);
	
	
	while(tileypixels <= testend){
		if(!this->map->is_passable(tilecoordx, tilecoordy)){
//			printf("app::vertical_colision:: pixel: %d,%d tile: %d,%d\n", x, y, tilecoordx, tilecoordy);
			return true;
		}
		tilecoordy++;
		tileypixels += 32;
	}

	return false;
}


	void app::deinitialize(void)
	{
		IQ_APP_TRACE("iq::app::deinitialize() {");

		remove_param_int(app::add_frame, this);

		IQ_APP_TRACE("} //iq::app::deinitialize()");
	}

	void app::draw(void)
	{
		IQ_APP_TRACE("iq::app::draw() {");

		(this->*(this->m_drawptr))();

		IQ_APP_TRACE("} //iq::app::draw()");
	}

	void app::draw_credits(void)
	{
		IQ_APP_TRACE("iq::app::draw_credits() {");

		IQ_APP_TRACE("} //iq::app::draw_credits()");
	}

	void app::draw_gameplay(void)
	{
		IQ_APP_TRACE("iq::app::draw_gameplay() {");

		iq::BITMAP_ptr bitmap;
		iq::entity_ptr entity;

/*
 * h4x: temporary just to see the screen dimensions and center.
 */
// rectfill(this->scrbuf.get(), 0, 0, this->scrbuf->w, this->scrbuf->h, WHITE);

// for(int i=0; i<3; i++)
// {
	// hline(this->scrbuf.get(), 0, this->scrbuf->h / 2 - 1 + i, this->scrbuf->w, RED);
	// vline(this->scrbuf.get(), this->scrbuf->w / 2 - 1 + i, 0, this->scrbuf->h, RED);
	// rect(this->scrbuf.get(), 0+i, 0+i, this->scrbuf->w - 1 - i, this->scrbuf->h - 1 - i, RED);
// }

		iq::uint map_len = this->map->layers.size();
		iq::uint map_mid = map_len/2;

		for(iq::uint i=0; i<map_mid; i++)
			this->map->draw(i, scrbuf, this->player);

		for(std::vector<iq::entity_ptr>::iterator i=this->drawn_entities.begin(); i != this->drawn_entities.end(); i++)
		{
			bitmap = (entity = *i)->current_frame(this->ms);
			masked_blit(bitmap.get(), this->scrbuf.get(), 0, 0, entity->screen_x(this->scrbuf, this->player), entity->screen_y(this->scrbuf, this->player), bitmap->w, bitmap->h);
		}

		for(iq::uint i=map_mid; i<map_len; i++)
			this->map->draw(i, scrbuf, this->player);

		//rect(this->scrbuf.get(), ((SCREEN_W/2)-this->player->w()/2), ((SCREEN_H/2) - this->player->h()/2) 
		//textprintf_ex(this->scrbuf.get(), font, 20, 20, WHITE, -1,
				//"frame-count: %d",
				//g_total_frames);

//for(int i=0, ilen=this->map->h(); i<ilen; i++)
//	for(int j=0, jlen=this->map->w(); j<jlen; j++)
//		textprintf_ex(this->scrbuf.get(), font, 500+(10*j), 30+(10*i), LIGHTGREEN, -1, "%d", this->map->is_passable(j, i));

		textprintf_ex(this->scrbuf.get(), font, 20, 40, LIGHTGREEN, -1,	"time: %s", this->timer->to_str().c_str());
//		textprintf_ex(this->scrbuf.get(), font, 20, 50, LIGHTGREEN, -1,	"player->m_x: %d player->m_y: %d", this->player->m_x, this->player->m_y);
		//textprintf_ex(app->scrbuf.get(), font, 20, 60, WHITE, -1,
				//"fps: %d",
				//g_frames_per_second);

		blit(this->scrbuf.get(), screen, 0, 0, 0, 0, this->scrbuf->w, this->scrbuf->h);
		clear(this->scrbuf.get());

		IQ_APP_TRACE("} //iq::app::draw_gameplay()");
	}

	void app::draw_scripted(void)
	{
		IQ_APP_TRACE("iq::app::draw_scripted() {");

		IQ_APP_TRACE("} //iq::app::draw_scripted()");
	}

	void app::draw_setup(void)
	{
		IQ_APP_TRACE("iq::app::draw_setup() {");

		IQ_APP_TRACE("} //iq::app::draw_setup()");
	}

	void app::initialize(void)
	{
		IQ_APP_TRACE("iq::app::initialize(void) {");

		int i, len;
		const char * const msg = "If at first you don't succeed,...you fail";
		const int X = 0, Y = 1;
		int r[][NUM_DIMENSIONS] = {
		                              {1024, 768},
		                              {800, 600},
		                              {640, 480}
		                          };

		if(allegro_init() != 0)
		{
			//printf("%s [to initialize Allegro]. %s.\n", msg, allegro_error);
			throw std::runtime_error(std::string("Failed to initialize Allegro. ") + allegro_error + ".");
		}

		if(install_keyboard() != 0)
		{
			//printf("%s [to install the keyboard routines].\n", msg);
			throw std::runtime_error("Failed to install the keyboard routines.");
		}

		if(install_mouse() < 0)
		{
			//printf("%s [to install the mouse routines]. %s.\n",
					//msg, allegro_error);
			throw std::runtime_error(std::string("Failed to install the mouse routines. ") + allegro_error + ".");
		}

		enable_hardware_cursor();

		if(install_timer() != 0)
		{
			//printf("%s [to install the timer routines].", msg);
			throw std::runtime_error("Failed to install the timer routines.");
		}

		set_color_depth(32);

		for(i=0, len=sizeof(r)/sizeof(int)/NUM_DIMENSIONS; i<len; i++)
		{
			if(set_gfx_mode(GFX_AUTODETECT_WINDOWED,
					r[i][X], r[i][Y], 0, 0) == 0)
			{
				break;
			}
		}

		if(i == len)
		{
			for(i=0; i<len; i++)
				if(set_gfx_mode(GFX_AUTODETECT, r[i][X], r[i][Y], 0, 0) == 0)
					break;

			if(i == len)
			{
				//printf("%s [to set the graphics mode]. %s.\n",
						//msg, allegro_error);
				throw std::runtime_error(std::string("Failed to set the graphics mode. ") + allegro_error + ".");
			}
		}

		this->scrbuf.reset(create_bitmap(r[i][X], r[i][Y]), destroy_bitmap);

		if(this->scrbuf.get() == NULL)
		{
			//printf("%s [to create a screen buffer].\n", msg);
			throw std::runtime_error("Failed to create a screen buffer.");
		}

		clear(this->scrbuf.get());

		if(!(this->os_cursor = !show_os_cursor(MOUSE_CURSOR_ARROW)))
		{
			printf("%s [to show the operating system mouse cursor].\n", msg);
		}

		if(set_display_switch_mode(SWITCH_BACKGROUND) != 0)
		{
			//printf("%s [to set display switch mode]. %s.\n",
					//msg, allegro_error);
			throw std::runtime_error(std::string("Failed to set display switch mode. ") + allegro_error + ".");
		}

		//LOCK_VARIABLE(g_close_button_pressed);
		LOCK_VARIABLE(this);
		//LOCK_VARIABLE(g_pending_frames);
		//LOCK_VARIABLE(g_frames_per_second);
		//LOCK_VARIABLE(g_frames_this_second);
		//LOCK_VARIABLE(g_seconds);
		//LOCK_VARIABLE(g_minutes);
		//LOCK_VARIABLE(g_hours);
		//LOCK_VARIABLE(g_days);

		LOCK_FUNCTION(app::add_frame);
		LOCK_FUNCTION(app::close_button_handler);
		//LOCK_FUNCTION(tick);

		if(install_param_int_ex(app::add_frame, this, BPS_TO_TIMER(this->target_fps)) != 0)
		{
			//printf("%s [to install the main application timer].\n", msg);
			throw std::runtime_error("Failed to install the main application timer.");
		}

		set_close_button_callback(app::close_button_handler);

		sem_init(this->sem.get(), 0, 1);

		IQ_APP_TRACE("} //iq::app::initialize(void)");
	}

	void app::load(void)
	{
		iq::TiXmlDocument_ptr doc(new TiXmlDocument(this->m_file));
		const TiXmlElement *root = NULL;

		if(!doc->LoadFile())
			throw std::runtime_error("Failed to load app from XML document '" + this->m_file + "'.");

		if((root = doc->RootElement()) == NULL)
			throw std::runtime_error("App XML missing root element.");

		this->load(root);
	}

	void app::load(const TiXmlElement * const app)
	{
		const TiXmlElement *element = NULL;

		if((element = app->FirstChildElement("entities")) != NULL)
			this->load_entities(element);
	}

	void app::load_entities(const TiXmlElement * const entities)
	{
		iq::entity_ptr entity;
		const TiXmlElement *element = NULL;
		std::map<std::string, iq::entity_ptr>::iterator it;
		std::string player;
		const char *str = NULL;

		if((str = entities->Attribute("player")) == NULL)
			throw std::runtime_error("Entities XML element missing required attribute 'player'.");
		else
			player = str;

		if((element = entities->FirstChildElement("entity")) == NULL)
			throw std::runtime_error("Entities XML missing entity elements.");

		do
		{
			if((str = element->Attribute("file")) != NULL)
				entity.reset(new iq::entity(str));
			else
				entity.reset(new iq::entity(element));

			if(this->entities.find(entity->name()) != this->entities.end())
				throw std::runtime_error("Dupliciate entity '" + entity->name() + "' found in XML. Duplicates are not allowed.");

			this->entities[entity->name()] = entity;
		}while((element = element->NextSiblingElement("entity")) != NULL);

		if((it = this->entities.find(player)) == this->entities.end())
			throw std::runtime_error("Player entity '" + player + "' doesn't exist.");

		this->player = it->second;
	}
	
	void app::logic(void)
	{
		IQ_APP_TRACE("iq::app::logic() {");

		(this->*(this->m_logicptr))();

		IQ_APP_TRACE("} //iq::app::logic()");
	}

	void app::logic_credits(void)
	{
		IQ_APP_TRACE("iq::app::logic_credits() {");

		IQ_APP_TRACE("} //iq::app::logic_credits()");
	}

	void app::logic_gameplay(void)
	{
		IQ_APP_TRACE("iq::app::logic_gameplay() {");

/*
 * h4x: QnD for a demonstration. :D
 */
 int velx = 0, vely = 0; //don't move the player at all by default
 int tilecoord;
 
if(key[KEY_UP])
{
	vely = -(this->player->speedy());
	//printf("::KEY_UP:: velx: %d vely: %d\n", velx, vely);
	
	// this->player->m_y -= this->player->speedy();

	if(this->player->facing() != entity::FACING_UP || !this->player->current_animation()->second->playing())
	{
		this->player->face(entity::FACING_UP);
		this->player->begin_animation("walk_up", this->ms);
	}
}
else if(key[KEY_RIGHT])
{
	velx = this->player->speedx();
	//printf("::KEY_RIGHT:: velx: %d vely: %d\n", velx, vely);
	// this->player->m_x += this->player->speed();
	
	if(this->player->facing() != entity::FACING_RIGHT || !this->player->current_animation()->second->playing())
	{
		this->player->face(entity::FACING_RIGHT);
		this->player->begin_animation("walk_right", this->ms);
	}
}
else if(key[KEY_DOWN])
{
	vely = this->player->speedy();
	//printf("::KEY_DOWN:: velx: %d vely: %d\n", velx, vely);
	// this->player->m_y += this->player->speed();

	if(this->player->facing() != entity::FACING_DOWN || !this->player->current_animation()->second->playing())
	{
		 this->player->face(entity::FACING_DOWN);
		 this->player->begin_animation("walk_down", this->ms);
	}
}
else if(key[KEY_LEFT])
{
	velx = -(this->player->speedx());
	//printf("::KEY_LEFT:: velx: %d vely: %d\n", velx, vely);
	// this->player->m_x -= this->player->speed();

	if(this->player->facing() != entity::FACING_LEFT || !this->player->current_animation()->second->playing())
	{
		this->player->face(entity::FACING_LEFT);
		this->player->begin_animation("walk_left", this->ms);
	}
}
else if(this->player->current_animation()->second->playing())
{
	this->player->reset_animation();
}

//check x-axis first (--)
if(velx > 0)
{
	//player is moving right
	if(vertical_collision(this->player->m_x + velx+this->player->w(), this->player->m_y, this->player->h(), tilecoord))	//collision on the right side.
			this->player->m_x = tilecoord*32 - this->player->w()-1;					//move to the edge of the tile (tile on the right -> mind the player width)
		else			//no collision
			this->player->m_x += velx;
	}		 
	else if(velx < 0){	//moving left
		if(vertical_collision(this->player->m_x + velx, this->player->m_y, this->player->h(), tilecoord))		//collision on the left side
			this->player->m_x = (tilecoord+1)*32 +1;				//move to the edge of the tile
		else
			this->player->m_x += velx;
}
if(vely < 0)
{
	//player is moving up
	if(horizontal_collision(this->player->m_x, this->player->m_y + vely, this->player->w(), tilecoord))	//collision on the right side.
			this->player->m_y = (tilecoord+1)*32 +1;					//move to the edge of the tile (tile on the right -> mind the player width)
		else			//no collision
			this->player->m_y += vely;
	}		 
else if(vely > 0){	//moving down
	if(horizontal_collision(this->player->m_x, this->player->m_y + vely + this->player->h(), this->player->w(), tilecoord))		//collision on the left side
		this->player->m_y = tilecoord*32 - this->player->h()-1;				//move to the edge of the tile
	else
		this->player->m_y += vely;
}

/*
 * Hard-coded NPC pacing for demo. Shhhh!!!
 */
std::map<std::string, iq::entity_ptr>::iterator it;

if((it = this->entities.find("boss")) != this->entities.end())
{
	static int boss_right = true;
	boost::shared_ptr<iq::entity> boss = it->second;

	int boss_velocity = boss->speedy();
	const int boss_pace_left_y = 50;
	const int boss_pace_right_y = 200;

	if(!boss_right)
		boss_velocity = -boss_velocity;

	boss->m_y += boss_velocity;

	if(boss_right && boss->m_y >= boss_pace_right_y)
	{
		boss->begin_animation("walk_up", this->ms);
		boss_right = !boss_right;
		boss->m_y = boss_pace_right_y;
	}
	else if(!boss_right && boss->m_y <= boss_pace_left_y)
	{
		boss->begin_animation("walk_down", this->ms);
		boss_right = !boss_right;
		boss->m_y = boss_pace_left_y;
	}
}

if((it = this->entities.find("security")) != this->entities.end())
{
	static int security_right = true;
	boost::shared_ptr<iq::entity> security = it->second;

	int security_velocity = security->speedx();
	int security_pace_left_x = 400;
	int security_pace_right_x = 600;

	if(!security_right)
		security_velocity = -security_velocity;

	security->m_x += security_velocity;

	if(security_right && security->m_x >= security_pace_right_x)
	{
		security->begin_animation("walk_left", this->ms);
		security_right = !security_right;
		security->m_x = security_pace_right_x;
	}
	else if(!security_right && security->m_x <= security_pace_left_x)
	{
		security->begin_animation("walk_right", this->ms);
		security_right = !security_right;
		security->m_x = security_pace_left_x;
	}
}

	horizontal_collision(this->player->m_x, this->player->m_y, this->player->w(), tilecoord);
	vertical_collision(this->player->m_x, this->player->m_y, this->player->h(), tilecoord);

		std::sort(this->drawn_entities.begin(), this->drawn_entities.end(), iq::entity::y_comparer());

		IQ_APP_TRACE("} //iq::app::logic_gameplay()");
		
	}

	void app::logic_scripted(void)
	{
		IQ_APP_TRACE("iq::app::logic_scripted() {");

		IQ_APP_TRACE("} //iq::app::logic_scripted()");
	}

	void app::logic_setup(void)
	{
		IQ_APP_TRACE("iq::app::logic_setup() {");

		IQ_APP_TRACE("} //iq::app::logic_setup()");
	}

	void app::parse_args(int argc, char *argv[])
	{
		IQ_APP_TRACE("iq::app::parse_args(int, char *[]) {");

		IQ_APP_TRACE("} //iq::app::parse_args(int, char *[])");
	}

	void app::sem_destroy(sem_t *sem)
	{
		IQ_APP_TRACE("iq::app::sem_destroy(sem_t) {");

		if(sem)
		{
			/*
			 * This call was the cause of a fun bug. Instance scope versus
			 * global scope. :P The result was infinite recursion leading up
			 * to a segfault. I should have known better when I named it. I
			 * proabably should heed the lesson and name it something less
			 * mistakable, but I have lessons to learn and I intend to learn
			 * them. ^_^
			 */
			::sem_destroy(sem);
			delete sem;
		}

		IQ_APP_TRACE("} //iq::app::sem_destroy(sem_t)");
	}

	void app::set_state(gamestate state)
	{
std::map<std::string, iq::entity_ptr>::iterator it;
		this->state = state;

/*
 * h4x: just demonstrating animations. This should be removed eventually
 * (unless it turns out to be correct :P).
 */
//iq::uint j=0;
//const char *anim[] = {"walk_left", "walk_down", "walk_right"};

		switch(state)
		{
		case CREDITS:
			this->timer->stop();

			this->m_drawptr = &app::draw_credits;
			this->m_logicptr = &app::logic_credits;
			break;
		case GAMEPLAY:
			this->timer->reset();
			this->timer->start();

/*
 * h4x: just demonstrating animations. This should be removed eventually
 * (unless it turns out to be correct :P).
 */
//for(std::map<std::string, iq::entity_ptr>::iterator i=this->entities.begin(); i != this->entities.end() && j<sizeof(anim); i++, j++)
	//i->second->begin_animation(anim[j], this->ms);

this->player->begin_animation("walk_left", this->ms); // h4x: had to start player animation or things crash. Fix this.
if((it = this->entities.find("boss")) != this->entities.end())
	it->second->begin_animation("walk_down", this->ms);
if((it = this->entities.find("security")) != this->entities.end())
	it->second->begin_animation("walk_left", this->ms);

			this->m_drawptr = &app::draw_gameplay;
			this->m_logicptr = &app::logic_gameplay;
			break;
		case SCRIPTED:
			this->timer->stop();

			this->m_drawptr = &app::draw_scripted;
			this->m_logicptr = &app::logic_scripted;
			break;
		case SETUP:
			this->timer->stop();

			this->m_drawptr = &app::draw_setup;
			this->m_logicptr = &app::logic_setup;
			break;
		}
	}
}

