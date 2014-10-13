
#include "LoginMenu.h"
#include "File.h"

void LoginMenu::draw()
{
	SDL_Rect rect = {w / 2 - 450, h / 2 - 60, 345, 205};

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(renderer, &rect);

	SDL_Rect button_rect = login_button->rect().getSDL_Rect();
	button_rect.x -= 5;
	button_rect.y -= 5;
	button_rect.w += 10;
	button_rect.h += 10;
	SDL_RenderDrawRect(renderer, &button_rect);

	username_box->draw();
	password_box->draw();
	login_button->draw();

	username_label->render(renderer, rect.x + 10, rect.y + 10);
	password_label->render(renderer, rect.x + 10, rect.y + 80);
	title_surface->render(renderer, w / 2 - title_surface->get_width() / 2, rect.y - 170);

	// draw register box
	SDL_Rect otherrect = {w / 2 + 100, h / 2 - 60, 345, 205};
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(renderer, &otherrect);

	username_label->render(renderer, otherrect.x + 10, otherrect.y + 10);
	password_label->render(renderer, otherrect.x + 10, otherrect.y + 80);

	SDL_Rect rbutton_rect = register_button->rect().getSDL_Rect();
	rbutton_rect.x -= 5;
	rbutton_rect.y -= 5;
	rbutton_rect.w += 10;
	rbutton_rect.h += 10;

	SDL_RenderDrawRect(renderer, &rbutton_rect);

	register_button->draw();
	new_username_box->draw();
	new_password_box->draw();
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
			else if (new_username_box->has_focus())
			{
				new_username_box->set_focus(false);
				new_password_box->set_focus(true);
			}
		}
		else if (e->key.keysym.sym == SDLK_RETURN)
		{
			if (password_box->has_focus())
			{
				authenticate();
			}
			else if (new_password_box->has_focus())
			{
				register_user();
			}
		}
	}

	username_box->handle_input(e);
	password_box->handle_input(e);
	login_button->handle_input(e);

	new_username_box->handle_input(e);
	new_password_box->handle_input(e);
	register_button->handle_input(e);
}

bool LoginMenu::load_media()
{
	font = new Font("fonts/cour.ttf", 24);
	title_font = new Font("fonts/cour.ttf", 64, TTF_STYLE_BOLD);

	return true;
}

void LoginMenu::init_controls()
{ 
	SDL_Rect rect = {w / 2 - 300, h / 2 - 60, 345, 205};

	login_button = new Button(0, this, renderer, font, 92, 30, (rect.x + rect.w) / 2 - 106, h / 2 + 100, "Login",
		        "images/icons.png", 404, 0, 92, 30);

	username_label = new TextSurface(font, 0, 0, 0, 0, false);
	password_label = new TextSurface(font, 0, 0, 0, 0, false);

	username_label->set_text(renderer, "Username ");
	password_label->set_text(renderer, "Password ");

	username_box = new TextBox(renderer, font, 200, 10, rect.x + 10 + username_label->get_width() - 150 , rect.y + 10, true, false);
	password_box = new TextBox(renderer, font, 200, 10, rect.x + 10 + username_label->get_width() - 150, rect.y + 80, true, false);
	password_box->set_password_box(true);

	title_surface = new TextSurface(title_font, 0, 0, 0, 0, false);
	title_surface->set_text(renderer, "canvaShare");

	SDL_Rect otherrect = {w / 2 + 100, h / 2 - 60, 345, 205};

	register_button = new Button(1, this, renderer, font, 78, 30, otherrect.x + + 134, h / 2 + 100, "Register",
		        "images/icons.png", 522, 0, 78, 30);

	new_username_box = new TextBox(renderer, font, 200, 10, otherrect.x + 10 + username_label->get_width() , rect.y + 10, true, false);
	new_password_box = new TextBox(renderer, font, 200, 10, otherrect.x + 10 + username_label->get_width(), rect.y + 80, true, false);
	new_password_box->set_password_box(true);
}

void LoginMenu::get_notification(std::string event, int id)
{
	if (id == 0)
	{
		authenticate();
	}
	else if (id == 1)
	{
		register_user();
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
	login_button->set_focus(false);
}

void LoginMenu::register_user()
{
	File f("user.txt");
	std::vector< std::vector< std::string > > users;

	users = f.get_lines_delimited();

	std::vector< std::string > v; 
	v.push_back(new_username_box->text());
	v.push_back(new_password_box->text());

	users.push_back(v);

	f.write_delimited_to_file(users);

	new_username_box->set_text(renderer, "");
	new_password_box->set_text(renderer, "");
	register_button->set_focus(false);
}
