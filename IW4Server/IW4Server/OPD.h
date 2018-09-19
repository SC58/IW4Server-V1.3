#define TOC 0x724C38

struct opd_s
{
	uint32_t sub;
	uint32_t toc;
};

typedef void(*UI_KeyboardCallback)(int localClientNum, bool ifExecuted, const char *text);

opd_s _printf_t = { 0x4C7AC8, TOC };
int(*_printf)(const char *format, ...) = (int(*)(const char *, ...))&_printf_t;

opd_s _vsnprintf_t = { 0x4C6C58, TOC };
int(*_vsnprintf)(char *s, size_t n, const char *format, va_list arg) = (int(*)(char *, size_t, const char *, va_list))&_vsnprintf_t;

opd_s va_t = { 0x27E0D0, TOC };
char *(*va)(const char *format, ...) = (char *(*)(const char *, ...))&va_t;

opd_s OpenKeyboard_t = { 0x238070, TOC };
void(*OpenKeyboard)(int localClientNum, const char *title, const char *presetMessage, size_t size, UI_KeyboardCallback function) = (void(*)(int, const char *, const char *, size_t, UI_KeyboardCallback))&OpenKeyboard_t;

opd_s GetLobbyMemberCount_t = { 0x2415E8, TOC };
int(*GetLobbyMemberCount)() = (int(*)())&GetLobbyMemberCount_t;

opd_s UI_OpenMenu_t = { 0x236378, TOC };
void(*UI_OpenMenu)(int localClientNum, const char *menuName) = (void(*)(int, const char *))&UI_OpenMenu_t;

opd_s Dvar_FindVar_t = { 0x2762C8, TOC };
dvar_t *(*Dvar_FindVar)(const char *dvarName) = (dvar_t *(*)(const char *))&Dvar_FindVar_t;

opd_s Dvar_Reset_t = { 0x273518, TOC };
void(*Dvar_Reset)(dvar_t *dvar, DvarSetSource setSource) = (void(*)(dvar_t *, DvarSetSource))&Dvar_Reset_t;

opd_s Dvar_SetFromStringByName_t = { 0x2767E0, TOC };
void(*Dvar_SetFromStringByName)(const char *dvarName, const char *value) = (void(*)(const char *, const char *))&Dvar_SetFromStringByName_t;

opd_s UI_PlayLocalSoundAliasByName_t = { 0x2449A0, TOC };
int(*UI_PlayLocalSoundAliasByName)(int localClientNum, const char *aliasname) = (int(*)(int, const char *))&UI_PlayLocalSoundAliasByName_t;

opd_s CG_GameMessage_t = { 0x5EFB0, TOC };
void(*CG_GameMessage)(int localClientNum, const char *msg) = (void(*)(int, const char *))&CG_GameMessage_t;

opd_s CG_GetClientNum_t = { 0x610F0, TOC };
int(*CG_GetClientNum)(int localClientNum) = (int(*)(int))&CG_GetClientNum_t;

opd_s CL_IsUIActive_t = { 0xAD3D8, TOC };
bool(*CL_IsUIActive)(int localClientNum) = (bool(*)(int))&CL_IsUIActive_t;

opd_s Sys_Milliseconds_t = { 0x2F3FB8, TOC };
int(*Sys_Milliseconds)() = (int(*)())&Sys_Milliseconds_t;

opd_s Dvar_GetBool_t = { 0x277138, TOC };
bool(*Dvar_GetBool)(const char *dvarName) = (bool(*)(const char *))&Dvar_GetBool_t;

opd_s Dvar_GetInt_t = { 0x2770C8, TOC };
int(*Dvar_GetInt)(const char *dvarName) = (int(*)(const char *))&Dvar_GetInt_t;

opd_s CL_DisconnectLocalClient_t = { 0xAB0A0, TOC };
void(*CL_DisconnectLocalClient)(int localClientNum) = (void(*)(int))&CL_DisconnectLocalClient_t;

opd_s SV_GameSendServerCommand_t = { 0x21A0A0, TOC };
void(*SV_GameSendServerCommand)(int clientNum, svscmd_type type, const char *text) = (void(*)(int, svscmd_type, const char *))&SV_GameSendServerCommand_t;

opd_s Key_IsDown_t = { 0xA4FF0, TOC };
int(*Key_IsDown)(int localClientNum, int keynum) = (int(*)(int, int))&Key_IsDown_t;

opd_s SL_ConvertToString_t = { 0x200280, TOC };
const char *(*SL_ConvertToString)(unsigned short stringValue) = (const char *(*)(unsigned short))&SL_ConvertToString_t;

opd_s UI_DrawText_t = { 0x23A138, TOC };
void(*UI_DrawText)(ScreenPlacement *scrPlace, const char *text, int maxChars, Font_s *font, float x, float y, int horzAlign, int vertAlign, float scale, const float *color, int style) = (void(*)(ScreenPlacement *, const char *, int, Font_s *, float, float, int, int, float, const float *, int))&UI_DrawText_t;

opd_s UI_GetFontHandle_t = { 0x239D88, TOC };
Font_s *(*UI_GetFontHandle)(ScreenPlacement *scrPlace, int fontEnum, float scale) = (Font_s *(*)(ScreenPlacement *, int, float))&UI_GetFontHandle_t;

