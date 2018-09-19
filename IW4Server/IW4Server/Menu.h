const char *MainMenu[2] = { "Host", "Clients" };
const char *HostMenu[8] = { "Edit Self FOV", "Toggle Draw FPS", "Toggle Force Host", "Toggle DLC Maps", "Unban Client By Name", "Clear Server Bans", "Print Ban Count", "Self Disconnect" };
const char *ClientOption[2] = { "Ban", "Kick" };

void MenuHudElemInit()
{
	DrawCenterScreenHud();
	DrawCenterScreenScrollBar(menuInfo.scroll);

	switch (menuInfo.selectedMenu)
	{
	case 0:
		DrawCenterScreenTitle("Main Menu");
		DrawCenterScreenOptions(MainMenu, 2);
		break;
	case 1:
		DrawCenterScreenTitle("Host Menu");
		DrawCenterScreenOptions(HostMenu, 8);
		break;
	case 2:
		DrawCenterScreenTitle("Client Menu");
		DrawCenterScreenClientList();
		break;
	case 3:
		char clientName[MAX_NETNAME];
		ClientCleanName(SV_GetClientName(menuInfo.clientNum), clientName, MAX_NETNAME);
		DrawCenterScreenTitle(clientName);
		DrawCenterScreenOptions(ClientOption, 2);
		break;
	}
}

