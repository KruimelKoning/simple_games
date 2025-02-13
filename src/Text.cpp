#include "../header/Text.hpp"

bool	extractValue(std::string line, std::string key, int& i)
{
	uint64_t	start = line.find(key);
	if (start == line.npos)
		return false;

	try
	{
		int	tmp = std::stoi(line.substr(start + key.length()));
		i = tmp;
	}
	catch(const std::exception& e)
	{
		std::cerr << "oeps\n";
		return false;
	}
	return true;
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
		if (extractValue(line, "id=", id)             == false) return false;
		if (extractValue(line, "x=", x)               == false) return false;
		if (extractValue(line, "y=", y)               == false) return false;
		if (extractValue(line, "width=", w)           == false) return false;
		if (extractValue(line, "height=", h)          == false) return false;
		if (extractValue(line, "xoffset=", xoffset)   == false) return false;
		if (extractValue(line, "yoffset=", yoffset)   == false) return false;
		if (extractValue(line, "xadvance=", xadvance) == false) return false;

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

		SDL_RenderCopy(renderer, fontTexture, &t.srcRect, &destRect);
		xoffset += t.xadvance;
	}
	SDL_RenderPresent(renderer);
}

void	renderCenteredText(SDL_Renderer* renderer, const std::string& text, SDL_Rect rect)
{
	int	width = 0;
	for (char c : text)
		width += tokenMap[c].xadvance;
	
	int	x = rect.x + (rect.w / 2) - (width / 2);

	int	xoffset = 0;
	for (char c : text)
	{
		if (tokenMap.find(c) == tokenMap.end()) continue;

		Token&		t = tokenMap[c];
		SDL_Rect	destRect = { x + xoffset + t.xoffset, (rect.y + rect.h / 4) + t.yoffset, t.srcRect.w, t.srcRect.h };

		SDL_RenderCopy(renderer, fontTexture, &t.srcRect, &destRect);
		xoffset += t.xadvance;
	}
	SDL_RenderPresent(renderer);
}