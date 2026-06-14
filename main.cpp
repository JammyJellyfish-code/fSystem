/*
 * f系统1.0 专业版 - 个人项目 / f System 1.0 Professional - Personal Project
 * 版权所有 (C) 2020~9999 JammyJellyfish@hotmail.com / Copyright (C) 2020~9999 JammyJellyfish@hotmail.com
 * 
 * 许可条款 / License Terms：
 * 1. 仅供个人学习和研究使用 / For personal learning and research use only
 * 2. 禁止商业使用 / Commercial use prohibited
 * 3. 禁止未经授权的分发 / Unauthorized distribution prohibited
 * 
 * 开发者：JammyJellyfish@hotmail.com / Developer: JammyJellyfish@hotmail.com
 * 最后更新：2025年 / Last Updated: 2025
 */
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <commdlg.h>
#include <shellapi.h>
#include <direct.h>  // 添加目录操作支持
#include <algorithm> // 添加algorithm头文件用于min/max函数

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
static HBITMAP hBackgroundBitmap = NULL;

// CD-KEY变量
static char cdKeyInput[50] = "";
static BOOL cdKeyError = FALSE;
static char cdKeyErrorMsg[100] = "";

// OOBE配置变量
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
static char osVersion[100] = "fOS 1.0 Professional (Build 200)";
static char cpuInfo[100] = "32位架构 第1代 型号1 修订版本100";
static char memoryInfo[100] = "512 MB RAM";
static char buildInfo[100] = "Build 200 (fOS.1.0.Professional)";
static char versionInfo[200] = "fOS 1.0 Professional\n版本 1.0.200.2025\n编译于 2025年";
static DWORD lastSystemInfoUpdate = 0;

// 文件管理器变量 - 改为真实文件操作
static char currentPath[MAX_PATH] = "";
static char fileList[100][256];
static int fileCount = 0;
static char commandInput[100] = "";  // 命令输入
static BOOL commandMode = FALSE;     // 命令模式标志
static char commandOutput[2000] = ""; // 命令输出 - 增大缓冲区

// 文件编辑器变量
static BOOL showFileEditor = FALSE;
static char editFileName[256] = "";
static char fileContent[10000] = "";  // 文件内容缓冲区
static int cursorPosition = 0;
static int scrollPosition = 0;
static BOOL isEditing = FALSE;
static int textFormat = 0;  // 0:普通文本 1:代码 2:富文本 3:Markdown

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

// 主题颜色定义
typedef struct {
    COLORREF desktopColor;
    COLORREF taskbarColor;
    COLORREF accentColor;
    COLORREF buttonColor;
    char themeName[20];
} Theme;

static Theme themes[] = {
    {RGB(255, 200, 220), RGB(60, 60, 60), RGB(0, 70, 140), RGB(80, 80, 80), "默认粉色"},
    {RGB(200, 220, 255), RGB(50, 70, 100), RGB(30, 100, 200), RGB(70, 100, 150), "专业蓝"},
    {RGB(220, 255, 220), RGB(60, 100, 70), RGB(40, 160, 80), RGB(80, 120, 90), "清新绿"},
    {RGB(240, 220, 255), RGB(80, 60, 100), RGB(120, 80, 180), RGB(100, 80, 130), "优雅紫"}
};

static int currentThemeIndex = 0;

// 版本信息
#define MAJOR_VERSION 1
#define MINOR_VERSION 0
#define BUILD_VERSION 200
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
        default:
            rect.left = rect.top = rect.right = rect.bottom = 0;
    }
    
    return rect;
}

// 简易设置软件实现

// 显示设置软件
void ShowSettings(HWND hwnd) {
    CloseAllApps(hwnd);
    showSettings = TRUE;
    settingsCurrentStep = 0;
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
            
            DrawInputBox(hdc, centerX - 200, centerY - 120, 400, 35, "新用户名:", newUsername, TRUE, FALSE, "输入新用户名");
            DrawInputBox(hdc, centerX - 200, centerY - 50, 400, 35, "新密码:", newPassword, FALSE, TRUE, "输入新密码（留空保持不变）");
            DrawInputBox(hdc, centerX - 200, centerY + 20, 400, 35, "确认密码:", newConfirmPassword, FALSE, TRUE, "确认新密码");
            
            if (settingsError) {
                SetTextColor(hdc, VISTA_RED);
                TextOut(hdc, centerX - 180, centerY + 70, settingsErrorMsg, strlen(settingsErrorMsg));
                SetTextColor(hdc, RGB(0, 0, 0));
            }
            
            SelectObject(hdc, hSmallFont);
            TextOut(hdc, centerX - 180, centerY + 100, "提示：密码至少需要4个字符，留空表示不修改密码", 44);
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
            TextOut(hdc, centerX - 120, centerY + 30, "fOS 1.0 Professional", 20);
            
            if (settingsError) {
                SetTextColor(hdc, VISTA_RED);
                TextOut(hdc, centerX - 180, centerY + 80, settingsErrorMsg, strlen(settingsErrorMsg));
                SetTextColor(hdc, RGB(0, 0, 0));
            }
            
            break;
        }
            
        case 2: { // 主题设置
            SelectObject(hdc, hBoldFont);
            TextOut(hdc, centerX - 80, centerY - 180, "系统主题设置", 10);
            SelectObject(hdc, hNormalFont);
            
            TextOut(hdc, centerX - 200, centerY - 120, "选择主题:", 8);
            
            // 绘制主题选择器
            DrawThemeSelector(hdc, centerX - 200, centerY - 80, 400, 120);
            
            TextOut(hdc, centerX - 200, centerY + 60, "当前主题:", 8);
            TextOut(hdc, centerX - 120, centerY + 60, themes[currentThemeIndex].themeName, strlen(themes[currentThemeIndex].themeName));
            
            SelectObject(hdc, hSmallFont);
            TextOut(hdc, centerX - 200, centerY + 90, "主题更改将在下次启动时生效", 24);
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

// 处理设置软件输入
void HandleSettingsInput(char key) {
    switch (settingsCurrentStep) {
        case 0: // 用户设置
            if (key == 8) { // Backspace
                if (strlen(newUsername) > 0) {
                    newUsername[strlen(newUsername) - 1] = '\0';
                }
            } else if (key >= 32 && key <= 126 && strlen(newUsername) < 49) {
                int len = strlen(newUsername);
                newUsername[len] = key;
                newUsername[len + 1] = '\0';
            }
            break;
            
        case 1: // 系统设置
            if (key == 8) { // Backspace
                if (strlen(newComputerName) > 0) {
                    newComputerName[strlen(newComputerName) - 1] = '\0';
                }
            } else if (key >= 32 && key <= 126 && strlen(newComputerName) < 49) {
                int len = strlen(newComputerName);
                newComputerName[len] = key;
                newComputerName[len + 1] = '\0';
            }
            break;
    }
    
    settingsError = FALSE;
}

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
        sprintf(commandOutput, "Error: Cannot access directory %s", currentPath);
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
        sprintf(commandOutput, "Error: Cannot create file %s", filename);
    } else {
        CloseHandle(hFile);
        sprintf(commandOutput, "File created: %s", filename);
        RefreshFileList();
    }
}

