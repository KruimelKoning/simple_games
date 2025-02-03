#include "../header/Text.hpp"

int	extractValue(std::string line, std::string key)
{
	uint64_t	start = line.find(key);
	if (start == line.npos)
		return -100;

	try
	{
		int	tmp = std::stoi(line.substr(start + key.length()));
		return tmp;
	}
	catch(const std::exception& e)
	{
		std::cerr << "oeps\n";
		return -100;
	}
}

bool	loadFontBM(SDL_Renderer* renderer)
{
	fontTexture = IMG_LoadTexture(renderer, FNT_TEXTURE_PATH);
	if (fontTexture == nullptr)
		return false;

	std::ifstream	fntFile(FNT_PATH);
	if (fntFile.is_open() == false)
		return SDL_DestroyTexture(fontTexture), false;
	
	std::string	line;
	while (std::getline(fntFile, line))
	{
		if (line.find("char id=") == line.npos) continue;

		int	id, x, y, w, h, xoffset, yoffset, xadvance;
		if ((id       = extractValue(line, "id="))       == -100) return false;
		if ((x        = extractValue(line, "x="))        == -100) return false;
		if ((y        = extractValue(line, "y="))        == -100) return false;
		if ((w        = extractValue(line, "width="))    == -100) return false;
		if ((h        = extractValue(line, "height="))   == -100) return false;
		if ((xoffset  = extractValue(line, "xoffset="))  == -100) return false;
		if ((yoffset  = extractValue(line, "yoffset="))  == -100) return false;
		if ((xadvance = extractValue(line, "xadvance=")) == -100) return false;

		tokenMap[id] = { xoffset, yoffset, xadvance, {x, y, w, h} };
	}
	return true;
}

void	renderText(SDL_Renderer* renderer, const std::string& text, int x, int y)
{
	int	xoffset = 0;
	for (char c : text)
	{
		if (tokenMap.find(c) == tokenMap.end()) continue;

		Token&		t = tokenMap[c];
		SDL_Rect	destRect = { x + xoffset + t.xoffset, y + t.yoffset, t.srcRect.w, t.srcRect.h };

		// std::cout << "\nRect | x: " << destRect.x << " y: " << destRect.y << " w: " << destRect.w << " h: " << destRect.h << std::endl;
		SDL_RenderCopy(renderer, fontTexture, &t.srcRect, &destRect);
		xoffset += t.xadvance;
	}
	SDL_RenderPresent(renderer);
}