void MenuInit()
{
	if (!menuInfo.open && ButtonPressed(BUTTON_DPADUP, 0) && ButtonPressed(BUTTON_SQUARE, 1))
	{
		menuInfo.open = true;
		PlaySound("mouse_over");
	}

	if (menuInfo.open && ButtonPressed(BUTTON_R3, 0))
	{
		menuInfo.open = false;
	}

	if (menuInfo.open)
	{
		MenuHudElemInit();

		if (ButtonPressed(BUTTON_DPADUP, 0))
		{
			PlaySound("mouse_over");
			menuInfo.scroll--;
			switch (menuInfo.selectedMenu)
			{
			case 0:
				if (menuInfo.scroll == -1)
					menuInfo.scroll = 1;
				break;
			case 1:
				if (menuInfo.scroll == -1)
					menuInfo.scroll = 7;
				break;
			case 2:
				if (menuInfo.scroll == -1)
					menuInfo.scroll = GetMaxClients() - 1;
				break;
			case 3:
				if (menuInfo.scroll == -1)
					menuInfo.scroll = 1;
				break;
			}
		}

		if (ButtonPressed(BUTTON_DPADDOWN, 0))
		{
			PlaySound("mouse_over");
			menuInfo.scroll++;
			switch (menuInfo.selectedMenu)
			{
			case 0:
				if (menuInfo.scroll == 2)
					menuInfo.scroll = 0;
				break;
			case 1:
				if (menuInfo.scroll == 8)
					menuInfo.scroll = 0;
				break;
			case 2:
				if (menuInfo.scroll == GetMaxClients())
					menuInfo.scroll = 0;
				break;
			case 3:
				if (menuInfo.scroll == 2)
					menuInfo.scroll = 0;
				break;
			}
		}

		if (ButtonPressed(BUTTON_X, 0))
		{
			PlaySound("mouse_click");
			switch (menuInfo.selectedMenu)
			{
			case 0:
				switch (menuInfo.scroll)
				{
				case 0:
					menuInfo.selectedMenu = 1;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 1:
					menuInfo.selectedMenu = 2;
					menuInfo.scroll = 0;
					goto END;
					break;
				}
				break;
			case 1:
				switch (menuInfo.scroll)
				{
				case 0:
					DrawKeyboard("Set FOV (Min: 60 | Max: 120) (Default: 65)", va("%g", SelfFOVValue), 3, CELL_OSKDIALOG_PANELMODE_NUMERAL, FOVKeyboardCallback);
					break;
				case 1:
					IW4DrawFPS = !IW4DrawFPS;
					if (IW4DrawFPS)
						iPrintln("Draw FPS: ^2Enabled");
					else
						iPrintln("Draw FPS: ^1Disabled");
					break;
				case 2:
					if (IsOnlineGame())
					{
						IW4ForceHost = !IW4ForceHost;
						if (IW4ForceHost)
						{
							SetDvar("ping_increment", "1");
							SetDvar("ping_default_min", "20");
							SetDvar("party_connectTimeout", "1");
							SetDvar("migration_msgtimeout", "0");
							SetDvar("migration_timeBetween", "999999");
							SetDvar("migration_verboseBroadcastTime", "0");
							SetDvar("migrationPingTime", "0");
							SetDvar("lobby_partySearchWaitTime", "0");
							SetDvar("g_kickHostIfIdle", "0");
							SetDvar("badhost_endGameIfISuck", "0");
							SetDvar("badhost_maxDoISuckFrames", "0");
							SetDvar("badhost_maxHappyPingTime", "999");
							SetDvar("badhost_minTotalClientsForHappyTest", "18");
							iPrintln("Force Host: ^2Enabled");
						}
						else
						{
							ResetDvar("ping_default_min");
							ResetDvar("ping_increment");
							ResetDvar("party_connectToOthers");
							ResetDvar("party_connectTimeout");
							ResetDvar("party_host");
							ResetDvar("party_hostmigration");
							ResetDvar("migration_msgtimeout");
							ResetDvar("migration_timeBetween");
							ResetDvar("migration_verboseBroadcastTime");
							ResetDvar("migrationPingTime");
							ResetDvar("lobby_partySearchWaitTime");
							ResetDvar("g_kickHostIfIdle");
							ResetDvar("badhost_endGameIfISuck");
							ResetDvar("badhost_maxDoISuckFrames");
							ResetDvar("badhost_maxHappyPingTime");
							ResetDvar("badhost_minTotalClientsForHappyTest");
							iPrintln("Force Host: ^1Disabled");
						}
					}
					else
					{
						iPrintln("You cannot use Force Host Offline.");
					}
					break;
				case 3:
					if (IsOnlineGame())
					{
						if (Content_IsAnyDLCPackInstalled())
						{
							IW4DLC = !IW4DLC;
							if (IW4DLC)
							{
								contentPack->contentPack = 1;
								iPrintln("DLC Maps: ^1Disabled");
							}
							else
							{
								contentPack->contentPack = Content_GetInstalledDLCContentPackIndex();
								iPrintln("DLC Maps: ^2Enabled");
							}
						}
						else
						{
							iPrintln("You do not have any DLC's installed.");
						}
					}
					else
					{
						iPrintln("You cannot toggle DLC Maps Offline.");
					}
					break;
				case 4:
					if (serverBansCount)
						DrawKeyboard("Enter Client Name To Unban (Max Char 32)", "", MAX_NETNAME, CELL_OSKDIALOG_PANELMODE_ALPHABET, UnbanClientKeyboardCallback);
					else
						iPrintln("You have yet to ban anyone, Therefore there is no need to unban a client.");
					break;
				case 5:
					if (serverBansCount)
						sys_ppu_thread_create(&ClearAllServerBansDialogThreadID, ClearAllServerBansDialogThread, 0, 1000, 0x1000, 0, "clear_server_bans_thread");
					else
						iPrintln("You have yet to ban anyone, Therefore there is no need to clear all bans.");
					break;
				case 6:
					iPrintln("Total Ban Count: %i/1024", serverBansCount);
					break;
				case 7:
					SelfDisconnect();
					break;
				}
				break;
			case 2:
				switch (menuInfo.scroll)
				{
				case 0:
					menuInfo.clientNum = 0;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 1:
					menuInfo.clientNum = 1;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 2:
					menuInfo.clientNum = 2;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 3:
					menuInfo.clientNum = 3;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 4:
					menuInfo.clientNum = 4;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 5:
					menuInfo.clientNum = 5;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 6:
					menuInfo.clientNum = 6;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 7:
					menuInfo.clientNum = 7;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 8:
					menuInfo.clientNum = 8;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 9:
					menuInfo.clientNum = 9;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 10:
					menuInfo.clientNum = 10;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 11:
					menuInfo.clientNum = 11;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 12:
					menuInfo.clientNum = 12;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 13:
					menuInfo.clientNum = 13;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 14:
					menuInfo.clientNum = 14;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 15:
					menuInfo.clientNum = 15;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 16:
					menuInfo.clientNum = 16;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				case 17:
					menuInfo.clientNum = 17;
					menuInfo.selectedMenu = 3;
					menuInfo.scroll = 0;
					goto END;
					break;
				}
				break;
			case 3:
				switch (menuInfo.scroll)
				{
				case 0:
					SV_BanClient(menuInfo.clientNum);
					break;
				case 1:
					if (IsHost(menuInfo.clientNum))
						iPrintln("You cannot kick yourself silly.");
					else
						KickClient(menuInfo.clientNum, "You have been kicked from '%s' server.", xenonUserData->gamertag);
					break;
				}
				break;
			}
		}

		if (ButtonPressed(BUTTON_CIRCLE, 0))
		{
			if (menuInfo.selectedMenu)
				PlaySound("mouse_over");
			switch (menuInfo.selectedMenu)
			{
			case 1:
				menuInfo.selectedMenu = 0;
				menuInfo.scroll = 0;
				break;
			case 2:
				menuInfo.selectedMenu = 0;
				menuInfo.scroll = 0;
				break;
			case 3:
				menuInfo.selectedMenu = 2;
				menuInfo.scroll = 0;
				break;
			}
		}
	END:
	}
}