// 删除文件
void DeleteFileReal(const char* filename) {
    char fullPath[MAX_PATH];
    sprintf(fullPath, "%s\\%s", currentPath, filename);
    
    if (DeleteFile(fullPath)) {
        sprintf(commandOutput, "File deleted: %s", filename);
        RefreshFileList();
    } else {
        sprintf(commandOutput, "Error: Cannot delete file %s", filename);
    }
}

// 创建目录
void CreateDirectoryReal(const char* dirname) {
    char fullPath[MAX_PATH];
    sprintf(fullPath, "%s\\%s", currentPath, dirname);
    
    if (CreateDirectory(fullPath, NULL)) {
        sprintf(commandOutput, "Directory created: %s", dirname);
        RefreshFileList();
    } else {
        sprintf(commandOutput, "Error: Cannot create directory %s", dirname);
    }
}

// 复制文件
void CopyFileReal(const char* source, const char* target) {
    char sourcePath[MAX_PATH];
    char targetPath[MAX_PATH];
    sprintf(sourcePath, "%s\\%s", currentPath, source);
    sprintf(targetPath, "%s\\%s", currentPath, target);
    
    if (CopyFile(sourcePath, targetPath, FALSE)) {
        sprintf(commandOutput, "File copied: %s -> %s", source, target);
        RefreshFileList();
    } else {
        sprintf(commandOutput, "Error: Cannot copy file %s to %s", source, target);
    }
}

// 重命名文件
void RenameFileReal(const char* oldname, const char* newname) {
    char oldPath[MAX_PATH];
    char newPath[MAX_PATH];
    sprintf(oldPath, "%s\\%s", currentPath, oldname);
    sprintf(newPath, "%s\\%s", currentPath, newname);
    
    if (MoveFile(oldPath, newPath)) {
        sprintf(commandOutput, "File renamed: %s -> %s", oldname, newname);
        RefreshFileList();
    } else {
        sprintf(commandOutput, "Error: Cannot rename file %s to %s", oldname, newname);
    }
}

