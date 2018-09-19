#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <float.h>
#include <np.h>
#include <fastmath.h>
#include <sys/sys_time.h>
#include <sys/time_util.h>
#include <sys/prx.h>
#include <sys/ppu_thread.h>
#include <sys/tty.h>
#include <sys/timer.h>
#include <sys/process.h>
#include <sysutil/sysutil_msgdialog.h>
#include <sysutil/sysutil_oskdialog.h>
#include <sysutil/sysutil_userinfo.h>
#include <cell/sysmodule.h>

#include "StructAndEnum.h"
#include "OPD.h"
#include "PS3.h"
#include "Engine.h"
#include "EngineDrawing.h"
#include "Menu.h"
#include "Stub.h"

SYS_MODULE_INFO(IW4Server, 0, 1, 1);
SYS_MODULE_START(IW4Server_prx_entry);

void ClientCommand(int clientNum)
{
	gentity_s *ent = &g_entities[clientNum];
	if (ent->client)
	{
		const char *cmdArgZero = SV_Cmd_Argv(0);

		if (!strcmp(cmdArgZero, "notify"))
		{
			Cmd_CheckNetNotify_f(clientNum);
		}
		else if (!strcmp(cmdArgZero, "score"))
		{
			SendScoreboard(ent);
		}
		else if (!strcmp(cmdArgZero, "mr"))
		{
			if (!strcmp(SV_Cmd_Argv(3), "endround"))
			{
				if (IsHost(clientNum))
					Cmd_MenuResponse_f(ent);
				else
					KickClient(clientNum, "'%s' server detected you where trying to end the game, Therefore you have been kicked.", xenonUserData->gamertag);
			}
			else
			{
				Cmd_MenuResponse_f(ent);
			}
		}
	}
}

void SV_ExecuteClientCommand(client_t *cl, const char *s, int fromOldServer)
{
	ucmd_t *u;
	SV_Cmd_TokenizeString(s);

	for (int i = 0; i < SV_Cmd_Argc(); i++)
	{
		const char *cmdArgv = SV_Cmd_Argv(i);

		if (!strcmp(cmdArgv, "savereplay") || !strcmp(cmdArgv, "say") || !strcmp(cmdArgv, "say_team") || !strcmp(cmdArgv, "follownext") || !strcmp(cmdArgv, "followprev") || !strcmp(cmdArgv, "where") || !strcmp(cmdArgv, "callvote") || !strcmp(cmdArgv, "vote") || !strcmp(cmdArgv, "entitylist") || !strcmp(cmdArgv, "set") || !strcmp(cmdArgv, "seta") || !strcmp(cmdArgv, "toggle") || !strcmp(cmdArgv, "togglep") || !strcmp(cmdArgv, "vstr") || !strcmp(cmdArgv, "bind") || !strcmp(cmdArgv, "bindaxis") || strstr(cmdArgv, "aim_"))
		{
			KickClient(cl - svs->clients, "'%s' server detected you where using config mods, Therefore you have been kicked.", xenonUserData->gamertag);
			break;
		}
	}

	for (u = ucmds; u->name; u++)
	{
		if (!strcmp(SV_Cmd_Argv(0), u->name))
		{
			if (!fromOldServer || u->allowFromOldServer)
				u->func(cl);
			break;
		}
	}

	if (!fromOldServer && !u->name && SV_Loaded())
		ClientCommand(cl - svs->clients);
	SV_Cmd_EndTokenizedString();
}

int SV_ClientCommand(client_t *cl, msg_t *msg, int fromOldServer)
{
	char strBuf[MAX_STRING_CHARS];

	int seq = MSG_ReadLong(msg);
	const char *s = MSG_ReadString(msg, strBuf, MAX_STRING_CHARS);

	if (cl->lastClientCommand < seq)
	{
		if (seq <= cl->lastClientCommand + 1)
		{
			SV_ExecuteClientCommand(cl, s, fromOldServer);
			cl->lastClientCommand = seq;
			Com_sprintf(cl->lastClientCommandString, MAX_STRING_CHARS, "%s", s);
			return 1;
		}
		else
		{
			SV_DropClient(cl, "EXE_LOSTRELIABLECOMMANDS", true);
			return 0;
		}
	}
	else
	{
		return 1;
	}
}

int SV_ProcessClientCommands(client_t *cl, msg_t *msg, int fromOldServer, int *lastCommand)
{
	*lastCommand = MSG_ReadBits(msg, 3);
	if (*lastCommand != clc_EOF)
	{
		while (*lastCommand == clc_clientCommand)
		{
			if (!SV_ClientCommand(cl, msg, fromOldServer) || cl->state == 1)
				return 0;
			*lastCommand = MSG_ReadBits(msg, 3);
			if (*lastCommand == clc_EOF)
				return 1;
		}
	}
	return 1;
}

