/*
 * f系统1.0 专业版 - 个人项目 / f System 1.0 Professional - Personal Project
 * 版权所有 (C) 2020~9999 Jellyfish Studio / Copyright (C) 2020~9999 Jellyfish Studio
 * 
 * 许可条款 / License Terms：
 * 1. 仅供个人学习和研究使用 / For personal learning and research use only
 * 2. 禁止商业使用 / Commercial use prohibited
 * 3. 禁止未经授权的分发 / Unauthorized distribution prohibited
 * 
 * 开发者：JammyJellyfish@hotmail.com / Developer: JammyJellyfish@hotmail.com
 * 最后更新：2026年 / Last Updated: 2026
 */
//30/11/25 15:57:加入了开机音乐
//30/11/25 15:58:正式更改项目名为f System 1.0 Professional (music!!!) codename nori 
//30/11/25 15:59:原项目发行前测试
//30/11/25 15:59:取消了原项目的发行计划
//30/11/25 16:00:f System 1.0 Professional (music!!!) codename nori 预计发布时间2026年1月 
//30/11/25 22:27:加入紫屏 
//01/12/25 23:26:加入了彩蛋
//01/12/25 23:26:计划加入音频播放器 
//07/12/25 13:28:加入了更多版权信息
//07/12/25 13:28:正式进入了最终测试阶段 
//21/12/25 13:27:正式成立Jellyfish Studio （开发团队工作室） 
//21/12/25 13:27:版权正式移交给Jellyfish Studio 
//21/12/25 13:30:更新了版权文件 
//01/01/26 00:01:260101(newyear):修复了一些bug，增加了增加了亿点bug，受开发者抵制版本第一名 
//05/01/26 13:30:26W02:修复了一些bug，对文件兼容性支持范围增加 
//07/01/26 17:35:26W02b:优化了系统输出文字的可读性 
//08/01/26 17:15:26W03:添加了中文输入支持 
//15/01/26 16:41:代码可读性更改① 
//16/02/26 11:49:26W04:添加了时钟和日历
//16/02/26 11:50:26W04-B:删除了时钟 
//17/02/26 00:01:26W05:删除了时钟 
//17/02/26 00:02:停止开发
//13/06/26 12:06:恢复开发
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <commdlg.h>
#include <shellapi.h>
#include <direct.h>
#include <algorithm>
#include <mmsystem.h>
#include <imm.h>

// 添加兼容性定义
#ifndef CP_ACP
#define CP_ACP 0
#endif

// 版本信息
#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define BUILD_VERSION 400
#define RELEASE_YEAR 2025

// 配置文件路径
#define CONFIG_FILE "f系统配置.dat"

// Vista风格颜色
#define VISTA_BLUE RGB(0, 70, 140)
#define VISTA_LIGHT_BLUE RGB(100, 150, 220)
#define VISTA_GREEN RGB(60, 180, 75)
#define VISTA_RED RGB(220, 80, 60)
#define VISTA_ORANGE RGB(240, 150, 30)
#define DESKTOP_PINK RGB(255, 200, 220)
#define BOOT_BLACK RGB(0, 0, 0)
#define TASKBAR_GRAY RGB(60, 60, 60)
#define TASKBAR_BUTTON RGB(80, 80, 80)
#define TASKBAR_BUTTON_HOVER RGB(100, 100, 100)
#define TASKBAR_BUTTON_PRESS RGB(120, 120, 120)
#define CALC_BG RGB(240, 240, 240)
#define CALC_BUTTON RGB(220, 220, 220)
#define CALC_OPERATOR RGB(180, 180, 220)
#define XP_BLUE RGB(0, 60, 116)
#define XP_LIGHT_BLUE RGB(185, 209, 234)
#define XP_GRAY RGB(212, 208, 200)
#define SPECIAL_BG RGB(255, 230, 240)
#define FINAL_BG RGB(255, 240, 200)
#define SHOUHUI_BODY RGB(255, 220, 100)
#define SHOUHUI_EYES RGB(0, 0, 0)
#define SHOUHUI_MOUTH RGB(255, 100, 100)
#define OOBE_BG RGB(30, 60, 120)
#define FILEMANAGER_BG RGB(255, 255, 240)
#define CDKEY_BG RGB(40, 80, 160)
#define GAME_BUTTON RGB(150, 100, 200)  // 水母游戏按钮颜色
#define EDITOR_BG RGB(255, 255, 255)
#define EDITOR_TEXT RGB(0, 0, 0)
#define EDITOR_LINE_NUM RGB(100, 100, 100)
#define EDITOR_SELECTION RGB(200, 220, 255)
#define SETTINGS_BG RGB(250, 250, 255)

using namespace std; // 使用std命名空间

// 水母游戏全局变量
static BOOL showGameEgg = FALSE;
static BOOL gameInitialized = FALSE;
static BOOL showGameButton = FALSE;  // 是否显示游戏按钮

// 水母游戏核心变量
static BOOL gameRunning = TRUE;
static int jellyfishX = 400, jellyfishY = 300;
static int studentX[5], studentY[5];
static int score = 0;
static int lives = 3;
static BOOL gameOver = FALSE;
static char scoreText[50];
static int gameState = 0; // 0:剧情 1:游戏中 2:过关 3:被制裁
static int dialogueIndex = 0;
static int level = 1;
static float tentacleWave = 0.0f;
static int levelTime = 0;
static int levelTargetTime = 200;


static BOOL shutdownMusicPlaying = FALSE;
static DWORD shutdownMusicStartTime = 0;

static BOOL isBugHovered = FALSE;      // Bug报告按钮悬停状态
static BOOL isBugPressed = FALSE;      // Bug报告按钮按下状态
// USB设备错误全局变量
static BOOL showUSBError = FALSE;
static char usbDriveLetter = 'D';
static char errorMessage[200] = "";

// 日历软件全局变量
static BOOL showCalendar = FALSE;
static int calendarYear = 2026;
static int calendarMonth = 2; // 2月
static int selectedDay = 14;   // 当前选中日期
static int calendarView = 0;    // 0:月视图 1:年视图
static BOOL showEventDialog = FALSE;
static char eventText[500] = "";
static char calendarEvents[12][31][200]; // 存储事件 [月][日]
static int eventCursorPos = 0;
static int eventScrollPos = 0;

// 时钟软件全局变量
static BOOL showClock = FALSE;
static int clockView = 0;        // 0:模拟时钟 1:数字时钟 2:秒表 3:计时器 4:闹钟
static BOOL stopwatchRunning = FALSE;
static DWORD stopwatchStartTime = 0;
static DWORD stopwatchElapsed = 0;
static int timerSeconds = 60;     // 计时器初始60秒
static int timerRemaining = 0;
static BOOL timerRunning = FALSE;
static DWORD timerStartTime = 0;
static BOOL alarmEnabled[5] = {FALSE, FALSE, FALSE, FALSE, FALSE};
static int alarmHours[5] = {7, 8, 12, 18, 22};
static int alarmMinutes[5] = {0, 30, 0, 0, 0};
static char alarmLabels[5][50] = {"起床", "工作", "午餐", "晚餐", "睡觉"};
static int alarmEditIndex = -1;

// 任务栏按钮状态
static BOOL isCalendarHovered = FALSE;
static BOOL isCalendarPressed = FALSE;
static BOOL isClockHovered = FALSE;
static BOOL isClockPressed = FALSE;

// 障碍物结构
typedef struct {
    int x, y, width, height;
    BOOL active;
} Obstacle;

// 道具结构
typedef struct {
    int x, y, type; // 0:加速 1:隐身 2:护盾 3:冰冻
    BOOL active;
    int duration;
} PowerUp;

// 玩家状态
static BOOL hasShield = FALSE;
static BOOL isInvisible = FALSE;
static BOOL isFrozen = FALSE;
static int freezeTime = 0;
static int powerUpTime = 0;

// 地图元素
#define MAX_OBSTACLES 10
#define MAX_POWERUPS 5
static Obstacle obstacles[MAX_OBSTACLES];
static PowerUp powerUps[MAX_POWERUPS];

// 剧情对话
const char* storyDialogues[] = {
    "第一章：教室惊魂",
    "你：终于写完了f系统代码...",
    "同学A：Jammy！作业借我抄抄！",
    "同学B：别跑！把你的编程作业交出来！",
    "你：救命啊！我只是个写代码的水母！",
    "小心障碍物，收集道具辅助逃亡！",
    "每关坚持10秒才能进入下一关！",
    "按空格键开始逃亡...",
    NULL
};

// 关卡对话
const char* levelDialogues[][10] = {
    { // 第一关
        "第一关：教室大逃亡",
        "躲避5个同学的围追堵截",
        "小心桌椅障碍物！",
        "收集道具获得特殊能力！",
        "坚持10秒进入下一关！",
        NULL
    },
    { // 第二关  
        "第二关：走廊追逐战",
        "同学们速度加快了！",
        "走廊里有更多障碍物！",
        "道具效果更加强力！",
        "坚持住，电脑就在前方！",
        NULL
    },
    { // 第三关
        "最终关：食堂混战", 
        "这是最后的考验！",
        "同学们已经疯狂了！",
        "障碍物密集，小心移动！",
        "逃回机房就能继续编程！",
        NULL
    }
};

// 被制裁对话
const char* punishmentDialogues[] = {
    "你被同学们抓住了！",
    "同学A：终于抓到你了！",
    "同学B：把作业交出来！",
    "你被强迫帮所有同学写作业...",
    "写了整整一晚上的作业...",
    "下次一定要跑快点！",
    "按R键重新开始...",
    NULL
};

// 桌面快捷方式结构
#define MAX_SHORTCUTS 50

typedef struct {
    char name[100];
    char targetPath[MAX_PATH];
    int x;
    int y;
    int width;
    int height;
    BOOL isDragging;
    int dragOffsetX;
    int dragOffsetY;
    BOOL selected;
} DesktopShortcut;

static DesktopShortcut desktopShortcuts[MAX_SHORTCUTS];
static int shortcutCount = 0;
static int draggingShortcutIndex = -1;
static int selectedShortcutIndex = -1;
static BOOL isDraggingShortcut = FALSE;
static POINT dragStartPoint;

// 水母游戏函数声明
void InitializeJellyfishGame();
void InitializeJellyfishLevel();
void DrawJellyfishGame(HDC hdc, HWND hwnd);
void DrawJellyfishStory(HDC hdc, HWND hwnd);
void DrawJellyfishGameplay(HDC hdc, HWND hwnd);
void DrawJellyfish(HDC hdc, int x, int y, int size);
void DrawStudent(HDC hdc, int x, int y, int size, int id);
void DrawJellyfishObstacles(HDC hdc);
void DrawJellyfishPowerUps(HDC hdc);
void DrawTentacleEffect(HDC hdc, int x, int y, int size);
void UpdateJellyfishGame(HWND hwnd);
void CheckJellyfishCollisions();
void ActivatePowerUp(int type);
void HandleJellyfishGameInput(WPARAM wParam, HWND hwnd);
void CheckGameActivation(WPARAM wParam, HWND hwnd);
// 文件管理器彩蛋函数声明
void DrawFOSEgg(HDC hdc, HWND hwnd);
void DrawDialogueScene(HDC hdc, int centerX, int centerY);
void DrawDevTimeline(HDC hdc, int centerX, int centerY);
void DrawFeatureShowcase(HDC hdc, int centerX, int centerY);
void DrawCredits(HDC hdc, int centerX, int centerY);
// USB设备错误界面函数声明
void ShowUSBErrorScreen(HWND hwnd, char driveLetter, const char* message);
void DrawUSBErrorScreen(HDC hdc, int centerX, int centerY);
void DrawUSBDeviceArt(HDC hdc, int centerX, int centerY);
void DrawUSBErrorButton(HDC hdc, int x, int y, int width, int height, const char* text, BOOL isPrimary);
void HandleUSBErrorInput(char key);
// 网线接头背景相关函数声明
void CreateEthernetBackground(HWND hwnd);
void DrawEthernetScene(HDC hdc);
void DrawDetailedEthernetJack(HDC hdc, int x, int y, int jackId);
void DrawEthernetCables(HDC hdc, POINT* positions);
void DrawNetworkHub(HDC hdc, int x, int y);
void DrawEthernetCable(HDC hdc, int startX, int startY, int endX, int endY, int cableId);
// Bug列表功能函数声明
void ShowBugList(HWND hwnd);
void DrawBugList(HDC hdc, int centerX, int centerY);
void HandleBugListInput(char key);
void LoadBugListFromFile();
void SaveBugListToFile();
void ToggleBugFixedStatus(int index);
void DeleteBug(int index);

void DrawJellyfishPleaScene(HDC hdc, int centerX, int centerY);
void DrawPleadingJellyfish(HDC hdc, int x, int y, int size);

// 全局变量
static BOOL showFOSBeta = TRUE;
static BOOL showBootAnimation = FALSE;
static BOOL showWelcome = FALSE;
static BOOL showSecondScreen = FALSE;
static BOOL showOOBE = FALSE;
static BOOL showComplete = FALSE;
static BOOL showDesktop = FALSE;
static BOOL showLogin = FALSE;
static BOOL showShutdown = FALSE;
static BOOL showCalculator = FALSE;
static BOOL showSystemInfo = FALSE;
static BOOL showFileManager = FALSE;
static BOOL showCDKeyScreen = FALSE;
static BOOL isFirstBoot = TRUE;
static BOOL isActivated = FALSE;
static BOOL isTrialMode = FALSE;
static int progressValue = 0;
static int fosProgressValue = 0;
static int currentStep = 0;
static int shutdownProgress = 0;
static int trialDaysRemaining = 30;
static int settingsFocusField = 0; // 0:用户名, 1:密码, 2:确认密码
static HBITMAP hBackgroundBitmap = NULL;

// CD-KEY变量
static char cdKeyInput[50] = "";
static BOOL cdKeyError = FALSE;
static char cdKeyErrorMsg[100] = "";

// OOBE配置变量 - 修复：添加密码跳过标志
static char username[50] = "User";
static char password[50] = "1234";
static char confirmPassword[50] = "1234";
static char loginUsername[50] = "";
static char loginPassword[50] = "";
static char computerName[50] = "FOS-PC";
static int cursorPos = 0;
static BOOL isPasswordMode = FALSE;
static BOOL loginError = FALSE;
static BOOL oobeError = FALSE;
static char oobeErrorMsg[100] = "";
static BOOL skipPasswordSetup = FALSE;  // 新增：密码跳过标志

// 任务栏按钮状态
static BOOL isShutdownHovered = FALSE;
static BOOL isShutdownPressed = FALSE;
static BOOL isSysInfoHovered = FALSE;
static BOOL isSysInfoPressed = FALSE;
static BOOL isCalcHovered = FALSE;
static BOOL isCalcPressed = FALSE;
static BOOL isFileManagerHovered = FALSE;
static BOOL isFileManagerPressed = FALSE;
static BOOL isDesktopHovered = FALSE;
static BOOL isDesktopPressed = FALSE;
static BOOL isGameHovered = FALSE;      // 水母游戏按钮悬停状态
static BOOL isGamePressed = FALSE;      // 水母游戏按钮按下状态
static BOOL isSettingsHovered = FALSE;  // 设置按钮悬停状态
static BOOL isSettingsPressed = FALSE;  // 设置按钮按下状态

// 计算器变量
static char calcDisplay[50] = "0";
static double calcValue = 0;
static char calcOperator = 0;
static BOOL calcWaitingOperand = FALSE;
static BOOL calcNewNumber = TRUE;
static BOOL calcError = FALSE;

// 彩蛋变量
static BOOL showSpecialMessage = FALSE;
static BOOL showFinalEgg = FALSE;
static char guessInput[50] = "";
static int specialMessageScrollY = 0;
static DWORD lastScrollTime = 0;

// 系统信息变量
static SYSTEM_INFO sysInfo;
static MEMORYSTATUSEX memoryStatus;
static char osVersion[100] = "fOS 1.0 Professional (Build 400) codename nori";
static char cpuInfo[100] = "f系统1.0专用处理器";
static char memoryInfo[100] = "版权归Jellyfish Studio所有（2020-2030），严禁二次分发！";
static char buildInfo[100] = "Build 400 (fOS.1.0.Professional)";
static char versionInfo[200] = "fOS 1.0 Professional\n版本 1.0.400.2025\n编译于 2025年";
static DWORD lastSystemInfoUpdate = 0;

// 文件管理器变量 - 改为真实文件操作
static char currentPath[MAX_PATH] = "";
static char fileList[100][256];
static int fileCount = 0;
static char commandInput[100] = "";  // 命令输入
static BOOL commandMode = FALSE;     // 命令模式标志
static char commandOutput[10000000] = ""; // 命令输出 - 增大缓冲区

// 文件编辑器变量
static BOOL showFileEditor = FALSE;
static char editFileName[256] = "";
static char fileContent[100000000] = "";  // 文件内容缓冲区
static int cursorPosition = 0;
static int scrollPosition = 0;
static BOOL isEditing = FALSE;
static int textFormat = 0;  // 0:普通文本 1:代码 2:富文本 3:Markdown

// 网线接头背景相关变量
static HBITMAP hEthernetBackground = NULL;
static BOOL isEthernetBackgroundLoaded = FALSE;

// 文件信息结构
typedef struct {
    char name[256];
    char type[20];  // "DIR" 或 "FILE"
    char size[20];  // 文件大小
    char date[20];  // 修改日期
    BOOL isHidden;  // 是否隐藏文件
} FileInfo;

static FileInfo fileInfoList[100];
static int selectedFileIndex = -1;
static BOOL showHiddenFiles = FALSE;

// 简易设置软件变量
static BOOL showSettings = FALSE;
static char newUsername[50] = "";
static char newPassword[50] = "";
static char newConfirmPassword[50] = "";
static char newComputerName[50] = "";
static int settingsCurrentStep = 0;
static BOOL settingsError = FALSE;
static char settingsErrorMsg[100] = "";
static int selectedTheme = 0;  // 0:默认 1:蓝色 2:绿色 3:紫色

// 文件管理器彩蛋变量 - 基于开发历程
static int fosInputCount = 0;
static BOOL showFOSEgg = FALSE;
static int fosEggState = 0; // 0:未激活 1:旁白对话 2:开发回顾 3:功能展示 4:致谢名单
static int fosEggProgress = 0;
static DWORD lastFOSEggTime = 0;
static int currentDevStage = 0;
static int dialogueIndexx = 0; // 对话索引

static BOOL showPurpleScreen = FALSE;
static int purpleScreenProgress = 0;
static DWORD purpleScreenStartTime = 0;
static BOOL purpleScreenLocked = FALSE; // 是否锁定无法返回

// 紫屏专属错误代码
#define PURPLE_SCREEN_CRITICAL 0xF0505050  // FOS专属错误

// Bug列表相关全局变量
static BOOL showBugList = FALSE;
static int bugListCount = 0;
static int selectedBugIndex = -1;
static int bugListScroll = 0;
static BOOL isBugListHovered = FALSE;
static BOOL isBugListPressed = FALSE;
#define MAX_BUGS 100

static HIMC hIMC = NULL;
static BOOL isIMEComposing = FALSE;
// Bug结构体
typedef struct {
    char title[100];
    char description[1000];
    int severity;
    int category;
    char date[50];
    char reporter[50];
    BOOL fixed;
} BugItem;

static BugItem bugList[MAX_BUGS];

// 主题颜色定义
typedef struct {
    COLORREF desktopColor;
    COLORREF taskbarColor;
    COLORREF accentColor;
    COLORREF buttonColor;
    char themeName[20];
} Theme;

static Theme themes[] = {
    {RGB(255, 200, 220), RGB(60, 60, 60), RGB(0, 70, 140), RGB(80, 80, 80), "默认粉"},
    {RGB(200, 220, 255), RGB(50, 70, 100), RGB(30, 100, 200), RGB(70, 100, 150), "专业蓝"},
    {RGB(220, 255, 220), RGB(60, 100, 70), RGB(40, 160, 80), RGB(80, 120, 90), "清新绿"},
    {RGB(240, 220, 255), RGB(80, 60, 100), RGB(120, 80, 180), RGB(100, 80, 130), "优雅紫"}
};

static int currentThemeIndex = 0;
static int currentField = 0; // Bug报告软件当前输入字段

// 版本信息
#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define BUILD_VERSION 400
#define RELEASE_YEAR 2025

// 配置文件路径
#define CONFIG_FILE "f系统配置.dat"

// Vista风格颜色
#define VISTA_BLUE RGB(0, 70, 140)
#define VISTA_LIGHT_BLUE RGB(100, 150, 220)
#define VISTA_GREEN RGB(60, 180, 75)
#define VISTA_RED RGB(220, 80, 60)
#define VISTA_ORANGE RGB(240, 150, 30)
#define DESKTOP_PINK RGB(255, 200, 220)
#define BOOT_BLACK RGB(0, 0, 0)
#define TASKBAR_GRAY RGB(60, 60, 60)
#define TASKBAR_BUTTON RGB(80, 80, 80)
#define TASKBAR_BUTTON_HOVER RGB(100, 100, 100)
#define TASKBAR_BUTTON_PRESS RGB(120, 120, 120)
#define CALC_BG RGB(240, 240, 240)
#define CALC_BUTTON RGB(220, 220, 220)
#define CALC_OPERATOR RGB(180, 180, 220)
#define XP_BLUE RGB(0, 60, 116)
#define XP_LIGHT_BLUE RGB(185, 209, 234)
#define XP_GRAY RGB(212, 208, 200)
#define SPECIAL_BG RGB(255, 230, 240)
#define FINAL_BG RGB(255, 240, 200)
#define SHOUHUI_BODY RGB(255, 220, 100)
#define SHOUHUI_EYES RGB(0, 0, 0)
#define SHOUHUI_MOUTH RGB(255, 100, 100)
#define OOBE_BG RGB(30, 60, 120)
#define FILEMANAGER_BG RGB(255, 255, 240)
#define CDKEY_BG RGB(40, 80, 160)
#define GAME_BUTTON RGB(150, 100, 200)  // 水母游戏按钮颜色
#define EDITOR_BG RGB(255, 255, 255)
#define EDITOR_TEXT RGB(0, 0, 0)
#define EDITOR_LINE_NUM RGB(100, 100, 100)
#define EDITOR_SELECTION RGB(200, 220, 255)
#define SETTINGS_BG RGB(250, 250, 255)

// 紫屏函数声明
void DrawCriticalPurpleScreen(HDC hdc, RECT rect);
void TriggerCriticalPurpleScreen(HWND hwnd);
void HandlePurpleScreenInput(char key, HWND hwnd);

// 函数声明
void clean(HWND hwnd);
void cleanSecond(HWND hwnd);
void StartOOBE(HWND hwnd);
void CompleteOOBE(HWND hwnd);
void ShowDesktop(HWND hwnd);
void ShowLoginScreen(HWND hwnd);
void ShowShutdownAnimation(HWND hwnd);
void ShowCalculator(HWND hwnd);
void ShowSystemInfo(HWND hwnd);
void ShowFileManager(HWND hwnd);
void ShowCDKeyScreen(HWND hwnd);
void CloseAllApps(HWND hwnd);
void DrawVistaProgressBar(HDC hdc, int x, int y, int width, int height, int progress);
void DrawBootProgressBar(HDC hdc, int x, int y, int width, int height, int progress);
void DrawFOSProgressBar(HDC hdc, int x, int y, int width, int height, int progress);
void CreateBackgroundBitmap(HWND hwnd);
void DrawVistaBackground(HDC hdc, RECT rect);
void DrawFOSBetaAnimation(HDC hdc, RECT rect);
void DrawBootAnimation(HDC hdc, RECT rect);
void DrawShutdownAnimation(HDC hdc, RECT rect);
void DrawOOBEInterface(HDC hdc, int centerX, int centerY);
void DrawLoginInterface(HDC hdc, int centerX, int centerY);
void DrawDesktop(HDC hdc, RECT rect);
void DrawTaskbar(HDC hdc, RECT rect);
void DrawCalculator(HDC hdc, int centerX, int centerY);
void DrawSystemInfo(HDC hdc, int centerX, int centerY);
void DrawFileManager(HDC hdc, int centerX, int centerY);
void DrawCDKeyInterface(HDC hdc, int centerX, int centerY);
void DrawTaskbarButton(HDC hdc, int x, int y, int width, int height, const char* text, BOOL isHovered, BOOL isPressed, BOOL isActive);
void DrawCalculatorButton(HDC hdc, int x, int y, int width, int height, const char* text, BOOL isOperator);
void HandleOOBEInput(char key);
void HandleLoginInput(char key);
void HandleCalculatorInput(char key);
void HandleGuessInput(char key);
void HandleCDKeyInput(char key);
void CalculatorButtonClick(const char* button);
void SaveConfig();
void LoadConfig();
void ShutdownSystem(HWND hwnd);
BOOL VerifyLogin();
void GetCurrentDateTime(char* dateStr, char* timeStr);
void InitializeSystemInfo();
void UpdateSystemInfo();
int GetTextWidth(HDC hdc, const char* text);
void CheckSpecialMessage();
void DrawFinalEgg(HDC hdc, int centerX, int centerY);
void NormalizeString(char* str);
void DrawShouhuiMascot(HDC hdc, int x, int y, int size);
void DrawOOBEBackground(HDC hdc, RECT rect);
void DrawOOBEStep(HDC hdc, int centerX, int centerY);
void DrawOOBEProgress(HDC hdc, int centerX, int centerY);
BOOL ValidateOOBEInput();
void InitializeVersionInfo();
void DrawInputBox(HDC hdc, int x, int y, int width, int height, const char* label, const char* value, BOOL isActive, BOOL isPassword, const char* placeholder);
void RefreshFileList();
void ExecuteFile(const char* filename);
BOOL ValidateCDKey(const char* cdKey);
void StartTrialMode(HWND hwnd);
void ApplyTrialRestrictions();
BOOL IsFeatureAvailable(int feature);
void ShowOOBE(HWND hwnd);
void HandleFileManagerCommand(const char* command);
void ExecuteFileManagerCommand();
void ListFiles();
void ChangeDirectory(const char* path);
void OpenFile(const char* filename);
void ShowHelp();
void HandleFileManagerInput(char key);
void CreateFileReal(const char* filename);
void DeleteFileReal(const char* filename);
void CreateDirectoryReal(const char* dirname);
void CopyFileReal(const char* source, const char* target);
void RenameFileReal(const char* oldname, const char* newname);
void FormatFileSize(char* buffer, DWORD fileSize);

// 文件编辑器函数声明
void ShowFileEditor(HWND hwnd, const char* filename);
void DrawFileEditor(HDC hdc, int centerX, int centerY);
void HandleFileEditorInput(char key);
void SaveFileContent();
void LoadFileContent(const char* filename);
void InsertCharAtCursor(char ch);
void DeleteCharAtCursor();
void MoveCursorLeft();
void MoveCursorRight();
void MoveCursorUp();
void MoveCursorDown();
void HandleFileEditorSpecialKeys(WPARAM wParam);
void DrawTextWithFormatting(HDC hdc, int x, int y, const char* text, int format);
int GetLineLength(int lineStart);

// 简易设置软件函数声明
void ShowSettings(HWND hwnd);
void DrawSettings(HDC hdc, int centerX, int centerY);
void HandleSettingsInput(char key);
void ApplySettings();
void DrawSettingsStep(HDC hdc, int centerX, int centerY);
void DrawThemeSelector(HDC hdc, int x, int y, int width, int height);
BOOL ValidateSettingsInput();


void ProcessChineseInput(HWND hwnd, WPARAM wParam);
void AddChineseToCurrentBuffer(const char* chineseChar);

// 日历软件函数声明
void ShowCalendar(HWND hwnd);
void DrawCalendar(HDC hdc, int centerX, int centerY);
void DrawMonthView(HDC hdc, int centerX, int centerY);
void DrawYearView(HDC hdc, int centerX, int centerY);
void DrawEventDialog(HDC hdc, int centerX, int centerY);
void HandleCalendarInput(char key);
void HandleCalendarMouseClick(int x, int y, int centerX, int centerY);
int GetDaysInMonth(int year, int month);
int GetFirstDayOfMonth(int year, int month);
void LoadCalendarEvents();
void SaveCalendarEvents();
void DrawCalendarEventIndicator(HDC hdc, int x, int y, int hasEvent);
void DrawCalendarToolbar(HDC hdc, int centerX, int centerY, int topY);

// 时钟软件函数声明
void ShowClock(HWND hwnd);
void DrawClock(HDC hdc, int centerX, int centerY);
void DrawAnalogClock(HDC hdc, int centerX, int centerY);
void DrawDigitalClock(HDC hdc, int centerX, int centerY);
void DrawStopwatch(HDC hdc, int centerX, int centerY);
void DrawTimer(HDC hdc, int centerX, int centerY);
void DrawAlarm(HDC hdc, int centerX, int centerY);
void DrawClockToolbar(HDC hdc, int centerX, int centerY, int topY);
void HandleClockInput(char key);
void HandleClockMouseClick(int x, int y, int centerX, int centerY);
void UpdateStopwatch();
void UpdateTimer();
void FormatTimeString(char* buffer, DWORD milliseconds);

// 应用程序矩形区域
typedef struct {
    int left;
    int top;
    int right;
    int bottom;
} AppRect;

// 获取应用程序窗口区域
AppRect GetAppRect(HWND hwnd, int appType) {
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);
    int centerX = clientRect.right / 2;
    int centerY = clientRect.bottom / 2;
    
    AppRect rect;
    
    switch (appType) {
        case 1: // 计算器
            rect.left = centerX - 200;
            rect.top = centerY - 250;
            rect.right = centerX + 200;
            rect.bottom = centerY + 250;
            break;
        case 2: // 系统信息
            rect.left = centerX - 300;
            rect.top = centerY - 200;
            rect.right = centerX + 300;
            rect.bottom = centerY + 200;
            break;
        case 3: // 文件管理器
            rect.left = centerX - 400;
            rect.top = centerY - 300;
            rect.right = centerX + 400;
            rect.bottom = centerY + 300;
            break;
        case 4: // CD-KEY界面
            rect.left = centerX - 400;
            rect.top = centerY - 300;
            rect.right = centerX + 400;
            rect.bottom = centerY + 300;
            break;
        case 5: // 文件编辑器
            rect.left = centerX - 450;
            rect.top = centerY - 350;
            rect.right = centerX + 450;
            rect.bottom = centerY + 350;
            break;
        case 6: // 设置软件
            rect.left = centerX - 350;
            rect.top = centerY - 300;
            rect.right = centerX + 350;
            rect.bottom = centerY + 300;
            break;
        case 7: // Bug记录软件
   			rect.left = centerX - 400;
    		rect.top = centerY - 300;
    		rect.right = centerX + 400;
    		rect.bottom = centerY + 300;
    		break;
    	case 8: // 日历软件
    		rect.left = centerX - 400;
    		rect.top = centerY - 300;
    		rect.right = centerX + 400;
    		rect.bottom = centerY + 300;
		    break;
		case 9: // 时钟软件
    		rect.left = centerX - 350;
    		rect.top = centerY - 300;
    		rect.right = centerX + 350;
    		rect.bottom = centerY + 300;
    		break;
        default:
            rect.left = rect.top = rect.right = rect.bottom = 0;
    }
    
    return rect;
}

// 绘制水母求饶界面（公水母代表开发团队）
void DrawJellyfishPleaScene(HDC hdc, int centerX, int centerY) {
    RECT sceneRect = {centerX - 400, centerY - 300, centerX + 400, centerY + 300};
    
    // 海洋背景
    for (int y = sceneRect.top; y < sceneRect.bottom; y++) {
        double ratio = (double)(y - sceneRect.top) / (sceneRect.bottom - sceneRect.top);
        int r = (int)(30 * (1 - ratio) + 100 * ratio);
        int g = (int)(60 * (1 - ratio) + 150 * ratio);
        int b = (int)(120 * (1 - ratio) + 200 * ratio);
        
        COLORREF color = RGB(r, g, b);
        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, sceneRect.left, y, NULL);
        LineTo(hdc, sceneRect.right, y);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
    
    // 绘制海底沙子
    RECT sandRect = {sceneRect.left, sceneRect.bottom - 100, sceneRect.right, sceneRect.bottom};
    HBRUSH hSandBrush = CreateSolidBrush(RGB(210, 180, 140));
    FillRect(hdc, &sandRect, hSandBrush);
    DeleteObject(hSandBrush);
    
    // 绘制一些海草
    HPEN hSeaweedPen = CreatePen(PS_SOLID, 3, RGB(60, 180, 75));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hSeaweedPen);
    
    for (int i = 0; i < 5; i++) {
        int x = sceneRect.left + 80 + i * 160;
        int y = sceneRect.bottom - 100;
        
        MoveToEx(hdc, x, y, NULL);
        for (int j = 0; j < 8; j++) {
            int offset = (int)(sin(j * 0.8 + i * 0.5) * 20);
            LineTo(hdc, x + offset, y - j * 15);
        }
    }
    SelectObject(hdc, hOldPen);
    DeleteObject(hSeaweedPen);
    
    // 绘制惊恐的公水母（代表开发团队）
    DrawPleadingJellyfish(hdc, centerX, centerY - 50, 100);
    
    // 对话框1 - 水母的求饶
    RECT dialogRect1 = {centerX - 350, centerY + 80, centerX + 350, centerY + 130};
    HBRUSH hDialogBrush1 = CreateSolidBrush(RGB(255, 255, 200));
    FillRect(hdc, &dialogRect1, hDialogBrush1);
    DeleteObject(hDialogBrush1);
    
    HPEN hDialogPen1 = CreatePen(PS_SOLID, 2, RGB(150, 150, 100));
    hOldPen = (HPEN)SelectObject(hdc, hDialogPen1);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    RoundRect(hdc, dialogRect1.left, dialogRect1.top, dialogRect1.right, dialogRect1.bottom, 15, 15);
    SelectObject(hdc, hOldPen);
    DeleteObject(hDialogPen1);
    
    // 对话框2 - 团队声明
    RECT dialogRect2 = {centerX - 350, centerY + 140, centerX + 350, centerY + 200};
    HBRUSH hDialogBrush2 = CreateSolidBrush(RGB(200, 230, 255));
    FillRect(hdc, &dialogRect2, hDialogBrush2);
    DeleteObject(hDialogBrush2);
    
    HPEN hDialogPen2 = CreatePen(PS_SOLID, 2, RGB(100, 150, 200));
    hOldPen = (HPEN)SelectObject(hdc, hDialogPen2);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    RoundRect(hdc, dialogRect2.left, dialogRect2.top, dialogRect2.right, dialogRect2.bottom, 15, 15);
    SelectObject(hdc, hOldPen);
    DeleteObject(hDialogPen2);
    
    // 对话框文本
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hDialogFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hDialogFont);
    
    const char* pleaText = "不要杀了我！我可是开发团队水母！";
    TextOut(hdc, centerX - GetTextWidth(hdc, pleaText) / 2, centerY + 90, pleaText, strlen(pleaText));
    
    // 团队声明文本
    HFONT hTeamFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "宋体");
    SelectObject(hdc, hTeamFont);
    
    const char* teamText1 = "我代表整个f系统开发团队！杀了我谁给你写代码？";
    const char* teamText2 = "我身后可是整个开发团队的尊严！";
    
    TextOut(hdc, centerX - GetTextWidth(hdc, teamText1) / 2, centerY + 150, teamText1, strlen(teamText1));
    TextOut(hdc, centerX - GetTextWidth(hdc, teamText2) / 2, centerY + 170, teamText2, strlen(teamText2));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hDialogFont);
    DeleteObject(hTeamFont);
    
    // 在水母旁边添加"♂"雄性符号
    HFONT hMaleSymbolFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    SelectObject(hdc, hMaleSymbolFont);
    SetTextColor(hdc, RGB(0, 100, 200));
    DeleteObject(hMaleSymbolFont);
    
    // 说明文字
    HFONT hHintFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "宋体");
    hOldFont = (HFONT)SelectObject(hdc, hHintFont);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    const char* hintText = "按ESC键返回文件管理器";
    TextOut(hdc, centerX - GetTextWidth(hdc, hintText) / 2, sceneRect.bottom - 30, hintText, strlen(hintText));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hHintFont);
}
// 绘制惊恐的求饶公水母
void DrawPleadingJellyfish(HDC hdc, int x, int y, int size) {
    HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
    
    // 水母头部 - 雄性蓝色，带胡子效果
    int pulse = (int)(8 * sin(GetTickCount() * 0.01f));
    HBRUSH hHeadBrush = CreateSolidBrush(RGB(0, 100, 200));  // 雄性蓝色
    SelectObject(hdc, hHeadBrush);
    
    int headSize = size - pulse;
    Ellipse(hdc, x - headSize/2, y - headSize/3, 
            x + headSize/2, y + headSize/3);
    
    // 内部光晕 - 更深蓝色表示雄性
    HBRUSH hInnerBrush = CreateSolidBrush(RGB(50, 150, 220));
    SelectObject(hdc, hInnerBrush);
    Ellipse(hdc, x - headSize/3, y - headSize/5, 
            x + headSize/3, y + headSize/5);
    DeleteObject(hInnerBrush);
    DeleteObject(hHeadBrush);
    
    // 绘制胡子（雄性特征）
    HPEN hBeardPen = CreatePen(PS_SOLID, 2, RGB(50, 50, 50));
    HPEN hOldBeardPen = (HPEN)SelectObject(hdc, hBeardPen);
    
    // 左胡子
    MoveToEx(hdc, x - size/4, y + size/8, NULL);
    LineTo(hdc, x - size/2, y + size/6);
    MoveToEx(hdc, x - size/4, y + size/8, NULL);
    LineTo(hdc, x - size/2, y);
    MoveToEx(hdc, x - size/4, y + size/8, NULL);
    LineTo(hdc, x - size/2, y - size/6);
    
    // 右胡子
    MoveToEx(hdc, x + size/4, y + size/8, NULL);
    LineTo(hdc, x + size/2, y + size/6);
    MoveToEx(hdc, x + size/4, y + size/8, NULL);
    LineTo(hdc, x + size/2, y);
    MoveToEx(hdc, x + size/4, y + size/8, NULL);
    LineTo(hdc, x + size/2, y - size/6);
    
    SelectObject(hdc, hOldBeardPen);
    DeleteObject(hBeardPen);
    
    // 惊恐的雄性表情
    HPEN hFacePen = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
    SelectObject(hdc, hFacePen);
    
    // 惊恐的浓眉大眼（雄性特征）
    // 左眉毛（浓眉）
    MoveToEx(hdc, x - size/3, y - size/6, NULL);
    LineTo(hdc, x - size/8, y - size/6);
    // 右眉毛
    MoveToEx(hdc, x + size/8, y - size/6, NULL);
    LineTo(hdc, x + size/3, y - size/6);
    
    // 惊恐的大眼睛
    Ellipse(hdc, x - size/3, y - size/8, x - size/6, y + size/8);
    Ellipse(hdc, x + size/6, y - size/8, x + size/3, y + size/8);
    
    // 红色眼珠（极度惊恐）
    HBRUSH hPupilBrush = CreateSolidBrush(RGB(255, 0, 0));
    SelectObject(hdc, hPupilBrush);
    Ellipse(hdc, x - size/4, y - size/16, x - size/5, y + size/16);
    Ellipse(hdc, x + size/5, y - size/16, x + size/4, y + size/16);
    DeleteObject(hPupilBrush);
    
    // 惊恐的张嘴大喊
    Arc(hdc, x - size/4, y + size/4, x + size/4, y + size/2,
        x - size/4, y + size/4, x + size/4, y + size/4);
    
    // 牙齿（惊恐到露出牙齿）
    HPEN hTeethPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
    SelectObject(hdc, hTeethPen);
    for (int i = -2; i <= 2; i++) {
        int toothX = x + i * (size/12);
        MoveToEx(hdc, toothX, y + size/4, NULL);
        LineTo(hdc, toothX, y + size/3);
    }
    SelectObject(hdc, hFacePen);
    DeleteObject(hTeethPen);
    
    DeleteObject(hFacePen);
    
    // 颤抖的雄性触须（更粗壮）
    HPEN hTentaclePen = CreatePen(PS_SOLID, 4, RGB(0, 80, 160));  // 深蓝色雄性触须
    SelectObject(hdc, hTentaclePen);
    
    float time = GetTickCount() * 0.01f;
    for (int i = 0; i < 12; i++) {
        double angle = 2 * 3.14159 * i / 12;
        int startX = x + (int)(size/4 * cos(angle));
        int startY = y + size/4 + (int)(size/8 * sin(angle));
        
        MoveToEx(hdc, startX, startY, NULL);
        
        // 强烈颤抖效果（极度恐惧）
        for (int t = 0; t <= 80; t += 5) {
            double progress = t / 80.0;
            float waveFactor = (float)(sin(time * 10 + i * 0.8 + progress * 15) * 
                                    (1.0 - progress) * 30);  // 增加颤抖幅度
            
            int currentX = startX + (int)(size * 0.6 * cos(angle) * progress);
            int currentY = startY + (int)(size * 0.8 * progress);
            
            LineTo(hdc, currentX + (int)(waveFactor * cos(angle + 1.57)), 
                      currentY + (int)(waveFactor * sin(angle + 1.57)));
        }
    }
    
    DeleteObject(hTentaclePen);
    SelectObject(hdc, hOldPen);
}
// 绘制致命紫屏 - 修改版
void DrawCriticalPurpleScreen(HDC hdc, RECT rect) {
//	PlaySound("音乐\\叮.mp3", NULL, SND_FILENAME | SND_ASYNC);
    // 深紫色背景
    HBRUSH hPurpleBrush = CreateSolidBrush(RGB(80, 0, 120));
    FillRect(hdc, &rect, hPurpleBrush);
    DeleteObject(hPurpleBrush);
    
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    
    HFONT hTitleFont = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hNormalFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hSmallFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hChineseFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        GB2312_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "微软雅黑");
    
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    int centerX = rect.right / 2;
    
    // 致命错误标题
    TextOut(hdc, centerX - 200, 80, "f系统遇到致命错误", 17);
    
    SelectObject(hdc, hNormalFont);
    
    // 错误信息
    TextOut(hdc, centerX - 300, 130, "系统核心组件出现不可恢复的错误", 30);
    TextOut(hdc, centerX - 300, 160, "文件管理器触发了系统保护机制", 28);
    
    // 错误代码
    char errorMsg[100];
    sprintf(errorMsg, "终止代码: FOS_CRITICAL_ERROR_0x%08X", PURPLE_SCREEN_CRITICAL);
    TextOut(hdc, centerX - 300, 190, errorMsg, strlen(errorMsg));
    
    // 收集信息进度
    TextOut(hdc, centerX - 300, 230, "正在收集错误信息...", 19);
    
    // 进度条
    RECT progressBg = {centerX - 200, 260, centerX + 200, 280};
    HBRUSH hProgressBg = CreateSolidBrush(RGB(60, 0, 90));
    FillRect(hdc, &progressBg, hProgressBg);
    DeleteObject(hProgressBg);
    
    RECT progressBar = {centerX - 200, 260, centerX - 200 + (400 * purpleScreenProgress / 100), 280};
    HBRUSH hProgress = CreateSolidBrush(RGB(200, 100, 255));
    FillRect(hdc, &progressBar, hProgress);
    DeleteObject(hProgress);
    
    char progressText[50];
    sprintf(progressText, "%d%%", purpleScreenProgress);
    TextOut(hdc, centerX - 20, 260, progressText, strlen(progressText));
    
    // 技术信息
    SelectObject(hdc, hSmallFont);
    TextOut(hdc, centerX - 300, 310, "*** 技术信息 ***", 16);
    
    char techInfo1[100];
    sprintf(techInfo1, "*** 模块: FILE_MANAGER");
    TextOut(hdc, centerX - 300, 330, techInfo1, strlen(techInfo1));
    
    char techInfo2[100];
    sprintf(techInfo2, "*** 地址: 0x%08X", rand() % 0xFFFFFFFF);
    TextOut(hdc, centerX - 300, 350, techInfo2, strlen(techInfo2));
    
    char techInfo3[100];
    sprintf(techInfo3, "*** 时间戳: %d", GetTickCount());
    TextOut(hdc, centerX - 300, 370, techInfo3, strlen(techInfo3));
    
    // 重启提示
    SelectObject(hdc, hNormalFont);
    TextOut(hdc, centerX - 250, 420, "此错误无法自动修复，请手动重启系统", 34);
    TextOut(hdc, centerX - 200, 450, "按 Ctrl+Alt+E 关闭 f系统", 24);
    DrawShouhuiMascot(hdc, centerX - 40, 500, 80);
        
    // 使用中文字体显示幽默提示
    SelectObject(hdc, hChineseFont);
    TextOut(hdc, centerX - 180, 590, "连Jellyfish Studio的核心成员小水母也救不了这个错误...", 53);
        
    // 额外的幽默技术信息
    SelectObject(hdc, hSmallFont);
    TextOut(hdc, centerX - 300, 620, "*** 开发者备注: 这个错误太严重了，我也没办法！ ***", 50);
    TextOut(hdc, centerX - 300, 640, "*** 建议: 不要随便在文件管理器输入 'error' ***", 46);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hNormalFont);
    DeleteObject(hSmallFont);
    DeleteObject(hChineseFont);
}

// 触发致命紫屏
void TriggerCriticalPurpleScreen(HWND hwnd) {
    showPurpleScreen = TRUE;
    purpleScreenProgress = 0;
    purpleScreenStartTime = GetTickCount();
    purpleScreenLocked = TRUE; // 锁定无法返回
    
    // 关闭所有其他界面
    showDesktop = FALSE;
    showCalculator = FALSE;
    showSystemInfo = FALSE;
    showFileManager = FALSE;
    showFileEditor = FALSE;
    showSettings = FALSE;
    showGameEgg = FALSE;
    
    // 设置紫屏定时器
    SetTimer(hwnd, 12, 80, NULL); // 进度更新
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 紫屏输入处理 - 大部分按键被锁定
void HandlePurpleScreenInput(char key, HWND hwnd) {
    // 只有Ctrl+Alt+E可以重启
    if ((key == 'E' || key == 'e') && GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_MENU)) {
        // 模拟重启
//        PlaySound("音乐\\叮.wav", NULL, SND_FILENAME | SND_ASYNC);
        showPurpleScreen = FALSE;
        purpleScreenLocked = FALSE;
        ShowShutdownAnimation(hwnd);
    }
    // 其他所有按键都被忽略，用户无法返回
}

//废弃代码\
// 创建精细的网线接头背景\
void CreateEthernetBackground(HWND hwnd) {\
    HDC hdc = GetDC(hwnd);\
    HDC hMemDC = CreateCompatibleDC(hdc);\
    hEthernetBackground = CreateCompatibleBitmap(hdc, 800, 600);\
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hEthernetBackground);\
    \
    // Windows 95风格的深蓝色渐变背景\
    for (int y = 0; y < 600; y++) {\
        double ratio = (double)y / 600;\
        int r = (int)(0 * (1 - ratio) + 50 * ratio);\
        int g = (int)(0 * (1 - ratio) + 50 * ratio);\
        int b = (int)(128 * (1 - ratio) + 100 * ratio);\
        \
        COLORREF color = RGB(r, g, b);\
        HPEN hPen = CreatePen(PS_SOLID, 1, color);\
        HPEN hOldPen = (HPEN)SelectObject(hMemDC, hPen);\
        MoveToEx(hMemDC, 0, y, NULL);\
        LineTo(hMemDC, 800, y);\
        SelectObject(hMemDC, hOldPen);\
        DeleteObject(hPen);\
    }\
    \
    // 绘制5个网线接头和连接线\
    DrawEthernetScene(hMemDC);\
    \
    // 恢复对象\
    SelectObject(hMemDC, hOldBitmap);\
    DeleteDC(hMemDC);\
    ReleaseDC(hwnd, hdc);\
    \
    isEthernetBackgroundLoaded = TRUE;\
}\
//废弃代码完 


// 显示Bug列表
void ShowBugList(HWND hwnd) {
    CloseAllApps(hwnd);
    showBugList = TRUE;
    selectedBugIndex = -1;
    bugListScroll = 0;
    LoadBugListFromFile();
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 绘制Bug列表界面
void DrawBugList(HDC hdc, int centerX, int centerY) {
    RECT bugListRect = {centerX - 400, centerY - 300, centerX + 400, centerY + 300};
    
    // 绘制窗口背景
    HBRUSH hBgBrush = CreateSolidBrush(RGB(250, 250, 255));
    FillRect(hdc, &bugListRect, hBgBrush);
    DeleteObject(hBgBrush);
    
    // 绘制窗口边框
    HPEN hBorderPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 200));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, bugListRect.left, bugListRect.top, bugListRect.right, bugListRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    // 绘制标题栏
    RECT titleRect = {bugListRect.left, bugListRect.top, bugListRect.right, bugListRect.top + 40};
    HBRUSH hTitleBrush = CreateSolidBrush(RGB(70, 130, 180));
    FillRect(hdc, &titleRect, hTitleBrush);
    DeleteObject(hTitleBrush);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hTitleFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    TextOut(hdc, bugListRect.left + 10, bugListRect.top + 8, "Bug列表 - 共", 10);
    
    char countText[50];
    sprintf(countText, "%d 个Bug", bugListCount);
    TextOut(hdc, bugListRect.left + 100, bugListRect.top + 8, countText, strlen(countText));
    
    // 在标题栏右侧绘制守慧吉祥物
    DrawShouhuiMascot(hdc, bugListRect.right - 50, bugListRect.top - 5, 40);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    
    // 绘制列表区域
    RECT listRect = {bugListRect.left + 20, bugListRect.top + 50, bugListRect.right - 20, bugListRect.bottom - 80};
    HBRUSH hListBgBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &listRect, hListBgBrush);
    DeleteObject(hListBgBrush);
    
    HPEN hListBorderPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    hOldPen = (HPEN)SelectObject(hdc, hListBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, listRect.left, listRect.top, listRect.right, listRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hListBorderPen);
    
    // 绘制列标题
    HFONT hHeaderFont = CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    hOldFont = (HFONT)SelectObject(hdc, hHeaderFont);
    
    SetTextColor(hdc, RGB(0, 0, 0));
    
    // 列标题
    int col1 = listRect.left + 10;
    int col2 = listRect.left + 200;
    int col3 = listRect.left + 300;
    int col4 = listRect.left + 400;
    int col5 = listRect.left + 500;
    
    TextOut(hdc, col1, listRect.top + 5, "状态", 4);
    TextOut(hdc, col2, listRect.top + 5, "标题", 4);
    TextOut(hdc, col3, listRect.top + 5, "分类", 4);
    TextOut(hdc, col4, listRect.top + 5, "严重程度", 8);
    TextOut(hdc, col5, listRect.top + 5, "日期", 4);
    
    // 绘制分隔线
    HPEN hLinePen = CreatePen(PS_SOLID, 1, RGB(220, 220, 220));
    hOldPen = (HPEN)SelectObject(hdc, hLinePen);
    MoveToEx(hdc, listRect.left, listRect.top + 30, NULL);
    LineTo(hdc, listRect.right, listRect.top + 30);
    SelectObject(hdc, hOldPen);
    DeleteObject(hLinePen);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hHeaderFont);
    
    // 绘制Bug列表项
    HFONT hItemFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    hOldFont = (HFONT)SelectObject(hdc, hItemFont);
    
    int itemHeight = 25;
    int visibleItems = (listRect.bottom - listRect.top - 40) / itemHeight;
    int startIndex = bugListScroll;
    int endIndex = min(startIndex + visibleItems, bugListCount);
    
    for (int i = startIndex; i < endIndex; i++) {
        int itemY = listRect.top + 40 + (i - startIndex) * itemHeight;
        
        // 绘制选中背景
        if (i == selectedBugIndex) {
            RECT selectedRect = {listRect.left, itemY, listRect.right, itemY + itemHeight};
            HBRUSH hSelectedBrush = CreateSolidBrush(RGB(200, 220, 255));
            FillRect(hdc, &selectedRect, hSelectedBrush);
            DeleteObject(hSelectedBrush);
        }
        
        // 状态复选框
        RECT checkRect = {col1, itemY + 5, col1 + 15, itemY + 20};
        HPEN hCheckPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
        hOldPen = (HPEN)SelectObject(hdc, hCheckPen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, checkRect.left, checkRect.top, checkRect.right, checkRect.bottom);
        
        if (bugList[i].fixed) {
            // 绘制勾选标记
            HPEN hTickPen = CreatePen(PS_SOLID, 2, RGB(0, 150, 0));
            SelectObject(hdc, hTickPen);
            MoveToEx(hdc, checkRect.left + 3, checkRect.top + 8, NULL);
            LineTo(hdc, checkRect.left + 6, checkRect.top + 13);
            LineTo(hdc, checkRect.left + 12, checkRect.top + 5);
            DeleteObject(hTickPen);
        }
        
        SelectObject(hdc, hOldPen);
        DeleteObject(hCheckPen);
        
        // Bug标题（截断过长的标题）
        char displayTitle[30];
        strncpy(displayTitle, bugList[i].title, 25);
        displayTitle[25] = '\0';
        if (strlen(bugList[i].title) > 25) {
            strcat(displayTitle, "...");
        }
        
        SetTextColor(hdc, bugList[i].fixed ? RGB(100, 100, 100) : RGB(0, 0, 0));
        TextOut(hdc, col2, itemY + 5, displayTitle, strlen(displayTitle));
        
        // 分类
        const char* categories[] = {"界面", "功能", "性能", "崩溃", "其他"};
        TextOut(hdc, col3, itemY + 5, categories[bugList[i].category], strlen(categories[bugList[i].category]));
        
        // 严重程度（带颜色）
        const char* severities[] = {"低", "中", "高", "严重"};
        COLORREF severityColors[] = {RGB(0, 150, 0), RGB(200, 150, 0), RGB(255, 100, 0), RGB(255, 0, 0)};
        
        SetTextColor(hdc, severityColors[bugList[i].severity - 1]);
        TextOut(hdc, col4, itemY + 5, severities[bugList[i].severity - 1], strlen(severities[bugList[i].severity - 1]));
        
        // 日期
        SetTextColor(hdc, bugList[i].fixed ? RGB(100, 100, 100) : RGB(0, 0, 0));
        TextOut(hdc, col5, itemY + 5, bugList[i].date, strlen(bugList[i].date));
    }
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hItemFont);
    
    // 绘制滚动条（如果需要）
    if (bugListCount > visibleItems) {
        RECT scrollRect = {listRect.right - 15, listRect.top + 40, listRect.right - 5, listRect.bottom - 10};
        HBRUSH hScrollBgBrush = CreateSolidBrush(RGB(240, 240, 240));
        FillRect(hdc, &scrollRect, hScrollBgBrush);
        DeleteObject(hScrollBgBrush);
        
        int scrollHeight = scrollRect.bottom - scrollRect.top;
        int thumbHeight = scrollHeight * visibleItems / bugListCount;
        int thumbY = scrollRect.top + (scrollHeight - thumbHeight) * bugListScroll / bugListCount;
        
        RECT thumbRect = {scrollRect.left, thumbY, scrollRect.right, thumbY + thumbHeight};
        HBRUSH hThumbBrush = CreateSolidBrush(RGB(150, 150, 150));
        FillRect(hdc, &thumbRect, hThumbBrush);
        DeleteObject(hThumbBrush);
    }
    
    // 绘制操作按钮
    int buttonWidth = 100;
    int buttonHeight = 35;
    int buttonY = bugListRect.bottom - 60;
    int buttonSpacing = 120;
    
    // 标记修复/取消修复按钮
    RECT toggleButtonRect = {centerX - buttonSpacing * 2, buttonY, centerX - buttonSpacing * 2 + buttonWidth, buttonY + buttonHeight};
    HBRUSH hToggleBrush = CreateSolidBrush(selectedBugIndex >= 0 ? RGB(0, 150, 0) : RGB(180, 180, 180));
    FillRect(hdc, &toggleButtonRect, hToggleBrush);
    DeleteObject(hToggleBrush);
    
    HPEN hTogglePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    hOldPen = (HPEN)SelectObject(hdc, hTogglePen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX - buttonSpacing * 2, buttonY, centerX - buttonSpacing * 2 + buttonWidth, buttonY + buttonHeight);
    SelectObject(hdc, hOldPen);
    DeleteObject(hTogglePen);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    HFONT hButtonFont = CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    hOldFont = (HFONT)SelectObject(hdc, hButtonFont);
    
    if (selectedBugIndex >= 0 && bugList[selectedBugIndex].fixed) {
        TextOut(hdc, centerX - buttonSpacing * 2 + 15, buttonY + 10, "取消修复", 8);
    } else {
        TextOut(hdc, centerX - buttonSpacing * 2 + 15, buttonY + 10, "标记修复", 8);
    }
    
    // 删除按钮
    RECT deleteButtonRect = {centerX - buttonSpacing, buttonY, centerX - buttonSpacing + buttonWidth, buttonY + buttonHeight};
    HBRUSH hDeleteBrush = CreateSolidBrush(selectedBugIndex >= 0 ? RGB(200, 0, 0) : RGB(180, 180, 180));
    FillRect(hdc, &deleteButtonRect, hDeleteBrush);
    DeleteObject(hDeleteBrush);
    
    HPEN hDeletePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    hOldPen = (HPEN)SelectObject(hdc, hDeletePen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX - buttonSpacing, buttonY, centerX - buttonSpacing + buttonWidth, buttonY + buttonHeight);
    SelectObject(hdc, hOldPen);
    DeleteObject(hDeletePen);
    
    TextOut(hdc, centerX - buttonSpacing + 30, buttonY + 10, "删除", 4);
    
    // 刷新按钮
    RECT refreshButtonRect = {centerX, buttonY, centerX + buttonWidth, buttonY + buttonHeight};
    HBRUSH hRefreshBrush = CreateSolidBrush(RGB(0, 100, 200));
    FillRect(hdc, &refreshButtonRect, hRefreshBrush);
    DeleteObject(hRefreshBrush);
    
    HPEN hRefreshPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    hOldPen = (HPEN)SelectObject(hdc, hRefreshPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX, buttonY, centerX + buttonWidth, buttonY + buttonHeight);
    SelectObject(hdc, hOldPen);
    DeleteObject(hRefreshPen);
    
    TextOut(hdc, centerX + 30, buttonY + 10, "刷新", 4);
    
    // 关闭按钮
    RECT closeButtonRect = {centerX + buttonSpacing, buttonY, centerX + buttonSpacing + buttonWidth, buttonY + buttonHeight};
    HBRUSH hCloseBrush = CreateSolidBrush(RGB(100, 100, 100));
    FillRect(hdc, &closeButtonRect, hCloseBrush);
    DeleteObject(hCloseBrush);
    
    HPEN hClosePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    hOldPen = (HPEN)SelectObject(hdc, hClosePen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX + buttonSpacing, buttonY, centerX + buttonSpacing + buttonWidth, buttonY + buttonHeight);
    SelectObject(hdc, hOldPen);
    DeleteObject(hClosePen);
    
    TextOut(hdc, centerX + buttonSpacing + 30, buttonY + 10, "关闭", 4);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hButtonFont);
    
    // 绘制操作提示
    HFONT hHintFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    hOldFont = (HFONT)SelectObject(hdc, hHintFont);
    
    SetTextColor(hdc, RGB(100, 100, 100));
    TextOut(hdc, bugListRect.left + 20, bugListRect.bottom - 30, 
           "提示: 使用上下箭头选择Bug，空格键切换修复状态，Delete键删除选中的Bug", 62);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hHintFont);
}

// 处理Bug列表输入
void HandleBugListInput(char key) {
    int visibleItems = 15; // 可视项目数
    
    switch (key) {
        case 27: // ESC键关闭
            showBugList = FALSE;
            break;
            
        case 38: // 上箭头
            if (selectedBugIndex > 0) {
                selectedBugIndex--;
                // 滚动逻辑
                if (selectedBugIndex < bugListScroll) {
                    bugListScroll = selectedBugIndex;
                }
            }
            break;
            
        case 40: // 下箭头
            if (selectedBugIndex < bugListCount - 1) {
                selectedBugIndex++;
                // 滚动逻辑
                if (selectedBugIndex >= bugListScroll + visibleItems) {
                    bugListScroll = selectedBugIndex - visibleItems + 1;
                }
            }
            break;
            
        case 32: // 空格键切换修复状态
            if (selectedBugIndex >= 0) {
                ToggleBugFixedStatus(selectedBugIndex);
            }
            break;
            
        case 8:  // Backspace键
        case 46: // Delete键删除
            if (selectedBugIndex >= 0) {
                DeleteBug(selectedBugIndex);
                if (selectedBugIndex >= bugListCount) {
                    selectedBugIndex = bugListCount - 1;
                }
            }
            break;
            
        case 13: // Enter键刷新
            LoadBugListFromFile();
            break;
    }
}

// 从文件加载Bug列表
void LoadBugListFromFile() {
    bugListCount = 0;
    
    FILE* file = fopen("bug.dat", "r");
    if (!file) {
        return;
    }
    
    char line[256];
    BugItem currentBug;
    memset(&currentBug, 0, sizeof(BugItem));
    
    while (fgets(line, sizeof(line), file) && bugListCount < MAX_BUGS) {
        if (strstr(line, "=== Bug 报告 ===")) {
            // 新的Bug记录开始
            memset(&currentBug, 0, sizeof(BugItem));
            currentBug.fixed = FALSE; // 默认未修复
        }
        else if (strstr(line, "时间:")) {
            // 提取时间
            char* timePos = strstr(line, ":");
            if (timePos) {
                strcpy(currentBug.date, timePos + 2);
                currentBug.date[strlen(currentBug.date) - 1] = '\0'; // 移除换行符
            }
        }
        else if (strstr(line, "用户:")) {
            // 提取报告者
            char* userPos = strstr(line, ":");
            if (userPos) {
                strcpy(currentBug.reporter, userPos + 2);
                currentBug.reporter[strlen(currentBug.reporter) - 1] = '\0';
            }
        }
        else if (strstr(line, "分类:")) {
            // 提取分类
            char* catPos = strstr(line, ":");
            if (catPos) {
                char category[20];
                strcpy(category, catPos + 2);
                category[strlen(category) - 1] = '\0';
                
                const char* categories[] = {"界面问题", "功能异常", "性能问题", "系统崩溃", "其他问题"};
                for (int i = 0; i < 5; i++) {
                    if (strcmp(category, categories[i]) == 0) {
                        currentBug.category = i;
                        break;
                    }
                }
            }
        }
        else if (strstr(line, "严重程度:")) {
            // 提取严重程度
            char* sevPos = strstr(line, ":");
            if (sevPos) {
                char severity[10];
                strcpy(severity, sevPos + 2);
                severity[strlen(severity) - 1] = '\0';
                
                const char* severities[] = {"低", "中", "高", "严重"};
                for (int i = 0; i < 4; i++) {
                    if (strcmp(severity, severities[i]) == 0) {
                        currentBug.severity = i + 1;
                        break;
                    }
                }
            }
        }
        else if (strstr(line, "标题:")) {
            // 提取标题
            char* titlePos = strstr(line, ":");
            if (titlePos) {
                strcpy(currentBug.title, titlePos + 2);
                currentBug.title[strlen(currentBug.title) - 1] = '\0';
            }
        }
        else if (strstr(line, "描述:")) {
            // 开始读取描述（多行）
            currentBug.description[0] = '\0';
            while (fgets(line, sizeof(line), file)) {
                if (strstr(line, "================")) {
                    // Bug记录结束
                    bugList[bugListCount++] = currentBug;
                    break;
                }
                else if (strlen(line) > 1) { // 不是空行
                    // 添加到描述中
                    if (strlen(currentBug.description) + strlen(line) < sizeof(currentBug.description) - 1) {
                        strcat(currentBug.description, line);
                    }
                }
            }
        }
    }
    
    fclose(file);
    
    // 尝试加载修复状态
    FILE* stateFile = fopen("bug_state.dat", "rb");
    if (stateFile) {
        BOOL fixedStates[MAX_BUGS];
        fread(fixedStates, sizeof(BOOL), MAX_BUGS, stateFile);
        for (int i = 0; i < bugListCount && i < MAX_BUGS; i++) {
            bugList[i].fixed = fixedStates[i];
        }
        fclose(stateFile);
    }
}

// 保存Bug列表状态到文件
void SaveBugListToFile() {
    FILE* stateFile = fopen("bug_state.dat", "wb");
    if (stateFile) {
        BOOL fixedStates[MAX_BUGS];
        for (int i = 0; i < bugListCount; i++) {
            fixedStates[i] = bugList[i].fixed;
        }
        fwrite(fixedStates, sizeof(BOOL), bugListCount, stateFile);
        fclose(stateFile);
    }
}

// 切换Bug修复状态
void ToggleBugFixedStatus(int index) {
    if (index >= 0 && index < bugListCount) {
        bugList[index].fixed = !bugList[index].fixed;
        SaveBugListToFile();
    }
}

// 删除Bug
void DeleteBug(int index) {
    if (index < 0 || index >= bugListCount) return;
    
    // 从数组中移除
    for (int i = index; i < bugListCount - 1; i++) {
        bugList[i] = bugList[i + 1];
    }
    bugListCount--;
    
    // 重新保存状态文件
    SaveBugListToFile();
}

// 绘制精细的网线场景
void DrawEthernetScene(HDC hdc) {
    // 网线接头位置
    POINT jackPositions[] = {
        {100, 150},  // 左上
        {300, 100},  // 中上
        {500, 150},  // 右上
        {200, 400},  // 左下
        {400, 400}   // 右下
    };
    
    // 绘制连接线（星型拓扑）
    DrawEthernetCables(hdc, jackPositions);
    
    // 绘制5个网线接头
    for (int i = 0; i < 5; i++) {
        DrawDetailedEthernetJack(hdc, jackPositions[i].x, jackPositions[i].y, i);
    }
}

// 绘制详细的网线接头
void DrawDetailedEthernetJack(HDC hdc, int x, int y, int jackId) {
    // 保存原始画笔和画刷
    HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    
    // 接头外壳 - 3D效果
    int jackWidth = 120;
    int jackHeight = 60;
    
    // 外壳主体 - 浅灰色
    HBRUSH hShellBrush = CreateSolidBrush(RGB(192, 192, 192));
    SelectObject(hdc, hShellBrush);
    RoundRect(hdc, x, y, x + jackWidth, y + jackHeight, 8, 8);
    
    // 3D边框效果
    HPEN hLightPen = CreatePen(PS_SOLID, 2, RGB(240, 240, 240));  // 亮边
    HPEN hDarkPen = CreatePen(PS_SOLID, 2, RGB(128, 128, 128));   // 暗边
    
    // 左上亮边
    SelectObject(hdc, hLightPen);
    MoveToEx(hdc, x + 1, y + jackHeight - 2, NULL);
    LineTo(hdc, x + 1, y + 1);
    LineTo(hdc, x + jackWidth - 2, y + 1);
    
    // 右下暗边
    SelectObject(hdc, hDarkPen);
    MoveToEx(hdc, x + jackWidth - 1, y + 1, NULL);
    LineTo(hdc, x + jackWidth - 1, y + jackHeight - 1);
    LineTo(hdc, x + 1, y + jackHeight - 1);
    
    // RJ45接口槽
    int slotWidth = 40;
    int slotHeight = 25;
    int slotX = x + (jackWidth - slotWidth) / 2;
    int slotY = y + 15;
    
    HBRUSH hSlotBrush = CreateSolidBrush(RGB(0, 0, 0));
    SelectObject(hdc, hSlotBrush);
    Rectangle(hdc, slotX, slotY, slotX + slotWidth, slotY + slotHeight);
    
    // 金属触点 - 8个，精细绘制
    int contactSpacing = 4;
    int contactStartX = slotX + 6;
    int contactY = slotY + 5;
    int contactHeight = 15;
    
    for (int i = 0; i < 8; i++) {
        int contactX = contactStartX + i * contactSpacing;
        
        // 触点底座 - 银色
        HBRUSH hContactBaseBrush = CreateSolidBrush(RGB(200, 200, 200));
        SelectObject(hdc, hContactBaseBrush);
        Rectangle(hdc, contactX, contactY, contactX + 2, contactY + contactHeight);
        DeleteObject(hContactBaseBrush);
        
        // 触点金属部分 - 金色，有光泽效果
        HBRUSH hContactBrush = CreateSolidBrush(RGB(218, 165, 32));
        SelectObject(hdc, hContactBrush);
        Rectangle(hdc, contactX, contactY + 2, contactX + 2, contactY + contactHeight - 2);
        DeleteObject(hContactBrush);
    }
    
    // 锁扣机构
    int latchWidth = 15;
    int latchHeight = 8;
    int latchX = slotX + slotWidth + 5;
    int latchY = slotY + (slotHeight - latchHeight) / 2;
    
    HBRUSH hLatchBrush = CreateSolidBrush(RGB(150, 150, 150));
    SelectObject(hdc, hLatchBrush);
    RoundRect(hdc, latchX, latchY, latchX + latchWidth, latchY + latchHeight, 3, 3);
    
    // 锁扣按钮
    HBRUSH hLatchButtonBrush = CreateSolidBrush(RGB(100, 100, 100));
    SelectObject(hdc, hLatchButtonBrush);
    Ellipse(hdc, latchX + latchWidth - 4, latchY - 2, latchX + latchWidth + 4, latchY + latchHeight + 2);
    
    // 状态指示灯（不同接头不同颜色）
    COLORREF statusColors[] = {
        RGB(0, 255, 0),    // 绿色 - 连接正常
        RGB(255, 255, 0),  // 黄色 - 活动
        RGB(0, 191, 255),  // 蓝色 - 100Mbps
        RGB(255, 165, 0),  // 橙色 - 1Gbps
        RGB(255, 0, 0)     // 红色 - 错误
    };
    
    int ledX = x + jackWidth - 20;
    int ledY = y + jackHeight - 15;
    
    HBRUSH hLedBrush = CreateSolidBrush(statusColors[jackId]);
    SelectObject(hdc, hLedBrush);
    Ellipse(hdc, ledX, ledY, ledX + 8, ledY + 8);
    
    // 接头标签
    HFONT hLabelFont = CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hLabelFont);
    
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    
    char labels[5][20] = {"Switch Port 1", "Router", "PC 1", "PC 2", "Server"};
    TextOut(hdc, x + 5, y + jackHeight + 5, labels[jackId], strlen(labels[jackId]));
    
    // 恢复原始对象
    SelectObject(hdc, hOldFont);
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
    
    // 清理资源
    DeleteObject(hShellBrush);
    DeleteObject(hSlotBrush);
    DeleteObject(hLatchBrush);
    DeleteObject(hLatchButtonBrush);
    DeleteObject(hLedBrush);
    DeleteObject(hLightPen);
    DeleteObject(hDarkPen);
    DeleteObject(hLabelFont);
}

// 绘制网线连接
void DrawEthernetCables(HDC hdc, POINT* positions) {
    // 中心节点（交换机/路由器）
    POINT hubPosition = {400, 250};
    
    // 绘制中心设备
    DrawNetworkHub(hdc, hubPosition.x, hubPosition.y);
    
    // 绘制5条网线连接到中心
    for (int i = 0; i < 5; i++) {
        DrawEthernetCable(hdc, positions[i].x + 60, positions[i].y + 30, 
                         hubPosition.x, hubPosition.y, i);
    }
}

// Bug记录软件全局变量
static BOOL showBugReporter = FALSE;
static char bugTitle[100] = "";
static char bugDescription[1000] = "";
static int bugSeverity = 1; // 1:低, 2:中, 3:高, 4:严重
static int bugCategory = 0; // 0:界面, 1:功能, 2:性能, 3:崩溃, 4:其他
static BOOL bugSubmitted = FALSE;

// Bug记录软件函数声明
void ShowBugReporter(HWND hwnd);
void DrawBugReporter(HDC hdc, int centerX, int centerY);
void HandleBugReporterInput(char key);
void SubmitBugReport();
void SaveBugToFile();
void DrawBugCategorySelector(HDC hdc, int x, int y, int width, int height);
void DrawBugSeveritySelector(HDC hdc, int x, int y, int width, int height);

// 显示Bug记录软件
void ShowBugReporter(HWND hwnd) {
    CloseAllApps(hwnd);
    showBugReporter = TRUE;
    strcpy(bugTitle, "");
    strcpy(bugDescription, "");
    bugSeverity = 1;
    bugCategory = 0;
    bugSubmitted = FALSE;
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 修改后的DrawBugReporter函数
void DrawBugReporter(HDC hdc, int centerX, int centerY) {
    RECT bugRect = {centerX - 400, centerY - 300, centerX + 400, centerY + 300};
    
    // 绘制窗口背景
    HBRUSH hBugBgBrush = CreateSolidBrush(RGB(250, 250, 255));
    FillRect(hdc, &bugRect, hBugBgBrush);
    DeleteObject(hBugBgBrush);
    
    // 绘制窗口边框
    HPEN hBorderPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 200));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, bugRect.left, bugRect.top, bugRect.right, bugRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    // 绘制标题栏
    RECT titleRect = {bugRect.left, bugRect.top, bugRect.right, bugRect.top + 40};
    HBRUSH hTitleBrush = CreateSolidBrush(RGB(70, 130, 180));
    FillRect(hdc, &titleRect, hTitleBrush);
    DeleteObject(hTitleBrush);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hTitleFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    TextOut(hdc, bugRect.left + 10, bugRect.top + 8, "f系统 Bug 报告工具", 16);
    
    // 在标题栏右侧绘制守慧吉祥物
    DrawShouhuiMascot(hdc, bugRect.right - 50, bugRect.top - 5, 40);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    
    if (bugSubmitted) {
        // 显示提交成功界面
        HFONT hSuccessFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
        hOldFont = (HFONT)SelectObject(hdc, hSuccessFont);
        
        SetTextColor(hdc, RGB(0, 150, 0));
        TextOut(hdc, centerX - 80, centerY - 50, "提交成功！", 10);
        
        HFONT hMsgFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
        SelectObject(hdc, hMsgFont);
        
        SetTextColor(hdc, RGB(0, 0, 0));
        TextOut(hdc, centerX - 180, centerY, "感谢您的反馈！Bug报告已保存到 bug.dat 文件", 42);
        TextOut(hdc, centerX - 190, centerY + 30, "开发团队会尽快处理您的问题,请将文件发送给JammyJellyfish@hotmail.com", 67);
        
        // 关闭按钮
        RECT closeButtonRect = {centerX - 60, centerY + 100, centerX + 60, centerY + 140};
        HBRUSH hCloseBrush = CreateSolidBrush(RGB(100, 200, 100));
        FillRect(hdc, &closeButtonRect, hCloseBrush);
        DeleteObject(hCloseBrush);
        
        HPEN hClosePen = CreatePen(PS_SOLID, 1, RGB(50, 150, 50));
        hOldPen = (HPEN)SelectObject(hdc, hClosePen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, centerX - 60, centerY + 100, centerX + 60, centerY + 140);
        SelectObject(hdc, hOldPen);
        DeleteObject(hClosePen);
        
        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, centerX - 20, centerY + 115, "关闭", 4);
        
        SelectObject(hdc, hOldFont);
        DeleteObject(hSuccessFont);
        DeleteObject(hMsgFont);
        return;
    }
    
    // 绘制内容区域
    HFONT hLabelFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hInputFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    
    hOldFont = (HFONT)SelectObject(hdc, hLabelFont);
    SetTextColor(hdc, RGB(0, 0, 0));
    
    // Bug标题
    TextOut(hdc, bugRect.left + 20, bugRect.top + 60, "Bug 标题:", 9);
    
    RECT titleInputRect = {bugRect.left + 120, bugRect.top + 55, bugRect.right - 20, bugRect.top + 85};
    
    // 修复：绘制输入框背景
    HBRUSH hTitleInputBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &titleInputRect, hTitleInputBrush);
    DeleteObject(hTitleInputBrush);
    
    // 修复：根据焦点状态绘制边框
    static int currentField = 0;
    BOOL isTitleFocused = (currentField == 0);
    HPEN hInputPen = CreatePen(PS_SOLID, isTitleFocused ? 2 : 1, 
                             isTitleFocused ? RGB(0, 120, 215) : RGB(150, 150, 150));
    hOldPen = (HPEN)SelectObject(hdc, hInputPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, titleInputRect.left, titleInputRect.top, titleInputRect.right, titleInputRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hInputPen);
    
    // 修复：确保文字正确显示
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, RGB(255, 255, 255));
    SetTextColor(hdc, RGB(0, 0, 0));
    
    SelectObject(hdc, hInputFont);
    TextOut(hdc, titleInputRect.left + 5, titleInputRect.top + 5, bugTitle, strlen(bugTitle));
    
    // 修复：添加光标显示
    if (isTitleFocused) {
        static DWORD lastBlink = GetTickCount();
        static BOOL showCursor = TRUE;
        
        if (GetTickCount() - lastBlink > 500) {
            showCursor = !showCursor;
            lastBlink = GetTickCount();
        }
        
        if (showCursor) {
            int textWidth = GetTextWidth(hdc, bugTitle);
            HPEN hCursorPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            hOldPen = (HPEN)SelectObject(hdc, hCursorPen);
            MoveToEx(hdc, titleInputRect.left + 5 + textWidth, titleInputRect.top + 5, NULL);
            LineTo(hdc, titleInputRect.left + 5 + textWidth, titleInputRect.top + 25);
            SelectObject(hdc, hOldPen);
            DeleteObject(hCursorPen);
        }
    }
    
    // Bug分类
    SelectObject(hdc, hLabelFont);
    TextOut(hdc, bugRect.left + 20, bugRect.top + 100, "分类:", 5);
    DrawBugCategorySelector(hdc, bugRect.left + 120, bugRect.top + 95, 250, 30);
    
    // 严重程度
    TextOut(hdc, bugRect.left + 20, bugRect.top + 140, "严重程度:", 8);
    DrawBugSeveritySelector(hdc, bugRect.left + 120, bugRect.top + 135, 250, 30);
    
    // Bug描述
    SelectObject(hdc, hLabelFont);
    TextOut(hdc, bugRect.left + 20, bugRect.top + 180, "详细描述:", 8);
    
    RECT descInputRect = {bugRect.left + 20, bugRect.top + 205, bugRect.right - 20, bugRect.top + 350};
    
    // 修复：描述输入框背景
    HBRUSH hDescInputBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &descInputRect, hDescInputBrush);
    DeleteObject(hDescInputBrush);
    
    // 修复：描述输入框边框
    BOOL isDescFocused = (currentField == 1);
    HPEN hDescPen = CreatePen(PS_SOLID, isDescFocused ? 2 : 1, 
                            isDescFocused ? RGB(0, 120, 215) : RGB(150, 150, 150));
    hOldPen = (HPEN)SelectObject(hdc, hDescPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, descInputRect.left, descInputRect.top, descInputRect.right, descInputRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hDescPen);
    
    // 修复：描述文字显示
    SetBkMode(hdc, OPAQUE);
    SetBkColor(hdc, RGB(255, 255, 255));
    SetTextColor(hdc, RGB(0, 0, 0));
    
    SelectObject(hdc, hInputFont);
    
    // 多行文本显示
    RECT textRect = {descInputRect.left + 5, descInputRect.top + 5, descInputRect.right - 5, descInputRect.bottom - 5};
    DrawText(hdc, bugDescription, -1, &textRect, DT_LEFT | DT_TOP | DT_WORDBREAK);
    
    // 修复：描述框光标显示
    if (isDescFocused) {
        static DWORD lastDescBlink = GetTickCount();
        static BOOL showDescCursor = TRUE;
        
        if (GetTickCount() - lastDescBlink > 500) {
            showDescCursor = !showDescCursor;
            lastDescBlink = GetTickCount();
        }
        
        if (showDescCursor) {
            // 计算描述文本的高度
            RECT tempRect = textRect;
            DrawText(hdc, bugDescription, -1, &tempRect, DT_LEFT | DT_TOP | DT_WORDBREAK | DT_CALCRECT);
            int cursorY = tempRect.bottom;
            if (cursorY < textRect.top + 5) cursorY = textRect.top + 5;
            
            HPEN hDescCursorPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            hOldPen = (HPEN)SelectObject(hdc, hDescCursorPen);
            MoveToEx(hdc, textRect.left, cursorY, NULL);
            LineTo(hdc, textRect.left + 10, cursorY);
            SelectObject(hdc, hOldPen);
            DeleteObject(hDescCursorPen);
        }
    }
    
    // 绘制按钮
    int buttonWidth = 100;
    int buttonHeight = 35;
    int buttonY = bugRect.bottom - 50;
    
//    // 提交按钮
//    RECT submitButtonRect = {centerX - 120, buttonY, centerX - 120 + buttonWidth, buttonY + buttonHeight};
//    HBRUSH hSubmitBrush = CreateSolidBrush(RGB(0, 150, 0));
//    FillRect(hdc, &submitButtonRect, hSubmitBrush);
//    DeleteObject(hSubmitBrush);
//    
//    HPEN hSubmitPen = CreatePen(PS_SOLID, 1, RGB(0, 100, 0));
//    hOldPen = (HPEN)SelectObject(hdc, hSubmitPen);
//    SelectObject(hdc, GetStockObject(NULL_BRUSH));
//    Rectangle(hdc, centerX - 120, buttonY, centerX - 120 + buttonWidth, buttonY + buttonHeight);
//    SelectObject(hdc, hOldPen);
//    DeleteObject(hSubmitPen);
//    
//    SetTextColor(hdc, RGB(255, 255, 255));
//    TextOut(hdc, centerX - 105, buttonY + 10, "提交报告", 8);
    
//    // 取消按钮
//    RECT cancelButtonRect = {centerX + 20, buttonY, centerX + 20 + buttonWidth, buttonY + buttonHeight};
//    HBRUSH hCancelBrush = CreateSolidBrush(RGB(200, 0, 0));
//    FillRect(hdc, &cancelButtonRect, hCancelBrush);
//    DeleteObject(hCancelBrush);
//    
//    HPEN hCancelPen = CreatePen(PS_SOLID, 1, RGB(150, 0, 0));
//    hOldPen = (HPEN)SelectObject(hdc, hCancelPen);
//    SelectObject(hdc, GetStockObject(NULL_BRUSH));
//    Rectangle(hdc, centerX + 20, buttonY, centerX + 20 + buttonWidth, buttonY + buttonHeight);
//    SelectObject(hdc, hOldPen);
//    DeleteObject(hCancelPen);
//    
//    SetTextColor(hdc, RGB(255, 255, 255));
//    TextOut(hdc, centerX + 40, buttonY + 10, "取消", 4);
    
    // 绘制提示信息
    SelectObject(hdc, hInputFont);
    SetTextColor(hdc, RGB(100, 100, 100));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, bugRect.left + 20, bugRect.bottom - 90, "提示: Bug报告将保存到 bug.dat 文件中，用于帮助改进f系统，按enter键提交，按esc键取消", 83);
    
    // 修复：添加字段切换提示
    TextOut(hdc, bugRect.left + 20, bugRect.bottom - 70, "使用Tab键切换输入字段", 22);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hLabelFont);
    DeleteObject(hInputFont);
}

// 绘制Bug分类选择器
void DrawBugCategorySelector(HDC hdc, int x, int y, int width, int height) {
    const char* categories[] = {"界面问题", "功能异常", "性能问题", "系统崩溃", "其他问题"};
    int categoryWidth = width / 5;
    
    for (int i = 0; i < 5; i++) {
        RECT catRect = {x + i * categoryWidth, y, x + (i + 1) * categoryWidth, y + height};
        
        COLORREF bgColor = (bugCategory == i) ? RGB(100, 150, 255) : RGB(220, 220, 220);
        HBRUSH hCatBrush = CreateSolidBrush(bgColor);
        FillRect(hdc, &catRect, hCatBrush);
        DeleteObject(hCatBrush);
        
        HPEN hCatPen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hCatPen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, catRect.left, catRect.top, catRect.right, catRect.bottom);
        SelectObject(hdc, hOldPen);
        DeleteObject(hCatPen);
        
        SetTextColor(hdc, (bugCategory == i) ? RGB(255, 255, 255) : RGB(0, 0, 0));
        HFONT hCatFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hCatFont);
        
        int textWidth = GetTextWidth(hdc, categories[i]);
        TextOut(hdc, catRect.left + (categoryWidth - textWidth) / 2, catRect.top + 8, categories[i], strlen(categories[i]));
        
        SelectObject(hdc, hOldFont);
        DeleteObject(hCatFont);
    }
}

// 绘制Bug严重程度选择器
void DrawBugSeveritySelector(HDC hdc, int x, int y, int width, int height) {
    const char* severities[] = {"低", "中", "高", "严重"};
    const COLORREF severityColors[] = {
        RGB(100, 200, 100),   // 低 - 绿色
        RGB(255, 200, 100),   // 中 - 黄色
        RGB(255, 150, 50),    // 高 - 橙色
        RGB(255, 50, 50)      // 严重 - 红色
    };
    
    int severityWidth = width / 4;
    
    for (int i = 0; i < 4; i++) {
        RECT sevRect = {x + i * severityWidth, y, x + (i + 1) * severityWidth, y + height};
        
        COLORREF bgColor = (bugSeverity == i + 1) ? severityColors[i] : RGB(220, 220, 220);
        HBRUSH hSevBrush = CreateSolidBrush(bgColor);
        FillRect(hdc, &sevRect, hSevBrush);
        DeleteObject(hSevBrush);
        
        HPEN hSevPen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hSevPen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, sevRect.left, sevRect.top, sevRect.right, sevRect.bottom);
        SelectObject(hdc, hOldPen);
        DeleteObject(hSevPen);
        
        SetTextColor(hdc, (bugSeverity == i + 1) ? RGB(255, 255, 255) : RGB(0, 0, 0));
        HFONT hSevFont = CreateFont(12, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hSevFont);
        
        int textWidth = GetTextWidth(hdc, severities[i]);
        TextOut(hdc, sevRect.left + (severityWidth - textWidth) / 2, sevRect.top + 8, severities[i], strlen(severities[i]));
        
        SelectObject(hdc, hOldFont);
        DeleteObject(hSevFont);
    }
}

// 修改后的HandleBugReporterInput函数
void HandleBugReporterInput(char key) {
    static int currentField = 0; // 0:标题, 1:描述
    
    if (bugSubmitted) {
        if (key == 13 || key == 27) { // Enter或ESC
            showBugReporter = FALSE;
        }
        return;
    }
    
    switch (key) {
        case 9: // Tab键切换字段
            currentField = (currentField + 1) % 2;
            InvalidateRect(NULL, NULL, TRUE); // 刷新显示
            break;
            
        case 8: // Backspace
            if (currentField == 0 && strlen(bugTitle) > 0) {
                bugTitle[strlen(bugTitle) - 1] = '\0';
            }
            else if (currentField == 1 && strlen(bugDescription) > 0) {
                bugDescription[strlen(bugDescription) - 1] = '\0';
            }
            InvalidateRect(NULL, NULL, TRUE); // 刷新显示
            break;
            
        case 13: // Enter键提交
            SubmitBugReport();
            InvalidateRect(NULL, NULL, TRUE); // 刷新显示
            break;
            
        case 27: // ESC键关闭（不提交）
            showBugReporter = FALSE;
            InvalidateRect(NULL, NULL, TRUE);
            break;
            
        default:
            if (key >= 32 && key <= 126) { // 可打印字符
                if (currentField == 0 && strlen(bugTitle) < 99) {
                    int len = strlen(bugTitle);
                    bugTitle[len] = key;
                    bugTitle[len + 1] = '\0';
                }
                else if (currentField == 1 && strlen(bugDescription) < 999) {
                    int len = strlen(bugDescription);
                    bugDescription[len] = key;
                    bugDescription[len + 1] = '\0';
                }
                InvalidateRect(NULL, NULL, TRUE); // 刷新显示
            }
            break;
    }
}
// 提交Bug报告
void SubmitBugReport() {
    if (strlen(bugTitle) == 0) {
        MessageBox(NULL, "请输入Bug标题", "提示", MB_OK | MB_ICONWARNING);
        return;
    }
    
    if (strlen(bugDescription) == 0) {
        MessageBox(NULL, "请输入Bug详细描述", "提示", MB_OK | MB_ICONWARNING);
        return;
    }
    
    SaveBugToFile();
    bugSubmitted = TRUE;
}

// 保存Bug到文件
void SaveBugToFile() {
    FILE* file = fopen("bug.dat", "a");
    if (file) {
        // 获取当前时间
        time_t now;
        struct tm* timeinfo;
        char timeStr[50];
        
        time(&now);
        timeinfo = localtime(&now);
        strftime(timeStr, 50, "%Y-%m-%d %H:%M:%S", timeinfo);
        
        // 分类名称
        const char* categories[] = {"界面问题", "功能异常", "性能问题", "系统崩溃", "其他问题"};
        const char* severities[] = {"低", "中", "高", "严重"};
        
        // 写入Bug信息
        fprintf(file, "=== Bug 报告 ===\n");
        fprintf(file, "时间: %s\n", timeStr);
        fprintf(file, "用户: %s\n", username);
        fprintf(file, "计算机: %s\n", computerName);
        fprintf(file, "分类: %s\n", categories[bugCategory]);
        fprintf(file, "严重程度: %s\n", severities[bugSeverity - 1]);
        fprintf(file, "标题: %s\n", bugTitle);
        fprintf(file, "描述:\n%s\n", bugDescription);
        fprintf(file, "================\n\n");
        
        fclose(file);
    }
}
// 绘制网络中心设备
void DrawNetworkHub(HDC hdc, int x, int y) {
    HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
    
    // 中心设备外壳
    int hubWidth = 80;
    int hubHeight = 60;
    
    HBRUSH hHubBrush = CreateSolidBrush(RGB(64, 64, 64));
    SelectObject(hdc, hHubBrush);
    RoundRect(hdc, x - hubWidth/2, y - hubHeight/2, x + hubWidth/2, y + hubHeight/2, 10, 10);
    
    // 设备前面板
    HBRUSH hPanelBrush = CreateSolidBrush(RGB(40, 40, 40));
    SelectObject(hdc, hPanelBrush);
    Rectangle(hdc, x - hubWidth/2 + 5, y - hubHeight/2 + 5, x + hubWidth/2 - 5, y + hubHeight/2 - 5);
    
    // 端口指示灯
    for (int i = 0; i < 8; i++) {
        int ledX = x - hubWidth/2 + 15 + i * 7;
        int ledY = y - hubHeight/2 + 15;
        
        HBRUSH hPortLedBrush = CreateSolidBrush(i < 5 ? RGB(0, 255, 0) : RGB(100, 100, 100));
        SelectObject(hdc, hPortLedBrush);
        Ellipse(hdc, ledX, ledY, ledX + 4, ledY + 4);
        DeleteObject(hPortLedBrush);
    }
    
    // 设备标签
    HFONT hHubFont = CreateFont(9, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hHubFont);
    
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    TextOut(hdc, x - 25, y + 5, "SWITCH", 6);
    
    SelectObject(hdc, hOldFont);
    SelectObject(hdc, hOldPen);
    
    DeleteObject(hHubBrush);
    DeleteObject(hPanelBrush);
    DeleteObject(hHubFont);
}

// 绘制单条网线
void DrawEthernetCable(HDC hdc, int startX, int startY, int endX, int endY, int cableId) {
    // 网线颜色（不同线缆不同颜色）
    COLORREF cableColors[] = {
        RGB(255, 0, 0),     // 红色
        RGB(0, 255, 0),     // 绿色
        RGB(0, 0, 255),     // 蓝色
        RGB(255, 255, 0),   // 黄色
        RGB(255, 0, 255)    // 紫色
    };
    
    COLORREF cableColor = cableColors[cableId];
    
    // 绘制网线主体
    HPEN hCablePen = CreatePen(PS_SOLID, 3, cableColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hCablePen);
    
    // 计算控制点用于曲线
    int controlX1 = (startX + endX) / 2;
    int controlY1 = startY - 50;
    int controlX2 = (startX + endX) / 2;
    int controlY2 = endY + 50;
    
    // 使用贝塞尔曲线绘制自然的网线
    POINT bezierPoints[4] = {
        {startX, startY},
        {controlX1, controlY1},
        {controlX2, controlY2},
        {endX, endY}
    };
    
    PolyBezier(hdc, bezierPoints, 4);
    
    // 恢复原始画笔
    SelectObject(hdc, hOldPen);
    
    // 清理资源
    DeleteObject(hCablePen);
}

// 简易设置软件实现

// 显示设置软件
void ShowSettings(HWND hwnd) {
    CloseAllApps(hwnd);
    showSettings = TRUE;
    settingsCurrentStep = 0;
    settingsFocusField = 0; // 初始化焦点到第一个字段
    settingsError = FALSE;
    
    // 初始化设置值
    strcpy(newUsername, username);
    strcpy(newComputerName, computerName);
    strcpy(newPassword, "");
    strcpy(newConfirmPassword, "");
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 绘制设置软件
void DrawSettings(HDC hdc, int centerX, int centerY) {
    RECT settingsRect = {centerX - 350, centerY - 300, centerX + 350, centerY + 300};
    
    // 绘制窗口背景
    HBRUSH hSettingsBgBrush = CreateSolidBrush(SETTINGS_BG);
    FillRect(hdc, &settingsRect, hSettingsBgBrush);
    DeleteObject(hSettingsBgBrush);
    
    // 绘制窗口边框
    HPEN hBorderPen = CreatePen(PS_SOLID, 2, themes[currentThemeIndex].accentColor);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, settingsRect.left, settingsRect.top, settingsRect.right, settingsRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    // 绘制标题栏
    RECT titleRect = {settingsRect.left, settingsRect.top, settingsRect.right, settingsRect.top + 40};
    HBRUSH hTitleBrush = CreateSolidBrush(themes[currentThemeIndex].accentColor);
    FillRect(hdc, &titleRect, hTitleBrush);
    DeleteObject(hTitleBrush);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hTitleFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    TextOut(hdc, settingsRect.left + 10, settingsRect.top + 8, "f系统设置 - 专业版", 16);
    
    // 在标题栏右侧绘制守慧吉祥物
    DrawShouhuiMascot(hdc, settingsRect.right - 50, settingsRect.top - 5, 40);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    
    // 绘制内容区域
    DrawSettingsStep(hdc, centerX, centerY);
}

// 绘制设置步骤
void DrawSettingsStep(HDC hdc, int centerX, int centerY) {
    HFONT hNormalFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hSmallFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hBoldFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    
    HFONT hOldFont = (HFONT)SelectObject(hdc, hBoldFont);
    
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    
    // 绘制步骤指示器
    int totalSteps = 3;
    int stepWidth = 100;
    int totalWidth = totalSteps * stepWidth;
    int startX = centerX - totalWidth / 2;
    int stepY = centerY - 250;
    
    SelectObject(hdc, hSmallFont);
    
    const char* stepNames[] = {"用户设置", "系统设置", "主题设置"};
    
    for (int i = 0; i < totalSteps; i++) {
        int x = startX + i * stepWidth;
        
        // 绘制步骤圆圈
        BOOL isCurrent = i == settingsCurrentStep;
        
        COLORREF circleColor = isCurrent ? themes[currentThemeIndex].accentColor : RGB(180, 180, 180);
        HBRUSH hCircleBrush = CreateSolidBrush(circleColor);
        HPEN hCirclePen = CreatePen(PS_SOLID, 2, circleColor);
        
        HPEN hOldPen = (HPEN)SelectObject(hdc, hCirclePen);
        SelectObject(hdc, hCircleBrush);
        Ellipse(hdc, x - 12, stepY, x + 12, stepY + 24);
        
        SelectObject(hdc, hOldPen);
        DeleteObject(hCircleBrush);
        DeleteObject(hCirclePen);
        
        // 绘制步骤文本
        SetTextColor(hdc, isCurrent ? themes[currentThemeIndex].accentColor : RGB(120, 120, 120));
        int textWidth = GetTextWidth(hdc, stepNames[i]);
        TextOut(hdc, x - textWidth/2, stepY + 30, stepNames[i], strlen(stepNames[i]));
        
        // 绘制连接线（除了最后一步）
        if (i < totalSteps - 1) {
            HPEN hLinePen = CreatePen(PS_SOLID, 2, RGB(200, 200, 200));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hLinePen);
            MoveToEx(hdc, x + 12, stepY + 12, NULL);
            LineTo(hdc, x + stepWidth - 12, stepY + 12);
            SelectObject(hdc, hOldPen);
            DeleteObject(hLinePen);
        }
    }
    
    SetTextColor(hdc, RGB(0, 0, 0));
    SelectObject(hdc, hNormalFont);
    
    // 绘制当前步骤内容
    switch (settingsCurrentStep) {
        case 0: { // 用户设置
    		SelectObject(hdc, hBoldFont);
  		   	TextOut(hdc, centerX - 80, centerY - 180, "用户账户设置", 10);
    		SelectObject(hdc, hNormalFont);
    
    		// 根据焦点状态绘制输入框
    		DrawInputBox(hdc, centerX - 200, centerY - 120, 400, 35, "新用户名:", 
                newUsername, settingsFocusField == 0, FALSE, "输入新用户名");
    		DrawInputBox(hdc, centerX - 200, centerY - 50, 400, 35, "新密码:", 
                newPassword, settingsFocusField == 1, TRUE, "输入新密码（留空保持不变）");
    		DrawInputBox(hdc, centerX - 200, centerY + 20, 400, 35, "确认密码:", 
                newConfirmPassword, settingsFocusField == 2, TRUE, "确认新密码");
    
    		if (settingsError) {
        		SetTextColor(hdc, VISTA_RED);
        		TextOut(hdc, centerX - 180, centerY + 70, settingsErrorMsg, strlen(settingsErrorMsg));
        		SetTextColor(hdc, RGB(0, 0, 0));
    		}
    
    		SelectObject(hdc, hSmallFont);
    		break;
		}
            
        case 1: { // 系统设置
            SelectObject(hdc, hBoldFont);
            TextOut(hdc, centerX - 80, centerY - 180, "系统信息设置", 10);
            SelectObject(hdc, hNormalFont);
            
            DrawInputBox(hdc, centerX - 200, centerY - 120, 400, 35, "计算机名:", newComputerName, TRUE, FALSE, "输入计算机名称");
            
            // 显示当前系统信息
            TextOut(hdc, centerX - 200, centerY - 50, "当前用户:", 8);
            TextOut(hdc, centerX - 120, centerY - 50, username, strlen(username));
            
            TextOut(hdc, centerX - 200, centerY - 10, "当前计算机名:", 12);
            TextOut(hdc, centerX - 120, centerY - 10, computerName, strlen(computerName));
            
            TextOut(hdc, centerX - 200, centerY + 30, "系统版本:", 8);
            TextOut(hdc, centerX - 120, centerY + 30, "fOS 1.0 Professional codename nori", 34);
            
            if (settingsError) {
                SetTextColor(hdc, VISTA_RED);
                TextOut(hdc, centerX - 180, centerY + 80, settingsErrorMsg, strlen(settingsErrorMsg));
                SetTextColor(hdc, RGB(0, 0, 0));
            }
            
            break;
        }
            
        case 2: { // 主题设置
            SelectObject(hdc, hBoldFont);
            TextOut(hdc, centerX - 80, centerY - 180, "系统主题设置", 12);
            SelectObject(hdc, hNormalFont);
            
            TextOut(hdc, centerX - 200, centerY - 120, "可选主题:", 9);
            
            // 绘制主题选择器
            DrawThemeSelector(hdc, centerX - 200, centerY - 80, 400, 120);
            
            TextOut(hdc, centerX - 200, centerY + 60, "当前主题:", 9);
            TextOut(hdc, centerX - 120, centerY + 60, themes[currentThemeIndex].themeName, strlen(themes[currentThemeIndex].themeName));
            
            SelectObject(hdc, hSmallFont);
            TextOut(hdc, centerX - 200, centerY + 90, "请按TAB键切换所选主题", 21);
            break;
        }
    }
    
    // 绘制导航按钮
    int buttonWidth = 100;
    int buttonHeight = 35;
    int buttonY = centerY + 180;
    
    // 上一步按钮（不是第一步时显示）
    if (settingsCurrentStep > 0) {
        RECT prevButtonRect = {centerX - 220, buttonY, centerX - 220 + buttonWidth, buttonY + buttonHeight};
        HBRUSH hPrevBrush = CreateSolidBrush(RGB(180, 180, 180));
        FillRect(hdc, &prevButtonRect, hPrevBrush);
        DeleteObject(hPrevBrush);
        
        HPEN hPrevPen = CreatePen(PS_SOLID, 1, RGB(120, 120, 120));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPrevPen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, centerX - 220, buttonY, centerX - 220 + buttonWidth, buttonY + buttonHeight);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPrevPen);
        
        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, centerX - 210, buttonY + 10, "上一步", 6);
    }
    
    // 下一步/应用按钮
    RECT nextButtonRect = {centerX - 50, buttonY, centerX - 50 + buttonWidth, buttonY + buttonHeight};
    HBRUSH hNextBrush = CreateSolidBrush(themes[currentThemeIndex].accentColor);
    FillRect(hdc, &nextButtonRect, hNextBrush);
    DeleteObject(hNextBrush);
    
    HPEN hNextPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hNextPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX - 50, buttonY, centerX - 50 + buttonWidth, buttonY + buttonHeight);
    SelectObject(hdc, hOldPen);
    DeleteObject(hNextPen);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    TextOut(hdc, centerX - 30, buttonY + 10, settingsCurrentStep == 2 ? "应用" : "下一步", 
           settingsCurrentStep == 2 ? 4 : 6);
    
    // 取消按钮
    RECT cancelButtonRect = {centerX + 70, buttonY, centerX + 70 + buttonWidth, buttonY + buttonHeight};
    HBRUSH hCancelBrush = CreateSolidBrush(VISTA_RED);
    FillRect(hdc, &cancelButtonRect, hCancelBrush);
    DeleteObject(hCancelBrush);
    
    HPEN hCancelPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    hOldPen = (HPEN)SelectObject(hdc, hCancelPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX + 70, buttonY, centerX + 70 + buttonWidth, buttonY + buttonHeight);
    SelectObject(hdc, hOldPen);
    DeleteObject(hCancelPen);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    TextOut(hdc, centerX + 90, buttonY + 10, "取消", 4);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hNormalFont);
    DeleteObject(hSmallFont);
    DeleteObject(hBoldFont);
}

// 绘制主题选择器
void DrawThemeSelector(HDC hdc, int x, int y, int width, int height) {
    int themeWidth = width / 4;
    int themeHeight = height;
    
    for (int i = 0; i < 4; i++) {
        int themeX = x + i * themeWidth;
        
        // 绘制主题预览
        RECT themeRect = {themeX + 5, y + 5, themeX + themeWidth - 5, y + themeHeight - 25};
        
        // 绘制主题背景
        HBRUSH hThemeBgBrush = CreateSolidBrush(themes[i].desktopColor);
        FillRect(hdc, &themeRect, hThemeBgBrush);
        DeleteObject(hThemeBgBrush);
        
        // 绘制任务栏预览
        RECT taskbarRect = {themeX + 5, y + themeHeight - 40, themeX + themeWidth - 5, y + themeHeight - 25};
        HBRUSH hTaskbarBrush = CreateSolidBrush(themes[i].taskbarColor);
        FillRect(hdc, &taskbarRect, hTaskbarBrush);
        DeleteObject(hTaskbarBrush);
        
        // 绘制按钮预览
        RECT buttonRect = {themeX + 10, y + themeHeight - 35, themeX + 40, y + themeHeight - 30};
        HBRUSH hButtonBrush = CreateSolidBrush(themes[i].buttonColor);
        FillRect(hdc, &buttonRect, hButtonBrush);
        DeleteObject(hButtonBrush);
        
        // 绘制选中边框
        if (i == currentThemeIndex) {
            HPEN hSelectPen = CreatePen(PS_SOLID, 3, themes[i].accentColor);
            HPEN hOldPen = (HPEN)SelectObject(hdc, hSelectPen);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, themeX + 3, y + 3, themeX + themeWidth - 3, y + themeHeight - 20);
            SelectObject(hdc, hOldPen);
            DeleteObject(hSelectPen);
        } else {
            HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, themeX + 3, y + 3, themeX + themeWidth - 3, y + themeHeight - 20);
            SelectObject(hdc, hOldPen);
            DeleteObject(hBorderPen);
        }
        
        // 绘制主题名称
        SetTextColor(hdc, RGB(0, 0, 0));
        SetBkMode(hdc, TRANSPARENT);
        HFONT hSmallFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hSmallFont);
        
        int textWidth = GetTextWidth(hdc, themes[i].themeName);
        TextOut(hdc, themeX + (themeWidth - textWidth) / 2, y + themeHeight - 15, 
               themes[i].themeName, strlen(themes[i].themeName));
        
        SelectObject(hdc, hOldFont);
        DeleteObject(hSmallFont);
    }
}

// 处理设置软件输入 - 修复版本
void HandleSettingsInput(char key) {
    switch (settingsCurrentStep) {
        case 0: { // 用户设置
            if (key == 9) { // Tab键 - 在输入框之间切换焦点
                settingsFocusField = (settingsFocusField + 1) % 3;
            }
            else if (key == 8) { // Backspace
                switch (settingsFocusField) {
                    case 0: // 用户名
                        if (strlen(newUsername) > 0) {
                            newUsername[strlen(newUsername) - 1] = '\0';
                        }
                        break;
                    case 1: // 密码
                        if (strlen(newPassword) > 0) {
                            newPassword[strlen(newPassword) - 1] = '\0';
                        }
                        break;
                    case 2: // 确认密码
                        if (strlen(newConfirmPassword) > 0) {
                            newConfirmPassword[strlen(newConfirmPassword) - 1] = '\0';
                        }
                        break;
                }
            } 
            else if (key == 13) { // Enter键 - 验证并进入下一步
                if (ValidateSettingsInput()) {
                    settingsCurrentStep = 1;
                    settingsFocusField = 0; // 重置焦点到第一个字段
                }
            }
            else if (key >= 32 && key <= 126) { // 可打印字符
                switch (settingsFocusField) {
                    case 0: // 用户名
                        if (strlen(newUsername) < 49) {
                            int len = strlen(newUsername);
                            newUsername[len] = key;
                            newUsername[len + 1] = '\0';
                        }
                        break;
                    case 1: // 密码
                        if (strlen(newPassword) < 49) {
                            int len = strlen(newPassword);
                            newPassword[len] = key;
                            newPassword[len + 1] = '\0';
                        }
                        break;
                    case 2: // 确认密码
                        if (strlen(newConfirmPassword) < 49) {
                            int len = strlen(newConfirmPassword);
                            newConfirmPassword[len] = key;
                            newConfirmPassword[len + 1] = '\0';
                        }
                        break;
                }
            }
            break;
        }
            
        case 1: { // 系统设置
            if (key == 9) { // Tab键
                // 可以在这里添加其他字段的切换逻辑
            }
            else if (key == 8) { // Backspace
                if (strlen(newComputerName) > 0) {
                    newComputerName[strlen(newComputerName) - 1] = '\0';
                }
            } 
            else if (key == 13) { // Enter键 - 验证并进入下一步
                if (ValidateSettingsInput()) {
                    settingsCurrentStep = 2;
                }
            }
            else if (key >= 32 && key <= 126 && strlen(newComputerName) < 49) {
                int len = strlen(newComputerName);
                newComputerName[len] = key;
                newComputerName[len + 1] = '\0';
            }
            break;
        }
            
        case 2: { // 主题设置
            if (key == 9) { // Tab键 - 在主题之间切换
                currentThemeIndex = (currentThemeIndex + 1) % 4;
            }
            else if (key == 13) { // Enter键 - 完成设置
                ApplySettings();
                showSettings = FALSE;
            }
            break;
        }
    }
    
    settingsError = FALSE;
    
}
//------------废弃代码开始 -----------------
//// 绘制USB设备错误界面 - 优化版本（无连接文字，居中，高对比度）
//void DrawUSBErrorScreen(HDC hdc, int centerX, int centerY) {
//    RECT errorRect = {centerX - 400, centerY - 250, centerX + 400, centerY + 250};
//    
//    // 高对比度渐变背景 - 深蓝到黑色
//    for (int y = errorRect.top; y < errorRect.bottom; y++) {
//        double ratio = (double)(y - errorRect.top) / (errorRect.bottom - errorRect.top);
//        int r = (int)(30 * (1 - ratio));
//        int g = (int)(60 * (1 - ratio));
//        int b = (int)(120 * (1 - ratio));
//        
//        COLORREF color = RGB(r, g, b);
//        HPEN hPen = CreatePen(PS_SOLID, 1, color);
//        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
//        MoveToEx(hdc, errorRect.left, y, NULL);
//        LineTo(hdc, errorRect.right, y);
//        SelectObject(hdc, hOldPen);
//        DeleteObject(hPen);
//    }
//    
//    // 边框 - 亮蓝色，高对比度
//    HPEN hBorderPen = CreatePen(PS_SOLID, 3, RGB(0, 191, 255));
//    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
//    SelectObject(hdc, GetStockObject(NULL_BRUSH));
//    Rectangle(hdc, errorRect.left, errorRect.top, errorRect.right, errorRect.bottom);
//    SelectObject(hdc, hOldPen);
//    DeleteObject(hBorderPen);
//    
//    SetBkMode(hdc, TRANSPARENT);
//    
//    // 标题 - 亮白色，高对比度
//    HFONT hTitleFont = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
//        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
//        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
//    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
//    
//    SetTextColor(hdc, RGB(255, 255, 255));
//    
//    char title[100];
//    sprintf(title, "USB设备识别错误 - %c盘", usbDriveLetter);
//    int titleWidth = GetTextWidth(hdc, title);
//    TextOut(hdc, centerX - titleWidth/2, centerY - 220, title, strlen(title));
//    
//    // 错误信息 - 亮黄色，高对比度，居中显示
//    HFONT hMessageFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
//        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
//        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
//    SelectObject(hdc, hMessageFont);
//    
//    SetTextColor(hdc, RGB(255, 255, 0)); // 亮黄色
//    
//    // 分行居中显示错误信息
//    int messageY = centerY - 120;
//    int messageWidth = GetTextWidth(hdc, errorMessage);
//    TextOut(hdc, centerX - messageWidth/2, messageY, errorMessage, strlen(errorMessage));
//    
//    // 建议操作 - 亮绿色，高对比度，居中显示
//    SetTextColor(hdc, RGB(0, 255, 0)); // 亮绿色
//    
//    const char* suggestions[] = {
//        "检查USB设备是否正确连接",
//        "尝试重新插拔USB设备", 
//        "咨询设备厂商兼容性",
//        "在fOS官网查看兼容设备列表"
//    };
//    
//    int suggestionY = centerY - 60;
//    HFONT hSuggestionFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
//        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
//        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
//    SelectObject(hdc, hSuggestionFont);
//    
//    for (int i = 0; i < 4; i++) {
//        int textWidth = GetTextWidth(hdc, suggestions[i]);
//        TextOut(hdc, centerX - textWidth/2, suggestionY, suggestions[i], strlen(suggestions[i]));
//        suggestionY += 30;
//    }
//    
//    // 操作按钮 - 居中显示
//    int buttonWidth = 200;
//    int buttonHeight = 45;
//    int buttonY = centerY + 80;
//    
//    // 返回按钮 - 高对比度橙色
//    RECT buttonRect = {centerX - buttonWidth/2, buttonY, centerX + buttonWidth/2, buttonY + buttonHeight};
//    HBRUSH hButtonBrush = CreateSolidBrush(RGB(255, 140, 0)); // 亮橙色
//    FillRect(hdc, &buttonRect, hButtonBrush);
//    DeleteObject(hButtonBrush);
//    
//    HPEN hButtonPen = CreatePen(PS_SOLID, 2, RGB(255, 200, 0));
//    hOldPen = (HPEN)SelectObject(hdc, hButtonPen);
//    SelectObject(hdc, GetStockObject(NULL_BRUSH));
//    Rectangle(hdc, centerX - buttonWidth/2, buttonY, centerX + buttonWidth/2, buttonY + buttonHeight);
//    SelectObject(hdc, hOldPen);
//    DeleteObject(hButtonPen);
//    
//    SetTextColor(hdc, RGB(255, 255, 255));
//    HFONT hButtonFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
//        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
//        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
//    HFONT hOldButtonFont = (HFONT)SelectObject(hdc, hButtonFont);
//    
//    const char* buttonText = "按ESC键返回文件管理器";
//    int buttonTextWidth = GetTextWidth(hdc, buttonText);
//    TextOut(hdc, centerX - buttonTextWidth/2, buttonY + 14, buttonText, strlen(buttonText));
//    
//    // 恢复原始字体
//    SelectObject(hdc, hOldButtonFont);
//    SelectObject(hdc, hOldFont);
//    
//    // 清理资源
//    DeleteObject(hTitleFont);
//    DeleteObject(hMessageFont);
//    DeleteObject(hSuggestionFont);
//    DeleteObject(hButtonFont);
//}
//
//
//// 绘制USB错误按钮 - 粉色版本
//void DrawUSBErrorButton(HDC hdc, int x, int y, int width, int height, const char* text, BOOL isPrimary) {
//    RECT buttonRect = {x, y, x + width, y + height};
//    
//    // 粉色按钮
//    COLORREF buttonColor = isPrimary ? RGB(220, 120, 180) : RGB(200, 150, 190);
//    HBRUSH hButtonBrush = CreateSolidBrush(buttonColor);
//    FillRect(hdc, &buttonRect, hButtonBrush);
//    DeleteObject(hButtonBrush);
//    
//    HPEN hBorderPen = CreatePen(PS_SOLID, 2, RGB(180, 100, 160));
//    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
//    SelectObject(hdc, GetStockObject(NULL_BRUSH));
//    Rectangle(hdc, x, y, x + width, y + height);
//    SelectObject(hdc, hOldPen);
//    DeleteObject(hBorderPen);
//    
//    SetTextColor(hdc, RGB(255, 255, 255));
//    HFONT hButtonFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
//        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
//        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
//    HFONT hOldFont = (HFONT)SelectObject(hdc, hButtonFont);
//    
//    int textWidth = GetTextWidth(hdc, text);
//    TextOut(hdc, x + (width - textWidth) / 2, y + 14, text, strlen(text));
//    
//    SelectObject(hdc, hOldFont);
//    DeleteObject(hButtonFont);
//}
//
//// 处理USB错误界面输入 - 直接关机版本
//void HandleUSBErrorInput(char key) {
//    switch (key) {
//        case 13: // Enter键
//        case 27: // ESC键
//        case 'r': case 'R': // R键
//        case 'b': case 'B': // B键
//        case ' ': // 空格键
//            // 直接运行关机函数
//            ShutdownSystem(NULL);
//            break;
//    }
//}
//// 显示USB设备错误界面 - 修复版本
//void ShowUSBErrorScreen(HWND hwnd, char driveLetter, const char* message) {
//    showUSBError = TRUE;
//    usbDriveLetter = driveLetter;
//    strcpy(errorMessage, message);
//    
//    // 关闭其他界面，但记住是从文件管理器来的
//    showFileManager = FALSE;  // 暂时隐藏文件管理器
//    showDesktop = FALSE;
//    showCalculator = FALSE;
//    showSystemInfo = FALSE;
//    showSettings = FALSE;
//    // 注意：不要修改 showOOBE 的状态
//    
//    if (hwnd) {
//        InvalidateRect(hwnd, NULL, TRUE);
//        UpdateWindow(hwnd);
//    }
//}
//---------------废弃代码结束 -----------------

// 验证设置输入
BOOL ValidateSettingsInput() {
    settingsError = FALSE;
    
    switch (settingsCurrentStep) {
        case 0: // 用户设置验证
            if (strlen(newUsername) == 0) {
                settingsError = TRUE;
                strcpy(settingsErrorMsg, "用户名不能为空");
                return FALSE;
            }
            if (strlen(newUsername) < 3) {
                settingsError = TRUE;
                strcpy(settingsErrorMsg, "用户名至少需要3个字符");
                return FALSE;
            }
            if (strlen(newPassword) > 0) {
                if (strlen(newPassword) < 4) {
                    settingsError = TRUE;
                    strcpy(settingsErrorMsg, "密码至少需要4个字符");
                    return FALSE;
                }
                if (strcmp(newPassword, newConfirmPassword) != 0) {
                    settingsError = TRUE;
                    strcpy(settingsErrorMsg, "两次输入的密码不一致");
                    return FALSE;
                }
            }
            break;
            
        case 1: // 系统设置验证
            if (strlen(newComputerName) == 0) {
                settingsError = TRUE;
                strcpy(settingsErrorMsg, "计算机名不能为空");
                return FALSE;
            }
            if (strlen(newComputerName) < 2) {
                settingsError = TRUE;
                strcpy(settingsErrorMsg, "计算机名至少需要2个字符");
                return FALSE;
            }
            break;
    }
    
    return TRUE;
}

// 应用设置
void ApplySettings() {
    // 应用用户设置
    if (strlen(newUsername) > 0) {
        strcpy(username, newUsername);
    }
    
    if (strlen(newPassword) > 0) {
        strcpy(password, newPassword);
    }
    
    // 应用系统设置
    if (strlen(newComputerName) > 0) {
        strcpy(computerName, newComputerName);
    }
    
    // 保存配置
    SaveConfig();
    
    // 显示成功消息
    MessageBox(NULL, "设置已成功应用！\n部分更改需要重启系统才能生效。", "设置成功", MB_OK | MB_ICONINFORMATION);
}

// 格式化文件大小显示
void FormatFileSize(char* buffer, DWORD fileSize) {
    if (fileSize < 1024) {
        sprintf(buffer, "%u B", fileSize);
    } else if (fileSize < 1024 * 1024) {
        sprintf(buffer, "%.1f KB", fileSize / 1024.0);
    } else {
        sprintf(buffer, "%.1f MB", fileSize / (1024.0 * 1024.0));
    }
}

// 刷新文件列表 - 真实文件系统操作
void RefreshFileList() {
    fileCount = 0;
    
    // 获取当前目录
    if (strlen(currentPath) == 0) {
        GetCurrentDirectory(MAX_PATH, currentPath);
    }
    
    WIN32_FIND_DATA findFileData;
    HANDLE hFind;
    char searchPath[MAX_PATH];
    
    sprintf(searchPath, "%s\\*", currentPath);
    
    hFind = FindFirstFile(searchPath, &findFileData);
    
    if (hFind == INVALID_HANDLE_VALUE) {
        sprintf(commandOutput, "错误：无法访问目录 %s", currentPath);
        return;
    }
    
    do {
        // 跳过 "." 和 ".."
        if (strcmp(findFileData.cFileName, ".") == 0 || strcmp(findFileData.cFileName, "..") == 0) {
            continue;
        }
        
        // 检查是否显示隐藏文件
        if (!showHiddenFiles && (findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) {
            continue;
        }
        
        if (fileCount < 100) {
            strcpy(fileInfoList[fileCount].name, findFileData.cFileName);
            
            if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                strcpy(fileInfoList[fileCount].type, "DIR");
                strcpy(fileInfoList[fileCount].size, "<DIR>");
            } else {
                strcpy(fileInfoList[fileCount].type, "FILE");
                FormatFileSize(fileInfoList[fileCount].size, findFileData.nFileSizeLow);
            }
            
            // 格式化日期
            FILETIME ft = findFileData.ftLastWriteTime;
            SYSTEMTIME st;
            FileTimeToSystemTime(&ft, &st);
            sprintf(fileInfoList[fileCount].date, "%04d-%02d-%02d", st.wYear, st.wMonth, st.wDay);
            
            fileInfoList[fileCount].isHidden = (findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) != 0;
            
            fileCount++;
        }
    } while (FindNextFile(hFind, &findFileData) != 0 && fileCount < 100);
    
    FindClose(hFind);
    
    // 按类型和名称排序
    for (int i = 0; i < fileCount - 1; i++) {
        for (int j = i + 1; j < fileCount; j++) {
            BOOL swap = FALSE;
            
            // 目录优先
            if (strcmp(fileInfoList[i].type, "FILE") == 0 && strcmp(fileInfoList[j].type, "DIR") == 0) {
                swap = TRUE;
            }
            // 同类型按名称排序
            else if (strcmp(fileInfoList[i].type, fileInfoList[j].type) == 0) {
                if (stricmp(fileInfoList[i].name, fileInfoList[j].name) > 0) {
                    swap = TRUE;
                }
            }
            
            if (swap) {
                FileInfo temp = fileInfoList[i];
                fileInfoList[i] = fileInfoList[j];
                fileInfoList[j] = temp;
            }
        }
    }
}

// 创建文件
void CreateFileReal(const char* filename) {
    char fullPath[MAX_PATH];
    sprintf(fullPath, "%s\\%s", currentPath, filename);
    
    HANDLE hFile = CreateFile(fullPath, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile == INVALID_HANDLE_VALUE) {
        sprintf(commandOutput, "错误：无法创建文件 %s", filename);
    } else {
        CloseHandle(hFile);
        sprintf(commandOutput, "文件已创建： %s", filename);
        RefreshFileList();
    }
}

// 删除文件
void DeleteFileReal(const char* filename) {
    char fullPath[MAX_PATH];
    sprintf(fullPath, "%s\\%s", currentPath, filename);
    
    if (DeleteFile(fullPath)) {
        sprintf(commandOutput, "文件已删除： %s", filename);
        RefreshFileList();
    } else {
        sprintf(commandOutput, "错误：无法删除文件 %s", filename);
    }
}

// 创建目录
void CreateDirectoryReal(const char* dirname) {
    char fullPath[MAX_PATH];
    sprintf(fullPath, "%s\\%s", currentPath, dirname);
    
    if (CreateDirectory(fullPath, NULL)) {
        sprintf(commandOutput, "目录已创建： %s", dirname);
        RefreshFileList();
    } else {
        sprintf(commandOutput, "错误：无法创建目录 %s", dirname);
    }
}

// 复制文件
void CopyFileReal(const char* source, const char* target) {
    char sourcePath[MAX_PATH];
    char targetPath[MAX_PATH];
    sprintf(sourcePath, "%s\\%s", currentPath, source);
    sprintf(targetPath, "%s\\%s", currentPath, target);
    
    if (CopyFile(sourcePath, targetPath, FALSE)) {
        sprintf(commandOutput, "文件已复制： %s -> %s", source, target);
        RefreshFileList();
    } else {
        sprintf(commandOutput, "错误：无法将文件 %s  复制到 %s", source, target);
    }
}

// 重命名文件
void RenameFileReal(const char* oldname, const char* newname) {
    char oldPath[MAX_PATH];
    char newPath[MAX_PATH];
    sprintf(oldPath, "%s\\%s", currentPath, oldname);
    sprintf(newPath, "%s\\%s", currentPath, newname);
    
    if (MoveFile(oldPath, newPath)) {
        sprintf(commandOutput, "文件已重命名： %s -> %s", oldname, newname);
        RefreshFileList();
    } else {
        sprintf(commandOutput, "错误：无法将文件 %s 重命名为 %s", oldname, newname);
    }
}

// 修改文件管理器命令处理函数，添加水母求饶命令
void HandleFileManagerCommand(const char* command) {
    char cmd[100];
    char param[100];
    strcpy(cmd, command);
    
    // 分离命令和参数
    char* spacePos = strchr(cmd, ' ');
    if (spacePos) {
        *spacePos = '\0';
        strcpy(param, spacePos + 1);
    } else {
        strcpy(param, "");
    }
    
    // 转换为小写以便比较
    for (int i = 0; cmd[i]; i++) {
        cmd[i] = tolower(cmd[i]);
    }
    
    // 检查是否是"castrate jellyfish"命令（新增）
    if (strcmp(cmd, "kill") == 0 && strcmp(param, "jellyfish") == 0 || strcmp(cmd, "jellyfish") == 0 && strcmp(param, "die") == 0) {
        showFOSEgg = TRUE;  // 复用现有的fos彩蛋显示机制
        showFileManager = FALSE;
        fosEggState = 5;    // 使用新的状态5表示水母求饶界面
        fosEggProgress = 0;
        currentDevStage = 0;
        dialogueIndexx = 0;
        lastFOSEggTime = GetTickCount();
        return;
    }
    
    if (strcmp(cmd, "list") == 0) {
        ListFiles();
    }
    else if (strcmp(cmd, "to") == 0 ) {
        if (strlen(param) > 0) {
            ChangeDirectory(param);
        } else {
            strcpy(commandOutput, "用法：to <路径> \n示例：to C:\\Windows");
        }
    }
    else if (strcmp(cmd, "new") == 0 ) {
        if (strlen(param) > 0) {
            CreateDirectoryReal(param);
        } else {
            strcpy(commandOutput, "用法：new <目录名称>");
        }
    }
    else if (strcmp(cmd, "read") == 0) {
        if (strlen(param) > 0) {
            OpenFile(param);
        } else {
            strcpy(commandOutput, "用法：read <文件名>");
        }
    }
    else if (strcmp(cmd, "rem") == 0 ) {
        if (strlen(param) > 0) {
            DeleteFileReal(param);
        } else {
            strcpy(commandOutput, "用法：rem <文件名>");
        }
    }
    else if (strcmp(cmd, "cop") == 0) {
        char source[100], target[100];
        if (sscanf(param, "%s %s", source, target) == 2) {
            CopyFileReal(source, target);
        } else {
            strcpy(commandOutput, "用法：cop <源文件> <目标路径>");
        }
    }
    else if (strcmp(cmd, "name") == 0 ) {
        char oldname[100], newname[100];
        if (sscanf(param, "%s %s", oldname, newname) == 2) {
            RenameFileReal(oldname, newname);
        } else {
            strcpy(commandOutput, "用法：name <原文件名> <新文件名>");
        }
    }
    else if (strcmp(cmd, "newf") == 0) {
        if (strlen(param) > 0) {
            CreateFileReal(param);
        } else {
            strcpy(commandOutput, "用法：newf <文件名>");
        }
    }
    else if (strcmp(cmd, "open") == 0) {
        if (strlen(param) > 0) {
            // 打开文件编辑器
            ShowFileEditor(NULL, param);
        } else {
            strcpy(commandOutput, "用法：open <文件名>");
        }
    }
    else if (strcmp(cmd, "hidden") == 0) {
        showHiddenFiles = !showHiddenFiles;
        RefreshFileList();
        sprintf(commandOutput, "隐藏文件现已 %s", showHiddenFiles ? "可见" : "隐藏");
    }
    else if (strcmp(cmd, "where") == 0) {
        sprintf(commandOutput, "当前目录： %s", currentPath);
    }
    else if (strcmp(cmd, "help") == 0) {
        ShowHelp();
    }
    else if (strcmp(cmd, "clear") == 0) {
        strcpy(commandOutput, "屏幕已清空");
    }
    else if (strcmp(cmd, "bye") == 0 ) {
        showFileManager = FALSE;
        strcpy(commandOutput, "文件管理器已关闭");
    }
    else if (strcmp(cmd, "about") == 0) {
        strcpy(commandOutput, "fOS 文件管理器 v1.0 专业版\n版本 200 (专业版)");
    }
    else if (strcmp(cmd, "now") == 0) {
        char dateStr[50], timeStr[50];
        GetCurrentDateTime(dateStr, timeStr);
        sprintf(commandOutput, "当前日期：%s\n当前时间：%s", dateStr, timeStr);
    }
    else if (strcmp(cmd, "bug") == 0) {
        ShowBugReporter(NULL);
    }
    else if (strcmp(cmd, "buglist") == 0) {
        ShowBugList(NULL);
    }
    else if (strcmp(cmd, "error") == 0) {
        TriggerCriticalPurpleScreen(NULL);
        strcpy(commandOutput, "触发系统致命错误...");
    }
    else if (strlen(cmd) > 0) {
        // 检查是否是打开文件命令
        if (strstr(cmd, ".txt") || strstr(cmd, ".c") || strstr(cmd, ".cpp") || 
            strstr(cmd, ".h") || strstr(cmd, ".exe") || strstr(cmd, ".bat") ||
			strstr(cmd, ".log") || strstr(cmd, ".docx") || strstr(cmd, ".doc")) {
            OpenFile(cmd);
        } else {
            sprintf(commandOutput, "未知命令：%s\n输入 'help' 查看可用命令", cmd);
        }
    }
}

// 执行文件管理器命令
void ExecuteFileManagerCommand() {
    if (strlen(commandInput) > 0) {
        HandleFileManagerCommand(commandInput);
        strcpy(commandInput, "");
    }
}

// 改进的列出文件函数
void ListFiles() {
    RefreshFileList();
    sprintf(commandOutput, "%s 的目录\n\n", currentPath);
    
    char temp[500];
    int dirCount = 0, fileCountTotal = 0;
    long long totalSize = 0;
    
    // 添加列标题
    strcat(commandOutput, "类型     大小       修改日期        名称\n");
    strcat(commandOutput, "-------- ---------- --------------- --------------------\n");
    
    for (int i = 0; i < fileCount; i++) {
        if (strcmp(fileInfoList[i].type, "DIR") == 0) {
            dirCount++;
            sprintf(temp, "DIR      %-10s %-15s [%s]\n", 
                   fileInfoList[i].size, fileInfoList[i].date, fileInfoList[i].name);
        } else {
            fileCountTotal++;
            // 计算文件大小用于统计
            if (strstr(fileInfoList[i].size, "KB")) {
                totalSize += (long long)(atof(fileInfoList[i].size) * 1024);
            } else if (strstr(fileInfoList[i].size, "MB")) {
                totalSize += (long long)(atof(fileInfoList[i].size) * 1024 * 1024);
            } else {
                totalSize += (long long)atof(fileInfoList[i].size);
            }
            
            sprintf(temp, "FILE     %-10s %-15s %s\n", 
                   fileInfoList[i].size, fileInfoList[i].date, fileInfoList[i].name);
        }
        strcat(commandOutput, temp);
    }
    
    sprintf(temp, "\n%12d File(s) %13lld bytes\n%12d Dir(s)", 
           fileCountTotal, totalSize, dirCount);
    strcat(commandOutput, temp);
}

// 改进的改变目录函数
void ChangeDirectory(const char* path) {
    if (strlen(path) == 0) {
        strcpy(commandOutput, "用法：to <路径> \n示例：to C:\\Windows");
        return;
    }
    
    char newPath[MAX_PATH];
    strcpy(newPath, currentPath); // 备份当前路径
    
    // 处理特殊路径
    if (strcmp(path, ".") == 0 || strcmp(path, "./") == 0) {
        // 当前目录，不做任何改变
        sprintf(commandOutput, "已切换到当前目录：%s", currentPath);
        return;
    }
    else if (strcmp(path, "..") == 0 || strcmp(path, "../") == 0) {
        // 返回到上级目录
        int len = strlen(currentPath);
        if (len > 3) { // 不是根目录
            // 移除末尾的反斜杠（如果有）
            if (currentPath[len-1] == '\\') {
                currentPath[len-1] = '\0';
                len--;
            }
            
            // 找到上一个反斜杠
            char* lastSlash = strrchr(currentPath, '\\');
            if (lastSlash) {
                // 如果是根目录（如 C:\），保留反斜杠
                if (lastSlash == currentPath + 2) { // 如 C:\Windows 中的位置
                    lastSlash[1] = '\0';
                } else {
                    *lastSlash = '\0';
                }
            }
        }
    }
    else if (strcmp(path, "/") == 0 || strcmp(path, "\\") == 0) {
        // 回到根目录
        if (strlen(currentPath) >= 3) {
            currentPath[3] = '\0'; // 保留盘符和冒号、反斜杠（如 C:\）
        }
    }
    else if (path[1] == ':') {
        // 绝对路径
        strcpy(currentPath, path);
    }
    else {
        // 相对路径
        // 确保当前路径以反斜杠结尾
        if (currentPath[strlen(currentPath)-1] != '\\') {
            strcat(currentPath, "\\");
        }
        strcat(currentPath, path);
    }
    
    // 确保路径以反斜杠结尾（根目录除外）
    int len = strlen(currentPath);
    if (len > 3 && currentPath[len-1] != '\\') {
        strcat(currentPath, "\\");
    }
    
    // 验证路径是否存在
    DWORD attr = GetFileAttributes(currentPath);
    if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        sprintf(commandOutput, "错误：找不到目录：%s", currentPath);
        // 恢复原路径
        strcpy(currentPath, newPath);
    } else {
        RefreshFileList();
        sprintf(commandOutput, "已切换到目录：%s", currentPath);
    }
}

// 改进的打开文件函数
void OpenFile(const char* filename) {
    char fullPath[MAX_PATH];
    sprintf(fullPath, "%s\\%s", currentPath, filename);
    
    DWORD attr = GetFileAttributes(fullPath);
    if (attr == INVALID_FILE_ATTRIBUTES) {
        sprintf(commandOutput, "错误：找不到文件：%s", filename);
        return;
    }
    
    if (attr & FILE_ATTRIBUTE_DIRECTORY) {
        sprintf(commandOutput, "错误：%s 是目录，请使用 'to %s'", filename, filename);
        return;
    }
    
    sprintf(commandOutput, "打开文件： %s\n\n", filename);
    
    const char* ext = strrchr(filename, '.');
    
    if (ext) {
        // 首先检查是不是可执行文件
        char lowerExt[20];
        strcpy(lowerExt, ext);
        for (int i = 0; lowerExt[i]; i++) {
            lowerExt[i] = tolower(lowerExt[i]);
        }
        
        // 可执行文件 - 直接运行
        if (strcmp(lowerExt, ".exe") == 0 || 
            strcmp(lowerExt, ".bat") == 0 || 
            strcmp(lowerExt, ".cmd") == 0 || 
            strcmp(lowerExt, ".com") == 0 || 
            strcmp(lowerExt, ".msi") == 0 ||
            strcmp(lowerExt, ".scr") == 0) {
            
            // 完全静默运行程序
            ShellExecute(NULL, "open", fullPath, NULL, currentPath, SW_SHOWNORMAL);
            return;
        }
        
        // 文本文件 - 打开编辑器
        else if (stricmp(ext, ".txt") == 0 || 
                 stricmp(ext, ".c") == 0 || 
                 stricmp(ext, ".cpp") == 0 || 
                 stricmp(ext, ".h") == 0 ||
                 stricmp(ext, ".md") == 0 || 
                 stricmp(ext, ".ini") == 0 ||
                 stricmp(ext, ".conf") == 0 ||
                 stricmp(ext, ".js") == 0 ||
                 stricmp(ext, ".html") == 0 ||
                 stricmp(ext, ".css") == 0) {
            ShowFileEditor(NULL, filename);
            return;
        }
        
        // 其他二进制文件，尝试用默认程序打开
        else if (stricmp(ext, ".jpg") == 0 || 
                 stricmp(ext, ".png") == 0 || 
                 stricmp(ext, ".bmp") == 0 ||
                 stricmp(ext, ".gif") == 0 ||
                 stricmp(ext, ".mp3") == 0 || 
                 stricmp(ext, ".wav") == 0 ||
                 stricmp(ext, ".mp4") == 0 || 
                 stricmp(ext, ".avi") == 0 ||
                 stricmp(ext, ".pdf") == 0 || 
                 stricmp(ext, ".doc") == 0 ||
                 stricmp(ext, ".docx") == 0 || 
                 stricmp(ext, ".xls") == 0 ||
                 stricmp(ext, ".xlsx") == 0 || 
                 stricmp(ext, ".ppt") == 0 ||
                 stricmp(ext, ".pptx") == 0) {
            
            // 用Windows默认程序打开
            ShellExecute(NULL, "open", fullPath, NULL, currentPath, SW_SHOWNORMAL);
            commandOutput[0] = '\0';
            return;
        }
        
        // 其他情况，尝试读取
        else {
            strcat(commandOutput, "二进制文件或未知格式\n");
            
            HANDLE hFile = CreateFile(fullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile != INVALID_HANDLE_VALUE) {
                DWORD fileSize = GetFileSize(hFile, NULL);
                if (fileSize > 0 && fileSize < 5000) {
                    char* buffer = (char*)malloc(fileSize + 1);
                    DWORD bytesRead;
                    if (ReadFile(hFile, buffer, fileSize, &bytesRead, NULL)) {
                        buffer[bytesRead] = '\0';
                        // 检查是否为文本文件
                        BOOL isText = TRUE;
                        for (DWORD i = 0; i < bytesRead; i++) {
                            if (buffer[i] == 0) {
                                isText = FALSE;
                                break;
                            }
                        }
                        if (isText) {
                            strcat(commandOutput, buffer);
                        } else {
                            strcat(commandOutput, "文件可能为二进制格式，内容未显示。");
                        }
                    } else {
                        strcat(commandOutput, "错误：读取文件内容失败");
                    }
                    free(buffer);
                } else {
                    strcat(commandOutput, "文件过大或为空，无法显示");
                }
                CloseHandle(hFile);
            } else {
                strcat(commandOutput, "错误：无法打开文件进行读取");
            }
        }
    } else {
        // 无扩展名文件，先尝试运行，不行再读取
        // 先尝试作为可执行文件运行
        STARTUPINFO si = {0};
        PROCESS_INFORMATION pi = {0};
        si.cb = sizeof(si);
        
        char cmdLine[MAX_PATH + 10];
        sprintf(cmdLine, "\"%s\"", fullPath);
        
        if (CreateProcess(NULL, cmdLine, NULL, NULL, FALSE, 0, NULL, currentPath, &si, &pi)) {
            // 运行成功
            CloseHandle(pi.hProcess);
            CloseHandle(pi.hThread);
            commandOutput[0] = '\0';
        } else {
            // 不是可执行文件，尝试读取
            strcat(commandOutput, "文件无扩展名。正在尝试以文本格式读取...\n\n");
            
            HANDLE hFile = CreateFile(fullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile != INVALID_HANDLE_VALUE) {
                DWORD fileSize = GetFileSize(hFile, NULL);
                if (fileSize > 0 && fileSize < 5000) {
                    char* buffer = (char*)malloc(fileSize + 1);
                    DWORD bytesRead;
                    if (ReadFile(hFile, buffer, fileSize, &bytesRead, NULL)) {
                        buffer[bytesRead] = '\0';
                        // 检查是否为文本文件
                        BOOL isText = TRUE;
                        for (DWORD i = 0; i < bytesRead; i++) {
                            if (buffer[i] == 0) {
                                isText = FALSE;
                                break;
                            }
                        }
                        if (isText) {
                            strcat(commandOutput, buffer);
                        } else {
                            strcat(commandOutput, "文件可能为二进制格式，内容未显示。");
                        }
                    } else {
                        strcat(commandOutput, "错误：读取文件内容失败");
                    }
                    free(buffer);
                } else {
                    strcat(commandOutput, "文件过大或为空，无法显示");
                }
                CloseHandle(hFile);
            } else {
                strcat(commandOutput, "错误：无法打开文件进行读取");
            }
        }
    }
}

// 改进的帮助函数
void ShowHelp() {
	strcpy(commandOutput, "fOS 文件管理器 - 命令帮助\n\n");
	strcat(commandOutput, "文件操作：\n");
	strcat(commandOutput, " list - 列出目录内容\n");
	strcat(commandOutput, " to <路径> - 切换目录\n");
	strcat(commandOutput, " read <文件> - 浏览文件\n");
	strcat(commandOutput, " open <文件> - 编辑文件\n");
	strcat(commandOutput, " rem <文件> - 删除文件\n");
	strcat(commandOutput, " cop <源> <目标> - 复制文件\n");
	strcat(commandOutput, " name <原> <新> - 重命名文件\n");
	strcat(commandOutput, " new <名称> - 创建目录\n");
	strcat(commandOutput, " newf <文件> - 创建空文件\n\n");
    
    strcat(commandOutput, "系统命令：\n");
	strcat(commandOutput, " where - 显示当前目录\n");
	strcat(commandOutput, " hidden - 切换隐藏文件显示\n");
	strcat(commandOutput, " clear - 清空屏幕\n");
	strcat(commandOutput, " about - 显示版本\n");
	strcat(commandOutput, " now - 显示日期和时间\n");
	strcat(commandOutput, " help - 显示此帮助\n");
	strcat(commandOutput, " bye - 退出文件管理器\n\n");
    
    strcat(commandOutput, "使用示例：\n");
	strcat(commandOutput, " list - 列出文件\n");
	strcat(commandOutput, " to C:\\Windows - 切换到 Windows 目录\n");
	strcat(commandOutput, " read readme.txt - 查看说明文件\n");
	strcat(commandOutput, " open notes.txt - 编辑文本文件\n");
	strcat(commandOutput, " new NewFolder - 创建新目录\n");
	strcat(commandOutput, " newf test.txt - 创建空文件\n");
}

// 处理文件管理器输入
void HandleFileManagerInput(char key) {
    if (key == 8) { // Backspace
        if (strlen(commandInput) > 0) {
            commandInput[strlen(commandInput) - 1] = '\0';
        }
    } else if (key == 13) { // Enter
        ExecuteFileManagerCommand();
    } else if (key >= 32 && key <= 126 && strlen(commandInput) < 99) {
        int len = strlen(commandInput);
        commandInput[len] = key;
        commandInput[len + 1] = '\0';
        
        // 检测fos彩蛋输入
        if (len >= 2) {
            char lastThree[4];
            strncpy(lastThree, &commandInput[len-2], 3);
            lastThree[3] = '\0';
            
            if (strcmp(lastThree, "fos") == 0) {
                fosInputCount++;
                
                // 显示输入计数提示
                char countMsg[50];
                
                // 达到5次触发彩蛋
                if (fosInputCount >= 5) {
                    showFOSEgg = TRUE;
                    showFileManager = FALSE;
                    fosInputCount = 0;
                    fosEggState = 1;
                    fosEggProgress = 0;
                    currentDevStage = 0;
                    dialogueIndexx = 0;
                    lastFOSEggTime = GetTickCount();
                }
            }
        }
    }
}

// 绘制fos开发历程彩蛋（修改版，添加水母求饶界面）
void DrawFOSEgg(HDC hdc, HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    
    // 渐变背景 - 从深蓝到黑色
    for (int y = rect.top; y < rect.bottom; y++) {
        double ratio = (double)(y - rect.top) / (rect.bottom - rect.top);
        int r = (int)(10 * (1 - ratio));
        int g = (int)(30 * (1 - ratio));
        int b = (int)(80 * (1 - ratio));
        COLORREF color = RGB(r, g, b);
        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, rect.left, y, NULL);
        LineTo(hdc, rect.right, y);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    int centerX = rect.right / 2;
    int centerY = rect.bottom / 2;
    
    switch (fosEggState) {
        case 1: { // 旁白对话
            DrawDialogueScene(hdc, centerX, centerY);
            break;
        }
        case 2: { // 开发历程回顾
            DrawDevTimeline(hdc, centerX, centerY);
            break;
        }
        case 3: { // 功能展示
            DrawFeatureShowcase(hdc, centerX, centerY);
            break;
        }
        case 4: { // 致谢名单
            DrawCredits(hdc, centerX, centerY);
            break;
        }
        case 5: { // 水母求饶界面（新增）- 明确是公水母代表开发团队
            DrawJellyfishPleaScene(hdc, centerX, centerY);
            break;
        }
    }
    
    // 绘制翻页提示（除了致谢名单和水母求饶界面）
    if (fosEggState != 4 && fosEggState != 5) {
        HFONT hHintFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "宋体");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hHintFont);
        TextOut(hdc, centerX - 80, rect.bottom - 40, "按Enter键继续", 12);
        SelectObject(hdc, hOldFont);
        DeleteObject(hHintFont);
    }
    
    // 绘制跳过提示（所有彩蛋界面都有）
    HFONT hHintFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "宋体");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hHintFont);
    TextOut(hdc, centerX - 60, rect.bottom - 20, "按ESC键跳过", 10);
    SelectObject(hdc, hOldFont);
    DeleteObject(hHintFont);
}

// 绘制旁白对话场景
void DrawDialogueScene(HDC hdc, int centerX, int centerY) {
    // 绘制对话背景
    RECT dialogueBg = {centerX - 350, centerY - 100, centerX + 350, centerY + 100};
    HBRUSH hDialogueBrush = CreateSolidBrush(RGB(30, 30, 60));
    FillRect(hdc, &dialogueBg, hDialogueBrush);
    DeleteObject(hDialogueBrush);
    
    HPEN hBorderPen = CreatePen(PS_SOLID, 3, RGB(100, 100, 200));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX - 350, centerY - 100, centerX + 350, centerY + 100);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    // 在左上角绘制两个守慧吉祥物作为对话角色
    DrawShouhuiMascot(hdc, centerX - 320, centerY - 80, 50);
    DrawShouhuiMascot(hdc, centerX - 250, centerY - 80, 50);
    
    HFONT hDialogueFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hDialogueFont);
    
    // 对话内容
    const char* dialogues[] = {
        "A: 哇！这个人居然发现了这个彩蛋！",
        "B: 是谁呀？这么厉害！",
        "A: 让我看看...是用户 [USER] 呢！",
        "B: 一看就是一位资深的彩蛋猎人！让 [USER] 看看我们准备的惊喜吧！"
    };
    
    // 替换用户名占位符
    char finalDialogue[200];
    strcpy(finalDialogue, dialogues[dialogueIndexx]);
    
    // 替换 [USER] 为实际用户名
    char* userPlaceholder = strstr(finalDialogue, "[USER]");
    if (userPlaceholder) {
        char temp[200];
        strncpy(temp, finalDialogue, userPlaceholder - finalDialogue);
        temp[userPlaceholder - finalDialogue] = '\0';
        strcat(temp, username);
        strcat(temp, userPlaceholder + 6); // 跳过 "[USER]"
        strcpy(finalDialogue, temp);
    }
    
    // 绘制对话文本
    TextOut(hdc, centerX - 320, centerY - 30, finalDialogue, strlen(finalDialogue));
    
    // 绘制说话者标识
    HFONT hSpeakerFont = CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    SelectObject(hdc, hSpeakerFont);
    
    if (dialogueIndexx == 0 || dialogueIndexx == 2) {
        SetTextColor(hdc, RGB(255, 200, 100)); // A角色颜色
        TextOut(hdc, centerX - 320, centerY - 60, "水母A", 6);
    } else {
        SetTextColor(hdc, RGB(100, 200, 255)); // B角色颜色
        TextOut(hdc, centerX - 250, centerY - 60, "水母B", 6);
    }
    
    SetTextColor(hdc, RGB(255, 255, 255)); // 恢复白色
    
    // 绘制对话进度
    char progressText[50];
    sprintf(progressText, "对话 %d/4", dialogueIndexx + 1);
    TextOut(hdc, centerX - 30, centerY + 70, progressText, strlen(progressText));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hDialogueFont);
    DeleteObject(hSpeakerFont);
}

// 绘制开发时间线
void DrawDevTimeline(HDC hdc, int centerX, int centerY) {
    // 标题
    HFONT hTitleFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    TextOut(hdc, centerX - 150, 50, "f系统1.0 开发历程", 14);
    
    // 开发阶段内容
    HFONT hStageFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    HFONT hContentFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "宋体");
    
    const char* stages[] = {
        "Pre-Build 阶段 - 概念验证期",
        "Pre-Alpha 阶段 - 核心框架建设", 
        "Alpha 阶段 - 功能扩展期",
        "停止开发整理代码期 (2周)",
        "Pre-Beta 阶段 - 系统优化期",
        "Beta 阶段 - 专业功能集成",
        "停止开发调整开学作息 (2周)",
        "Release Candidate 阶段 - 发布准备"
    };
    
    const char* stageDetails[] = {
        " 2025年2月\n 项目启动和概念设计\n 界面原型设计\n 技术可行性分析\n 项目构思阶段，确定技术路线",
        " 2025年3-5月\n 系统初始化框架\n OOBE基础界面\n 配置数据存储结构\n 建立系统基础配置框架",
        " 2025年5-6月\n 桌面环境实现\n 首个应用程序\n 多用户支持实验\n 应用生态开始建设",
        " 2025年6月\n 代码重构和优化\n 架构重新设计\n 性能问题修复\n 为后续开发奠定基础",
        " 2025年7月\n 用户体验升级\n 系统配置完善\n 架构优化重构\n 从功能堆砌转向体验优化",
        " 2025年7-8月\n 文件管理时代开始\n 真实文件操作\n 功能整合优化\n 专业功能深度集成",
        " 2025年8月\n 开发节奏调整\n 功能规划重新评估\n 用户体验深度思考\n 为最终发布做准备",
        " 2025年9-2026年1月\n 发布准备阶段\n CD-KEY验证系统\n 最终质量验证\n 项目圆满完成"
    };
    
    SelectObject(hdc, hStageFont);
    TextOut(hdc, centerX - 200, 120, stages[currentDevStage], strlen(stages[currentDevStage]));
    
    SelectObject(hdc, hContentFont);
    
    // 分行显示详细内容
    char* detailCopy = _strdup(stageDetails[currentDevStage]);
    char* line = strtok(detailCopy, "\n");
    int lineY = 170;
    
    while (line != NULL) {
        TextOut(hdc, centerX - 300, lineY, line, strlen(line));
        line = strtok(NULL, "\n");
        lineY += 30;
    }
    
    free(detailCopy);
    
    // 绘制进度指示器
    char progressText[50];
    sprintf(progressText, "开发历程 %d/8", currentDevStage + 1);
    TextOut(hdc, centerX - 40, centerY + 150, progressText, strlen(progressText));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hStageFont);
    DeleteObject(hContentFont);
}
// 绘制功能展示
void DrawFeatureShowcase(HDC hdc, int centerX, int centerY) {
    HFONT hTitleFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    TextOut(hdc, centerX - 120, 50, "核心功能展示", 10);
    
    HFONT hFeatureFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    HFONT hDescFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "宋体");
    
    const char* features[] = {
        "?? 完整的桌面环境",
        "?? 用户账户系统", 
        "?? 真实文件管理器",
        "?? 多功能计算器",
        "?? 系统设置面板",
        "?? 隐藏彩蛋游戏"
    };
    
    const char* featureDetails[] = {
        "仿Windows的完整桌面体验\n任务栏系统、窗口管理、多应用支持",
        "OOBE配置向导、用户登录\n密码保护、个性化设置",
        "真实文件系统操作\n目录浏览、文件操作、命令支持",
        "科学计算、特殊彩蛋\n582彩蛋、班级纪念功能",
        "主题切换、用户设置\n系统配置、个性化定制",
        "水母逃亡游戏\n开发历程回顾彩蛋"
    };
    
    SelectObject(hdc, hFeatureFont);
    TextOut(hdc, centerX - 100, 120, features[currentDevStage], strlen(features[currentDevStage]));
    
    SelectObject(hdc, hDescFont);
    
    // 分行显示功能详情
    char* detailCopy = _strdup(featureDetails[currentDevStage]);
    char* line = strtok(detailCopy, "\n");
    int lineY = 160;
    
    while (line != NULL) {
        TextOut(hdc, centerX - 200, lineY, line, strlen(line));
        line = strtok(NULL, "\n");
        lineY += 25;
    }
    
    free(detailCopy);
    
    // 绘制进度指示器
    char progressText[50];
    sprintf(progressText, "功能展示 %d/6", currentDevStage + 1);
    TextOut(hdc, centerX - 40, centerY + 150, progressText, strlen(progressText));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hFeatureFont);
    DeleteObject(hDescFont);
}
// 绘制致谢名单
void DrawCredits(HDC hdc, int centerX, int centerY) {
    HFONT hTitleFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    TextOut(hdc, centerX - 100, 80, "开发人员名单", 10);
    
    HFONT hNameFont = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    HFONT hRoleFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "宋体");
    HFONT hThanksFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    
    // 在中央绘制大型守慧吉祥物
    DrawShouhuiMascot(hdc, centerX - 60, centerY - 50, 120);
    
    SelectObject(hdc, hNameFont);
    TextOut(hdc, centerX - 80, centerY + 80, "jammyjellyfish", 14);
    
    SelectObject(hdc, hRoleFont);
    TextOut(hdc, centerX - 150, centerY + 120, "唯一开发者 · 项目创始人 · 代码艺术家", 38);
    
    SelectObject(hdc, hThanksFont);
    
    // 感谢信息
    const char* thanksLines[] = {
        "感谢所有测试过f系统的朋友们",
        "感谢每一个发现彩蛋的探索者",
        "项目历时近一年，终于圆满完成",
        "f系统1.0 Professional - 梦想成真",
		"我们只能知道fos的爸爸是谁，但却无法知道fos的妈妈是谁" 
    };
    
    for (int i = 0; i < 5; i++) {
        TextOut(hdc, centerX - 200, centerY + 160 + i * 30, thanksLines[i], strlen(thanksLines[i]));
    }
    
    // 最终信息
    TextOut(hdc, centerX - 120, centerY + 320, "按ESC键返回文件管理器", 20);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hNameFont);
    DeleteObject(hRoleFont);
    DeleteObject(hThanksFont);
}
// 文件编辑器函数实现

// 显示文件编辑器
void ShowFileEditor(HWND hwnd, const char* filename) {
    showFileEditor = TRUE;
    showFileManager = FALSE;
    strcpy(editFileName, filename);
    cursorPosition = 0;
    scrollPosition = 0;
    isEditing = FALSE;
    
    // 根据文件扩展名设置文本格式
    const char* ext = strrchr(filename, '.');
    if (ext) {
        if (stricmp(ext, ".c") == 0 || stricmp(ext, ".cpp") == 0 || stricmp(ext, ".h") == 0) {
            textFormat = 1; // 代码格式
        } else if (stricmp(ext, ".md") == 0) {
            textFormat = 3; // Markdown格式
        } else {
            textFormat = 0; // 普通文本
        }
    } else {
        textFormat = 0;
    }
    
    LoadFileContent(filename);
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

// 绘制文件编辑器
void DrawFileEditor(HDC hdc, int centerX, int centerY) {
    RECT editorRect = {centerX - 450, centerY - 350, centerX + 450, centerY + 350};
    
    // 绘制编辑器背景
    HBRUSH hEditorBgBrush = CreateSolidBrush(EDITOR_BG);
    FillRect(hdc, &editorRect, hEditorBgBrush);
    DeleteObject(hEditorBgBrush);
    
    // 绘制边框
    HPEN hBorderPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, editorRect.left, editorRect.top, editorRect.right, editorRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    // 绘制标题栏
    RECT titleRect = {editorRect.left, editorRect.top, editorRect.right, editorRect.top + 30};
    HBRUSH hTitleBrush = CreateSolidBrush(VISTA_BLUE);
    FillRect(hdc, &titleRect, hTitleBrush);
    DeleteObject(hTitleBrush);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hTitleFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    char title[300];
    sprintf(title, "文件编辑器 - %s", editFileName);
    TextOut(hdc, editorRect.left + 10, editorRect.top + 5, title, strlen(title));
    
    // 显示格式信息
    const char* formatNames[] = {"普通文本", "代码", "富文本", "Markdown"};
    char formatInfo[100];
    sprintf(formatInfo, "格式: %s", formatNames[textFormat]);
    TextOut(hdc, editorRect.right - 150, editorRect.top + 5, formatInfo, strlen(formatInfo));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    
    // 绘制工具栏
    RECT toolbarRect = {editorRect.left, editorRect.top + 30, editorRect.right, editorRect.top + 60};
    HBRUSH hToolbarBrush = CreateSolidBrush(RGB(240, 240, 240));
    FillRect(hdc, &toolbarRect, hToolbarBrush);
    DeleteObject(hToolbarBrush);
    
    HPEN hToolbarPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    hOldPen = (HPEN)SelectObject(hdc, hToolbarPen);
    MoveToEx(hdc, editorRect.left, editorRect.top + 60, NULL);
    LineTo(hdc, editorRect.right, editorRect.top + 60);
    SelectObject(hdc, hOldPen);
    DeleteObject(hToolbarPen);
    
    // 绘制工具栏按钮
    HFONT hToolbarFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    hOldFont = (HFONT)SelectObject(hdc, hToolbarFont);
    
    SetTextColor(hdc, RGB(0, 0, 0));
    
    // 保存按钮
    RECT saveButton = {editorRect.left + 10, editorRect.top + 35, editorRect.left + 60, editorRect.top + 55};
    HBRUSH hSaveBrush = CreateSolidBrush(VISTA_GREEN);
    FillRect(hdc, &saveButton, hSaveBrush);
    DeleteObject(hSaveBrush);
    
    HPEN hSavePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    hOldPen = (HPEN)SelectObject(hdc, hSavePen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, saveButton.left, saveButton.top, saveButton.right, saveButton.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hSavePen);
    
    TextOut(hdc, saveButton.left + 5, saveButton.top + 5, "保存", 4);
    
    // 退出按钮
    RECT exitButton = {editorRect.left + 70, editorRect.top + 35, editorRect.left + 120, editorRect.top + 55};
    HBRUSH hExitBrush = CreateSolidBrush(VISTA_RED);
    FillRect(hdc, &exitButton, hExitBrush);
    DeleteObject(hExitBrush);
    
    HPEN hExitPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    hOldPen = (HPEN)SelectObject(hdc, hExitPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, exitButton.left, exitButton.top, exitButton.right, exitButton.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hExitPen);
    
    TextOut(hdc, exitButton.left + 5, exitButton.top + 5, "退出", 4);
    
    // 格式选择按钮
    RECT formatButton = {editorRect.left + 130, editorRect.top + 35, editorRect.left + 200, editorRect.top + 55};
    HBRUSH hFormatBrush = CreateSolidBrush(VISTA_LIGHT_BLUE);
    FillRect(hdc, &formatButton, hFormatBrush);
    DeleteObject(hFormatBrush);
    
    HPEN hFormatPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    hOldPen = (HPEN)SelectObject(hdc, hFormatPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, formatButton.left, formatButton.top, formatButton.right, formatButton.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hFormatPen);
    
    TextOut(hdc, formatButton.left + 5, formatButton.top + 5, "切换格式", 8);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hToolbarFont);
    
    // 绘制编辑区域
    RECT editRect = {editorRect.left + 10, editorRect.top + 70, editorRect.right - 10, editorRect.bottom - 10};
    HBRUSH hEditBrush = CreateSolidBrush(EDITOR_BG);
    FillRect(hdc, &editRect, hEditBrush);
    DeleteObject(hEditBrush);
    
    HPEN hEditPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    hOldPen = (HPEN)SelectObject(hdc, hEditPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, editRect.left, editRect.top, editRect.right, editRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hEditPen);
    
    // 绘制文本内容
    HFONT hEditFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Consolas");
    hOldFont = (HFONT)SelectObject(hdc, hEditFont);
    
    SetTextColor(hdc, EDITOR_TEXT);
    
    // 计算可见文本范围
    int lineHeight = 20;
    int visibleLines = (editRect.bottom - editRect.top) / lineHeight;
    int startLine = scrollPosition;
    int endLine = startLine + visibleLines;
    
    // 绘制行号
    HFONT hLineNumFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Consolas");
    HFONT hOldFont2 = (HFONT)SelectObject(hdc, hLineNumFont);
    
    SetTextColor(hdc, EDITOR_LINE_NUM);
    
    RECT lineNumRect = {editRect.left + 5, editRect.top, editRect.left + 40, editRect.bottom};
    HBRUSH hLineNumBrush = CreateSolidBrush(RGB(250, 250, 250));
    FillRect(hdc, &lineNumRect, hLineNumBrush);
    DeleteObject(hLineNumBrush);
    
    for (int i = startLine; i < endLine && i < 1000; i++) {
        char lineNum[10];
        sprintf(lineNum, "%4d", i + 1);
        TextOut(hdc, editRect.left + 5, editRect.top + (i - startLine) * lineHeight, lineNum, strlen(lineNum));
    }
    
    HPEN hLinePen = CreatePen(PS_SOLID, 1, RGB(230, 230, 230));
    hOldPen = (HPEN)SelectObject(hdc, hLinePen);
    MoveToEx(hdc, editRect.left + 45, editRect.top, NULL);
    LineTo(hdc, editRect.left + 45, editRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hLinePen);
    
    SelectObject(hdc, hOldFont2);
    DeleteObject(hLineNumFont);
    
    // 绘制文本内容（修复换行显示问题）
    SelectObject(hdc, hEditFont);
    SetTextColor(hdc, EDITOR_TEXT);
    
    // 按行分割文本内容
    char* textCopy = _strdup(fileContent);
    char* line = strtok(textCopy, "\n");
    int currentLine = 0;
    int textY = editRect.top + 5;
    
    while (line != NULL && currentLine < endLine) {
        if (currentLine >= startLine) {
            // 绘制当前行
            TextOut(hdc, editRect.left + 50, textY, line, strlen(line));
            textY += lineHeight;
        }
        line = strtok(NULL, "\n");
        currentLine++;
    }
    
    free(textCopy);
    
    // 绘制光标
    static DWORD lastBlink = GetTickCount();
    static BOOL showCursor = TRUE;
    
    if (GetTickCount() - lastBlink > 500) {
        showCursor = !showCursor;
        lastBlink = GetTickCount();
    }
    
    if (showCursor && isEditing) {
        // 计算光标位置
        int cursorLine = 0;
        int cursorCol = 0;
        int currentPos = 0;
        
        char* cursorText = _strdup(fileContent);
        char* lineStart = cursorText;
        
        for (int i = 0; i < cursorPosition && cursorText[i] != '\0'; i++) {
            if (cursorText[i] == '\n') {
                cursorLine++;
                cursorCol = 0;
                lineStart = &cursorText[i + 1];
            } else {
                cursorCol++;
            }
        }
        
        // 计算光标所在行的文本宽度
        char currentLineText[1000] = "";
        char* lineEnd = strchr(lineStart, '\n');
        if (lineEnd) {
            strncpy(currentLineText, lineStart, lineEnd - lineStart);
            currentLineText[lineEnd - lineStart] = '\0';
        } else {
            strcpy(currentLineText, lineStart);
        }
        
        // 计算光标前的文本宽度
        char beforeCursor[1000] = "";
        strncpy(beforeCursor, currentLineText, cursorCol);
        beforeCursor[cursorCol] = '\0';
        
        int textWidth = GetTextWidth(hdc, beforeCursor);
        
        if (cursorLine >= startLine && cursorLine < endLine) {
            int cursorY = editRect.top + 5 + (cursorLine - startLine) * lineHeight;
            MoveToEx(hdc, editRect.left + 50 + textWidth, cursorY, NULL);
            LineTo(hdc, editRect.left + 50 + textWidth, cursorY + lineHeight - 2);
        }
        
        free(cursorText);
    }
    
    // 绘制状态栏
    RECT statusRect = {editorRect.left, editorRect.bottom - 25, editorRect.right, editorRect.bottom};
    HBRUSH hStatusBrush = CreateSolidBrush(RGB(240, 240, 240));
    FillRect(hdc, &statusRect, hStatusBrush);
    DeleteObject(hStatusBrush);
    
    HPEN hStatusPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    hOldPen = (HPEN)SelectObject(hdc, hStatusPen);
    MoveToEx(hdc, editorRect.left, editorRect.bottom - 25, NULL);
    LineTo(hdc, editorRect.right, editorRect.bottom - 25);
    SelectObject(hdc, hOldPen);
    DeleteObject(hStatusPen);
    
    HFONT hStatusFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    hOldFont = (HFONT)SelectObject(hdc, hStatusFont);
    
    SetTextColor(hdc, RGB(100, 100, 100));
    
    // 计算行数和列数
    int lineCount = 1;
    int colCount = 0;
    int currentCol = 0;
    for (int i = 0; i < strlen(fileContent); i++) {
        if (fileContent[i] == '\n') {
            lineCount++;
            currentCol = 0;
        } else {
            currentCol++;
            if (currentCol > colCount) colCount = currentCol;
        }
    }
    
    char statusInfo[100];
    sprintf(statusInfo, "行: %d 列: %d 字符数: %d", cursorPosition, colCount, strlen(fileContent));
    TextOut(hdc, editRect.left + 10, editRect.bottom - 20, statusInfo, strlen(statusInfo));
    
    TextOut(hdc, editRect.right - 200, editRect.bottom - 20, "按Ctrl+S保存, Esc退出", 22);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hStatusFont);
    DeleteObject(hEditFont);
}

// 处理文件编辑器输入
void HandleFileEditorInput(char key) {
    if (key == 27) { // ESC - 退出编辑器
        showFileEditor = FALSE;
        showFileManager = TRUE;
        return;
    }
    
    if (key == 8) { // Backspace
        if (cursorPosition > 0) {
            DeleteCharAtCursor();
        }
    } else if (key == 13) { // Enter
        InsertCharAtCursor('\n');
    } else if (key >= 32 && key <= 126) { // 可打印字符
        InsertCharAtCursor(key);
    }
    
    isEditing = TRUE;
}

// 处理文件编辑器特殊按键
void HandleFileEditorSpecialKeys(WPARAM wParam) {
    switch (wParam) {
        case VK_LEFT:
            MoveCursorLeft();
            break;
        case VK_RIGHT:
            MoveCursorRight();
            break;
        case VK_UP:
            MoveCursorUp();
            break;
        case VK_DOWN:
            MoveCursorDown();
            break;
        case VK_HOME:
            cursorPosition = 0;
            break;
        case VK_END:
            cursorPosition = strlen(fileContent);
            break;
        case VK_DELETE:
            if (cursorPosition < strlen(fileContent)) {
                // 删除光标后的字符
                for (int i = cursorPosition; i < strlen(fileContent); i++) {
                    fileContent[i] = fileContent[i + 1];
                }
            }
            break;
        case VK_PRIOR: // Page Up
            scrollPosition = max(0, scrollPosition - 10);
            break;
        case VK_NEXT: // Page Down
            scrollPosition += 10;
            break;
    }
    
    isEditing = TRUE;
}

// 在光标位置插入字符
void InsertCharAtCursor(char ch) {
    int len = strlen(fileContent);
    if (len < 9999) {
        // 移动后续字符
        for (int i = len; i >= cursorPosition; i--) {
            fileContent[i + 1] = fileContent[i];
        }
        fileContent[cursorPosition] = ch;
        cursorPosition++;
    }
}

// 删除光标位置的字符
void DeleteCharAtCursor() {
    int len = strlen(fileContent);
    if (cursorPosition > 0 && len > 0) {
        // 移动后续字符
        for (int i = cursorPosition - 1; i < len; i++) {
            fileContent[i] = fileContent[i + 1];
        }
        cursorPosition--;
    }
}

// 光标左移
void MoveCursorLeft() {
    if (cursorPosition > 0) {
        cursorPosition--;
    }
}

// 光标右移
void MoveCursorRight() {
    if (cursorPosition < strlen(fileContent)) {
        cursorPosition++;
    }
}

// 光标上移
void MoveCursorUp() {
    // 找到上一行的位置
    if (cursorPosition == 0) return;
    
    int currentLineStart = 0;
    int prevLineStart = 0;
    int currentCol = 0;
    
    // 计算当前行的起始位置和列数
    for (int i = 0; i < cursorPosition; i++) {
        if (fileContent[i] == '\n') {
            prevLineStart = currentLineStart;
            currentLineStart = i + 1;
            currentCol = 0;
        } else {
            currentCol++;
        }
    }
    
    if (prevLineStart < currentLineStart) {
        // 移动到上一行的相应列位置
        int targetPos = prevLineStart + min(currentCol, GetLineLength(prevLineStart));
        cursorPosition = targetPos;
    }
}

// 光标下移
void MoveCursorDown() {
    // 找到下一行的位置
    int currentLineStart = 0;
    int nextLineStart = -1;
    int currentCol = 0;
    
    // 计算当前行的起始位置和列数
    for (int i = 0; i < cursorPosition; i++) {
        if (fileContent[i] == '\n') {
            currentLineStart = i + 1;
            currentCol = 0;
        } else {
            currentCol++;
        }
    }
    
    // 找到下一行的起始位置
    for (int i = cursorPosition; i < strlen(fileContent); i++) {
        if (fileContent[i] == '\n') {
            nextLineStart = i + 1;
            break;
        }
    }
    
    if (nextLineStart != -1) {
        // 移动到下一行的相应列位置
        int targetPos = nextLineStart + min(currentCol, GetLineLength(nextLineStart));
        cursorPosition = targetPos;
    } else {
        // 移动到文件末尾
        cursorPosition = strlen(fileContent);
    }
}

// 获取指定行的长度
int GetLineLength(int lineStart) {
    int length = 0;
    for (int i = lineStart; i < strlen(fileContent); i++) {
        if (fileContent[i] == '\n' || fileContent[i] == '\0') {
            break;
        }
        length++;
    }
    return length;
}

// 保存文件内容
void SaveFileContent() {
    char fullPath[MAX_PATH];
    sprintf(fullPath, "%s\\%s", currentPath, editFileName);
    
    HANDLE hFile = CreateFile(fullPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD bytesWritten;
        WriteFile(hFile, fileContent, strlen(fileContent), &bytesWritten, NULL);
        CloseHandle(hFile);
        
        // 显示保存成功消息
        MessageBox(NULL, "文件保存成功！", "保存", MB_OK | MB_ICONINFORMATION);
    } else {
        MessageBox(NULL, "保存文件失败！", "错误", MB_OK | MB_ICONERROR);
    }
}

// 加载文件内容
void LoadFileContent(const char* filename) {
    char fullPath[MAX_PATH];
    sprintf(fullPath, "%s\\%s", currentPath, filename);
    
    HANDLE hFile = CreateFile(fullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    
    if (hFile != INVALID_HANDLE_VALUE) {
        DWORD fileSize = GetFileSize(hFile, NULL);
        if (fileSize > 0 && fileSize < 10000) {
            DWORD bytesRead;
            ReadFile(hFile, fileContent, fileSize, &bytesRead, NULL);
            fileContent[bytesRead] = '\0';
        } else {
            strcpy(fileContent, "");
        }
        CloseHandle(hFile);
    } else {
        strcpy(fileContent, "");
    }
    
    cursorPosition = 0;
    scrollPosition = 0;
}

// 带格式的文本绘制（简化实现）
void DrawTextWithFormatting(HDC hdc, int x, int y, const char* text, int format) {
    switch (format) {
        case 0: // 普通文本
            SetTextColor(hdc, RGB(0, 0, 0));
            break;
        case 1: // 代码
            SetTextColor(hdc, RGB(0, 0, 128));
            break;
        case 2: // 富文本（简化）
            SetTextColor(hdc, RGB(0, 100, 0));
            break;
        case 3: // Markdown
            SetTextColor(hdc, RGB(128, 0, 128));
            break;
    }
    
    TextOut(hdc, x, y, text, strlen(text));
}

// CD-KEY验证函数
BOOL ValidateCDKey(const char* cdKey) {
    // 有效CD-KEY列表
    const char* validKeys[] = {
        "FOS62-20250-JELLY-FISH6-2CLASS",  // 主要CD-KEY
        "SHOUH-UI520-06262-FOSRC-20250",   // 备用CD-KEY
        "TEST0-00000-00000-00000-00001",    // 测试CD-KEY
        "PROF2-00000-00000-00000-00200",    // 专业版CD-KEY
        NULL
    };
    
    // 检查格式：XXXXX-XXXXX-XXXXX-XXXXX-XXXXX
    if (strlen(cdKey) != 29) return FALSE;
    
    for (int i = 0; i < 29; i++) {
        if ((i + 1) % 6 == 0) {
            if (cdKey[i] != '-') return FALSE;
        } else {
            if (!((cdKey[i] >= '0' && cdKey[i] <= '9') || 
                  (cdKey[i] >= 'A' && cdKey[i] <= 'Z'))) {
                return FALSE;
            }
        }
    }
    
    // 检查是否在有效密钥列表中
    for (int i = 0; validKeys[i] != NULL; i++) {
        if (strcmp(cdKey, validKeys[i]) == 0) {
            return TRUE;
        }
    }
    
    return FALSE;
}

// 应用试用限制
void ApplyTrialRestrictions() {
    // 在试用模式下限制某些功能
}

// 检查功能是否可用（试用模式限制）
BOOL IsFeatureAvailable(int feature) {
    if (isActivated) return TRUE;
    
    switch (feature) {
        case 1: // 计算器 - 完全可用
            return TRUE;
        case 2: // 系统信息 - 完全可用
            return TRUE;
        case 3: // 文件管理器 - 受限
            return (trialDaysRemaining > 15);
        case 4: // 设置软件 - 专业版功能
            return isActivated;
        default:
            return TRUE;
    }
}

// 检测游戏激活组合键
void CheckGameActivation(WPARAM wParam, HWND hwnd) {
    static BOOL ctrlPressed = FALSE;
    static BOOL jPressed = FALSE;
    
    if (wParam == VK_CONTROL) {
        ctrlPressed = TRUE;
    } else if (wParam == 'J' || wParam == 'j') {
        jPressed = TRUE;
    }
    
    // 检测Ctrl+J组合
    if (ctrlPressed && jPressed) {
        // 激活游戏
        showGameEgg = TRUE;
        showFinalEgg = FALSE;
        showCalculator = FALSE;
        showDesktop = FALSE;
        
        // 初始化游戏
        if (!gameInitialized) {
            InitializeJellyfishGame();
            gameInitialized = TRUE;
        }
        
        // 设置游戏定时器
        SetTimer(hwnd, 9, 50, NULL);
        
        InvalidateRect(hwnd, NULL, TRUE);
        
        // 重置按键状态
        ctrlPressed = FALSE;
        jPressed = FALSE;
    }
    
    // 处理按键释放
    if (wParam == VK_CONTROL) {
        ctrlPressed = FALSE;
    } else if (wParam == 'J' || wParam == 'j') {
        jPressed = FALSE;
    }
}

// 初始化水母游戏
void InitializeJellyfishGame() {
    gameRunning = TRUE;
    jellyfishX = 400;
    jellyfishY = 300;
    lives = 3;
    score = 0;
    gameOver = FALSE;
    gameState = 0;
    dialogueIndex = 0;
    level = 1;
    tentacleWave = 0.0f;
    levelTime = 0;
    
    // 重置玩家状态
    hasShield = FALSE;
    isInvisible = FALSE;
    isFrozen = FALSE;
    freezeTime = 0;
    powerUpTime = 0;
    
    // 初始化同学位置
    srand((unsigned)time(NULL));
    for (int i = 0; i < 5; i++) {
        studentX[i] = rand() % 800;
        studentY[i] = rand() % 600;
    }
    
    // 初始化关卡
    InitializeJellyfishLevel();
}

// 初始化水母游戏关卡
void InitializeJellyfishLevel() {
    // 清空障碍物和道具
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].active = FALSE;
    }
    for (int i = 0; i < MAX_POWERUPS; i++) {
        powerUps[i].active = FALSE;
    }
    
    // 根据关卡设置障碍物
    int obstacleCount = 3 + level * 2;
    for (int i = 0; i < obstacleCount && i < MAX_OBSTACLES; i++) {
        obstacles[i].x = 100 + rand() % 600;
        obstacles[i].y = 100 + rand() % 400;
        obstacles[i].width = 40 + rand() % 60;
        obstacles[i].height = 40 + rand() % 60;
        obstacles[i].active = TRUE;
    }
    
    // 生成道具
    int powerUpCount = 2 + level;
    for (int i = 0; i < powerUpCount && i < MAX_POWERUPS; i++) {
        powerUps[i].x = 50 + rand() % 700;
        powerUps[i].y = 50 + rand() % 500;
        powerUps[i].type = rand() % 4;
        powerUps[i].active = TRUE;
        powerUps[i].duration = 100 + level * 20;
    }
}

// 绘制水母游戏主函数
void DrawJellyfishGame(HDC hdc, HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    
    if (gameState == 0 || gameState == 2 || gameState == 3) {
        DrawJellyfishStory(hdc, hwnd);
    } else {
        DrawJellyfishGameplay(hdc, hwnd);
    }
}

// 绘制水母游戏剧情界面
void DrawJellyfishStory(HDC hdc, HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    
    // 背景
    HBRUSH hBgBrush = CreateSolidBrush(RGB(50, 50, 80));
    FillRect(hdc, &rect, hBgBrush);
    DeleteObject(hBgBrush);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hTitleFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    // 绘制当前对话
    if (gameState == 0) {
        if (storyDialogues[dialogueIndex] != NULL) {
            TextOut(hdc, 100, 200, storyDialogues[dialogueIndex], 
                   strlen(storyDialogues[dialogueIndex]));
        }
    } else if (gameState == 2) {
        if (levelDialogues[level-1][dialogueIndex] != NULL) {
            TextOut(hdc, 100, 200, levelDialogues[level-1][dialogueIndex], 
                   strlen(levelDialogues[level-1][dialogueIndex]));
        }
    } else if (gameState == 3) {
        if (punishmentDialogues[dialogueIndex] != NULL) {
            TextOut(hdc, 100, 200, punishmentDialogues[dialogueIndex], 
                   strlen(punishmentDialogues[dialogueIndex]));
        }
    }
    
    // 提示文字
    HFONT hHintFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "宋体");
    SelectObject(hdc, hHintFont);
    
    if (gameState == 3) {
        TextOut(hdc, 300, 500, "按R键重新开始...", 16);
    } else {
        TextOut(hdc, 300, 500, "按空格键继续...", 14);
    }
    
    // 绘制水母
    if (gameState != 3) {
        DrawJellyfish(hdc, 600, 300, 80);
    }
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hHintFont);
}

// 绘制水母游戏玩法界面
void DrawJellyfishGameplay(HDC hdc, HWND hwnd) {
    RECT rect;
    GetClientRect(hwnd, &rect);
    
    // 根据关卡改变背景
    COLORREF bgColors[] = { RGB(173, 216, 230), RGB(135, 206, 250), RGB(70, 130, 180) };
    HBRUSH hBgBrush = CreateSolidBrush(bgColors[level-1]);
    FillRect(hdc, &rect, hBgBrush);
    DeleteObject(hBgBrush);
    
    // 绘制障碍物
    DrawJellyfishObstacles(hdc);
    
    // 绘制道具
    DrawJellyfishPowerUps(hdc);
    
    // 绘制同学
    for (int i = 0; i < 5; i++) {
        DrawStudent(hdc, studentX[i], studentY[i], 40, i);
    }
    
    // 绘制水母
    DrawJellyfish(hdc, jellyfishX, jellyfishY, 60);
    
    // 绘制分数和状态
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    HFONT hFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hFont);
    
    sprintf(scoreText, "得分: %d  生命: %d  关卡: %d/3  时间: %d/%d秒", 
            score, lives, level, levelTime/20, levelTargetTime/20);
    TextOut(hdc, 10, 10, scoreText, strlen(scoreText));
    
    // 显示当前效果
    char statusText[100] = "";
    if (hasShield) strcat(statusText, "护盾 ");
    if (isInvisible) strcat(statusText, "隐身 ");
    if (isFrozen) strcat(statusText, "冰冻 ");
    if (powerUpTime > 0 && !hasShield && !isInvisible) strcat(statusText, "加速 ");
    
    if (strlen(statusText) > 0) {
        TextOut(hdc, 10, 35, "效果: ", 6);
        TextOut(hdc, 50, 35, statusText, strlen(statusText));
    }
    
    // 游戏结束显示
    if (gameOver) {
        RECT gameOverRect = {rect.right/2 - 200, rect.bottom/2 - 80, 
                           rect.right/2 + 200, rect.bottom/2 + 80};
        HBRUSH hOverlayBrush = CreateSolidBrush(RGB(0, 0, 0));
        FillRect(hdc, &gameOverRect, hOverlayBrush);
        DeleteObject(hOverlayBrush);
        
        SetTextColor(hdc, level == 3 ? RGB(0, 255, 0) : RGB(255, 0, 0));
        
        if (level == 3) {
            TextOut(hdc, rect.right/2 - 80, rect.bottom/2 - 40, "通关成功!", 10);
            TextOut(hdc, rect.right/2 - 120, rect.bottom/2 - 10, "你成功逃回了机房!", 18);
        } else {
            TextOut(hdc, rect.right/2 - 80, rect.bottom/2 - 40, "游戏结束!", 10);
        }
        
        char finalScore[50];
        sprintf(finalScore, "最终得分: %d", score);
        TextOut(hdc, rect.right/2 - 60, rect.bottom/2 + 20, finalScore, strlen(finalScore));
        
        TextOut(hdc, rect.right/2 - 100, rect.bottom/2 + 50, "按R键重新开始", 16);
        TextOut(hdc, rect.right/2 - 100, rect.bottom/2 + 80, "按ESC返回桌面", 16);
    }
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hFont);
}

// 绘制美观的水母
void DrawJellyfish(HDC hdc, int x, int y, int size) {
    HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
    
    // 水母头部 - 半透明效果
    int pulse = (int)(8 * sin(tentacleWave * 3));
    HBRUSH hHeadBrush;
    
    if (hasShield) {
        // 护盾状态 - 金色
        hHeadBrush = CreateSolidBrush(RGB(255, 215, 0));
    } else if (isInvisible) {
        // 隐身状态 - 半透明
        hHeadBrush = CreateSolidBrush(RGB(255, 105, 180));
    } else {
        // 正常状态 - 果酱色
        hHeadBrush = CreateSolidBrush(RGB(255, 105, 180));
    }
    
    SelectObject(hdc, hHeadBrush);
    
    // 更圆润的水母头部
    int headSize = size - pulse;
    Ellipse(hdc, x - headSize/2, y - headSize/3, 
            x + headSize/2, y + headSize/3);
    
    // 内部光晕
    HBRUSH hInnerBrush = CreateSolidBrush(RGB(255, 150, 200));
    SelectObject(hdc, hInnerBrush);
    Ellipse(hdc, x - headSize/3, y - headSize/5, 
            x + headSize/3, y + headSize/5);
    DeleteObject(hInnerBrush);
    
    DeleteObject(hHeadBrush);
    
    // 绘制触须特效
    DrawTentacleEffect(hdc, x, y, size);
    
    // 表情 - 根据状态变化
    HPEN hFacePen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
    SelectObject(hdc, hFacePen);
    
    if (gameState == 0 || gameState == 2) {
        // 剧情模式 - 好奇表情
        Arc(hdc, x - size/4, y - size/8, x - size/8, y, 
            x - size/4, y - size/16, x - size/8, y - size/16);
        Arc(hdc, x + size/8, y - size/8, x + size/4, y, 
            x + size/8, y - size/16, x + size/4, y - size/16);
        Arc(hdc, x - size/6, y + size/8, x + size/6, y + size/4, 
            x - size/6, y + size/6, x + size/6, y + size/6);
    } else {
        // 游戏模式 - 惊恐表情
        Ellipse(hdc, x - size/3, y - size/6, x - size/6, y);
        Ellipse(hdc, x + size/6, y - size/6, x + size/3, y);
        Ellipse(hdc, x - size/6, y + size/6, x + size/6, y + size/4);
    }
    
    // 护盾效果
    if (hasShield) {
        HPEN hShieldPen = CreatePen(PS_SOLID, 3, RGB(255, 215, 0));
        SelectObject(hdc, hShieldPen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Ellipse(hdc, x - size/2 - 5, y - size/2 - 5, 
                x + size/2 + 5, y + size/2 + 5);
        DeleteObject(hShieldPen);
    }
    
    DeleteObject(hFacePen);
    SelectObject(hdc, hOldPen);
}

// 绘制触须特效
void DrawTentacleEffect(HDC hdc, int x, int y, int size) {
    // 主触须 - 带波浪效果
    HPEN hTentaclePen = CreatePen(PS_SOLID, 3, RGB(220, 20, 140));
    SelectObject(hdc, hTentaclePen);
    
    for (int i = 0; i < 12; i++) {
        double angle = 2 * 3.14159 * i / 12;
        int startX = x + (int)(size/4 * cos(angle));
        int startY = y + size/4 + (int)(size/8 * sin(angle));
        
        MoveToEx(hdc, startX, startY, NULL);
        
        // 动态波浪效果
        for (int t = 0; t <= 80; t += 5) {
            double progress = t / 80.0;
            float waveFactor = (float)(sin(tentacleWave * 4 + i * 0.8 + progress * 12) * 
                                    (1.0 - progress) * 20);
            
            int currentX = startX + (int)(size * 0.6 * cos(angle) * progress);
            int currentY = startY + (int)(size * 0.8 * progress);
            
            LineTo(hdc, currentX + (int)(waveFactor * cos(angle + 1.57)), 
                      currentY + (int)(waveFactor * sin(angle + 1.57)));
        }
    }
    
    DeleteObject(hTentaclePen);
}

// 绘制同学
void DrawStudent(HDC hdc, int x, int y, int size, int id) {
    HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
    
    // 冰冻效果
    if (isFrozen) {
        HBRUSH hFrozenBrush = CreateSolidBrush(RGB(100, 100, 255));
        SelectObject(hdc, hFrozenBrush);
    } else {
        // 同学身体 - 根据关卡改变颜色
        COLORREF studentColors[3][5] = {
            { RGB(100, 100, 255), RGB(255, 100, 100), RGB(100, 255, 100), RGB(255, 255, 100), RGB(255, 100, 255) },
            { RGB(80, 80, 200), RGB(200, 80, 80), RGB(80, 200, 80), RGB(200, 200, 80), RGB(200, 80, 200) },
            { RGB(60, 60, 150), RGB(150, 60, 60), RGB(60, 150, 60), RGB(150, 150, 60), RGB(150, 60, 150) }
        };
        
        HBRUSH hBodyBrush = CreateSolidBrush(studentColors[level-1][id % 5]);
        SelectObject(hdc, hBodyBrush);
    }
    
    // 同学身体
    Ellipse(hdc, x - size/2, y - size/2, x + size/2, y + size/2);
    
    // 隐身状态下同学半透明
    if (isInvisible) {
        HBRUSH hTransparentBrush = CreateSolidBrush(RGB(128, 128, 128));
        SelectObject(hdc, hTransparentBrush);
        Ellipse(hdc, x - size/2, y - size/2, x + size/2, y + size/2);
        DeleteObject(hTransparentBrush);
    }
    
    // 表情
    HPEN hFacePen = CreatePen(PS_SOLID, level, RGB(0, 0, 0));
    SelectObject(hdc, hFacePen);
    
    // 倒眉
    MoveToEx(hdc, x - size/3, y - size/4, NULL);
    LineTo(hdc, x - size/6, y - size/6);
    MoveToEx(hdc, x + size/3, y - size/4, NULL);
    LineTo(hdc, x + size/6, y - size/6);
    
    // 嘴巴
    if (isFrozen) {
        // 冰冻表情 - 惊讶
        Ellipse(hdc, x - size/8, y + size/6, x + size/8, y + size/4);
    } else {
        // 愤怒表情
        MoveToEx(hdc, x - size/4, y + size/6, NULL);
        LineTo(hdc, x + size/4, y + size/6);
    }
    
    DeleteObject(hFacePen);
    SelectObject(hdc, hOldPen);
}

// 绘制障碍物
void DrawJellyfishObstacles(HDC hdc) {
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].active) {
            HBRUSH hObsBrush;
            switch(level) {
                case 1: hObsBrush = CreateSolidBrush(RGB(139, 69, 19)); break;
                case 2: hObsBrush = CreateSolidBrush(RGB(160, 82, 45)); break;
                case 3: hObsBrush = CreateSolidBrush(RGB(205, 133, 63)); break;
            }
            SelectObject(hdc, hObsBrush);
            Rectangle(hdc, obstacles[i].x, obstacles[i].y, 
                     obstacles[i].x + obstacles[i].width, 
                     obstacles[i].y + obstacles[i].height);
            DeleteObject(hObsBrush);
        }
    }
}

// 绘制道具
void DrawJellyfishPowerUps(HDC hdc) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerUps[i].active) {
            HBRUSH hPowerBrush;
            char symbol[2] = "?";
            
            switch(powerUps[i].type) {
                case 0: // 加速
                    hPowerBrush = CreateSolidBrush(RGB(0, 255, 0));
                    strcpy(symbol, "S");
                    break;
                case 1: // 隐身
                    hPowerBrush = CreateSolidBrush(RGB(128, 128, 128));
                    strcpy(symbol, "I");
                    break;
                case 2: // 护盾
                    hPowerBrush = CreateSolidBrush(RGB(255, 215, 0));
                    strcpy(symbol, "P");
                    break;
                case 3: // 冰冻
                    hPowerBrush = CreateSolidBrush(RGB(0, 100, 255));
                    strcpy(symbol, "F");
                    break;
            }
            
            SelectObject(hdc, hPowerBrush);
            Ellipse(hdc, powerUps[i].x - 15, powerUps[i].y - 15, 
                    powerUps[i].x + 15, powerUps[i].y + 15);
            DeleteObject(hPowerBrush);
            
            // 道具符号
            SetTextColor(hdc, RGB(255, 255, 255));
            SetBkMode(hdc, TRANSPARENT);
            TextOut(hdc, powerUps[i].x - 5, powerUps[i].y - 8, symbol, 1);
        }
    }
}

// 更新水母游戏状态
void UpdateJellyfishGame(HWND hwnd) {
    if (gameOver || gameState == 0 || gameState == 2 || gameState == 3) return;
    
    // 更新触须波浪
    tentacleWave += 0.1f;
    if (tentacleWave > 2 * 3.14159) tentacleWave = 0;
    
    // 更新关卡时间
    levelTime++;
    
    // 更新道具效果时间
    if (powerUpTime > 0) {
        powerUpTime--;
        if (powerUpTime == 0) {
            isInvisible = FALSE;
            hasShield = FALSE;
        }
    }
    
    if (freezeTime > 0) {
        freezeTime--;
        if (freezeTime == 0) {
            isFrozen = FALSE;
        }
    }
    
    // 同学追逐逻辑
    int studentSpeed = 2 + level;
    if (isFrozen) studentSpeed = 0;
    
    for (int i = 0; i < 5; i++) {
        // 向水母移动
        if (studentX[i] < jellyfishX) studentX[i] += studentSpeed;
        else studentX[i] -= studentSpeed;
        
        if (studentY[i] < jellyfishY) studentY[i] += studentSpeed;
        else studentY[i] -= studentSpeed;
        
        // 同学边界检查
        if (studentX[i] < 20) studentX[i] = 20;
        if (studentX[i] > 780) studentX[i] = 780;
        if (studentY[i] < 20) studentY[i] = 20;
        if (studentY[i] > 580) studentY[i] = 580;
    }
    
    score++;
    
    // 检测碰撞
    CheckJellyfishCollisions();
    
    // 关卡进度检查
    if (levelTime >= levelTargetTime) {
        gameState = 2;
        dialogueIndex = 0;
        if (level < 3) {
            level++;
            InitializeJellyfishLevel();
            levelTime = 0;
        } else {
            // 通关
            gameOver = TRUE;
        }
    }
    
    InvalidateRect(hwnd, NULL, TRUE);
}

// 检测碰撞
void CheckJellyfishCollisions() {
    // 检测障碍物碰撞
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].active) {
            if (jellyfishX > obstacles[i].x && jellyfishX < obstacles[i].x + obstacles[i].width &&
                jellyfishY > obstacles[i].y && jellyfishY < obstacles[i].y + obstacles[i].height) {
                // 碰到障碍物，反弹
                if (jellyfishX < obstacles[i].x + obstacles[i].width / 2) {
                    jellyfishX = obstacles[i].x - 20;
                } else {
                    jellyfishX = obstacles[i].x + obstacles[i].width + 20;
                }
            }
        }
    }
    
    // 检测道具碰撞
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerUps[i].active) {
            int dx = powerUps[i].x - jellyfishX;
            int dy = powerUps[i].y - jellyfishY;
            double distance = sqrt(dx * dx + dy * dy);
            
            if (distance < 25) {
                ActivatePowerUp(powerUps[i].type);
                powerUps[i].active = FALSE;
                score += 50;
            }
        }
    }
    
    // 检测同学碰撞
    if (!hasShield && !isInvisible) {
        for (int i = 0; i < 5; i++) {
            int dx = studentX[i] - jellyfishX;
            int dy = studentY[i] - jellyfishY;
            double distance = sqrt(dx * dx + dy * dy);
            
            if (distance < 35) {
                lives--;
                studentX[i] = rand() % 800;
                studentY[i] = rand() % 600;
                
                if (lives <= 0) {
                    gameState = 3;
                    dialogueIndex = 0;
                }
            }
        }
    }
}

// 激活道具
void ActivatePowerUp(int type) {
    switch(type) {
        case 0: // 加速
            powerUpTime = 150;
            break;
        case 1: // 隐身
            isInvisible = TRUE;
            powerUpTime = 200;
            break;
        case 2: // 护盾
            hasShield = TRUE;
            powerUpTime = 180;
            break;
        case 3: // 冰冻
            isFrozen = TRUE;
            freezeTime = 120;
            break;
    }
}

// 处理水母游戏输入
void HandleJellyfishGameInput(WPARAM wParam, HWND hwnd) {
    if (wParam == VK_ESCAPE) {
        // 退出游戏返回桌面
        showGameEgg = FALSE;
        showDesktop = TRUE;
        KillTimer(hwnd, 9);
        InvalidateRect(hwnd, NULL, TRUE);
        return;
    }
    
    if (gameState == 0 || gameState == 2 || gameState == 3) {
        // 剧情模式输入处理
        if (wParam == VK_SPACE) {
            dialogueIndex++;
            if ((gameState == 0 && storyDialogues[dialogueIndex] == NULL) ||
                (gameState == 2 && levelDialogues[level-1][dialogueIndex] == NULL) ||
                (gameState == 3 && punishmentDialogues[dialogueIndex] == NULL)) {
                if (gameState == 3) {
                    // 重新开始游戏
                    InitializeJellyfishGame();
                } else {
                    gameState = 1;
                }
            }
            InvalidateRect(hwnd, NULL, TRUE);
        }
        else if ((wParam == 'R' || wParam == 'r') && gameState == 3) {
            // 重新开始游戏
            InitializeJellyfishGame();
            InvalidateRect(hwnd, NULL, TRUE);
        }
    } else if (gameState == 1 && !gameOver) {
        // 游戏模式输入处理
        int moveSpeed = 10;
        if (powerUpTime > 0 && !hasShield && !isInvisible) {
            moveSpeed = 15;
        }
        
        switch(wParam) {
            case VK_LEFT:  jellyfishX -= moveSpeed; break;
            case VK_RIGHT: jellyfishX += moveSpeed; break;
            case VK_UP:    jellyfishY -= moveSpeed; break;
            case VK_DOWN:  jellyfishY += moveSpeed; break;
        }
        
        // 边界检查
        if (jellyfishX < 20) jellyfishX = 20;
        if (jellyfishX > 780) jellyfishX = 780;
        if (jellyfishY < 20) jellyfishY = 20;
        if (jellyfishY > 580) jellyfishY = 580;
        
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

// 初始化版本信息
void InitializeVersionInfo() {
    if (isActivated) {
        sprintf(osVersion, "fOS %d.%d Professional (Build 400) - 已激活", MAJOR_VERSION, MINOR_VERSION);
    } else if (isTrialMode) {
        sprintf(osVersion, "fOS %d.%d Professional (Build 400) - 试用版 (%d天)", MAJOR_VERSION, MINOR_VERSION, trialDaysRemaining);
    } else {
        sprintf(osVersion, "fOS %d.%d Professional (Build 400) - 未激活", MAJOR_VERSION, MINOR_VERSION);
    }
    
    sprintf(buildInfo, "Build %d (fOS.%d.%d.Professional)", BUILD_VERSION, MAJOR_VERSION, MINOR_VERSION);
    sprintf(versionInfo, "fOS %d.%d Professional\n版本 %d.%d.%d.%d\n编译于 %d年", 
            MAJOR_VERSION, MINOR_VERSION, 
            MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION, RELEASE_YEAR,
            RELEASE_YEAR);
}

// 绘制守慧吉祥物
void DrawShouhuiMascot(HDC hdc, int x, int y, int size) {
    // 保存原始画笔和画刷
    HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, GetStockObject(NULL_BRUSH));
    
    // 绘制身体（黄色圆形）
    HBRUSH hBodyBrush = CreateSolidBrush(SHOUHUI_BODY);
    SelectObject(hdc, hBodyBrush);
    Ellipse(hdc, x, y, x + size, y + size);
    DeleteObject(hBodyBrush);
    
    // 绘制眼睛
    int eyeSize = size / 10;
    int eyeOffset = size / 4;
    HBRUSH hEyeBrush = CreateSolidBrush(SHOUHUI_EYES);
    SelectObject(hdc, hEyeBrush);
    Ellipse(hdc, x + eyeOffset - eyeSize/2, y + eyeOffset - eyeSize/2, 
            x + eyeOffset + eyeSize/2, y + eyeOffset + eyeSize/2);
    Ellipse(hdc, x + size - eyeOffset - eyeSize/2, y + eyeOffset - eyeSize/2, 
            x + size - eyeOffset + eyeSize/2, y + eyeOffset + eyeSize/2);
    DeleteObject(hEyeBrush);
    
    // 绘制嘴巴（微笑）
    HPEN hMouthPen = CreatePen(PS_SOLID, size/20, SHOUHUI_MOUTH);
    SelectObject(hdc, hMouthPen);
    Arc(hdc, x + size/4, y + size/2, x + size*3/4, y + size*3/4, 
        x + size/4, y + size/2, x + size*3/4, y + size/2);
    DeleteObject(hMouthPen);
    
    // 绘制触角
    int antennaHeight = size / 3;
    HPEN hAntennaPen = CreatePen(PS_SOLID, size/30, SHOUHUI_BODY);
    SelectObject(hdc, hAntennaPen);
    
    // 左触角
    MoveToEx(hdc, x + size/3, y + size/6, NULL);
    LineTo(hdc, x + size/6, y - antennaHeight/2);
    Ellipse(hdc, x + size/8 - size/20, y - antennaHeight/2 - size/20, 
            x + size/8 + size/20, y - antennaHeight/2 + size/20);
    
    // 右触角
    MoveToEx(hdc, x + size*2/3, y + size/6, NULL);
    LineTo(hdc, x + size*5/6, y - antennaHeight/2);
    Ellipse(hdc, x + size*7/8 - size/20, y - antennaHeight/2 - size/20, 
            x + size*7/8 + size/20, y - antennaHeight/2 + size/20);
    
    DeleteObject(hAntennaPen);
    
    // 恢复原始画笔和画刷
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldBrush);
}

// 绘制CD-KEY界面背景
void DrawCDKeyBackground(HDC hdc, RECT rect) {
    // 创建渐变背景
    for (int y = rect.top; y < rect.bottom; y++) {
        double ratio = (double)(y - rect.top) / (rect.bottom - rect.top);
        int r = (int)(40 + (255-40) * ratio);
        int g = (int)(80 + (255-80) * ratio);
        int b = (int)(160 + (255-160) * ratio);
        
        COLORREF color = RGB(r, g, b);
        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, rect.left, y, NULL);
        LineTo(hdc, rect.right, y);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
}

// 绘制CD-KEY界面
void DrawCDKeyInterface(HDC hdc, int centerX, int centerY) {
    RECT cdKeyRect = {centerX - 400, centerY - 300, centerX + 400, centerY + 300};
    
    // 绘制背景
    DrawCDKeyBackground(hdc, cdKeyRect);
    
    // 绘制窗口边框
    HPEN hBorderPen = CreatePen(PS_SOLID, 3, VISTA_LIGHT_BLUE);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, cdKeyRect.left, cdKeyRect.top, cdKeyRect.right, cdKeyRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    // 在左上角绘制守慧吉祥物
    DrawShouhuiMascot(hdc, cdKeyRect.left + 30, cdKeyRect.top + 30, 80);
    
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    
    HFONT hTitleFont = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hNormalFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hSmallFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    // 绘制标题
    TextOut(hdc, centerX - 150, centerY - 250, "产品激活", 8);
    
    SelectObject(hdc, hNormalFont);
    TextOut(hdc, centerX - 350, centerY - 200, "感谢您选择 f系统1.0 Professional", 34);
    TextOut(hdc, centerX - 280, centerY - 170, "请输入您的CD-KEY以激活完整功能", 30);
    
    // 绘制CD-KEY输入框
    RECT inputRect = {centerX - 200, centerY - 120, centerX + 200, centerY - 85};
    HBRUSH hInputBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &inputRect, hInputBrush);
    DeleteObject(hInputBrush);
    
    HPEN hInputPen = CreatePen(PS_SOLID, 2, VISTA_LIGHT_BLUE);
    hOldPen = (HPEN)SelectObject(hdc, hInputPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX - 200, centerY - 120, centerX + 200, centerY - 85);
    SelectObject(hdc, hOldPen);
    DeleteObject(hInputPen);
    
    SetTextColor(hdc, RGB(0, 0, 0));
    TextOut(hdc, centerX - 190, centerY - 115, cdKeyInput, strlen(cdKeyInput));
    
    if (cdKeyError) {
        SetTextColor(hdc, VISTA_RED);
        TextOut(hdc, centerX - 180, centerY - 70, cdKeyErrorMsg, strlen(cdKeyErrorMsg));
        SetTextColor(hdc, RGB(255, 255, 255));
    }
    
    // 绘制CD-KEY格式提示
    SelectObject(hdc, hSmallFont);
    TextOut(hdc, centerX - 180, centerY - 30, "格式: XXXXX-XXXXX-XXXXX-XXXXX-XXXXX", 35);
    
    // 绘制功能对比
    TextOut(hdc, centerX - 350, centerY + 20, "功能对比:", 10);
    
    // 已激活功能
    SetTextColor(hdc, VISTA_GREEN);
    TextOut(hdc, centerX - 350, centerY + 50, "? 完整系统功能", 14);
    TextOut(hdc, centerX - 350, centerY + 80, "? 无限制使用时间", 15);
    TextOut(hdc, centerX - 350, centerY + 110, "? 所有应用程序", 13);
    TextOut(hdc, centerX - 350, centerY + 140, "? 专业版设置工具", 14);
    TextOut(hdc, centerX - 350, centerY + 170, "? 多主题支持", 10);
    
    // 试用版限制
    SetTextColor(hdc, VISTA_ORANGE);
    TextOut(hdc, centerX - 100, centerY + 50, "? 30天试用期", 12);
    TextOut(hdc, centerX - 100, centerY + 80, "? 部分功能受限", 13);
    TextOut(hdc, centerX - 100, centerY + 110, "? 无技术支持", 11);
    TextOut(hdc, centerX - 100, centerY + 140, "? 仅供评估使用", 13);
    TextOut(hdc, centerX - 100, centerY + 170, "? 无设置工具", 11);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    
    // 绘制按钮
    int buttonWidth = 120;
    int buttonHeight = 40;
    int buttonY = centerY + 180;
    
    // 激活按钮
    RECT activateRect = {centerX - 270, buttonY, centerX - 270 + buttonWidth, buttonY + buttonHeight};
    HBRUSH hActivateBrush = CreateSolidBrush(VISTA_GREEN);
    FillRect(hdc, &activateRect, hActivateBrush);
    DeleteObject(hActivateBrush);
    
    HPEN hActivatePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    hOldPen = (HPEN)SelectObject(hdc, hActivatePen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX - 270, buttonY, centerX - 270 + buttonWidth, buttonY + buttonHeight);
    SelectObject(hdc, hOldPen);
    DeleteObject(hActivatePen);
    
    TextOut(hdc, centerX - 250, buttonY + 12, "激活产品", 8);
    
    // 跳过按钮（试用）
    RECT trialRect = {centerX - 130, buttonY, centerX - 130 + buttonWidth, buttonY + buttonHeight};
    HBRUSH hTrialBrush = CreateSolidBrush(VISTA_ORANGE);
    FillRect(hdc, &trialRect, hTrialBrush);
    DeleteObject(hTrialBrush);
    
    HPEN hTrialPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    hOldPen = (HPEN)SelectObject(hdc, hTrialPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX - 130, buttonY, centerX - 130 + buttonWidth, buttonY + buttonHeight);
    SelectObject(hdc, hOldPen);
    DeleteObject(hTrialPen);
    
    TextOut(hdc, centerX - 110, buttonY + 12, "试用版", 6);
    
    // 退出按钮
    RECT exitRect = {centerX + 10, buttonY, centerX + 10 + buttonWidth, buttonY + buttonHeight};
    HBRUSH hExitBrush = CreateSolidBrush(VISTA_RED);
    FillRect(hdc, &exitRect, hExitBrush);
    DeleteObject(hExitBrush);
    
    HPEN hExitPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    hOldPen = (HPEN)SelectObject(hdc, hExitPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX + 10, buttonY, centerX + 10 + buttonWidth, buttonY + buttonHeight);
    SelectObject(hdc, hOldPen);
    DeleteObject(hExitPen);
    
    TextOut(hdc, centerX + 30, buttonY + 12, "退出", 4);
    
    // 绘制提示信息
    SelectObject(hdc, hSmallFont);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hNormalFont);
    DeleteObject(hSmallFont);
}

// 处理CD-KEY输入
void HandleCDKeyInput(char key) {
    if (key == 8) {
        if (strlen(cdKeyInput) > 0) {
            cdKeyInput[strlen(cdKeyInput) - 1] = '\0';
        }
        cdKeyError = FALSE;
    } else if (key == 13) {
        if (ValidateCDKey(cdKeyInput)) {
            isActivated = TRUE;
            isTrialMode = FALSE;
            cdKeyError = FALSE;
            ShowOOBE(NULL);
        } else {
            cdKeyError = TRUE;
            strcpy(cdKeyErrorMsg, "无效的CD-KEY，请检查后重试");
        }
    } else if (key == 27) {
        // 修复：试用版按钮按下后正确启动试用模式
        StartTrialMode(NULL);
    } else if (key >= 32 && key <= 126 && strlen(cdKeyInput) < 29) {
        int len = strlen(cdKeyInput);
        
        // 自动添加分隔符
        if (len == 5 || len == 11 || len == 17 || len == 23) {
            cdKeyInput[len] = '-';
            cdKeyInput[len + 1] = '\0';
        }
        
        // 转换为大写
        cdKeyInput[len] = toupper(key);
        cdKeyInput[len + 1] = '\0';
        
        cdKeyError = FALSE;
    }
}

// 显示CD-KEY界面
void ShowCDKeyScreen(HWND hwnd) {
    showCDKeyScreen = TRUE;
    showBootAnimation = FALSE;
    showWelcome = FALSE;
    showSecondScreen = FALSE;
    showOOBE = FALSE;
    showComplete = FALSE;
    showDesktop = FALSE;
    showShutdown = FALSE;
    strcpy(cdKeyInput, "");
    cdKeyError = FALSE;
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 绘制OOBE背景
void DrawOOBEBackground(HDC hdc, RECT rect) {
    // 创建渐变背景
    for (int y = rect.top; y < rect.bottom; y++) {
        double ratio = (double)(y - rect.top) / (rect.bottom - rect.top);
        int r = (int)(30 + (255-30) * ratio);
        int g = (int)(60 + (255-60) * ratio);
        int b = (int)(120 + (255-120) * ratio);
        
        COLORREF color = RGB(r, g, b);
        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, rect.left, y, NULL);
        LineTo(hdc, rect.right, y);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
    
    // 在右上角绘制版本信息
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(200, 200, 200));
    
    HFONT hVersionFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hVersionFont);
    
    char versionText[100];
    if (isActivated) {
        sprintf(versionText, "fOS %d.%d Build %d - 专业版已激活", MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION);
    } else if (isTrialMode) {
        sprintf(versionText, "fOS %d.%d Build %d - 试用版", MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION);
    } else {
        sprintf(versionText, "fOS %d.%d Build %d", MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION);
    }
    TextOut(hdc, rect.right - 200, rect.top + 20, versionText, strlen(versionText));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hVersionFont);
}

// 绘制OOBE进度指示器
void DrawOOBEProgress(HDC hdc, int centerX, int centerY) {
    int totalSteps = 4;
    int stepWidth = 80;
    int totalWidth = totalSteps * stepWidth;
    int startX = centerX - totalWidth / 2;
    int y = centerY + 200;
    
    HFONT hStepFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hStepFont);
    
    const char* stepNames[] = {"欢迎", "用户设置", "密码设置", "完成"};
    
    for (int i = 0; i < totalSteps; i++) {
        int x = startX + i * stepWidth;
        
        // 绘制步骤圆圈
        BOOL isCompleted = i < currentStep;
        BOOL isCurrent = i == currentStep;
        
        COLORREF circleColor = isCompleted ? VISTA_GREEN : (isCurrent ? VISTA_LIGHT_BLUE : RGB(100, 100, 100));
        HBRUSH hCircleBrush = CreateSolidBrush(circleColor);
        HPEN hCirclePen = CreatePen(PS_SOLID, 2, circleColor);
        
        SelectObject(hdc, hCircleBrush);
        SelectObject(hdc, hCirclePen);
        Ellipse(hdc, x - 15, y - 15, x + 15, y + 15);
        
        DeleteObject(hCircleBrush);
        DeleteObject(hCirclePen);
        
        // 绘制步骤文本
        SetTextColor(hdc, isCompleted || isCurrent ? RGB(255, 255, 255) : RGB(150, 150, 150));
        int textWidth = GetTextWidth(hdc, stepNames[i]);
        TextOut(hdc, x - textWidth/2, y + 25, stepNames[i], strlen(stepNames[i]));
        
        // 绘制连接线（除了最后一步）
        if (i < totalSteps - 1) {
            HPEN hLinePen = CreatePen(PS_SOLID, 2, isCompleted ? VISTA_GREEN : RGB(100, 100, 100));
            SelectObject(hdc, hLinePen);
            MoveToEx(hdc, x + 15, y, NULL);
            LineTo(hdc, x + stepWidth - 15, y);
            DeleteObject(hLinePen);
        }
    }
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hStepFont);
}

BOOL ValidateOOBEInput() {
    oobeError = FALSE;
    
    switch (currentStep) {
        case 1:
            if (strlen(username) == 0) {
                oobeError = TRUE;
                strcpy(oobeErrorMsg, "用户名不能为空");
                return FALSE;
            }
            if (strlen(username) < 3) {
                oobeError = TRUE;
                strcpy(oobeErrorMsg, "用户名至少需要3个字符");
                return FALSE;
            }
            break;
            
        case 2:
        	
            if (skipPasswordSetup) {
                strcpy(password, "1234");
                strcpy(confirmPassword, "1234");
                return TRUE;
            }
            
            if (strlen(password) == 0) {
                oobeError = TRUE;
                strcpy(oobeErrorMsg, "密码不能为空");
                return FALSE;
            }
            if (strlen(password) < 4) {
                oobeError = TRUE;
                strcpy(oobeErrorMsg, "密码至少需要4个字符");
                return FALSE;
            }
            if (strcmp(password, confirmPassword) != 0) {
                oobeError = TRUE;
                strcpy(oobeErrorMsg, "两次输入的密码不一致");
                return FALSE;
            }
            break;
            
        case 3:
            if (strlen(computerName) == 0) {
                oobeError = TRUE;
                strcpy(oobeErrorMsg, "计算机名不能为空");
                return FALSE;
            }
            if (strlen(computerName) < 2) {
                oobeError = TRUE;
                strcpy(oobeErrorMsg, "计算机名至少需要2个字符");
                return FALSE;
            }
            break;
    }
    
    return TRUE;
}

// 绘制输入框
void DrawInputBox(HDC hdc, int x, int y, int width, int height, const char* label, const char* value, 
                 BOOL isActive, BOOL isPassword, const char* placeholder) {
    SetTextColor(hdc, RGB(255, 255, 255));
    TextOut(hdc, x, y - 25, label, strlen(label));
    
    RECT inputRect = {x, y, x + width, y + height};
    
    // 绘制输入框背景
    HBRUSH hInputBrush = CreateSolidBrush(isActive ? RGB(240, 240, 240) : RGB(220, 220, 220));
    FillRect(hdc, &inputRect, hInputBrush);
    DeleteObject(hInputBrush);
    
    // 绘制边框
    HPEN hBorderPen = CreatePen(PS_SOLID, 2, isActive ? VISTA_LIGHT_BLUE : RGB(150, 150, 150));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, x, y, x + width, y + height);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    // 绘制文本
    SetTextColor(hdc, RGB(0, 0, 0));
    if (strlen(value) > 0) {
        char display[100];
        if (isPassword) {
            int len = strlen(value);
            for (int i = 0; i < len; i++) {
                display[i] = '*';
            }
            display[len] = '\0';
        } else {
            strcpy(display, value);
        }
        TextOut(hdc, x + 10, y + 12, display, strlen(display));
    } else if (placeholder && !isActive) {
        SetTextColor(hdc, RGB(150, 150, 150));
        TextOut(hdc, x + 10, y + 12, placeholder, strlen(placeholder));
        SetTextColor(hdc, RGB(0, 0, 0));
    }
    
    // 绘制光标
    if (isActive) {
        int textWidth = 0;
        if (strlen(value) > 0) {
            char display[100];
            if (isPassword) {
                int len = strlen(value);
                for (int i = 0; i < len; i++) display[i] = '*';
                display[len] = '\0';
                textWidth = GetTextWidth(hdc, display);
            } else {
                textWidth = GetTextWidth(hdc, value);
            }
        }
        
        static DWORD lastBlink = GetTickCount();
        static BOOL showCursor = TRUE;
        
        if (GetTickCount() - lastBlink > 500) {
            showCursor = !showCursor;
            lastBlink = GetTickCount();
        }
        
        if (showCursor) {
            MoveToEx(hdc, x + 10 + textWidth, y + 8, NULL);
            LineTo(hdc, x + 10 + textWidth, y + height - 8);
        }
    }
    
    SetTextColor(hdc, RGB(255, 255, 255));
}

// 绘制OOBE步骤界面 - 修复：添加密码跳过按钮和主题选择
void DrawOOBEStep(HDC hdc, int centerX, int centerY) {
    HFONT hTitleFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hNormalFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hSmallFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    // 在左上角绘制守慧吉祥物
    DrawShouhuiMascot(hdc, 50, 50, 60);
    
    // 显示步骤标题和说明
    switch (currentStep) {
        case 0: {
            TextOut(hdc, centerX - 200, centerY - 250, "欢迎使用 f系统1.0 专业版", 22);
            SelectObject(hdc, hNormalFont);
            TextOut(hdc, centerX - 300, centerY - 180, "感谢您选择 fOS 1.0 Professional codename nori", 45);
            TextOut(hdc, centerX - 250, centerY - 140, "这是一个全新的操作系统体验", 26);
            
            // 显示激活状态
            if (isActivated) {
                SetTextColor(hdc, VISTA_GREEN);
                TextOut(hdc, centerX - 100, centerY - 80, "? 系统已激活 - 完整功能", 22);
            } else if (isTrialMode) {
                SetTextColor(hdc, VISTA_ORANGE);
                TextOut(hdc, centerX - 120, centerY - 80, "? 试用版 - 部分功能受限", 23);
            }
            SetTextColor(hdc, RGB(255, 255, 255));
            
            // 显示版本特性
            SelectObject(hdc, hSmallFont);
            TextOut(hdc, centerX - 200, centerY - 40, "? 现代化的用户界面", 18);
            TextOut(hdc, centerX - 200, centerY - 10, "? 内置实用工具", 14);
            TextOut(hdc, centerX - 200, centerY + 20, "? 安全可靠的系统架构", 20);
            TextOut(hdc, centerX - 200, centerY + 50, "? 友好的用户体验", 17);
            TextOut(hdc, centerX - 200, centerY + 80, "? 专业版设置工具", 16);
            TextOut(hdc, centerX - 200, centerY + 110, "? 多主题支持", 10);
            
            // 绘制下一步按钮
            RECT buttonRect = {centerX - 80, centerY + 80, centerX + 80, centerY + 120};
            HBRUSH hButtonBrush = CreateSolidBrush(VISTA_GREEN);
            FillRect(hdc, &buttonRect, hButtonBrush);
            DeleteObject(hButtonBrush);
    
            HPEN hButtonPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hButtonPen);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, centerX - 80, centerY + 80, centerX + 80, centerY + 120);
            SelectObject(hdc, hOldPen);
            DeleteObject(hButtonPen);
    
            SetTextColor(hdc, RGB(255, 255, 255));
            SelectObject(hdc, hNormalFont);
            TextOut(hdc, centerX - 30, centerY + 95, "开始设置", 8);
            break;
        }
            
        case 1: {
            TextOut(hdc, centerX - 150, centerY - 250, "创建用户账户", 12);
            SelectObject(hdc, hNormalFont);
            TextOut(hdc, centerX - 250, centerY - 200, "请为您的账户设置用户名", 24);
            
            DrawInputBox(hdc, centerX - 200, centerY - 120, 400, 35, "用户名:", username, TRUE, FALSE, "输入用户名（至少3个字符）");
            
            if (oobeError) {
                SetTextColor(hdc, VISTA_RED);
                TextOut(hdc, centerX - 180, centerY - 70, oobeErrorMsg, strlen(oobeErrorMsg));
                SetTextColor(hdc, RGB(255, 255, 255));
            }
            
            SelectObject(hdc, hSmallFont);
            TextOut(hdc, centerX - 180, centerY + 30, "请输入用户名，然后按Tab键继续", 29);
            break;
        }
            
        case 2: {
            TextOut(hdc, centerX - 150, centerY - 250, "设置密码", 8);
            SelectObject(hdc, hNormalFont);
            TextOut(hdc, centerX - 250, centerY - 200, "请为您的账户设置安全密码", 24);
            
            DrawInputBox(hdc, centerX - 200, centerY - 120, 400, 35, "密码:", password, TRUE, TRUE, "输入密码（至少4个字符）");
            DrawInputBox(hdc, centerX - 200, centerY - 50, 400, 35, "确认密码:", confirmPassword, FALSE, TRUE, "再次输入密码");
            
            // 修复：添加密码跳过按钮
            RECT skipButtonRect = {centerX - 200, centerY + 20, centerX + 200, centerY + 60};
            HBRUSH hSkipBrush = CreateSolidBrush(VISTA_ORANGE);
            FillRect(hdc, &skipButtonRect, hSkipBrush);
            DeleteObject(hSkipBrush);
    
            HPEN hSkipPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hSkipPen);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, centerX - 200, centerY + 20, centerX + 200, centerY + 60);
            SelectObject(hdc, hOldPen);
            DeleteObject(hSkipPen);
    
            SetTextColor(hdc, RGB(255, 255, 255));
            SelectObject(hdc, hNormalFont);
            TextOut(hdc, centerX - 60, centerY + 35, "跳过密码设置", 10);
            
            if (oobeError) {
                SetTextColor(hdc, VISTA_RED);
                TextOut(hdc, centerX - 180, centerY + 80, oobeErrorMsg, strlen(oobeErrorMsg));
                SetTextColor(hdc, RGB(255, 255, 255));
            }
            
            SelectObject(hdc, hSmallFont);
            TextOut(hdc, centerX - 180, centerY + 100, "请输入密码，然后按Tab键继续，或点击跳过", 38);
            break;
        }
            
        case 3: {
            TextOut(hdc, centerX - 150, centerY - 250, "计算机名称", 10);
            SelectObject(hdc, hNormalFont);
            TextOut(hdc, centerX - 250, centerY - 200, "为您的计算机设置一个名称", 24);
            
            DrawInputBox(hdc, centerX - 200, centerY - 120, 400, 35, "计算机名:", computerName, TRUE, FALSE, "输入计算机名称");
            
            // 修复：添加主题选择
            TextOut(hdc, centerX - 200, centerY - 50, "选择主题:", 8);
            DrawThemeSelector(hdc, centerX - 200, centerY - 10, 400, 80);
            
            if (oobeError) {
                SetTextColor(hdc, VISTA_RED);
                TextOut(hdc, centerX - 180, centerY + 70, oobeErrorMsg, strlen(oobeErrorMsg));
                SetTextColor(hdc, RGB(255, 255, 255));
            }
            
            SelectObject(hdc, hSmallFont);
            TextOut(hdc, centerX - 180, centerY + 100, "请输入计算机名，选择主题，然后按Enter键完成配置", 46);
            
            // 绘制完成按钮
            RECT completeButtonRect = {centerX - 80, centerY + 130, centerX + 80, centerY + 170};
            HBRUSH hCompleteBrush = CreateSolidBrush(VISTA_GREEN);
            FillRect(hdc, &completeButtonRect, hCompleteBrush);
            DeleteObject(hCompleteBrush);
    
            HPEN hCompletePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hCompletePen);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, centerX - 80, centerY + 130, centerX + 80, centerY + 170);
            SelectObject(hdc, hOldPen);
            DeleteObject(hCompletePen);
    
            SetTextColor(hdc, RGB(255, 255, 255));
            SelectObject(hdc, hNormalFont);
            TextOut(hdc, centerX - 30, centerY + 145, "完成配置", 8);
            break;
        }
    }
    
    // 绘制进度指示器
    DrawOOBEProgress(hdc, centerX, centerY);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hNormalFont);
    DeleteObject(hSmallFont);
}

// 绘制OOBE界面
void DrawOOBEInterface(HDC hdc, int centerX, int centerY) {
    RECT rect;
    GetClientRect(WindowFromDC(hdc), &rect);
    
    // 绘制背景
    DrawOOBEBackground(hdc, rect);
    
    // 绘制内容
    DrawOOBEStep(hdc, centerX, centerY);
}

// 字符串标准化（移除空格并转为小写）
void NormalizeString(char* str) {
    int i = 0, j = 0;
    while (str[i]) {
        if (str[i] != ' ') {
            str[j] = tolower(str[i]);
            j++;
        }
        i++;
    }
    str[j] = '\0';
}

// 获取文本宽度
int GetTextWidth(HDC hdc, const char* text) {
    SIZE size;
    if (GetTextExtentPoint32A(hdc, text, strlen(text), &size)) {
        return size.cx;
    }
    return 0;
}

// 检查特殊消息（520+62彩蛋 - 我爱你六二班）
void CheckSpecialMessage() {
    if (strcmp(calcDisplay, "582") == 0) {
        showSpecialMessage = TRUE;
        specialMessageScrollY = 0;
        lastScrollTime = GetTickCount();
        strcpy(guessInput, "");
    }
}

// 处理猜谜输入
void HandleGuessInput(char key) {
    if (key == 8) {
        if (strlen(guessInput) > 0) {
            guessInput[strlen(guessInput) - 1] = '\0';
        }
    } else if (key == 13) {
        char normalizedGuess[50];
        strcpy(normalizedGuess, guessInput);
        NormalizeString(normalizedGuess);
        
        if (strcmp(normalizedGuess, "huangzihan") == 0) {
            showFinalEgg = TRUE;
            showSpecialMessage = FALSE;
            specialMessageScrollY = 0;
            showGameButton = TRUE;
        } else {
            strcpy(guessInput, "");
        }
    } else if (key >= 32 && key <= 126 && strlen(guessInput) < 49) {
        int len = strlen(guessInput);
        guessInput[len] = key;
        guessInput[len + 1] = '\0';
    }
}

// 保存配置到文件
void SaveConfig() {
    FILE* file = fopen(CONFIG_FILE, "wb");
    if (file) {
        fwrite(&isFirstBoot, sizeof(BOOL), 1, file);
        fwrite(username, sizeof(char), 50, file);
        fwrite(password, sizeof(char), 50, file);
        fwrite(computerName, sizeof(char), 50, file);
        fwrite(&isActivated, sizeof(BOOL), 1, file);
        fwrite(&isTrialMode, sizeof(BOOL), 1, file);
        fwrite(&trialDaysRemaining, sizeof(int), 1, file);
        fwrite(&showGameButton, sizeof(BOOL), 2, file);
        fwrite(&currentThemeIndex, sizeof(int), 1, file);
        fclose(file);
    }
}

// 从文件加载配置
void LoadConfig() {
    FILE* file = fopen(CONFIG_FILE, "rb");
    if (file) {
        fread(&isFirstBoot, sizeof(BOOL), 1, file);
        fread(username, sizeof(char), 50, file);
        fread(password, sizeof(char), 50, file);
        fread(computerName, sizeof(char), 50, file);
        fread(&isActivated, sizeof(BOOL), 1, file);
        fread(&isTrialMode, sizeof(BOOL), 1, file);
        fread(&trialDaysRemaining, sizeof(int), 1, file);
        fread(&showGameButton, sizeof(BOOL), 2, file);
        fread(&currentThemeIndex, sizeof(int), 1, file);
        fclose(file);
    } else {
        // 设置默认值
        strcpy(username, "User");
        strcpy(password, "1234");
        strcpy(computerName, "FOS-PC");
        isFirstBoot = TRUE;
        showGameButton = FALSE;
        currentThemeIndex = 0;
    }
}

// 验证登录
BOOL VerifyLogin() {
    BOOL success = (strcmp(loginUsername, username) == 0 && strcmp(loginPassword, password) == 0);
    
    if (success) {
        // 直接播放登录音乐
        PlaySound("音乐\\登录.mp3", NULL, SND_FILENAME | SND_ASYNC);
    }
    
    return success;
}

// 获取当前日期和时间
void GetCurrentDateTime(char* dateStr, char* timeStr) {
    time_t now;
    struct tm* timeinfo;
    
    time(&now);
    timeinfo = localtime(&now);
    
    strftime(dateStr, 50, "%Y年%m月%d日", timeinfo);
    strftime(timeStr, 50, "%H:%M:%S", timeinfo);
}

// 初始化系统信息
void InitializeSystemInfo() {
    GetSystemInfo(&sysInfo);
    
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);
    
    lastSystemInfoUpdate = GetTickCount();
    UpdateSystemInfo();
    InitializeVersionInfo();
}

// 更新系统信息
void UpdateSystemInfo() {
    DWORD currentTime = GetTickCount();
    
    if (currentTime - lastSystemInfoUpdate < 5000) {
        return;
    }
    
    memoryStatus.dwLength = sizeof(memoryStatus);
    GlobalMemoryStatusEx(&memoryStatus);
    
    double totalMemMB = (double)memoryStatus.ullTotalPhys / (1024 * 1024);
    double availMemMB = (double)memoryStatus.ullAvailPhys / (1024 * 1024);
    sprintf(memoryInfo, "物理内存:                %.0f MB , %.0f MB 总计", availMemMB, totalMemMB);
    
    lastSystemInfoUpdate = currentTime;
}

// 处理OOBE输入 - 修复：添加密码跳过和主题选择处理
void HandleOOBEInput(char key) {
    switch (currentStep) {
        case 0:
            if (key == 13) {
                currentStep = 1;
                oobeError = FALSE;
            }
            break;
            
        case 1:
            if (key == 8) {
                if (strlen(username) > 0) {
                    username[strlen(username) - 1] = '\0';
                }
            } else if (key == 9 || key == 13) {
                if (ValidateOOBEInput()) {
                    currentStep = 2;
                    oobeError = FALSE;
                }
            } else if (key >= 32 && key <= 126 && strlen(username) < 49) {
                int len = strlen(username);
                username[len] = key;
                username[len + 1] = '\0';
            }
            break;
            
        case 2:
            if (key == 8) {
                if (strlen(password) > 0) {
                    password[strlen(password) - 1] = '\0';
                }
            } else if (key == 9) {
                currentStep = 3;
                oobeError = FALSE;
            } else if (key >= 32 && key <= 126 && strlen(password) < 49) {
                int len = strlen(password);
                password[len] = key;
                password[len + 1] = '\0';
            }
            break;
            
        case 3:
            if (key == 8) {
                if (strlen(computerName) > 0) {
                    computerName[strlen(computerName) - 1] = '\0';
                }
            } else if (key == 9) {
                // 修复：在主题选择步骤处理Tab键
                currentThemeIndex = (currentThemeIndex + 1) % 4;
            } else if (key == 13) {
                if (ValidateOOBEInput()) {
                    CompleteOOBE(NULL);
                }
            } else if (key >= 32 && key <= 126 && strlen(computerName) < 49) {
                int len = strlen(computerName);
                computerName[len] = key;
                computerName[len + 1] = '\0';
            }
            break;
    }
}

// 处理登录输入
void HandleLoginInput(char key) {
    if (currentStep == 0) {
        if (key == 8) {
            if (strlen(loginUsername) > 0) {
                loginUsername[strlen(loginUsername) - 1] = '\0';
            }
        } else if (key == 9) {
            currentStep = 1;
        } else if (key >= 32 && key <= 126 && strlen(loginUsername) < 49) {
            int len = strlen(loginUsername);
            loginUsername[len] = key;
            loginUsername[len + 1] = '\0';
        }
    } else if (currentStep == 1) {
        if (key == 8) {
            if (strlen(loginPassword) > 0) {
                loginPassword[strlen(loginPassword) - 1] = '\0';
            }
        } else if (key == 13) {
            if (VerifyLogin()) {
                loginError = FALSE;
                ShowDesktop(NULL);
            } else {
                loginError = TRUE;
                strcpy(loginPassword, "");
            }
        } else if (key >= 32 && key <= 126 && strlen(loginPassword) < 49) {
            int len = strlen(loginPassword);
            loginPassword[len] = key;
            loginPassword[len + 1] = '\0';
        }
    }
}

// 处理计算器输入
void HandleCalculatorInput(char key) {
    if (showFinalEgg) {
        if (key == 13 || key == 27) {
            showFinalEgg = FALSE;
            showSpecialMessage = FALSE;
            specialMessageScrollY = 0;
        }
        return;
    }
    
    if (showSpecialMessage) {
        HandleGuessInput(key);
        return;
    }
    
    if (key >= '0' && key <= '9') {
        char button[2] = {key, '\0'};
        CalculatorButtonClick(button);
    }
    else if (key == '+' || key == '-' || key == '*' || key == '/') {
        char button[2] = {key, '\0'};
        CalculatorButtonClick(button);
    }
    else if (key == 13 || key == '=') {
        CalculatorButtonClick("=");
        CheckSpecialMessage();
    }
    else if (key == 8 || key == 'c' || key == 'C') {
        CalculatorButtonClick("C");
    }
    else if (key == '.') {
        CalculatorButtonClick(".");
    }
}

// 计算器按钮点击处理
void CalculatorButtonClick(const char* button) {
    if (calcError) {
        strcpy(calcDisplay, "0");
        calcError = FALSE;
    }
    
    if (strcmp(button, "C") == 0) {
        strcpy(calcDisplay, "0");
        calcValue = 0;
        calcOperator = 0;
        calcWaitingOperand = FALSE;
        calcNewNumber = TRUE;
        calcError = FALSE;
        showSpecialMessage = FALSE;
        showFinalEgg = FALSE;
        specialMessageScrollY = 0;
    }
    else if (strcmp(button, "=") == 0) {
        if (calcOperator && !calcWaitingOperand) {
            double operand = atof(calcDisplay);
            switch (calcOperator) {
                case '+': calcValue += operand; break;
                case '-': calcValue -= operand; break;
                case '*': calcValue *= operand; break;
                case '/': 
                    if (operand != 0) {
                        calcValue /= operand;
                    } else {
                        strcpy(calcDisplay, "错误:除零");
                        calcError = TRUE;
                        return;
                    }
                    break;
            }
            if (calcValue == (int)calcValue) {
                sprintf(calcDisplay, "%d", (int)calcValue);
            } else {
                sprintf(calcDisplay, "%.6f", calcValue);
                int len = strlen(calcDisplay);
                while (len > 0 && calcDisplay[len-1] == '0') {
                    calcDisplay[len-1] = '\0';
                    len--;
                }
                if (len > 0 && calcDisplay[len-1] == '.') {
                    calcDisplay[len-1] = '\0';
                }
            }
            calcOperator = 0;
            calcWaitingOperand = TRUE;
        }
    }
    else if (strcmp(button, "+") == 0 || strcmp(button, "-") == 0 || 
             strcmp(button, "*") == 0 || strcmp(button, "/") == 0) {
        if (!calcWaitingOperand) {
            if (calcOperator) {
                CalculatorButtonClick("=");
            }
            calcValue = atof(calcDisplay);
            calcOperator = button[0];
            calcWaitingOperand = TRUE;
        }
    }
    else if (strcmp(button, ".") == 0) {
        if (calcWaitingOperand || calcNewNumber) {
            strcpy(calcDisplay, "0.");
            calcWaitingOperand = FALSE;
            calcNewNumber = FALSE;
        } else if (strchr(calcDisplay, '.') == NULL) {
            strcat(calcDisplay, ".");
        }
    }
    else {
        if (calcWaitingOperand || calcNewNumber) {
            strcpy(calcDisplay, button);
            calcWaitingOperand = FALSE;
            calcNewNumber = FALSE;
        } else {
            if (strcmp(calcDisplay, "0") == 0) {
                strcpy(calcDisplay, button);
            } else {
                strcat(calcDisplay, button);
            }
        }
    }
}

// 绘制FOS启动进度条
void DrawFOSProgressBar(HDC hdc, int x, int y, int width, int height, int progress) {
    RECT bgRect = {x, y, x + width, y + height};
    HBRUSH hBgBrush = CreateSolidBrush(RGB(30, 30, 30));
    FillRect(hdc, &bgRect, hBgBrush);
    DeleteObject(hBgBrush);
    
    if (progress > 0) {
        RECT progressRect = {x, y, x + (width * progress / 100), y + height};
        HBRUSH hProgressBrush = CreateSolidBrush(RGB(0, 255, 0));
        FillRect(hdc, &progressRect, hProgressBrush);
        DeleteObject(hProgressBrush);
    }
    
    HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, x, y, x + width, y + height);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
}

// 绘制FOS启动画面
void DrawFOSBetaAnimation(HDC hdc, RECT rect) {
    HBRUSH hBlackBrush = CreateSolidBrush(BOOT_BLACK);
    FillRect(hdc, &rect, hBlackBrush);
    DeleteObject(hBlackBrush);
    
    int centerX = (rect.right - rect.left) / 2;
    int centerY = (rect.bottom - rect.top) / 2;
    
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 255, 0));
    
    HFONT hTitleFont = CreateFont(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, FIXED_PITCH, "Terminal");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    TextOut(hdc, centerX - 80, centerY - 150, "FOS", 3);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    
    HFONT hVersionFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, FIXED_PITCH, "Terminal");
    hOldFont = (HFONT)SelectObject(hdc, hVersionFont);
    
    char versionText[100];
    sprintf(versionText, "fOS %d.%d Build %d", MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION);
    TextOut(hdc, centerX - 120, centerY - 80, versionText, strlen(versionText));
    TextOut(hdc, centerX - 200, centerY - 40, "Professional Edition(music plus!!!)", 35);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hVersionFont);
    
    HFONT hInfoFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, FIXED_PITCH, "Terminal");
    hOldFont = (HFONT)SelectObject(hdc, hInfoFont);
    
    TextOut(hdc, centerX - 180, centerY + 10, "未泄漏内部测试版本，version 399", 31);
    TextOut(hdc, centerX - 180, centerY + 40, "本软件版权归Jellyfish Studio所有，严禁二次分发！", 48);
    TextOut(hdc, centerX - 180, centerY + 70, "启动系统中……", 14);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hInfoFont);
    
    int progressBarWidth = 400;
    int progressBarHeight = 20;
    DrawFOSProgressBar(hdc, centerX - progressBarWidth/2, centerY + 120, progressBarWidth, progressBarHeight, fosProgressValue);
    
    HFONT hProgressFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, FIXED_PITCH, "Terminal");
    hOldFont = (HFONT)SelectObject(hdc, hProgressFont);
    
    char progressText[50];
    sprintf(progressText, "正在准备启动 FOS... %d%%", fosProgressValue);
    TextOut(hdc, centerX - 80, centerY + 150, progressText, strlen(progressText));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hProgressFont);
    
    // 版权信息和致敬文字部分
    HFONT hCopyrightFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, FIXED_PITCH, "Terminal");
    hOldFont = (HFONT)SelectObject(hdc, hCopyrightFont);
    
    TextOut(hdc, centerX - 150, rect.bottom - 50, "(C) 2025 FOS 开发组. 保留所有权利，严禁二次分发！", 49);
    TextOut(hdc, centerX - 170, rect.bottom - 30, "Professional Edition.To all versions from fos0.0 to 1.0 Build 400", 65);

    // 恢复原始设置
    SelectObject(hdc, hOldFont);
    SetTextColor(hdc, RGB(0, 255, 0));
    DeleteObject(hCopyrightFont);
}

// 绘制开机动画进度条
void DrawBootProgressBar(HDC hdc, int x, int y, int width, int height, int progress) {
    RECT bgRect = {x, y, x + width, y + height};
    HBRUSH hBgBrush = CreateSolidBrush(RGB(50, 50, 50));
    FillRect(hdc, &bgRect, hBgBrush);
    DeleteObject(hBgBrush);
    
    if (progress > 0) {
        RECT progressRect = {x, y, x + (width * progress / 100), y + height};
        HBRUSH hProgressBrush = CreateSolidBrush(RGB(0, 120, 215));
        FillRect(hdc, &progressRect, hProgressBrush);
        DeleteObject(hProgressBrush);
    }
    
    HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, x, y, x + width, y + height);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
}

// 绘制关机动画进度条
void DrawShutdownProgressBar(HDC hdc, int x, int y, int width, int height, int progress) {
    RECT bgRect = {x, y, x + width, y + height};
    HBRUSH hBgBrush = CreateSolidBrush(RGB(30, 30, 30));
    FillRect(hdc, &bgRect, hBgBrush);
    DeleteObject(hBgBrush);
    
    if (progress > 0) {
        RECT progressRect = {x, y, x + (width * progress / 100), y + height};
        HBRUSH hProgressBrush = CreateSolidBrush(VISTA_ORANGE);
        FillRect(hdc, &progressRect, hProgressBrush);
        DeleteObject(hProgressBrush);
    }
    
    HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(80, 80, 80));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, x, y, x + width, y + height);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
}

// 绘制开机动画
void DrawBootAnimation(HDC hdc, RECT rect) {
    HBRUSH hBlackBrush = CreateSolidBrush(BOOT_BLACK);
    FillRect(hdc, &rect, hBlackBrush);
    DeleteObject(hBlackBrush);
    
    int centerX = (rect.right - rect.left) / 2;
    int centerY = (rect.bottom - rect.top) / 2;
    
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    
    HFONT hLogoFont = CreateFont(72, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hLogoFont);
    
    TextOut(hdc, centerX - 80, centerY - 100, "f系统", 5);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hLogoFont);
    
    HFONT hVersionFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    hOldFont = (HFONT)SelectObject(hdc, hVersionFont);
    
    char versionText[100];
    sprintf(versionText, "版本 %d.%d (Build %d)", MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION);
    TextOut(hdc, centerX - 80, centerY - 10, versionText, strlen(versionText));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hVersionFont);
    
    int progressBarWidth = 400;
    int progressBarHeight = 20;
    DrawBootProgressBar(hdc, centerX - progressBarWidth/2, centerY + 50, progressBarWidth, progressBarHeight, progressValue);
    
    HFONT hLoadingFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    hOldFont = (HFONT)SelectObject(hdc, hLoadingFont);
    
    char loadingText[50];
    sprintf(loadingText, "正在启动... %d%%", progressValue);
    TextOut(hdc, centerX - 50, centerY + 80, loadingText, strlen(loadingText));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hLoadingFont);
}

// 绘制关机动画
void DrawShutdownAnimation(HDC hdc, RECT rect) {
    HBRUSH hBlackBrush = CreateSolidBrush(BOOT_BLACK);
    FillRect(hdc, &rect, hBlackBrush);
    DeleteObject(hBlackBrush);
    
    int centerX = (rect.right - rect.left) / 2;
    int centerY = (rect.bottom - rect.top) / 2;
    
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(255, 255, 255));
    
    HFONT hLogoFont = CreateFont(72, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hLogoFont);
    
    TextOut(hdc, centerX - 80, centerY - 100, "f系统", 5);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hLogoFont);
    
    HFONT hVersionFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    hOldFont = (HFONT)SelectObject(hdc, hVersionFont);
    
    TextOut(hdc, centerX - 40, centerY - 10, "正在关机...", 11);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hVersionFont);
    
    int progressBarWidth = 400;
    int progressBarHeight = 20;
    DrawShutdownProgressBar(hdc, centerX - progressBarWidth/2, centerY + 50, progressBarWidth, progressBarHeight, shutdownProgress);
    
    HFONT hLoadingFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    hOldFont = (HFONT)SelectObject(hdc, hLoadingFont);
    
    char loadingText[50];
    sprintf(loadingText, "正在保存设置... %d%%", shutdownProgress);
    TextOut(hdc, centerX - 70, centerY + 80, loadingText, strlen(loadingText));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hLoadingFont);
}

// 创建背景位图
void CreateBackgroundBitmap(HWND hwnd) {
    if (hBackgroundBitmap) {
        DeleteObject(hBackgroundBitmap);
    }
    
    RECT rect;
    GetClientRect(hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;
    
    HDC hdc = GetDC(hwnd);
    HDC hMemDC = CreateCompatibleDC(hdc);
    hBackgroundBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackgroundBitmap);
    
    for (int y = 0; y < height; y++) {
        double ratio = (double)y / height;
        int r = (int)(0 + ratio * 64);
        int g = (int)(48 + ratio * 128);
        int b = (int)(128 + ratio * 208);
        
        COLORREF color = RGB(r, g, b);
        HBRUSH hBrush = CreateSolidBrush(color);
        RECT lineRect = {0, y, width, y + 1};
        FillRect(hMemDC, &lineRect, hBrush);
        DeleteObject(hBrush);
    }
    
    SelectObject(hMemDC, hOldBitmap);
    DeleteDC(hMemDC);
    ReleaseDC(hwnd, hdc);
}

// 绘制背景
void DrawVistaBackground(HDC hdc, RECT rect) {
    if (hBackgroundBitmap) {
        HDC hMemDC = CreateCompatibleDC(hdc);
        HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBackgroundBitmap);
        
        BitBlt(hdc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
               hMemDC, 0, 0, SRCCOPY);
        
        SelectObject(hMemDC, hOldBitmap);
        DeleteDC(hMemDC);
    } else {
        HBRUSH hBrush = CreateSolidBrush(VISTA_BLUE);
        FillRect(hdc, &rect, hBrush);
        DeleteObject(hBrush);
    }
}

// 绘制Vista风格进度条
void DrawVistaProgressBar(HDC hdc, int x, int y, int width, int height, int progress) {
    RECT bgRect = {x, y, x + width, y + height};
    HBRUSH hBgBrush = CreateSolidBrush(RGB(220, 220, 220));
    FillRect(hdc, &bgRect, hBgBrush);
    DeleteObject(hBgBrush);
    
    if (progress > 0) {
        RECT progressRect = {x + 2, y + 2, x + 2 + ((width - 4) * progress / 100), y + height - 2};
        HBRUSH hProgressBrush = CreateSolidBrush(VISTA_GREEN);
        FillRect(hdc, &progressRect, hProgressBrush);
        DeleteObject(hProgressBrush);
    }
    
    HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, x, y, x + width, y + height);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
}

// 绘制登录界面
void DrawLoginInterface(HDC hdc, int centerX, int centerY) {
    HFONT hTitleFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hNormalFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hSmallFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    // 在左上角绘制守慧吉祥物
    DrawShouhuiMascot(hdc, 50, 50, 60);
    
    TextOut(hdc, centerX - 100, centerY - 200, "f系统1.0", 8);
    
    SelectObject(hdc, hNormalFont);
    TextOut(hdc, centerX - 80, centerY - 150, "用户会话登录", 12);
    
    DrawInputBox(hdc, centerX - 200, centerY - 80, 400, 35, "用户名:", loginUsername, currentStep == 0, FALSE, "输入用户名");
    DrawInputBox(hdc, centerX - 200, centerY - 10, 400, 35, "密码:", loginPassword, currentStep == 1, TRUE, "输入密码");
    
    if (loginError) {
        SetTextColor(hdc, VISTA_RED);
        TextOut(hdc, centerX - 100, centerY + 40, "用户名或密码错误，请重试", 24);
        SetTextColor(hdc, RGB(255, 255, 255));
    }
    
    SelectObject(hdc, hSmallFont);
    if (currentStep == 0) {
        TextOut(hdc, centerX - 180, centerY + 70, "请输入用户名，然后按Tab键继续", 29);
    } else if (currentStep == 1) {
        TextOut(hdc, centerX - 180, centerY + 70, "请输入密码，然后按Enter键登录", 29);
    }
    
    RECT buttonRect = {centerX - 80, centerY + 110, centerX + 80, centerY + 150};
    HBRUSH hButtonBrush = CreateSolidBrush(VISTA_GREEN);
    FillRect(hdc, &buttonRect, hButtonBrush);
    DeleteObject(hButtonBrush);
    
    HPEN hButtonPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hButtonPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX - 80, centerY + 110, centerX + 80, centerY + 150);
    SelectObject(hdc, hOldPen);
    DeleteObject(hButtonPen);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SelectObject(hdc, hNormalFont);
    TextOut(hdc, centerX - 20, centerY + 125, "登录", 4);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hNormalFont);
    DeleteObject(hSmallFont);
}

// 绘制粉色桌面
void DrawDesktop(HDC hdc, RECT rect) {
    HBRUSH hDesktopBrush = CreateSolidBrush(themes[currentThemeIndex].desktopColor);
    FillRect(hdc, &rect, hDesktopBrush);
    DeleteObject(hDesktopBrush);
    
    // 在桌面右下角绘制守慧吉祥物
    DrawShouhuiMascot(hdc, rect.right - 100, rect.bottom - 100, 80);
    
    HFONT hTitleFont = CreateFont(48, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hNormalFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hSmallFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    
    SetTextColor(hdc, RGB(80, 80, 80));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    int centerX = (rect.right - rect.left) / 2;
    int centerY = (rect.bottom - rect.top) / 2;
    
    TextOut(hdc, centerX - 200, centerY - 100, "欢迎回来！", 10);
    
    SelectObject(hdc, hNormalFont);
    char welcomeMsg[100];
    sprintf(welcomeMsg, "欢迎您，%s", username);
    TextOut(hdc, centerX - 100, centerY - 30, welcomeMsg, strlen(welcomeMsg));
    
    char versionMsg[100];
    sprintf(versionMsg, "f系统 %d.%d Build %d", MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION);
    TextOut(hdc, centerX - 150, centerY + 30, versionMsg, strlen(versionMsg));
    
    // 显示激活状态
    SelectObject(hdc, hSmallFont);
    if (isActivated) {
        SetTextColor(hdc, VISTA_GREEN);
        TextOut(hdc, centerX - 80, centerY + 80, "? 系统已激活", 11);
    } else if (isTrialMode) {
        SetTextColor(hdc, VISTA_ORANGE);
        char trialMsg[50];
        sprintf(trialMsg, "? 试用版 (%d天剩余)", trialDaysRemaining);
        TextOut(hdc, centerX - 100, centerY + 80, trialMsg, strlen(trialMsg));
    } else {
        SetTextColor(hdc, VISTA_RED);
        TextOut(hdc, centerX - 80, centerY + 80, "? 未激活", 9);
    }
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hNormalFont);
    DeleteObject(hSmallFont);
}

// 绘制任务栏
void DrawTaskbar(HDC hdc, RECT rect) {
    int taskbarHeight = 60;
    RECT taskbarRect = {0, 0, rect.right, taskbarHeight};
    
    HBRUSH hTaskbarBrush = CreateSolidBrush(themes[currentThemeIndex].taskbarColor);
    FillRect(hdc, &taskbarRect, hTaskbarBrush);
    DeleteObject(hTaskbarBrush);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hTaskbarFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTaskbarFont);
    
    char taskbarText[100];
    sprintf(taskbarText, "f系统%d.%d | 用户: %s ", MAJOR_VERSION, MINOR_VERSION, username);
    TextOut(hdc, 10, 15, taskbarText, strlen(taskbarText));
    
    SetTextColor(hdc, RGB(255, 255, 255));
    
    // 显示时间
    char dateStr[50], timeStr[50];
    GetCurrentDateTime(dateStr, timeStr);
    
    char datetimeStr[100];
    sprintf(datetimeStr, "(c)版权归Jellyfish Studio所有（2020-2030），严禁二次分发！ | %s %s", dateStr, timeStr);
    
    int textX = rect.right - GetTextWidth(hdc, datetimeStr) - 10;
    
    TextOut(hdc, textX, 15, datetimeStr, strlen(datetimeStr));
    
    // 修复：统一的任务栏按钮位置计算
    int buttonWidth = 100;
    int buttonHeight = 40;
    int buttonY = 10;
    int buttonStartX = 200;
    int buttonSpacing = 110;
    int buttonIndex = 0;
    
    // 显示桌面按钮
    int desktopX = buttonStartX + buttonSpacing * buttonIndex++;
    DrawTaskbarButton(hdc, desktopX, buttonY, buttonWidth, buttonHeight, "显示桌面", 
                     isDesktopHovered, isDesktopPressed, FALSE);
    
    // 文件管理器按钮
    int fileX = buttonStartX + buttonSpacing * buttonIndex++;
    DrawTaskbarButton(hdc, fileX, buttonY, buttonWidth, buttonHeight, "文件管理器", 
                     isFileManagerHovered, isFileManagerPressed, showFileManager);
    
    // 计算器按钮
    int calcX = buttonStartX + buttonSpacing * buttonIndex++;
    DrawTaskbarButton(hdc, calcX, buttonY, buttonWidth, buttonHeight, "计 算 器", 
                     isCalcHovered, isCalcPressed, showCalculator);
    
    // 系统信息按钮
    int sysinfoX = buttonStartX + buttonSpacing * buttonIndex++;
    DrawTaskbarButton(hdc, sysinfoX, buttonY, buttonWidth, buttonHeight, "系统信息", 
                     isSysInfoHovered, isSysInfoPressed, showSystemInfo);
    
    // 设置按钮（专业版功能）
    if (isActivated) {
        int settingsX = buttonStartX + buttonSpacing * buttonIndex++;
        DrawTaskbarButton(hdc, settingsX, buttonY, buttonWidth, buttonHeight, "系统设置", 
                         isSettingsHovered, isSettingsPressed, showSettings);
    }
    
    // 水母游戏按钮（猜对作者名字后显示）
    if (showGameButton) {
        int gameX = buttonStartX + buttonSpacing * buttonIndex++;
        DrawTaskbarButton(hdc, gameX, buttonY, buttonWidth, buttonHeight, "水母游戏", 
                         isGameHovered, isGamePressed, showGameEgg);
    }
    
    // Bug报告按钮（专业版功能）
    if (isActivated) {
        int bugX = buttonStartX + buttonSpacing * buttonIndex++;
        DrawTaskbarButton(hdc, bugX, buttonY, buttonWidth, buttonHeight, "Bug报告", 
                         isBugHovered, isBugPressed, showBugReporter);
    }
    
    // Bug列表按钮（专业版功能）
    if (isActivated) {
        int bugListX = buttonStartX + buttonSpacing * buttonIndex++;
        DrawTaskbarButton(hdc, bugListX, buttonY, buttonWidth, buttonHeight, "Bug列表", 
                         isBugListHovered, isBugListPressed, showBugList);
    }
	// 日历按钮
	int calendarX = buttonStartX + buttonSpacing * buttonIndex++;
	DrawTaskbarButton(hdc, calendarX, buttonY, buttonWidth, buttonHeight, "日 历", 
                 isCalendarHovered, isCalendarPressed, showCalendar);

	// 关机按钮
	int shutdownX = buttonStartX + buttonSpacing * buttonIndex++;
	DrawTaskbarButton(hdc, shutdownX, buttonY, buttonWidth, buttonHeight, "关 机", 
                 isShutdownHovered, isShutdownPressed, showShutdown);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTaskbarFont);
}

// 绘制任务栏按钮
void DrawTaskbarButton(HDC hdc, int x, int y, int width, int height, const char* text, 
                      BOOL isHovered, BOOL isPressed, BOOL isActive) {
    COLORREF buttonColor;
    
    if (isPressed) {
        buttonColor = TASKBAR_BUTTON_PRESS;
    } else if (isHovered) {
        buttonColor = TASKBAR_BUTTON_HOVER;
    } else if (isActive) {
        if (strcmp(text, "水母游戏") == 0) {
            buttonColor = GAME_BUTTON;
        } else if (strcmp(text, "系统设置") == 0) {
            buttonColor = themes[currentThemeIndex].accentColor;
        } else {
            buttonColor = themes[currentThemeIndex].buttonColor;
        }
    } else {
        buttonColor = themes[currentThemeIndex].buttonColor;
    }
    
    RECT buttonRect = {x, y, x + width, y + height};
    HBRUSH hButtonBrush = CreateSolidBrush(buttonColor);
    FillRect(hdc, &buttonRect, hButtonBrush);
    DeleteObject(hButtonBrush);
    
    HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, x, y, x + width, y + height);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hButtonFont = CreateFont(14, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hButtonFont);
    
    int textX = x + (width - GetTextWidth(hdc, text)) / 2;
    int textY = y + (height - 20) / 2;
    
    TextOut(hdc, textX, textY, text, strlen(text));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hButtonFont);
}

// 绘制计算器按钮
void DrawCalculatorButton(HDC hdc, int x, int y, int width, int height, const char* text, BOOL isOperator) {
    COLORREF buttonColor = isOperator ? CALC_OPERATOR : CALC_BUTTON;
    
    RECT buttonRect = {x, y, x + width, y + height};
    HBRUSH hButtonBrush = CreateSolidBrush(buttonColor);
    FillRect(hdc, &buttonRect, hButtonBrush);
    DeleteObject(hButtonBrush);
    
    HPEN hBorderPen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, x, y, x + width, y + height);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hButtonFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hButtonFont);
    
    int textX = x + (width - GetTextWidth(hdc, text)) / 2;
    int textY = y + (height - 20) / 2;
    
    TextOut(hdc, textX, textY, text, strlen(text));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hButtonFont);
}

// 绘制最终彩蛋界面
void DrawFinalEgg(HDC hdc, int centerX, int centerY) {
    RECT finalRect = {centerX - 200, centerY - 250, centerX + 200, centerY + 250};
    
    HBRUSH hFinalBgBrush = CreateSolidBrush(FINAL_BG);
    FillRect(hdc, &finalRect, hFinalBgBrush);
    DeleteObject(hFinalBgBrush);
    
    HPEN hBorderPen = CreatePen(PS_SOLID, 3, RGB(255, 150, 0));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX - 200, centerY - 250, centerX + 200, centerY + 250);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    // 在右上角绘制守慧吉祥物
    DrawShouhuiMascot(hdc, centerX + 120, centerY - 220, 60);
    
    SetTextColor(hdc, RGB(200, 80, 0));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hTitleFont = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    HFONT hContentFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
    
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    TextOut(hdc, centerX - 180, centerY - 220, "恭喜你！", 8);
    
    SelectObject(hdc, hContentFont);
    
    int textY = centerY - 170;
    TextOut(hdc, centerX - 180, textY, "你猜对了！我就是", 16);
    textY += 40;
    
    // 显示网名
    HFONT hNameFont = CreateFont(24, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    SelectObject(hdc, hNameFont);
    TextOut(hdc, centerX - 100, textY, "黄梓涵", 6);
    SelectObject(hdc, hContentFont);
    
    textY += 50;
    TextOut(hdc, centerX - 180, textY, "作为六二班的电教委员，我用心制作了", 34);
    textY += 30;
    TextOut(hdc, centerX - 180, textY, "这个系统来纪念我们美好的小学时光。", 34);
    textY += 40;
    TextOut(hdc, centerX - 180, textY, "就像水母在海洋中自由探索，", 26);
    textY += 30;
    TextOut(hdc, centerX - 180, textY, "我用技术在数字世界里记录我们的友谊。", 36);
    textY += 40;
    TextOut(hdc, centerX - 180, textY, "感谢六二班的每一位老师和同学！", 30);
    textY += 40;
    
    // 特别提示：水母游戏按钮已解锁
    SetTextColor(hdc, RGB(100, 100, 200));
    TextOut(hdc, centerX - 180, textY, "特别提示：水母游戏按钮已解锁！", 28);
    textY += 30;
    TextOut(hdc, centerX - 180, textY, "返回桌面开始游戏吧！", 20);
    textY += 40;
    
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hContentFont);
    DeleteObject(hNameFont);
}

// 绘制计算器界面
void DrawCalculator(HDC hdc, int centerX, int centerY) {
    RECT calcRect = {centerX - 200, centerY - 250, centerX + 200, centerY + 250};
    
    if (showFinalEgg) {
        DrawFinalEgg(hdc, centerX, centerY);
        return;
    }
    
    if (showSpecialMessage) {
        // 自动滚动逻辑
        DWORD currentTime = GetTickCount();
        if (currentTime - lastScrollTime > 1) {
            specialMessageScrollY-=10;
            lastScrollTime = currentTime;
            
            if (specialMessageScrollY < -1200) {
                specialMessageScrollY = 0;
            }
        }
        
        // 显示特殊消息背景
        HBRUSH hSpecialBgBrush = CreateSolidBrush(SPECIAL_BG);
        FillRect(hdc, &calcRect, hSpecialBgBrush);
        DeleteObject(hSpecialBgBrush);
        
        HPEN hBorderPen = CreatePen(PS_SOLID, 3, RGB(255, 100, 150));
        HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, centerX - 200, centerY - 250, centerX + 200, centerY + 250);
        SelectObject(hdc, hOldPen);
        DeleteObject(hBorderPen);
        
        // 在右上角绘制守慧吉祥物
        DrawShouhuiMascot(hdc, centerX + 120, centerY - 220, 50);
        
        SetTextColor(hdc, RGB(200, 50, 100));
        SetBkMode(hdc, TRANSPARENT);
        
        // 创建滚动区域
        HRGN hClipRegion = CreateRectRgn(centerX - 190, centerY - 240, centerX + 190, centerY + 240);
        SelectClipRgn(hdc, hClipRegion);
        
        // 绘制滚动内容
        int textY = centerY - 240 + specialMessageScrollY;
        
        // 绘制标题
        HFONT hTitleFont = CreateFont(28, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
        
        TextOut(hdc, centerX - 150, textY, "六二班毕业纪念", 14);
        textY += 50;
        
        // 绘制班级信息
        HFONT hClassFont = CreateFont(22, 0, 0, 0, FW_BOLD, TRUE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
        SelectObject(hdc, hClassFont);
        
        TextOut(hdc, centerX - 60, textY, "520 + 62 = 582", 14);
        textY += 40;
        TextOut(hdc, centerX - 80, textY, "我爱你 六二班", 13);
        textY += 50;
        
        // 绘制正文
        HFONT hContentFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
        SelectObject(hdc, hContentFont);
        
        TextOut(hdc, centerX - 180, textY, "六年时光，转瞬即逝，感谢有你们！", 32);
        textY += 40;
        
        // 绘制任课老师
        HFONT hSmallFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "楷体");
        SelectObject(hdc, hSmallFont);
        
        TextOut(hdc, centerX - 180, textY, "任课老师:", 9);
        textY += 30;
        
        // 老师名单
        const char* teachers[] = {
            "潘  建 - 语文",
            "李婷婷 - 数学", 
            "王静婷 - 科学",
            "陈  汭 - 美术",
            "马淮娜 - 英语",
            "张  鑫 - 音乐",
            "刘  洋 - 体育",
            "吴兴霞 - 信息技术"
        };
        
        for (int i = 0; i < 8; i++) {
            TextOut(hdc, centerX - 170, textY, teachers[i], strlen(teachers[i]));
            textY += 25;
        }
        
        textY += 20;
        
        // 绘制同学名单
        SelectObject(hdc, hContentFont);
        TextOut(hdc, centerX - 180, textY, "亲爱的同学们:", 13);
        textY += 30;
        
        SelectObject(hdc, hSmallFont);
        
        // 同学名单
        const char* classmates[] = {
            "程思诚", "刘沈阳", "马槿荣", "鲍瑞璋", "吴子敬", "何家灏", "张紫辰", "方宸沣",
            "黄俊赫", "徐尹乐", "颜律恒", "丁思朗", "李辰逸", "费子尚", "王  森", "夏文豪",
            "江俊序", "程骏楠", "董正宇", "陈宇彬", "杨锦岳", "郭光鑫", "方弘毅",
            "刘美铄", "陈锦意", "金秋宜", "宋安琪", "章睦瑶", "李梦琪", "刘菁涵", "贾良睿",
            "胡乐艳", "陈琦钰", "陈  夕", "郝梓萱", "徐子淇", "柏钰涵", "张梓桐", "王诗诗",
            "姜涵月", "程昭檬", "杨晟熠", "袁翊宸", "刘雨棠", "刘思远", "马依一", "柏  典",
            "饶铭宇", "      " 
        };
        
        int totalClassmates = 49;
        int leftColX = centerX - 170;
        int rightColX = centerX + 10;
        int currentColY = textY;
        
        // 第一列（前24个同学）
        for (int i = 0; i < 24; i++) {
            TextOut(hdc, leftColX, currentColY, classmates[i], strlen(classmates[i]));
            currentColY += 20;
        }
        
        // 第二列（剩余同学）
        currentColY = textY;
        for (int i = 24; i < totalClassmates; i++) {
            TextOut(hdc, rightColX, currentColY, classmates[i], strlen(classmates[i]));
            currentColY += 20;
        }
        
        textY = currentColY + 30;
        
        // 绘制祝福语
        SelectObject(hdc, hContentFont);
        TextOut(hdc, centerX - 180, textY, "愿我们的友谊地久天长！", 22);
        textY += 50;
        
        // 绘制猜谜部分
        TextOut(hdc, centerX - 180, textY, "猜猜这个系统的开发者是谁？", 26);
        textY += 30;
        TextOut(hdc, centerX - 180, textY, "提示：似乎刚才的名单中少了谁（写出拼音）", 40);
        textY += 40;
        TextOut(hdc, centerX - 180, textY, "请输入答案：", 12);
        textY += 30;
        
        // 绘制输入框
        RECT inputRect = {centerX - 180, textY, centerX + 180, textY + 35};
        HBRUSH hInputBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect(hdc, &inputRect, hInputBrush);
        DeleteObject(hInputBrush);
        
        HPEN hInputPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
        hOldPen = (HPEN)SelectObject(hdc, hInputPen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, centerX - 180, textY, centerX + 180, textY + 35);
        SelectObject(hdc, hOldPen);
        DeleteObject(hInputPen);
        
        SetTextColor(hdc, RGB(0, 0, 0));
        TextOut(hdc, centerX - 170, textY + 8, guessInput, strlen(guessInput));
        
        // 绘制闪烁的光标
        static DWORD lastBlink = GetTickCount();
        static BOOL showCursor = TRUE;
        
        if (GetTickCount() - lastBlink > 500) {
            showCursor = !showCursor;
            lastBlink = GetTickCount();
        }
        
        if (showCursor) {
            int textWidth = GetTextWidth(hdc, guessInput);
            MoveToEx(hdc, centerX - 170 + textWidth, textY + 5, NULL);
            LineTo(hdc, centerX - 170 + textWidth, textY + 30);
        }
        
        textY += 60;
        SetTextColor(hdc, RGB(100, 100, 100));
        TextOut(hdc, centerX - 180, textY, "按Enter键提交答案", 17);
        
        // 恢复裁剪区域
        SelectClipRgn(hdc, NULL);
        DeleteObject(hClipRegion);
        
        SelectObject(hdc, hOldFont);
        DeleteObject(hTitleFont);
        DeleteObject(hClassFont);
        DeleteObject(hContentFont);
        DeleteObject(hSmallFont);
        
        return;
    }
    
    // 正常计算器界面
    HBRUSH hCalcBgBrush = CreateSolidBrush(CALC_BG);
    FillRect(hdc, &calcRect, hCalcBgBrush);
    DeleteObject(hCalcBgBrush);
    
    HPEN hBorderPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX - 200, centerY - 250, centerX + 200, centerY + 250);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    // 在右上角绘制守慧吉祥物
    DrawShouhuiMascot(hdc, centerX + 150, centerY - 220, 40);
    
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hTitleFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    TextOut(hdc, centerX - 50, centerY - 230, "计算器", 6);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    
    RECT displayRect = {centerX - 180, centerY - 190, centerX + 180, centerY - 140};
    HBRUSH hDisplayBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &displayRect, hDisplayBrush);
    DeleteObject(hDisplayBrush);
    
    HPEN hDisplayPen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
    hOldPen = (HPEN)SelectObject(hdc, hDisplayPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX - 180, centerY - 190, centerX + 180, centerY - 140);
    SelectObject(hdc, hOldPen);
    DeleteObject(hDisplayPen);
    
    HFONT hDisplayFont = CreateFont(24, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    hOldFont = (HFONT)SelectObject(hdc, hDisplayFont);
    
    SetTextColor(hdc, calcError ? RGB(255, 0, 0) : RGB(0, 0, 0));
    TextOut(hdc, centerX - 170, centerY - 185, calcDisplay, strlen(calcDisplay));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hDisplayFont);
    
    int buttonWidth = 70;
    int buttonHeight = 40;
    int startX = centerX - 165;
    int startY = centerY - 120;
    
    DrawCalculatorButton(hdc, startX, startY, buttonWidth, buttonHeight, "7", FALSE);
    DrawCalculatorButton(hdc, startX + 80, startY, buttonWidth, buttonHeight, "8", FALSE);
    DrawCalculatorButton(hdc, startX + 160, startY, buttonWidth, buttonHeight, "9", FALSE);
    DrawCalculatorButton(hdc, startX + 240, startY, buttonWidth, buttonHeight, "+", TRUE);
    
    DrawCalculatorButton(hdc, startX, startY + 50, buttonWidth, buttonHeight, "4", FALSE);
    DrawCalculatorButton(hdc, startX + 80, startY + 50, buttonWidth, buttonHeight, "5", FALSE);
    DrawCalculatorButton(hdc, startX + 160, startY + 50, buttonWidth, buttonHeight, "6", FALSE);
    DrawCalculatorButton(hdc, startX + 240, startY + 50, buttonWidth, buttonHeight, "-", TRUE);
    
    DrawCalculatorButton(hdc, startX, startY + 100, buttonWidth, buttonHeight, "1", FALSE);
    DrawCalculatorButton(hdc, startX + 80, startY + 100, buttonWidth, buttonHeight, "2", FALSE);
    DrawCalculatorButton(hdc, startX + 160, startY + 100, buttonWidth, buttonHeight, "3", FALSE);
    DrawCalculatorButton(hdc, startX + 240, startY + 100, buttonWidth, buttonHeight, "*", TRUE);
    
    DrawCalculatorButton(hdc, startX, startY + 150, buttonWidth, buttonHeight, "0", FALSE);
    DrawCalculatorButton(hdc, startX + 80, startY + 150, buttonWidth, buttonHeight, "C", TRUE);
    DrawCalculatorButton(hdc, startX + 160, startY + 150, buttonWidth, buttonHeight, "=", TRUE);
    DrawCalculatorButton(hdc, startX + 240, startY + 150, buttonWidth, buttonHeight, "/", TRUE);
    
    // 添加小数点按钮
    DrawCalculatorButton(hdc, startX, startY + 200, buttonWidth, buttonHeight, ".", FALSE);
}

// 绘制系统信息窗口
void DrawSystemInfo(HDC hdc, int centerX, int centerY) {
    RECT sysinfoRect = {centerX - 300, centerY - 200, centerX + 300, centerY + 200};
    
    // 绘制窗口背景（XP蓝色渐变）
    for (int y = sysinfoRect.top; y < sysinfoRect.bottom; y++) {
        double ratio = (double)(y - sysinfoRect.top) / (sysinfoRect.bottom - sysinfoRect.top);
        int r = (int)(185 + ratio * 70);
        int g = (int)(209 + ratio * 46);
        int b = (int)(234 + ratio * 21);
        
        COLORREF color = RGB(r, g, b);
        HPEN hPen = CreatePen(PS_SOLID, 1, color);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hPen);
        MoveToEx(hdc, sysinfoRect.left, y, NULL);
        LineTo(hdc, sysinfoRect.right, y);
        SelectObject(hdc, hOldPen);
        DeleteObject(hPen);
    }
    
    // 绘制窗口边框
    HPEN hBorderPen = CreatePen(PS_SOLID, 2, XP_BLUE);
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, sysinfoRect.left, sysinfoRect.top, sysinfoRect.right, sysinfoRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    // 绘制标题栏
    RECT titleRect = {sysinfoRect.left, sysinfoRect.top, sysinfoRect.right, sysinfoRect.top + 30};
    HBRUSH hTitleBrush = CreateSolidBrush(XP_BLUE);
    FillRect(hdc, &titleRect, hTitleBrush);
    DeleteObject(hTitleBrush);
    
    // 在标题栏左侧绘制守慧吉祥物
    DrawShouhuiMascot(hdc, sysinfoRect.left + 10, sysinfoRect.top - 5, 40);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hTitleFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    TextOut(hdc, centerX - 80, sysinfoRect.top + 5, "fOS 系统信息", 11);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    
    // 绘制内容区域
    HFONT hContentFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    hOldFont = (HFONT)SelectObject(hdc, hContentFont);
    
    SetTextColor(hdc, RGB(0, 0, 0));
    
    // 绘制系统信息内容
    int textY = sysinfoRect.top + 50;
    TextOut(hdc, centerX - 250, textY, "操作系统:", 8);
    TextOut(hdc, centerX - 150, textY, osVersion, strlen(osVersion));
    textY += 30;
    
    TextOut(hdc, centerX - 250, textY, "处理器:", 7);
    TextOut(hdc, centerX - 150, textY, cpuInfo, strlen(cpuInfo));
    textY += 30;
    
    TextOut(hdc, centerX - 250, textY, memoryInfo, strlen(memoryInfo));
    textY += 30;
    
    TextOut(hdc, centerX - 250, textY, "系统架构:", 8);
    TextOut(hdc, centerX - 150, textY, "32位", 4);
    textY += 30;
    
    TextOut(hdc, centerX - 250, textY, "版本信息:", 8);
    TextOut(hdc, centerX - 150, textY, buildInfo, strlen(buildInfo));
    textY += 30;
    
    TextOut(hdc, centerX - 250, textY, "注册用户:", 8);
    TextOut(hdc, centerX - 150, textY, username, strlen(username));
    textY += 30;
    
    TextOut(hdc, centerX - 250, textY, "计算机名:", 8);
    TextOut(hdc, centerX - 150, textY, computerName, strlen(computerName));
    textY += 30;
    
    // 显示激活状态
    TextOut(hdc, centerX - 250, textY, "激活状态:", 8);
    if (isActivated) {
        SetTextColor(hdc, VISTA_GREEN);
        TextOut(hdc, centerX - 150, textY, "专业版已激活 - 完整功能", 22);
    } else if (isTrialMode) {
        SetTextColor(hdc, VISTA_ORANGE);
        char trialStatus[50];
        sprintf(trialStatus, "试用版 (%d天剩余)", trialDaysRemaining);
        TextOut(hdc, centerX - 150, textY, trialStatus, strlen(trialStatus));
    } else {
        SetTextColor(hdc, VISTA_RED);
        TextOut(hdc, centerX - 150, textY, "未激活 - 功能受限", 17);
    }
    SetTextColor(hdc, RGB(0, 0, 0));
    
    textY += 40;
    
    // 绘制分隔线
    HPEN hLinePen = CreatePen(PS_SOLID, 1, RGB(150, 150, 150));
    hOldPen = (HPEN)SelectObject(hdc, hLinePen);
    MoveToEx(hdc, centerX - 250, textY, NULL);
    LineTo(hdc, centerX + 250, textY);
    SelectObject(hdc, hOldPen);
    DeleteObject(hLinePen);
    
    textY += 20;
    
    // 绘制版权信息
    HFONT hSmallFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    SelectObject(hdc, hSmallFont);
    
    TextOut(hdc, centerX - 250, textY, "(c) 2025 Jellyfish Studio", 29);
    textY += 20;
    
    char versionText[100];
    sprintf(versionText, "版本 %d.%d.%d - Professional Edition", MAJOR_VERSION, MINOR_VERSION, BUILD_VERSION);
    TextOut(hdc, centerX - 250, textY, versionText, strlen(versionText));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hContentFont);
    DeleteObject(hSmallFont);
}

// 绘制文件管理器
void DrawFileManager(HDC hdc, int centerX, int centerY) {
    RECT fileRect = {centerX - 400, centerY - 300, centerX + 400, centerY + 300};
    
    // 检查功能是否可用
    if (!IsFeatureAvailable(3)) {
        // 绘制功能受限提示
        HBRUSH hRestrictedBgBrush = CreateSolidBrush(RGB(255, 240, 240));
        FillRect(hdc, &fileRect, hRestrictedBgBrush);
        DeleteObject(hRestrictedBgBrush);
        
        HPEN hBorderPen = CreatePen(PS_SOLID, 2, VISTA_RED);
        HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, fileRect.left, fileRect.top, fileRect.right, fileRect.bottom);
        SelectObject(hdc, hOldPen);
        DeleteObject(hBorderPen);
        
        SetTextColor(hdc, VISTA_RED);
        SetBkMode(hdc, TRANSPARENT);
        
        HFONT hTitleFont = CreateFont(24, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
        HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
        
        TextOut(hdc, centerX - 150, centerY - 50, "功能受限", 8);
        
        HFONT hContentFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
        SelectObject(hdc, hContentFont);
        
        TextOut(hdc, centerX - 200, centerY, "文件管理器在试用模式下不可用", 26);
        TextOut(hdc, centerX - 180, centerY + 30, "请激活系统以使用完整功能", 24);
        
        // 绘制激活按钮
        RECT activateRect = {centerX - 80, centerY + 80, centerX + 80, centerY + 120};
        HBRUSH hActivateBrush = CreateSolidBrush(VISTA_GREEN);
        FillRect(hdc, &activateRect, hActivateBrush);
        DeleteObject(hActivateBrush);
    
        HPEN hActivatePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
        hOldPen = (HPEN)SelectObject(hdc, hActivatePen);
        SelectObject(hdc, GetStockObject(NULL_BRUSH));
        Rectangle(hdc, centerX - 80, centerY + 80, centerX + 80, centerY + 120);
        SelectObject(hdc, hOldPen);
        DeleteObject(hActivatePen);
    
        SetTextColor(hdc, RGB(255, 255, 255));
        TextOut(hdc, centerX - 30, centerY + 95, "激活", 4);
        
        SelectObject(hdc, hOldFont);
        DeleteObject(hTitleFont);
        DeleteObject(hContentFont);
        return;
    }
    
    // 正常文件管理器界面
    HBRUSH hFileBgBrush = CreateSolidBrush(FILEMANAGER_BG);
    FillRect(hdc, &fileRect, hFileBgBrush);
    DeleteObject(hFileBgBrush);
    
    HPEN hBorderPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 100));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, fileRect.left, fileRect.top, fileRect.right, fileRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    // 在标题栏左侧绘制守慧吉祥物
    DrawShouhuiMascot(hdc, fileRect.left + 10, fileRect.top - 5, 40);
    
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hTitleFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    TextOut(hdc, centerX - 60, fileRect.top + 10, "文件管理器", 10);
    
    HFONT hContentFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    SelectObject(hdc, hContentFont);
    
    // 显示当前路径
    TextOut(hdc, fileRect.left + 20, fileRect.top + 50, "路径：", 6);
    TextOut(hdc, fileRect.left + 60, fileRect.top + 50, currentPath, strlen(currentPath));
    
    // 绘制命令输入区域
    RECT inputRect = {fileRect.left + 20, fileRect.top + 80, fileRect.right - 20, fileRect.top + 110};
    HBRUSH hInputBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &inputRect, hInputBrush);
    DeleteObject(hInputBrush);
    
    HPEN hInputPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    hOldPen = (HPEN)SelectObject(hdc, hInputPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, fileRect.left + 20, fileRect.top + 80, fileRect.right - 20, fileRect.top + 110);
    SelectObject(hdc, hOldPen);
    DeleteObject(hInputPen);
    
    // 显示命令提示符和输入
    TextOut(hdc, fileRect.left + 25, fileRect.top + 85, "∈->", 4);
    TextOut(hdc, fileRect.left + 65, fileRect.top + 85, commandInput, strlen(commandInput));
    
    // 绘制光标
    static DWORD lastBlink = GetTickCount();
    static BOOL showCursor = TRUE;
    
    if (GetTickCount() - lastBlink > 500) {
        showCursor = !showCursor;
        lastBlink = GetTickCount();
    }
    
    if (showCursor) {
        int textWidth = GetTextWidth(hdc, commandInput);
        MoveToEx(hdc, fileRect.left + 65 + textWidth, fileRect.top + 85, NULL);
        LineTo(hdc, fileRect.left + 65 + textWidth, fileRect.top + 105);
    }
    
    // 绘制输出区域
    RECT outputRect = {fileRect.left + 20, fileRect.top + 120, fileRect.right - 20, fileRect.bottom - 60};
    HBRUSH hOutputBrush = CreateSolidBrush(RGB(240, 240, 240));
    FillRect(hdc, &outputRect, hOutputBrush);
    DeleteObject(hOutputBrush);
    
    HPEN hOutputPen = CreatePen(PS_SOLID, 1, RGB(200, 200, 200));
    hOldPen = (HPEN)SelectObject(hdc, hOutputPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, fileRect.left + 20, fileRect.top + 120, fileRect.right - 20, fileRect.bottom - 60);
    SelectObject(hdc, hOldPen);
    DeleteObject(hOutputPen);
    
    // 显示命令输出
    RECT textRect = {fileRect.left + 25, fileRect.top + 125, fileRect.right - 25, fileRect.bottom - 65};
    DrawText(hdc, commandOutput, -1, &textRect, DT_LEFT | DT_TOP | DT_WORDBREAK);
    
    // 绘制帮助提示
    HFONT hSmallFont = CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    SelectObject(hdc, hSmallFont);
    
    SetTextColor(hdc, RGB(100, 100, 100));
    TextOut(hdc, fileRect.left + 20, fileRect.bottom - 40, "输入 'help' 查看可用命令", 24);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hContentFont);
    DeleteObject(hSmallFont);
}

// 执行文件
void ExecuteFile(const char* filename) {
    // 模拟文件执行
    MessageBox(NULL, "文件执行功能", "提示", MB_OK | MB_ICONINFORMATION);
}

// 关闭所有应用
void CloseAllApps(HWND hwnd) {
	showCalendar = FALSE;
	showClock = FALSE;
	showEventDialog = FALSE;
    showCalculator = FALSE;
    showSystemInfo = FALSE;
    showFileManager = FALSE;
    showFileEditor = FALSE;
    showSettings = FALSE;
    showSpecialMessage = FALSE;
    showFinalEgg = FALSE;
    showBugReporter = FALSE;
    specialMessageScrollY = 0;
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

// 启动OOBE
void StartOOBE(HWND hwnd) {
    showOOBE = TRUE;
    showSecondScreen = FALSE;
    showCDKeyScreen = FALSE;
    currentStep = 0;
    strcpy(username, "");
    strcpy(password, "");
    strcpy(confirmPassword, "");
    strcpy(computerName, "FOS-PC");
    oobeError = FALSE;
    skipPasswordSetup = FALSE;  // 重置密码跳过标志
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 完成OOBE
void CompleteOOBE(HWND hwnd) {
    showOOBE = FALSE;
    showComplete = TRUE;
    isFirstBoot = FALSE;
    SaveConfig();
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
        
        SetTimer(hwnd, 5, 3000, NULL);
    }
}

// 显示登录界面
void ShowLoginScreen(HWND hwnd) {
    showLogin = TRUE;
    showBootAnimation = FALSE;
    showWelcome = FALSE;
    showSecondScreen = FALSE;
    showOOBE = FALSE;
    showComplete = FALSE;
    showDesktop = FALSE;
    showShutdown = FALSE;
    showCalculator = FALSE;
    showSystemInfo = FALSE;
    showFileManager = FALSE;
    showFileEditor = FALSE;
    showCDKeyScreen = FALSE;
    showSettings = FALSE;
    specialMessageScrollY = 0;
    currentStep = 0;
    loginError = FALSE;
    strcpy(loginUsername, "");
    strcpy(loginPassword, "");
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 显示桌面
void ShowDesktop(HWND hwnd) {
    showDesktop = TRUE;
    showLogin = FALSE;
    showWelcome = FALSE;
    showSecondScreen = FALSE;
    showOOBE = FALSE;
    showComplete = FALSE;
    showBootAnimation = FALSE;
    showShutdown = FALSE;
    showCalculator = FALSE;
    showSystemInfo = FALSE;
    showFileManager = FALSE;
    showFileEditor = FALSE;
    showCDKeyScreen = FALSE;
    showSettings = FALSE;
    showSpecialMessage = FALSE;
    showFinalEgg = FALSE;
    specialMessageScrollY = 0;
    showCalendar = FALSE;
	showClock = FALSE;
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 修改ShowShutdownAnimation函数
void ShowShutdownAnimation(HWND hwnd) {
    showShutdown = TRUE;
    showDesktop = FALSE;
    showCalculator = FALSE;
    showSystemInfo = FALSE;
    showFileManager = FALSE;
    showFileEditor = FALSE;
    showSettings = FALSE;
    showSpecialMessage = FALSE;
    showFinalEgg = FALSE;
    specialMessageScrollY = 0;
    shutdownProgress = 0;
    
    // 播放关机音乐
    if (!shutdownMusicPlaying) {
        PlaySound("音乐\\结束.wav", NULL, SND_FILENAME | SND_ASYNC);
        shutdownMusicPlaying = TRUE;
        shutdownMusicStartTime = GetTickCount();
    }
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
        
        SetTimer(hwnd, 6, 50, NULL);
    }
}

// 显示计算器
void ShowCalculator(HWND hwnd) {
    CloseAllApps(hwnd);
    showCalculator = TRUE;
    calcError = FALSE;
    specialMessageScrollY = 0;
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 显示系统信息
void ShowSystemInfo(HWND hwnd) {
    CloseAllApps(hwnd);
    showSystemInfo = TRUE;
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 显示文件管理器
void ShowFileManager(HWND hwnd) {
    CloseAllApps(hwnd);
    if (IsFeatureAvailable(3)) {
        showFileManager = TRUE;
        strcpy(commandInput, "");
        strcpy(commandOutput, "fOS 文件管理器 v1.0 专业版\n输入 'help' 查看可用命令\n");
        // 初始化当前路径
        if (strlen(currentPath) == 0) {
            GetCurrentDirectory(MAX_PATH, currentPath);
        }
        RefreshFileList();
    } else {
        // 显示功能受限提示
        MessageBox(hwnd, "文件管理器在试用模式下不可用\n请激活系统以使用完整功能", "功能受限", MB_OK | MB_ICONWARNING);
        return;
    }
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 结束FOS启动画面
void EndFOSBetaAnimation(HWND hwnd) {
    showFOSBeta = FALSE;
    showBootAnimation = TRUE;
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 结束开机动画
void EndBootAnimation(HWND hwnd) {
    showBootAnimation = FALSE;
    
    if (isFirstBoot && !isActivated && !isTrialMode) {
        ShowCDKeyScreen(hwnd);
    } else {
        showWelcome = TRUE;
        SetTimer(hwnd, 1, 5000, NULL);
        SetTimer(hwnd, 3, 100, NULL);
    }
}

// 关机系统
void ShutdownSystem(HWND hwnd) {
    ShowShutdownAnimation(hwnd);
}

// 清屏函数
void clean(HWND hwnd) {
    showWelcome = FALSE;
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 第二次清屏函数
void cleanSecond(HWND hwnd) {
    showSecondScreen = FALSE;
    if (isFirstBoot) {
        if (!isActivated && !isTrialMode) {
            ShowCDKeyScreen(hwnd);
        } else {
            StartOOBE(hwnd);
        }
    } else {
        ShowLoginScreen(hwnd);
    }
}

// 显示OOBE界面
void ShowOOBE(HWND hwnd) {
    showOOBE = TRUE;
    showCDKeyScreen = FALSE;
    showBootAnimation = FALSE;
    showWelcome = FALSE;
    showSecondScreen = FALSE;
    showComplete = FALSE;
    showDesktop = FALSE;
    showLogin = FALSE;
    showShutdown = FALSE;
    currentStep = 0;
    oobeError = FALSE;
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 启动试用模式 - 修复：正确设置试用模式并启动OOBE
void StartTrialMode(HWND hwnd) {
    isTrialMode = TRUE;
    isActivated = FALSE;
    trialDaysRemaining = 30;
    ApplyTrialRestrictions();
    
    // 修复：直接启动OOBE，而不是跳转到老界面
    if (hwnd) {
        showCDKeyScreen = FALSE;
        showBootAnimation = FALSE;
        showWelcome = FALSE;
        showSecondScreen = FALSE;
        StartOOBE(hwnd);
    }
}

// 处理中文输入
void ProcessChineseInput(HWND hwnd, WPARAM wParam) {
    static char firstByte = 0;
    
    if (firstByte == 0) {
        // 第一个字节（高位）
        if (wParam >= 0xA1 && wParam <= 0xFE) {
            firstByte = (char)wParam;
        }
    } else {
        // 第二个字节（低位）
        if (wParam >= 0xA1 && wParam <= 0xFE) {
            char chineseChar[3] = {firstByte, (char)wParam, '\0'};
            AddChineseToCurrentBuffer(chineseChar);
            firstByte = 0;
        } else {
            firstByte = 0;  // 重置
        }
    }
}

// 添加中文到当前缓冲区
void AddChineseToCurrentBuffer(const char* chineseChar) {
    if (!chineseChar || strlen(chineseChar) != 2) return;
    
    // 根据当前活动窗口添加到对应缓冲区
    if (showFileManager) {
        // 文件管理器命令输入
        if (strlen(commandInput) + 2 < 99) {
            strcat(commandInput, chineseChar);
        }
    }
    else if (showFileEditor && isEditing) {
        // 文件编辑器
        int fileLen = strlen(fileContent);
        if (fileLen + 2 < 9999) {
            // 在光标位置插入
            for (int i = fileLen; i >= cursorPosition; i--) {
                fileContent[i + 2] = fileContent[i];
            }
            fileContent[cursorPosition] = chineseChar[0];
            fileContent[cursorPosition + 1] = chineseChar[1];
            cursorPosition += 2;
        }
    }
    else if (showCalculator && showSpecialMessage) {
        // 猜谜输入
        if (strlen(guessInput) + 2 < 49) {
            strcat(guessInput, chineseChar);
        }
    }
    else if (showOOBE) {
        // OOBE输入
        if (currentStep == 1) {
            if (strlen(username) + 2 < 49) {
                strcat(username, chineseChar);
            }
        }
        else if (currentStep == 2) {
            if (strlen(password) + 2 < 49) {
                strcat(password, chineseChar);
            }
        }
        else if (currentStep == 3) {
            if (strlen(computerName) + 2 < 49) {
                strcat(computerName, chineseChar);
            }
        }
    }
    else if (showLogin) {
        // 登录界面
        if (currentStep == 0) {
            if (strlen(loginUsername) + 2 < 49) {
                strcat(loginUsername, chineseChar);
            }
        }
        else if (currentStep == 1) {
            if (strlen(loginPassword) + 2 < 49) {
                strcat(loginPassword, chineseChar);
            }
        }
    }
    else if (showSettings) {
        // 设置软件
        if (settingsCurrentStep == 0) {
            if (settingsFocusField == 0) { // 新用户名
                if (strlen(newUsername) + 2 < 49) {
                    strcat(newUsername, chineseChar);
                }
            }
            else if (settingsFocusField == 1) { // 新密码
                if (strlen(newPassword) + 2 < 49) {
                    strcat(newPassword, chineseChar);
                }
            }
            else if (settingsFocusField == 2) { // 确认密码
                if (strlen(newConfirmPassword) + 2 < 49) {
                    strcat(newConfirmPassword, chineseChar);
                }
            }
        }
        else if (settingsCurrentStep == 1) {
            // 计算机名
            if (strlen(newComputerName) + 2 < 49) {
                strcat(newComputerName, chineseChar);
            }
        }
    }
}

// 显示日历软件
void ShowCalendar(HWND hwnd) {
    CloseAllApps(hwnd);
    showCalendar = TRUE;
    calendarView = 0; // 默认为月视图
    showEventDialog = FALSE;
    LoadCalendarEvents();
    
    // 获取当前日期
    time_t now;
    struct tm* timeinfo;
    time(&now);
    timeinfo = localtime(&now);
    calendarYear = timeinfo->tm_year + 1900;
    calendarMonth = timeinfo->tm_mon + 1;
    selectedDay = timeinfo->tm_mday;
    
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 加载日历事件
void LoadCalendarEvents() {
    memset(calendarEvents, 0, sizeof(calendarEvents));
    FILE* file = fopen("calendar_events.dat", "rb");
    if (file) {
        fread(calendarEvents, sizeof(calendarEvents), 1, file);
        fclose(file);
    }
}

// 保存日历事件
void SaveCalendarEvents() {
    FILE* file = fopen("calendar_events.dat", "wb");
    if (file) {
        fwrite(calendarEvents, sizeof(calendarEvents), 1, file);
        fclose(file);
    }
}

// 获取月份天数
int GetDaysInMonth(int year, int month) {
    if (month == 2) {
        // 闰年判断
        if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
            return 29;
        else
            return 28;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
        return 30;
    else
        return 31;
}

// 获取月份第一天是星期几（0=星期日, 1=星期一, ... 6=星期六）
int GetFirstDayOfMonth(int year, int month) {
    struct tm timeinfo = {0};
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = 1;
    mktime(&timeinfo);
    return timeinfo.tm_wday; // 0 = Sunday
}

// 绘制事件指示器
void DrawCalendarEventIndicator(HDC hdc, int x, int y, int hasEvent) {
    if (hasEvent) {
        HBRUSH hEventBrush = CreateSolidBrush(RGB(255, 100, 100));
        HPEN hOldPen = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));
        SelectObject(hdc, hEventBrush);
        Ellipse(hdc, x - 3, y - 3, x + 3, y + 3);
        DeleteObject(hEventBrush);
        SelectObject(hdc, hOldPen);
    }
}

// 绘制日历工具栏
void DrawCalendarToolbar(HDC hdc, int centerX, int centerY, int topY) {
    // 年份切换按钮
    int buttonWidth = 40;
    int buttonHeight = 30;
    
    // 显示当前年月
    char yearMonth[50];
    sprintf(yearMonth, "%d年 %d月", calendarYear, calendarMonth);
    HFONT hMonthFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hMonthFont);
    SetTextColor(hdc, RGB(0, 0, 0));
    int textWidth = GetTextWidth(hdc, yearMonth);
    TextOut(hdc, centerX - textWidth/2, topY + 15, yearMonth, strlen(yearMonth));
    
}

// 绘制月视图
void DrawMonthView(HDC hdc, int centerX, int centerY) {
    int topY = centerY - 200;
    
    // 星期标题
    const char* weekDays[] = {"日", "一", "二", "三", "四", "五", "六"};
    int cellWidth = 100;
    int cellHeight = 70;
    int startX = centerX - 350;
    int startY = topY + 100;
    
    HFONT hWeekFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hDayFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    
    HPEN hGridPen = CreatePen(PS_SOLID, 1, RGB(180, 180, 180));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hGridPen);
    
    // 绘制星期标题
    HFONT hOldFont = (HFONT)SelectObject(hdc, hWeekFont);
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    
    for (int i = 0; i < 7; i++) {
        int x = startX + i * cellWidth;
        RECT cellRect = {x, startY - 30, x + cellWidth, startY};
        FrameRect(hdc, &cellRect, (HBRUSH)GetStockObject(BLACK_BRUSH));
        TextOut(hdc, x + 40, startY - 25, weekDays[i], 2);
    }
    
    // 获取当月信息
    int daysInMonth = GetDaysInMonth(calendarYear, calendarMonth);
    int firstDay = GetFirstDayOfMonth(calendarYear, calendarMonth);
    
    // 获取当前日期用于高亮
    time_t now;
    struct tm* timeinfo;
    time(&now);
    timeinfo = localtime(&now);
    int todayYear = timeinfo->tm_year + 1900;
    int todayMonth = timeinfo->tm_mon + 1;
    int todayDay = timeinfo->tm_mday;
    
    // 绘制日期格子
    SelectObject(hdc, hDayFont);
    
    int day = 1;
    for (int row = 0; row < 6; row++) {
        for (int col = 0; col < 7; col++) {
            int cellX = startX + col * cellWidth;
            int cellY = startY + row * cellHeight;
            RECT cellRect = {cellX, cellY, cellX + cellWidth, cellY + cellHeight};
            
            // 绘制边框
            Rectangle(hdc, cellX, cellY, cellX + cellWidth, cellY + cellHeight);
            
            // 计算当前格子对应的日期
            int cellDay = day;
            if (row == 0 && col < firstDay) {
                // 上月日期
                int prevMonth = calendarMonth - 1;
                int prevYear = calendarYear;
                if (prevMonth == 0) {
                    prevMonth = 12;
                    prevYear--;
                }
                int prevDays = GetDaysInMonth(prevYear, prevMonth);
                cellDay = prevDays - (firstDay - col - 1);
                SetTextColor(hdc, RGB(150, 150, 150));
            } else if (day > daysInMonth) {
                // 下月日期
                cellDay = day - daysInMonth;
                SetTextColor(hdc, RGB(150, 150, 150));
                day++;
            } else {
                // 当月日期
                SetTextColor(hdc, RGB(0, 0, 0));
                day++;
            }
            
            // 高亮今天
            if (calendarYear == todayYear && calendarMonth == todayMonth && cellDay == todayDay) {
                HBRUSH hTodayBrush = CreateSolidBrush(RGB(255, 255, 200));
                RECT highlightRect = {cellX + 2, cellY + 2, cellX + cellWidth - 2, cellY + cellHeight - 2};
                FillRect(hdc, &highlightRect, hTodayBrush);
                DeleteObject(hTodayBrush);
            }
            
            // 高亮选中日期
            if (cellDay == selectedDay && row != 0 && col >= firstDay && day <= daysInMonth + 1) {
                HPEN hSelectPen = CreatePen(PS_SOLID, 3, RGB(0, 120, 215));
                SelectObject(hdc, hSelectPen);
                SelectObject(hdc, GetStockObject(NULL_BRUSH));
                Rectangle(hdc, cellX, cellY, cellX + cellWidth, cellY + cellHeight);
                DeleteObject(hSelectPen);
                SelectObject(hdc, hGridPen);
            }
            
            // 绘制日期数字
            char dayStr[10];
            sprintf(dayStr, "%d", cellDay);
            TextOut(hdc, cellX + 5, cellY + 5, dayStr, strlen(dayStr));
            
            // 绘制事件指示器
            if (calendarYear == todayYear && calendarMonth == todayMonth) {
                if (strlen(calendarEvents[calendarMonth-1][cellDay-1]) > 0) {
                    DrawCalendarEventIndicator(hdc, cellX + cellWidth - 15, cellY + 15, TRUE);
                }
            }
            
            // 简单事件预览
            if (calendarYear == todayYear && calendarMonth == todayMonth) {
                if (strlen(calendarEvents[calendarMonth-1][cellDay-1]) > 0) {
                    HFONT hSmallFont = CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
                    HFONT hOldSmallFont = (HFONT)SelectObject(hdc, hSmallFont);
                    
                    char eventPreview[30];
                    strncpy(eventPreview, calendarEvents[calendarMonth-1][cellDay-1], 12);
                    eventPreview[12] = '\0';
                    if (strlen(calendarEvents[calendarMonth-1][cellDay-1]) > 12)
                        strcat(eventPreview, "...");
                    
                    TextOut(hdc, cellX + 5, cellY + 25, eventPreview, strlen(eventPreview));
                    
                    SelectObject(hdc, hOldSmallFont);
                    DeleteObject(hSmallFont);
                }
            }
        }
        if (day > daysInMonth) break;
    }
    
    SelectObject(hdc, hOldFont);
    SelectObject(hdc, hOldPen);
    DeleteObject(hWeekFont);
    DeleteObject(hDayFont);
    DeleteObject(hGridPen);
}

// 绘制年视图
void DrawYearView(HDC hdc, int centerX, int centerY) {
    int topY = centerY - 200;
    int monthWidth = 180;
    int monthHeight = 120;
    int startX = centerX - 350;
    
    HFONT hMonthTitleFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hDayFont = CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    
    HPEN hGridPen = CreatePen(PS_SOLID, 1, RGB(180, 180, 180));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hGridPen);
    HFONT hOldFont = (HFONT)SelectObject(hdc, hMonthTitleFont);
    
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    
    const char* monthNames[] = {"一月", "二月", "三月", "四月", "五月", "六月",
                                "七月", "八月", "九月", "十月", "十一月", "十二月"};
    
    for (int m = 0; m < 12; m++) {
        int row = m / 4;
        int col = m % 4;
        int monthX = startX + col * (monthWidth + 20);
        int monthY = topY + 50 + row * (monthHeight + 30);
        
        // 绘制月份边框
        RECT monthRect = {monthX, monthY, monthX + monthWidth, monthY + monthHeight};
        Rectangle(hdc, monthRect.left, monthRect.top, monthRect.right, monthRect.bottom);
        
        // 高亮当前月份
        time_t now;
        struct tm* timeinfo;
        time(&now);
        timeinfo = localtime(&now);
        int currentMonth = timeinfo->tm_mon;
        
        if (m == currentMonth && calendarYear == timeinfo->tm_year + 1900) {
            HBRUSH hCurrentBrush = CreateSolidBrush(RGB(255, 255, 200));
            FillRect(hdc, &monthRect, hCurrentBrush);
            DeleteObject(hCurrentBrush);
        }
        
        // 绘制月份标题
        char title[50];
        sprintf(title, "%s %d", monthNames[m], calendarYear);
        TextOut(hdc, monthX + 10, monthY + 5, title, strlen(title));
        
        // 绘制小日历格子
        SelectObject(hdc, hDayFont);
        int daysInMonth = GetDaysInMonth(calendarYear, m + 1);
        int firstDay = GetFirstDayOfMonth(calendarYear, m + 1);
        
        int day = 1;
        for (int w = 0; w < 5; w++) {
            for (int d = 0; d < 7; d++) {
                int cellX = monthX + 10 + d * 20;
                int cellY = monthY + 30 + w * 15;
                
                if (w == 0 && d < firstDay) {
                    // 空白
                } else if (day <= daysInMonth) {
                    char dayStr[3];
                    sprintf(dayStr, "%d", day);
                    TextOut(hdc, cellX, cellY, dayStr, strlen(dayStr));
                    
                    // 事件指示器
                    if (strlen(calendarEvents[m][day-1]) > 0) {
                        DrawCalendarEventIndicator(hdc, cellX + 10, cellY - 2, TRUE);
                    }
                    
                    day++;
                }
            }
        }
        
        SelectObject(hdc, hMonthTitleFont);
    }
    
    SelectObject(hdc, hOldFont);
    SelectObject(hdc, hOldPen);
    DeleteObject(hMonthTitleFont);
    DeleteObject(hDayFont);
    DeleteObject(hGridPen);
}

// 绘制事件对话框
void DrawEventDialog(HDC hdc, int centerX, int centerY) {
    RECT dialogRect = {centerX - 250, centerY - 200, centerX + 250, centerY + 200};
    HBRUSH hDialogBrush = CreateSolidBrush(RGB(240, 240, 255));
    FillRect(hdc, &dialogRect, hDialogBrush);
    DeleteObject(hDialogBrush);
    
    HPEN hBorderPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 200));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, dialogRect.left, dialogRect.top, dialogRect.right, dialogRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    HFONT hTitleFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hNormalFont = CreateFont(16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    SetTextColor(hdc, RGB(0, 0, 0));
    SetBkMode(hdc, TRANSPARENT);
    
    char title[100];
    sprintf(title, "%d年%d月%d日 事件", calendarYear, calendarMonth, selectedDay);
    TextOut(hdc, centerX - 150, centerY - 180, title, strlen(title));
    
    // 事件输入框
    RECT inputRect = {centerX - 200, centerY - 120, centerX + 200, centerY - 80};
    HBRUSH hInputBrush = CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hdc, &inputRect, hInputBrush);
    DeleteObject(hInputBrush);
    
    HPEN hInputPen = CreatePen(PS_SOLID, 2, RGB(0, 120, 215));
    hOldPen = (HPEN)SelectObject(hdc, hInputPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, inputRect.left, inputRect.top, inputRect.right, inputRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hInputPen);
    
    SelectObject(hdc, hNormalFont);
    TextOut(hdc, centerX - 190, centerY - 115, eventText, strlen(eventText));
    
    // 光标
    static DWORD lastBlink = GetTickCount();
    static BOOL showCursor = TRUE;
    if (GetTickCount() - lastBlink > 500) {
        showCursor = !showCursor;
        lastBlink = GetTickCount();
    }
    if (showCursor) {
        int textWidth = GetTextWidth(hdc, eventText);
        MoveToEx(hdc, centerX - 190 + textWidth, centerY - 115, NULL);
        LineTo(hdc, centerX - 190 + textWidth, centerY - 95);
    }
    
    // 按钮
    RECT saveRect = {centerX - 150, centerY - 40, centerX - 30, centerY};
    HBRUSH hSaveBrush = CreateSolidBrush(RGB(100, 200, 100));
    FillRect(hdc, &saveRect, hSaveBrush);
    DeleteObject(hSaveBrush);
    HPEN hButtonPen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
    hOldPen = (HPEN)SelectObject(hdc, hButtonPen);
    Rectangle(hdc, saveRect.left, saveRect.top, saveRect.right, saveRect.bottom);
    TextOut(hdc, centerX - 130, centerY - 30, "保存", 4);
    
    RECT deleteRect = {centerX - 20, centerY - 40, centerX + 100, centerY};
    HBRUSH hDeleteBrush = CreateSolidBrush(RGB(200, 100, 100));
    FillRect(hdc, &deleteRect, hDeleteBrush);
    DeleteObject(hDeleteBrush);
    Rectangle(hdc, deleteRect.left, deleteRect.top, deleteRect.right, deleteRect.bottom);
    TextOut(hdc, centerX, centerY - 30, "删除", 4);
    
    RECT cancelRect = {centerX + 110, centerY - 40, centerX + 230, centerY};
    HBRUSH hCancelBrush = CreateSolidBrush(RGB(150, 150, 150));
    FillRect(hdc, &cancelRect, hCancelBrush);
    DeleteObject(hCancelBrush);
    Rectangle(hdc, cancelRect.left, cancelRect.top, cancelRect.right, cancelRect.bottom);
    TextOut(hdc, centerX + 130, centerY - 30, "取消", 4);
    
    SelectObject(hdc, hOldPen);
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hNormalFont);
    DeleteObject(hButtonPen);
}

// 绘制日历主界面
void DrawCalendar(HDC hdc, int centerX, int centerY) {
    RECT calendarRect = {centerX - 400, centerY - 300, centerX + 400, centerY + 300};
    
    // 背景
    HBRUSH hBgBrush = CreateSolidBrush(RGB(250, 250, 255));
    FillRect(hdc, &calendarRect, hBgBrush);
    DeleteObject(hBgBrush);
    
    // 边框
    HPEN hBorderPen = CreatePen(PS_SOLID, 2, RGB(100, 100, 200));
    HPEN hOldPen = (HPEN)SelectObject(hdc, hBorderPen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, calendarRect.left, calendarRect.top, calendarRect.right, calendarRect.bottom);
    SelectObject(hdc, hOldPen);
    DeleteObject(hBorderPen);
    
    // 标题栏
    RECT titleRect = {calendarRect.left, calendarRect.top, calendarRect.right, calendarRect.top + 40};
    HBRUSH hTitleBrush = CreateSolidBrush(RGB(70, 130, 180));
    FillRect(hdc, &titleRect, hTitleBrush);
    DeleteObject(hTitleBrush);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    
    HFONT hTitleFont = CreateFont(20, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
    
    TextOut(hdc, calendarRect.left + 10, calendarRect.top + 8, "日历 - 专业版", 11);
    
    // 守慧吉祥物
    DrawShouhuiMascot(hdc, calendarRect.right - 50, calendarRect.top - 5, 40);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    
    // 绘制工具栏
    DrawCalendarToolbar(hdc, centerX, centerY, calendarRect.top);
    
    // 绘制日历视图
    if (calendarView == 0) {
        DrawMonthView(hdc, centerX, centerY);
    } else {
        DrawYearView(hdc, centerX, centerY);
    }
    
    // 如果显示事件对话框
    if (showEventDialog) {
        DrawEventDialog(hdc, centerX, centerY);
    }
}

// 处理日历鼠标点击 - 修复版本
void HandleCalendarMouseClick(int x, int y, int centerX, int centerY) {
    int topY = centerY - 200;
    
    // 工具栏按钮 - 使用绝对坐标
    // 上一年
    if (x >= centerX - 250 && x <= centerX - 210 && y >= topY + 10 && y <= topY + 40) {
        calendarYear--;
        selectedDay = 1;
        InvalidateRect(GetActiveWindow(), NULL, TRUE);
        return;
    }
    // 上个月
    else if (x >= centerX - 200 && x <= centerX - 160 && y >= topY + 10 && y <= topY + 40) {
        calendarMonth--;
        if (calendarMonth < 1) {
            calendarMonth = 12;
            calendarYear--;
        }
        selectedDay = 1;
        InvalidateRect(GetActiveWindow(), NULL, TRUE);
        return;
    }
    // 下个月
    else if (x >= centerX + 160 && x <= centerX + 200 && y >= topY + 10 && y <= topY + 40) {
        calendarMonth++;
        if (calendarMonth > 12) {
            calendarMonth = 1;
            calendarYear++;
        }
        selectedDay = 1;
        InvalidateRect(GetActiveWindow(), NULL, TRUE);
        return;
    }
    // 下一年
    else if (x >= centerX + 210 && x <= centerX + 250 && y >= topY + 10 && y <= topY + 40) {
        calendarYear++;
        selectedDay = 1;
        InvalidateRect(GetActiveWindow(), NULL, TRUE);
        return;
    }
    // 视图切换
    else if (x >= centerX - 80 && x <= centerX + 80 && y >= topY + 50 && y <= topY + 80) {
        calendarView = !calendarView;
        InvalidateRect(GetActiveWindow(), NULL, TRUE);
        return;
    }
    // 今日
    else if (x >= centerX + 100 && x <= centerX + 180 && y >= topY + 50 && y <= topY + 80) {
        time_t now;
        struct tm* timeinfo;
        time(&now);
        timeinfo = localtime(&now);
        calendarYear = timeinfo->tm_year + 1900;
        calendarMonth = timeinfo->tm_mon + 1;
        selectedDay = timeinfo->tm_mday;
        InvalidateRect(GetActiveWindow(), NULL, TRUE);
        return;
    }
    // 日期格子点击（月视图）
    else if (calendarView == 0 && y >= topY + 100 && y <= topY + 100 + 6 * 70) {
        int startX = centerX - 350;
        int row = (y - (topY + 100)) / 70;
        int col = (x - startX) / 100;
        
        if (row >= 0 && row < 6 && col >= 0 && col < 7) {
            int daysInMonth = GetDaysInMonth(calendarYear, calendarMonth);
            int firstDay = GetFirstDayOfMonth(calendarYear, calendarMonth);
            
            int clickedDay = row * 7 + col - firstDay + 1;
            if (clickedDay >= 1 && clickedDay <= daysInMonth) {
                selectedDay = clickedDay;
                // 打开事件对话框
                showEventDialog = TRUE;
                strcpy(eventText, calendarEvents[calendarMonth-1][selectedDay-1]);
                InvalidateRect(GetActiveWindow(), NULL, TRUE);
            }
        }
        return;
    }
}

// 处理日历输入 - 修复版本
void HandleCalendarInput(char key) {
    if (showEventDialog) {
        switch (key) {
            case 8: // Backspace
                if (strlen(eventText) > 0) {
                    eventText[strlen(eventText) - 1] = '\0';
                }
                InvalidateRect(GetActiveWindow(), NULL, TRUE);
                break;
            case 13: // Enter - 保存
                strcpy(calendarEvents[calendarMonth-1][selectedDay-1], eventText);
                SaveCalendarEvents();
                showEventDialog = FALSE;
                InvalidateRect(GetActiveWindow(), NULL, TRUE);
                break;
            case 27: // ESC - 取消
                showEventDialog = FALSE;
                InvalidateRect(GetActiveWindow(), NULL, TRUE);
                break;
            default:
                if (key >= 32 && key <= 126 && strlen(eventText) < 199) {
                    int len = strlen(eventText);
                    eventText[len] = key;
                    eventText[len + 1] = '\0';
                }
                InvalidateRect(GetActiveWindow(), NULL, TRUE);
                break;
        }
    } else {
        switch (key) {
            case 27: // ESC - 关闭日历
                showCalendar = FALSE;
                InvalidateRect(GetActiveWindow(), NULL, TRUE);
                break;
        }
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch(Message) {
        case WM_CREATE: {
            LoadConfig();
            InitializeSystemInfo();
            SetTimer(hwnd, 8, 40, NULL);
            SetTimer(hwnd, 7, 1000, NULL);
            
            // 初始化IME
            hIMC = ImmGetContext(hwnd);
            if (hIMC) {
                // 设置输入法状态
                DWORD convMode, sentMode;
                ImmGetConversionStatus(hIMC, &convMode, &sentMode);
                
                // 启用中文输入
                convMode |= IME_CMODE_NATIVE;
                ImmSetConversionStatus(hIMC, convMode, sentMode);
                
                ImmReleaseContext(hwnd, hIMC);
            }
            
            // 加载日历事件
            LoadCalendarEvents();
            break;
        }
        
        case WM_SIZE: {
            CreateBackgroundBitmap(hwnd);
            break;
        }
        
        case WM_TIMER: {
            if (wParam == 1) {
                // 欢迎界面定时器
                KillTimer(hwnd, 1);
                clean(hwnd);
                showSecondScreen = TRUE;
                InvalidateRect(hwnd, NULL, TRUE);
                SetTimer(hwnd, 2, 10000, NULL);
            }
            else if (wParam == 2) {
                // 第二界面定时器
                KillTimer(hwnd, 2);
                cleanSecond(hwnd);
            }
            else if (wParam == 3) {
                // 进度条动画定时器
                progressValue = (progressValue + 2) % 100;
                if (!showOOBE && !showComplete && !showDesktop && !showBootAnimation && !showLogin && !showCDKeyScreen && !showGameEgg && !showFileEditor && !showSettings && !showUSBError && !showBugReporter && !showBugList && !showCalendar) {
                    InvalidateRect(hwnd, NULL, FALSE);
                }
            }
            else if (wParam == 4) {
                // 启动进度定时器
                progressValue += 2;
                if (progressValue >= 100) {
                    KillTimer(hwnd, 4);
                    progressValue = 100;
                    
                    if (isFirstBoot) {
                        EndBootAnimation(hwnd);
                        SetTimer(hwnd, 1, 5000, NULL);
                        SetTimer(hwnd, 3, 100, NULL);
                    } else {
                        ShowLoginScreen(hwnd);
                    }
                }
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (wParam == 5) {
                // OOBE完成定时器
                KillTimer(hwnd, 5);
                ShowShutdownAnimation(hwnd);
            }
            else if (wParam == 6) {
                // 关机进度定时器
                shutdownProgress += 2;
                if (shutdownProgress >= 100) {
                    KillTimer(hwnd, 6);
                    shutdownProgress = 100;
                    
                    if (!ExitWindowsEx(EWX_SHUTDOWN | EWX_FORCE, 0)) {
                        MessageBox(hwnd, "请按按键退出", "提示", MB_OK | MB_ICONINFORMATION);
                        PostQuitMessage(0);
                    }
                }
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (wParam == 7) {
                // 桌面时间更新
                if (showDesktop) {
                    InvalidateRect(hwnd, NULL, TRUE);
                }
                break;
            }
            else if (wParam == 8) {
                // FOS启动进度
                fosProgressValue += 2;
                if (fosProgressValue >= 100) {
                    KillTimer(hwnd, 8);
                    fosProgressValue = 100;
                    
                    EndFOSBetaAnimation(hwnd);
                    SetTimer(hwnd, 4, 40, NULL);
                }
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (wParam == 9) {
                // 水母游戏更新定时器
                UpdateJellyfishGame(hwnd);
                break;
            }
            else if (wParam == 12 && showPurpleScreen) {
                // 紫屏进度更新
                purpleScreenProgress += 1;
                if (purpleScreenProgress >= 100) {
                    purpleScreenProgress = 100;
                    // 保持在100%，不自动退出
                }
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        }
        
        // IME输入法消息处理
        case WM_IME_STARTCOMPOSITION:
            isIMEComposing = TRUE;
            break;
            
        case WM_IME_ENDCOMPOSITION:
            isIMEComposing = FALSE;
            break;
            
        case WM_IME_COMPOSITION:
            if (lParam & GCS_RESULTSTR) {
                // IME完成输入
                HIMC hIMCTemp = ImmGetContext(hwnd);
                if (hIMCTemp) {
                    // 获取输入结果长度
                    LONG dwSize = ImmGetCompositionString(hIMCTemp, GCS_RESULTSTR, NULL, 0);
                    if (dwSize > 0) {
                        // 获取输入结果
                        char* lpStr = new char[dwSize + 1];
                        ImmGetCompositionString(hIMCTemp, GCS_RESULTSTR, lpStr, dwSize);
                        lpStr[dwSize] = '\0';
                        
                        // 添加到当前缓冲区
                        if (showFileManager) {
                            // 文件管理器命令输入
                            if (strlen(commandInput) + dwSize < 99) {
                                strcat(commandInput, lpStr);
                            }
                        }
                        else if (showFileEditor && isEditing) {
                            // 文件编辑器
                            int fileLen = strlen(fileContent);
                            if (fileLen + dwSize < 9999) {
                                // 在光标位置插入
                                for (int i = fileLen; i >= cursorPosition; i--) {
                                    fileContent[i + dwSize] = fileContent[i];
                                }
                                strncpy(&fileContent[cursorPosition], lpStr, dwSize);
                                cursorPosition += dwSize;
                            }
                        }
                        else if (showCalculator && showSpecialMessage) {
                            // 猜谜输入
                            if (strlen(guessInput) + dwSize < 49) {
                                strcat(guessInput, lpStr);
                            }
                        }
                        else if (showOOBE) {
                            // OOBE输入
                            if (currentStep == 1) {
                                if (strlen(username) + dwSize < 49) {
                                    strcat(username, lpStr);
                                }
                            }
                            else if (currentStep == 2) {
                                if (strlen(password) + dwSize < 49) {
                                    strcat(password, lpStr);
                                }
                            }
                            else if (currentStep == 3) {
                                if (strlen(computerName) + dwSize < 49) {
                                    strcat(computerName, lpStr);
                                }
                            }
                        }
                        else if (showLogin) {
                            // 登录界面
                            if (currentStep == 0) {
                                if (strlen(loginUsername) + dwSize < 49) {
                                    strcat(loginUsername, lpStr);
                                }
                            }
                            else if (currentStep == 1) {
                                if (strlen(loginPassword) + dwSize < 49) {
                                    strcat(loginPassword, lpStr);
                                }
                            }
                        }
                        else if (showSettings) {
                            // 设置软件
                            if (settingsCurrentStep == 0) {
                                if (settingsFocusField == 0) { // 新用户名
                                    if (strlen(newUsername) + dwSize < 49) {
                                        strcat(newUsername, lpStr);
                                    }
                                }
                                else if (settingsFocusField == 1) { // 新密码
                                    if (strlen(newPassword) + dwSize < 49) {
                                        strcat(newPassword, lpStr);
                                    }
                                }
                                else if (settingsFocusField == 2) { // 确认密码
                                    if (strlen(newConfirmPassword) + dwSize < 49) {
                                        strcat(newConfirmPassword, lpStr);
                                    }
                                }
                            }
                            else if (settingsCurrentStep == 1) {
                                // 计算机名
                                if (strlen(newComputerName) + dwSize < 49) {
                                    strcat(newComputerName, lpStr);
                                }
                            }
                        }
                        else if (showCalendar && showEventDialog) {
                            // 日历事件输入
                            if (strlen(eventText) + dwSize < 199) {
                                strcat(eventText, lpStr);
                            }
                        }
                        
                        delete[] lpStr;
                    }
                    ImmReleaseContext(hwnd, hIMCTemp);
                    
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            break;
        
        case WM_CHAR: {
            // 水母游戏优先处理
            if (showGameEgg) {
                HandleJellyfishGameInput(wParam, hwnd);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }
            
            // fos彩蛋输入处理
            if (showFOSEgg) {
                // 处理"castrate jellyfish"彩蛋的特殊输入
                if (fosEggState == 5) {
                    // 水母求饶界面只响应ESC键
                    if (wParam == 27) { // ESC键
                        showFOSEgg = FALSE;
                        showFileManager = TRUE;
                        fosInputCount = 0;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                }
                return 0;
            }
            
            if (showCDKeyScreen) {
                HandleCDKeyInput((char)wParam);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (showOOBE) {
                HandleOOBEInput((char)wParam);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (showLogin) {
                HandleLoginInput((char)wParam);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (showCalculator) {
                HandleCalculatorInput((char)wParam);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (showFileManager) {
                HandleFileManagerInput((char)wParam);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (showFileEditor) {
                HandleFileEditorInput((char)wParam);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (showSettings) {
                HandleSettingsInput((char)wParam);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (showBugReporter) {
                HandleBugReporterInput((char)wParam);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (showBugList) {
                HandleBugListInput((char)wParam);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (showPurpleScreen) {
                HandlePurpleScreenInput((char)wParam, hwnd);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            else if (showCalendar) {
                HandleCalendarInput((char)wParam);
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        }
        
        case WM_KEYDOWN: {
            // 水母游戏激活检测
            CheckGameActivation(wParam, hwnd);
            
            // 水母游戏输入处理
            if (showGameEgg) {
                HandleJellyfishGameInput(wParam, hwnd);
                return 0;
            }
            
            if (showPurpleScreen) {
                // 在紫屏状态下，检查Ctrl+Alt+E
                if ((wParam == 'E' || wParam == 'e') && GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_MENU)) {
                    // 触发重启
                    ShowShutdownAnimation(hwnd);
                    showPurpleScreen = FALSE;
                    purpleScreenLocked = FALSE;
                    return 0;
                }
            }
            
            // fos彩蛋按键处理（包含"castrate jellyfish"彩蛋）
            if (showFOSEgg) {
                if (wParam == VK_ESCAPE) {
                    // ESC键跳过彩蛋
                    showFOSEgg = FALSE;
                    showFileManager = TRUE;
                    fosInputCount = 0;
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                else if (wParam == VK_RETURN) {
                    // Enter键翻页（水母求饶界面不需要翻页，直接返回）
                    if (fosEggState == 5) {
                        // "castrate jellyfish"彩蛋界面 - 按Enter直接返回
                        showFOSEgg = FALSE;
                        showFileManager = TRUE;
                        fosInputCount = 0;
                    } else {
                        // 原有的fos彩蛋翻页逻辑
                        switch (fosEggState) {
                            case 1: { // 旁白对话
                                dialogueIndexx++;
                                if (dialogueIndexx >= 4) {
                                    fosEggState = 2;
                                    dialogueIndexx = 0;
                                    currentDevStage = 0;
                                }
                                break;
                            }
                            case 2: { // 开发历程回顾
                                currentDevStage++;
                                if (currentDevStage >= 8) {
                                    fosEggState = 3;
                                    currentDevStage = 0;
                                }
                                break;
                            }
                            case 3: { // 功能展示
                                currentDevStage++;
                                if (currentDevStage >= 6) {
                                    fosEggState = 4;
                                    currentDevStage = 0;
                                }
                                break;
                            }
                            case 4: { // 致谢名单
                                showFOSEgg = FALSE;
                                showFileManager = TRUE;
                                fosInputCount = 0;
                                break;
                            }
                        }
                    }
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                return 0;
            }   
            
            // 文件编辑器特殊按键处理
            if (showFileEditor) {
                HandleFileEditorSpecialKeys(wParam);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }
            
            // Bug列表特殊按键处理
            if (showBugList) {
                HandleBugListInput((char)wParam);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }
            
            // OOBE界面Tab键处理
            if (showOOBE) {
                if (wParam == VK_TAB) {
                    switch (currentStep) {
                        case 1: currentStep = 2; break;
                        case 2: currentStep = 3; break;
                        case 3: currentStep = 3; break;
                    }
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            else if (showLogin) {
                // 登录界面Tab键处理
                if (wParam == VK_TAB && currentStep == 0) {
                    currentStep = 1;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            else if (showDesktop) {
                // 桌面快捷键
                if (wParam == VK_F1) {
                    ShowCalculator(hwnd);
                }
                else if (wParam == VK_F2) {
                    ShowSystemInfo(hwnd);
                }
                else if (wParam == VK_F3) {
                    ShowFileManager(hwnd);
                }
                else if (wParam == VK_F4 && isActivated) {
                    ShowSettings(hwnd);
                }
                else if (wParam == VK_F5) {
                    // F5打开Bug报告工具
                    ShowBugReporter(hwnd);
                }
                else if (wParam == VK_F6 && isActivated) {
                    // F6打开Bug列表
                    ShowBugList(hwnd);
                }
                else if (wParam == VK_F7) {
                    // F7打开日历
                    ShowCalendar(hwnd);
                }
            }
            break;
        }
        
        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            
            if (showGameEgg) {
                return 0;
            }
            
            // "castrate jellyfish"彩蛋界面点击处理
            if (showFOSEgg && fosEggState == 5) {
                // 水母求饶界面点击ESC区域也可以返回
                RECT rect;
                GetClientRect(hwnd, &rect);
                int centerX = rect.right / 2;
                
                // 检查是否点击了ESC提示区域
                HFONT hHintFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                    DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                    DEFAULT_QUALITY, DEFAULT_PITCH, "宋体");
                HDC hdc = GetDC(hwnd);
                HFONT hOldFont = (HFONT)SelectObject(hdc, hHintFont);
                const char* hintText = "按ESC键返回文件管理器";
                int textWidth = GetTextWidth(hdc, hintText);
                int textX = centerX - textWidth / 2;
                int textY = rect.bottom - 50;
                
                if (x >= textX && x <= textX + textWidth && 
                    y >= textY && y <= textY + 20) {
                    showFOSEgg = FALSE;
                    showFileManager = TRUE;
                    fosInputCount = 0;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
                
                SelectObject(hdc, hOldFont);
                DeleteObject(hHintFont);
                ReleaseDC(hwnd, hdc);
                return 0;
            }
            
            // Bug列表点击处理
            if (showBugList) {
                AppRect bugListRect = GetAppRect(hwnd, 8); // 假设8是Bug列表的appType
                
                int relX = x - bugListRect.left;
                int relY = y - bugListRect.top;
                
                // 列表区域
                RECT listRect = {bugListRect.left + 20, bugListRect.top + 50, bugListRect.right - 20, bugListRect.bottom - 80};
                if (relX >= 20 && relX <= (bugListRect.right - bugListRect.left - 20) &&
                    relY >= 50 && relY <= (bugListRect.bottom - bugListRect.top - 80)) {
                    
                    int itemHeight = 25;
                    int startY = 90; // listRect.top + 40 的相对位置
                    int clickIndex = (relY - startY) / itemHeight + bugListScroll;
                    
                    if (clickIndex >= 0 && clickIndex < bugListCount) {
                        selectedBugIndex = clickIndex;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    
                    // 检查复选框点击
                    int col1 = 30; // 第一列位置
                    if (relX >= col1 && relX <= col1 + 15 && 
                        relY >= startY + clickIndex * itemHeight + 5 && 
                        relY <= startY + clickIndex * itemHeight + 20) {
                        if (clickIndex >= 0 && clickIndex < bugListCount) {
                            ToggleBugFixedStatus(clickIndex);
                            InvalidateRect(hwnd, NULL, TRUE);
                        }
                    }
                    return 0;
                }
                
                // 操作按钮区域
                int buttonWidth = 100;
                int buttonHeight = 35;
                int buttonY = bugListRect.bottom - 60;
                int centerX = (bugListRect.right - bugListRect.left) / 2;
                int buttonSpacing = 120;
                
                // 标记修复/取消修复按钮
                RECT toggleButtonRect = {centerX - buttonSpacing * 2, buttonY, centerX - buttonSpacing * 2 + buttonWidth, buttonY + buttonHeight};
                if (relX >= centerX - buttonSpacing * 2 && relX <= centerX - buttonSpacing * 2 + buttonWidth &&
                    relY >= buttonY && relY <= buttonY + buttonHeight && selectedBugIndex >= 0) {
                    ToggleBugFixedStatus(selectedBugIndex);
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                // 删除按钮
                RECT deleteButtonRect = {centerX - buttonSpacing, buttonY, centerX - buttonSpacing + buttonWidth, buttonY + buttonHeight};
                if (relX >= centerX - buttonSpacing && relX <= centerX - buttonSpacing + buttonWidth &&
                    relY >= buttonY && relY <= buttonY + buttonHeight && selectedBugIndex >= 0) {
                    DeleteBug(selectedBugIndex);
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                // 刷新按钮
                RECT refreshButtonRect = {centerX, buttonY, centerX + buttonWidth, buttonY + buttonHeight};
                if (relX >= centerX && relX <= centerX + buttonWidth &&
                    relY >= buttonY && relY <= buttonY + buttonHeight) {
                    LoadBugListFromFile();
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                // 关闭按钮
                RECT closeButtonRect = {centerX + buttonSpacing, buttonY, centerX + buttonSpacing + buttonWidth, buttonY + buttonHeight};
                if (relX >= centerX + buttonSpacing && relX <= centerX + buttonSpacing + buttonWidth &&
                    relY >= buttonY && relY <= buttonY + buttonHeight) {
                    showBugList = FALSE;
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                return 0;
            }
            
            // Bug记录软件点击处理
            if (showBugReporter) {
                AppRect bugRect = GetAppRect(hwnd, 7);
                
                int relX = x - bugRect.left;
                int relY = y - bugRect.top;
                
                if (bugSubmitted) {
                    // 成功界面关闭按钮
                    RECT closeButtonRect = {bugRect.left + 340, bugRect.top + 350, bugRect.left + 460, bugRect.top + 390};
                    if (relX >= 340 && relX <= 460 && relY >= 350 && relY <= 390) {
                        showBugReporter = FALSE;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    return 0;
                }
                
                // 分类选择
                RECT categoryRect = {bugRect.left + 120, bugRect.top + 95, bugRect.left + 370, bugRect.top + 125};
                if (relX >= 120 && relX <= 370 && relY >= 95 && relY <= 125) {
                    int categoryWidth = 250 / 5;
                    int selectedCategory = (relX - 120) / categoryWidth;
                    bugCategory = selectedCategory;
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                // 严重程度选择
                RECT severityRect = {bugRect.left + 120, bugRect.top + 135, bugRect.left + 370, bugRect.top + 165};
                if (relX >= 120 && relX <= 370 && relY >= 135 && relY <= 165) {
                    int severityWidth = 250 / 4;
                    int selectedSeverity = (relX - 120) / severityWidth + 1;
                    bugSeverity = selectedSeverity;
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                // 标题输入框点击
                RECT titleInputRect = {bugRect.left + 120, bugRect.top + 55, bugRect.right - 20, bugRect.top + 85};
                if (relX >= 120 && relX <= (bugRect.right - bugRect.left - 20) && relY >= 55 && relY <= 85) {
                    currentField = 0; // 激活标题输入
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                // 描述输入框点击
                RECT descInputRect = {bugRect.left + 20, bugRect.top + 205, bugRect.right - 20, bugRect.top + 350};
                if (relX >= 20 && relX <= (bugRect.right - bugRect.left - 20) && relY >= 205 && relY <= 350) {
                    currentField = 1; // 激活描述输入
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                // 提交按钮
                RECT submitButtonRect = {bugRect.left + 280, bugRect.top + 550, bugRect.left + 380, bugRect.top + 585};
                if (relX >= 280 && relX <= 380 && relY >= 550 && relY <= 585) {
                    SubmitBugReport();
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                // 取消按钮
                RECT cancelButtonRect = {bugRect.left + 420, bugRect.top + 550, bugRect.left + 520, bugRect.top + 585};
                if (relX >= 420 && relX <= 520 && relY >= 550 && relY <= 585) {
                    showBugReporter = FALSE;
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                return 0;
            }
            
            // 修复：OOBE界面鼠标点击处理
            if (showOOBE) {
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                int centerX = clientRect.right / 2;
                int centerY = clientRect.bottom / 2;
                
                switch (currentStep) {
                    case 0: {
                        // 欢迎界面点击开始设置按钮
                        RECT buttonRect = {centerX - 80, centerY + 80, centerX + 80, centerY + 120};
                        if (x >= buttonRect.left && x <= buttonRect.right &&
                            y >= buttonRect.top && y <= buttonRect.bottom) {
                            currentStep = 1;
                            InvalidateRect(hwnd, NULL, TRUE);
                        }
                        break;
                    }
                    case 2: {
                        // 密码设置界面点击跳过按钮
                        RECT skipButtonRect = {centerX - 200, centerY + 20, centerX + 200, centerY + 60};
                        if (x >= skipButtonRect.left && x <= skipButtonRect.right &&
                            y >= skipButtonRect.top && y <= skipButtonRect.bottom) {
                            skipPasswordSetup = TRUE;
                            currentStep = 3;
                            InvalidateRect(hwnd, NULL, TRUE);
                        }
                        break;
                    }
                    case 3: {
                        // 主题选择界面点击主题
                        int themeWidth = 400 / 4;
                        int themeStartX = centerX - 200;
                        int themeStartY = centerY - 10;
                        
                        for (int i = 0; i < 4; i++) {
                            int themeX = themeStartX + i * themeWidth;
                            if (x >= themeX && x <= themeX + themeWidth &&
                                y >= themeStartY && y <= themeStartY + 80) {
                                currentThemeIndex = i;
                                InvalidateRect(hwnd, NULL, TRUE);
                                break;
                            }
                        }
                        
                        // 完成配置按钮
                        RECT completeButtonRect = {centerX - 80, centerY + 130, centerX + 80, centerY + 170};
                        if (x >= completeButtonRect.left && x <= completeButtonRect.right &&
                            y >= completeButtonRect.top && y <= completeButtonRect.bottom) {
                            if (ValidateOOBEInput()) {
                                CompleteOOBE(hwnd);
                            }
                        }
                        break;
                    }
                }
                return 0;
            }
            
            if (showSettings) {
                AppRect settingsRect = GetAppRect(hwnd, 6);
                
                int relX = x - settingsRect.left;
                int relY = y - settingsRect.top;
                
                // 检查主题选择
                if (settingsCurrentStep == 2) {
                    int themeWidth = 400 / 4;
                    int themeStartX = settingsRect.left + 50;
                    int themeStartY = settingsRect.top + 220;
                    
                    for (int i = 0; i < 4; i++) {
                        int themeX = themeStartX + i * themeWidth;
                        if (relX >= themeX && relX <= themeX + themeWidth &&
                            relY >= themeStartY && relY <= themeStartY + 120) {
                            currentThemeIndex = i;
                            InvalidateRect(hwnd, NULL, TRUE);
                            return 0;
                        }
                    }
                }
                
                // 检查导航按钮点击
                int buttonWidth = 100;
                int buttonHeight = 35;
                int buttonY = settingsRect.top + 480;
                
                // 上一步按钮
                if (settingsCurrentStep > 0) {
                    RECT prevButtonRect = {settingsRect.left + 130, buttonY, settingsRect.left + 130 + buttonWidth, buttonY + buttonHeight};
                    if (relX >= 130 && relX <= 130 + buttonWidth && relY >= 480 && relY <= 480 + buttonHeight) {
                        settingsCurrentStep--;
                        InvalidateRect(hwnd, NULL, TRUE);
                        return 0;
                    }
                }
                
                // 下一步/应用按钮
                RECT nextButtonRect = {settingsRect.left + 250, buttonY, settingsRect.left + 250 + buttonWidth, buttonY + buttonHeight};
                if (relX >= 250 && relX <= 250 + buttonWidth && relY >= 480 && relY <= 480 + buttonHeight) {
                    if (ValidateSettingsInput()) {
                        if (settingsCurrentStep < 2) {
                            settingsCurrentStep++;
                        } else {
                            ApplySettings();
                            showSettings = FALSE;
                        }
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    return 0;
                }
                
                // 取消按钮
                RECT cancelButtonRect = {settingsRect.left + 370, buttonY, settingsRect.left + 370 + buttonWidth, buttonY + buttonHeight};
                if (relX >= 370 && relX <= 370 + buttonWidth && relY >= 480 && relY <= 480 + buttonHeight) {
                    showSettings = FALSE;
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                return 0;
            }
            
            if (showFileEditor) {
                AppRect editorRect = GetAppRect(hwnd, 5);
                
                int relX = x - editorRect.left;
                int relY = y - editorRect.top;
                
                // 检查工具栏按钮点击
                RECT saveButton = {editorRect.left + 10, editorRect.top + 35, editorRect.left + 60, editorRect.top + 55};
                if (relX >= 10 && relX <= 60 && relY >= 35 && relY <= 55) {
                    SaveFileContent();
                    return 0;
                }
                
                RECT exitButton = {editorRect.left + 70, editorRect.top + 35, editorRect.left + 120, editorRect.top + 55};
                if (relX >= 70 && relX <= 120 && relY >= 35 && relY <= 55) {
                    showFileEditor = FALSE;
                    showFileManager = TRUE;
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                RECT formatButton = {editorRect.left + 130, editorRect.top + 35, editorRect.left + 200, editorRect.top + 55};
                if (relX >= 130 && relX <= 200 && relY >= 35 && relY <= 55) {
                    textFormat = (textFormat + 1) % 4;
                    InvalidateRect(hwnd, NULL, TRUE);
                    return 0;
                }
                
                // 编辑区域点击处理
                RECT editRect = {editorRect.left + 10, editorRect.top + 70, editorRect.right - 10, editorRect.bottom - 10};
                if (relX >= 10 && relX <= (editorRect.right - editorRect.left - 10) &&
                    relY >= 70 && relY <= (editorRect.bottom - editorRect.top - 10)) {
                    isEditing = TRUE;
                    cursorPosition = strlen(fileContent);
                    InvalidateRect(hwnd, NULL, TRUE);
                }
                
                return 0;
            }
            
            if (showDesktop) {
                // 修复：统一的任务栏按钮位置计算
                int buttonWidth = 100;
                int buttonHeight = 40;
                int buttonY = 10;
                int buttonStartX = 200;
                int buttonSpacing = 110;
                int buttonIndex = 0;
                
                // 显示桌面按钮
                int desktopX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT desktopRect = {desktopX, buttonY, desktopX + buttonWidth, buttonY + buttonHeight};
                if (x >= desktopRect.left && x <= desktopRect.right &&
                    y >= desktopRect.top && y <= desktopRect.bottom) {
                    isDesktopPressed = TRUE;
                    InvalidateRect(hwnd, &desktopRect, TRUE);
                    return 0;
                }
                
                // 文件管理器按钮
                int fileX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT fileRect = {fileX, buttonY, fileX + buttonWidth, buttonY + buttonHeight};
                if (x >= fileRect.left && x <= fileRect.right &&
                    y >= fileRect.top && y <= fileRect.bottom) {
                    isFileManagerPressed = TRUE;
                    InvalidateRect(hwnd, &fileRect, TRUE);
                    return 0;
                }
                
                // 计算器按钮
                int calcX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT calcRect = {calcX, buttonY, calcX + buttonWidth, buttonY + buttonHeight};
                if (x >= calcRect.left && x <= calcRect.right &&
                    y >= calcRect.top && y <= calcRect.bottom) {
                    isCalcPressed = TRUE;
                    InvalidateRect(hwnd, &calcRect, TRUE);
                    return 0;
                }
                
                // 系统信息按钮
                int sysinfoX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT sysinfoRect = {sysinfoX, buttonY, sysinfoX + buttonWidth, buttonY + buttonHeight};
                if (x >= sysinfoRect.left && x <= sysinfoRect.right &&
                    y >= sysinfoRect.top && y <= sysinfoRect.bottom) {
                    isSysInfoPressed = TRUE;
                    InvalidateRect(hwnd, &sysinfoRect, TRUE);
                    return 0;
                }
                
                // 设置按钮（专业版）
                if (isActivated) {
                    int settingsX = buttonStartX + buttonSpacing * buttonIndex++;
                    RECT settingsRect = {settingsX, buttonY, settingsX + buttonWidth, buttonY + buttonHeight};
                    if (x >= settingsRect.left && x <= settingsRect.right &&
                        y >= settingsRect.top && y <= settingsRect.bottom) {
                        isSettingsPressed = TRUE;
                        InvalidateRect(hwnd, &settingsRect, TRUE);
                        return 0;
                    }
                }
                
                // 水母游戏按钮
                if (showGameButton) {
                    int gameX = buttonStartX + buttonSpacing * buttonIndex++;
                    RECT gameRect = {gameX, buttonY, gameX + buttonWidth, buttonY + buttonHeight};
                    if (x >= gameRect.left && x <= gameRect.right &&
                        y >= gameRect.top && y <= gameRect.bottom) {
                        isGamePressed = TRUE;
                        InvalidateRect(hwnd, &gameRect, TRUE);
                        return 0;
                    }
                }
                
                // Bug报告按钮（专业版功能）
                if (isActivated) {
                    int bugX = buttonStartX + buttonSpacing * buttonIndex++;
                    RECT bugRect = {bugX, buttonY, bugX + buttonWidth, buttonY + buttonHeight};
                    if (x >= bugRect.left && x <= bugRect.right &&
                        y >= bugRect.top && y <= bugRect.bottom) {
                        isBugPressed = TRUE;
                        InvalidateRect(hwnd, &bugRect, TRUE);
                        return 0;
                    }
                }
                
                // Bug列表按钮（专业版功能）
                if (isActivated) {
                    int bugListX = buttonStartX + buttonSpacing * buttonIndex++;
                    RECT bugListRect = {bugListX, buttonY, bugListX + buttonWidth, buttonY + buttonHeight};
                    if (x >= bugListRect.left && x <= bugListRect.right &&
                        y >= bugListRect.top && y <= bugListRect.bottom) {
                        isBugListPressed = TRUE;
                        InvalidateRect(hwnd, &bugListRect, TRUE);
                        return 0;
                    }
                }
                
                // 日历按钮
                int calendarX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT calendarRect = {calendarX, buttonY, calendarX + buttonWidth, buttonY + buttonHeight};
                if (x >= calendarRect.left && x <= calendarRect.right &&
                    y >= calendarRect.top && y <= calendarRect.bottom) {
                    isCalendarPressed = TRUE;
                    InvalidateRect(hwnd, &calendarRect, TRUE);
                    return 0;
                }
                
                // 关机按钮
                int shutdownX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT shutdownRect = {shutdownX, buttonY, shutdownX + buttonWidth, buttonY + buttonHeight};
                if (x >= shutdownRect.left && x <= shutdownRect.right &&
                    y >= shutdownRect.top && y <= shutdownRect.bottom) {
                    isShutdownPressed = TRUE;
                    InvalidateRect(hwnd, &shutdownRect, TRUE);
                    return 0;
                }
            }
            else if (showCDKeyScreen) {
                int centerX = GetSystemMetrics(SM_CXSCREEN) / 2;
                int centerY = GetSystemMetrics(SM_CYSCREEN) / 2;
                int buttonY = centerY + 180;
                int buttonWidth = 120;
                int buttonHeight = 40;
                
                // 激活按钮
                if (x >= centerX - 270 && x <= centerX - 270 + buttonWidth && 
                    y >= buttonY && y <= buttonY + buttonHeight) {
                    HandleCDKeyInput(13);
                }
                // 试用按钮
                else if (x >= centerX - 130 && x <= centerX - 130 + buttonWidth && 
                         y >= buttonY && y <= buttonY + buttonHeight) {
                    StartTrialMode(hwnd);
                }
                // 退出按钮
                else if (x >= centerX + 10 && x <= centerX + 10 + buttonWidth && 
                         y >= buttonY && y <= buttonY + buttonHeight) {
                    PostQuitMessage(0);
                }
            }
            else if (showCalculator && !showGameEgg) {
                AppRect calcRect = GetAppRect(hwnd, 1);
                
                int relX = x - calcRect.left;
                int relY = y - calcRect.top;
                
                if (showFinalEgg) {
                    int centerX = (calcRect.right - calcRect.left) / 2;
                    int centerY = (calcRect.bottom - calcRect.top) / 2;
                    RECT closeButtonRect = {centerX - 60, centerY + 180, centerX + 60, centerY + 220};
                    
                    if (relX >= closeButtonRect.left && relX <= closeButtonRect.right &&
                        relY >= closeButtonRect.top && relY <= closeButtonRect.bottom) {
                        showFinalEgg = FALSE;
                        showSpecialMessage = FALSE;
                        specialMessageScrollY = 0;
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                    return 0;
                }
                
                if (showSpecialMessage) {
                    return 0;
                }
                
                // 正常计算器按钮布局
                int buttonWidth = 70;
                int buttonHeight = 40;
                int startX = calcRect.left + 35;
                int startY = calcRect.top + 130;
                
                // 第一行按钮
                if (relY >= startY && relY <= startY + buttonHeight) {
                    if (relX >= startX && relX <= startX + buttonWidth) {
                        CalculatorButtonClick("7");
                    } else if (relX >= startX + 80 && relX <= startX + 80 + buttonWidth) {
                        CalculatorButtonClick("8");
                    } else if (relX >= startX + 160 && relX <= startX + 160 + buttonWidth) {
                        CalculatorButtonClick("9");
                    } else if (relX >= startX + 240 && relX <= startX + 240 + buttonWidth) {
                        CalculatorButtonClick("+");
                    }
                }
                // 第二行按钮
                else if (relY >= startY + 50 && relY <= startY + 50 + buttonHeight) {
                    if (relX >= startX && relX <= startX + buttonWidth) {
                        CalculatorButtonClick("4");
                    } else if (relX >= startX + 80 && relX <= startX + 80 + buttonWidth) {
                        CalculatorButtonClick("5");
                    } else if (relX >= startX + 160 && relX <= startX + 160 + buttonWidth) {
                        CalculatorButtonClick("6");
                    } else if (relX >= startX + 240 && relX <= startX + 240 + buttonWidth) {
                        CalculatorButtonClick("-");
                    }
                }
                // 第三行按钮
                else if (relY >= startY + 100 && relY <= startY + 100 + buttonHeight) {
                    if (relX >= startX && relX <= startX + buttonWidth) {
                        CalculatorButtonClick("1");
                    } else if (relX >= startX + 80 && relX <= startX + 80 + buttonWidth) {
                        CalculatorButtonClick("2");
                    } else if (relX >= startX + 160 && relX <= startX + 160 + buttonWidth) {
                        CalculatorButtonClick("3");
                    } else if (relX >= startX + 240 && relX <= startX + 240 + buttonWidth) {
                        CalculatorButtonClick("*");
                    }
                }
                // 第四行按钮
                else if (relY >= startY + 150 && relY <= startY + 150 + buttonHeight) {
                    if (relX >= startX && relX <= startX + buttonWidth) {
                        CalculatorButtonClick("0");
                    } else if (relX >= startX + 80 && relX <= startX + 80 + buttonWidth) {
                        CalculatorButtonClick("C");
                    } else if (relX >= startX + 160 && relX <= startX + 160 + buttonWidth) {
                        CalculatorButtonClick("=");
                        CheckSpecialMessage();
                    } else if (relX >= startX + 240 && relX <= startX + 240 + buttonWidth) {
                        CalculatorButtonClick("/");
                    }
                }
                // 第五行按钮（小数点）
                else if (relY >= startY + 200 && relY <= startY + 200 + buttonHeight) {
                    if (relX >= startX && relX <= startX + buttonWidth) {
                        CalculatorButtonClick(".");
                    }
                }
                
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }
            
            // 日历软件点击处理
            if (showCalendar && !showEventDialog) {
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                int centerX = clientRect.right / 2;
                int centerY = clientRect.bottom / 2;
                HandleCalendarMouseClick(x, y, centerX, centerY);
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
            }
            break;
        }
        
        case WM_LBUTTONUP: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            
            if (showDesktop) {
                // 修复：统一的任务栏按钮位置计算
                int buttonWidth = 100;
                int buttonHeight = 40;
                int buttonY = 10;
                int buttonStartX = 200;
                int buttonSpacing = 110;
                int buttonIndex = 0;
                
                // 显示桌面按钮
                int desktopX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT desktopRect = {desktopX, buttonY, desktopX + buttonWidth, buttonY + buttonHeight};
                if (isDesktopPressed) {
                    isDesktopPressed = FALSE;
                    InvalidateRect(hwnd, &desktopRect, TRUE);
                    
                    if (x >= desktopRect.left && x <= desktopRect.right &&
                        y >= desktopRect.top && y <= desktopRect.bottom) {
                        CloseAllApps(hwnd);
                    }
                }
                
                // 文件管理器按钮
                int fileX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT fileRect = {fileX, buttonY, fileX + buttonWidth, buttonY + buttonHeight};
                if (isFileManagerPressed) {
                    isFileManagerPressed = FALSE;
                    InvalidateRect(hwnd, &fileRect, TRUE);
                    
                    if (x >= fileRect.left && x <= fileRect.right &&
                        y >= fileRect.top && y <= fileRect.bottom) {
                        ShowFileManager(hwnd);
                    }
                }
                
                // 计算器按钮
                int calcX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT calcRect = {calcX, buttonY, calcX + buttonWidth, buttonY + buttonHeight};
                if (isCalcPressed) {
                    isCalcPressed = FALSE;
                    InvalidateRect(hwnd, &calcRect, TRUE);
                    
                    if (x >= calcRect.left && x <= calcRect.right &&
                        y >= calcRect.top && y <= calcRect.bottom) {
                        ShowCalculator(hwnd);
                    }
                }
                
                // 系统信息按钮
                int sysinfoX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT sysinfoRect = {sysinfoX, buttonY, sysinfoX + buttonWidth, buttonY + buttonHeight};
                if (isSysInfoPressed) {
                    isSysInfoPressed = FALSE;
                    InvalidateRect(hwnd, &sysinfoRect, TRUE);
                    
                    if (x >= sysinfoRect.left && x <= sysinfoRect.right &&
                        y >= sysinfoRect.top && y <= sysinfoRect.bottom) {
                        ShowSystemInfo(hwnd);
                    }
                }
                
                // 设置按钮（专业版）
                if (isActivated) {
                    int settingsX = buttonStartX + buttonSpacing * buttonIndex++;
                    RECT settingsRect = {settingsX, buttonY, settingsX + buttonWidth, buttonY + buttonHeight};
                    if (isSettingsPressed) {
                        isSettingsPressed = FALSE;
                        InvalidateRect(hwnd, &settingsRect, TRUE);
                        
                        if (x >= settingsRect.left && x <= settingsRect.right &&
                            y >= settingsRect.top && y <= settingsRect.bottom) {
                            ShowSettings(hwnd);
                        }
                    }
                }
                
                // 水母游戏按钮
                if (showGameButton) {
                    int gameX = buttonStartX + buttonSpacing * buttonIndex++;
                    RECT gameRect = {gameX, buttonY, gameX + buttonWidth, buttonY + buttonHeight};
                    if (isGamePressed) {
                        isGamePressed = FALSE;
                        InvalidateRect(hwnd, &gameRect, TRUE);
                        
                        if (x >= gameRect.left && x <= gameRect.right &&
                            y >= gameRect.top && y <= gameRect.bottom) {
                            showGameEgg = TRUE;
                            showDesktop = FALSE;
                            if (!gameInitialized) {
                                InitializeJellyfishGame();
                                gameInitialized = TRUE;
                            }
                            SetTimer(hwnd, 9, 50, NULL);
                            InvalidateRect(hwnd, NULL, TRUE);
                        }
                    }
                }
                
                // Bug报告按钮（专业版功能）
                if (isActivated) {
                    int bugX = buttonStartX + buttonSpacing * buttonIndex++;
                    RECT bugRect = {bugX, buttonY, bugX + buttonWidth, buttonY + buttonHeight};
                    if (isBugPressed) {
                        isBugPressed = FALSE;
                        InvalidateRect(hwnd, &bugRect, TRUE);
                        
                        if (x >= bugRect.left && x <= bugRect.right &&
                            y >= bugRect.top && y <= bugRect.bottom) {
                            ShowBugReporter(hwnd);
                        }
                    }
                }
                
                // Bug列表按钮（专业版功能）
                if (isActivated) {
                    int bugListX = buttonStartX + buttonSpacing * buttonIndex++;
                    RECT bugListRect = {bugListX, buttonY, bugListX + buttonWidth, buttonY + buttonHeight};
                    if (isBugListPressed) {
                        isBugListPressed = FALSE;
                        InvalidateRect(hwnd, &bugListRect, TRUE);
                        
                        if (x >= bugListRect.left && x <= bugListRect.right &&
                            y >= bugListRect.top && y <= bugListRect.bottom) {
                            ShowBugList(hwnd);
                        }
                    }
                }
                
                // 日历按钮
                int calendarX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT calendarRect = {calendarX, buttonY, calendarX + buttonWidth, buttonY + buttonHeight};
                if (isCalendarPressed) {
                    isCalendarPressed = FALSE;
                    InvalidateRect(hwnd, &calendarRect, TRUE);
                    
                    if (x >= calendarRect.left && x <= calendarRect.right &&
                        y >= calendarRect.top && y <= calendarRect.bottom) {
                        ShowCalendar(hwnd);
                    }
                }
                
                // 关机按钮
                int shutdownX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT shutdownRect = {shutdownX, buttonY, shutdownX + buttonWidth, buttonY + buttonHeight};
                if (isShutdownPressed) {
                    isShutdownPressed = FALSE;
                    InvalidateRect(hwnd, &shutdownRect, TRUE);
                    
                    if (x >= shutdownRect.left && x <= shutdownRect.right &&
                        y >= shutdownRect.top && y <= shutdownRect.bottom) {
                        ShutdownSystem(hwnd);
                    }
                }
            }
            break;
        }
        
        case WM_MOUSEMOVE: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            
            if (showDesktop) {
                // 修复：统一的任务栏按钮位置计算
                int buttonWidth = 100;
                int buttonHeight = 40;
                int buttonY = 10;
                int buttonStartX = 200;
                int buttonSpacing = 110;
                int buttonIndex = 0;
                
                // 显示桌面按钮
                int desktopX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT desktopRect = {desktopX, buttonY, desktopX + buttonWidth, buttonY + buttonHeight};
                BOOL wasDesktopHovered = isDesktopHovered;
                isDesktopHovered = (x >= desktopRect.left && x <= desktopRect.right &&
                                  y >= desktopRect.top && y <= desktopRect.bottom);
                
                // 文件管理器按钮
                int fileX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT fileRect = {fileX, buttonY, fileX + buttonWidth, buttonY + buttonHeight};
                BOOL wasFileManagerHovered = isFileManagerHovered;
                isFileManagerHovered = (x >= fileRect.left && x <= fileRect.right &&
                                      y >= fileRect.top && y <= fileRect.bottom);
                
                // 计算器按钮
                int calcX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT calcRect = {calcX, buttonY, calcX + buttonWidth, buttonY + buttonHeight};
                BOOL wasCalcHovered = isCalcHovered;
                isCalcHovered = (x >= calcRect.left && x <= calcRect.right &&
                               y >= calcRect.top && y <= calcRect.bottom);
                
                // 系统信息按钮
                int sysinfoX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT sysinfoRect = {sysinfoX, buttonY, sysinfoX + buttonWidth, buttonY + buttonHeight};
                BOOL wasSysInfoHovered = isSysInfoHovered;
                isSysInfoHovered = (x >= sysinfoRect.left && x <= sysinfoRect.right &&
                                  y >= sysinfoRect.top && y <= sysinfoRect.bottom);
                
                // 设置按钮悬停（专业版）
                BOOL wasSettingsHovered = isSettingsHovered;
                if (isActivated) {
                    int settingsX = buttonStartX + buttonSpacing * buttonIndex++;
                    RECT settingsRect = {settingsX, buttonY, settingsX + buttonWidth, buttonY + buttonHeight};
                    isSettingsHovered = (x >= settingsRect.left && x <= settingsRect.right &&
                                       y >= settingsRect.top && y <= settingsRect.bottom);
                } else {
                    isSettingsHovered = FALSE;
                }
                
                // 水母游戏按钮悬停
                BOOL wasGameHovered = isGameHovered;
                if (showGameButton) {
                    int gameX = buttonStartX + buttonSpacing * buttonIndex++;
                    RECT gameRect = {gameX, buttonY, gameX + buttonWidth, buttonY + buttonHeight};
                    isGameHovered = (x >= gameRect.left && x <= gameRect.right &&
                                   y >= gameRect.top && y <= gameRect.bottom);
                } else {
                    isGameHovered = FALSE;
                }
                
                // Bug报告按钮悬停
                BOOL wasBugHovered = isBugHovered;
                if (isActivated) {
                    int bugX = buttonStartX + buttonSpacing * buttonIndex++;
                    RECT bugRect = {bugX, buttonY, bugX + buttonWidth, buttonY + buttonHeight};
                    isBugHovered = (x >= bugRect.left && x <= bugRect.right &&
                                  y >= bugRect.top && y <= bugRect.bottom);
                } else {
                    isBugHovered = FALSE;
                }
                
                // Bug列表按钮悬停
                BOOL wasBugListHovered = isBugListHovered;
                if (isActivated) {
                    int bugListX = buttonStartX + buttonSpacing * buttonIndex++;
                    RECT bugListRect = {bugListX, buttonY, bugListX + buttonWidth, buttonY + buttonHeight};
                    isBugListHovered = (x >= bugListRect.left && x <= bugListRect.right &&
                                      y >= bugListRect.top && y <= bugListRect.bottom);
                } else {
                    isBugListHovered = FALSE;
                }
                
                // 日历按钮悬停
                int calendarX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT calendarRect = {calendarX, buttonY, calendarX + buttonWidth, buttonY + buttonHeight};
                BOOL wasCalendarHovered = isCalendarHovered;
                isCalendarHovered = (x >= calendarRect.left && x <= calendarRect.right &&
                                   y >= calendarRect.top && y <= calendarRect.bottom);
                
                // 关机按钮悬停
                int shutdownX = buttonStartX + buttonSpacing * buttonIndex++;
                RECT shutdownRect = {shutdownX, buttonY, shutdownX + buttonWidth, buttonY + buttonHeight};
                BOOL wasShutdownHovered = isShutdownHovered;
                isShutdownHovered = (x >= shutdownRect.left && x <= shutdownRect.right &&
                                   y >= shutdownRect.top && y <= shutdownRect.bottom);
                
                if (wasDesktopHovered != isDesktopHovered || 
                    wasFileManagerHovered != isFileManagerHovered ||
                    wasCalcHovered != isCalcHovered ||
                    wasSysInfoHovered != isSysInfoHovered ||
                    wasSettingsHovered != isSettingsHovered ||
                    wasGameHovered != isGameHovered ||
                    wasBugHovered != isBugHovered ||
                    wasBugListHovered != isBugListHovered ||
                    wasCalendarHovered != isCalendarHovered ||
                    wasShutdownHovered != isShutdownHovered) {
                    RECT taskbarRect = {0, 0, GetSystemMetrics(SM_CXSCREEN), 60};
                    InvalidateRect(hwnd, &taskbarRect, TRUE);
                }
                
                if (isDesktopHovered || isFileManagerHovered || isCalcHovered || 
                    isSysInfoHovered || isSettingsHovered || isGameHovered || 
                    isBugHovered || isBugListHovered || isCalendarHovered || 
                    isShutdownHovered) {
                    SetCursor(LoadCursor(NULL, IDC_HAND));
                } else {
                    SetCursor(LoadCursor(NULL, IDC_ARROW));
                }
            }
            break;
        }
        
        case WM_DESTROY: {
            // 停止所有定时器
            KillTimer(hwnd, 1);
            KillTimer(hwnd, 2);
            KillTimer(hwnd, 3);
            KillTimer(hwnd, 4);
            KillTimer(hwnd, 5);
            KillTimer(hwnd, 6);
            KillTimer(hwnd, 7);
            KillTimer(hwnd, 8);
            KillTimer(hwnd, 9);
            
            // 清理IME
            if (hIMC) {
                ImmReleaseContext(hwnd, hIMC);
                hIMC = NULL;
            }
            
            // 清理资源
            if (hBackgroundBitmap) {
                DeleteObject(hBackgroundBitmap);
            }
            if (hEthernetBackground) {
                DeleteObject(hEthernetBackground);
            }
            
            // 保存配置和日历事件
            SaveConfig();
            SaveCalendarEvents(); // 保存日历事件
            
            PostQuitMessage(0);
            break;
        }
        
        case WM_ERASEBKGND: {
            // 防止闪烁
            return 1;
        }
        
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            
            RECT rect;
            GetClientRect(hwnd, &rect);
            int centerX = rect.right / 2;
            int centerY = rect.bottom / 2;
            
            if (showPurpleScreen) {
                DrawCriticalPurpleScreen(hdc, rect);
                EndPaint(hwnd, &ps);
                return 0;
            }
            
            // 水母游戏优先绘制
            if (showGameEgg) {
                DrawJellyfishGame(hdc, hwnd);
                EndPaint(hwnd, &ps);
                return 0;
            }
            
            // fos彩蛋绘制（包含彩蛋）
            if (showFOSEgg) {
                DrawFOSEgg(hdc, hwnd);
                EndPaint(hwnd, &ps);
                return 0;
            }
            
            if (showFOSBeta) {
                DrawFOSBetaAnimation(hdc, rect);
            }
            else if (showBootAnimation) {
                DrawBootAnimation(hdc, rect);
            }
            else if (showShutdown) {
                DrawShutdownAnimation(hdc, rect);
            }
            else if (showCDKeyScreen) {
                DrawCDKeyInterface(hdc, centerX, centerY);
            }
            else if (showDesktop) {
                DrawDesktop(hdc, rect);
                DrawTaskbar(hdc, rect);
                
                if (showCalculator) {
                    DrawCalculator(hdc, centerX, centerY);
                }
                if (showSystemInfo) {
                    DrawSystemInfo(hdc, centerX, centerY);
                }
                if (showFileManager) {
                    DrawFileManager(hdc, centerX, centerY);
                }
                if (showFileEditor) {
                    DrawFileEditor(hdc, centerX, centerY);
                }
                if (showSettings) {
                    DrawSettings(hdc, centerX, centerY);
                }
                if (showBugReporter) {
                    DrawBugReporter(hdc, centerX, centerY);
                }
                if (showBugList) {
                    DrawBugList(hdc, centerX, centerY);
                }
                if (showCalendar) {
                    DrawCalendar(hdc, centerX, centerY);
                }
            } else {
                DrawVistaBackground(hdc, rect);
                
                SetBkMode(hdc, TRANSPARENT);
                SetTextColor(hdc, RGB(255, 255, 255));
                
                if (showWelcome) {
                    HFONT hTitleFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
                    HFONT hNormalFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
                    
                    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
                    TextOut(hdc, centerX - 180, centerY - 100, "系统应急修复程序", 16);
                    SelectObject(hdc, hNormalFont);
                    TextOut(hdc, centerX - 180, centerY - 30, "我们检测到您的系统出现了问题", 28);
                    TextOut(hdc, centerX - 180, centerY + 10, "我们进入了此修补程序", 20);
                    TextOut(hdc, centerX - 180, centerY + 50, "你的文件将会保留，如果进度条卡住，请按空格键以推动进度条", 56);
                    SelectObject(hdc, hOldFont);
                    
                    DrawVistaProgressBar(hdc, centerX - 200, centerY + 100, 400, 20, progressValue);
                    
                    DeleteObject(hTitleFont);
                    DeleteObject(hNormalFont);
                } 
                else if (showSecondScreen) {
                    HFONT hTitleFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
                    HFONT hNormalFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
                    
                    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
                    TextOut(hdc, centerX - 180, centerY - 120, "系统应急修复程序", 16);
                    SelectObject(hdc, hNormalFont);
                    TextOut(hdc, centerX - 180, centerY - 50, "请稍后...", 9);
                    TextOut(hdc, centerX - 180, centerY, "正在初始化应急修复程序，如果进度条卡住，请按空格键以推动进度条（此问题由于配置文件被修改导致读取失败而产生的溢出）", 114);
                    TextOut(hdc, centerX - 180, centerY + 50, "正在启动: 修补程序", 18);
                    SelectObject(hdc, hOldFont);
                    
                    DrawVistaProgressBar(hdc, centerX - 200, centerY + 100, 400, 20, progressValue);
                    
                    DeleteObject(hTitleFont);
                    DeleteObject(hNormalFont);
                } 
                else if (showOOBE) {
                    DrawOOBEInterface(hdc, centerX, centerY);
                }
                else if (showLogin) {
                    DrawLoginInterface(hdc, centerX, centerY);
                }
                else if (showComplete) {
                    HFONT hTitleFont = CreateFont(36, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
                    HFONT hNormalFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
                    
                    HFONT hOldFont = (HFONT)SelectObject(hdc, hTitleFont);
                    TextOut(hdc, centerX - 200, centerY - 120, "配置完成！", 10);
                    SelectObject(hdc, hNormalFont);
                    TextOut(hdc, centerX - 200, centerY - 50, "欢迎您，", 8);
                    TextOut(hdc, centerX - 120, centerY - 50, username, strlen(username));
                    TextOut(hdc, centerX - 200, centerY, "您的账户已成功创建", 18);
                    TextOut(hdc, centerX - 200, centerY + 50, "系统即将重启...", 14);
                    SelectObject(hdc, hOldFont);
                    
                    DeleteObject(hTitleFont);
                    DeleteObject(hNormalFont);
                }
            }
            
            EndPaint(hwnd, &ps);
            break;
        }
        default:
            return DefWindowProc(hwnd, Message, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;

    memset(&wc, 0, sizeof(wc));
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInstance;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszClassName = "f系统1.0-codename nori(music plus!!!)";
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);
    wc.style         = CS_HREDRAW | CS_VREDRAW;

    if(!RegisterClassEx(&wc)) {
        MessageBox(NULL, "窗口注册失败！","错误!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    hwnd = CreateWindowEx(
        WS_EX_CLIENTEDGE,
        "f系统1.0-codename nori(music plus!!!)",
        "f系统1.0专业版(music plus!!!)codename nori",
        WS_POPUP | WS_VISIBLE,
        0, 0,
        screenWidth, screenHeight,
        NULL, NULL, hInstance, NULL
    );

    if(hwnd == NULL) {
        MessageBox(NULL, "窗口创建失败!","错误!",MB_ICONEXCLAMATION|MB_OK);
        return 0;
    }

    ShowWindow(hwnd, SW_MAXIMIZE);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

