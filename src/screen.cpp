#include "screen.hpp"
#include "sdl2.hpp"

#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>
#include <string>

Screen::Screen()
	: SCREEN_WIDTH(1170), SCREEN_HEIGHT(525)
	, window(SDL_CreateWindow("Nighthawk - Kingdoms",
				 SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
				 SCREEN_WIDTH, SCREEN_HEIGHT, 0))
{	
	if (!window)
	{
		std::cout << "error - failed to open window\n" << SDL_GetError();
		return;
	}

	renderer.reset(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED));
	if (!renderer)
	{
		std::cout << "error - failed to create renderer\n" << SDL_GetError();
		return;
	}

	SDL_SetRenderDrawBlendMode(renderer.get(), SDL_BLENDMODE_BLEND);
}

void Screen::update()
{
	SDL_RenderPresent(renderer.get());
}

void Screen::clear()
{
	SDL_SetRenderDrawColor(renderer.get(), 120, 255, 0, 255);
	SDL_RenderClear(renderer.get());

	int fw = SCREEN_WIDTH / 5, fh = SCREEN_HEIGHT / 2;
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 2; ++j)
			Screen::get().image("../assets/grass.png", i * fw, j * fh, fw + 1, fh + 1);
	}
}

void Screen::rect(int x, int y, int width, int height, SDL_Color clr)
{
	SDL_Rect rect{ x, y, width, height };

	SDL_SetRenderDrawColor(renderer.get(), clr.r, clr.g, clr.b, 200);
	SDL_RenderFillRect(renderer.get(), &rect);
}

void Screen::text(std::string const& text, SDL_Color const& colour, std::string const& font, int size,
	int x, int y, sdl2::TTF_Align alignment)
{
	sdl2::font_ptr ttf_font(TTF_OpenFont(font.c_str(), size));
	sdl2::surface_ptr text_surface(TTF_RenderText_Solid(ttf_font.get(), text.c_str(), colour));
	sdl2::texture_ptr text_texture(SDL_CreateTextureFromSurface(renderer.get(), text_surface.get()));

	SDL_Rect text_rect;
	int s = TTF_SizeText(ttf_font.get(), text.c_str(), &text_rect.w, &text_rect.h);

	switch (alignment)
	{
	case sdl2::TTF_Align::LEFT:
		text_rect.x = x;
		text_rect.y = y;
		break;
	case sdl2::TTF_Align::CENTER:
		text_rect.x = x - (text_rect.w / 2);
		text_rect.y = y;
		break;
	case sdl2::TTF_Align::RIGHT:
		text_rect.x = (SCREEN_WIDTH - text_rect.w) - x;
		text_rect.y = y;
		break;
	};

	SDL_RenderCopy(renderer.get(), text_texture.get(), NULL, &text_rect);
}

void Screen::image(std::string const& file, int x, int y, int width, int height)
{
	SDL_Rect grass_rect{ x, y, width, height };

	sdl2::surface_ptr image(IMG_Load(file.c_str()));
	if (!image)
		std::cout << "[error]\n" << IMG_GetError() << '\n';

	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.get(), image.get());
	SDL_RenderCopy(renderer.get(), texture, NULL, &grass_rect);
}