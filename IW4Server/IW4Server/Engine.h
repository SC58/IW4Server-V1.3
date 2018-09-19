double strtod(const char *str, char **endptr)
{
	double number;
	int exponent;
	int negative;
	char *p = (char *)str;
	double p10;
	int n;
	int num_digits;
	int num_decimals;

	while (isspace(*p))
		p++;

	negative = 0;
	switch (*p)
	{
	case '-':
		negative = 1;
	case '+':
		p++;
	}

	number = 0.;
	exponent = 0;
	num_digits = 0;
	num_decimals = 0;

	while (isdigit(*p))
	{
		number = number * 10. + (*p - '0');
		p++;
		num_digits++;
	}

	if (*p == '.')
	{
		p++;

		while (isdigit(*p))
		{
			number = number * 10. + (*p - '0');
			p++;
			num_digits++;
			num_decimals++;
		}

		exponent -= num_decimals;
	}

	if (num_digits == 0)
	{
		return 0.0;
	}

	if (negative)
		number = -number;

	if (*p == 'e' || *p == 'E')
	{
		negative = 0;
		switch (*++p)
		{
		case '-':
			negative = 1;
		case '+':
			p++;
		}
		n = 0;
		while (isdigit(*p))
		{
			n = n * 10 + (*p - '0');
			p++;
		}

		if (negative)
		{
			exponent -= n;
		}
		else
		{
			exponent += n;
		}
	}

	if (exponent < DBL_MIN_EXP || exponent > DBL_MAX_EXP)
	{
		return HUGE_VAL;
	}
	p10 = 10.;
	n = exponent;
	if (n < 0)
		n = -n;
	while (n)
	{
		if (n & 1)
		{
			if (exponent < 0)
			{
				number /= p10;
			}
			else
			{
				number *= p10;
			}
		}
		n >>= 1;
		p10 *= p10;
	}

	if (endptr)
		*endptr = p;

	return number;
}

double atof(const char *str)
{
	return strtod(str, NULL);
}

void SetBuildVersion(const char *str, float location)
{
	WriteProcessMemory<float>(0x728DD0, location, sizeof(float));
	WriteProcessMemory<uint32_t>(0x728AF8, (uint32_t)str, sizeof(uint32_t));
}

bool UI_VerifyString(const char *str, bool psnName)
{
	for (size_t i = 0; i < strlen(str); i++)
	{
		if (psnName && str[i] == ' ')
			return true;

		for (size_t c = 0; c < sizeof(invalidChars); c++)
			if (str[i] == invalidChars[c])
				return true;
	}
	return false;
}

bool ButtonPressed(keyNum_t key, int num)
{
	if (Sys_Milliseconds() - lastTimeButton[num] < 0xAA)
		return false;
	if (Key_IsDown(0, key))
	{
		lastTimeButton[num] = Sys_Milliseconds();
		return true;
	}
	else
		return false;
}

int SV_Cmd_Argc()
{
	return sv_cmd_args->argc[sv_cmd_args->nesting];
}

const char * SV_Cmd_Argv(int argIndex)
{
	if (argIndex >= sv_cmd_args->argc[sv_cmd_args->nesting])
		return "";
	else
		return sv_cmd_args->argv[sv_cmd_args->nesting][argIndex];
}

int MSG_GetByte(msg_t *msg, int where)
{
	if (where < msg->cursize)
		return msg->data[where];
	else
		return msg->splitData[where - msg->cursize];
}

int MSG_ReadBits(msg_t *msg, int bits)
{
	int value = 0;
	for (int i = 0; i < bits; i++)
	{
		int bit = msg->bit & 7;
		if (!bit)
		{
			if (msg->readcount >= msg->splitSize + msg->cursize)
			{
				msg->overflowed = 1;
				return -1;
			}
			msg->bit = 8 * msg->readcount++;
		}
		value |= ((MSG_GetByte(msg, msg->bit >> 3) >> bit) & 1) << i;
		msg->bit++;
	}
	return value;
}

