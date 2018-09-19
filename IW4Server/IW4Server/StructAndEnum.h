void ClearAllServerBansDialogThread(uint64_t);
sys_ppu_thread_t ClearAllServerBansDialogThreadID;

char invalidChars[0x14] = { '{', '}', '@', '^', '\x1', '\x2', '\x3', '\x4', '\x5', '\x6', '\xE', '\xF', '\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17' };

#define LOWORD(a) ((short)(a))
#define HIWORD(a) ((short)(((int)(a) >> 16) & 0xFFFF))

#define	SCREEN_WIDTH 640
#define	SCREEN_HEIGHT 480

#define CG_ALIGN_LEFT 1
#define CG_ALIGN_RIGHT 2
#define CG_ALIGN_CENTER 3
#define CG_ALIGN_TOP 4
#define CG_ALIGN_BOTTOM 8
#define CG_ALIGN_MIDDLE 12

#define UI_FONT_DEFAULT 0
#define UI_FONT_BIG 2
#define UI_FONT_SMALL 3
#define UI_FONT_BOLD 4
#define UI_FONT_CONSOLE 5
#define UI_FONT_OBJECTIVE 6
#define UI_FONT_NORMAL 7
#define UI_FONT_EXTRABIG 8
#define UI_FONT_HUDBIG 9
#define UI_FONT_HUDSMALL 10

#define colorLtMatte 0x2B2B2B7F
#define colorGreen 0x228B22FF
#define colorWhite 0xFFFFFFFF

#define	MAX_STRING_CHARS 1024
#define MAX_SERVER_BANS 1024
#define MAX_NETNAME 32
#define MAX_NETCLANNAME 4
#define MAX_CLIENTS 18

#define CMD_MAX_NESTING 8

#define	FPS_FRAMES 32

typedef void Font_s;
typedef void Material;
typedef void VariableValue;
typedef void SessionData;
typedef void UiContext;
typedef void dvar_t;
typedef void PartyData_s;

PartyData_s *g_lobbyData = (PartyData_s *)0xA13558;
SessionData *g_serverSession = (SessionData *)0x1FF49B0;

bool IW4ServerInit = false;

float SelfFOVValue = 65;

int lastTimeButton[0x5];

bool IW4ForceHost = false;
bool IW4DLC = false;
bool IW4DrawFPS = false;

struct MenuInfo
{
	bool open;
	int scroll;
	int selectedMenu;
	int clientNum;
};

MenuInfo menuInfo;

uint32_t serverBansCount = 0;

struct serverBan_t
{
	uint64_t xuid;
};

serverBan_t serverBans[MAX_SERVER_BANS];

enum clc_ops_e
{
	clc_move = 0x0,
	clc_moveNoDelta = 0x1,
	clc_clientCommand = 0x2,
	clc_EOF = 0x3,
};

enum svscmd_type
{
	SV_CMD_CAN_IGNORE = 0x0,
	SV_CMD_RELIABLE = 0x1,
};

enum DvarSetSource
{
	DVAR_SOURCE_INTERNAL = 0x0,
	DVAR_SOURCE_EXTERNAL = 0x1,
	DVAR_SOURCE_SCRIPT = 0x2,
};

enum keyNum_t
{
	BUTTON_X = 0x1,
	BUTTON_CIRCLE = 0x2,
	BUTTON_SQUARE = 0x3,
	BUTTON_TRIANGLE = 0x4,
	BUTTON_L1 = 0x5,
	BUTTON_R1 = 0x6,
	BUTTON_START = 0xE,
	BUTTON_SELECT = 0xF,
	BUTTON_L3 = 0x10,
	BUTTON_R3 = 0x11,
	BUTTON_L2 = 0x12,
	BUTTON_R2 = 0x13,
	BUTTON_DPADUP = 0x14,
	BUTTON_DPADDOWN = 0x15,
	BUTTON_DPADLEFT = 0x16,
	BUTTON_DPADRIGHT = 0x17,
};

enum clientStateStatus_t
{
	CS_FREE = 0x0,
	CS_ZOMBIE = 0x1,
	CS_RECONNECTING = 0x2,
	CS_CONNECTED = 0x3,
	CS_CLIENTLOADING = 0x4,
	CS_ACTIVE = 0x5,
};

struct msg_t
{
	int overflowed;
	int readOnly;
	char *data;
	char *splitData;
	int maxsize;
	int cursize;
	int splitSize;
	int readcount;
	int bit;
	int lastEntityRef;
};

struct ScreenPlacement
{
	float scaleVirtualToReal[2];
	float scaleVirtualToFull[2];
	float scaleRealToVirtual[2];
	float realViewportPosition[2];
	float realViewportSize[2];
	float virtualViewableMin[2];
	float virtualViewableMax[2];
	float realViewableMin[2];
	float realViewableMax[2];
	float virtualAdjustableMin[2];
	float virtualAdjustableMax[2];
	float realAdjustableMin[2];
	float realAdjustableMax[2];
	float subScreenLeft;
};

struct StructuredDataBuffer
{
	char *data;
	unsigned int size;
};

struct XenonUserData
{
	int signinState;
	char gamertag[MAX_NETNAME];
	uint64_t xuid;
	char xuidString[17];
};

XenonUserData * xenonUserData = (XenonUserData *)0x1F9F118;

struct CmdArgs
{
	int nesting;
	int localClientNum[8];
	int controllerIndex[8];
	int argc[CMD_MAX_NESTING];
	const char **argv[CMD_MAX_NESTING];
};

CmdArgs *sv_cmd_args = (CmdArgs *)0x17476F4;

struct ContentPackList
{
	int contentPack;
	char unk0[0x88];
	char dlcName1[0x5];
	char unk1[0x83];
	char dlcName2[0x5];
};

ContentPackList *contentPack = (ContentPackList *)0x1F96708;

struct fpsInfo
{
	int previous;
	int index;
	int previousTimes[FPS_FRAMES];
};

fpsInfo *fps = (fpsInfo *)0x904EA8;

struct gclient_s
{
	char unk0[0x32CC];
	char clanAbbrev[MAX_NETCLANNAME];
	char unk1[0x430];
};

struct gentity_s
{
	char unk0[0x158];
	gclient_s *client;
	char unk1[0x124];
};

gentity_s *g_entities = (gentity_s *)0x1319800;

struct client_t
{
	clientStateStatus_t state;
	char unk0[0x20E90];
	int lastClientCommand;
	char lastClientCommandString[MAX_STRING_CHARS];
	char unk1[0x4];
	char name[MAX_NETNAME];
	char unk2[0x76CC4];
};

struct ucmd_t
{
	char *name;
	void(*func)(client_t *);
	int allowFromOldServer;
};

ucmd_t *ucmds = (ucmd_t *)0x8AA288;

struct serverStatic_t
{
	char unk0[0x401C];
	client_t *clients;
};

serverStatic_t *svs = (serverStatic_t *)0x1BF5100;