// 改进的文件管理器命令处理
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
    
    if (strcmp(cmd, "lc") == 0 || strcmp(cmd, "list") == 0 || strcmp(cmd, "dir") == 0 || strcmp(cmd, "ls") == 0) {
        ListFiles();
    }
    else if (strcmp(cmd, "go") == 0 || strcmp(cmd, "cd") == 0) {
        if (strlen(param) > 0) {
            ChangeDirectory(param);
        } else {
            strcpy(commandOutput, "Usage: go <path> or cd <path>\nExample: go C:\\Windows");
        }
    }
    else if (strcmp(cmd, "mkdir") == 0 || strcmp(cmd, "md") == 0) {
        if (strlen(param) > 0) {
            CreateDirectoryReal(param);
        } else {
            strcpy(commandOutput, "Usage: mkdir <directory_name>");
        }
    }
    else if (strcmp(cmd, "type") == 0 || strcmp(cmd, "cat") == 0) {
        if (strlen(param) > 0) {
            OpenFile(param);
        } else {
            strcpy(commandOutput, "Usage: type <filename>");
        }
    }
    else if (strcmp(cmd, "del") == 0 || strcmp(cmd, "rm") == 0) {
        if (strlen(param) > 0) {
            DeleteFileReal(param);
        } else {
            strcpy(commandOutput, "Usage: del <filename>");
        }
    }
    else if (strcmp(cmd, "copy") == 0 || strcmp(cmd, "cp") == 0) {
        char source[100], target[100];
        if (sscanf(param, "%s %s", source, target) == 2) {
            CopyFileReal(source, target);
        } else {
            strcpy(commandOutput, "Usage: copy <source> <destination>");
        }
    }
    else if (strcmp(cmd, "rename") == 0 || strcmp(cmd, "ren") == 0) {
        char oldname[100], newname[100];
        if (sscanf(param, "%s %s", oldname, newname) == 2) {
            RenameFileReal(oldname, newname);
        } else {
            strcpy(commandOutput, "Usage: rename <oldname> <newname>");
        }
    }
    else if (strcmp(cmd, "create") == 0 || strcmp(cmd, "touch") == 0) {
        if (strlen(param) > 0) {
            CreateFileReal(param);
        } else {
            strcpy(commandOutput, "Usage: create <filename>");
        }
    }
    else if (strcmp(cmd, "edit") == 0) {
        if (strlen(param) > 0) {
            // 打开文件编辑器
            ShowFileEditor(NULL, param);
        } else {
            strcpy(commandOutput, "Usage: edit <filename>");
        }
    }
    else if (strcmp(cmd, "hidden") == 0) {
        showHiddenFiles = !showHiddenFiles;
        RefreshFileList();
        sprintf(commandOutput, "Hidden files are now %s", showHiddenFiles ? "visible" : "hidden");
    }
    else if (strcmp(cmd, "pwd") == 0) {
        sprintf(commandOutput, "Current directory: %s", currentPath);
    }
    else if (strcmp(cmd, "help") == 0 || strcmp(cmd, "?") == 0) {
        ShowHelp();
    }
    else if (strcmp(cmd, "cls") == 0 || strcmp(cmd, "clear") == 0) {
        strcpy(commandOutput, "Screen cleared");
    }
    else if (strcmp(cmd, "exit") == 0 || strcmp(cmd, "quit") == 0) {
        showFileManager = FALSE;
        strcpy(commandOutput, "File manager closed");
    }
    else if (strcmp(cmd, "ver") == 0 || strcmp(cmd, "version") == 0) {
        strcpy(commandOutput, "fOS File Manager v1.0 Professional\nBuild 200 (Professional Edition)");
    }
    else if (strcmp(cmd, "date") == 0) {
        char dateStr[50], timeStr[50];
        GetCurrentDateTime(dateStr, timeStr);
        sprintf(commandOutput, "Current date: %s\nCurrent time: %s", dateStr, timeStr);
    }
    else if (strlen(cmd) > 0) {
        // 检查是否是打开文件命令
        if (strstr(cmd, ".txt") || strstr(cmd, ".c") || strstr(cmd, ".cpp") || 
            strstr(cmd, ".h") || strstr(cmd, ".exe") || strstr(cmd, ".bat")) {
            OpenFile(cmd);
        } else {
            sprintf(commandOutput, "Unknown command: %s\nType 'help' for available commands", cmd);
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
    sprintf(commandOutput, "Directory of %s\n\n", currentPath);
    
    char temp[500];
    int dirCount = 0, fileCountTotal = 0;
    long long totalSize = 0;
    
    // 添加列标题
    strcat(commandOutput, "Type     Size       Date Modified    Name\n");
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
        strcpy(commandOutput, "Usage: go <path> or cd <path>\nExample: go C:\\Windows");
        return;
    }
    
    char newPath[MAX_PATH];
    
    // 处理特殊路径
    if (strcmp(path, "..") == 0 || strcmp(path, "../") == 0) {
        // 返回到上级目录
        int len = strlen(currentPath);
        if (len > 3) { // 不是根目录
            for (int i = len - 2; i >= 0; i--) {
                if (currentPath[i] == '\\') {
                    currentPath[i + 1] = '\0';
                    break;
                }
            }
        }
    }
    else if (strcmp(path, "/") == 0 || strcmp(path, "\\") == 0) {
        // 回到根目录
        GetCurrentDirectory(MAX_PATH, currentPath);
        char drive[10];
        strncpy(drive, currentPath, 3);
        drive[3] = '\0';
        strcpy(currentPath, drive);
    }
    else if (path[1] == ':') {
        // 绝对路径
        strcpy(currentPath, path);
    }
    else {
        // 相对路径
        if (currentPath[strlen(currentPath)-1] != '\\') {
            strcat(currentPath, "\\");
        }
        strcat(currentPath, path);
    }
    
    // 确保路径以反斜杠结尾
    if (currentPath[strlen(currentPath)-1] != '\\') {
        strcat(currentPath, "\\");
    }
    
    // 验证路径是否存在
    DWORD attr = GetFileAttributes(currentPath);
    if (attr == INVALID_FILE_ATTRIBUTES || !(attr & FILE_ATTRIBUTE_DIRECTORY)) {
        sprintf(commandOutput, "Error: Directory not found: %s", currentPath);
        // 恢复原路径
        GetCurrentDirectory(MAX_PATH, currentPath);
    } else {
        RefreshFileList();
        sprintf(commandOutput, "Changed to directory: %s", currentPath);
    }
}