bool IsInGame()
{
	return Dvar_GetBool("cl_ingame");
}

bool IsOnlineGame()
{
	return Dvar_GetBool("onlinegame");
}

bool IsPrivateMatch()
{
	return Dvar_GetBool("xblive_privatematch");
}

int GetMaxClients()
{
	return Dvar_GetInt("sv_maxclients");
}

bool IsUIActive()
{
	return CL_IsUIActive(0);
}

int GetSelfClientNum()
{
	return CG_GetClientNum(0);
}

bool IsServerHost()
{
	return Session_IsHost(g_serverSession, GetSelfClientNum());
}

bool IsHost(int clientNum)
{
	return Session_IsHost(g_serverSession, clientNum);
}

bool IsPublicLobbyActive()
{
	return Party_IsRunning(g_lobbyData) && !IsPrivateMatch();
}

void SelfDisconnect()
{
	CL_DisconnectLocalClient(0);
}

void OpenSelectMapMenu()
{
	UI_OpenMenu(0, "settings_map");
}

void PlaySound(const char *soundName)
{
	UI_PlayLocalSoundAliasByName(0, soundName);
}

void SetDvar(const char *dvarName, const char *value)
{
	Dvar_SetFromStringByName(dvarName, value);
}

void SetSelfFOV(float value)
{
	SetDvar("cg_fov", va("%g", value));
}

void ResetDvar(const char *dvarName)
{
	Dvar_Reset(Dvar_FindVar(dvarName), DVAR_SOURCE_EXTERNAL);
}

bool SV_IsPlayer(int entNum)
{
	return g_entities[entNum].client;
}

bool SV_IsClientActive(int clientNum)
{
	return svs->clients[clientNum].state == CS_ACTIVE;
}

const char * SV_GetClientName(int clientNum)
{
	const char *clientName = svs->clients[clientNum].name;
	if (SV_IsClientActive(clientNum) && strlen(clientName))
		return clientName;
	else
		return "**Invalid**";
}

void iPrintln(const char *messageFormat, ...)
{
	va_list argptr;
	char text[MAX_STRING_CHARS];
	va_start(argptr, messageFormat);
	_vsnprintf(text, MAX_STRING_CHARS, messageFormat, argptr);
	va_end(argptr);
	CG_GameMessage(0, text);
}

void SetClientDvar(int clientNum, const char *dvarName, const char *value)
{
	SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, va("v %s \"%s\"", dvarName, value));
}

void KickClient(int clientNum, const char *reasonFormat, ...)
{
	va_list argptr;
	char text[MAX_STRING_CHARS];
	va_start(argptr, reasonFormat);
	_vsnprintf(text, MAX_STRING_CHARS, reasonFormat, argptr);
	va_end(argptr);
	if (SV_IsClientActive(clientNum))
		SV_GameSendServerCommand(clientNum, SV_CMD_RELIABLE, va("w \"%s\"", text));
	else
		iPrintln("You cannot kick a Invalid client.");
}

int SV_LiveStorage_DataGetInternalInt(int clientNum, const char *navStrings, int navStringCount)
{
	StructuredDataBuffer buffer;
	buffer.data = SV_GetClientPersistentDataBuffer(clientNum);
	buffer.size = 0x1FFC;
	return LiveStorage_DataGetInternalInt((int)&navStrings, navStringCount, &buffer);
}

int SV_GetClientPrestige(int clientNum)
{
	return SV_LiveStorage_DataGetInternalInt(clientNum, "prestige", 1);
}

uint64_t SV_GetClientXUID(int clientNum)
{
	return Session_GetXuidEvenIfInactive(g_serverSession, clientNum);
}

void SV_ClearServerBans()
{
	serverBansCount = 0;
	for (uint32_t banSlot = 0; banSlot < MAX_SERVER_BANS; banSlot++)
		serverBans[banSlot].xuid = 0;
}

