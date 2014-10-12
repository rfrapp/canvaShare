
#include "LoginMenu.h"
#include "File.h"

void LoginMenu::draw()
{
	username_box->draw();
	password_box->draw();
	login_button->draw();

	SDL_Rect rect = {w / 2 - 172, h / 2 - 60, 345, 205};
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(renderer, &rect);

	SDL_Rect button_rect = login_button->rect().getSDL_Rect();
	button_rect.x -= 5;
	button_rect.y -= 5;
	button_rect.w += 10;
	button_rect.h += 10;
	SDL_RenderDrawRect(renderer, &button_rect);

	username_label->render(renderer, rect.x + 10, rect.y + 10);
	password_label->render(renderer, rect.x + 10, rect.y + 80);
}

void LoginMenu::handle_input(SDL_Event * e)
{
	if (e->type == SDL_KEYDOWN)
	{
		if (e->key.keysym.sym == SDLK_TAB)
		{
			if (username_box->has_focus())
			{
				username_box->set_focus(false);
				password_box->set_focus(true);
			}
		}
		else if (e->key.keysym.sym == SDLK_RETURN)
		{
			if (password_box->has_focus())
			{
				authenticate();
			}
		}
	}

	username_box->handle_input(e);
	password_box->handle_input(e);
	login_button->handle_input(e);
}

bool LoginMenu::load_media()
{
	font = new Font("fonts/cour.ttf", 24);

	return true;
}

void LoginMenu::init_controls()
{ 
	username_box = new TextBox(renderer, font, 200, 10, w / 2 - 40, h / 2 - 50, true, false);
	password_box = new TextBox(renderer, font, 200, 10, w / 2 - 40, h / 2 + 20, true, false);
	password_box->set_password_box(true);

	login_button = new Button(0, this, renderer, font, 92, 30, w / 2 - 45, h / 2 + 100, "Login",
		        "images/icons.png", 404, 0, 92, 30);

	username_label = new TextSurface(font, 0, 0, 0, 0, false);
	password_label = new TextSurface(font, 0, 0, 0, 0, false);

	username_label->set_text(renderer, "Username ");
	password_label->set_text(renderer, "Password ");
}

void LoginMenu::get_notification(std::string event, int id)
{
	if (id == 0)
	{
		std::cout << "here" << std::endl;
		authenticate();
	}
}

void LoginMenu::authenticate()
{
	File f("user.txt");
	std::vector< std::vector< std::string > > users;
	bool found = false;

	users = f.get_lines_delimited();

	std::cout << users.size() << std::endl;

	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].size() > 1)
		{
			if (users[i][0] == username_box->text()
				&& users[i][1] == password_box->text())
				found = true;
		}
	}

	logged_in = found; 
}