// 改进的打开文件函数
void OpenFile(const char* filename) {
    char fullPath[MAX_PATH];
    sprintf(fullPath, "%s\\%s", currentPath, filename);
    
    DWORD attr = GetFileAttributes(fullPath);
    if (attr == INVALID_FILE_ATTRIBUTES) {
        sprintf(commandOutput, "Error: File not found: %s", filename);
        return;
    }
    
    if (attr & FILE_ATTRIBUTE_DIRECTORY) {
        sprintf(commandOutput, "Error: %s is a directory, use 'go %s'", filename, filename);
        return;
    }
    
    // 检查文件扩展名，如果是文本文件，则打开编辑器
    const char* ext = strrchr(filename, '.');
    if (ext && (stricmp(ext, ".txt") == 0 || stricmp(ext, ".c") == 0 || 
                stricmp(ext, ".cpp") == 0 || stricmp(ext, ".h") == 0 ||
                stricmp(ext, ".md") == 0 || stricmp(ext, ".ini") == 0 ||
                stricmp(ext, ".conf") == 0)) {
        ShowFileEditor(NULL, filename);
        return;
    }
    
    sprintf(commandOutput, "Opening file: %s\n\n", filename);
    
    if (ext) {
        if (stricmp(ext, ".txt") == 0 || stricmp(ext, ".c") == 0 || 
            stricmp(ext, ".cpp") == 0 || stricmp(ext, ".h") == 0) {
            // 读取文本文件
            HANDLE hFile = CreateFile(fullPath, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile != INVALID_HANDLE_VALUE) {
                DWORD fileSize = GetFileSize(hFile, NULL);
                if (fileSize > 0 && fileSize < 10000) { // 限制文件大小
                    char* buffer = (char*)malloc(fileSize + 1);
                    DWORD bytesRead;
                    if (ReadFile(hFile, buffer, fileSize, &bytesRead, NULL)) {
                        buffer[bytesRead] = '\0';
                        strcat(commandOutput, buffer);
                    } else {
                        strcat(commandOutput, "Error reading file content");
                    }
                    free(buffer);
                } else {
                    strcat(commandOutput, "File is too large to display or empty");
                }
                CloseHandle(hFile);
            } else {
                strcat(commandOutput, "Error opening file for reading");
            }
        }
        else if (stricmp(ext, ".exe") == 0) {
            strcat(commandOutput, "Executable file. Use ShellExecute to run it.\n");
            strcat(commandOutput, "In real implementation, this would launch the program.");
        }
        else {
            strcat(commandOutput, "Binary file. Content cannot be displayed in text mode.");
        }
    } else {
        strcat(commandOutput, "File has no extension. Attempting to read as text...\n\n");
        
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
                        strcat(commandOutput, "File appears to be binary. Content not displayed.");
                    }
                } else {
                    strcat(commandOutput, "Error reading file content");
                }
                free(buffer);
            } else {
                strcat(commandOutput, "File is too large to display or empty");
            }
            CloseHandle(hFile);
        } else {
            strcat(commandOutput, "Error opening file for reading");
        }
    }
}