int SV_FindFreeBanSlot()
{
	for (uint32_t banSlot = 0; banSlot < MAX_SERVER_BANS; banSlot++)
		if (!serverBans[banSlot].xuid)
			return banSlot;
}

bool SV_IsClientBanned(int clientNum)
{
	if (serverBansCount)
		for (uint32_t banSlot = 0; banSlot < MAX_SERVER_BANS; banSlot++)
			return serverBans[banSlot].xuid == SV_GetClientXUID(clientNum);
}

void SV_BanClient(int clientNum)
{
	if (serverBansCount >= MAX_SERVER_BANS)
	{
		DrawOkayMessageDialog("Error:\n\nMax number of bans executed.\n\nClear all bans or unban a client by name in host menu before you can ban anyone again.");
	}
	else
	{
		if (SV_IsClientActive(clientNum))
		{
			if (IsHost(clientNum))
			{
				iPrintln("You cannot ban yourself silly.");
			}
			else
			{
				if (SV_IsClientBanned(clientNum))
				{
					char clientName[MAX_NETNAME];
					ClientCleanName(SV_GetClientName(clientNum), clientName, MAX_NETNAME);
					iPrintln("'%s' is already ban, They should not be in the game.", clientName);
				}
				else
				{
					serverBans[SV_FindFreeBanSlot()].xuid = SV_GetClientXUID(clientNum);
					KickClient(clientNum, "You have been permanently banned from playing in '%s' server.", xenonUserData->gamertag);
					serverBansCount++;
				}
			}
		}
		else
		{
			iPrintln("You cannot ban a Invalid client.");
		}
	}
}

bool SV_UnbanClient(const char *clientName)
{
	for (uint32_t banSlot = 0; banSlot < MAX_SERVER_BANS; banSlot++)
	{
		if (serverBans[banSlot].xuid == bdAuthUtility_getUserID(clientName))
		{
			serverBans[banSlot].xuid = 0;
			serverBansCount--;
			return true;
		}
		else
		{
			return false;
		}
	}
}

void UnbanClientKeyboardCallback(int localClientNum, bool ifExecuted, const char *text)
{
	if (ifExecuted && strlen(text))
	{
		if (SV_UnbanClient(text))
			iPrintln("'%s' has been unban.", text);
		else
			iPrintln("'%s' is not in ban list.", text);
	}
}

void FOVKeyboardCallback(int localClientNum, bool ifExecuted, const char *text)
{
	if (ifExecuted && strlen(text))
	{
		float fovText = atof(text);
		if (fovText >= 60 && fovText <= 120)
		{
			SelfFOVValue = fovText;
			iPrintln("FOV Set to %g", fovText);
		}
	}
}

void DrawKeyboard(const char *title, const char *presetMessage, size_t size, uint32_t panelMode, UI_KeyboardCallback function)
{
	WriteProcessMemory<uint16_t>(0x2F7BDA, HIWORD(panelMode), sizeof(uint16_t));
	OpenKeyboard(0, title, presetMessage, size, function);
	WriteProcessMemory<uint16_t>(0x2F7BDA, HIWORD(CELL_OSKDIALOG_PANELMODE_ALPHABET), sizeof(uint16_t));
}

bool Content_IsDLC1Installed()
{
	return !strcmp(contentPack->dlcName1, "DLC_1") || !strcmp(contentPack->dlcName2, "DLC_1");
}

bool Content_IsDLC2Installed()
{
	return !strcmp(contentPack->dlcName1, "DLC_2") || !strcmp(contentPack->dlcName2, "DLC_2");
}

bool Content_IsAnyDLCPackInstalled()
{
	return Content_IsDLC1Installed() || Content_IsDLC2Installed();
}

int Content_GetInstalledDLCContentPackIndex()
{
	if (Content_IsDLC1Installed() && Content_IsDLC2Installed())
		return 3;
	else if (Content_IsDLC1Installed() || Content_IsDLC2Installed())
		return 2;
	else
		return 1;
}