
#include <app.hpp>

namespace iq
{
	bool app::close_button_pressed = false;
	const std::string app::DEFAULT_FILE = "config/app.config";

	app::app(int argc, char *argv[]):
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
			this->load();
		}
		catch(...)
		{
			this->deinitialize();
			throw;
		}

		this->demo_map.reset(new iq::tilemap("map/example.map", this->tiles));

		this->timer.reset(new iq::timer());

this->player->m_x = 300;
this->player->m_y = 150;

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

/*
 * h4x: temporary just to draw TILEMAPS and have something pretty to look
 * at. ^_^
 */

demo_map->draw(scrbuf, this->player);

/*
 * h4x: temporary just to draw animations and have something pretty to look
 * at. ^_^
 */
iq::BITMAP_ptr bitmap;
iq::uint j=0;
int n[] = {-55, 0, +55};

int x = (this->scrbuf->w / 2);
int y = (this->scrbuf->h / 2);

for(std::map<std::string, iq::entity_ptr>::iterator i=this->entities.begin(); i != this->entities.end() && j<sizeof(n); i++, j++)
{
	bitmap = i->second->current_frame(this->ms);
	masked_blit(bitmap.get(), this->scrbuf.get(), 0, 0, x - (i->second->w() / 2) + n[j], y - (i->second->h() / 2), bitmap->w, bitmap->h);
}

		//textprintf_ex(this->scrbuf.get(), font, 20, 20, WHITE, -1,
				//"frame-count: %d",
				//g_total_frames);
		textprintf_ex(this->scrbuf.get(), font, 20, 40, BLUE, -1,
				"time: %s", this->timer->to_str().c_str());
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
if(key[KEY_UP])
{
	if(this->player->facing() != entity::FACING_UP || !this->player->current_animation()->second->playing())
	{
		this->player->face(entity::FACING_UP);
		this->player->begin_animation("walk_up", this->ms);
	}
}
else if(key[KEY_RIGHT])
{
	if(this->player->facing() != entity::FACING_RIGHT || !this->player->current_animation()->second->playing())
	{
		this->player->face(entity::FACING_RIGHT);
		this->player->begin_animation("walk_right", this->ms);
	}
}
else if(key[KEY_DOWN])
{
	if(this->player->facing() != entity::FACING_DOWN || !this->player->current_animation()->second->playing())
	{
		this->player->face(entity::FACING_DOWN);
		this->player->begin_animation("walk_down", this->ms);
	}
}
else if(key[KEY_LEFT])
{
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

/*
		g_total_frames++;
		g_frames_this_second++;
*/

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
		this->state = state;

/*
 * h4x: just demonstrating animations. This should be removed eventually
 * (unless it turns out to be correct :P).
 */
iq::uint j=0;
const char *anim[] = {"walk_left", "walk_down", "walk_right"};

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
for(std::map<std::string, iq::entity_ptr>::iterator i=this->entities.begin(); i != this->entities.end() && j<sizeof(anim); i++, j++)
	i->second->begin_animation(anim[j], this->ms);

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