// 改进的帮助函数
void ShowHelp() {
    strcpy(commandOutput, "fOS File Manager - Command Help\n\n");
    strcat(commandOutput, "File Operations:\n");
    strcat(commandOutput, "  ls, dir, list    - List directory contents\n");
    strcat(commandOutput, "  cd, go <path>    - Change directory\n");
    strcat(commandOutput, "  type <file>      - Display file content\n");
    strcat(commandOutput, "  edit <file>      - Edit text file\n");
    strcat(commandOutput, "  del, rm <file>   - Delete file\n");
    strcat(commandOutput, "  copy <src> <dst> - Copy file\n");
    strcat(commandOutput, "  rename <old> <new> - Rename file\n");
    strcat(commandOutput, "  mkdir <name>     - Create directory\n");
    strcat(commandOutput, "  create <file>    - Create empty file\n\n");
    
    strcat(commandOutput, "System Commands:\n");
    strcat(commandOutput, "  pwd             - Show current directory\n");
    strcat(commandOutput, "  hidden          - Toggle hidden files\n");
    strcat(commandOutput, "  cls, clear      - Clear screen\n");
    strcat(commandOutput, "  ver, version    - Show version\n");
    strcat(commandOutput, "  date            - Show date and time\n");
    strcat(commandOutput, "  help, ?         - Show this help\n");
    strcat(commandOutput, "  exit, quit      - Exit file manager\n\n");
    
    strcat(commandOutput, "Examples:\n");
    strcat(commandOutput, "  ls                    List files\n");
    strcat(commandOutput, "  go C:\\Windows         Change to Windows directory\n");
    strcat(commandOutput, "  type readme.txt       View readme file\n");
    strcat(commandOutput, "  edit notes.txt        Edit text file\n");
    strcat(commandOutput, "  mkdir NewFolder       Create new directory\n");
    strcat(commandOutput, "  create test.txt       Create empty file\n");
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
    }
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
    TextOut(hdc, editorRect.left + 10, editorRect.bottom - 20, statusInfo, strlen(statusInfo));
    
    TextOut(hdc, editorRect.right - 200, editorRect.bottom - 20, "按Ctrl+S保存, Esc退出", 22);
    
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
        sprintf(osVersion, "fOS %d.%d Professional (Build 200) - 已激活", MAJOR_VERSION, MINOR_VERSION);
    } else if (isTrialMode) {
        sprintf(osVersion, "fOS %d.%d Professional (Build 200) - 试用版 (%d天)", MAJOR_VERSION, MINOR_VERSION, trialDaysRemaining);
    } else {
        sprintf(osVersion, "fOS %d.%d Professional (Build 200) - 未激活", MAJOR_VERSION, MINOR_VERSION);
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
    TextOut(hdc, centerX - 350, centerY + 220, "提示: 有效CD-KEY示例: FOS62-20250-JELLY-FISH6-2CLASS", 46);
    
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

// 验证OOBE输入
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

// 绘制OOBE步骤界面
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
            TextOut(hdc, centerX - 300, centerY - 180, "感谢您选择 fOS 1.0 Professional", 30);
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
            
            if (oobeError) {
                SetTextColor(hdc, VISTA_RED);
                TextOut(hdc, centerX - 180, centerY + 10, oobeErrorMsg, strlen(oobeErrorMsg));
                SetTextColor(hdc, RGB(255, 255, 255));
            }
            
            SelectObject(hdc, hSmallFont);
            TextOut(hdc, centerX - 180, centerY + 50, "请输入密码，然后按Tab键继续", 28);
            break;
        }
            
        case 3: {
            TextOut(hdc, centerX - 150, centerY - 250, "计算机名称", 10);
            SelectObject(hdc, hNormalFont);
            TextOut(hdc, centerX - 250, centerY - 200, "为您的计算机设置一个名称", 24);
            
            DrawInputBox(hdc, centerX - 200, centerY - 120, 400, 35, "计算机名:", computerName, TRUE, FALSE, "输入计算机名称");
            
            if (oobeError) {
                SetTextColor(hdc, VISTA_RED);
                TextOut(hdc, centerX - 180, centerY - 70, oobeErrorMsg, strlen(oobeErrorMsg));
                SetTextColor(hdc, RGB(255, 255, 255));
            }
            
            SelectObject(hdc, hSmallFont);
            TextOut(hdc, centerX - 180, centerY + 30, "请输入计算机名，然后按Enter键完成配置", 36);
            
            // 绘制完成按钮
            RECT completeButtonRect = {centerX - 80, centerY + 80, centerX + 80, centerY + 120};
            HBRUSH hCompleteBrush = CreateSolidBrush(VISTA_GREEN);
            FillRect(hdc, &completeButtonRect, hCompleteBrush);
            DeleteObject(hCompleteBrush);
    
            HPEN hCompletePen = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
            HPEN hOldPen = (HPEN)SelectObject(hdc, hCompletePen);
            SelectObject(hdc, GetStockObject(NULL_BRUSH));
            Rectangle(hdc, centerX - 80, centerY + 80, centerX + 80, centerY + 120);
            SelectObject(hdc, hOldPen);
            DeleteObject(hCompletePen);
    
            SetTextColor(hdc, RGB(255, 255, 255));
            SelectObject(hdc, hNormalFont);
            TextOut(hdc, centerX - 30, centerY + 95, "完成配置", 8);
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
        
        if (strcmp(normalizedGuess, "jammyjellyfish") == 0) {
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
    return (strcmp(loginUsername, username) == 0 && strcmp(loginPassword, password) == 0);
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

// 处理OOBE输入
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
                if (strlen(confirmPassword) > 0) {
                    confirmPassword[strlen(confirmPassword) - 1] = '\0';
                }
            } else if (key == 9) {
                currentStep = 3;
            } else if (key == 13) {
                if (ValidateOOBEInput()) {
                    CompleteOOBE(NULL);
                }
            } else if (key >= 32 && key <= 126 && strlen(confirmPassword) < 49) {
                int len = strlen(confirmPassword);
                confirmPassword[len] = key;
                confirmPassword[len + 1] = '\0';
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
    TextOut(hdc, centerX - 100, centerY - 40, "Professional Edition", 19);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hVersionFont);
    
    HFONT hInfoFont = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, FIXED_PITCH, "Terminal");
    hOldFont = (HFONT)SelectObject(hdc, hInfoFont);
    
    TextOut(hdc, centerX - 180, centerY + 10, "Initializing system components...", 32);
    TextOut(hdc, centerX - 180, centerY + 40, "Loading kernel modules...", 25);
    TextOut(hdc, centerX - 180, centerY + 70, "Starting services...", 20);
    
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
    sprintf(progressText, "Starting FOS... %d%%", fosProgressValue);
    TextOut(hdc, centerX - 80, centerY + 150, progressText, strlen(progressText));
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hProgressFont);
    
    HFONT hCopyrightFont = CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, FIXED_PITCH, "Terminal");
    hOldFont = (HFONT)SelectObject(hdc, hCopyrightFont);
    
    TextOut(hdc, centerX - 150, rect.bottom - 50, "(C) 2025 FOS Development Team. All rights reserved.", 52);
    TextOut(hdc, centerX - 100, rect.bottom - 30, "Professional Edition", 19);
    
    SelectObject(hdc, hOldFont);
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
    TextOut(hdc, centerX - 80, centerY - 150, "用户登录", 8);
    
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
    sprintf(welcomeMsg, "您好，%s", username);
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

