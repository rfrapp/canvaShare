
#include "LoginMenu.h"
#include "File.h"
#include "Program.h"

LoginMenu::LoginMenu(Program *p, SDL_Renderer * r, int _w, int _h) 
	    : logged_in(false), w(_w), h(_h), renderer(r),
	      show_message(true), parent(p)
{
}

void LoginMenu::draw()
{
	SDL_Rect rect = {w / 2 - 345 / 2, h / 2 - 60, 345, 205};

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

	if (show_message)
		message_surface->render(renderer, w / 2 - message_surface->get_width() / 2, rect.y - 90);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);


	SDL_Rect rbutton_rect = register_button->rect().getSDL_Rect();
	rbutton_rect.x -= 5;
	rbutton_rect.y -= 5;
	rbutton_rect.w += 10;
	rbutton_rect.h += 10;

	SDL_RenderDrawRect(renderer, &rbutton_rect);

	register_button->draw();
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
	SDL_Rect rect = {w / 2 - 345 / 2, h / 2 - 60, 345, 205};

	login_button = new Button(0, this, renderer, font, 92, 30, rect.x + 72, h / 2 + 100, "Login",
		        "images/icons.png", 404, 0, 92, 30);

	username_label = new TextSurface(font, 0, 0, 0, 0, false);
	password_label = new TextSurface(font, 0, 0, 0, 0, false);

	username_label->set_text(renderer, "Username ");
	password_label->set_text(renderer, "Password ");

	username_box = new TextBox(renderer, font, 200, 10, rect.x + 10 + username_label->get_width(), rect.y + 10, true, false);
	password_box = new TextBox(renderer, font, 200, 10, rect.x + 10 + username_label->get_width(), rect.y + 80, true, false);
	password_box->set_password_box(true);

	title_surface = new TextSurface(title_font, 0, 0, 0, 0, false);
	title_surface->set_text(renderer, "canvaShare");

	message_surface = new TextSurface(font, 0, 0, 0, 0, false);

	register_button = new Button(1, this, renderer, font, 78, 30, rect.x + 202, h / 2 + 100, "Register",
		        "images/icons.png", 522, 0, 78, 30);

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
	// File f("user.txt");
	// std::vector< std::vector< std::string > > users;
	// bool found = false;

	// users = f.get_lines_delimited();

	// std::cout << users.size() << std::endl;

	// for (int i = 0; i < users.size(); i++)
	// {
	// 	if (users[i].size() > 1)
	// 	{
	// 		if (users[i][0] == username_box->text()
	// 			&& users[i][1] == password_box->text())
	// 			found = true;
	// 	}
	// }

	// logged_in = found; 
	// login_button->set_focus(false);

	// if (!logged_in)
	// {
	// 	show_message = true;
	// 	message_surface->set_text(renderer, "Invalid Credentials");
	// }

	std::string str = "login\n" + username_box->text() + "\n" + password_box->text() + "\n";

	parent->send_message(str);
}

void LoginMenu::receive_message(std::string message)
{
	if (message == "logged in")
		logged_in = true;
	else if (message == "not logged in")
	{
		show_message = true;
		message_surface->set_text(renderer, "Invalid Credentials");
		login_button->set_focus(false);
	}
	else if (message == "already logged in")
	{
		show_message = true;
		message_surface->set_text(renderer, "This user is already logged in");
		login_button->set_focus(false);
	}
	else if (message == "already registered")
	{
		show_message = true;
		message_surface->set_text(renderer, "You are already registered");
		register_button->set_focus(false);
	}
	else if (message == "registered")
	{
		SDL_Rect rect = {w / 2 - 345 / 2, h / 2 - 60, 345, 205};
		
		show_message = true;
		message_surface->set_text(renderer, "You have been registered!");
		register_button->set_focus(false);

		delete username_box;
		delete password_box;

		username_box = new TextBox(renderer, font, 200, 10, rect.x + 10 + username_label->get_width(), rect.y + 10, true, false);
		password_box = new TextBox(renderer, font, 200, 10, rect.x + 10 + username_label->get_width(), rect.y + 80, true, false);
		password_box->set_password_box(true);
	}
}

void LoginMenu::register_user()
{
	if (username_box->text() != "" && password_box->text() != "")
	{
		// File f("user.txt");
		// std::vector< std::vector< std::string > > users;
		// bool found = false;

		// users = f.get_lines_delimited();

		// for (int i = 0; i < users.size(); i++)
		// {
		// 	if (users[i][0] == username_box->text())
		// 		found = true;
		// }

		// if (!found)
		// {
		// 	std::vector< std::string > v; 
		// 	v.push_back(username_box->text());
		// 	v.push_back(password_box->text());

		// 	users.push_back(v);

		// 	f.write_delimited_to_file(users);

		// 	register_button->set_focus(false);
		// 	show_message = true;
		// 	message_surface->set_text(renderer, "You have been registered!");

		// 	delete username_box;
		// 	delete password_box;

		// 	username_box = new TextBox(renderer, font, 200, 10, rect.x + 10 + username_label->get_width(), rect.y + 10, true, false);
		// 	password_box = new TextBox(renderer, font, 200, 10, rect.x + 10 + username_label->get_width(), rect.y + 80, true, false);
		// 	password_box->set_password_box(true);
		// }
		// else
		// {
		// 	show_message = true;
		// 	message_surface->set_text(renderer, "This username is already registered!");
		// 	register_button->set_focus(false);	
		// }

		std::string str = "register\n" + username_box->text() + "\n" + password_box->text() + "\n";
		parent->send_message(str);

		// username_box->set_text(renderer, "");
		// password_box->set_text(renderer, "");
	}
	else
	{
		show_message = true;
		message_surface->set_text(renderer, "Please fill out all values in the form");
		register_button->set_focus(false);
	}
}
