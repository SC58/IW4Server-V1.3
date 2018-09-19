void RGBA(float *rgba, uint32_t hexColor)
{
	rgba[0] = ((hexColor >> 24) & 0xFF) / 255.0;
	rgba[1] = ((hexColor >> 16) & 0xFF) / 255.0;
	rgba[2] = ((hexColor >> 8) & 0xFF) / 255.0;
	rgba[3] = ((hexColor)& 0xFF) / 255.0;
}

ScreenPlacement * GetActiveScreenPlacement()
{
	return ScrPlace_GetActivePlacement(0);
}

Font_s * GetFont(int fontEnum, float scale)
{
	return UI_GetFontHandle(GetActiveScreenPlacement(), fontEnum, scale);
}

Material * GetMaterial(const char *materialName)
{
	return Material_RegisterHandle(materialName, 0);
}

float GetTextWidth(const char *text, int fontEnum, float scale)
{
	return UI_TextWidth(text, 0, GetFont(fontEnum, scale), scale);
}

float CalculateCenterWidthAdjustment(float width)
{
	return (SCREEN_WIDTH - width) / 2;
}

float CalculateCenterHeightAdjustment(float height)
{
	return (SCREEN_HEIGHT - height) / 2;
}

void DrawText(const char *text, int fontEnum, float x, float y, float scale, uint32_t color)
{
	float textColor[4];
	RGBA(textColor, color);
	UI_DrawText(GetActiveScreenPlacement(), text, INT_MAX, GetFont(fontEnum, scale), x, y, 0, 0, scale, textColor, 0);
}

void DrawHandlePic(float x, float y, float width, float height, uint32_t color, const char *material)
{
	float picColor[4];
	RGBA(picColor, color);
	UI_DrawHandlePic(GetActiveScreenPlacement(), x, y, width, height, 0, 0, picColor, GetMaterial(material));
}

void DrawCenterScreenOptions(const char **option, uint32_t textArrayLenInterval)
{
	for (uint32_t i = 0; i < textArrayLenInterval; i++)
	{
		float w = GetTextWidth(option[i], UI_FONT_DEFAULT, .35);
		DrawText(option[i], UI_FONT_DEFAULT, CalculateCenterWidthAdjustment(w), 100 + (i * 16), .35, colorWhite);
	}
}

void DrawCenterScreenClientList()
{
	for (uint32_t i = 0; i < GetMaxClients(); i++)
	{
		char clientName[MAX_CLIENTS][MAX_NETNAME];
		ClientCleanName(SV_GetClientName(i), clientName[i], MAX_NETNAME);
		float w = GetTextWidth(clientName[i], UI_FONT_DEFAULT, .35);
		DrawText(clientName[i], UI_FONT_DEFAULT, CalculateCenterWidthAdjustment(w), 100 + (i * 16), .35, colorWhite);
	}
}

void DrawCenterScreenTitle(const char *title)
{
	float w = GetTextWidth(title, UI_FONT_OBJECTIVE, .35);
	DrawText(title, UI_FONT_OBJECTIVE, CalculateCenterWidthAdjustment(w), 80, .35, colorWhite);
}

void DrawCenterScreenScrollBar(int scrollY)
{
	DrawHandlePic(208, 86 + (scrollY * 16), 224, 13, colorGreen, "white");
}

void DrawCenterScreenHud()
{
	DrawHandlePic(CalculateCenterWidthAdjustment(224), CalculateCenterHeightAdjustment(512), 224, 512, colorLtMatte, "white");
}

void CG_DrawFPS()
{
	if (fps->index > FPS_FRAMES)
	{
		int total = 0;
		for (int i = 0; i < FPS_FRAMES; i++)
			total += fps->previousTimes[i];

		if (!total)
			total = 1;

		int fps = floorf((32000.5 / total) + .5);

		ScreenPlacement *scrPlace = GetActiveScreenPlacement();

		float textColor[4];
		RGBA(textColor, colorWhite);
		CG_DrawDevString(scrPlace, scrPlace->virtualViewableMax[0] - scrPlace->virtualViewableMin[0], 0, .5, .5, va("FPS: %i", fps), textColor, CG_ALIGN_TOP | CG_ALIGN_RIGHT, GetFont(UI_FONT_HUDSMALL, .5));
	}
}