void OnPlayerBegin(int clientNum)
{
	SetClientDvar(clientNum, "loc_warnings", "0");
	SetClientDvar(clientNum, "loc_warningsUI", "0");

	const char *selfName = xenonUserData->gamertag;

	if (SV_IsClientBanned(clientNum))
		KickClient(clientNum, "You are permanently banned from joining '%s' server.", selfName);
	else if (UI_VerifyString(g_entities->client[clientNum].clanAbbrev, false))
		KickClient(clientNum, "You have been kicked from '%s' server for having a invalid clan name.", selfName);
	else if (UI_VerifyString(SV_GetClientName(clientNum), true))
		KickClient(clientNum, "You have been kicked from '%s' server for having a invalid name.", selfName);
	else if (SV_GetClientPrestige(clientNum) >= 11)
		KickClient(clientNum, "You have been kicked from '%s' server for having a invalid prestige.", selfName);
}

void VM_Notify_Hook(unsigned int notifyListOwnerId, unsigned short stringValue, VariableValue *top)
{
	int entNum = Scr_GetEntNum(notifyListOwnerId);
	if (SV_IsPlayer(entNum))
	{
		if (!strcmp(SL_ConvertToString(stringValue), "begin"))
			OnPlayerBegin(entNum);
	}
	VM_Notify_Stub(notifyListOwnerId, stringValue, top);
}

void Menu_PaintAll_Internal_Hook(UiContext *dc, int init)
{
	if (IW4ServerInit)
	{
		DrawOkayMessageDialog("Success:\n\nWelcome to IW4 Server.\n\nBuild: 1.3\n\nMade By SC58");
		IW4ServerInit = false;
	}
	
	if (IsInGame())
	{
		if (IsServerHost())
		{
			if (IW4DrawFPS)
				CG_DrawFPS();

			if (!IsUIActive())
				MenuInit();

			SetSelfFOV(SelfFOVValue);
		}
	}
	else
	{
		if (IsOnlineGame() && IsPublicLobbyActive() && IW4ForceHost)
		{
			if (ButtonPressed(BUTTON_SELECT, 0))
				OpenSelectMapMenu();

			if (GetLobbyMemberCount() >= 2)
				SetDvar("party_connectToOthers", "0");
			else
				SetDvar("party_connectToOthers", "1");
			SetDvar("party_host", "1");
			SetDvar("party_hostmigration", "0");
		}
	}
	Menu_PaintAll_Internal_Stub(dc, init);
}

void G_InitGame_Hook(int levelTime, unsigned int randomSeed, int restart, int registerDvars, int savegame)
{
	memset(&menuInfo, 0, sizeof(menuInfo));
	G_InitGame_Stub(levelTime, randomSeed, restart, registerDvars, savegame);
}

void ClearAllServerBansDialogThread(uint64_t)
{
	Sleep(10);
	if (DrawYesNoMessageDialog("Are you sure you want to clear all server bans?\n\Server Ban Count: %i", serverBansCount))
	{
		SV_ClearServerBans();
		iPrintln("All Server Bans have been cleared.");
	}
	sys_ppu_thread_exit(0);
}

extern "C" int IW4Server_prx_entry()
{
	int32_t wp_result = WriteProcessMemory<bool>((uint32_t)&IW4ServerInit, true, sizeof(bool));
	if (wp_result == SUCCEEDED)
	{
		SetBuildVersion("IW4 Server %s.%i.%i", -80);

		// Set number of matched players to 0 to reveal their true names
		WriteProcessMemory<uint8_t>(0x54EE58, 0x0, sizeof(uint8_t));
		WriteProcessMemory<uint8_t>(0xC848B, 0x0, sizeof(uint8_t));

		// Enable ping on scoreboard
		WriteProcessMemory<uint8_t>(0x72180, 0x41, sizeof(uint8_t));

		// Remove ppu thread id check on CL_ConsolePrint to be called on any ppu thread
		for (int i = 0; i < 0x5; i++)
			WriteProcessMemory<uint32_t>(0x9DE24 + (i * sizeof(uint32_t)), 0x60000000, sizeof(uint32_t));

		HookFunctionStart(0x253670, *(uint32_t*)Menu_PaintAll_Internal_Hook, *(uint32_t*)Menu_PaintAll_Internal_Stub);
		HookFunctionStart(0x20B7C8, *(uint32_t*)VM_Notify_Hook, *(uint32_t*)VM_Notify_Stub);
		HookFunctionStart(0x18A2B8, *(uint32_t*)G_InitGame_Hook, *(uint32_t*)G_InitGame_Stub);
		HookFunction(0x215310, *(uint32_t*)SV_ProcessClientCommands);
	}
	else if (wp_result == ENOSYS)
	{
		DrawOkayMessageDialog("Error: 0x80010003\n\nWrite(0x389)/Read(0x388) Memory Process syscalls are not implemented in lv2.\n\nYou need to be on DEX or DECR in order to use IW4Server.sprx.");
	}
	else
	{
		DrawOkayMessageDialog("Error: 0x%X\n\nFailed to init IW4 Server.\n\nTry restarting the game or changing the EBOOT.BIN that loads IW4Server.sprx.", wp_result);
	}
    return SYS_PRX_RESIDENT;
}