opd_s UI_DrawHandlePic_t = { 0x22A000, TOC };
void(*UI_DrawHandlePic)(ScreenPlacement *scrPlace, float x, float y, float w, float h, int horzAlign, int vertAlign, const float *color, Material *material) = (void(*)(ScreenPlacement *, float, float, float, float, int, int, const float *, Material *))&UI_DrawHandlePic_t;

opd_s Material_RegisterHandle_t = { 0x4C6D28, TOC };
Material *(*Material_RegisterHandle)(const char *name, int imageTrack) = (Material *(*)(const char *, int))&Material_RegisterHandle_t;

opd_s ScrPlace_GetActivePlacement_t = { 0xDB9B0, TOC };
ScreenPlacement *(*ScrPlace_GetActivePlacement)(int localClientNum) = (ScreenPlacement *(*)(int))&ScrPlace_GetActivePlacement_t;

opd_s Scr_GetEntNum_t = { 0x2024F0, TOC };
int(*Scr_GetEntNum)(unsigned int id) = (int(*)(unsigned int))&Scr_GetEntNum_t;

opd_s Session_GetXuidEvenIfInactive_t = { 0x2EF810, TOC };
uint64_t(*Session_GetXuidEvenIfInactive)(SessionData *session, int clientNum) = (uint64_t(*)(SessionData *, int))&Session_GetXuidEvenIfInactive_t;

opd_s Cmd_MenuResponse_f_t = { 0x179868, TOC };
void(*Cmd_MenuResponse_f)(gentity_s *pEnt) = (void(*)(gentity_s *))&Cmd_MenuResponse_f_t;

opd_s Cmd_CheckNetNotify_f_t = { 0x1DA520, TOC };
void(*Cmd_CheckNetNotify_f)(int clientNum) = (void(*)(int))&Cmd_CheckNetNotify_f_t;

opd_s SendScoreboard_t = { 0x17AA30, TOC };
void(*SendScoreboard)(gentity_s *pEnt) = (void(*)(gentity_s *))&SendScoreboard_t;

opd_s UI_TextWidth_t = { 0x23B1F8, TOC };
int(*UI_TextWidth)(const char *text, int maxChars, Font_s *font, float scale) = (int(*)(const char *, int, Font_s *, float))&UI_TextWidth_t;

opd_s Session_IsHost_t = { 0x2F19A8, TOC };
bool(*Session_IsHost)(SessionData *session, int clientNum) = (bool(*)(SessionData *, int))&Session_IsHost_t;

opd_s Party_IsRunning_t = { 0xC71B8, TOC };
int(*Party_IsRunning)(PartyData_s *party) = (int(*)(PartyData_s *))&Party_IsRunning_t;

opd_s ClientCleanName_t = { 0x16C450, TOC };
void(*ClientCleanName)(const char *in, char *out, int outSize) = (void(*)(const char *, char *, int))&ClientCleanName_t;

opd_s SV_GetClientPersistentDataBuffer_t = { 0x2142A8, TOC };
char *(*SV_GetClientPersistentDataBuffer)(int clientNum) = (char *(*)(int))&SV_GetClientPersistentDataBuffer_t;

opd_s LiveStorage_DataGetInternalInt_t = { 0x1DCD58, TOC };
int(*LiveStorage_DataGetInternalInt)(int navStrings, int navStringCount, StructuredDataBuffer *buffer) = (int(*)(int, int, StructuredDataBuffer *))&LiveStorage_DataGetInternalInt_t;

opd_s bdAuthUtility_getUserID_t = { 0x4C7B58, TOC };
uint64_t(*bdAuthUtility_getUserID)(const char *username) = (uint64_t(*)(const char *))&bdAuthUtility_getUserID_t;

opd_s SV_Cmd_TokenizeString_t = { 0x1DB040, TOC };
void(*SV_Cmd_TokenizeString)(const char *text_in) = (void(*)(const char *))&SV_Cmd_TokenizeString_t;

opd_s SV_Loaded_t = { 0x21A770, TOC };
bool(*SV_Loaded)() = (bool(*)())&SV_Loaded_t;

opd_s SV_Cmd_EndTokenizedString_t = { 0x1D9270, TOC };
void(*SV_Cmd_EndTokenizedString)() = (void(*)())&SV_Cmd_EndTokenizedString_t;

opd_s MSG_ReadLong_t = { 0x1ECDF0, TOC };
int(*MSG_ReadLong)(msg_t *msg) = (int(*)(msg_t *))&MSG_ReadLong_t;

opd_s MSG_ReadString_t = { 0x1ED388, TOC };
char *(*MSG_ReadString)(msg_t *msg, char *string, unsigned int maxChars) = (char *(*)(msg_t *, char *, unsigned int))&MSG_ReadString_t;

opd_s SV_DropClient_t = { 0x215000, TOC };
void(*SV_DropClient)(client_t *drop, const char *reason, bool tellThem) = (void(*)(client_t *, const char *, bool))&SV_DropClient_t;

opd_s Com_sprintf_t = { 0x27E2F8, TOC };
int(*Com_sprintf)(char *dest, int size, const char *fmt, ...) = (int(*)(char *, int, const char *, ...))&Com_sprintf_t;

opd_s CG_DrawDevString_t = { 0x4EB70, TOC };
int(*CG_DrawDevString)(ScreenPlacement *scrPlace, float x, float y, float xScale, float yScale, const char *s, const float *color, int align, Font_s *font) = (int(*)(ScreenPlacement *, float, float, float, float, const char *, const float *, int, Font_s *))&CG_DrawDevString_t;