// 绘制任务栏（在上方）
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
    sprintf(taskbarText, "f系统%d.%d | 用户: %s | 计算机: %s", MAJOR_VERSION, MINOR_VERSION, username, computerName);
    TextOut(hdc, 10, 15, taskbarText, strlen(taskbarText));
    
    // 显示激活状态
    char activationText[50];
    if (isActivated) {
        sprintf(activationText, "专业版已激活");
        SetTextColor(hdc, VISTA_GREEN);
    } else if (isTrialMode) {
        sprintf(activationText, "试用版 (%d天)", trialDaysRemaining);
        SetTextColor(hdc, VISTA_ORANGE);
    } else {
        sprintf(activationText, "未激活");
        SetTextColor(hdc, VISTA_RED);
    }
    TextOut(hdc, 400, 15, activationText, strlen(activationText));
    SetTextColor(hdc, RGB(255, 255, 255));
    
    // 显示时间
    char dateStr[50], timeStr[50];
    GetCurrentDateTime(dateStr, timeStr);
    
    char datetimeStr[100];
    sprintf(datetimeStr, "%s %s", dateStr, timeStr);
    
    int textX = rect.right - GetTextWidth(hdc, datetimeStr) - 10;
    
    TextOut(hdc, textX, 15, datetimeStr, strlen(datetimeStr));
    
    // 绘制任务栏按钮
    int buttonWidth = 100;
    int buttonHeight = 40;
    int buttonY = 10;
    
    // 显示桌面按钮
    DrawTaskbarButton(hdc, 200, buttonY, buttonWidth, buttonHeight, "显示桌面", 
                     isDesktopHovered, isDesktopPressed, FALSE);
    
    // 文件管理器按钮
    DrawTaskbarButton(hdc, 310, buttonY, buttonWidth, buttonHeight, "文件管理", 
                     isFileManagerHovered, isFileManagerPressed, showFileManager);
    
    // 计算器按钮
    DrawTaskbarButton(hdc, 420, buttonY, buttonWidth, buttonHeight, "计 算 器", 
                     isCalcHovered, isCalcPressed, showCalculator);
    
    // 系统信息按钮
    DrawTaskbarButton(hdc, 530, buttonY, buttonWidth, buttonHeight, "系统信息", 
                     isSysInfoHovered, isSysInfoPressed, showSystemInfo);
    
    // 设置按钮（专业版功能）
    if (isActivated) {
        DrawTaskbarButton(hdc, 640, buttonY, buttonWidth, buttonHeight, "系统设置", 
                         isSettingsHovered, isSettingsPressed, showSettings);
    }
    
    // 水母游戏按钮（猜对作者名字后显示）
    if (showGameButton) {
        int gameX = isActivated ? 750 : 640;
        DrawTaskbarButton(hdc, gameX, buttonY, buttonWidth, buttonHeight, "水母游戏", 
                         isGameHovered, isGamePressed, showGameEgg);
    }
    
    // 关机按钮
    int shutdownX = showGameButton ? (isActivated ? 860 : 750) : (isActivated ? 750 : 640);
    DrawTaskbarButton(hdc, shutdownX, buttonY, buttonWidth, buttonHeight, "关 机", 
                     isShutdownHovered, isShutdownPressed, FALSE);
    
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
    TextOut(hdc, centerX - 100, textY, "JammyJellyfish", 14);
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
    TextOut(hdc, centerX - 180, textY, "返回桌面点击按钮开始游戏吧！", 26);
    textY += 40;
    
    // 绘制关闭按钮
    RECT closeButtonRect = {centerX - 60, centerY + 180, centerX + 60, centerY + 220};
    HBRUSH hCloseBrush = CreateSolidBrush(RGB(100, 200, 100));
    FillRect(hdc, &closeButtonRect, hCloseBrush);
    DeleteObject(hCloseBrush);
    
    HPEN hClosePen = CreatePen(PS_SOLID, 1, RGB(50, 150, 50));
    hOldPen = (HPEN)SelectObject(hdc, hClosePen);
    SelectObject(hdc, GetStockObject(NULL_BRUSH));
    Rectangle(hdc, centerX - 60, centerY + 180, centerX + 60, centerY + 220);
    SelectObject(hdc, hOldPen);
    DeleteObject(hClosePen);
    
    SetTextColor(hdc, RGB(255, 255, 255));
    HFONT hButtonFont = CreateFont(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    SelectObject(hdc, hButtonFont);
    
    TextOut(hdc, centerX - 20, centerY + 195, "关闭", 4);
    
    SelectObject(hdc, hOldFont);
    DeleteObject(hTitleFont);
    DeleteObject(hContentFont);
    DeleteObject(hNameFont);
    DeleteObject(hButtonFont);
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
            specialMessageScrollY--;
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
            "江俊序", "程骏楠", "董正宇", "陈宇彬", "杨锦岳", "黄梓涵", "郭光鑫", "方弘毅",
            "刘美铄", "陈锦意", "金秋宜", "宋安琪", "章睦瑶", "李梦琪", "刘菁涵", "贾良睿",
            "胡乐艳", "陈琦钰", "陈  夕", "郝梓萱", "徐子淇", "柏钰涵", "张梓桐", "王诗诗",
            "姜涵月", "程昭檬", "杨晟熠", "袁翊宸", "刘雨棠", "刘思远", "马依一", "柏  典",
            "饶铭宇"
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
        TextOut(hdc, centerX - 180, textY, "提示：ubuntu", 12);
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
        textY += 30;
        TextOut(hdc, centerX - 180, textY, "内容自动滚动中...", 17);
        
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
    
    TextOut(hdc, centerX - 250, textY, "? 2025 fOS Development Team", 26);
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
    
    TextOut(hdc, centerX - 60, fileRect.top + 10, "File Manager", 11);
    
    HFONT hContentFont = CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        DEFAULT_QUALITY, DEFAULT_PITCH, "Arial");
    SelectObject(hdc, hContentFont);
    
    // 显示当前路径
    TextOut(hdc, fileRect.left + 20, fileRect.top + 50, "Path:", 5);
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
    TextOut(hdc, fileRect.left + 25, fileRect.top + 85, "C:\\>", 4);
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
    TextOut(hdc, fileRect.left + 20, fileRect.bottom - 40, "Type 'help' for available commands", 32);
    
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
    showCalculator = FALSE;
    showSystemInfo = FALSE;
    showFileManager = FALSE;
    showFileEditor = FALSE;
    showSettings = FALSE;
    showSpecialMessage = FALSE;
    showFinalEgg = FALSE;
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
    if (hwnd) {
        InvalidateRect(hwnd, NULL, TRUE);
        UpdateWindow(hwnd);
    }
}

// 显示关机动画
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
        strcpy(commandOutput, "fOS File Manager v1.0 Professional\nType 'help' for available commands\n");
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

// 启动试用模式
void StartTrialMode(HWND hwnd) {
    isTrialMode = TRUE;
    isActivated = FALSE;
    trialDaysRemaining = 30;
    ApplyTrialRestrictions();
    
    if (hwnd) {
        ShowOOBE(hwnd);
    }
}

// 窗口过程函数
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
    switch(Message) {
        case WM_CREATE: {
            LoadConfig();
            InitializeSystemInfo();
            SetTimer(hwnd, 8, 40, NULL);
            SetTimer(hwnd, 7, 1000, NULL);
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
                if (!showOOBE && !showComplete && !showDesktop && !showBootAnimation && !showLogin && !showCDKeyScreen && !showGameEgg && !showFileEditor && !showSettings) {
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
            break;
        }
        
        case WM_CHAR: {
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
            
            // 文件编辑器特殊按键处理
            if (showFileEditor) {
                HandleFileEditorSpecialKeys(wParam);
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
            }
            break;
        }
        
        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            
            if (showGameEgg) {
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
                int buttonWidth = 100;
                int buttonHeight = 40;
                int buttonY = 10;
                
                // 显示桌面按钮
                RECT desktopRect = {200, buttonY, 200 + buttonWidth, buttonY + buttonHeight};
                if (x >= desktopRect.left && x <= desktopRect.right &&
                    y >= desktopRect.top && y <= desktopRect.bottom) {
                    isDesktopPressed = TRUE;
                    InvalidateRect(hwnd, &desktopRect, TRUE);
                    return 0;
                }
                
                // 文件管理器按钮
                RECT fileRect = {310, buttonY, 310 + buttonWidth, buttonY + buttonHeight};
                if (x >= fileRect.left && x <= fileRect.right &&
                    y >= fileRect.top && y <= fileRect.bottom) {
                    isFileManagerPressed = TRUE;
                    InvalidateRect(hwnd, &fileRect, TRUE);
                    return 0;
                }
                
                // 计算器按钮
                RECT calcRect = {420, buttonY, 420 + buttonWidth, buttonY + buttonHeight};
                if (x >= calcRect.left && x <= calcRect.right &&
                    y >= calcRect.top && y <= calcRect.bottom) {
                    isCalcPressed = TRUE;
                    InvalidateRect(hwnd, &calcRect, TRUE);
                    return 0;
                }
                
                // 系统信息按钮
                RECT sysinfoRect = {530, buttonY, 530 + buttonWidth, buttonY + buttonHeight};
                if (x >= sysinfoRect.left && x <= sysinfoRect.right &&
                    y >= sysinfoRect.top && y <= sysinfoRect.bottom) {
                    isSysInfoPressed = TRUE;
                    InvalidateRect(hwnd, &sysinfoRect, TRUE);
                    return 0;
                }
                
                // 设置按钮（专业版）
                if (isActivated) {
                    RECT settingsRect = {640, buttonY, 640 + buttonWidth, buttonY + buttonHeight};
                    if (x >= settingsRect.left && x <= settingsRect.right &&
                        y >= settingsRect.top && y <= settingsRect.bottom) {
                        isSettingsPressed = TRUE;
                        InvalidateRect(hwnd, &settingsRect, TRUE);
                        return 0;
                    }
                }
                
                // 水母游戏按钮
                if (showGameButton) {
                    int gameX = isActivated ? 750 : 640;
                    RECT gameRect = {gameX, buttonY, gameX + buttonWidth, buttonY + buttonHeight};
                    if (x >= gameRect.left && x <= gameRect.right &&
                        y >= gameRect.top && y <= gameRect.bottom) {
                        isGamePressed = TRUE;
                        InvalidateRect(hwnd, &gameRect, TRUE);
                        return 0;
                    }
                }
                
                // 关机按钮
                int shutdownX = showGameButton ? (isActivated ? 860 : 750) : (isActivated ? 750 : 640);
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
            else if (showOOBE && currentStep == 0) {
                // 检查是否点击了开始设置按钮
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                int centerX = clientRect.right / 2;
                int centerY = clientRect.bottom / 2;
                
                RECT buttonRect = {centerX - 80, centerY + 80, centerX + 80, centerY + 120};
                if (x >= buttonRect.left && x <= buttonRect.right &&
                    y >= buttonRect.top && y <= buttonRect.bottom) {
                    currentStep = 1;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            else if (showOOBE && currentStep == 3) {
                // 检查是否点击了完成配置按钮
                RECT clientRect;
                GetClientRect(hwnd, &clientRect);
                int centerX = clientRect.right / 2;
                int centerY = clientRect.bottom / 2;
                
                RECT buttonRect = {centerX - 80, centerY + 80, centerX + 80, centerY + 120};
                if (x >= buttonRect.left && x <= buttonRect.right &&
                    y >= buttonRect.top && y <= buttonRect.bottom) {
                    if (ValidateOOBEInput()) {
                        CompleteOOBE(hwnd);
                    }
                }
            }
            break;
        }
        
        case WM_LBUTTONUP: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
            
            if (showDesktop) {
                int buttonWidth = 100;
                int buttonHeight = 40;
                int buttonY = 10;
                
                // 显示桌面按钮
                RECT desktopRect = {200, buttonY, 200 + buttonWidth, buttonY + buttonHeight};
                if (isDesktopPressed) {
                    isDesktopPressed = FALSE;
                    InvalidateRect(hwnd, &desktopRect, TRUE);
                    
                    if (x >= desktopRect.left && x <= desktopRect.right &&
                        y >= desktopRect.top && y <= desktopRect.bottom) {
                        CloseAllApps(hwnd);
                    }
                }
                
                // 文件管理器按钮
                RECT fileRect = {310, buttonY, 310 + buttonWidth, buttonY + buttonHeight};
                if (isFileManagerPressed) {
                    isFileManagerPressed = FALSE;
                    InvalidateRect(hwnd, &fileRect, TRUE);
                    
                    if (x >= fileRect.left && x <= fileRect.right &&
                        y >= fileRect.top && y <= fileRect.bottom) {
                        ShowFileManager(hwnd);
                    }
                }
                
                // 计算器按钮
                RECT calcRect = {420, buttonY, 420 + buttonWidth, buttonY + buttonHeight};
                if (isCalcPressed) {
                    isCalcPressed = FALSE;
                    InvalidateRect(hwnd, &calcRect, TRUE);
                    
                    if (x >= calcRect.left && x <= calcRect.right &&
                        y >= calcRect.top && y <= calcRect.bottom) {
                        ShowCalculator(hwnd);
                    }
                }
                
                // 系统信息按钮
                RECT sysinfoRect = {530, buttonY, 530 + buttonWidth, buttonY + buttonHeight};
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
                    RECT settingsRect = {640, buttonY, 640 + buttonWidth, buttonY + buttonHeight};
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
                    int gameX = isActivated ? 750 : 640;
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
                
                // 关机按钮
                int shutdownX = showGameButton ? (isActivated ? 860 : 750) : (isActivated ? 750 : 640);
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
                int buttonWidth = 100;
                int buttonHeight = 40;
                int buttonY = 10;
                
                RECT desktopRect = {200, buttonY, 200 + buttonWidth, buttonY + buttonHeight};
                BOOL wasDesktopHovered = isDesktopHovered;
                isDesktopHovered = (x >= desktopRect.left && x <= desktopRect.right &&
                                  y >= desktopRect.top && y <= desktopRect.bottom);
                
                RECT fileRect = {310, buttonY, 310 + buttonWidth, buttonY + buttonHeight};
                BOOL wasFileManagerHovered = isFileManagerHovered;
                isFileManagerHovered = (x >= fileRect.left && x <= fileRect.right &&
                                      y >= fileRect.top && y <= fileRect.bottom);
                
                RECT calcRect = {420, buttonY, 420 + buttonWidth, buttonY + buttonHeight};
                BOOL wasCalcHovered = isCalcHovered;
                isCalcHovered = (x >= calcRect.left && x <= calcRect.right &&
                               y >= calcRect.top && y <= calcRect.bottom);
                
                RECT sysinfoRect = {530, buttonY, 530 + buttonWidth, buttonY + buttonHeight};
                BOOL wasSysInfoHovered = isSysInfoHovered;
                isSysInfoHovered = (x >= sysinfoRect.left && x <= sysinfoRect.right &&
                                  y >= sysinfoRect.top && y <= sysinfoRect.bottom);
                
                // 设置按钮悬停（专业版）
                BOOL wasSettingsHovered = isSettingsHovered;
                if (isActivated) {
                    RECT settingsRect = {640, buttonY, 640 + buttonWidth, buttonY + buttonHeight};
                    isSettingsHovered = (x >= settingsRect.left && x <= settingsRect.right &&
                                       y >= settingsRect.top && y <= settingsRect.bottom);
                } else {
                    isSettingsHovered = FALSE;
                }
                
                // 水母游戏按钮悬停
                BOOL wasGameHovered = isGameHovered;
                if (showGameButton) {
                    int gameX = isActivated ? 750 : 640;
                    RECT gameRect = {gameX, buttonY, gameX + buttonWidth, buttonY + buttonHeight};
                    isGameHovered = (x >= gameRect.left && x <= gameRect.right &&
                                   y >= gameRect.top && y <= gameRect.bottom);
                } else {
                    isGameHovered = FALSE;
                }
                
                // 关机按钮悬停
                int shutdownX = showGameButton ? (isActivated ? 860 : 750) : (isActivated ? 750 : 640);
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
                    wasShutdownHovered != isShutdownHovered) {
                    RECT taskbarRect = {0, 0, GetSystemMetrics(SM_CXSCREEN), 60};
                    InvalidateRect(hwnd, &taskbarRect, TRUE);
                }
                
                if (isDesktopHovered || isFileManagerHovered || isCalcHovered || 
                    isSysInfoHovered || isSettingsHovered || isGameHovered || isShutdownHovered) {
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
            
            // 清理资源
            if (hBackgroundBitmap) {
                DeleteObject(hBackgroundBitmap);
            }
            
            // 保存配置
            SaveConfig();
            
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
            
            // 水母游戏优先绘制
            if (showGameEgg) {
                DrawJellyfishGame(hdc, hwnd);
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
                    TextOut(hdc, centerX - 180, centerY - 100, "f系统1.0 专业版", 14);
                    SelectObject(hdc, hNormalFont);
                    TextOut(hdc, centerX - 180, centerY - 30, "欢迎使用 f系统1.0 专业版", 24);
                    TextOut(hdc, centerX - 180, centerY + 10, "在使用系统前，请确保您的所有软件已经关闭", 40);
                    TextOut(hdc, centerX - 180, centerY + 50, "5秒钟后进入...", 14);
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
                    TextOut(hdc, centerX - 180, centerY - 120, "海内存知己，天涯若比邻", 22);
                    SelectObject(hdc, hNormalFont);
                    TextOut(hdc, centerX - 180, centerY - 50, "请稍后...", 9);
                    TextOut(hdc, centerX - 180, centerY, "系统正在为第一次启动做准备！", 28);
                    TextOut(hdc, centerX - 180, centerY + 50, "用户验证: 修补程序工作", 22);
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
    wc.lpszClassName = "f系统1.0-beta";
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
        "f系统1.0-beta",
        "f系统1.0 专业版 - fOS 1.0 Professional Edition",
        WS_POPUP | WS_VISIBLE,
        0, 0,
        screenWidth, screenHeight,
        NULL, NULL, hInstance, NULL
    );

    if(hwnd == NULL) {
        MessageBox(NULL, "窗口创建失败！","错误!",MB_ICONEXCLAMATION|MB_OK);
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
