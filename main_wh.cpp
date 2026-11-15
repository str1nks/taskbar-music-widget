// ==WindhawkMod==
// @id              taskbar-music-widget
// @name            Taskbar Music Widget
// @name:ru-RU      Музыкальный виджет на панели задач
// @description     Now-playing widget for the taskbar with an expandable island and a built-in Yandex Music player (search, playlists, My Wave): seek, volume, synced lyrics and colors from the album art
// @description:ru-RU Виджет «сейчас играет» на панели задач с раскрывающимся островом и встроенным плеером Яндекс Музыки (поиск, плейлисты, «Моя волна»): перемотка, громкость, текст песни и цвета из обложки
// @version         1.22.0
// @author          str1nks
// @github          https://github.com/str1nks/taskbar-music-widget
// @include         explorer.exe
// @compilerOptions -lgdi32 -lshell32 -lole32 -loleaut32 -lruntimeobject -lwinhttp -ladvapi32
// ==/WindhawkMod==

// ==WindhawkModReadme==
/*
# Taskbar Music Widget

A transparent "now playing" widget that lives on your taskbar. It shows the current track (cover, title, artist)
from any app that publishes media info to Windows (Yandex Music by default), and turns into a Dynamic Island-style
player on right click.

## Features

**Compact mode**
- Cover, title and artist right on the taskbar, no background.
- Hover to reveal previous / play-pause / next buttons.
- Click to open the app; click again to hide it to the tray.
- Hides automatically while another app is fullscreen.

**Expanded player (right click)**
- Large cover, seekable progress bar with times, transport buttons.
- Volume slider for the target app only (its own session in the Windows volume mixer), mute button, mouse-wheel volume with a configurable step.
- Dynamic palette: background gradient, text and accents are taken from the album art and fade smoothly between tracks.
- Adjustable background opacity.
- Lyrics panel (button next to the controls): synchronized lyrics with the current line highlighted and auto-scrolled, click a line to seek, mouse wheel to scroll. Lyrics come from [LRCLIB](https://lrclib.net); if LRCLIB has nothing, [Genius](https://genius.com) (needs an API token) is tried. The small "i" button in the top-right corner of the lyrics panel checks that the token really works and shows why a text was not found (the button can be turned off in the settings). At the very end of the lyrics a small footnote tells which source the text was taken from.
- Spring-like expand animation, staggered content reveal, cover cross-fade, hover and press effects.
- Collapses instantly, after a delay, or never when the cursor leaves.

## Built-in Yandex Music player

Set **Operating mode** to *Built-in player* and fill in **Yandex Music token**: the widget stops following the system media session and plays music itself (audio streams straight from Yandex, no app and no files on disk):

- Play / pause, previous / next, seeking, volume and mute, "like" heart next to the title (outline when the track is not liked, filled when it is).
- **Library** button (left of the transport buttons): search field (tracks and artists; an artist opens a profile with their tracks), **My Wave** button, your playlists (including "Liked"), click a track to play it, "Play all" inside a playlist.
- My Wave sends the same feedback to Yandex as the app does (started / finished / skipped).
- A badge above the title (and in the compact widget) tells where the track is playing from: "My Wave", "Wave by track", "Playlist: …", "Album: …", "Artist: …", "Search: …" or "Liked". The My Wave button in the library is highlighted while the wave plays.
- Right click a track (in the library or the current track in the player) for a menu: **Wave by track**, **Open album**, and the artist profiles. Right click elsewhere still expands/collapses the player.
- In this mode a left click on the compact widget expands it instead of opening the target app.
- With *Player app* mode (or without a token) the mod works as before: it shows and controls whatever plays in the system, and a left click opens/hides the app.

## Setup

1. Set **Path to the app exe** (used to open/hide the app and to find its audio session for the volume slider).
2. Set **Music source filter** to a part of the app's AppUserModelId (e.g. `yandex`, `spotify`). Leave empty to follow the current system media session.
3. Tweak alignment, offset, scale, width and colors to taste.

## Settings

| Setting | Description |
| --- | --- |
| Operating mode | *Player app*: the widget only shows/controls the desktop app's player, LMB opens/hides the app. *Built-in player*: the widget is the music source (needs the Yandex Music token), LMB expands it. Without a token the built-in mode behaves like *Player app* |
| Path to the app exe | App to open/hide on click; also used to match its audio session for volume control |
| Music source filter | Part of the media session's AppUserModelId (case-insensitive); empty = current system session |
| Alignment, horizontal offset | Where the widget sits on the taskbar |
| Player size (%) | Scales the whole widget (50-200) |
| Width, font size, text color | Compact mode appearance |
| Collapse after cursor leaves | Instantly / after a delay / never (right click always toggles) |
| Collapse delay | Seconds for the "after a delay" mode |
| Background opacity (%) | Opacity of the expanded player background (5-100) |
| Volume wheel step (%) | Volume change per wheel notch over the expanded player (1-50) |
| Lyrics sync offset (ms) | Shift lyrics earlier/later if they drift |
| Show the "i" button | Show or hide the source-check button in the corner of the lyrics panel (on by default) |
| Yandex Music token | OAuth token of your account. Required for the *Built-in player* mode (search, playlists, My Wave). Empty = follow the system media session |
| Genius API token | Optional. Enables Genius as a fallback lyrics source. Create an API client at https://genius.com/api-clients and paste its Client Access Token |

## Notes

- Works with a horizontal taskbar (bottom or top). A vertical taskbar is not supported.
- The volume slider needs the app to have an active audio session; a value set while paused is applied when sound starts.
- Seeking and the progress bar need the app to publish timeline info; without it the bar stays empty.
- Lyrics are downloaded from lrclib.net in the background. If the site is unreachable, the panel shows "Text not found". The system proxy settings are used.
- Not every track has lyrics in the LRCLIB database.
- Genius lyrics are plain text without timing, so line highlighting and click-to-seek are not available for them. Genius is queried only when LRCLIB finds nothing and a token is set.
- Source order: LRCLIB, then Genius.
- If genius.com answers "403" (its bot protection), the mod retries with a different request profile and, as the last resort, through the site's own search; if the lyrics page stays closed, it tries the embed script of the song.
- Open the lyrics panel and press the "i" button in its corner: the mod checks LRCLIB and Genius (token, lyrics pages) and prints the result in the panel.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
- playerMode: own
  $name: Режим работы
  $description: 'Как виджет получает музыку и что делает клик. «Плеер приложения»: виджет только показывает и управляет плеером приложения на ПК (см. Путь к exe и Фильтр источника), клик ЛКМ открывает/прячет это приложение. «Встроенный плеер»: виджет сам источник музыки (нужен Токен Яндекс Музыки), клик ЛКМ по виджету разворачивает его, приложение не открывается. Без токена «Встроенный плеер» работает как «Плеер приложения»'
  $options:
  - app: Плеер приложения (виджет только управляет приложением на ПК)
  - own: Встроенный плеер (виджет — источник музыки, ЛКМ разворачивает)
- exePath: '%LOCALAPPDATA%\Programs\YandexMusic\Яндекс Музыка.exe'
  $name: Путь к exe приложения
  $description: Что запускать/сворачивать по клику (переменные окружения поддерживаются)
- appFilter: yandex
  $name: Фильтр источника музыки
  $description: Часть AppUserModelId медиа-сессии (регистр не важен). Пусто — брать текущую системную сессию
- alignment: left
  $name: Привязка виджета
  $description: От какого края панели задач отсчитывается положение
  $options:
  - left: По левому краю
  - center: По центру
  - right: По правому краю
- offsetX: 12
  $name: Сдвиг по горизонтали (px при 100%)
  $description: Положительное значение — сдвиг вправо, отрицательное — влево (от выбранной привязки)
- scale: 100
  $name: Размер плеера (%)
  $description: Масштабирует весь виджет целиком — ширину, обложку, шрифты и кнопки (50–200). Обложка не выше панели задач
- width: 320
  $name: Ширина виджета (px при 100%)
  $description: Базовая ширина; итоговая = ширина × размер плеера
- collapseMode: delay
  $name: Сворачивание после ухода курсора
  $description: Что делать с развёрнутым плеером, когда курсор ушёл с него (в любом режиме можно свернуть правым кликом)
  $options:
  - instant: Сразу
  - delay: Через паузу (см. ниже)
  - never: Не сворачивать
- collapseDelay: 4
  $name: Пауза до сворачивания (сек)
  $description: Используется в режиме «Через паузу»
- bgOpacity: 85
  $name: Непрозрачность фона развёрнутого плеера (%)
  $description: 100 — сплошной фон, меньше — прозрачнее (5–100). Текст и обложка остаются чёткими
- lyricsOffset: 0
  $name: Сдвиг синхронизации текста песни (мс)
  $description: Если строки опаздывают — поставьте положительное значение, если спешат — отрицательное
- showInfoButton: true
  $name: Показывать кнопку «i»
  $description: Кнопка в углу панели текста, которая проверяет источники (LRCLIB, Genius) и показывает, почему текст не найден. Выключите, чтобы скрыть её
- geniusToken: ""
  $name: Genius API токен
  $description: 'Запасной источник текста, если в LRCLIB его нет. Токен: genius.com/api-clients → New API Client → Generate Access Token (Client Access Token). Пусто — Genius не используется'
- ymToken: ""
  $name: Токен Яндекс Музыки
  $description: 'OAuth-токен аккаунта. Нужен для режима «Встроенный плеер»: виджет играет музыку сам (поиск, плейлисты, «Моя волна», лайки), без приложения. Пусто — виджет показывает то, что играет в системе'
- volumeStep: 5
  $name: Шаг громкости колесом мыши (%)
  $description: На сколько процентов меняется громкость за один щелчок колеса над развёрнутым плеером (1–50)
- fontSize: 14
  $name: Размер шрифта заголовка
- textColor: FFFFFF
  $name: Цвет текста (HEX, RRGGBB)
  $description: Для светлой панели задач поставьте 000000
*/
// ==/WindhawkModSettings==

#include <windows.h>
#include <windowsx.h>
#include <shellapi.h>
#include <mmdeviceapi.h>
#include <audiopolicy.h>
#include <winhttp.h>
#include <wincrypt.h>
#undef GetCurrentTime

#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <chrono>
#include <ctime>
#include <deque>
#include <map>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Control.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Graphics.Imaging.h>
#include <winrt/Windows.Media.h>
#include <winrt/Windows.Media.Core.h>
#include <winrt/Windows.Media.Playback.h>

using namespace winrt::Windows::Media::Control;

// ---------------------------------------------------------------- настройки
struct Settings {
    std::wstring exePath;
    std::wstring appFilter;
    int alignment = 0;  // 0 - слева, 1 - по центру, 2 - справа
    int offsetX = 12;
    int width = 320;
    int scalePct = 100;  // общий масштаб плеера, %
    int fontSize = 14;
    int collapseMode = 1;   // 0 - сразу, 1 - через паузу, 2 - не сворачивать
    int collapseDelay = 4;  // сек (режим 1)
    int volumeStep = 5;     // % за щелчок колеса
    int bgOpacity = 85;     // непрозрачность фона острова, %
    int lyricsOffsetMs = 0; // сдвиг синхронизации текста, мс
    std::string geniusToken; // токен Genius API (пусто — Genius выключен)
    std::string ymToken;     // OAuth-токен Яндекс Музыки (пусто — берём музыку из системной медиа-сессии)
    bool showInfoBtn = true; // показывать кнопку «i» в панели текста
    bool ownSource = true;   // режим работы: false - плеер приложения, true - встроенный плеер (источник музыки)
    BYTE r = 255, g = 255, b = 255;
} g_set;

static std::wstring ToLower(std::wstring s) {
    for (auto& c : s) c = towlower(c);
    return s;
}

static std::vector<std::pair<DWORD, bool>> g_pidCache;

static void LoadSettings() {
    PCWSTR s = Wh_GetStringSetting(L"exePath");
    wchar_t buf[MAX_PATH * 2];
    ExpandEnvironmentStringsW(s, buf, ARRAYSIZE(buf));
    g_set.exePath = buf;
    Wh_FreeStringSetting(s);

    s = Wh_GetStringSetting(L"playerMode");
    g_set.ownSource = ToLower(s) != L"app";
    Wh_FreeStringSetting(s);

    s = Wh_GetStringSetting(L"appFilter");
    g_set.appFilter = ToLower(s);
    Wh_FreeStringSetting(s);

    s = Wh_GetStringSetting(L"alignment");
    std::wstring al = ToLower(s);
    Wh_FreeStringSetting(s);
    g_set.alignment = al == L"center" ? 1 : al == L"right" ? 2 : 0;

    g_set.offsetX = Wh_GetIntSetting(L"offsetX");
    g_set.width = std::max(120, Wh_GetIntSetting(L"width"));
    g_set.scalePct = std::clamp((int)Wh_GetIntSetting(L"scale"), 50, 200);
    g_set.fontSize = std::max(8, Wh_GetIntSetting(L"fontSize"));
    g_set.collapseDelay = std::max(0, Wh_GetIntSetting(L"collapseDelay"));
    g_set.volumeStep = std::clamp((int)Wh_GetIntSetting(L"volumeStep"), 1, 50);
    g_set.bgOpacity = std::clamp((int)Wh_GetIntSetting(L"bgOpacity"), 5, 100);
    g_set.lyricsOffsetMs = std::clamp((int)Wh_GetIntSetting(L"lyricsOffset"), -5000, 5000);
    g_set.showInfoBtn = Wh_GetIntSetting(L"showInfoButton") != 0;
    s = Wh_GetStringSetting(L"geniusToken");
    g_set.geniusToken.clear();
    for (PCWSTR p = s; p && *p; p++)
        if (*p > 32 && *p < 127) g_set.geniusToken += (char)*p;  // токен — ASCII, пробелы по краям отбрасываем
    Wh_FreeStringSetting(s);
    s = Wh_GetStringSetting(L"ymToken");
    g_set.ymToken.clear();
    for (PCWSTR p = s; p && *p; p++)
        if (*p > 32 && *p < 127) g_set.ymToken += (char)*p;
    Wh_FreeStringSetting(s);
    s = Wh_GetStringSetting(L"collapseMode");
    std::wstring cm = ToLower(s);
    Wh_FreeStringSetting(s);
    g_set.collapseMode = cm == L"instant" ? 0 : cm == L"never" ? 2 : 1;
    g_pidCache.clear();  // путь приложения мог измениться

    s = Wh_GetStringSetting(L"textColor");
    unsigned long v = wcstoul(s, nullptr, 16);
    Wh_FreeStringSetting(s);
    g_set.r = (v >> 16) & 255;
    g_set.g = (v >> 8) & 255;
    g_set.b = v & 255;
}

// ---------------------------------------------------------------- состояние
static HWND g_hwnd = nullptr;
static HANDLE g_thread = nullptr;
static volatile bool g_stopping = false;

static int g_x, g_y, g_w, g_h;           // текущее окно
static int g_hc, g_wc, g_we, g_he;       // компактный / развёрнутый размеры
static bool g_bottom = true;             // панель внизу экрана — растём вверх
static int g_oxC, g_oyC, g_oxE, g_oyE;   // смещение «кадров» содержимого внутри окна
static double g_scale = 1.0;
static bool g_hover = false;
static int g_btn = 0;  // 0 - нет, 1 - назад, 2 - пауза/играть, 3 - далее
static bool g_expand = false;   // целевое состояние: развёрнут ли плеер
static double g_expT = 0.0;     // линейный прогресс раскрытия 0..1
static int g_drag = 0;          // 0 - нет, 1 - перемотка, 2 - громкость
static double g_dragFrac = 0.0;

// состояния анимаций
static double g_stageT = 0.0;    // поэтапное появление содержимого развёрнутого плеера 0..1
static double g_playT = 0.0;     // 0 — пауза, 1 — играет (значок play/pause, размер обложки)
static double g_textT = 1.0;     // появление текста при смене трека
static double g_hotT[20] = {};   // 15 — имя исполнителя, 16 — настройки волны, 17 — встряхнуть; наведение: 1 назад, 2 play, 3 вперёд, 4 значок громкости, 5 шкала, 6 громкость, 7 текст песни, 8 проверка источников,
                                 // 9 библиотека, 10 «нравится», 11 поле поиска, 12 «Моя волна», 13 и 14 вкладки библиотеки
static double g_pressT[8] = {};  // нажатие кнопок 1..7 (6 — библиотека, 7 — «нравится»)
static int g_hot = 0, g_pressed = 0;
static std::vector<DWORD> g_coverOld;  // предыдущая обложка для кросс-фейда
static int g_coverOldPx = 0;
static double g_coverFade = 1.0;

// текст песни
struct LyLine {
    long long t;
    std::wstring text;
};
static bool g_lyOpen = false;            // панель текста открыта
static double g_lyT = 0.0;               // анимация открытия панели 0..1
static std::wstring g_lyKey;             // для какого трека загружен/запрошен текст
static bool g_lyBusy = false;            // идёт загрузка
static int g_lyState = 0;                // 0 - нет, 1 - загрузка, 2 - готово, 3 - не найден, 4 - инструментал
static bool g_lySynced = false;
static std::vector<LyLine> g_lyrics;
static int g_lyIdx = 0;                  // текущая строка (по времени)
static double g_lyScroll = 0.0;          // плавная «позиция прокрутки» в строках
static double g_lyManual = 0.0;          // ручная прокрутка колесом, px
static ULONGLONG g_lyManualUntil = 0;
static std::vector<int> g_lyH;           // кэш высот строк
static int g_lyHW = 0, g_lyHF = 0;
static std::vector<int> g_lyPosY;        // верхние края строк в окне (для клика по строке)
static HANDLE g_lyThread = nullptr;
static std::wstring g_lyDiag;            // почему текст не нашёлся (по источникам)
static std::wstring g_lySource;          // откуда взят показанный текст (для сноски в конце)
static bool g_diagShow = false;          // в панели текста показан отчёт проверки источников
static bool g_chkBusy = false;           // проверка источников выполняется
static std::wstring g_diagText;          // отчёт проверки
static HANDLE g_chkThread = nullptr;

static std::wstring g_title;
static std::wstring g_artist;
static bool g_playing = false;

// таймлайн трека (в тиках по 100 нс)
static long long g_durTicks = 0;      // длительность; 0 — приложение таймлайн не отдаёт
static long long g_startTicks = 0;    // начало шкалы (обычно 0)
static long long g_posBase = 0;       // позиция (от начала) на момент g_posBaseTick
static ULONGLONG g_posBaseTick = 0;
static ULONGLONG g_seekHold = 0;      // до этого момента не принимаем позицию от приложения (после перемотки)
static bool g_posFreeze = false;      // свой плеер: трек открывается/буферизуется — шкала стоит

static long long CurPos() {
    long long p = g_posBase;
    if (g_playing && !g_posFreeze) p += (long long)(GetTickCount64() - g_posBaseTick) * 10000LL;
    if (g_durTicks > 0) p = std::min(p, g_durTicks);
    return std::max(0LL, p);
}

// громкость приложения-источника (сессия в микшере Windows)
static float g_vol = 1.0f;
static bool g_muted = false;
static bool g_volPending = false;  // громкость задана, но аудио-сессии ещё нет — применим, когда появится
static ULONGLONG g_volApplyTick = 0;

static GlobalSystemMediaTransportControlsSessionManager g_mgr{nullptr};

// обложка (премультиплицированный BGRA, квадрат g_coverPx x g_coverPx)
static std::vector<DWORD> g_cover;
static int g_coverPx = 0;
static std::wstring g_coverKey;
static int g_coverTries = 0;
static std::wstring g_trackKey;
static ULONGLONG g_lastSeen = 0;  // когда в последний раз видели валидный трек

// подписка на события сессии (мгновенная реакция на смену трека)
static GlobalSystemMediaTransportControlsSession g_subSession{nullptr};
static std::wstring g_subId;
static winrt::event_token g_tokProps{}, g_tokPlay{};

// S — с учётом DPI и пользовательского масштаба плеера; SD — только DPI
static int S(int v) { return (int)(v * g_scale * g_set.scalePct / 100.0 + 0.5); }
static int SD(int v) { return (int)(v * g_scale + 0.5); }
static int CellW() { return S(36); }

static void StartAnim(HWND h);  // определена ниже

// встроенный плеер Яндекс Музыки: включён режим «Встроенный плеер» и указан токен;
// иначе — системная медиа-сессия (плеер приложения на ПК)
static bool OwnMode() { return g_set.ownSource && !g_set.ymToken.empty(); }
static void OwnPoll();
static void OwnToggle();
static void OwnNext();
static void OwnPrev();
static void OwnSeek(double frac);
static void OwnApplyVolume();
static void OwnSaveVolume();
static void CloseSearchPopup();
static bool g_menuOpen = false;  // открыто контекстное меню трека (курсор «уходит» с виджета, но сворачивать нельзя)
static void OnLeave(HWND h);     // определена ниже

// ---------------------------------------------------------------- палитра по обложке
struct Rgb {
    float r, g, b;
};
struct Palette {
    Rgb bgTop, bgBot, text, text2, accent;
};

static Palette DefaultPalette() {
    return Palette{{52, 52, 58}, {26, 26, 30}, {245, 245, 247}, {160, 160, 168}, {235, 235, 240}};
}
static Palette g_palCur = DefaultPalette();  // то, что рисуется сейчас
static Palette g_palTgt = DefaultPalette();  // к чему плавно идём

static Rgb MixRgb(const Rgb& a, const Rgb& b, float t) {
    return Rgb{a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t};
}

// r,g,b, h,s,l — всё в диапазоне 0..1
static void RgbToHsl(float r, float g, float b, float& h, float& s, float& l) {
    float mx = std::max({r, g, b}), mn = std::min({r, g, b});
    l = (mx + mn) * 0.5f;
    float d = mx - mn;
    float den = 1.f - fabsf(2.f * l - 1.f);
    if (d < 1e-6f || den < 1e-6f) {
        h = 0.f;
        s = 0.f;
        return;
    }
    s = d / den;
    if (mx == r) h = fmodf((g - b) / d, 6.f);
    else if (mx == g) h = (b - r) / d + 2.f;
    else h = (r - g) / d + 4.f;
    h /= 6.f;
    if (h < 0.f) h += 1.f;
}

static void HslToRgb(float h, float s, float l, float& r, float& g, float& b) {
    float c = (1.f - fabsf(2.f * l - 1.f)) * s;
    float hp = h * 6.f;
    float x = c * (1.f - fabsf(fmodf(hp, 2.f) - 1.f));
    float r1 = 0, g1 = 0, b1 = 0;
    if (hp < 1.f) { r1 = c; g1 = x; }
    else if (hp < 2.f) { r1 = x; g1 = c; }
    else if (hp < 3.f) { g1 = c; b1 = x; }
    else if (hp < 4.f) { g1 = x; b1 = c; }
    else if (hp < 5.f) { r1 = x; b1 = c; }
    else { r1 = c; b1 = x; }
    float m = l - c * 0.5f;
    r = r1 + m;
    g = g1 + m;
    b = b1 + m;
}

static Rgb HslRgb255(float h, float s, float l) {
    float r, g, b;
    HslToRgb(h, s, l, r, g, b);
    return Rgb{r * 255.f, g * 255.f, b * 255.f};
}

// Доминирующий «цветной» оттенок обложки -> тёмный фон, светлый текст и яркий акцент
static Palette PaletteFromCover() {
    if (g_cover.empty() || g_coverPx <= 0) return DefaultPalette();
    const int n = g_coverPx;
    const int step = std::max(1, n / 40);
    double wSum[12] = {}, rS[12] = {}, gS[12] = {}, bS[12] = {};
    int cnt = 0;
    for (int y = 0; y < n; y += step) {
        for (int x = 0; x < n; x += step) {
            DWORD p = g_cover[(size_t)y * n + x];
            unsigned a = p >> 24;
            if (a < 16) continue;
            float r = std::min(1.f, ((p >> 16) & 255) / (float)a);
            float g = std::min(1.f, ((p >> 8) & 255) / (float)a);
            float b = std::min(1.f, (p & 255) / (float)a);
            float h, s, l;
            RgbToHsl(r, g, b, h, s, l);
            float w = s * (1.f - fabsf(2.f * l - 1.f));  // насыщенные, не слишком тёмные/светлые
            w *= w;
            int k = std::min(11, (int)(h * 12.f));
            wSum[k] += w;
            rS[k] += r * w;
            gS[k] += g * w;
            bS[k] += b * w;
            cnt++;
        }
    }
    if (!cnt) return DefaultPalette();
    int best = 0;
    for (int k = 1; k < 12; k++)
        if (wSum[k] > wSum[best]) best = k;

    float h = 0.f, s = 0.f, l = 0.f;
    if (wSum[best] >= 0.03 * cnt)
        RgbToHsl((float)(rS[best] / wSum[best]), (float)(gS[best] / wSum[best]),
                 (float)(bS[best] / wSum[best]), h, s, l);
    // иначе обложка почти серая — палитра нейтральная

    const bool col = s > 0.f;
    Palette P;
    P.bgTop = HslRgb255(h, col ? std::clamp(s * 0.8f, 0.30f, 0.70f) : 0.f, 0.26f);
    P.bgBot = HslRgb255(h, col ? std::clamp(s * 0.8f, 0.30f, 0.70f) : 0.f, 0.12f);
    P.text = HslRgb255(h, col ? 0.35f : 0.f, 0.96f);
    P.text2 = MixRgb(P.text, P.bgTop, 0.45f);
    P.accent = HslRgb255(h, col ? std::clamp(s, 0.55f, 0.95f) : 0.f, col ? 0.66f : 0.88f);
    return P;
}

// плавное приближение текущей палитры к целевой; true — ещё движется
static bool StepPalette(double ms) {
    float k = (float)std::min(1.0, ms / 140.0);
    float diff = 0.f;
    auto st = [&](Rgb& a, const Rgb& b) {
        a.r += (b.r - a.r) * k;
        a.g += (b.g - a.g) * k;
        a.b += (b.b - a.b) * k;
        diff += fabsf(b.r - a.r) + fabsf(b.g - a.g) + fabsf(b.b - a.b);
    };
    st(g_palCur.bgTop, g_palTgt.bgTop);
    st(g_palCur.bgBot, g_palTgt.bgBot);
    st(g_palCur.text, g_palTgt.text);
    st(g_palCur.text2, g_palTgt.text2);
    st(g_palCur.accent, g_palTgt.accent);
    if (diff < 2.f) {
        g_palCur = g_palTgt;
        return false;
    }
    return true;
}


// ---------------------------------------------------------------- SMTC
static GlobalSystemMediaTransportControlsSession FindSession() {
    try {
        if (!g_mgr) return nullptr;
        if (g_set.appFilter.empty()) return g_mgr.GetCurrentSession();
        for (auto const& s : g_mgr.GetSessions()) {
            std::wstring id = ToLower(std::wstring(s.SourceAppUserModelId().c_str()));
            if (id.find(g_set.appFilter) != std::wstring::npos) return s;
        }
    } catch (...) {
        g_mgr = nullptr;  // менеджер протух (служба SMTC перезапущена) — Poll создаст заново
    }
    return nullptr;
}

// размер обложки в компактном режиме (не выше панели задач) и в развёрнутом
static int CoverCompactSize() {
    if (g_hc <= 0) return 0;
    int base = g_hc - SD(12);  // размер обложки при 100%
    int sz = (int)(base * g_set.scalePct / 100.0 + 0.5);
    return std::clamp(sz, 16, std::max(16, g_hc - 2));
}
static int CoverExpSize() { return S(56); }
// грузим сразу по большему размеру и масштабируем при отрисовке
static int CoverSize() {
    if (g_hc <= 0) return 0;
    return std::max(CoverCompactSize(), CoverExpSize());
}

// true — обложка загружена и заменена; при неудаче старая остаётся нетронутой
static bool LoadCoverStream(winrt::Windows::Storage::Streams::IRandomAccessStream const& stream, int size) {
    namespace Img = winrt::Windows::Graphics::Imaging;
    try {
        auto decoder = Img::BitmapDecoder::CreateAsync(stream).get();
        Img::BitmapTransform tr;
        tr.ScaledWidth(size);
        tr.ScaledHeight(size);
        tr.InterpolationMode(Img::BitmapInterpolationMode::Fant);
        auto sb = decoder
                      .GetSoftwareBitmapAsync(Img::BitmapPixelFormat::Bgra8,
                                              Img::BitmapAlphaMode::Premultiplied, tr,
                                              Img::ExifOrientationMode::IgnoreExifOrientation,
                                              Img::ColorManagementMode::DoNotColorManage)
                      .get();
        winrt::Windows::Storage::Streams::Buffer buf((uint32_t)size * size * 4);
        sb.CopyToBuffer(buf);
        buf.Length((uint32_t)size * size * 4);
        DWORD* d = (DWORD*)buf.data();
        g_cover.assign(d, d + (size_t)size * size);
        g_coverPx = size;
        g_palTgt = PaletteFromCover();
        if (g_hwnd) StartAnim(g_hwnd);  // плавная смена цветов
        return true;
    } catch (...) {
        return false;
    }
}

static bool LoadCover(GlobalSystemMediaTransportControlsSessionMediaProperties const& props, int size) {
    try {
        auto ref = props.Thumbnail();
        if (!ref) return false;
        auto stream = ref.OpenReadAsync().get();
        return LoadCoverStream(stream, size);
    } catch (...) {
        return false;
    }
}

static void Unsubscribe() {
    if (!g_subSession) return;
    try {
        g_subSession.MediaPropertiesChanged(g_tokProps);
        g_subSession.PlaybackInfoChanged(g_tokPlay);
    } catch (...) {
    }
    g_subSession = nullptr;
    g_subId.clear();
}

static void Subscribe(GlobalSystemMediaTransportControlsSession const& s) {
    std::wstring id = s.SourceAppUserModelId().c_str();
    if (g_subSession && id == g_subId) return;
    Unsubscribe();
    g_subSession = s;
    g_subId = id;
    g_tokProps = s.MediaPropertiesChanged(
        [](GlobalSystemMediaTransportControlsSession const&, MediaPropertiesChangedEventArgs const&) {
            if (g_hwnd) PostMessageW(g_hwnd, WM_APP + 2, 0, 0);
        });
    g_tokPlay = s.PlaybackInfoChanged(
        [](GlobalSystemMediaTransportControlsSession const&, PlaybackInfoChangedEventArgs const&) {
            if (g_hwnd) PostMessageW(g_hwnd, WM_APP + 2, 0, 0);
        });
}

static void ClearState() {
    Unsubscribe();
    g_title.clear();
    g_artist.clear();
    g_playing = false;
    g_cover.clear();
    g_coverPx = 0;
    g_coverKey.clear();
    g_trackKey.clear();
    g_coverOld.clear();
    g_durTicks = 0;
    g_posBase = 0;
    g_palTgt = DefaultPalette();
    if (g_hwnd) StartAnim(g_hwnd);
}

// При быстром переключении сессия/метаданные на мгновение пропадают —
// состояние сбрасываем только если трека нет дольше 3 секунд.
static void MaybeClear() {
    if (GetTickCount64() - g_lastSeen > 5000) ClearState();
}

static void EnsureManager() {
    if (g_mgr) return;
    static ULONGLONG lastTry = 0;
    ULONGLONG now = GetTickCount64();
    if (lastTry && now - lastTry < 3000) return;
    lastTry = now;
    try {
        g_mgr = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
    } catch (...) {
    }
}

static void Poll() {
    if (OwnMode()) {  // свой плеер: состояние ведёт OwnPoll
        OwnPoll();
        return;
    }
    try {
        EnsureManager();
        auto s = FindSession();
        if (!s) {
            MaybeClear();
            return;
        }
        Subscribe(s);
        // не виснем навсегда, если приложение-источник не отвечает
        auto op = s.TryGetMediaPropertiesAsync();
        if (op.wait_for(std::chrono::seconds(2)) !=
            winrt::Windows::Foundation::AsyncStatus::Completed) {
            op.Cancel();
            return;
        }
        auto props = op.GetResults();
        std::wstring newTitle = props.Title().c_str();
        std::wstring newArtist = props.Artist().c_str();
        if (newTitle.empty() && newArtist.empty()) {
            MaybeClear();
            return;
        }
        g_lastSeen = GetTickCount64();
        g_title = newTitle;
        g_artist = newArtist;
        bool nowPlaying = s.GetPlaybackInfo().PlaybackStatus() ==
                          GlobalSystemMediaTransportControlsSessionPlaybackStatus::Playing;
        if (nowPlaying != g_playing) {
            g_playing = nowPlaying;
            if (g_hwnd) StartAnim(g_hwnd);  // анимация значка и обложки
        }

        try {  // таймлайн: длительность и позиция (с экстраполяцией по времени обновления)
            auto tl = s.GetTimelineProperties();
            long long st = tl.StartTime().count();
            long long en = tl.EndTime().count();
            long long pos = tl.Position().count() - st;
            g_startTicks = st;
            g_durTicks = en > st ? en - st : 0;
            if (g_playing) {
                long long el = (winrt::clock::now() - tl.LastUpdatedTime()).count();
                if (el > 0 && el < 36000000000LL) pos += el;  // игнорируем мусорные метки времени
            }
            if (g_durTicks > 0) pos = std::clamp(pos, 0LL, g_durTicks);
            else pos = std::max(0LL, pos);
            if (GetTickCount64() >= g_seekHold && g_drag != 1) {
                g_posBase = pos;
                g_posBaseTick = GetTickCount64();
            }
        } catch (...) {
            g_durTicks = 0;
        }

        const int kMaxTries = 15;
        std::wstring key = g_title + L"\x1f" + g_artist;
        if (key != g_trackKey) {
            g_trackKey = key;
            g_coverTries = 0;
            g_textT = 0.0;  // текст нового трека плавно появляется
            if (g_hwnd) StartAnim(g_hwnd);
        }
        int cs = CoverSize();
        bool newTrack = key != g_coverKey;
        bool resize = !newTrack && !g_cover.empty() && g_coverPx != cs;
        if (cs > 0 && ((newTrack && g_coverTries < kMaxTries) || resize)) {
            g_coverTries++;
            std::vector<DWORD> prevCover;
            int prevPx = 0;
            if (newTrack && !g_cover.empty()) {
                prevCover = g_cover;
                prevPx = g_coverPx;
            }
            if (LoadCover(props, cs)) {
                g_coverKey = key;  // старая обложка заменена только теперь — без «мигания»
                if (!prevCover.empty()) {  // плавный переход старой обложки в новую
                    g_coverOld.swap(prevCover);
                    g_coverOldPx = prevPx;
                    g_coverFade = 0.0;
                    if (g_hwnd) StartAnim(g_hwnd);
                }
            } else if (newTrack) {
                if (g_coverTries < kMaxTries) {
                    if (g_hwnd) SetTimer(g_hwnd, 3, 200, nullptr);  // быстрый повтор
                } else {
                    g_cover.clear();  // у трека реально нет обложки
                    g_coverPx = 0;
                    g_palTgt = DefaultPalette();
                    if (g_hwnd) StartAnim(g_hwnd);
                    g_coverKey = key;
                }
            }
        }
    } catch (...) {
    }
}

static void DoMediaCommand(int btn) {
    if (OwnMode()) {
        if (btn == 1) OwnPrev();
        else if (btn == 2) OwnToggle();
        else if (btn == 3) OwnNext();
        return;
    }
    try {
        auto s = FindSession();
        if (!s) return;
        if (btn == 1) s.TrySkipPreviousAsync();
        else if (btn == 2) s.TryTogglePlayPauseAsync();
        else if (btn == 3) s.TrySkipNextAsync();
    } catch (...) {
    }
}

// перемотка: frac 0..1 от длительности трека
static void DoSeek(double frac) {
    if (OwnMode()) {
        OwnSeek(frac);
        return;
    }
    try {
        auto s = FindSession();
        if (!s || g_durTicks <= 0) return;
        frac = std::clamp(frac, 0.0, 1.0);
        long long rel = (long long)(frac * (double)g_durTicks);
        s.TryChangePlaybackPositionAsync(g_startTicks + rel);
        g_posBase = rel;
        g_posBaseTick = GetTickCount64();
        g_seekHold = g_posBaseTick + 1500;  // пока приложение не «догнало» — не откатываем ползунок
    } catch (...) {
    }
}

// ---------------------------------------------------------------- громкость приложения (Core Audio)
static const GUID kClsidMMDeviceEnumerator = {0xBCDE0395, 0xE52F, 0x467C, {0x8E, 0x3D, 0xC4, 0x57, 0x92, 0x91, 0x69, 0x2E}};
static const GUID kIidMMDeviceEnumerator = {0xA95664D2, 0x9614, 0x4F35, {0xA7, 0x46, 0xDE, 0x8D, 0xB6, 0x36, 0x17, 0xE6}};
static const GUID kIidAudioSessionManager2 = {0x77AA99A0, 0x1BD6, 0x484F, {0x8B, 0xC7, 0x2C, 0x65, 0x4C, 0x9A, 0x9B, 0x6F}};
static const GUID kIidAudioSessionControl2 = {0xBFB7FF88, 0x7239, 0x4FC9, {0x8F, 0xA2, 0x07, 0xC9, 0x50, 0xBE, 0x9C, 0x6D}};
static const GUID kIidSimpleAudioVolume = {0x87CE5498, 0x68D6, 0x44E5, {0x92, 0x15, 0x6D, 0xA4, 0x7E, 0xF8, 0x83, 0xD8}};

static std::wstring BaseNameLower(const std::wstring& p) {
    size_t i = p.find_last_of(L"\\/");
    return ToLower(i == std::wstring::npos ? p : p.substr(i + 1));
}

// принадлежит ли процесс целевому приложению (у Electron звук играет в дочернем процессе того же exe)
static ULONGLONG g_pidCacheTick = 0;

static bool PidIsTarget(DWORD pid) {
    ULONGLONG now = GetTickCount64();
    if (now - g_pidCacheTick > 10000) {
        g_pidCache.clear();
        g_pidCacheTick = now;
    }
    for (auto& e : g_pidCache)
        if (e.first == pid) return e.second;
    bool ok = false;
    HANDLE p = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (p) {
        wchar_t path[MAX_PATH * 2];
        DWORD len = ARRAYSIZE(path);
        if (QueryFullProcessImageNameW(p, 0, path, &len) && !g_set.exePath.empty())
            ok = BaseNameLower(path) == BaseNameLower(g_set.exePath);
        CloseHandle(p);
    }
    g_pidCache.push_back({pid, ok});
    return ok;
}

// вызывает fn для каждой аудио-сессии целевого приложения; true — найдена хотя бы одна
template <class F>
static bool ForEachTargetVolume(F&& fn) {
    bool any = false;
    IMMDeviceEnumerator* en = nullptr;
    IMMDevice* dev = nullptr;
    IAudioSessionManager2* mgr = nullptr;
    IAudioSessionEnumerator* se = nullptr;
    if (SUCCEEDED(CoCreateInstance(kClsidMMDeviceEnumerator, nullptr, CLSCTX_ALL,
                                   kIidMMDeviceEnumerator, (void**)&en)) && en &&
        SUCCEEDED(en->GetDefaultAudioEndpoint(eRender, eMultimedia, &dev)) && dev &&
        SUCCEEDED(dev->Activate(kIidAudioSessionManager2, CLSCTX_ALL, nullptr, (void**)&mgr)) && mgr &&
        SUCCEEDED(mgr->GetSessionEnumerator(&se)) && se) {
        int n = 0;
        se->GetCount(&n);
        for (int i = 0; i < n; i++) {
            IAudioSessionControl* c = nullptr;
            if (FAILED(se->GetSession(i, &c)) || !c) continue;
            IAudioSessionControl2* c2 = nullptr;
            if (SUCCEEDED(c->QueryInterface(kIidAudioSessionControl2, (void**)&c2)) && c2) {
                DWORD pid = 0;
                if (SUCCEEDED(c2->GetProcessId(&pid)) && pid && PidIsTarget(pid)) {
                    ISimpleAudioVolume* v = nullptr;
                    if (SUCCEEDED(c->QueryInterface(kIidSimpleAudioVolume, (void**)&v)) && v) {
                        fn(v);
                        any = true;
                        v->Release();
                    }
                }
                c2->Release();
            }
            c->Release();
        }
    }
    if (se) se->Release();
    if (mgr) mgr->Release();
    if (dev) dev->Release();
    if (en) en->Release();
    return any;
}

// записать g_vol/g_muted в сессии приложения (force=false — не чаще раза в 30 мс)
static void ApplyVolume(bool force) {
    if (OwnMode()) {  // свой плеер: громкость задаём прямо MediaPlayer
        OwnApplyVolume();
        if (force) OwnSaveVolume();
        return;
    }
    ULONGLONG now = GetTickCount64();
    if (!force && now - g_volApplyTick < 30) return;
    g_volApplyTick = now;
    const float vol = g_vol;
    const BOOL mu = g_muted ? TRUE : FALSE;
    bool any = ForEachTargetVolume([&](ISimpleAudioVolume* v) {
        v->SetMasterVolume(vol, nullptr);
        v->SetMute(mu, nullptr);
    });
    g_volPending = !any;
}

// прочитать актуальную громкость приложения (когда не тянем ползунок)
static void PollVolume() {
    if (g_drag == 2 || OwnMode()) return;
    float mx = -1.f;
    bool mu = false;
    bool any = ForEachTargetVolume([&](ISimpleAudioVolume* v) {
        float f = 0.f;
        if (SUCCEEDED(v->GetMasterVolume(&f))) mx = std::max(mx, f);
        BOOL m = FALSE;
        if (SUCCEEDED(v->GetMute(&m)) && m) mu = true;
    });
    if (!any) return;
    if (g_volPending) {
        ApplyVolume(true);
        return;
    }
    if (mx >= 0.f) {
        g_vol = mx;
        g_muted = mu;
    }
}

// ---------------------------------------------------------------- текст песни (LRCLIB, Genius)
struct LyJob {
    std::wstring key, artist, title;
    long long durSec;
    std::string geniusToken;
};
struct LyResult {
    std::wstring key;
    int state;  // 2 — готово, 3 — не найдено, 4 — инструментал
    bool synced;
    std::vector<LyLine> lines;
    std::wstring diag;  // что сказал каждый источник (показывается, если текст не найден)
    std::wstring source;  // источник, из которого взят текст
};

static std::string Utf8(const std::wstring& w) {
    if (w.empty()) return std::string();
    int n = WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::string s(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.c_str(), (int)w.size(), &s[0], n, nullptr, nullptr);
    return s;
}
static std::wstring Wide(const std::string& s) {
    if (s.empty()) return std::wstring();
    int n = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), nullptr, 0);
    std::wstring w(n, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, s.c_str(), (int)s.size(), &w[0], n);
    return w;
}

static std::wstring UrlEnc(const std::string& s) {
    static const wchar_t* hex = L"0123456789ABCDEF";
    std::wstring o;
    for (unsigned char c : s) {
        if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '-' || c == '_' ||
            c == '.' || c == '~') {
            o += (wchar_t)c;
        } else {
            o += L'%';
            o += hex[c >> 4];
            o += hex[c & 15];
        }
    }
    return o;
}

static void AppendUtf8(std::string& o, unsigned cp) {
    if (cp < 0x80) {
        o += (char)cp;
    } else if (cp < 0x800) {
        o += (char)(0xC0 | (cp >> 6));
        o += (char)(0x80 | (cp & 0x3F));
    } else if (cp < 0x10000) {
        o += (char)(0xE0 | (cp >> 12));
        o += (char)(0x80 | ((cp >> 6) & 0x3F));
        o += (char)(0x80 | (cp & 0x3F));
    } else {
        o += (char)(0xF0 | (cp >> 18));
        o += (char)(0x80 | ((cp >> 12) & 0x3F));
        o += (char)(0x80 | ((cp >> 6) & 0x3F));
        o += (char)(0x80 | (cp & 0x3F));
    }
}

static bool Hex4(const std::string& s, size_t i, unsigned& v) {
    if (i + 4 > s.size()) return false;
    v = 0;
    for (size_t k = 0; k < 4; k++) {
        char c = s[i + k];
        v <<= 4;
        if (c >= '0' && c <= '9') v |= (unsigned)(c - '0');
        else if (c >= 'a' && c <= 'f') v |= (unsigned)(c - 'a' + 10);
        else if (c >= 'A' && c <= 'F') v |= (unsigned)(c - 'A' + 10);
        else return false;
    }
    return true;
}

// все значения строкового поля key в JSON (по порядку; null -> пустая строка)
static std::vector<std::string> JsonStrings(const std::string& js, const char* key) {
    std::vector<std::string> out;
    const std::string pat = std::string("\"") + key + "\"";
    size_t pos = 0;
    while ((pos = js.find(pat, pos)) != std::string::npos) {
        size_t i = pos + pat.size();
        pos = i;
        while (i < js.size() && (js[i] == ' ' || js[i] == '\t' || js[i] == '\n' || js[i] == '\r')) i++;
        if (i >= js.size() || js[i] != ':') continue;
        i++;
        while (i < js.size() && (js[i] == ' ' || js[i] == '\t' || js[i] == '\n' || js[i] == '\r')) i++;
        if (i >= js.size() || js[i] != '"') {
            out.push_back(std::string());  // null и т.п.
            continue;
        }
        i++;
        std::string v;
        while (i < js.size() && js[i] != '"') {
            char c = js[i];
            if (c == '\\' && i + 1 < js.size()) {
                char e = js[++i];
                if (e == 'n') v += '\n';
                else if (e == 't') v += '\t';
                else if (e == 'u') {
                    unsigned cp = 0;
                    if (Hex4(js, i + 1, cp)) {
                        i += 4;
                        if (cp >= 0xD800 && cp < 0xDC00 && i + 6 < js.size() && js[i + 1] == '\\' &&
                            js[i + 2] == 'u') {
                            unsigned lo = 0;
                            if (Hex4(js, i + 3, lo) && lo >= 0xDC00 && lo < 0xE000) {
                                cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
                                i += 6;
                            }
                        }
                        AppendUtf8(v, cp);
                    }
                } else if (e != 'r' && e != 'b' && e != 'f') {
                    v += e;  // \" \\ \/
                }
            } else {
                v += c;
            }
            i++;
        }
        out.push_back(v);
        pos = i;
    }
    return out;
}

static std::string TrimStr(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && (s[a] == ' ' || s[a] == '\t' || s[a] == '\r' || s[a] == '\n')) a++;
    while (b > a && (s[b - 1] == ' ' || s[b - 1] == '\t' || s[b - 1] == '\r' || s[b - 1] == '\n')) b--;
    return s.substr(a, b - a);
}

// [mm:ss.xx] в позиции i -> тики; i сдвигается за скобку
static bool ParseLrcTag(const std::string& s, size_t& i, long long& ticks) {
    size_t j = i + 1;
    int mm = 0, ss = 0;
    bool d = false;
    while (j < s.size() && s[j] >= '0' && s[j] <= '9') {
        mm = mm * 10 + (s[j] - '0');
        j++;
        d = true;
    }
    if (!d || j >= s.size() || s[j] != ':') return false;
    j++;
    d = false;
    while (j < s.size() && s[j] >= '0' && s[j] <= '9') {
        ss = ss * 10 + (s[j] - '0');
        j++;
        d = true;
    }
    if (!d) return false;
    double fr = 0.0;
    if (j < s.size() && (s[j] == '.' || s[j] == ':')) {
        j++;
        double sc = 0.1;
        while (j < s.size() && s[j] >= '0' && s[j] <= '9') {
            fr += (s[j] - '0') * sc;
            sc *= 0.1;
            j++;
        }
    }
    if (j >= s.size() || s[j] != ']') return false;
    ticks = (long long)((mm * 60.0 + ss + fr) * 10000000.0);
    i = j + 1;
    return true;
}

static std::vector<std::string> SplitLines(const std::string& s) {
    std::vector<std::string> out;
    size_t a = 0;
    while (a <= s.size()) {
        size_t b = s.find('\n', a);
        if (b == std::string::npos) b = s.size();
        out.push_back(s.substr(a, b - a));
        a = b + 1;
    }
    return out;
}

// synced (LRC) в приоритете; иначе обычный текст
static void ParseLyrics(const std::string& synced, const std::string& plain, std::vector<LyLine>& out,
                        bool& isSynced) {
    out.clear();
    isSynced = false;
    if (!synced.empty()) {
        for (const std::string& line : SplitLines(synced)) {
            size_t i = 0;
            std::vector<long long> ts;
            while (i < line.size() && line[i] == '[') {
                long long t = 0;
                if (!ParseLrcTag(line, i, t)) break;
                ts.push_back(t);
            }
            if (ts.empty()) continue;  // метаданные [ar:...] и т.п.
            std::wstring w = Wide(TrimStr(line.substr(i)));
            if (w.empty()) w = L"\u266A";
            for (long long t : ts) out.push_back({t, w});
        }
        std::stable_sort(out.begin(), out.end(), [](const LyLine& a, const LyLine& b) { return a.t < b.t; });
        isSynced = !out.empty();
    }
    if (out.empty() && !plain.empty()) {
        bool prevEmpty = true;  // убираем пустые строки в начале и подряд
        for (const std::string& line : SplitLines(plain)) {
            std::wstring w = Wide(TrimStr(line));
            if (w.empty() && prevEmpty) continue;
            prevEmpty = w.empty();
            out.push_back({0, w});
        }
        while (!out.empty() && out.back().text.empty()) out.pop_back();
    }
}

// результат запроса: HTTP-код (0 — ответа не было) и код ошибки WinHTTP
struct HttpInfo {
    int status = 0;
    DWORD err = 0;
};

// Тело возвращается при коде 200; если передан info — и при любом другом (чтобы разобрать ответ с ошибкой)
static std::string HttpGet(const wchar_t* host, const std::wstring& path, const std::wstring& headers = L"",
                           const wchar_t* ua = nullptr, HttpInfo* info = nullptr, bool http1 = false) {
    std::string out;
    HttpInfo local;
    HttpInfo& hi = info ? *info : local;
    hi = HttpInfo();
    HINTERNET ses = WinHttpOpen(ua ? ua : L"TaskbarMusicWidget/1.0 (Windhawk mod)", 4 /*AUTOMATIC_PROXY*/,
                                nullptr, nullptr, 0);
    if (!ses) {
        hi.err = GetLastError();
        return out;
    }
    WinHttpSetTimeouts(ses, 4000, 4000, 5000, 6000);
    if (ua) {  // запрос «как из браузера» (Genius/Cloudflare режут всё, что на браузер не похоже)
        DWORD prot = 0x00000A00;  // TLS 1.2 | TLS 1.3
        if (!WinHttpSetOption(ses, 84 /*WINHTTP_OPTION_SECURE_PROTOCOLS*/, &prot, sizeof(prot))) {
            prot = 0x00000800;
            WinHttpSetOption(ses, 84, &prot, sizeof(prot));
        }
        if (!http1) {  // http1 — «упрощённый» запрос по HTTP/1.1 (другой отпечаток соединения, если HTTP/2 не прошёл)
            DWORD h2 = 1;  // WINHTTP_PROTOCOL_FLAG_HTTP2
            WinHttpSetOption(ses, 133 /*WINHTTP_OPTION_ENABLE_HTTP_PROTOCOL*/, &h2, sizeof(h2));
        }
        DWORD dec = 3;  // WINHTTP_DECOMPRESSION_FLAG_ALL
        WinHttpSetOption(ses, 118 /*WINHTTP_OPTION_DECOMPRESSION*/, &dec, sizeof(dec));
    }
    HINTERNET con = WinHttpConnect(ses, host, INTERNET_DEFAULT_HTTPS_PORT, 0);
    HINTERNET req = con ? WinHttpOpenRequest(con, L"GET", path.c_str(), nullptr, WINHTTP_NO_REFERER,
                                             WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE)
                        : nullptr;
    bool ok = req && WinHttpSendRequest(req, headers.empty() ? WINHTTP_NO_ADDITIONAL_HEADERS : headers.c_str(),
                                        headers.empty() ? 0 : (DWORD)-1L, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
              WinHttpReceiveResponse(req, nullptr);
    if (!ok) {
        hi.err = GetLastError();
    } else {
        DWORD status = 0, sz = sizeof(status);
        WinHttpQueryHeaders(req, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                            WINHTTP_HEADER_NAME_BY_INDEX, &status, &sz, WINHTTP_NO_HEADER_INDEX);
        hi.status = (int)status;
        if (status == 200 || info) {
            DWORD avail = 0;
            while (WinHttpQueryDataAvailable(req, &avail) && avail) {
                std::string buf(avail, '\0');
                DWORD rd = 0;
                if (!WinHttpReadData(req, &buf[0], avail, &rd) || !rd) break;
                out.append(buf, 0, rd);
                if (out.size() > 4 * 1024 * 1024) break;
            }
        }
    }
    if (req) WinHttpCloseHandle(req);
    if (con) WinHttpCloseHandle(con);
    WinHttpCloseHandle(ses);
    return out;
}

// первый исполнитель из «A, B», «A & B», «A feat. B»
static std::wstring FirstArtist(const std::wstring& a) {
    size_t cut = a.find_first_of(L",&;");
    std::wstring r = a.substr(0, cut);
    size_t f = ToLower(r).find(L" feat");
    if (f != std::wstring::npos) r = r.substr(0, f);
    while (!r.empty() && r.back() == L' ') r.pop_back();
    return r;
}

// ---------------------------------------------------------------- Genius (запасной источник)
static std::string HtmlDecode(const std::string& s) {
    std::string o;
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] != '&') {
            o += s[i];
            continue;
        }
        size_t e = s.find(';', i);
        if (e == std::string::npos || e - i > 10) {
            o += s[i];
            continue;
        }
        std::string ent = s.substr(i + 1, e - i - 1);
        if (ent == "amp") o += '&';
        else if (ent == "lt") o += '<';
        else if (ent == "gt") o += '>';
        else if (ent == "quot") o += '"';
        else if (ent == "apos") o += '\'';
        else if (ent == "nbsp") o += ' ';
        else if (!ent.empty() && ent[0] == '#') {
            unsigned long cp = (ent.size() > 1 && (ent[1] == 'x' || ent[1] == 'X')) ? strtoul(ent.c_str() + 2, nullptr, 16)
                                                                                    : strtoul(ent.c_str() + 1, nullptr, 10);
            if (cp) AppendUtf8(o, (unsigned)cp);
            else o += s.substr(i, e - i + 1);
        } else {
            o += s.substr(i, e - i + 1);
        }
        i = e;
    }
    return o;
}

// текст из <div data-lyrics-container="true"> страницы Genius (<br> -> перевод строки, остальные теги убираем)
static std::string GeniusLyricsFromHtml(const std::string& h) {
    std::string out;
    const std::string key = "data-lyrics-container=\"true\"";
    size_t pos = 0;
    while ((pos = h.find(key, pos)) != std::string::npos) {
        size_t gt = h.find('>', pos);
        if (gt == std::string::npos) break;
        size_t i = gt + 1;
        int depth = 1, skip = 0;  // skip > 0 — внутри блока data-exclude-from-selection (шапка, «Read more»)
        while (i < h.size() && depth > 0) {
            if (h[i] == '<') {
                size_t e = h.find('>', i);
                if (e == std::string::npos) {
                    i = h.size();
                    break;
                }
                std::string tag = h.substr(i + 1, e - i - 1);
                bool closing = !tag.empty() && tag[0] == '/';
                size_t ns = closing ? 1 : 0, ne = ns;
                while (ne < tag.size() && isalnum((unsigned char)tag[ne])) ne++;
                std::string name = tag.substr(ns, ne - ns);
                for (auto& c : name) c = (char)tolower((unsigned char)c);
                if (name == "div") {
                    if (closing) {
                        depth--;
                        if (skip > 0) skip--;
                    } else {
                        depth++;
                        if (skip > 0) skip++;
                        else if (tag.find("data-exclude-from-selection") != std::string::npos) skip = 1;
                    }
                } else if (name == "br" && !skip) {
                    out += '\n';
                }
                i = e + 1;
            } else {
                size_t n = h.find('<', i);
                if (n == std::string::npos) n = h.size();
                if (!skip) out.append(h, i, n - i);
                i = n;
            }
        }
        out += '\n';
        pos = std::max(i, gt + 1);
    }
    out = HtmlDecode(out);
    // убираем служебные строки вида [Куплет 1], [Chorus]
    std::string res;
    for (const std::string& line : SplitLines(out)) {
        std::string t = TrimStr(line);
        if (t.size() >= 2 && t.front() == '[' && t.back() == ']') continue;
        res += line;
        res += '\n';
    }
    return res;
}

// для сравнения названий/исполнителей: только буквы и цифры в нижнем регистре, без скобок вроде (feat. X)
static std::wstring NormCmp(const std::wstring& s) {
    auto run = [&](bool dropParens) {
        std::wstring o;
        int depth = 0;
        for (wchar_t c : s) {
            if (dropParens && (c == L'(' || c == L'[')) {
                depth++;
                continue;
            }
            if (dropParens && (c == L')' || c == L']')) {
                if (depth > 0) depth--;
                continue;
            }
            if (depth) continue;
            if (iswalnum(c)) o += (wchar_t)towlower(c);
        }
        return o;
    };
    std::wstring r = run(true);
    return r.empty() ? run(false) : r;
}

static bool LooseMatch(const std::wstring& want, const std::wstring& got) {
    if (want.empty() || got.empty()) return false;
    if (want == got || got.find(want) != std::wstring::npos) return true;
    return got.size() >= 3 && want.find(got) != std::wstring::npos;
}

// расшифровка ошибки соединения WinHTTP
static std::wstring NetErrText(DWORD e) {
    switch (e) {
        case 12007: return L"сервер не найден (DNS)";
        case 12002: return L"таймаут — сервер не отвечает";
        case 12029: return L"не удаётся подключиться (сайт недоступен или заблокирован)";
        case 12030: return L"соединение разорвано";
        case 12175: case 12157: return L"ошибка TLS/сертификата";
        case 0: return L"нет ответа";
    }
    return L"нет соединения (код " + std::to_wstring(e) + L")";
}

static std::wstring GeniusHttpText(const HttpInfo& hi) {
    if (!hi.status) return NetErrText(hi.err);
    if (hi.status == 401) return L"токен не принят (401) — нужен Client Access Token";
    if (hi.status == 403) return L"доступ запрещён (403) — Genius отклонил запрос (защита от ботов)";
    if (hi.status == 429) return L"слишком много запросов (429)";
    return L"ответ HTTP " + std::to_wstring(hi.status);
}

static const wchar_t* kBrowserUA =
    L"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/124.0 Safari/537.36";
static const wchar_t* kAltUA = L"Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:125.0) Gecko/20100101 Firefox/125.0";

// Genius (genius.com стоит за Cloudflare) на часть запросов отвечает 403. Поэтому каждый запрос пробуется в нескольких
// «профилях»: сначала как Chrome по HTTP/2 с обычными браузерными заголовками, затем упрощённо как Firefox по HTTP/1.1.
static const wchar_t* kGeniusPageHdr =
    L"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
    L"Accept-Language: en-US,en;q=0.9\r\n"
    L"Referer: https://genius.com/\r\n"
    L"Upgrade-Insecure-Requests: 1\r\n"
    L"Sec-Fetch-Dest: document\r\nSec-Fetch-Mode: navigate\r\nSec-Fetch-Site: same-origin";
static const wchar_t* kGeniusPageHdrSimple = L"Accept: text/html,application/xhtml+xml,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.5";
static const wchar_t* kGeniusJsonHdr = L"Accept: application/json, text/plain, */*\r\nAccept-Language: en-US,en;q=0.9\r\nReferer: https://genius.com/";
static const wchar_t* kGeniusScriptHdr = L"Accept: */*\r\nAccept-Language: en-US,en;q=0.9\r\nReferer: https://genius.com/";

// ответ — страница-проверка Cloudflare, а не нужный контент
static bool LooksCloudflare(const std::string& b) {
    return b.find("Just a moment") != std::string::npos || b.find("challenge-platform") != std::string::npos ||
           b.find("cf-chl") != std::string::npos || b.find("cf_chl") != std::string::npos;
}

// запрос к genius.com (страница, embed.js, внутренний поиск): профиль A, при 403/429/503 — профиль B
static std::string GeniusSiteGet(const std::wstring& path, const wchar_t* hdrA, const wchar_t* hdrB, HttpInfo& hi) {
    std::string a = HttpGet(L"genius.com", path, hdrA, kBrowserUA, &hi);
    if (hi.status != 403 && hi.status != 429 && hi.status != 503) return a;
    HttpInfo h2;
    std::string b = HttpGet(L"genius.com", path, hdrB, kAltUA, &h2, true);
    if (h2.status == 200) {
        hi = h2;
        return b;
    }
    return a;
}

// запрос к api.genius.com; при 403 перебираем способы: без лишних заголовков, другой профиль, токен в параметре,
// «честный» User-Agent. via (если задан) получает номер сработавшего способа
static std::string GeniusApiGet(const std::string& token, const std::wstring& pathQuery, HttpInfo& hi, int* via = nullptr) {
    const std::wstring bearer = L"Authorization: Bearer " + Wide(token) + L"\r\n";
    const std::wstring accept = L"Accept: application/json";
    const std::wstring sep = pathQuery.find(L'?') == std::wstring::npos ? L"?" : L"&";
    struct Try {
        std::wstring path, hdr;
        const wchar_t* ua;
        bool http1;
    } tries[] = {
        {pathQuery, bearer + accept, kBrowserUA, false},
        {pathQuery, bearer + accept, kAltUA, true},
        {pathQuery + sep + L"access_token=" + Wide(token), accept, kBrowserUA, true},
        {pathQuery, bearer + accept, nullptr, true},
    };
    std::string js;
    int n = 0;
    for (const Try& tr : tries) {
        js = HttpGet(L"api.genius.com", tr.path, tr.hdr, tr.ua, &hi, tr.http1);
        if (hi.status != 403) {
            if (via) *via = n;
            return js;
        }
        n++;
    }
    return js;
}

// определены ниже, в разделе JSON-помощников
static std::vector<std::string> JsonArrayObjects(const std::string& js, const char* key, size_t maxN);
static std::string JsonScalar(const std::string& js, const char* key);

// поиск песни: сначала api.genius.com (с токеном); если API закрыт (403) — открытый поиск самого сайта genius.com.
// via: 0..3 — способ запроса к API, 9 — поиск через сайт
static std::string GeniusSearch(const std::string& token, const std::wstring& encQuery, HttpInfo& hi, int* via = nullptr) {
    std::string js = GeniusApiGet(token, L"/search?q=" + encQuery, hi, via);
    if (hi.status == 403) {
        HttpInfo h2;
        std::string js2 = GeniusSiteGet(L"/api/search/song?per_page=10&q=" + encQuery, kGeniusJsonHdr, kGeniusJsonHdr, h2);
        if (h2.status == 200) {
            hi = h2;
            if (via) *via = 9;
            return js2;
        }
    }
    return js;
}

// «встроенный» текст: genius.com/songs/<id>/embed.js (скрипт для сторонних сайтов, обычно не за капчей)
static std::string HtmlToText(const std::string& h) {
    std::string o;
    for (size_t i = 0; i < h.size();) {
        if (h[i] == '<') {
            size_t e = h.find('>', i);
            if (e == std::string::npos) break;
            std::string tag = h.substr(i + 1, e - i - 1);
            bool closing = !tag.empty() && tag[0] == '/';
            size_t ns = closing ? 1 : 0, ne = ns;
            while (ne < tag.size() && isalnum((unsigned char)tag[ne])) ne++;
            std::string name = tag.substr(ns, ne - ns);
            for (auto& c : name) c = (char)tolower((unsigned char)c);
            if (name == "br" || (closing && (name == "p" || name == "div"))) o += '\n';
            i = e + 1;
        } else {
            o += h[i++];
        }
    }
    return HtmlDecode(o);
}

static std::string GeniusLyricsFromEmbed(const std::string& js) {
    size_t p = js.find("JSON.parse('");
    if (p == std::string::npos) return std::string();
    std::string json;  // JS-литерал в одинарных кавычках -> JSON-строка
    for (size_t i = p + 12; i < js.size(); i++) {
        char c = js[i];
        if (c == '\\' && i + 1 < js.size()) {
            char e = js[++i];
            if (e == 'n') json += '\n';
            else if (e == 't') json += '\t';
            else if (e == 'u') json += "\\u";  // JsonStrings сам разберёт \uXXXX
            else json += e;
        } else if (c == '\'') {
            break;
        } else {
            json += c;
        }
    }
    auto v = JsonStrings(std::string("{\"x\":") + json + "}", "x");
    if (v.empty()) return std::string();
    std::string html = v[0];
    size_t b = html.find("rg_embed_body");
    if (b == std::string::npos) return std::string();
    size_t gt = html.find('>', b);
    if (gt == std::string::npos) return std::string();
    size_t f = html.find("rg_embed_footer", gt);
    std::string text = HtmlToText(html.substr(gt + 1, f == std::string::npos ? std::string::npos : f - gt - 1));
    std::string res;
    for (const std::string& line : SplitLines(text)) {
        std::string tl = TrimStr(line);
        if (tl.size() >= 2 && tl.front() == '[' && tl.back() == ']') continue;
        res += line;
        res += '\n';
    }
    return res;
}

// Страницу genius.com через WinHTTP Cloudflare режет по отпечатку TLS. Python-библиотека requests (OpenSSL) проходит,
// поэтому при 403 страницу скачивает внешний python (нужны python и пакет requests). status — HTTP-код страницы (0 — не вышло)
static std::string PyFetch(const std::wstring& url, int* status) {
    *status = 0;
    wchar_t tmp[MAX_PATH];
    DWORD n = GetTempPathW(MAX_PATH, tmp);
    if (!n || n >= MAX_PATH) return std::string();
    const std::wstring script = std::wstring(tmp) + L"tmw_genius_fetch.py";
    static const char* kPy =
        "import sys, requests\n"
        "H = {'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) "
        "Chrome/124.0.0.0 Safari/537.36', "
        "'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8', "
        "'Accept-Language': 'en-US,en;q=0.9', 'Accept-Encoding': 'gzip, deflate', "
        "'Upgrade-Insecure-Requests': '1', 'Sec-Fetch-Dest': 'document', 'Sec-Fetch-Mode': 'navigate', "
        "'Sec-Fetch-Site': 'none', 'Sec-Fetch-User': '?1'}\n"
        "r = requests.get(sys.argv[1], headers=H, timeout=15)\n"
        "sys.stdout.buffer.write(str(r.status_code).encode() + b'\\n' + r.content)\n";
    HANDLE f = CreateFileW(script.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (f == INVALID_HANDLE_VALUE) return std::string();
    DWORD wr = 0;
    WriteFile(f, kPy, (DWORD)strlen(kPy), &wr, nullptr);
    CloseHandle(f);

    std::string out;
    for (int attempt = 0; attempt < 2 && out.empty(); attempt++) {
        SECURITY_ATTRIBUTES sa{sizeof(sa), nullptr, TRUE};
        HANDLE rd = nullptr, wp = nullptr;
        if (!CreatePipe(&rd, &wp, &sa, 0)) break;
        SetHandleInformation(rd, HANDLE_FLAG_INHERIT, 0);
        HANDLE nul = CreateFileW(L"NUL", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &sa,
                                 OPEN_EXISTING, 0, nullptr);
        std::wstring cmd = std::wstring(attempt == 0 ? L"python" : L"py -3") + L" \"" + script + L"\" \"" + url + L"\"";
        std::vector<wchar_t> buf(cmd.begin(), cmd.end());
        buf.push_back(0);
        STARTUPINFOW si{};
        si.cb = sizeof(si);
        si.dwFlags = STARTF_USESTDHANDLES;
        si.hStdInput = nul;
        si.hStdOutput = wp;
        si.hStdError = nul;
        PROCESS_INFORMATION pi{};
        bool started = CreateProcessW(nullptr, buf.data(), nullptr, nullptr, TRUE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi);
        CloseHandle(wp);  // свою копию закрываем, чтобы чтение закончилось, когда python завершится
        if (nul != INVALID_HANDLE_VALUE) CloseHandle(nul);
        if (started) {
            char b[16384];
            DWORD got = 0;
            while (ReadFile(rd, b, sizeof(b), &got, nullptr) && got) {
                out.append(b, got);
                if (out.size() > 4 * 1024 * 1024) break;
            }
            WaitForSingleObject(pi.hProcess, 3000);
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
        }
        CloseHandle(rd);
    }
    size_t nl = out.find('\n');
    if (nl == std::string::npos || nl > 5) return std::string();
    *status = atoi(out.substr(0, nl).c_str());
    return out.substr(nl + 1);
}

// ищет песню через Genius (нужен токен) и достаёт текст со страницы; пусто — не нашли
// diag (если задан) получает причину неудачи
static std::string GeniusLyrics(const std::string& token, const std::wstring& artist, const std::wstring& title,
                                std::wstring* diag = nullptr) {
    if (token.empty()) return std::string();
    std::wstring a1 = FirstArtist(artist);
    HttpInfo hi;
    int via = 0;
    std::string js = GeniusSearch(token, UrlEnc(Utf8(a1 + L" " + title)), hi, &via);
    if (hi.status != 200) {
        if (diag) *diag += L"Genius: поиск — " + GeniusHttpText(hi) + L"\n";
        return std::string();
    }

    struct Hit {
        std::string path, id;
        std::wstring title, artist;
    };
    std::vector<Hit> hits;
    for (const std::string& o : JsonArrayObjects(js, "hits", 20)) {
        Hit h;
        h.path = JsonScalar(o, "path");
        h.title = Wide(JsonScalar(o, "title"));
        h.artist = Wide(JsonScalar(o, "artist_names"));
        const std::string ap = JsonScalar(o, "api_path");
        if (ap.compare(0, 7, "/songs/") == 0) h.id = ap.substr(7);
        if (!h.path.empty()) hits.push_back(h);
    }
    const std::wstring wantT = NormCmp(title), wantA = NormCmp(a1);
    int tried = 0;
    HttpInfo page, emb;
    bool pageCf = false, embTried = false;
    int pyStatus = -1;  // -1 — python не пробовали, 0 — не запустился/нет requests
    for (size_t i = 0; i < hits.size() && tried < 2; i++) {
        const Hit& h = hits[i];
        std::wstring gt = NormCmp(h.title), ga = NormCmp(h.artist);
        std::wstring lp = ToLower(Wide(h.path));
        // страницы-переводы («Genius English Translations» и т.п.) не нужны
        if (ga.find(L"genius") != std::wstring::npos || lp.find(L"translation") != std::wstring::npos ||
            lp.find(L"traduc") != std::wstring::npos)
            continue;
        // не берём текст «похожей» песни: должны совпасть и название, и исполнитель
        if (!LooseMatch(wantT, gt) || !LooseMatch(wantA, ga)) continue;
        tried++;
        std::string html = GeniusSiteGet(Wide(h.path), kGeniusPageHdr, kGeniusPageHdrSimple, page);
        std::string text = GeniusLyricsFromHtml(html);
        if (!TrimStr(text).empty()) return text;
        if (page.status != 200) pageCf = pageCf || LooksCloudflare(html);
        if (!h.id.empty()) {  // страница закрыта или разметка изменилась — пробуем embed.js по id песни
            embTried = true;
            std::string ej = GeniusSiteGet(L"/songs/" + Wide(h.id) + L"/embed.js", kGeniusScriptHdr, kGeniusScriptHdr, emb);
            std::string et = GeniusLyricsFromEmbed(ej);
            if (!TrimStr(et).empty()) return et;
        }
        if (page.status == 403 || pageCf) {
            // WinHTTP закрыт Cloudflare — пробуем скачать страницу через python requests
            int pst = 0;
            std::string ph = PyFetch(L"https://genius.com" + Wide(h.path), &pst);
            std::string pt = GeniusLyricsFromHtml(ph);
            if (!TrimStr(pt).empty()) return pt;
            pyStatus = pst;
            break;  // остальные страницы Cloudflare тоже закроет
        }
    }
    if (diag) {
        if (tried > 0) {
            *diag += L"Genius: песня найдена, но текст не получен (страница: ";
            *diag += page.status == 200 ? L"загрузилась, текст на ней не распознан"
                     : page.status      ? L"HTTP " + std::to_wstring(page.status)
                                        : NetErrText(page.err);
            if (pageCf) *diag += L", проверка Cloudflare";
            *diag += L")";
            if (embTried)
                *diag += L", embed.js: " + (emb.status == 200 ? std::wstring(L"получен, текст не распознан")
                                            : emb.status      ? L"HTTP " + std::to_wstring(emb.status)
                                                              : NetErrText(emb.err));
            if (pyStatus == 0)
                *diag += L"\nPython не сработал (нужны python в PATH и пакет requests: pip install requests)";
            else if (pyStatus > 0)
                *diag += L"\nPython requests: HTTP " + std::to_wstring(pyStatus);
            if (page.status == 403 || emb.status == 403)
                *diag += L"\nGenius блокирует запросы с этого компьютера/сети (попробуйте VPN или другой источник)";
            *diag += L"\n";
        } else if (hits.empty()) {
            *diag += L"Genius: по запросу ничего не найдено\n";
        } else {
            *diag += L"Genius: найдено " + std::to_wstring(hits.size()) +
                     L", но нет совпадения по названию и исполнителю (первый результат: " + hits[0].title + L" — " +
                     hits[0].artist + L")\n";
        }
    }
    return std::string();
}

// ---------------------------------------------------------------- запасные источники без защиты от ботов
// NetEase Cloud Music: открытый API, отдаёт текст с метками времени (LRC). Ищем по «исполнитель название»,
// берём только результат, у которого совпали и название, и исполнитель
static std::string NeteaseLyrics(const std::wstring& artist, const std::wstring& title, std::wstring* diag = nullptr) {
    const std::wstring hdr = L"Referer: https://music.163.com/\r\nCookie: appver=2.0.2\r\nAccept: */*";
    std::wstring a1 = FirstArtist(artist);
    HttpInfo hi;
    std::string js = HttpGet(L"music.163.com",
                             L"/api/search/get/web?csrf_token=&hlpretag=&hlposttag=&type=1&offset=0&total=true&limit=8&s=" +
                                 UrlEnc(Utf8(a1 + L" " + title)),
                             hdr, kBrowserUA, &hi);
    if (hi.status != 200) {
        if (diag) *diag += L"NetEase: поиск — " + (hi.status ? L"HTTP " + std::to_wstring(hi.status) : NetErrText(hi.err)) + L"\n";
        return std::string();
    }
    const std::wstring wantT = NormCmp(title), wantA = NormCmp(a1);
    for (const std::string& o : JsonArrayObjects(js, "songs", 8)) {
        std::vector<std::string> names = JsonStrings(o, "name");  // [название, исполнители..., альбом]
        if (names.size() < 2) continue;
        if (!LooseMatch(wantT, NormCmp(Wide(names[0])))) continue;
        bool artistOk = false;
        for (size_t i = 1; i < names.size() && !artistOk; i++) artistOk = LooseMatch(wantA, NormCmp(Wide(names[i])));
        if (!artistOk) continue;
        const std::string id = JsonScalar(o, "id");
        if (id.empty()) continue;
        HttpInfo h2;
        std::string lj = HttpGet(L"music.163.com", L"/api/song/lyric?lv=1&kv=1&tv=-1&id=" + Wide(id), hdr, kBrowserUA, &h2);
        if (h2.status != 200 || lj.find("\"nolyric\":true") != std::string::npos) continue;
        auto v = JsonStrings(lj, "lyric");  // первый — оригинал (lrc), второй — перевод (tlyric), его не берём
        if (!v.empty() && !TrimStr(v[0]).empty()) return v[0];
    }
    if (diag) *diag += L"NetEase: текст не найден\n";
    return std::string();
}

// lyrics.ovh: простой открытый API, обычный текст без меток времени
static std::string OvhLyrics(const std::wstring& artist, const std::wstring& title, std::wstring* diag = nullptr) {
    HttpInfo hi;
    std::string js = HttpGet(L"api.lyrics.ovh", L"/v1/" + UrlEnc(Utf8(FirstArtist(artist))) + L"/" + UrlEnc(Utf8(title)), L"",
                             nullptr, &hi);
    if (hi.status == 200) {
        auto v = JsonStrings(js, "lyrics");
        if (!v.empty() && !TrimStr(v[0]).empty()) return v[0];
    }
    if (diag) *diag += L"lyrics.ovh: текст не найден\n";
    return std::string();
}

// ---------------------------------------------------------------- встроенный плеер Яндекс Музыки
// Всё сетевое (API Яндекс Музыки) выполняется в фоновых потоках, результат приходит в окно сообщением WM_APP+6.
// Звук играет системный MediaPlayer (Windows.Media.Playback): без внешних программ и без файлов на диске.
namespace Pb = winrt::Windows::Media::Playback;
namespace MCore = winrt::Windows::Media::Core;

struct YmTrack {
    std::string id, albumId, cover;  // cover — coverUri без схемы, с «%%» вместо размера
    std::wstring title, artists, albumTitle;
    std::vector<std::pair<std::string, std::wstring>> artistList;  // исполнители: id и имя (для перехода в профиль)
    long long durMs = 0;
    bool available = true;
};
struct YmPlaylist {
    std::string kind;  // «liked» — «Мне нравится»
    std::wstring title;
    int count = 0;
    std::string cover;  // uri обложки без схемы, с «%%» вместо размера (пусто — обложки нет)
    std::string uid;    // владелец, если плейлист не свой (например «Плейлист дня»); пусто — свой
};
struct YmArtist {
    std::string id;
    std::wstring name;
    std::string cover;
};

static std::string YmFullId(const YmTrack& t) {
    return t.albumId.empty() ? t.id : t.id + ":" + t.albumId;
}

// ---- минимальный разбор JSON (ответы API вложенные, поэтому искать подстроки уже недостаточно)
struct Jv {
    enum Type { Null, Bool, Num, Str, Arr, Obj } t = Null;
    bool b = false;
    double n = 0;
    std::string s;
    std::vector<Jv> a;
    std::vector<std::string> keys;
    std::vector<Jv> vals;

    const Jv& operator[](const char* k) const {
        static const Jv none{};
        for (size_t i = 0; i < keys.size(); i++)
            if (keys[i] == k) return vals[i];
        return none;
    }
    const Jv& at(size_t i) const {
        static const Jv none{};
        return i < a.size() ? a[i] : none;
    }
    size_t size() const { return a.size(); }
    std::string str() const {  // строка; число — как целое
        if (t == Str) return s;
        if (t == Num) {
            char buf[40];
            snprintf(buf, sizeof(buf), "%.0f", n);
            return buf;
        }
        return std::string();
    }
    long long num() const { return t == Num ? (long long)n : 0; }
};

struct JsonP {
    const std::string& s;
    size_t i = 0;
    int depth = 0;
    explicit JsonP(const std::string& str) : s(str) {}
    void ws() {
        while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == '\n' || s[i] == '\r')) i++;
    }
    bool lit(const char* w) {
        size_t n = strlen(w);
        if (s.compare(i, n, w) == 0) {
            i += n;
            return true;
        }
        return false;
    }
    bool str(std::string& o) {
        if (i >= s.size() || s[i] != '"') return false;
        i++;
        while (i < s.size() && s[i] != '"') {
            char c = s[i];
            if (c == '\\' && i + 1 < s.size()) {
                char e = s[++i];
                switch (e) {
                    case 'n': o += '\n'; break;
                    case 't': o += '\t'; break;
                    case 'r': o += '\r'; break;
                    case 'b': o += '\b'; break;
                    case 'f': o += '\f'; break;
                    case 'u': {
                        unsigned cp = 0;
                        if (!Hex4(s, i + 1, cp)) return false;
                        i += 4;
                        if (cp >= 0xD800 && cp < 0xDC00 && i + 6 < s.size() && s[i + 1] == '\\' && s[i + 2] == 'u') {
                            unsigned lo = 0;
                            if (Hex4(s, i + 3, lo) && lo >= 0xDC00 && lo < 0xE000) {
                                cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
                                i += 6;
                            }
                        }
                        AppendUtf8(o, cp);
                        break;
                    }
                    default: o += e; break;
                }
            } else {
                o += c;
            }
            i++;
        }
        if (i >= s.size()) return false;
        i++;  // закрывающая кавычка
        return true;
    }
    bool val(Jv& v) {
        ws();
        if (i >= s.size() || depth > 64) return false;
        const char c = s[i];
        if (c == '{') {
            v.t = Jv::Obj;
            i++;
            depth++;
            ws();
            if (i < s.size() && s[i] == '}') {
                i++;
                depth--;
                return true;
            }
            for (;;) {
                ws();
                std::string k;
                if (!str(k)) return false;
                ws();
                if (i >= s.size() || s[i] != ':') return false;
                i++;
                Jv x;
                if (!val(x)) return false;
                v.keys.push_back(std::move(k));
                v.vals.push_back(std::move(x));
                ws();
                if (i < s.size() && s[i] == ',') {
                    i++;
                    continue;
                }
                if (i < s.size() && s[i] == '}') {
                    i++;
                    break;
                }
                return false;
            }
            depth--;
            return true;
        }
        if (c == '[') {
            v.t = Jv::Arr;
            i++;
            depth++;
            ws();
            if (i < s.size() && s[i] == ']') {
                i++;
                depth--;
                return true;
            }
            for (;;) {
                Jv x;
                if (!val(x)) return false;
                v.a.push_back(std::move(x));
                ws();
                if (i < s.size() && s[i] == ',') {
                    i++;
                    continue;
                }
                if (i < s.size() && s[i] == ']') {
                    i++;
                    break;
                }
                return false;
            }
            depth--;
            return true;
        }
        if (c == '"') {
            v.t = Jv::Str;
            return str(v.s);
        }
        if (lit("true")) {
            v.t = Jv::Bool;
            v.b = true;
            return true;
        }
        if (lit("false")) {
            v.t = Jv::Bool;
            v.b = false;
            return true;
        }
        if (lit("null")) return true;
        char* end = nullptr;
        double d = strtod(s.c_str() + i, &end);
        if (end == s.c_str() + i) return false;
        v.t = Jv::Num;
        v.n = d;
        i = (size_t)(end - s.c_str());
        return true;
    }
};

static Jv JsonParse(const std::string& js) {
    Jv v;
    JsonP p(js);
    if (!p.val(v)) return Jv();
    return v;
}

// ---- MD5 (подпись прямой ссылки) через CryptoAPI
static std::string Md5Hex(const std::string& data) {
    std::string out;
    HCRYPTPROV prov = 0;
    HCRYPTHASH hash = 0;
    if (CryptAcquireContextW(&prov, nullptr, nullptr, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        if (CryptCreateHash(prov, CALG_MD5, 0, 0, &hash)) {
            if (CryptHashData(hash, (const BYTE*)data.data(), (DWORD)data.size(), 0)) {
                BYTE d[16];
                DWORD n = sizeof(d);
                if (CryptGetHashParam(hash, HP_HASHVAL, d, &n, 0) && n == 16) {
                    static const char* hx = "0123456789abcdef";
                    for (int i = 0; i < 16; i++) {
                        out += hx[d[i] >> 4];
                        out += hx[d[i] & 15];
                    }
                }
            }
            CryptDestroyHash(hash);
        }
        CryptReleaseContext(prov, 0);
    }
    return out;
}

// ---- HTTP к api.music.yandex.net (тело возвращается при любом коде — в нём бывает описание ошибки)
static const wchar_t* kYmHost = L"api.music.yandex.net";
static const wchar_t* kYmClientHdr = L"X-Yandex-Music-Client: YandexMusicAndroid/24023621";

static std::string YmRequest(const std::string& token, bool post, const std::wstring& path, const std::string& body,
                             const wchar_t* ctype, HttpInfo* info) {
    std::string out;
    HttpInfo local;
    HttpInfo& hi = info ? *info : local;
    hi = HttpInfo();
    HINTERNET ses = WinHttpOpen(L"Yandex-Music-API", 4 /*AUTOMATIC_PROXY*/, nullptr, nullptr, 0);
    if (!ses) {
        hi.err = GetLastError();
        return out;
    }
    WinHttpSetTimeouts(ses, 5000, 5000, 8000, 12000);
    HINTERNET con = WinHttpConnect(ses, kYmHost, INTERNET_DEFAULT_HTTPS_PORT, 0);
    HINTERNET req = con ? WinHttpOpenRequest(con, post ? L"POST" : L"GET", path.c_str(), nullptr, WINHTTP_NO_REFERER,
                                             WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE)
                        : nullptr;
    std::wstring hdr = std::wstring(kYmClientHdr) + L"\r\nAccept: application/json\r\nAccept-Language: ru\r\n";
    if (!token.empty()) hdr += L"Authorization: OAuth " + Wide(token) + L"\r\n";
    if (post && ctype) hdr += std::wstring(L"Content-Type: ") + ctype + L"\r\n";
    const DWORD blen = post ? (DWORD)body.size() : 0;
    bool ok = req && WinHttpSendRequest(req, hdr.c_str(), (DWORD)-1L,
                                        blen ? (LPVOID)body.data() : WINHTTP_NO_REQUEST_DATA, blen, blen, 0) &&
              WinHttpReceiveResponse(req, nullptr);
    if (!ok) {
        hi.err = GetLastError();
    } else {
        DWORD status = 0, sz = sizeof(status);
        WinHttpQueryHeaders(req, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER, WINHTTP_HEADER_NAME_BY_INDEX,
                            &status, &sz, WINHTTP_NO_HEADER_INDEX);
        hi.status = (int)status;
        DWORD avail = 0;
        while (WinHttpQueryDataAvailable(req, &avail) && avail) {
            std::string buf(avail, '\0');
            DWORD rd = 0;
            if (!WinHttpReadData(req, &buf[0], avail, &rd) || !rd) break;
            out.append(buf, 0, rd);
            if (out.size() > 8 * 1024 * 1024) break;
        }
    }
    if (req) WinHttpCloseHandle(req);
    if (con) WinHttpCloseHandle(con);
    WinHttpCloseHandle(ses);
    return out;
}

static std::string UrlEncA(const std::string& s) {
    return Utf8(UrlEnc(s));
}

static std::wstring YmErr(const HttpInfo& hi, const Jv& root) {
    if (!hi.status) return NetErrText(hi.err);
    if (hi.status == 401 || hi.status == 403) return L"токен не принят (HTTP " + std::to_wstring(hi.status) + L")";
    std::wstring m = L"HTTP " + std::to_wstring(hi.status);
    std::string e = root["error"]["message"].str();
    if (e.empty()) e = root["error"]["name"].str();
    if (!e.empty()) m += L": " + Wide(e);
    return m;
}

// ---- разбор объектов API
static YmTrack YmParseTrack(const Jv& t) {
    YmTrack o;
    o.id = t["id"].str();
    if (o.id.empty()) o.id = t["realId"].str();
    size_t c = o.id.find(':');  // бывает «трек:альбом»
    if (c != std::string::npos) {
        o.albumId = o.id.substr(c + 1);
        o.id.resize(c);
    }
    if (o.albumId.empty()) o.albumId = t["albums"].at(0)["id"].str();
    o.albumTitle = Wide(t["albums"].at(0)["title"].str());
    o.title = Wide(t["title"].str());
    std::string ar;
    const Jv& A = t["artists"];
    for (size_t i = 0; i < A.size(); i++) {
        std::string n = A.at(i)["name"].str();
        if (n.empty()) continue;
        if (!ar.empty()) ar += ", ";
        ar += n;
        o.artistList.push_back({A.at(i)["id"].str(), Wide(n)});
    }
    o.artists = Wide(ar);
    o.durMs = t["durationMs"].num();
    o.cover = t["coverUri"].str();
    if (o.cover.empty()) o.cover = t["ogImage"].str();
    const Jv& av = t["available"];
    o.available = !(av.t == Jv::Bool && !av.b);
    return o;
}

// ---- вызовы API (блокирующие — только из фоновых потоков)
static const size_t kMaxPlTracks = 300;  // сколько треков плейлиста берём (чтобы «Мне нравится» на тысячи треков не грузился минутами)

static std::string YmUserId(const std::string& token, std::wstring* err) {
    HttpInfo hi;
    std::string js = YmRequest(token, false, L"/account/status", "", nullptr, &hi);
    Jv r = JsonParse(js);
    std::string uid = r["result"]["account"]["uid"].str();
    if (uid.empty() && err) *err = YmErr(hi, r);
    return uid;
}

static bool YmSearch(const std::string& token, const std::string& q, std::vector<YmTrack>& out, std::wstring& err) {
    HttpInfo hi;
    std::string js = YmRequest(
        token, false, L"/search?text=" + UrlEnc(q) + L"&nocorrect=false&type=track&page=0&playlist-in-best=true", "",
        nullptr, &hi);
    Jv r = JsonParse(js);
    const Jv& res = r["result"]["tracks"]["results"];
    if (res.t != Jv::Arr) {
        if (hi.status == 200) return true;  // ничего не нашлось
        err = YmErr(hi, r);
        return false;
    }
    for (size_t i = 0; i < res.size(); i++) {
        YmTrack t = YmParseTrack(res.at(i));
        if (!t.id.empty() && t.available) out.push_back(t);
    }
    return true;
}

// поиск исполнителей (до 5 первых)
static bool YmSearchArtists(const std::string& token, const std::string& q, std::vector<YmArtist>& out) {
    HttpInfo hi;
    std::string js = YmRequest(token, false, L"/search?text=" + UrlEnc(q) + L"&nocorrect=false&type=artist&page=0", "",
                               nullptr, &hi);
    Jv r = JsonParse(js);
    const Jv& res = r["result"]["artists"]["results"];
    if (res.t != Jv::Arr) return false;
    for (size_t i = 0; i < res.size() && out.size() < 5; i++) {
        const Jv& a = res.at(i);
        YmArtist o;
        o.id = a["id"].str();
        o.name = Wide(a["name"].str());
        o.cover = a["cover"]["uri"].str();
        if (o.cover.empty()) o.cover = a["ogImage"].str();
        if (!o.id.empty() && !o.name.empty()) out.push_back(o);
    }
    return true;
}

// треки исполнителя (профиль)
static bool YmArtistTracks(const std::string& token, const std::string& id, std::vector<YmTrack>& out,
                           std::wstring& err) {
    HttpInfo hi;
    std::string js = YmRequest(token, false, L"/artists/" + Wide(id) + L"/tracks?page=0&page-size=100", "", nullptr, &hi);
    Jv r = JsonParse(js);
    const Jv& res = r["result"]["tracks"];
    if (res.t != Jv::Arr) {
        err = YmErr(hi, r);
        return false;
    }
    for (size_t i = 0; i < res.size(); i++) {
        YmTrack t = YmParseTrack(res.at(i));
        if (!t.id.empty() && t.available) out.push_back(t);
    }
    return true;
}

// треки альбома (все тома подряд)
static bool YmAlbumTracks(const std::string& token, const std::string& id, std::vector<YmTrack>& out,
                          std::wstring& err) {
    HttpInfo hi;
    std::string js = YmRequest(token, false, L"/albums/" + Wide(id) + L"/with-tracks", "", nullptr, &hi);
    Jv r = JsonParse(js);
    const Jv& vols = r["result"]["volumes"];
    if (vols.t != Jv::Arr) {
        err = YmErr(hi, r);
        return false;
    }
    const std::wstring at = Wide(r["result"]["title"].str());
    for (size_t v = 0; v < vols.size(); v++) {
        const Jv& vt = vols.at(v);
        for (size_t i = 0; i < vt.size(); i++) {
            YmTrack t = YmParseTrack(vt.at(i));
            if (t.id.empty() || !t.available) continue;
            if (t.albumId.empty()) t.albumId = id;
            if (t.albumTitle.empty()) t.albumTitle = at;
            out.push_back(t);
        }
    }
    return true;
}

static std::string YmPlaylistCoverUri(const Jv& p) {
    const Jv& cv = p["cover"];
    std::string c = cv["uri"].str();
    if (c.empty()) {
        const Jv& mo = cv["itemsUri"];
        if (mo.t == Jv::Arr && mo.size() > 0) c = mo.at(0).str();
    }
    if (c.empty()) c = p["ogImage"].str();
    return c;
}

// «Плейлист дня»: персональная подборка Яндекса, лежит в блоке главной страницы (запасной путь — лента)
static bool YmDailyPlaylist(const std::string& token, YmPlaylist& out) {
    auto fill = [&](const Jv& p) -> bool {
        std::string uid = p["uid"].str();
        if (uid.empty()) uid = p["owner"]["uid"].str();
        const std::string kind = p["kind"].str();
        if (uid.empty() || kind.empty()) return false;
        out = YmPlaylist();
        out.kind = kind;
        out.uid = uid;
        out.title = L"Плейлист дня";
        out.count = (int)p["trackCount"].num();
        out.cover = YmPlaylistCoverUri(p);
        return true;
    };
    {
        HttpInfo hi;
        std::string js = YmRequest(token, false, L"/landing3?blocks=personalplaylists", "", nullptr, &hi);
        Jv r = JsonParse(js);
        const Jv& blocks = r["result"]["blocks"];
        for (size_t i = 0; i < blocks.size(); i++) {
            const Jv& ents = blocks.at(i)["entities"];
            for (size_t k = 0; k < ents.size(); k++) {
                const Jv& p = ents.at(k)["data"]["data"];
                if (p["generatedPlaylistType"].str() == "playlistOfTheDay" && fill(p)) return true;
            }
        }
    }
    {
        HttpInfo hi;
        std::string js = YmRequest(token, false, L"/feed", "", nullptr, &hi);
        Jv r = JsonParse(js);
        const Jv& gp = r["result"]["generatedPlaylists"];
        for (size_t i = 0; i < gp.size(); i++) {
            const Jv& g = gp.at(i);
            if (g["type"].str() == "playlistOfTheDay" && fill(g["data"])) return true;
        }
    }
    return false;
}

static bool YmPlaylists(const std::string& token, const std::string& uid, std::vector<YmPlaylist>& out,
                        std::wstring& err) {
    HttpInfo hi;
    std::string js = YmRequest(token, false, L"/users/" + Wide(uid) + L"/playlists/list", "", nullptr, &hi);
    Jv r = JsonParse(js);
    const Jv& res = r["result"];
    if (res.t != Jv::Arr) {
        err = YmErr(hi, r);
        return false;
    }
    YmPlaylist liked;
    liked.kind = "liked";
    liked.title = L"Мне нравится";
    out.push_back(liked);
    for (size_t i = 0; i < res.size(); i++) {
        const Jv& p = res.at(i);
        YmPlaylist pl;
        pl.kind = p["kind"].str();
        pl.title = Wide(p["title"].str());
        pl.count = (int)p["trackCount"].num();
        {   // обложка плейлиста: одиночная картинка, иначе первая из «мозаики», иначе ogImage
            const Jv& cv = p["cover"];
            pl.cover = cv["uri"].str();
            if (pl.cover.empty()) {
                const Jv& mo = cv["itemsUri"];
                if (mo.t == Jv::Arr && mo.size() > 0) pl.cover = mo.at(0).str();
            }
            if (pl.cover.empty()) pl.cover = p["ogImage"].str();
        }
        if (!pl.kind.empty()) out.push_back(pl);
    }
    YmPlaylist daily;  // «Плейлист дня» — второй строкой, под «Мне нравится»; если его нет, просто пропускаем
    if (YmDailyPlaylist(token, daily)) out.insert(out.begin() + (out.empty() ? 0 : 1), daily);
    return true;
}

static bool YmPlaylistTracks(const std::string& token, const std::string& uid, const std::string& kind,
                             std::vector<YmTrack>& out, std::wstring& err) {
    HttpInfo hi;
    std::wstring path = kind == "liked" ? L"/users/" + Wide(uid) + L"/likes/tracks"
                                        : L"/users/" + Wide(uid) + L"/playlists/" + Wide(kind);
    std::string js = YmRequest(token, false, path, "", nullptr, &hi);
    Jv r = JsonParse(js);
    const Jv& items = kind == "liked" ? r["result"]["library"]["tracks"] : r["result"]["tracks"];
    if (items.t != Jv::Arr) {
        err = YmErr(hi, r);
        return false;
    }
    const size_t n = std::min(items.size(), kMaxPlTracks);
    std::vector<YmTrack> full(n);
    std::vector<char> have(n, 0);
    std::vector<std::string> need;     // «трек:альбом» для запроса недостающих описаний
    std::map<std::string, size_t> pos;  // id трека -> позиция в списке
    for (size_t i = 0; i < n; i++) {
        const Jv& it = items.at(i);
        const Jv& tr = it["track"];
        if (tr.t == Jv::Obj && !tr["title"].str().empty()) {
            full[i] = YmParseTrack(tr);
            have[i] = 1;
        } else {
            std::string id = it["id"].str(), alb = it["albumId"].str();
            if (id.empty()) continue;
            pos[id] = i;
            need.push_back(alb.empty() ? id : id + ":" + alb);
        }
    }
    for (size_t k = 0; k < need.size(); k += 100) {
        std::string body = "with-positions=True";
        for (size_t m = k; m < std::min(need.size(), k + 100); m++) body += "&track-ids=" + UrlEncA(need[m]);
        HttpInfo h2;
        std::string js2 = YmRequest(token, true, L"/tracks", body, L"application/x-www-form-urlencoded", &h2);
        Jv r2 = JsonParse(js2);
        const Jv& arr = r2["result"];
        for (size_t m = 0; m < arr.size(); m++) {
            YmTrack t = YmParseTrack(arr.at(m));
            auto f = pos.find(t.id);
            if (f != pos.end()) {
                full[f->second] = t;
                have[f->second] = 1;
            }
        }
    }
    for (size_t i = 0; i < n; i++)
        if (have[i] && full[i].available && !full[i].id.empty()) out.push_back(full[i]);
    return true;
}

// прямая ссылка на MP3 максимального битрейта; вызывать в момент запуска — ссылки быстро протухают
static bool YmDirectLink(const std::string& token, const std::string& id, std::string& url, std::wstring& err) {
    HttpInfo hi;
    std::string js = YmRequest(token, false, L"/tracks/" + Wide(id) + L"/download-info", "", nullptr, &hi);
    Jv r = JsonParse(js);
    const Jv& list = r["result"];
    std::string best;
    long long bestKbps = -1;
    for (size_t i = 0; i < list.size(); i++) {
        const Jv& e = list.at(i);
        if (e["codec"].str() != "mp3") continue;
        long long kb = e["bitrateInKbps"].num();
        if (kb > bestKbps) {
            bestKbps = kb;
            best = e["downloadInfoUrl"].str();
        }
    }
    if (best.empty()) {
        err = list.t == Jv::Arr ? std::wstring(L"для трека нет MP3-потока") : YmErr(hi, r);
        return false;
    }
    std::string u = best;
    size_t p = u.find("://");
    if (p != std::string::npos) u = u.substr(p + 3);
    size_t sl = u.find('/');
    if (sl == std::string::npos) {
        err = L"странная ссылка на поток";
        return false;
    }
    const std::wstring host = Wide(u.substr(0, sl));
    const std::wstring path = Wide(u.substr(sl)) + L"&format=json";
    HttpInfo h2;
    std::string js2 = HttpGet(host.c_str(), path, kYmClientHdr, nullptr, &h2);
    Jv d = JsonParse(js2);
    const std::string h = d["host"].str(), pth = d["path"].str(), ts = d["ts"].str(), s = d["s"].str();
    if (h.empty() || pth.size() < 2 || ts.empty() || s.empty()) {
        err = h2.status ? L"не удалось получить ссылку (HTTP " + std::to_wstring(h2.status) + L")" : NetErrText(h2.err);
        return false;
    }
    const std::string sign = Md5Hex("XGRlBW9FXlekgbPrRHuSiA" + pth.substr(1) + s);
    if (sign.empty()) {
        err = L"не удалось посчитать подпись";
        return false;
    }
    url = "https://" + h + "/get-mp3/" + sign + "/" + ts + pth;
    return true;
}

static bool YmCover(const std::string& coverUri, std::string& bytes, const char* size = "400x400") {
    if (coverUri.empty()) return false;
    std::string u = coverUri;
    size_t p = u.find("%%");
    if (p != std::string::npos) u.replace(p, 2, size);
    size_t sc = u.find("://");
    if (sc != std::string::npos) u = u.substr(sc + 3);
    size_t sl = u.find('/');
    if (sl == std::string::npos) return false;
    const std::wstring host = Wide(u.substr(0, sl));
    bytes = HttpGet(host.c_str(), Wide(u.substr(sl)));
    return !bytes.empty();
}

static std::string YmNowStamp() {
    SYSTEMTIME st;
    GetLocalTime(&st);
    char ts[64];
    snprintf(ts, sizeof(ts), "%04d-%02d-%02dT%02d:%02d:%02d.%03d000", (int)st.wYear, (int)st.wMonth, (int)st.wDay,
             (int)st.wHour, (int)st.wMinute, (int)st.wSecond, (int)st.wMilliseconds);
    return ts;
}

static std::string JsonQ(const std::string& s) {
    std::string o = "\"";
    for (char c : s) {
        if ((unsigned char)c < 0x20) continue;
        if (c == '"' || c == '\\') o += '\\';
        o += c;
    }
    o += '"';
    return o;
}

// настройки «Моей волны»: 0 — занятие, 1 — характер, 2 — настроение, 3 — язык; значения по умолчанию = «любое»
static const char* const kWaveDefault[4] = {"any", "default", "all", "any"};

// ответ rotor/session/new и rotor/session/{id}/tracks: радио-сессия, пачка (batchId) и треки
static bool YmWaveSessionResult(const std::string& js, const HttpInfo& hi, std::vector<YmTrack>& out,
                                std::string& batch, std::string& session, std::wstring& err) {
    Jv r = JsonParse(js);
    const Jv& res = r["result"];
    const Jv& seq = res["sequence"];
    if (hi.status != 200 || seq.t != Jv::Arr) {
        err = YmErr(hi, r);
        return false;
    }
    const std::string sid = res["radioSessionId"].str();
    if (!sid.empty()) session = sid;
    batch = res["batchId"].str();
    const size_t before = out.size();
    for (size_t i = 0; i < seq.size(); i++) {
        YmTrack t = YmParseTrack(seq.at(i)["track"]);
        if (!t.id.empty()) out.push_back(t);
    }
    if (out.size() == before) {
        err = L"пустой список треков";
        return false;
    }
    return true;
}

// станция: «Моя волна» или волна по треку (seed вида «track:ID»)
static std::string WaveStation(const std::string& seed) {
    return seed.empty() ? std::string("user:onyourwave") : seed;
}

// старый интерфейс станций (Яндекс его отключает) — оставлен как запасной путь
static bool YmWaveTracksLegacy(const std::string& token, const std::string& lastId, std::vector<YmTrack>& out,
                               std::string& batch, std::wstring& err, const std::string& seed = std::string()) {
    HttpInfo hi;
    std::wstring path = L"/rotor/station/" + Wide(WaveStation(seed)) + L"/tracks?settings2=true";  // как в yandex-music: settings2=True
    if (!lastId.empty()) path += L"&queue=" + UrlEnc(lastId);
    std::string js = YmRequest(token, false, path, "", nullptr, &hi);
    Jv r = JsonParse(js);
    const Jv& seq = r["result"]["sequence"];
    if (seq.t != Jv::Arr) {
        err = YmErr(hi, r);
        return false;
    }
    batch = r["result"]["batchId"].str();
    for (size_t i = 0; i < seq.size(); i++) {
        YmTrack t = YmParseTrack(seq.at(i)["track"]);
        if (!t.id.empty()) out.push_back(t);
    }
    return true;
}

// обратная связь «Моей волне»: radioStarted / trackStarted / trackFinished / skip
// session пусто — старый интерфейс станций, иначе радио-сессия
static void YmWaveFeedback(const std::string& token, const std::string& session, const std::string& type,
                           const std::string& fullId, double secs, const std::string& batch, bool radioStarted,
                           const std::string& seed = std::string()) {
    std::string ev = "{\"type\":" + JsonQ(type) + ",\"timestamp\":" + JsonQ(YmNowStamp());
    if (session.empty() && radioStarted) {
        ev += ",\"from\":\"user-onyourwave\"";
    } else if (!radioStarted) {
        ev += ",\"trackId\":" + JsonQ(fullId);
        if (secs >= 0) {
            char sb[32];
            snprintf(sb, sizeof(sb), "%.2f", secs);
            ev += std::string(",\"totalPlayedSeconds\":") + sb;
        }
    }
    ev += "}";
    if (session.empty()) {
        std::wstring path = L"/rotor/station/" + Wide(WaveStation(seed)) + L"/feedback";
        if (!batch.empty()) path += L"?batch-id=" + UrlEnc(batch);
        YmRequest(token, true, path, ev, L"application/json", nullptr);
    } else {
        const std::string body = "{\"event\":" + ev + ",\"batchId\":" + JsonQ(batch) + "}";
        YmRequest(token, true, L"/rotor/session/" + Wide(session) + L"/feedback", body, L"application/json", nullptr);
    }
}

static bool YmLikedIds(const std::string& token, const std::string& uid, std::vector<std::string>& ids,
                       std::wstring& err) {
    HttpInfo hi;
    std::string js = YmRequest(token, false, L"/users/" + Wide(uid) + L"/likes/tracks", "", nullptr, &hi);
    Jv r = JsonParse(js);
    const Jv& items = r["result"]["library"]["tracks"];
    if (items.t != Jv::Arr) {
        err = YmErr(hi, r);
        return false;
    }
    for (size_t i = 0; i < items.size(); i++) {
        std::string id = items.at(i)["id"].str();
        if (!id.empty()) ids.push_back(id);
    }
    return true;
}

static bool YmSetLike(const std::string& token, const std::string& uid, const std::string& id, bool add,
                      std::wstring& err) {
    HttpInfo hi;
    std::wstring path = L"/users/" + Wide(uid) + (add ? L"/likes/tracks/add-multiple" : L"/likes/tracks/remove");
    std::string js = YmRequest(token, true, path, "track-ids=" + UrlEncA(id), L"application/x-www-form-urlencoded", &hi);
    if (hi.status >= 200 && hi.status < 300) return true;
    err = YmErr(hi, JsonParse(js));
    return false;
}

// ---- фоновые рабочие потоки: очередь заданий, результат уходит в окно (WM_APP+6)
enum { YJ_SEARCH = 1, YJ_PLAYLISTS, YJ_PL_TRACKS, YJ_PLAY, YJ_WAVE, YJ_FEEDBACK, YJ_LIKE, YJ_LIKED, YJ_COVER, YJ_THUMB, YJ_ART_TRACKS, YJ_ALBUM };

struct YmJob {
    int kind = 0;
    long long gen = 0;
    std::string token, uid, s1, s2;
    std::string session;   // радио-сессия «Моей волны» (пусто — создать новую)
    std::string seed;      // пусто — «Моя волна»; «track:ID» — волна по треку
    std::string wcfg[4];   // настройки волны: занятие, характер, настроение, язык
    YmTrack track;
    double secs = -1;
    bool flag = false;
};
struct YmRes {
    int kind = 0;
    long long gen = 0;
    bool ok = false;
    std::wstring err;
    std::string uid, url, s1, bytes, session;
    std::vector<YmTrack> tracks;
    std::vector<YmPlaylist> pls;
    std::vector<YmArtist> artists;
    std::vector<std::string> ids;
    YmTrack track;
    double secs = 0;
};

// «Моя волна»: радио-сессия с сидами настроек (rotor/session/*); если сессии недоступны — старый интерфейс станций.
// j.s1 — последний сыгранный трек, j.session — текущая сессия, j.wcfg — настройки, j.flag — волна только что запущена
static bool YmWaveFetch(const YmJob& j, std::vector<YmTrack>& out, std::string& batch, std::string& session,
                        std::wstring& err) {
    auto cfg = [&](int k) { return j.wcfg[k].empty() ? std::string(kWaveDefault[k]) : j.wcfg[k]; };
    session = j.session;
    HttpInfo hi;
    if (!session.empty()) {  // продолжаем сессию: просим следующую пачку
        const std::string body = j.s1.empty() ? std::string("{}") : "{\"queue\":[" + JsonQ(j.s1) + "]}";
        std::string js = YmRequest(j.token, true, L"/rotor/session/" + Wide(session) + L"/tracks", body,
                                   L"application/json", &hi);
        if (YmWaveSessionResult(js, hi, out, batch, session, err)) return true;
        out.clear();
        session.clear();  // сессия протухла — создаём заново
    }

    // новая сессия: занятие — отдельная станция (activity:…), остальные настройки — сиды «settingXxx:значение»
    std::vector<std::string> st;
    if (cfg(1) != "default") st.push_back("settingDiversity:" + cfg(1));
    if (cfg(2) != "all") st.push_back("settingMoodEnergy:" + cfg(2));
    if (cfg(3) != "any") st.push_back("settingLanguage:" + cfg(3));
    const std::string head = !j.seed.empty() ? j.seed
                             : cfg(0) != "any" ? "activity:" + cfg(0) : std::string("user:onyourwave");
    std::vector<std::vector<std::string>> tries;
    {
        std::vector<std::string> a{head};
        a.insert(a.end(), st.begin(), st.end());
        tries.push_back(a);
        if (!st.empty()) tries.push_back(std::vector<std::string>{head});  // сервер не принял настройки — хотя бы станция
    }
    for (const auto& seeds : tries) {
        std::string body = "{\"seeds\":[";
        for (size_t i = 0; i < seeds.size(); i++) body += (i ? "," : "") + JsonQ(seeds[i]);
        body += "],\"includeTracksInResponse\":true}";
        HttpInfo h2;
        std::string js = YmRequest(j.token, true, L"/rotor/session/new", body, L"application/json", &h2);
        std::vector<YmTrack> got;
        std::string b2, s2;
        std::wstring e2;
        if (YmWaveSessionResult(js, h2, got, b2, s2, e2) && !s2.empty()) {
            out = got;
            batch = b2;
            session = s2;
            YmWaveFeedback(j.token, session, "radioStarted", "", -1, batch, true);
            return true;
        }
        err = e2;
    }

    // запасной путь: станции (настройки передаём через settings2)
    session.clear();
    if (cfg(1) != "default" || cfg(2) != "all" || cfg(3) != "any") {
        const std::string body = "{\"moodEnergy\":" + JsonQ(cfg(2)) + ",\"diversity\":" + JsonQ(cfg(1)) +
                                 ",\"language\":" + JsonQ(cfg(3)) + ",\"type\":\"rotor\"}";
        YmRequest(j.token, true, L"/rotor/station/" + Wide(WaveStation(j.seed)) + L"/settings2", body,
                  L"application/json", nullptr);
    }
    if (j.flag) YmWaveFeedback(j.token, "", "radioStarted", "", -1, "", true, j.seed);
    std::wstring e3;
    if (YmWaveTracksLegacy(j.token, j.s1, out, batch, e3, j.seed)) return true;
    if (err.empty()) err = e3;
    return false;
}

static CRITICAL_SECTION g_ymCs;
static bool g_ymCsInit = false;
static std::deque<YmJob*> g_ymQ;
static HANDLE g_ymSem = nullptr;
static HANDLE g_ymThreads[3] = {};
static volatile bool g_ymStop = false;

static void YmEnqueue(YmJob* j) {
    if (!g_ymSem || !g_ymCsInit || g_ymStop) {
        delete j;
        return;
    }
    EnterCriticalSection(&g_ymCs);
    g_ymQ.push_back(j);
    LeaveCriticalSection(&g_ymCs);
    ReleaseSemaphore(g_ymSem, 1, nullptr);
}

static void YmRunJob(YmJob* j) {
    YmRes* r = new YmRes();
    r->kind = j->kind;
    r->gen = j->gen;
    r->track = j->track;
    r->s1 = j->s1;
    r->secs = j->secs;
    try {
        std::string uid = j->uid;
        auto needUid = [&]() -> bool {
            if (uid.empty()) uid = YmUserId(j->token, &r->err);
            r->uid = uid;
            return !uid.empty();
        };
        switch (j->kind) {
            case YJ_SEARCH: {
                const bool okT = YmSearch(j->token, j->s1, r->tracks, r->err);
                const bool okA = YmSearchArtists(j->token, j->s1, r->artists);
                r->ok = okT || (okA && !r->artists.empty());
                break;
            }
            case YJ_ART_TRACKS:
                r->ok = YmArtistTracks(j->token, j->s1, r->tracks, r->err);
                break;
            case YJ_ALBUM:
                r->ok = YmAlbumTracks(j->token, j->s1, r->tracks, r->err);
                break;
            case YJ_PLAYLISTS:
                if (needUid()) r->ok = YmPlaylists(j->token, uid, r->pls, r->err);
                break;
            case YJ_PL_TRACKS:  // s2 — владелец плейлиста, если он не наш («Плейлист дня»)
                if (!j->s2.empty()) r->ok = YmPlaylistTracks(j->token, j->s2, j->s1, r->tracks, r->err);
                else if (needUid()) r->ok = YmPlaylistTracks(j->token, uid, j->s1, r->tracks, r->err);
                break;
            case YJ_PLAY:
                r->ok = YmDirectLink(j->token, j->track.id, r->url, r->err);
                break;
            case YJ_WAVE:
                r->s1.clear();
                r->ok = YmWaveFetch(*j, r->tracks, r->s1, r->session, r->err);
                break;
            case YJ_FEEDBACK:
                YmWaveFeedback(j->token, j->session, j->s1, YmFullId(j->track), j->secs, j->s2, false, j->seed);
                delete r;
                r = nullptr;
                break;
            case YJ_LIKE:
                if (needUid()) r->ok = YmSetLike(j->token, uid, j->track.id, j->s1 == "add", r->err);
                break;
            case YJ_LIKED:
                if (needUid()) r->ok = YmLikedIds(j->token, uid, r->ids, r->err);
                break;
            case YJ_COVER:
                r->ok = YmCover(j->track.cover, r->bytes);
                break;
            case YJ_THUMB:  // миниатюра для строки списка; r->s1 — uri обложки (ключ кэша)
                r->ok = YmCover(j->s1, r->bytes, "100x100");
                break;
        }
    } catch (...) {
        if (r) {
            r->ok = false;
            r->err = L"внутренняя ошибка";
        }
    }
    delete j;
    if (r && (!g_hwnd || !PostMessageW(g_hwnd, WM_APP + 6, 0, (LPARAM)r))) delete r;
}

static DWORD WINAPI YmWorker(LPVOID) {
    for (;;) {
        WaitForSingleObject(g_ymSem, INFINITE);
        if (g_ymStop) break;
        YmJob* j = nullptr;
        EnterCriticalSection(&g_ymCs);
        if (!g_ymQ.empty()) {
            j = g_ymQ.front();
            g_ymQ.pop_front();
        }
        LeaveCriticalSection(&g_ymCs);
        if (j) YmRunJob(j);
    }
    return 0;
}

static void YmStart() {
    if (g_ymSem) return;
    InitializeCriticalSection(&g_ymCs);
    g_ymCsInit = true;
    g_ymStop = false;
    g_ymSem = CreateSemaphoreW(nullptr, 0, 1024, nullptr);
    if (!g_ymSem) return;
    for (int i = 0; i < 3; i++) g_ymThreads[i] = CreateThread(nullptr, 0, YmWorker, nullptr, 0, nullptr);
}

static void YmStop() {
    if (!g_ymSem) return;
    g_ymStop = true;
    ReleaseSemaphore(g_ymSem, 3, nullptr);
    for (int i = 0; i < 3; i++) {
        if (!g_ymThreads[i]) continue;
        WaitForSingleObject(g_ymThreads[i], 8000);
        CloseHandle(g_ymThreads[i]);
        g_ymThreads[i] = nullptr;
    }
    CloseHandle(g_ymSem);
    g_ymSem = nullptr;
    for (YmJob* j : g_ymQ) delete j;
    g_ymQ.clear();
    DeleteCriticalSection(&g_ymCs);
    g_ymCsInit = false;
}

static YmJob* NewJob(int kind);  // ниже, после состояния

// ---- состояние встроенного плеера (всё — в потоке окна)
enum { PM_IDLE = 0, PM_SINGLE, PM_LIST, PM_WAVE };
static const int kMaxSkips = 10;  // сколько недоступных треков подряд пропускаем

static int g_pmode = PM_IDLE;
static std::vector<YmTrack> g_q;  // очередь воспроизведения (одиночный трек / плейлист)
static int g_qi = -1;
static std::string g_uid;
static std::vector<std::string> g_liked;
static bool g_likedLoaded = false;

static std::vector<YmTrack> g_waveBuf;
static std::vector<YmTrack> g_waveHist;  // уже сыгранные треки «Моей волны» (для кнопки «назад»)
static const size_t kWaveHistMax = 50;
static std::string g_waveBatch, g_waveLast, g_waveSession;  // пачка, последний трек, радио-сессия «Моей волны»
static bool g_waveBusy = false, g_waveWait = false;
static long long g_waveGen = 0;
static std::string g_waveSeed;       // пусто — обычная «Моя волна»; «track:ID» — волна по треку
static std::wstring g_waveSeedName;  // название трека, по которому запущена волна
static std::wstring g_srcLabel;      // откуда играет очередь («Плейлист: …», «Альбом: …»); пусто — не показываем

static long long g_playGen = 0;     // поколение запроса запуска (устаревшие ответы отбрасываем)
static YmTrack g_cur;
static bool g_haveCur = false;
static bool g_wantPlay = false;     // что хочет пользователь: играть или пауза
static bool g_opening = false;      // источник ещё не открылся
static double g_startAt = 0;        // с какой секунды начать после открытия (перемотка/возобновление)
static int g_skipRun = 0;
static int g_retry = 0;
static ULONGLONG g_pausedAt = 0;
static std::string g_coverBytes;
static std::wstring g_status;       // последняя ошибка — показывается вместо исполнителя, пока ничего не играет

static Pb::MediaPlayer g_mp{nullptr};
static winrt::event_token g_tokEnded{}, g_tokFailed{}, g_tokOpened{}, g_tokNext{}, g_tokPrev{};

// библиотека (панель поиска и плейлистов)
static bool g_libOpen = false;
static double g_libT = 0.0;         // анимация открытия 0..1
static int g_libMode = 0;           // 0 — плейлисты, 1 — результаты поиска, 2 — треки плейлиста/исполнителя, 3 — настройки «Моей волны»
static std::vector<YmPlaylist> g_pls;
static std::vector<YmTrack> g_found, g_plTracks;
static std::vector<YmArtist> g_foundArtists;  // исполнители из поиска (показываются над треками)
static int g_libBack = 0;                     // откуда открыт список треков (режим 2): 0 — плейлисты, 1 — поиск
static double g_libBackScroll = 0.0;
static std::wstring g_libQuery, g_plTitle, g_errPl, g_errSearch, g_errTracks;
static std::string g_plKind;
static bool g_plLoaded = false, g_busyPl = false, g_busySearch = false, g_busyTracks = false;
static long long g_genPl = 0, g_genSearch = 0, g_genTracks = 0;
static double g_libScroll = 0.0;    // px
static int g_libHover = -1;

// ---- настройки «Моей волны»: под занятие, по характеру, под настроение, по языку
struct WaveOpt {
    const char* id;
    const wchar_t* label;
};
static const WaveOpt kWaveAct[] = {{"any", L"Любое"},           {"wake-up", L"Просыпаюсь"}, {"road-trip", L"В дороге"},
                                   {"work-background", L"Работаю"}, {"workout", L"Тренируюсь"}, {"fall-asleep", L"Засыпаю"}};
static const WaveOpt kWaveDiv[] = {{"default", L"Любое"}, {"favorite", L"Любимое"}, {"discover", L"Незнакомое"},
                                   {"popular", L"Популярное"}};
static const WaveOpt kWaveMood[] = {{"all", L"Любое"}, {"active", L"Бодрое"}, {"fun", L"Весёлое"},
                                    {"calm", L"Спокойное"}, {"sad", L"Грустное"}};
static const WaveOpt kWaveLang[] = {{"any", L"Любой"}, {"russian", L"Русский"}, {"not-russian", L"Иностранный"},
                                    {"without-words", L"Без слов"}};
struct WaveGroup {
    const wchar_t* title;
    const WaveOpt* opts;
    int n;
};
static const WaveGroup kWaveGroups[4] = {{L"Под занятие", kWaveAct, (int)ARRAYSIZE(kWaveAct)},
                                         {L"По характеру", kWaveDiv, (int)ARRAYSIZE(kWaveDiv)},
                                         {L"Под настроение", kWaveMood, (int)ARRAYSIZE(kWaveMood)},
                                         {L"По языку", kWaveLang, (int)ARRAYSIZE(kWaveLang)}};
static std::string g_wCfg[4] = {"any", "default", "all", "any"};  // текущий выбор (id опций)
static bool g_wCfgLoaded = false;

static void WaveCfgLoad() {
    if (g_wCfgLoaded) return;
    g_wCfgLoaded = true;
    wchar_t buf[160] = {};
    Wh_GetStringValue(L"waveCfg", buf, ARRAYSIZE(buf));
    const std::string s = Utf8(buf);
    size_t a = 0;
    for (int k = 0; k < 4 && a <= s.size(); k++) {
        size_t b = s.find('|', a);
        if (b == std::string::npos) b = s.size();
        const std::string v = s.substr(a, b - a);
        a = b + 1;
        for (int i = 0; i < kWaveGroups[k].n; i++)
            if (v == kWaveGroups[k].opts[i].id) {
                g_wCfg[k] = v;
                break;
            }
    }
}

static void WaveCfgSave() {
    const std::string s = g_wCfg[0] + "|" + g_wCfg[1] + "|" + g_wCfg[2] + "|" + g_wCfg[3];
    Wh_SetStringValue(L"waveCfg", Wide(s).c_str());
}

static YmJob* NewJob(int kind) {
    YmJob* j = new YmJob();
    j->kind = kind;
    j->token = g_set.ymToken;
    j->uid = g_uid;
    for (int k = 0; k < 4; k++) j->wcfg[k] = g_wCfg[k];
    j->seed = g_waveSeed;
    return j;
}

static bool IsLiked(const std::string& id) {
    return std::find(g_liked.begin(), g_liked.end(), id) != g_liked.end();
}

static bool OwnAdvance(bool record = true);
static void OwnTrackFailed(const std::wstring& why);
static void OwnWaveFetch(bool first = false);

static void OwnApplyVolume() {
    if (!g_mp) return;
    try {
        g_mp.Volume(g_muted ? 0.0 : (double)std::clamp(g_vol, 0.f, 1.f));
    } catch (...) {
    }
}

static void OwnSaveVolume() {
    if (OwnMode()) Wh_SetIntValue(L"volume", (int)(std::clamp(g_vol, 0.f, 1.f) * 100.f + 0.5f));
}

// «пусто»: ничего не играет
static void OwnIdle() {
    g_playGen++;
    try {
        if (g_mp) {
            g_mp.Pause();
            g_mp.Source(nullptr);
        }
    } catch (...) {
    }
    g_haveCur = false;
    g_wantPlay = false;
    g_opening = false;
    g_pmode = PM_IDLE;
    g_q.clear();
    g_qi = -1;
    g_waveBuf.clear();
    g_waveHist.clear();
    g_waveSession.clear();
    g_waveWait = false;
    g_waveGen++;
    g_title = L"Яндекс Музыка";
    g_artist = g_status.empty() ? std::wstring(L"Нажмите ▶ — запустится «Моя волна»") : g_status;
    g_trackKey.clear();
    g_durTicks = 0;
    g_posBase = 0;
    g_posBaseTick = GetTickCount64();
    g_playing = false;
    g_cover.clear();
    g_coverPx = 0;
    g_coverBytes.clear();
    g_coverOld.clear();
    g_palTgt = DefaultPalette();
    g_textT = 0.0;
    if (g_hwnd) StartAnim(g_hwnd);
}

static void OwnShutdownPlayer() {
    if (!g_mp) return;
    try {
        g_mp.MediaEnded(g_tokEnded);
        g_mp.MediaFailed(g_tokFailed);
        g_mp.MediaOpened(g_tokOpened);
        auto cm = g_mp.CommandManager();
        cm.NextReceived(g_tokNext);
        cm.PreviousReceived(g_tokPrev);
        g_mp.Pause();
        g_mp.Source(nullptr);
        g_mp.Close();
    } catch (...) {
    }
    g_mp = nullptr;
}

static void OwnEnsurePlayer() {
    if (!OwnMode()) {
        OwnShutdownPlayer();
        return;
    }
    if (g_mp) return;
    try {
        g_mp = Pb::MediaPlayer();
        g_mp.AutoPlay(false);  // Play() вызываем сами
        g_tokEnded = g_mp.MediaEnded([](Pb::MediaPlayer const&, winrt::Windows::Foundation::IInspectable const&) {
            if (g_hwnd) PostMessageW(g_hwnd, WM_APP + 8, 1, 0);
        });
        g_tokFailed = g_mp.MediaFailed([](Pb::MediaPlayer const&, Pb::MediaPlayerFailedEventArgs const&) {
            if (g_hwnd) PostMessageW(g_hwnd, WM_APP + 8, 2, 0);
        });
        g_tokOpened = g_mp.MediaOpened([](Pb::MediaPlayer const&, winrt::Windows::Foundation::IInspectable const&) {
            if (g_hwnd) PostMessageW(g_hwnd, WM_APP + 8, 3, 0);
        });
        // мультимедийные клавиши и системный оверлей громкости: «следующий»/«предыдущий» обрабатываем сами
        auto cm = g_mp.CommandManager();
        cm.NextBehavior().EnablingRule(Pb::MediaCommandEnablingRule::Always);
        cm.PreviousBehavior().EnablingRule(Pb::MediaCommandEnablingRule::Always);
        g_tokNext = cm.NextReceived(
            [](Pb::MediaPlaybackCommandManager const&, Pb::MediaPlaybackCommandManagerNextReceivedEventArgs const& a) {
                a.Handled(true);
                if (g_hwnd) PostMessageW(g_hwnd, WM_APP + 8, 4, 0);
            });
        g_tokPrev = cm.PreviousReceived(
            [](Pb::MediaPlaybackCommandManager const&,
               Pb::MediaPlaybackCommandManagerPreviousReceivedEventArgs const& a) {
                a.Handled(true);
                if (g_hwnd) PostMessageW(g_hwnd, WM_APP + 8, 5, 0);
            });
        WaveCfgLoad();
        g_vol = (float)std::clamp((int)Wh_GetIntValue(L"volume", 100), 0, 100) / 100.f;
        OwnApplyVolume();
    } catch (...) {
        Wh_Log(L"MediaPlayer недоступен");
        g_mp = nullptr;
    }
}

// декодирует обложку из байтов (JPEG/PNG) и подменяет текущую; старая плавно перетекает в новую
static bool OwnDecodeCover(bool crossfade) {
    const int cs = CoverSize();
    if (cs <= 0 || g_coverBytes.empty()) return false;
    try {
        winrt::Windows::Storage::Streams::InMemoryRandomAccessStream ms;
        winrt::Windows::Storage::Streams::DataWriter dw(ms);
        dw.WriteBytes(winrt::array_view<uint8_t const>((uint8_t const*)g_coverBytes.data(),
                                                       (uint32_t)g_coverBytes.size()));
        dw.StoreAsync().get();
        dw.DetachStream();
        ms.Seek(0);
        std::vector<DWORD> prev = g_cover;
        const int prevPx = g_coverPx;
        if (!LoadCoverStream(ms, cs)) return false;
        if (crossfade && !prev.empty()) {
            g_coverOld.swap(prev);
            g_coverOldPx = prevPx;
            g_coverFade = 0.0;
            if (g_hwnd) StartAnim(g_hwnd);
        }
        return true;
    } catch (...) {
        return false;
    }
}

// новый текущий трек: заголовок, длительность, сброс текста песни, запрос обложки
static void OwnSetCurrent(const YmTrack& t) {
    g_cur = t;
    g_haveCur = true;
    g_retry = 0;
    g_title = t.title;
    g_artist = t.artists;
    g_durTicks = t.durMs * 10000LL;
    g_startTicks = 0;
    g_posBase = 0;
    g_posBaseTick = GetTickCount64();
    g_seekHold = 0;
    g_trackKey = g_title + L"\x1f" + g_artist;
    g_lastSeen = GetTickCount64();
    g_textT = 0.0;
    g_coverBytes.clear();
    if (t.cover.empty()) {
        g_cover.clear();
        g_coverPx = 0;
        g_palTgt = DefaultPalette();
    } else {
        YmJob* c = NewJob(YJ_COVER);
        c->track = t;
        YmEnqueue(c);
    }
    if (g_hwnd) StartAnim(g_hwnd);
}

static void OwnWaveFeedback(const char* type, double secs) {
    if (g_pmode != PM_WAVE || !g_haveCur) return;
    YmJob* j = NewJob(YJ_FEEDBACK);
    j->s1 = type;
    j->s2 = g_waveBatch;
    j->session = g_waveSession;
    j->track = g_cur;
    j->secs = secs;
    YmEnqueue(j);
}

// ссылка получена — отдаём её MediaPlayer
static void OwnStartStream(const std::string& url, double startSec) {
    auto src = MCore::MediaSource::CreateFromUri(winrt::Windows::Foundation::Uri(winrt::hstring(Wide(url))));
    Pb::MediaPlaybackItem item(src);
    try {  // сведения о треке для системного оверлея
        auto dp = item.GetDisplayProperties();
        dp.Type(winrt::Windows::Media::MediaPlaybackType::Music);
        dp.MusicProperties().Title(winrt::hstring(g_cur.title));
        dp.MusicProperties().Artist(winrt::hstring(g_cur.artists));
        item.ApplyDisplayProperties(dp);
    } catch (...) {
    }
    g_startAt = startSec;
    g_opening = true;
    g_mp.Source(item);
    OwnApplyVolume();
    if (g_wantPlay) g_mp.Play();
}

// запустить трек (startSec > 0 — продолжить с этой секунды, метаданные не трогаем)
static void OwnPlayTrack(const YmTrack& t, double startSec = 0.0) {
    OwnEnsurePlayer();
    if (!g_mp) return;
    try {
        g_mp.Pause();
    } catch (...) {
    }
    g_status.clear();
    if (!(g_haveCur && g_cur.id == t.id && startSec > 0.0)) OwnSetCurrent(t);
    g_wantPlay = true;
    g_opening = true;
    g_playing = true;
    g_posFreeze = true;
    if (startSec > 0.0) {
        g_posBase = (long long)(startSec * 1e7);
        g_posBaseTick = GetTickCount64();
    }
    g_playGen++;
    YmJob* j = NewJob(YJ_PLAY);
    j->gen = g_playGen;
    j->track = t;
    j->secs = startSec;
    YmEnqueue(j);
    if (g_hwnd) StartAnim(g_hwnd);
}

static void OwnWaveFetch(bool first) {
    if (g_waveBusy) return;
    g_waveBusy = true;
    YmJob* j = NewJob(YJ_WAVE);
    j->gen = g_waveGen;
    j->s1 = g_waveLast;
    j->session = g_waveSession;
    j->flag = first;
    YmEnqueue(j);
}

// следующий трек по текущему режиму; false — играть больше нечего
static bool OwnAdvance(bool record) {
    if (g_pmode == PM_LIST) {
        for (;;) {
            g_qi++;
            if (g_qi >= (int)g_q.size()) {
                OwnIdle();
                return false;
            }
            if (g_q[g_qi].available) {
                YmTrack t = g_q[g_qi];
                OwnPlayTrack(t);
                return true;
            }
        }
    }
    if (g_pmode == PM_WAVE) {
        if (g_waveBuf.empty()) {
            g_waveWait = true;
            OwnWaveFetch();
            return true;
        }
        YmTrack t = g_waveBuf.front();
        g_waveBuf.erase(g_waveBuf.begin());
        g_waveLast = YmFullId(t);
        if (record && g_haveCur && !g_cur.id.empty()) {  // запоминаем трек, с которого уходим
            g_waveHist.push_back(g_cur);
            if (g_waveHist.size() > kWaveHistMax) g_waveHist.erase(g_waveHist.begin());
        }
        OwnPlayTrack(t);
        if (g_waveBuf.size() < 2) OwnWaveFetch();  // подкачиваем следующую пачку заранее
        return true;
    }
    OwnIdle();
    return false;
}

static void OwnTrackFailed(const std::wstring& why) {
    Wh_Log(L"YM: %s", why.c_str());
    g_skipRun++;
    if (g_skipRun < kMaxSkips && (g_pmode == PM_LIST || g_pmode == PM_WAVE)) {
        OwnAdvance(false);  // трек не заиграл — в историю «назад» его не кладём
        return;
    }
    g_status = L"Не удалось воспроизвести: " + why;
    OwnIdle();
}

static void OwnStartWave(bool keepSeed = false) {
    OwnEnsurePlayer();
    if (!g_mp) return;
    if (!keepSeed) {  // обычная «Моя волна»
        g_waveSeed.clear();
        g_waveSeedName.clear();
    }
    try {
        g_mp.Pause();
    } catch (...) {
    }
    g_pmode = PM_WAVE;
    g_q.clear();
    g_qi = -1;
    g_waveBuf.clear();
    g_waveHist.clear();
    g_waveBatch.clear();
    g_waveLast.clear();
    g_waveSession.clear();  // новая радио-сессия — с текущими настройками волны
    g_waveGen++;
    g_waveBusy = false;
    g_waveWait = true;
    g_skipRun = 0;
    g_status.clear();
    g_playGen++;  // ответ на запуск предыдущего трека больше не нужен
    g_wantPlay = true;
    g_haveCur = false;
    g_title = g_waveSeed.empty() ? L"Моя волна" : L"Волна по треку";
    g_artist = L"Подбираю треки…";
    g_trackKey.clear();
    g_durTicks = 0;
    g_playing = true;
    OwnWaveFetch(true);
    if (g_hwnd) StartAnim(g_hwnd);
}

// выбор опции настройки «Моей волны»; если волна уже играет — перезапускаем её с новыми настройками
static void OwnSetWaveCfg(int group, const std::string& id) {
    if (group < 0 || group > 3 || g_wCfg[group] == id) return;
    g_wCfg[group] = id;
    WaveCfgSave();
    if (g_pmode == PM_WAVE) OwnStartWave(true);
}

// «Волна по треку»: радио-сессия с сидом «track:ID»
static void OwnStartWaveFromTrack(const YmTrack& t) {
    if (t.id.empty()) return;
    g_waveSeed = "track:" + t.id;
    g_waveSeedName = t.title;
    OwnStartWave(true);
}

// «Встряхнуть»: отдаём Яндексу «пропуск» текущего трека и просим совершенно новую радио-сессию (свежая подборка)
static void OwnShakeWave() {
    OwnEnsurePlayer();
    if (!g_mp) return;
    const bool wasWave = g_pmode == PM_WAVE;
    if (wasWave && g_haveCur) OwnWaveFeedback("skip", (double)CurPos() / 1e7);
    OwnStartWave(wasWave);  // волна по треку остаётся волной по треку; из других режимов — обычная «Моя волна»
}

static void OwnPlayQueue(const std::vector<YmTrack>& tracks, int idx, int mode) {
    if (idx < 0 || idx >= (int)tracks.size()) return;
    OwnEnsurePlayer();
    if (!g_mp) return;
    std::vector<YmTrack> copy = tracks;  // tracks может быть самим g_q
    g_pmode = mode;
    g_q = std::move(copy);
    g_qi = idx;
    g_waveBuf.clear();
    g_waveHist.clear();
    g_waveGen++;
    g_waveWait = false;
    g_skipRun = 0;
    YmTrack t = g_q[idx];
    OwnPlayTrack(t);
}

static void OwnSeekTicks(long long ticks) {
    if (!g_mp || !g_haveCur) return;
    ticks = std::max(0LL, ticks);
    if (g_durTicks > 0) ticks = std::min(ticks, std::max(0LL, g_durTicks - 10000000LL));
    g_posBase = ticks;
    g_posBaseTick = GetTickCount64();
    g_seekHold = g_posBaseTick + 1500;
    if (g_opening) {  // ещё открывается — применим позицию, когда откроется
        g_startAt = (double)ticks / 1e7;
        return;
    }
    try {
        g_mp.PlaybackSession().Position(winrt::Windows::Foundation::TimeSpan(ticks));
    } catch (...) {
    }
}

static void OwnSeek(double frac) {
    if (g_durTicks <= 0) return;
    OwnSeekTicks((long long)(std::clamp(frac, 0.0, 1.0) * (double)g_durTicks));
}

static void OwnToggle() {
    if (!g_mp) return;
    if (!g_haveCur) {
        if (g_pmode == PM_IDLE) OwnStartWave();  // ничего не играло — включаем «Мою волну»
        return;
    }
    try {
        if (g_wantPlay) {
            g_posBase = CurPos();
            g_posBaseTick = GetTickCount64();
            g_wantPlay = false;
            g_playing = false;
            g_pausedAt = GetTickCount64();
            g_mp.Pause();
        } else {
            g_wantPlay = true;
            if (GetTickCount64() - g_pausedAt > 60000 && !g_opening) {
                // долгая пауза: соединение/ссылка могли протухнуть — берём свежую и продолжаем с той же секунды
                YmTrack t = g_cur;
                OwnPlayTrack(t, std::max(0.5, (double)g_posBase / 1e7));
            } else {
                g_playing = true;
                g_posBaseTick = GetTickCount64();
                g_mp.Play();
            }
        }
    } catch (...) {
    }
    if (g_hwnd) StartAnim(g_hwnd);
}

static void OwnNext() {
    if (!g_mp) return;
    if (!g_haveCur) {
        if (g_pmode == PM_IDLE) OwnStartWave();
        return;
    }
    if (g_pmode == PM_WAVE) {
        OwnWaveFeedback("skip", (double)CurPos() / 1e7);
        OwnAdvance();
    } else if (g_pmode == PM_LIST) {
        OwnAdvance();
    }  // одиночный трек: следующего нет
}

static void OwnPrev() {
    if (!g_mp || !g_haveCur) return;
    if (CurPos() > 5LL * 10000000LL) {
        OwnSeekTicks(0);  // трек играет уже давно — в начало трека
        return;
    }
    if (g_pmode == PM_WAVE) {  // «Моя волна»: возвращаемся к предыдущему треку из истории
        if (g_waveHist.empty()) {
            OwnSeekTicks(0);
            return;
        }
        YmTrack t = g_waveHist.back();
        g_waveHist.pop_back();
        g_waveBuf.insert(g_waveBuf.begin(), g_cur);  // «следующий» снова вернёт трек, с которого ушли
        OwnPlayTrack(t);
        return;
    }
    if (g_pmode != PM_LIST || g_qi <= 0) {
        OwnSeekTicks(0);  // в начало трека
        return;
    }
    for (int i = g_qi - 1; i >= 0; i--) {
        if (g_q[i].available) {
            g_qi = i;
            YmTrack t = g_q[i];
            OwnPlayTrack(t);
            return;
        }
    }
    OwnSeekTicks(0);
}

static void OwnToggleLike() {
    if (!g_haveCur || g_cur.id.empty()) return;
    const bool add = !IsLiked(g_cur.id);
    if (add) g_liked.push_back(g_cur.id);
    else g_liked.erase(std::remove(g_liked.begin(), g_liked.end(), g_cur.id), g_liked.end());
    YmJob* j = NewJob(YJ_LIKE);
    j->track = g_cur;
    j->s1 = add ? "add" : "remove";
    YmEnqueue(j);
    if (g_hwnd) StartAnim(g_hwnd);
}

// события MediaPlayer (приходят сообщением WM_APP+8)
static void OwnOnEnded() {
    if (g_opening || !g_haveCur) return;
    OwnWaveFeedback("trackFinished", (double)g_cur.durMs / 1000.0);
    OwnAdvance();  // одиночный трек закончился — вернёмся в «пусто»
}

static void OwnOnFailed() {
    if (!g_haveCur) return;
    if (g_retry < 1) {  // ссылка могла протухнуть — один раз пробуем заново с того же места
        g_retry++;
        YmTrack t = g_cur;
        OwnPlayTrack(t, (double)CurPos() / 1e7);
        return;
    }
    OwnTrackFailed(L"ошибка потока");
}

static void OwnOnOpened() {
    g_opening = false;
    g_skipRun = 0;
    if (g_startAt > 0.5) {
        try {
            g_mp.PlaybackSession().Position(winrt::Windows::Foundation::TimeSpan((long long)(g_startAt * 1e7)));
        } catch (...) {
        }
        g_posBase = (long long)(g_startAt * 1e7);
        g_posBaseTick = GetTickCount64();
        g_seekHold = g_posBaseTick + 1500;
    }
    g_startAt = 0;
}

// вызывается из Poll(): синхронизируем позицию и состояние с MediaPlayer
static void OwnPoll() {
    try {
        OwnEnsurePlayer();
        if (!g_mp || !g_haveCur) return;
        const bool pl = g_wantPlay;
        if (pl != g_playing) {
            g_playing = pl;
            if (g_hwnd) StartAnim(g_hwnd);
        }
        auto ps = g_mp.PlaybackSession();
        const auto st = ps.PlaybackState();
        g_posFreeze = g_opening || st == Pb::MediaPlaybackState::Opening || st == Pb::MediaPlaybackState::Buffering;
        if (!g_opening && (st == Pb::MediaPlaybackState::Playing || st == Pb::MediaPlaybackState::Paused)) {
            long long pos = ps.Position().count();
            if (g_durTicks > 0) pos = std::clamp(pos, 0LL, g_durTicks);
            else pos = std::max(0LL, pos);
            if (GetTickCount64() >= g_seekHold && g_drag != 1) {
                g_posBase = pos;
                g_posBaseTick = GetTickCount64();
            }
        }
        const int cs = CoverSize();  // масштаб/размер панели поменялся — декодируем обложку заново
        if (cs > 0 && !g_cover.empty() && !g_coverBytes.empty() && g_coverPx != cs) OwnDecodeCover(false);
    } catch (...) {
    }
}

// сменили токен/режим в настройках
static void OwnReconfigure() {
    g_uid.clear();
    g_liked.clear();
    g_likedLoaded = false;
    g_plLoaded = false;
    g_pls.clear();
    g_found.clear();
    g_foundArtists.clear();
    g_plTracks.clear();
    g_busyPl = g_busySearch = g_busyTracks = false;
    g_genPl++;
    g_genSearch++;
    g_genTracks++;
    g_errPl.clear();
    g_errSearch.clear();
    g_errTracks.clear();
    g_libOpen = false;
    g_libMode = 0;
    g_libScroll = 0.0;
    g_status.clear();
    if (OwnMode()) {
        OwnEnsurePlayer();
        OwnIdle();
        YmEnqueue(NewJob(YJ_LIKED));
    } else {
        OwnIdle();
        OwnShutdownPlayer();
        g_playGen++;
        ClearState();  // дальше Poll() снова берёт данные из системной медиа-сессии
    }
}

// ---- библиотека: поиск и плейлисты
static void LibSearch(const std::wstring& q) {
    g_libQuery = q;
    g_libMode = 1;
    g_found.clear();
    g_foundArtists.clear();
    g_errSearch.clear();
    g_libScroll = 0.0;
    g_busySearch = true;
    YmJob* j = NewJob(YJ_SEARCH);
    j->gen = ++g_genSearch;
    j->s1 = Utf8(q);
    YmEnqueue(j);
}

static void LibLoadPlaylists(bool force) {
    if (g_busyPl || (g_plLoaded && !force)) return;
    g_busyPl = true;
    g_errPl.clear();
    YmJob* j = NewJob(YJ_PLAYLISTS);
    j->gen = ++g_genPl;
    YmEnqueue(j);
}

static void LibOpenPlaylist(int idx) {
    if (idx < 0 || idx >= (int)g_pls.size()) return;
    g_libMode = 2;
    g_libBack = 0;
    g_plTitle = g_pls[idx].title;
    g_plKind = g_pls[idx].kind;
    g_plTracks.clear();
    g_errTracks.clear();
    g_libScroll = 0.0;
    g_busyTracks = true;
    YmJob* j = NewJob(YJ_PL_TRACKS);
    j->gen = ++g_genTracks;
    j->s1 = g_plKind;
    j->s2 = g_pls[(size_t)idx].uid;
    YmEnqueue(j);
}

// профиль исполнителя: его треки показываем тем же списком, что и плейлист
// back — куда ведёт «назад»: 0 — к плейлистам, 1 — к результатам поиска
static void LibOpenArtistId(const std::string& id, const std::wstring& name, int back) {
    g_libBack = back;
    g_libMode = 2;
    g_plTitle = name;
    g_plKind = "artist:" + id;
    g_plTracks.clear();
    g_errTracks.clear();
    g_libScroll = 0.0;
    g_busyTracks = true;
    YmJob* j = NewJob(YJ_ART_TRACKS);
    j->gen = ++g_genTracks;
    j->s1 = id;
    YmEnqueue(j);
}

// альбом: его треки показываем тем же списком, что и плейлист
static void LibOpenAlbumId(const std::string& id, const std::wstring& title, int back) {
    g_libBack = back;
    g_libMode = 2;
    g_plTitle = title.empty() ? std::wstring(L"Альбом") : title;
    g_plKind = "album:" + id;
    g_plTracks.clear();
    g_errTracks.clear();
    g_libScroll = 0.0;
    g_busyTracks = true;
    YmJob* j = NewJob(YJ_ALBUM);
    j->gen = ++g_genTracks;
    j->s1 = id;
    YmEnqueue(j);
}

// откуда запускается очередь из открытого списка библиотеки
static std::wstring LibSourceLabel() {
    if (g_libMode == 1) return g_libQuery.empty() ? std::wstring(L"Поиск") : L"Поиск: " + g_libQuery;
    if (g_libMode != 2) return std::wstring();
    if (g_plKind.compare(0, 7, "artist:") == 0) return L"Исполнитель: " + g_plTitle;
    if (g_plKind.compare(0, 6, "album:") == 0) return L"Альбом: " + g_plTitle;
    if (g_plKind == "liked") return L"Мне нравится";
    return L"Плейлист: " + g_plTitle;
}

static void LibOpenArtist(int idx) {
    if (idx < 0 || idx >= (int)g_foundArtists.size()) return;
    const YmArtist a = g_foundArtists[(size_t)idx];
    g_libBackScroll = g_libScroll;
    LibOpenArtistId(a.id, a.name, 1);
}

static const std::vector<YmTrack>& LibTracks() {
    return g_libMode == 2 ? g_plTracks : g_found;
}

// в режиме поиска сверху идут исполнители, потом треки
static int LibArtistRows() { return g_libMode == 1 ? (int)g_foundArtists.size() : 0; }

static int LibRowCount() {
    if (g_libMode == 3) return 0;  // настройки волны — не список строк
    return g_libMode == 0 ? (int)g_pls.size() : LibArtistRows() + (int)LibTracks().size();
}

static const YmArtist* LibRowArtist(int i) {
    return (g_libMode == 1 && i >= 0 && i < (int)g_foundArtists.size()) ? &g_foundArtists[(size_t)i] : nullptr;
}

static const YmTrack* LibRowTrack(int i) {
    if (g_libMode == 0 || g_libMode == 3) return nullptr;
    const int k = i - LibArtistRows();
    const std::vector<YmTrack>& v = LibTracks();
    return (k >= 0 && k < (int)v.size()) ? &v[(size_t)k] : nullptr;
}

// ---- миниатюры обложек в строках списка (поиск, плейлисты): кэш по uri обложки
struct Thumb {
    std::vector<DWORD> px;
    int sz = 0;
    bool req = false;  // запрос уже отправлен
};
static std::map<std::string, Thumb> g_thumbs;
static int ThumbSize() { return std::max(16, S(32)); }

static bool DecodeThumb(const std::string& bytes, int size, std::vector<DWORD>& out) {
    namespace Img = winrt::Windows::Graphics::Imaging;
    if (bytes.empty() || size <= 0) return false;
    try {
        winrt::Windows::Storage::Streams::InMemoryRandomAccessStream ms;
        winrt::Windows::Storage::Streams::DataWriter dw(ms);
        dw.WriteBytes(winrt::array_view<uint8_t const>((uint8_t const*)bytes.data(), (uint32_t)bytes.size()));
        dw.StoreAsync().get();
        dw.DetachStream();
        ms.Seek(0);
        auto decoder = Img::BitmapDecoder::CreateAsync(ms).get();
        Img::BitmapTransform tr;
        tr.ScaledWidth(size);
        tr.ScaledHeight(size);
        tr.InterpolationMode(Img::BitmapInterpolationMode::Fant);
        auto sb = decoder
                      .GetSoftwareBitmapAsync(Img::BitmapPixelFormat::Bgra8, Img::BitmapAlphaMode::Premultiplied, tr,
                                              Img::ExifOrientationMode::IgnoreExifOrientation,
                                              Img::ColorManagementMode::DoNotColorManage)
                      .get();
        winrt::Windows::Storage::Streams::Buffer buf((uint32_t)size * size * 4);
        sb.CopyToBuffer(buf);
        buf.Length((uint32_t)size * size * 4);
        DWORD* d = (DWORD*)buf.data();
        out.assign(d, d + (size_t)size * size);
        return true;
    } catch (...) {
        return false;
    }
}

static const std::string& LibRowCover(int i) {
    static const std::string none;
    if (g_libMode == 0) return (i >= 0 && i < (int)g_pls.size()) ? g_pls[(size_t)i].cover : none;
    if (const YmArtist* a = LibRowArtist(i)) return a->cover;
    const YmTrack* t = LibRowTrack(i);
    return t ? t->cover : none;
}

// загруженная миниатюра строки или nullptr
static const Thumb* LibThumb(int i) {
    const std::string& c = LibRowCover(i);
    if (c.empty()) return nullptr;
    auto it = g_thumbs.find(c);
    return (it != g_thumbs.end() && !it->second.px.empty()) ? &it->second : nullptr;
}

// заказываем миниатюры для видимых строк (и чуть дальше); вызывается по таймеру, повторные запросы отсекаются
static void LibRequestThumbs() {
    if (!g_libOpen || !OwnMode()) return;
    const int n = LibRowCount();
    if (n <= 0) return;
    const int rowH = std::max(1, S(40));
    const int first = std::max(0, (int)(g_libScroll / (double)rowH) - 2);
    const int last = std::min(n, first + 18);
    if (g_thumbs.size() > 300) g_thumbs.clear();  // не копим бесконечно
    for (int i = first; i < last; i++) {
        const std::string& c = LibRowCover(i);
        if (c.empty()) continue;
        Thumb& th = g_thumbs[c];
        if (th.req) continue;
        th.req = true;
        YmJob* j = NewJob(YJ_THUMB);
        j->s1 = c;
        YmEnqueue(j);
    }
}

// что написать в списке вместо строк (загрузка/ошибка/пусто)
static std::wstring LibMessage() {
    if (g_libMode == 3) return L"";
    if (g_libMode == 0) {
        if (g_busyPl) return L"Загружаю плейлисты…";
        if (!g_errPl.empty()) return L"Не удалось загрузить плейлисты: " + g_errPl;
        if (g_pls.empty()) return g_plLoaded ? L"Плейлистов нет" : L"";
        return L"";
    }
    if (g_libMode == 1) {
        if (g_busySearch) return L"Ищу…";
        if (!g_errSearch.empty()) return L"Ошибка поиска: " + g_errSearch;
        if (g_found.empty() && g_foundArtists.empty()) return g_libQuery.empty() ? L"Нажмите на строку поиска и введите запрос" : L"Ничего не найдено";
        return L"";
    }
    if (g_busyTracks) return L"Загружаю треки…";
    if (!g_errTracks.empty()) return L"Не удалось загрузить треки: " + g_errTracks;
    if (g_plTracks.empty()) return L"Плейлист пуст";
    return L"";
}

// ответ фонового потока (WM_APP+6)
static void OwnOnResult(YmRes* r) {
    if (!r->uid.empty() && r->uid != g_uid) g_uid = r->uid;
    switch (r->kind) {
        case YJ_SEARCH:
            if (r->gen != g_genSearch) break;
            g_busySearch = false;
            if (r->ok) {
                g_found = std::move(r->tracks);
                g_foundArtists = std::move(r->artists);
                g_errSearch.clear();
            } else {
                g_errSearch = r->err.empty() ? std::wstring(L"неизвестная ошибка") : r->err;
            }
            break;
        case YJ_PLAYLISTS:
            if (r->gen != g_genPl) break;
            g_busyPl = false;
            if (r->ok) {
                g_pls = std::move(r->pls);
                g_plLoaded = true;
                g_errPl.clear();
            } else {
                g_errPl = r->err.empty() ? std::wstring(L"неизвестная ошибка") : r->err;
            }
            break;
        case YJ_PL_TRACKS:
        case YJ_ART_TRACKS:
        case YJ_ALBUM:
            if (r->gen != g_genTracks) break;
            g_busyTracks = false;
            if (r->ok) {
                g_plTracks = std::move(r->tracks);
                g_errTracks.clear();
            } else {
                g_errTracks = r->err.empty() ? std::wstring(L"неизвестная ошибка") : r->err;
            }
            break;
        case YJ_PLAY:
            if (r->gen != g_playGen) break;
            if (!r->ok) {
                OwnTrackFailed(r->err.empty() ? std::wstring(L"нет ссылки на поток") : r->err);
                break;
            }
            if (!g_mp) break;
            try {
                OwnStartStream(r->url, r->secs);
            } catch (...) {
                OwnTrackFailed(L"плеер не принял поток");
                break;
            }
            if (r->secs <= 0.0) OwnWaveFeedback("trackStarted", -1.0);
            break;
        case YJ_WAVE:
            g_waveBusy = false;
            if (r->gen != g_waveGen || g_pmode != PM_WAVE) break;
            if (r->ok) {
                g_waveBatch = r->s1;
                g_waveSession = r->session;
                for (auto& t : r->tracks)
                    if (t.available) g_waveBuf.push_back(t);
            }
            if (g_waveWait) {
                g_waveWait = false;
                if (!g_waveBuf.empty()) {
                    OwnAdvance();
                } else {
                    g_status = r->ok ? std::wstring(L"«Моя волна» вернула пустой список")
                                     : L"«Моя волна»: " + r->err;
                    OwnIdle();
                }
            }
            break;
        case YJ_LIKE:
            if (!r->ok) {  // не получилось — откатываем сердечко
                if (r->s1 == "add") g_liked.erase(std::remove(g_liked.begin(), g_liked.end(), r->track.id), g_liked.end());
                else if (!IsLiked(r->track.id)) g_liked.push_back(r->track.id);
            }
            break;
        case YJ_LIKED:
            if (r->ok) {
                g_liked = std::move(r->ids);
                g_likedLoaded = true;
            }
            break;
        case YJ_THUMB: {
            auto it = g_thumbs.find(r->s1);
            if (it == g_thumbs.end()) break;
            if (r->ok) {
                const int sz = ThumbSize();
                if (DecodeThumb(r->bytes, sz, it->second.px)) it->second.sz = sz;
            }
            break;
        }
        case YJ_COVER:
            if (r->track.id != g_cur.id || !g_haveCur) break;
            if (r->ok) {
                g_coverBytes = std::move(r->bytes);
                if (!OwnDecodeCover(true) && g_cover.empty()) g_palTgt = DefaultPalette();
            } else {
                g_cover.clear();
                g_coverPx = 0;
                g_palTgt = DefaultPalette();
            }
            break;
    }
}

// ---------------------------------------------------------------- JSON-помощники
// первое значение поля key (строка или число) в виде строки; пусто — поля нет
static std::string JsonScalar(const std::string& js, const char* key) {
    auto v = JsonStrings(js, key);
    if (!v.empty() && !v[0].empty()) return v[0];
    const std::string pat = std::string("\"") + key + "\"";
    size_t p = js.find(pat);
    if (p == std::string::npos) return std::string();
    p += pat.size();
    while (p < js.size() && (js[p] == ' ' || js[p] == ':' || js[p] == '\t' || js[p] == '\n' || js[p] == '\r')) p++;
    size_t e = p;
    while (e < js.size() && ((js[e] >= '0' && js[e] <= '9') || js[e] == '-' || js[e] == '.')) e++;
    return js.substr(p, e - p);
}

// объекты {...} массива, который идёт после первого "key": [ ... ] (не больше maxN штук)
static std::vector<std::string> JsonArrayObjects(const std::string& js, const char* key, size_t maxN) {
    std::vector<std::string> out;
    const std::string pat = std::string("\"") + key + "\"";
    size_t p = js.find(pat);
    while (p != std::string::npos) {
        size_t i = p + pat.size();
        while (i < js.size() && (js[i] == ' ' || js[i] == '\t' || js[i] == '\n' || js[i] == '\r')) i++;
        if (i < js.size() && js[i] == ':') {
            i++;
            while (i < js.size() && (js[i] == ' ' || js[i] == '\t' || js[i] == '\n' || js[i] == '\r')) i++;
            if (i < js.size() && js[i] == '[') {
                i++;
                while (i < js.size() && out.size() < maxN) {
                    while (i < js.size() && (js[i] == ',' || js[i] == ' ' || js[i] == '\t' || js[i] == '\n' || js[i] == '\r'))
                        i++;
                    if (i >= js.size() || js[i] != '{') break;
                    size_t st = i;
                    int depth = 0;
                    bool inStr = false;
                    for (; i < js.size(); i++) {
                        char c = js[i];
                        if (inStr) {
                            if (c == '\\') i++;
                            else if (c == '"') inStr = false;
                        } else if (c == '"') {
                            inStr = true;
                        } else if (c == '{') {
                            depth++;
                        } else if (c == '}') {
                            if (--depth == 0) {
                                i++;
                                break;
                            }
                        }
                    }
                    out.push_back(js.substr(st, i - st));
                }
                return out;
            }
        }
        p = js.find(pat, p + pat.size());
    }
    return out;
}

// ---------------------------------------------------------------- проверка токена (кнопка «i» в панели текста)
struct CheckJob {
    std::string genius;
};

static DWORD WINAPI CheckThread(LPVOID p) {
    CheckJob* job = (CheckJob*)p;
    std::wstring* out = new std::wstring();
    try {
        std::wstring& o = *out;
        auto stop = []() { return g_stopping; };

        HttpInfo hi;
        HttpGet(L"lrclib.net", L"/api/get?artist_name=Queen&track_name=Bohemian%20Rhapsody", L"", nullptr, &hi);
        o += L"LRCLIB: ";
        o += (hi.status == 200 || hi.status == 404) ? std::wstring(L"доступен") : GeniusHttpText(hi);
        o += L"\n";

        if (job->genius.empty()) {
            o += L"Genius: токен не задан в настройках\n";
        } else if (!stop()) {
            HttpInfo g;
            int via = 0;
            std::string js = GeniusSearch(job->genius, L"Queen%20Bohemian%20Rhapsody", g, &via);
            o += L"Genius: ";
            if (g.status == 200)
                o += via == 0   ? std::wstring(L"токен работает (OK)")
                     : via == 9 ? std::wstring(L"API отклоняет запросы (403), но поиск через сайт работает (OK)")
                                : std::wstring(L"токен работает (OK, запасной способ запроса)");
            else
                o += GeniusHttpText(g);
            o += L"\n";
            if (g.status == 200 && !stop()) {
                std::string path = "/Queen-bohemian-rhapsody-lyrics", id;
                auto hitsV = JsonArrayObjects(js, "hits", 1);
                if (!hitsV.empty()) {
                    std::string pp = JsonScalar(hitsV[0], "path"), ap = JsonScalar(hitsV[0], "api_path");
                    if (!pp.empty()) path = pp;
                    if (ap.compare(0, 7, "/songs/") == 0) id = ap.substr(7);
                }
                HttpInfo pg;
                std::string html = GeniusSiteGet(Wide(path), kGeniusPageHdr, kGeniusPageHdrSimple, pg);
                const bool pageOk = pg.status == 200 && !TrimStr(GeniusLyricsFromHtml(html)).empty();
                o += L"Genius, страница с текстом: ";
                if (pageOk) o += L"открывается, текст читается (OK)";
                else if (pg.status == 200) o += L"открывается, но текст не распознан";
                else {
                    o += pg.status ? GeniusHttpText(pg) : NetErrText(pg.err);
                    if (LooksCloudflare(html)) o += L" (проверка Cloudflare)";
                }
                o += L"\n";
                if (!pageOk && !id.empty() && !stop()) {
                    HttpInfo ei;
                    std::string ej = GeniusSiteGet(L"/songs/" + Wide(id) + L"/embed.js", kGeniusScriptHdr, kGeniusScriptHdr, ei);
                    o += L"Genius, embed.js: ";
                    if (ei.status == 200)
                        o += TrimStr(GeniusLyricsFromEmbed(ej)).empty() ? std::wstring(L"получен, но текст не распознан")
                                                                         : std::wstring(L"текст читается (OK)");
                    else
                        o += ei.status ? GeniusHttpText(ei) : NetErrText(ei.err);
                    o += L"\n";
                }
            }
        }
        if (stop()) o += L"(проверка прервана)\n";
    } catch (...) {
        *out += L"Ошибка при проверке\n";
    }
    delete job;
    if (!g_hwnd || !PostMessageW(g_hwnd, WM_APP + 5, 0, (LPARAM)out)) delete out;
    return 0;
}

static DWORD WINAPI LyThread(LPVOID p) {
    LyJob* job = (LyJob*)p;
    LyResult* res = new LyResult();
    res->key = job->key;
    res->state = 3;
    res->synced = false;
    try {
        const wchar_t* host = L"lrclib.net";
        std::wstring a1 = FirstArtist(job->artist);
        std::wstring q = L"/api/get?artist_name=" + UrlEnc(Utf8(a1)) + L"&track_name=" + UrlEnc(Utf8(job->title));
        if (job->durSec > 0) q += L"&duration=" + std::to_wstring(job->durSec);
        HttpInfo hi1, hi2;
        std::string js = HttpGet(host, q, L"", nullptr, &hi1);
        std::string synced, plain;
        auto pick = [&](const std::string& json) {
            auto sv = JsonStrings(json, "syncedLyrics");
            auto pv = JsonStrings(json, "plainLyrics");
            for (auto& s : sv)
                if (!s.empty()) {
                    synced = s;
                    break;
                }
            for (auto& s : pv)
                if (!s.empty()) {
                    plain = s;
                    break;
                }
        };
        pick(js);
        bool instrumental = js.find("\"instrumental\":true") != std::string::npos;
        if (synced.empty() && plain.empty() && !instrumental) {  // точное совпадение не нашлось — общий поиск
            js = HttpGet(host, L"/api/search?q=" + UrlEnc(Utf8(a1 + L" " + job->title)), L"", nullptr, &hi2);
            pick(js);
        }
        auto have = [&]() { return !synced.empty() || !plain.empty(); };
        std::wstring diag;
        std::wstring src;
        if (have()) src = L"LRCLIB";
        if (!have() && !instrumental) {
            if (!hi1.status && !hi2.status) diag += L"LRCLIB: " + NetErrText(hi1.err) + L"\n";
            else diag += L"LRCLIB: этого трека нет в базе\n";
        }

        // порядок: LRCLIB -> Genius (обычный текст) -> NetEase (с метками времени) -> lyrics.ovh (обычный текст)
        if (!have() && !instrumental) {
            if (!job->geniusToken.empty()) plain = GeniusLyrics(job->geniusToken, job->artist, job->title, &diag);
            else diag += L"Токен Genius не задан в настройках\n";
            if (have()) src = L"Genius";
        }
        // Genius за Cloudflare часто закрыт (403) — пробуем источники без защиты от ботов
        if (!have() && !instrumental) {
            std::string nl = NeteaseLyrics(job->artist, job->title, &diag);  // с метками времени
            if (!nl.empty()) {
                synced = nl;
                src = L"NetEase Cloud Music";
            }
        }
        if (!have() && !instrumental) {
            plain = OvhLyrics(job->artist, job->title, &diag);
            if (have()) src = L"lyrics.ovh";
        }

        if (have()) {
            ParseLyrics(synced, plain, res->lines, res->synced);
            if (!res->lines.empty()) {
                res->state = 2;
                res->source = src;
            }
        } else if (instrumental) {
            res->state = 4;
        }
        res->diag = diag;
    } catch (...) {
    }
    delete job;
    if (!g_hwnd || !PostMessageW(g_hwnd, WM_APP + 4, 0, (LPARAM)res)) delete res;
    return 0;
}

// запросить текст для текущего трека, если открыта панель текста и он ещё не загружен
static void LyEnsure() {
    if (!g_lyOpen || g_trackKey.empty() || g_trackKey == g_lyKey || g_lyBusy) return;
    g_lyKey = g_trackKey;
    g_lyState = 1;
    g_lyrics.clear();
    g_lySynced = false;
    g_lyIdx = 0;
    g_lyScroll = 0.0;
    g_lyManual = 0.0;
    g_lyH.clear();
    g_lyDiag.clear();
    g_lySource.clear();
    LyJob* job = new LyJob{g_lyKey, g_artist, g_title, (g_durTicks + 5000000) / 10000000, g_set.geniusToken};
    g_lyBusy = true;
    HANDLE th = CreateThread(nullptr, 0, LyThread, job, 0, nullptr);
    if (!th) {
        g_lyBusy = false;
        g_lyState = 3;
        delete job;
        return;
    }
    if (g_lyThread) CloseHandle(g_lyThread);
    g_lyThread = th;
}

// кнопка «i»: показать/скрыть отчёт проверки токенов; повторное нажатие после закрытия запускает проверку заново
static void ToggleCheck(HWND h) {
    if (g_chkBusy) return;
    if (g_diagShow) {
        g_diagShow = false;
        StartAnim(h);
        return;
    }
    g_diagShow = true;
    g_diagText = L"Проверяю…";
    CheckJob* job = new CheckJob{g_set.geniusToken};
    g_chkBusy = true;
    HANDLE th = CreateThread(nullptr, 0, CheckThread, job, 0, nullptr);
    if (!th) {
        g_chkBusy = false;
        g_diagText = L"Не удалось запустить проверку";
        delete job;
    } else {
        if (g_chkThread) CloseHandle(g_chkThread);
        g_chkThread = th;
    }
    StartAnim(h);
}

// индекс текущей строки синхронизированного текста
static int LyCurIndex() {
    if (!g_lySynced || g_lyrics.empty()) return 0;
    long long pos = CurPos() + (long long)g_set.lyricsOffsetMs * 10000LL + 3000000LL;  // небольшое «опережение» 300 мс
    int idx = 0;
    for (int i = 0; i < (int)g_lyrics.size(); i++) {
        if (g_lyrics[i].t <= pos) idx = i;
        else break;
    }
    return idx;
}

// ---------------------------------------------------------------- открыть/свернуть приложение
static BOOL CALLBACK EnumProc(HWND h, LPARAM lp) {
    if (!IsWindowVisible(h) || GetWindow(h, GW_OWNER)) return TRUE;
    if (GetWindowLongW(h, GWL_EXSTYLE) & WS_EX_TOOLWINDOW) return TRUE;
    if (GetWindowTextLengthW(h) == 0) return TRUE;

    DWORD pid = 0;
    GetWindowThreadProcessId(h, &pid);
    HANDLE p = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if (!p) return TRUE;
    wchar_t path[MAX_PATH * 2];
    DWORD len = ARRAYSIZE(path);
    BOOL ok = QueryFullProcessImageNameW(p, 0, path, &len);
    CloseHandle(p);
    if (ok && lstrcmpiW(path, g_set.exePath.c_str()) == 0) {
        *(HWND*)lp = h;
        return FALSE;
    }
    return TRUE;
}

// окно, которое мы сами спрятали в трей (скрытые окна EnumWindows не находит из-за IsWindowVisible)
static HWND g_hiddenApp = nullptr;

static void ToggleApp() {
    // 1) если мы ранее спрятали окно — возвращаем его
    if (g_hiddenApp) {
        HWND hw = g_hiddenApp;
        g_hiddenApp = nullptr;
        if (IsWindow(hw) && !IsWindowVisible(hw)) {
            ShowWindow(hw, IsIconic(hw) ? SW_RESTORE : SW_SHOW);
            SetForegroundWindow(hw);
            return;
        }
    }

    HWND w = nullptr;
    EnumWindows(EnumProc, (LPARAM)&w);
    if (!w) {
        // окна нет (приложение не запущено или полностью скрыто само) — запускаем/будим
        ShellExecuteW(nullptr, L"open", g_set.exePath.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
        return;
    }
    if (IsIconic(w)) {
        ShowWindow(w, SW_RESTORE);
        SetForegroundWindow(w);
    } else if (GetAncestor(GetForegroundWindow(), GA_ROOT) == w) {
        // повторный клик: прячем окно — оно пропадает с панели задач, но процесс продолжает работать
        ShowWindow(w, SW_HIDE);
        g_hiddenApp = w;
    } else {
        SetForegroundWindow(w);
    }
}

// ---------------------------------------------------------------- отрисовка
// ---- анимации: кнопки при наведении, раскрытие «острова», смена палитры
static const ULONGLONG kAnimMs = 240;    // кнопки при наведении
static double g_animT = 0.0;             // линейный прогресс 0..1 (к g_hover)
static ULONGLONG g_animLast = 0;         // время предыдущего шага
static int g_btnLeft = 0;                // левый край блока кнопок компактного режима (считается в Render)

static double EaseOutCubic(double t) {
    t = std::clamp(t, 0.0, 1.0);
    double u = 1.0 - t;
    return 1.0 - u * u * u;
}

// запуск/продолжение анимации
static void StartAnim(HWND h) {
    g_animLast = GetTickCount64();
    SetTimer(h, 4, 15, nullptr);
}

// пружина раскрытия: при раскрытии чуть «перелетает» (как Dynamic Island), при сворачивании — без колебаний
static double g_expV = 0.0;
static bool StepSpring(double ms) {
    const double target = g_expand ? 1.0 : 0.0;
    const double k = 220.0;
    const double c = g_expand ? 20.8 : 2.0 * sqrt(k);
    double dt = std::min(ms, 50.0) / 1000.0;
    int n = std::max(1, (int)ceil(dt / 0.004));
    double hs = dt / n;
    for (int i = 0; i < n; i++) {
        double a = k * (target - g_expT) - c * g_expV;
        g_expV += a * hs;
        g_expT += g_expV * hs;
    }
    if (fabs(target - g_expT) < 0.0008 && fabs(g_expV) < 0.02) {
        g_expT = target;
        g_expV = 0.0;
        return false;
    }
    return true;
}

// плавное приближение v к цели со скоростью rate (доля за шаг); true — ещё идёт
static bool Approach(double& v, double target, double step) {
    if (v < target) v = std::min(target, v + step);
    else if (v > target) v = std::max(target, v - step);
    return v != target;
}

// шаг анимации; true — что-то ещё движется
static bool StepAnim() {
    ULONGLONG now = GetTickCount64();
    double ms = (double)(now - g_animLast);
    g_animLast = now;
    bool busy = false;

    // кнопки компактного режима при наведении
    double dt = ms / (double)kAnimMs;
    double target = g_hover ? 1.0 : 0.0;
    if (g_animT < target) g_animT = std::min(target, g_animT + dt);
    else if (g_animT > target) g_animT = std::max(target, g_animT - dt * 1.3);  // скрытие чуть быстрее
    busy |= g_animT != target;

    busy |= StepSpring(ms);

    // поэтапное появление элементов развёрнутого плеера (при сворачивании — быстрее)
    busy |= Approach(g_stageT, g_expand ? 1.0 : 0.0, ms / (g_expand ? 520.0 : 260.0));

    // значок play/pause и размер обложки
    busy |= Approach(g_playT, g_playing ? 1.0 : 0.0, ms / 220.0);

    // появление текста при смене трека
    busy |= Approach(g_textT, 1.0, ms / 320.0);

    // кросс-фейд обложек
    if (!g_coverOld.empty()) {
        g_coverFade += ms / 380.0;
        if (g_coverFade >= 1.0) {
            g_coverFade = 1.0;
            g_coverOld.clear();
            g_coverOldPx = 0;
        } else {
            busy = true;
        }
    }

    // наведение и нажатие в развёрнутом режиме
    for (int i = 1; i < 20; i++) busy |= Approach(g_hotT[i], g_hot == i ? 1.0 : 0.0, ms / 140.0);
    for (int i = 1; i < 8; i++) busy |= Approach(g_pressT[i], g_pressed == i ? 1.0 : 0.0, ms / 90.0);

    // панель текста песни: открытие и плавная прокрутка к текущей строке
    if (!g_expand && g_lyOpen) g_lyOpen = false;  // при сворачивании плеера панель закрывается
    if (!g_lyOpen || !g_set.showInfoBtn) g_diagShow = false;
    busy |= Approach(g_lyT, g_lyOpen ? 1.0 : 0.0, ms / 380.0);

    // панель библиотеки: при сворачивании плеера закрывается вместе с окном ввода запроса
    if (!g_expand && g_libOpen) {
        g_libOpen = false;
        CloseSearchPopup();
    }
    if (!OwnMode() && g_libOpen) g_libOpen = false;
    busy |= Approach(g_libT, g_libOpen ? 1.0 : 0.0, ms / 380.0);
    if (g_lyOpen && g_lyState == 2) {
        if (g_lySynced) g_lyIdx = LyCurIndex();
        double d = g_lyIdx - g_lyScroll;
        if (fabs(d) > 0.004) {
            g_lyScroll += d * std::min(1.0, ms / 150.0);
            busy = true;
        } else {
            g_lyScroll = g_lyIdx;
        }
        if (g_lySynced && g_lyManual != 0.0 && GetTickCount64() > g_lyManualUntil) {  // возврат к текущей строке
            g_lyManual -= g_lyManual * std::min(1.0, ms / 160.0);
            if (fabs(g_lyManual) < 0.5) g_lyManual = 0.0;
            else busy = true;
        }
    }

    busy |= StepPalette(ms);
    return busy;
}

static int HitBtn(int x) {
    if (!g_hover || g_btnLeft <= 0) return 0;
    if (x < g_btnLeft || x >= g_btnLeft + 3 * CellW()) return 0;
    return (x - g_btnLeft) / CellW() + 1;
}

static double Smooth01(double t) {
    t = std::clamp(t, 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

static bool In(const RECT& r, int x, int y) {
    return x >= r.left && x < r.right && y >= r.top && y < r.bottom;
}

static void DrawTextRect(HDC dc, HFONT font, COLORREF col, PCWSTR text, RECT rc, UINT fmt) {
    HGDIOBJ old = SelectObject(dc, font);
    SetTextColor(dc, col);
    DrawTextW(dc, text, -1, &rc, fmt | DT_NOPREFIX | DT_SINGLELINE);
    SelectObject(dc, old);
}

// ---- раскладка развёрнутого плеера (координаты внутри его «кадра» g_we x g_he)
struct ExpLay {
    RECT cover, title, artist;
    RECT bar, barHit, tCur, tTot;
    RECT prev, play, next;
    RECT volIcon, volBar, volHit;
    RECT lyrics, lyBtn;  // панель текста сверху и кнопка её открытия
    RECT lyChk;          // кнопка «i» (проверка источников) в углу панели текста
    RECT libBtn, like;   // кнопка библиотеки (слева в ряду кнопок) и сердечко «нравится» (справа от названия)
    RECT wavePill;       // плашка «Моя волна» над названием трека (только пока играет волна)
    int lyFull;          // полная высота панели текста
    int libFull;         // полная высота панели библиотеки
    int H;
};

// подпись плашки: что сейчас играет
static std::wstring WavePillLabel() {
    if (g_pmode == PM_WAVE) return g_waveSeed.empty() ? L"Моя волна" : L"Волна по треку";
    return g_srcLabel;
}

// плашка «откуда играет» нужна: волна (даже пока подбираются треки) или очередь из плейлиста/альбома/поиска
static bool SourcePillOn() {
    if (!OwnMode()) return false;
    if (g_pmode == PM_WAVE) return true;
    return g_pmode == PM_LIST && g_haveCur && !g_srcLabel.empty();
}

static int WavePillWidth() {
    static int cacheKey = -1;
    static std::wstring cacheLbl;
    static int cacheW = 0;
    const int fpx = S(std::max(9, g_set.fontSize - 3));
    const std::wstring lbl = WavePillLabel();
    if (cacheKey != fpx || cacheLbl != lbl) {
        HDC dc = GetDC(nullptr);
        HFONT f = CreateFontW(-fpx, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, ANTIALIASED_QUALITY, 0,
                              L"Segoe UI");
        HGDIOBJ old = SelectObject(dc, f);
        SIZE sz = {};
        GetTextExtentPoint32W(dc, lbl.c_str(), (int)lbl.size(), &sz);
        SelectObject(dc, old);
        DeleteObject(f);
        ReleaseDC(nullptr, dc);
        cacheKey = fpx;
        cacheLbl = lbl;
        cacheW = (int)sz.cx;
    }
    return cacheW + S(30);  // слева место под «эквалайзер»
}

static ExpLay CalcExpLay() {
    ExpLay L = {};
    const int W = g_we;
    const int pad = S(14), cs = CoverExpSize();
    // место под панель текста песни сверху (растёт при её открытии)
    const int lyFull = S(176), libFull = S(272), lyGap = S(10);
    const bool own = OwnMode();
    // панели текста и библиотеки делят одно место; пока одна закрывается, а другая открывается, высоты складываются
    const int yo = (int)std::lround((lyFull + lyGap) * Smooth01(g_lyT) + (libFull + lyGap) * Smooth01(g_libT));
    const int top = pad + yo;
    L.lyFull = lyFull;
    L.libFull = libFull;
    L.lyrics = {pad, pad, W - pad, pad + std::max(0, yo - lyGap)};
    if (g_set.showInfoBtn && !g_libOpen && L.lyrics.bottom - L.lyrics.top > S(40)) {
        const int cb = S(26), cm = S(6);
        L.lyChk = {L.lyrics.right - cm - cb, L.lyrics.top + cm, L.lyrics.right - cm, L.lyrics.top + cm + cb};
    }
    L.cover = {pad, top, pad + cs, top + cs};

    const int tx = pad + cs + S(12);
    const int lh1 = S(g_set.fontSize + 2) + S(6), lh2 = S(g_set.fontSize) + S(4);
    const bool waveOn = SourcePillOn();  // над названием — плашка «откуда играет»
    const int lw = waveOn ? S(18) : 0;
    const int ty = top + (cs - lh1 - lh2 - lw) / 2;
    int tr = W - pad;
    if (own) {  // сердечко «нравится» справа от названия
        const int lk = S(32), lky = top + (cs - lk) / 2;
        L.like = {W - pad - lk, lky, W - pad, lky + lk};
        tr = L.like.left - S(6);
    }
    if (waveOn) L.wavePill = {tx, ty, std::min(tr, tx + WavePillWidth()), ty + S(16)};
    L.title = {tx, ty + lw, tr, ty + lw + lh1};
    L.artist = {tx, ty + lw + lh1, tr, ty + lw + lh1 + lh2};

    const int barH = std::max(3, S(5));
    const int by = top + cs + S(12);
    L.bar = {pad, by, W - pad, by + barH};
    L.barHit = {pad, by - S(8), W - pad, by + barH + S(8)};
    const int ly = by + barH + S(3), lh = S(15);
    L.tCur = {pad, ly, pad + S(70), ly + lh};
    L.tTot = {W - pad - S(70), ly, W - pad, ly + lh};

    const int cy = ly + lh + S(6) + S(20), half = S(20);
    L.play = {W / 2 - half, cy - half, W / 2 + half, cy + half};
    L.lyBtn = {W - pad - S(34), cy - S(17), W - pad, cy + S(17)};
    if (own) L.libBtn = {pad, cy - S(17), pad + S(34), cy + S(17)};
    L.prev = {W / 2 - S(64) - half, cy - half, W / 2 - S(64) + half, cy + half};
    L.next = {W / 2 + S(64) - half, cy - half, W / 2 + S(64) + half, cy + half};

    const int vy = cy + half + S(16);
    L.volIcon = {pad, vy - S(12), pad + S(26), vy + S(12)};
    const int vx0 = pad + S(32);
    L.volBar = {vx0, vy - barH / 2, W - pad, vy - barH / 2 + barH};
    L.volHit = {vx0 - S(4), vy - S(12), W - pad, vy + S(12)};
    L.H = vy + S(12) + pad;
    return L;
}

// ---- исполнители в строке под названием трека: у каждого имени свой участок (x в координатах «кадра»)
static int g_artHover = -1;  // над каким исполнителем курсор (индекс в g_cur.artistList)

static void ArtistSpans(const ExpLay& L, std::vector<std::pair<int, int>>& sp) {
    sp.clear();
    if (!g_haveCur || g_cur.artistList.empty()) return;
    HDC dc = GetDC(nullptr);
    HFONT f = CreateFontW(-S(g_set.fontSize), 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, 0, 0, ANTIALIASED_QUALITY, 0,
                          L"Segoe UI");
    HGDIOBJ old = SelectObject(dc, f);
    std::wstring pre;
    for (size_t i = 0; i < g_cur.artistList.size(); i++) {
        if (i) pre += L", ";
        SIZE a = {}, b = {};
        GetTextExtentPoint32W(dc, pre.c_str(), (int)pre.size(), &a);
        pre += g_cur.artistList[i].second;
        GetTextExtentPoint32W(dc, pre.c_str(), (int)pre.size(), &b);
        const int x0 = (int)L.artist.left + (int)a.cx;
        const int x1 = std::min((int)L.artist.left + (int)b.cx, (int)L.artist.right);
        sp.push_back({x0, std::max(x0, x1)});
    }
    SelectObject(dc, old);
    DeleteObject(f);
    ReleaseDC(nullptr, dc);
}

// индекс исполнителя под координатой fx или -1 (кликабельны только имена с известным id)
static int ArtistAtX(const ExpLay& L, int fx) {
    std::vector<std::pair<int, int>> sp;
    ArtistSpans(L, sp);
    for (size_t i = 0; i < sp.size(); i++)
        if (fx >= sp[i].first && fx < sp[i].second && !g_cur.artistList[i].first.empty()) return (int)i;
    return -1;
}

// профиль исполнителя: открываем библиотеку и показываем его треки (как при выборе исполнителя в поиске)
static void OpenArtistFromPlayer(HWND h, int idx) {
    if (!g_haveCur || idx < 0 || idx >= (int)g_cur.artistList.size()) return;
    const std::pair<std::string, std::wstring> a = g_cur.artistList[(size_t)idx];
    if (a.first.empty()) return;
    g_lyOpen = false;  // панели текста и библиотеки делят одно место
    g_diagShow = false;
    CloseSearchPopup();
    g_libOpen = true;
    LibLoadPlaylists(false);  // чтобы кнопка «назад» вела к списку плейлистов
    LibOpenArtistId(a.first, a.second, 0);
    StartAnim(h);
}

// открыть библиотеку и показать профиль исполнителя / альбом (из меню трека)
static void OpenLibraryFor(HWND h, bool& fromSearch) {
    fromSearch = g_libOpen && g_libMode == 1;
    if (fromSearch) g_libBackScroll = g_libScroll;
    g_lyOpen = false;
    g_diagShow = false;
    if (!fromSearch) CloseSearchPopup();
    if (!g_libOpen) g_libOpen = true;
    LibLoadPlaylists(false);
}

static void OpenAlbumOfTrack(HWND h, const YmTrack& t) {
    if (t.albumId.empty()) return;
    bool fs = false;
    OpenLibraryFor(h, fs);
    LibOpenAlbumId(t.albumId, t.albumTitle, fs ? 1 : 0);
    StartAnim(h);
}

static void OpenArtistOf(HWND h, const std::string& id, const std::wstring& name) {
    if (id.empty()) return;
    bool fs = false;
    OpenLibraryFor(h, fs);
    LibOpenArtistId(id, name, fs ? 1 : 0);
    StartAnim(h);
}

// контекстное меню трека (ПКМ): волна по треку, альбом, исполнители
static void ShowTrackMenu(HWND h, YmTrack t) {  // копия: пока меню открыто, списки могут обновиться
    enum { C_WAVE = 1, C_ALBUM, C_ART0 = 10 };
    HMENU m = CreatePopupMenu();
    if (!m) return;
    AppendMenuW(m, MF_STRING, C_WAVE, L"Волна по треку");
    AppendMenuW(m, MF_STRING | (t.albumId.empty() ? MF_GRAYED : 0), C_ALBUM, L"Открыть альбом");
    bool sep = false;
    for (size_t i = 0; i < t.artistList.size() && i < 5; i++) {
        if (t.artistList[i].first.empty()) continue;
        if (!sep) {
            AppendMenuW(m, MF_SEPARATOR, 0, nullptr);
            sep = true;
        }
        std::wstring name = t.artistList[i].second, esc;
        for (wchar_t c : name) {
            esc += c;
            if (c == L'&') esc += L'&';
        }
        AppendMenuW(m, MF_STRING, (UINT_PTR)(C_ART0 + i), (L"Исполнитель: " + esc).c_str());
    }
    POINT pt;
    GetCursorPos(&pt);
    g_menuOpen = true;
    SetForegroundWindow(h);
    const int cmd = (int)TrackPopupMenu(m, TPM_RETURNCMD | TPM_RIGHTBUTTON | TPM_NONOTIFY, pt.x, pt.y, 0, h, nullptr);
    PostMessageW(h, WM_NULL, 0, 0);
    DestroyMenu(m);
    g_menuOpen = false;
    if (cmd == C_WAVE) {
        OwnStartWaveFromTrack(t);
    } else if (cmd == C_ALBUM) {
        OpenAlbumOfTrack(h, t);
    } else if (cmd >= C_ART0 && cmd < C_ART0 + (int)t.artistList.size()) {
        OpenArtistOf(h, t.artistList[(size_t)(cmd - C_ART0)].first, t.artistList[(size_t)(cmd - C_ART0)].second);
    }
    // курсор мог уйти с виджета, пока было открыто меню
    RECT wr;
    POINT cp;
    GetWindowRect(h, &wr);
    GetCursorPos(&cp);
    if (!PtInRect(&wr, cp)) OnLeave(h);
    StartAnim(h);
}

// ---- библиотека (поиск, плейлисты): раскладка панели; координаты в той же системе, что и переданный прямоугольник
struct LibLay {
    RECT search, wave, tune, shake, tabA, tabB, title, list;
    int rowH;
};

// lp — прямоугольник панели (при открытии растёт сверху вниз), libFull — её полная высота;
// содержимое привязано к нижнему краю, поэтому при анимации оно «выезжает», а не прыгает
static LibLay CalcLibLay(const RECT& lp, int libFull) {
    LibLay Y = {};
    const int m = S(8), hh = S(32), tabH = S(24), gap = S(6);
    const int y0 = lp.bottom - libFull;
    const int waveW = S(84), btnW = S(30);
    // верхний ряд: поиск | «Моя волна» | настройки волны | встряхнуть
    Y.shake = {lp.right - m - btnW, y0 + m, lp.right - m, y0 + m + hh};
    Y.tune = {Y.shake.left - gap - btnW, y0 + m, Y.shake.left - gap, y0 + m + hh};
    Y.wave = {Y.tune.left - gap - waveW, y0 + m, Y.tune.left - gap, y0 + m + hh};
    Y.search = {lp.left + m, y0 + m, Y.wave.left - gap, y0 + m + hh};
    const int ty = y0 + m + hh + gap;
    Y.tabA = {lp.left + m, ty, lp.left + m + S(98), ty + tabH};
    Y.tabB = {Y.tabA.right + gap, ty, Y.tabA.right + gap + S(98), ty + tabH};
    Y.title = {Y.tabB.right + S(10), ty, lp.right - m, ty + tabH};
    Y.list = {lp.left + m, ty + tabH + S(4), lp.right - m, lp.bottom - m};
    Y.rowH = S(40);
    return Y;
}

// ---- настройки «Моей волны»: «чипы» опций, перенос по строкам; координаты — внутри списка (без прокрутки)
struct WaveChip {
    RECT r;
    int g, o;  // группа и опция
};
struct WaveLay {
    std::vector<WaveChip> chips;
    int labelY[4];
    int labelH;
    int noteY;
    int contentH;
};

static WaveLay CalcWaveLay(const LibLay& Y) {
    WaveLay W = {};
    const int fpx = S(std::max(9, g_set.fontSize - 2));
    static int cacheKey = -1;
    static int cw[4][8];
    if (cacheKey != fpx) {  // ширины подписей по реальному шрифту (тот же, что у вкладок библиотеки)
        HDC dc = GetDC(nullptr);
        HFONT f = CreateFontW(-fpx, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, 0, 0, ANTIALIASED_QUALITY, 0,
                              L"Segoe UI");
        HGDIOBJ old = SelectObject(dc, f);
        for (int g = 0; g < 4; g++)
            for (int o = 0; o < kWaveGroups[g].n && o < 8; o++) {
                SIZE sz = {};
                const wchar_t* t = kWaveGroups[g].opts[o].label;
                GetTextExtentPoint32W(dc, t, (int)wcslen(t), &sz);
                cw[g][o] = (int)sz.cx;
            }
        SelectObject(dc, old);
        DeleteObject(f);
        ReleaseDC(nullptr, dc);
        cacheKey = fpx;
    }
    const int left = S(6), right = (int)(Y.list.right - Y.list.left) - S(10);
    const int chipH = S(26), gx = S(6), gy = S(6), padX = S(11);
    W.labelH = S(16);
    int y = S(2);
    W.noteY = y;
    y += S(18);
    for (int g = 0; g < 4; g++) {
        W.labelY[g] = y;
        y += W.labelH + S(3);
        int x = left;
        for (int o = 0; o < kWaveGroups[g].n && o < 8; o++) {
            const int w = cw[g][o] + 2 * padX;
            if (x + w > right && x > left) {
                x = left;
                y += chipH + gy;
            }
            RECT r = {x, y, x + w, y + chipH};
            W.chips.push_back({r, g, o});
            x += w + gx;
        }
        y += chipH + S(12);
    }
    W.contentH = y;
    return W;
}

static double LibMaxScroll(const LibLay& Y) {
    const double lh = (double)(Y.list.bottom - Y.list.top);
    if (g_libMode == 3) return std::max(0.0, (double)CalcWaveLay(Y).contentH - lh);
    return std::max(0.0, (double)LibRowCount() * Y.rowH - lh);
}

// чип настроек под курсором (индекс в WaveLay::chips) или -1
static int WaveChipAt(const LibLay& Y, int x, int y) {
    if (!In(Y.list, x, y)) return -1;
    const WaveLay W = CalcWaveLay(Y);
    const int ox = (int)Y.list.left, oy = (int)Y.list.top - (int)std::lround(g_libScroll);
    for (size_t i = 0; i < W.chips.size(); i++) {
        RECT r = W.chips[i].r;
        OffsetRect(&r, ox, oy);
        if (In(r, x, y)) return (int)i;
    }
    return -1;
}

static int LibRowAt(const LibLay& Y, int x, int y) {
    if (Y.rowH <= 0 || !In(Y.list, x, y)) return -1;
    int i = (int)floor(((double)(y - Y.list.top) + g_libScroll) / (double)Y.rowH);
    return (i >= 0 && i < LibRowCount()) ? i : -1;
}

// ---- ввод поискового запроса. Виджет не активируется (WS_EX_NOACTIVATE), поэтому обычное поле ввода
// клавиатуру не получает. Поле рисует сам виджет, а клавиши, пока оно активно, перехватывает низкоуровневый хук:
// съедаются только печатные символы и клавиши правки (←, →, Home, End, Del, Backspace, Enter, Esc, Ctrl+V),
// всё остальное (Win-/Alt-комбинации, F-клавиши, громкость и т. д.) проходит как обычно
static bool g_searchActive = false;
static std::wstring g_searchText;
static int g_searchCaret = 0;
static HHOOK g_kbHook = nullptr;
static ULONGLONG g_searchBeganAt = 0, g_searchLastKey = 0;
static RECT g_searchScr = {};  // поле поиска в экранных координатах (для проверки кликов мимо)
static bool g_kbSwallowed[256] = {};
static const size_t kSearchMax = 200;

static LRESULT CALLBACK SearchKbProc(int code, WPARAM w, LPARAM l) {
    if (code != HC_ACTION || !g_searchActive || !g_hwnd || !l) return CallNextHookEx(nullptr, code, w, l);
    const KBDLLHOOKSTRUCT* k = (const KBDLLHOOKSTRUCT*)l;
    const DWORD vk = k->vkCode;
    if (vk > 255 || (k->flags & LLKHF_INJECTED)) return CallNextHookEx(nullptr, code, w, l);
    const bool down = (w == WM_KEYDOWN || w == WM_SYSKEYDOWN);
    if (!down) {  // отпускание клавиши, нажатие которой мы съели
        if (g_kbSwallowed[vk]) {
            g_kbSwallowed[vk] = false;
            return 1;
        }
        return CallNextHookEx(nullptr, code, w, l);
    }
    const bool ctrl = (GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
    const bool alt = (k->flags & LLKHF_ALTDOWN) != 0;
    const bool shift = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
    const bool win = ((GetAsyncKeyState(VK_LWIN) | GetAsyncKeyState(VK_RWIN)) & 0x8000) != 0;
    if (win || (alt && !ctrl)) return CallNextHookEx(nullptr, code, w, l);  // системные комбинации
    const bool editCtrl = ctrl && !alt;  // Ctrl+Alt — это AltGr: даёт обычные символы

    bool handled = false;
    wchar_t chars[4] = {};
    int nch = 0;
    switch (vk) {
        case VK_BACK:
        case VK_DELETE:
        case VK_LEFT:
        case VK_RIGHT:
        case VK_HOME:
        case VK_END:
        case VK_RETURN:
        case VK_ESCAPE:
        case VK_TAB:
            handled = true;
            break;
        default:
            if (editCtrl) {
                handled = (vk == 'V');
            } else {
                BYTE ks[256] = {};
                if (shift) ks[VK_SHIFT] = 0x80;
                if (ctrl) ks[VK_CONTROL] = 0x80;
                if (alt) ks[VK_MENU] = 0x80;
                if (GetKeyState(VK_CAPITAL) & 1) ks[VK_CAPITAL] = 1;
                HWND fg = GetForegroundWindow();
                HKL hkl = GetKeyboardLayout(fg ? GetWindowThreadProcessId(fg, nullptr) : 0);
                WCHAR buf[8] = {};
                const int n = ToUnicodeEx(vk, k->scanCode, ks, buf, 8, 0x4, hkl);  // 0x4 — не менять состояние «мёртвых» клавиш
                if (n > 0 && buf[0] >= 0x20 && buf[0] != 0x7f) {
                    for (int i = 0; i < n && nch < 3; i++) chars[nch++] = buf[i];
                    handled = true;
                } else if (n < 0) {
                    handled = true;  // «мёртвая» клавиша — не отдаём чужому окну
                }
            }
    }
    if (!handled) return CallNextHookEx(nullptr, code, w, l);
    g_kbSwallowed[vk] = true;
    const WPARAM wp = (WPARAM)vk | (editCtrl ? 0x100 : 0);
    if (nch == 0) PostMessageW(g_hwnd, WM_APP + 10, wp, 0);
    else
        for (int i = 0; i < nch; i++) PostMessageW(g_hwnd, WM_APP + 10, wp, (LPARAM)chars[i]);
    return 1;
}

static void SearchUnhook() {
    if (g_kbHook) {
        UnhookWindowsHookEx(g_kbHook);
        g_kbHook = nullptr;
    }
    std::fill(std::begin(g_kbSwallowed), std::end(g_kbSwallowed), false);
}

// закончить ввод; submit — выполнить поиск по набранному
static void SearchEnd(bool submit) {
    if (!g_searchActive) return;
    g_searchActive = false;
    SearchUnhook();
    std::wstring t = g_searchText;
    size_t b = t.find_first_not_of(L" \t\r\n"), e = t.find_last_not_of(L" \t\r\n");
    t = b == std::wstring::npos ? std::wstring() : t.substr(b, e - b + 1);
    if (submit && !t.empty()) LibSearch(t);
    if (g_hwnd) PostMessageW(g_hwnd, WM_APP + 9, 0, 0);  // если курсор не над плеером — обычное сворачивание
}

static void CloseSearchPopup() { SearchEnd(false); }

static void SearchShutdown() {  // при выгрузке мода: без сообщений окну
    g_searchActive = false;
    SearchUnhook();
}

// fr — прямоугольник поля поиска в координатах «кадра» развёрнутого плеера
static void SearchBegin(const RECT& fr) {
    if (g_searchActive) return;
    g_kbHook = SetWindowsHookExW(WH_KEYBOARD_LL, SearchKbProc, GetModuleHandleW(nullptr), 0);
    if (!g_kbHook) {
        Wh_Log(L"не удалось установить перехват клавиатуры: %lu", GetLastError());
        return;
    }
    g_searchText = g_libQuery;
    g_searchCaret = (int)g_searchText.size();
    g_searchBeganAt = g_searchLastKey = GetTickCount64();
    const int x = g_x + g_oxE + fr.left, y = g_y + g_oyE + fr.top;
    g_searchScr = {x, y, x + (fr.right - fr.left), y + (fr.bottom - fr.top)};
    std::fill(std::begin(g_kbSwallowed), std::end(g_kbSwallowed), false);
    g_searchActive = true;
}

static void SearchPaste() {
    if (!OpenClipboard(g_hwnd)) return;
    HANDLE hd = GetClipboardData(CF_UNICODETEXT);
    if (hd) {
        const wchar_t* p = (const wchar_t*)GlobalLock(hd);
        if (p) {
            std::wstring add(p);
            GlobalUnlock(hd);
            for (auto& ch : add)
                if (ch < 0x20) ch = L' ';  // переводы строк и табуляции — в пробелы
            const size_t room = g_searchText.size() < kSearchMax ? kSearchMax - g_searchText.size() : 0;
            if (add.size() > room) add.resize(room);
            g_searchText.insert((size_t)g_searchCaret, add);
            g_searchCaret += (int)add.size();
        }
    }
    CloseClipboard();
}

// клавиша из хука (приходит сообщением WM_APP+10)
static void SearchKey(UINT vk, bool ctrl, wchar_t ch) {
    g_searchLastKey = GetTickCount64();
    std::wstring& t = g_searchText;
    int& c = g_searchCaret;
    c = std::clamp(c, 0, (int)t.size());
    if (vk == 'V' && ctrl) {
        SearchPaste();
        return;
    }
    switch (vk) {
        case VK_RETURN:
            SearchEnd(true);
            return;
        case VK_ESCAPE:
            SearchEnd(false);
            return;
        case VK_BACK:
            if (ctrl) {  // слово целиком
                int st = c;
                while (st > 0 && t[(size_t)st - 1] == L' ') st--;
                while (st > 0 && t[(size_t)st - 1] != L' ') st--;
                t.erase((size_t)st, (size_t)(c - st));
                c = st;
            } else if (c > 0) {
                t.erase((size_t)c - 1, 1);
                c--;
            }
            return;
        case VK_DELETE:
            if (c < (int)t.size()) t.erase((size_t)c, 1);
            return;
        case VK_LEFT:
            if (c > 0) c--;
            return;
        case VK_RIGHT:
            if (c < (int)t.size()) c++;
            return;
        case VK_HOME:
            c = 0;
            return;
        case VK_END:
            c = (int)t.size();
            return;
        default:
            break;
    }
    if (ch >= 0x20 && t.size() < kSearchMax) {
        t.insert((size_t)c, 1, ch);
        c++;
    }
}

static void ToggleLibrary(HWND h) {
    g_libOpen = !g_libOpen;
    if (g_libOpen) {
        g_lyOpen = false;  // панели текста и библиотеки делят одно место
        g_diagShow = false;
        LibLoadPlaylists(false);
    }
    StartAnim(h);
}

// клик внутри открытой панели библиотеки; fx, fy — в координатах «кадра»
static void LibClick(HWND h, const ExpLay& L, int fx, int fy) {
    const LibLay Y = CalcLibLay(L.lyrics, L.libFull);
    if (In(Y.search, fx, fy)) {
        SearchBegin(Y.search);
    } else if (In(Y.wave, fx, fy)) {
        OwnStartWave();
    } else if (In(Y.tune, fx, fy)) {  // настройки «Моей волны» (повторное нажатие — назад к плейлистам)
        if (g_libMode == 3) {
            g_libMode = 0;
            LibLoadPlaylists(false);
        } else {
            g_libMode = 3;
        }
        g_libScroll = 0.0;
    } else if (In(Y.shake, fx, fy)) {  // «Встряхнуть Мою волну»
        OwnShakeWave();
    } else if (In(Y.tabA, fx, fy)) {  // «Плейлисты» / «‹ Плейлисты» / «‹ Поиск» (из профиля исполнителя)
        if (g_libMode == 2 && g_libBack == 1) {
            g_libMode = 1;
            g_libScroll = g_libBackScroll;
        } else {
            g_libMode = 0;
            g_libScroll = 0.0;
            LibLoadPlaylists(false);
        }
    } else if (In(Y.tabB, fx, fy)) {  // «Поиск» / «▶ Играть всё»
        if (g_libMode == 2) {
            g_srcLabel = LibSourceLabel();
            OwnPlayQueue(g_plTracks, 0, PM_LIST);
        } else {
            g_libMode = 1;
            g_libScroll = 0.0;
        }
    } else if (g_libMode == 3) {  // выбор опции настройки волны
        const int ci = WaveChipAt(Y, fx, fy);
        if (ci >= 0) {
            const WaveLay WL = CalcWaveLay(Y);
            const WaveChip& c = WL.chips[(size_t)ci];
            OwnSetWaveCfg(c.g, kWaveGroups[c.g].opts[c.o].id);
        }
    } else {
        const int row = LibRowAt(Y, fx, fy);
        if (row >= 0) {
            if (g_libMode == 0) {
                LibOpenPlaylist(row);
            } else if (g_libMode == 1) {
                if (LibRowArtist(row)) LibOpenArtist(row);  // профиль исполнителя
                else {
                    g_srcLabel = LibSourceLabel();
                    OwnPlayQueue(g_found, row - LibArtistRows(), PM_LIST);  // треки поиска — очередь
                }
            } else {
                g_srcLabel = LibSourceLabel();
                OwnPlayQueue(g_plTracks, row, PM_LIST);
            }
        }
    }
    StartAnim(h);
}

// ---- растровые примитивы (буфер — премультиплицированный BGRA размером g_w x g_h)
static inline void BlendPx(DWORD& d, unsigned sr, unsigned sg, unsigned sb, unsigned a) {
    if (!a) return;
    if (a > 255) a = 255;
    unsigned ia = 255 - a;
    unsigned dA = d >> 24, dR = (d >> 16) & 255, dG = (d >> 8) & 255, dB = d & 255;
    unsigned oA = std::min(255u, a + dA * ia / 255);
    unsigned oR = std::min(255u, (sr * a + dR * ia) / 255);
    unsigned oG = std::min(255u, (sg * a + dG * ia) / 255);
    unsigned oB = std::min(255u, (sb * a + dB * ia) / 255);
    d = (oA << 24) | (oR << 16) | (oG << 8) | oB;
}

// покрытие пикселя (центр px,py) скруглённым прямоугольником, 0..1 с антиалиасингом
static float RRCov(float px, float py, float x0, float y0, float x1, float y1, float r) {
    float cx = (x0 + x1) * 0.5f, cy = (y0 + y1) * 0.5f;
    float hx = (x1 - x0) * 0.5f, hy = (y1 - y0) * 0.5f;
    r = std::max(0.f, std::min(r, std::min(hx, hy)));
    float qx = fabsf(px - cx) - (hx - r), qy = fabsf(py - cy) - (hy - r);
    float dist = sqrtf(std::max(qx, 0.f) * std::max(qx, 0.f) + std::max(qy, 0.f) * std::max(qy, 0.f)) +
                 std::min(std::max(qx, qy), 0.f) - r;
    return std::clamp(0.5f - dist, 0.f, 1.f);
}

static void FillRR(DWORD* px, float x0, float y0, float x1, float y1, float r, unsigned R, unsigned G,
                   unsigned B, unsigned a) {
    if (!a || x1 <= x0 || y1 <= y0) return;
    int ix0 = std::max(0, (int)floorf(x0) - 1), iy0 = std::max(0, (int)floorf(y0) - 1);
    int ix1 = std::min(g_w, (int)ceilf(x1) + 1), iy1 = std::min(g_h, (int)ceilf(y1) + 1);
    for (int y = iy0; y < iy1; y++) {
        for (int x = ix0; x < ix1; x++) {
            float c = RRCov(x + 0.5f, y + 0.5f, x0, y0, x1, y1, r);
            if (c > 0.f) BlendPx(px[(size_t)y * g_w + x], R, G, B, (unsigned)(a * c + 0.5f));
        }
    }
}

// треугольник с антиалиасингом 4x4
static void FillTri(DWORD* px, float ax, float ay, float bx, float by, float cx, float cy, unsigned R,
                    unsigned G, unsigned B, unsigned a) {
    auto edge = [](float x0, float y0, float x1, float y1, float x, float y) {
        return (x1 - x0) * (y - y0) - (y1 - y0) * (x - x0);
    };
    float area = edge(ax, ay, bx, by, cx, cy);
    if (fabsf(area) < 1e-3f || !a) return;
    int ix0 = std::max(0, (int)floorf(std::min({ax, bx, cx})));
    int ix1 = std::min(g_w, (int)ceilf(std::max({ax, bx, cx})));
    int iy0 = std::max(0, (int)floorf(std::min({ay, by, cy})));
    int iy1 = std::min(g_h, (int)ceilf(std::max({ay, by, cy})));
    for (int y = iy0; y < iy1; y++) {
        for (int x = ix0; x < ix1; x++) {
            int hit = 0;
            for (int j = 0; j < 4; j++) {
                for (int i = 0; i < 4; i++) {
                    float sxp = x + (i + 0.5f) / 4.f, syp = y + (j + 0.5f) / 4.f;
                    float w0 = edge(bx, by, cx, cy, sxp, syp);
                    float w1 = edge(cx, cy, ax, ay, sxp, syp);
                    float w2 = edge(ax, ay, bx, by, sxp, syp);
                    bool in = area > 0 ? (w0 >= 0 && w1 >= 0 && w2 >= 0) : (w0 <= 0 && w1 <= 0 && w2 <= 0);
                    if (in) hit++;
                }
            }
            if (hit) BlendPx(px[(size_t)y * g_w + x], R, G, B, a * hit / 16);
        }
    }
}

// сердечко по кривой (x²+y²−1)³ = x²·y³, покрытие считается по подвыборкам 4x4 (без артефактов на краях).
// (cx,cy) — начало координат кривой (центр сердечка на 0.125·s выше), s — масштаб.
// k > 0 — вырезать такое же сердечко меньшего размера (k — его доля): получается контур; k = 0 — сплошное
static void DrawHeart(DWORD* px, float cx, float cy, float s, float k, unsigned R, unsigned G, unsigned B,
                      unsigned a, int clipT = -1000000, int clipB = 1000000) {
    if (s <= 0.f || !a) return;
    const float vc = 0.125f;
    auto inside = [&](float u, float v, float kk) {
        u /= kk;
        v = (v - vc) / kk + vc;
        const float A = u * u + v * v - 1.f;
        return A * A * A - u * u * v * v * v <= 0.f;
    };
    const int x0 = std::max(0, (int)floorf(cx - 1.4f * s)), x1 = std::min(g_w, (int)ceilf(cx + 1.4f * s));
    const int y0 = std::max(std::max(0, clipT), (int)floorf(cy - 1.5f * s));
    const int y1 = std::min(std::min(g_h, clipB), (int)ceilf(cy + 1.3f * s));
    const int N = 4;
    for (int y = y0; y < y1; y++) {
        for (int x = x0; x < x1; x++) {
            int hit = 0, hitIn = 0;
            for (int sy = 0; sy < N; sy++) {
                for (int sx = 0; sx < N; sx++) {
                    const float u = ((float)x + ((float)sx + 0.5f) / N - cx) / s;
                    const float v = (cy - ((float)y + ((float)sy + 0.5f) / N)) / s;
                    if (inside(u, v, 1.f)) {
                        hit++;
                        if (k > 0.f && inside(u, v, k)) hitIn++;
                    }
                }
            }
            const int c = hit - hitIn;
            if (c > 0) BlendPx(px[(size_t)y * g_w + x], R, G, B, (unsigned)(a * c / (N * N)));
        }
    }
}

// скруглённая плитка sz x sz с диагональным градиентом (по вертикали обрезается clipT..clipB)
static void DrawGradTile(DWORD* px, int x0, int y0, int sz, float rad, unsigned r1, unsigned g1, unsigned b1,
                         unsigned r2, unsigned g2, unsigned b2, float opacity, int clipT, int clipB) {
    if (sz <= 0 || opacity <= 0.f) return;
    for (int y = 0; y < sz; y++) {
        const int dy = y0 + y;
        if (dy < 0 || dy >= g_h || dy < clipT || dy >= clipB) continue;
        for (int x = 0; x < sz; x++) {
            const int dx = x0 + x;
            if (dx < 0 || dx >= g_w) continue;
            const float cov = RRCov(x + 0.5f, y + 0.5f, 0.f, 0.f, (float)sz, (float)sz, rad) * opacity;
            if (cov <= 0.f) continue;
            const float t = (float)(x + y) / (2.f * (float)sz);
            const unsigned R = (unsigned)(r1 + ((float)r2 - (float)r1) * t + 0.5f);
            const unsigned G = (unsigned)(g1 + ((float)g2 - (float)g1) * t + 0.5f);
            const unsigned B = (unsigned)(b1 + ((float)b2 - (float)b1) * t + 0.5f);
            BlendPx(px[(size_t)dy * g_w + dx], R, G, B, (unsigned)(255.f * cov + 0.5f));
        }
    }
}

// обложка cs x cs в точке (cx,cy) со скруглением, билинейным масштабированием и прозрачностью
static void DrawCover(DWORD* px, int cx, int cy, int cs, float rad, const std::vector<DWORD>& src, int sp,
                      float opacity, int clipT = -1000000, int clipB = 1000000) {
    if (cs <= 0 || sp <= 0 || src.empty() || opacity <= 0.f) return;
    const float sc = (float)sp / (float)cs;
    for (int y = 0; y < cs; y++) {
        int dy = cy + y;
        if (dy < 0 || dy >= g_h || dy < clipT || dy >= clipB) continue;
        for (int x = 0; x < cs; x++) {
            int dx = cx + x;
            if (dx < 0 || dx >= g_w) continue;
            float cov = RRCov(x + 0.5f, y + 0.5f, 0.f, 0.f, (float)cs, (float)cs, rad) * opacity;
            if (cov <= 0.f) continue;
            float u = (x + 0.5f) * sc - 0.5f, v = (y + 0.5f) * sc - 0.5f;
            int u0 = (int)floorf(u), v0 = (int)floorf(v);
            float fu = u - u0, fv = v - v0;
            auto at = [&](int xx, int yy) {
                xx = std::clamp(xx, 0, sp - 1);
                yy = std::clamp(yy, 0, sp - 1);
                return src[(size_t)yy * sp + xx];
            };
            DWORD p00 = at(u0, v0), p10 = at(u0 + 1, v0), p01 = at(u0, v0 + 1), p11 = at(u0 + 1, v0 + 1);
            float w00 = (1 - fu) * (1 - fv), w10 = fu * (1 - fv), w01 = (1 - fu) * fv, w11 = fu * fv;
            auto ch = [&](int sh) {
                return ((p00 >> sh) & 255) * w00 + ((p10 >> sh) & 255) * w10 + ((p01 >> sh) & 255) * w01 +
                       ((p11 >> sh) & 255) * w11;
            };
            float sA = ch(24) * cov, sR = ch(16) * cov, sG = ch(8) * cov, sB = ch(0) * cov;
            DWORD& d = px[(size_t)dy * g_w + dx];
            float ia = 1.f - sA / 255.f;
            unsigned oA = (unsigned)std::min(255.f, sA + ((d >> 24) & 255) * ia + 0.5f);
            unsigned oR = (unsigned)std::min(255.f, sR + ((d >> 16) & 255) * ia + 0.5f);
            unsigned oG = (unsigned)std::min(255.f, sG + ((d >> 8) & 255) * ia + 0.5f);
            unsigned oB = (unsigned)std::min(255.f, sB + (d & 255) * ia + 0.5f);
            d = (oA << 24) | (oR << 16) | (oG << 8) | oB;
        }
    }
}

static std::wstring FmtTime(long long ticks) {
    long long sec = std::max(0LL, ticks) / 10000000LL;
    long long h = sec / 3600, m = (sec / 60) % 60, s = sec % 60;
    std::wstring r;
    if (h > 0) r = std::to_wstring(h) + L":" + (m < 10 ? L"0" : L"") + std::to_wstring(m);
    else r = std::to_wstring(m);
    r += L":";
    if (s < 10) r += L"0";
    r += std::to_wstring(s);
    return r;
}

static void Render(HWND hwnd) {
    if (g_w <= 0 || g_h <= 0) return;

    const double pe = std::max(0.0, g_expT);  // пружина: может чуть выходить за 1 (перелёт)
    const double pc = std::clamp(g_expT, 0.0, 1.0);
    auto smooth = [](double t) {
        t = std::clamp(t, 0.0, 1.0);
        return t * t * (3.0 - 2.0 * t);
    };
    const bool showComp = pc < 0.3;  // компактное содержимое гаснет в начале раскрытия
    const double compFade = std::clamp(1.0 - pc / 0.3, 0.0, 1.0);
    const bool showExp = g_stageT > 0.0;
    auto stage = [&](double delay) { return smooth((g_stageT - delay) / 0.4); };  // поэтапное появление
    const double tt = smooth(g_textT);                    // появление текста при смене трека
    const int tdx = (int)((1.0 - tt) * S(10) + 0.5);      // …с лёгким выездом справа
    const Palette& P = g_palCur;
    const ExpLay L = CalcExpLay();
    auto U8 = [](float v) { return (unsigned)std::clamp((int)(v + 0.5f), 0, 255); };
    auto I255 = [](double v) { return (int)std::clamp(v * 255.0 + 0.5, 0.0, 255.0); };

    HDC screen = GetDC(nullptr);
    HDC mem = CreateCompatibleDC(screen);  // итоговая картинка
    HDC mm = CreateCompatibleDC(screen);   // маска текста: R/G/B-каналы = три цветовые группы

    BITMAPINFO bi = {};
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = g_w;
    bi.bmiHeader.biHeight = -g_h;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;
    DWORD* px = nullptr;
    DWORD* mk = nullptr;
    HBITMAP bmp = CreateDIBSection(screen, &bi, DIB_RGB_COLORS, (void**)&px, nullptr, 0);
    HBITMAP bmpM = CreateDIBSection(screen, &bi, DIB_RGB_COLORS, (void**)&mk, nullptr, 0);
    if (!bmp || !bmpM || !px || !mk) {
        if (bmp) DeleteObject(bmp);
        if (bmpM) DeleteObject(bmpM);
        DeleteDC(mem);
        DeleteDC(mm);
        ReleaseDC(nullptr, screen);
        return;
    }
    HGDIOBJ oldBmp = SelectObject(mem, bmp);
    HGDIOBJ oldBmpM = SelectObject(mm, bmpM);

    const size_t N = (size_t)g_w * g_h;
    for (size_t i = 0; i < N; i++) px[i] = 0x01000000;  // почти невидимо, но ловит мышь
    memset(mk, 0, N * 4);
    SetBkMode(mm, TRANSPARENT);

    std::vector<HFONT> fonts;
    auto MkFont = [&](int hpx, int weight, PCWSTR face) {
        HFONT f = CreateFontW(-hpx, 0, 0, 0, weight, 0, 0, 0, DEFAULT_CHARSET, 0, 0, ANTIALIASED_QUALITY, 0, face);
        fonts.push_back(f);
        return f;
    };

    // маска -> цвет: три группы (R, G, B) красятся каждая своим цветом, затем маска очищается
    auto Composite = [&](const unsigned col[3][3], unsigned fd) {
        for (size_t i = 0; i < N; i++) {
            DWORD m = mk[i];
            if (!(m & 0x00FFFFFF)) continue;
            for (int k = 0; k < 3; k++) {
                unsigned c = (m >> (16 - 8 * k)) & 255;
                if (!c) continue;
                unsigned a = (c * fd) >> 8;
                if (a) BlendPx(px[i], col[k][0], col[k][1], col[k][2], a);
            }
        }
        memset(mk, 0, N * 4);
    };

    // ---- фон «острова» с градиентом из палитры обложки
    if (pc > 0.01) {
        const float R = (float)S(24) * (float)std::min(pe, 1.0);
        const unsigned bgA = (unsigned)(255.0 * std::clamp(pc * 1.8, 0.0, 1.0) * g_set.bgOpacity / 100.0);
        for (int y = 0; y < g_h; y++) {
            for (int x = 0; x < g_w; x++) {
                float cov = RRCov(x + 0.5f, y + 0.5f, 0.f, 0.f, (float)g_w, (float)g_h, R);
                if (cov <= 0.f) continue;
                float t = ((float)x / g_w + (float)y / g_h) * 0.5f;
                BlendPx(px[(size_t)y * g_w + x], U8(P.bgTop.r + (P.bgBot.r - P.bgTop.r) * t),
                        U8(P.bgTop.g + (P.bgBot.g - P.bgTop.g) * t),
                        U8(P.bgTop.b + (P.bgBot.b - P.bgTop.b) * t), (unsigned)(bgA * cov + 0.5f));
            }
        }
    }

    // ---- обложка: переезжает из компактной позиции в развёрнутую; на паузе чуть уменьшается;
    //      при смене трека плавно перетекает в новую
    if (!g_cover.empty() && g_coverPx > 0) {
        const int csC = CoverCompactSize();
        const int csE0 = L.cover.right - L.cover.left;
        const int csE = (int)std::lround(csE0 * (0.9 + 0.1 * g_playT));
        const int cxC = g_oxC + S(8), cyC = g_oyC + (g_hc - csC) / 2;
        const int cxE = g_oxE + L.cover.left + (csE0 - csE) / 2, cyE = g_oyE + L.cover.top + (csE0 - csE) / 2;
        auto lerpI = [&](int a, int b) { return (int)std::lround(a + (b - a) * pe); };
        float rad = (float)S(6) + (float)(S(10) - S(6)) * (float)std::min(pe, 1.0);
        int cx = lerpI(cxC, cxE), cy = lerpI(cyC, cyE), cs = lerpI(csC, csE);
        const bool fading = !g_coverOld.empty() && g_coverFade < 1.0;
        if (fading) DrawCover(px, cx, cy, cs, rad, g_coverOld, g_coverOldPx, 1.f);
        DrawCover(px, cx, cy, cs, rad, g_cover, g_coverPx, fading ? (float)smooth(g_coverFade) : 1.f);
    }

    // ---- развёрнутый плеер: фигуры
    long long dur = g_durTicks;
    long long posShown = (g_drag == 1 && dur > 0) ? (long long)(g_dragFrac * (double)dur) : CurPos();
    const double sT = stage(0.10), sB = stage(0.20), sC = stage(0.30), sV = stage(0.40);
    // сдвиг по вертикали: элементы «подплывают» снизу
    auto T = [&](RECT r, double s) {
        OffsetRect(&r, g_oxE, g_oyE + (int)((1.0 - s) * S(10) + 0.5));
        return r;
    };
    if (showExp) {
        const unsigned tR = U8(P.text.r), tG = U8(P.text.g), tB = U8(P.text.b);
        const unsigned aR = U8(P.accent.r), aG = U8(P.accent.g), aB = U8(P.accent.b);

        if (g_cover.empty()) {  // заглушка вместо обложки
            RECT c = T(L.cover, sT);
            FillRR(px, (float)c.left, (float)c.top, (float)c.right, (float)c.bottom, (float)S(10), tR, tG, tB,
                   I255(sT) * 40 / 255);
        }

        // имя исполнителя под курсором подчёркивается — по нему можно открыть профиль
        if (OwnMode() && g_haveCur && g_hotT[15] > 0.01 && g_artHover >= 0) {
            std::vector<std::pair<int, int>> sp;
            ArtistSpans(L, sp);
            if (g_artHover < (int)sp.size() && sp[(size_t)g_artHover].second > sp[(size_t)g_artHover].first) {
                const RECT ra2 = T(L.artist, sT);
                const float uy = (float)ra2.bottom - (float)S(2);
                FillRR(px, (float)(g_oxE + sp[(size_t)g_artHover].first), uy,
                       (float)(g_oxE + sp[(size_t)g_artHover].second), uy + (float)std::max(1, S(1)), 0.f, tR, tG, tB,
                       (unsigned)(200.0 * g_hotT[15] * sT));
            }
        }

        // плашка «Моя волна» (или «Волна по треку»): понятно, что сейчас играет именно волна
        if (SourcePillOn() && L.wavePill.right > L.wavePill.left) {
            const RECT wp = T(L.wavePill, sT);
            const float ph = (float)(wp.bottom - wp.top);
            FillRR(px, (float)wp.left, (float)wp.top, (float)wp.right, (float)wp.bottom, ph / 2.f, aR, aG, aB,
                   (unsigned)(I255(sT) * 0.34));
            // три «столбика эквалайзера»; на паузе — одинаковой высоты
            const float bw = (float)std::max(2, S(2)), bg = (float)S(2), bx0 = (float)wp.left + (float)S(9);
            const float hs[3] = {0.45f, 0.85f, 0.6f};
            const float maxH = ph * 0.55f, cyb = ph * 0.5f + (float)wp.top;
            for (int i = 0; i < 3; i++) {
                const float k = g_playing ? hs[i] : 0.5f;
                const float bh = maxH * k;
                FillRR(px, bx0 + i * (bw + bg), cyb - bh / 2.f, bx0 + i * (bw + bg) + bw, cyb + bh / 2.f, bw / 2.f,
                       tR, tG, tB, (unsigned)(I255(sT) * 0.9));
            }
        }

        // шкала воспроизведения: при наведении/перетаскивании утолщается, ползунок растёт
        {
            const unsigned fa = I255(sB);
            RECT b = T(L.bar, sB);
            const float hot = (float)g_hotT[5];
            float cy = (b.top + b.bottom) * 0.5f;
            float hh = (b.bottom - b.top) * 0.5f * (1.f + 0.7f * hot);
            float rr = hh;
            FillRR(px, (float)b.left, cy - hh, (float)b.right, cy + hh, rr, tR, tG, tB, fa * 55 / 255);
            if (dur > 0) {
                double frac = std::clamp((double)posShown / (double)dur, 0.0, 1.0);
                float fx = b.left + (b.right - b.left) * (float)frac;
                FillRR(px, (float)b.left, cy - hh, fx, cy + hh, rr, aR, aG, aB, fa);
                float kr = (float)S(4) + (float)(S(7) - S(4)) * hot;
                FillRR(px, fx - kr, cy - kr, fx + kr, cy + kr, kr, tR, tG, tB, fa);
            }
        }

        // громкость приложения
        {
            const unsigned fa = I255(sV);
            RECT b = T(L.volBar, sV);
            const float hot = (float)g_hotT[6];
            float cy = (b.top + b.bottom) * 0.5f;
            float hh = (b.bottom - b.top) * 0.5f * (1.f + 0.7f * hot);
            FillRR(px, (float)b.left, cy - hh, (float)b.right, cy + hh, hh, tR, tG, tB, fa * 55 / 255);
            float vv = g_muted ? 0.f : std::clamp(g_vol, 0.f, 1.f);
            float fx = b.left + (b.right - b.left) * vv;
            FillRR(px, (float)b.left, cy - hh, fx, cy + hh, hh, tR, tG, tB, fa);
            float kr = (float)S(4) + (float)(S(7) - S(4)) * hot;
            FillRR(px, fx - kr, cy - kr, fx + kr, cy + kr, kr, tR, tG, tB, fa);

            // мягкая подсветка значка громкости при наведении
            RECT vi = T(L.volIcon, sV);
            float vcx = (vi.left + vi.right) * 0.5f, vcy = (vi.top + vi.bottom) * 0.5f;
            float vr = (vi.bottom - vi.top) * 0.5f;
            float va = (float)(g_hotT[4] * 0.12 + g_pressT[4] * 0.12);
            FillRR(px, vcx - vr, vcy - vr, vcx + vr, vcy + vr, vr, tR, tG, tB, (unsigned)(fa * va));
        }

        // кнопки управления: подсветка при наведении, «пружинка» при нажатии
        {
            const unsigned fa = I255(sC);
            for (int i = 1; i <= 3; i += 2) {  // prev, next
                RECT r = T(i == 1 ? L.prev : L.next, sC);
                float cx = (r.left + r.right) * 0.5f, cy = (r.top + r.bottom) * 0.5f;
                float rad = (r.right - r.left) * 0.5f * (1.f - 0.08f * (float)g_pressT[i]);
                float a = (float)(g_hotT[i] * 0.12 + g_pressT[i] * 0.14);
                FillRR(px, cx - rad, cy - rad, cx + rad, cy + rad, rad, tR, tG, tB, (unsigned)(fa * a));
            }
            // play/pause: круг чуть растёт при наведении и сжимается при нажатии
            RECT p = T(L.play, sC);
            float cx = (p.left + p.right) * 0.5f, cy = (p.top + p.bottom) * 0.5f;
            float base = (p.right - p.left) * 0.5f;
            float rad = base * (1.f + 0.07f * (float)g_hotT[2] - 0.10f * (float)g_pressT[2]);
            FillRR(px, cx - rad, cy - rad, cx + rad, cy + rad, rad, tR, tG, tB, fa);
            unsigned gR = U8(P.bgBot.r), gG = U8(P.bgBot.g), gB = U8(P.bgBot.b);
            float h = rad * 0.46f;
            float pl = (float)g_playT;  // 1 — пауза-значок (играет), 0 — треугольник
            if (pl > 0.02f) {           // две полоски; при смене плавно появляются/уменьшаются
                float hp = h * (0.6f + 0.4f * pl);
                float w = hp * 0.62f, gap = hp * 0.55f;
                unsigned a = (unsigned)(fa * pl);
                FillRR(px, cx - gap / 2 - w, cy - hp, cx - gap / 2, cy + hp, 1.5f, gR, gG, gB, a);
                FillRR(px, cx + gap / 2, cy - hp, cx + gap / 2 + w, cy + hp, 1.5f, gR, gG, gB, a);
            }
            if (pl < 0.98f) {
                float tp = 1.f - pl;
                float ht = h * (0.6f + 0.4f * tp);
                FillTri(px, cx - ht * 0.55f, cy - ht, cx - ht * 0.55f, cy + ht, cx + ht * 0.95f, cy, gR, gG, gB,
                        (unsigned)(fa * tp));
            }
        }
    }

    // ---- панель текста песни (фон) и кнопка её открытия
    const double lyGeo = Smooth01(g_lyT);
    const double libGeo = Smooth01(g_libT);
    // содержимое двух панелей не рисуется одновременно: открытая «забирает» место, закрывающаяся гаснет
    const double lyFade = Smooth01((g_lyT - 0.25) / 0.75) * stage(0.10) * (g_libOpen ? 0.0 : 1.0);
    const double libFade =
        OwnMode() ? Smooth01((g_libT - 0.25) / 0.75) * stage(0.10) * (g_lyOpen ? 0.0 : 1.0) : 0.0;
    const double bgFade = std::max(Smooth01((g_lyT - 0.25) / 0.75), Smooth01((g_libT - 0.25) / 0.75)) * stage(0.10);
    RECT lp = L.lyrics;
    OffsetRect(&lp, g_oxE, g_oyE);
    const bool panelVisible = showExp && (lp.bottom - lp.top) > 2;
    const bool lyVisible = panelVisible && !g_libOpen;
    const bool libOn = panelVisible && libFade > 0.003;
    if (showExp) {
        const unsigned tR = U8(P.text.r), tG = U8(P.text.g), tB = U8(P.text.b);
        if (panelVisible)
            FillRR(px, (float)lp.left, (float)lp.top, (float)lp.right, (float)lp.bottom, (float)S(14), 0, 0, 0,
                   (unsigned)I255(0.24 * bgFade));
        const unsigned fa = I255(stage(0.30));
        RECT b = T(L.lyBtn, stage(0.30));
        float cx = (b.left + b.right) * 0.5f, cy = (b.top + b.bottom) * 0.5f;
        float a = 0.06f + (float)(g_hotT[7] * 0.10 + g_pressT[5] * 0.12) + (float)lyGeo * 0.16f;
        FillRR(px, (float)b.left, (float)b.top, (float)b.right, (float)b.bottom, (float)S(10), tR, tG, tB,
               (unsigned)(fa * std::min(a, 1.f)));
        if (lyVisible && L.lyChk.right > L.lyChk.left) {  // подсветка кнопки «i»
            RECT c = L.lyChk;
            OffsetRect(&c, g_oxE, g_oyE);
            float ccx = (c.left + c.right) * 0.5f, ccy = (c.top + c.bottom) * 0.5f, cr = (c.right - c.left) * 0.5f;
            float ca = (float)(g_hotT[8] * 0.16) + (g_diagShow ? 0.14f : 0.f);
            FillRR(px, ccx - cr, ccy - cr, ccx + cr, ccy + cr, cr, tR, tG, tB, (unsigned)(I255(lyFade) * ca));
        }
        Rgb ic = MixRgb(P.text, P.accent, (float)lyGeo);
        float lw = (float)S(16), th = std::max(1.5f, (float)S(2)), sp = (float)S(5);
        float x0 = cx - lw * 0.5f;
        const float ws[3] = {1.0f, 0.62f, 0.84f};
        for (int k = 0; k < 3; k++) {
            float yy = cy + (k - 1) * sp;
            FillRR(px, x0, yy - th * 0.5f, x0 + lw * ws[k], yy + th * 0.5f, th * 0.5f, U8(ic.r), U8(ic.g), U8(ic.b), fa);
        }

        if (L.libBtn.right > L.libBtn.left) {  // кнопка библиотеки: фон подсвечивается при наведении и пока панель открыта
            RECT b2 = T(L.libBtn, stage(0.30));
            float a2 = 0.06f + (float)(g_hotT[9] * 0.10 + g_pressT[6] * 0.12) + (float)libGeo * 0.16f;
            FillRR(px, (float)b2.left, (float)b2.top, (float)b2.right, (float)b2.bottom, (float)S(10), tR, tG, tB,
                   (unsigned)(fa * std::min(a2, 1.f)));
        }
        if (L.like.right > L.like.left) {  // сердечко «нравится»: два круга и треугольник одним цветом
            RECT b3 = T(L.like, sT);
            const unsigned fa3 = (unsigned)I255(sT);
            const float cx3 = (b3.left + b3.right) * 0.5f, cy3 = (b3.top + b3.bottom) * 0.5f;
            const float r3 = (b3.right - b3.left) * 0.5f;
            const float ha = (float)(g_hotT[10] * 0.07 + g_pressT[7] * 0.09);
            FillRR(px, cx3 - r3, cy3 - r3, cx3 + r3, cy3 + r3, r3, tR, tG, tB, (unsigned)(fa3 * ha));
            const bool liked = g_haveCur && IsLiked(g_cur.id);
            const float w = r3 * 0.5f * (1.f - 0.08f * (float)g_pressT[7]);
            if (liked) {  // лайкнут — сердечко залито цветом акцента
                const float rr = w * 0.54f, hy = cy3 - w * 0.06f;
                const unsigned hR = U8(P.accent.r), hG = U8(P.accent.g), hB = U8(P.accent.b);
                FillRR(px, cx3 - w * 0.5f - rr, hy - w * 0.28f - rr, cx3 - w * 0.5f + rr, hy - w * 0.28f + rr, rr, hR, hG, hB, fa3);
                FillRR(px, cx3 + w * 0.5f - rr, hy - w * 0.28f - rr, cx3 + w * 0.5f + rr, hy - w * 0.28f + rr, rr, hR, hG, hB, fa3);
                FillTri(px, cx3 - w, hy - w * 0.12f, cx3 + w, hy - w * 0.12f, cx3, hy + w * 0.95f, hR, hG, hB, fa3);
            } else {  // не лайкнут — пустое сердечко (только контур)
                const float sc = w * 0.9f;
                const float oy = cy3 + 0.125f * sc;  // центр сердечка — в центре кнопки
                DrawHeart(px, cx3, oy, sc, 0.70f, tR, tG, tB, (unsigned)(fa3 * 0.95f));
            }
        }
    }

    // ---- библиотека (поиск и плейлисты): фигуры
    LibLay LY = {};
    if (libOn) {
        LY = CalcLibLay(lp, L.libFull);
        const unsigned tR = U8(P.text.r), tG = U8(P.text.g), tB = U8(P.text.b);
        const unsigned aR = U8(P.accent.r), aG = U8(P.accent.g), aB = U8(P.accent.b);
        const unsigned fa = (unsigned)I255(libFade);
        // всё, что выходит за пределы растущей панели, обрезается по вертикали
        auto Box = [&](const RECT& r0, float rad, unsigned R, unsigned G, unsigned B, double alpha) {
            RECT r = r0;
            r.top = std::max(r.top, lp.top);
            r.bottom = std::min(r.bottom, lp.bottom);
            if (r.bottom <= r.top) return;
            FillRR(px, (float)r.left, (float)r.top, (float)r.right, (float)r.bottom, rad, R, G, B,
                   (unsigned)(fa * std::clamp(alpha, 0.0, 1.0)));
        };
        Box(LY.search, (float)S(10), tR, tG, tB, 0.10 + 0.08 * g_hotT[11] + (g_searchActive ? 0.10 : 0.0));
        Box(LY.wave, (float)S(10), aR, aG, aB,
            0.30 + 0.22 * g_hotT[12] + (g_pmode == PM_WAVE && g_waveSeed.empty() ? 0.30 : 0.0));
        Box(LY.tune, (float)S(10), tR, tG, tB, 0.10 + 0.08 * g_hotT[16] + (g_libMode == 3 ? 0.18 : 0.0));
        Box(LY.shake, (float)S(10), tR, tG, tB, 0.10 + 0.08 * g_hotT[17]);
        Box(LY.tabA, (float)S(8), tR, tG, tB, 0.07 + 0.10 * g_hotT[13] + (g_libMode == 0 ? 0.12 : 0.0));
        if (g_libMode == 2) Box(LY.tabB, (float)S(8), aR, aG, aB, 0.30 + 0.22 * g_hotT[14]);
        else Box(LY.tabB, (float)S(8), tR, tG, tB, 0.07 + 0.10 * g_hotT[14] + (g_libMode == 1 ? 0.12 : 0.0));

        g_libScroll = std::clamp(g_libScroll, 0.0, LibMaxScroll(LY));
        RECT lc = LY.list;
        lc.top = std::max(lc.top, lp.top);
        lc.bottom = std::min(lc.bottom, lp.bottom);
        if (lc.bottom > lc.top && g_libMode == 3) {  // настройки «Моей волны»: «чипы» опций
            const WaveLay WL = CalcWaveLay(LY);
            const int ox = (int)LY.list.left, oy = (int)LY.list.top - (int)std::lround(g_libScroll);
            for (size_t i = 0; i < WL.chips.size(); i++) {
                RECT r = WL.chips[i].r;
                OffsetRect(&r, ox, oy);
                if (r.bottom <= lc.top || r.top >= lc.bottom) continue;
                const bool sel = g_wCfg[WL.chips[i].g] == kWaveGroups[WL.chips[i].g].opts[WL.chips[i].o].id;
                const bool hov = (int)i == g_libHover;
                const double a = sel ? 0.34 + (hov ? 0.08 : 0.0) : 0.08 + (hov ? 0.10 : 0.0);
                FillRR(px, (float)r.left, (float)std::max(r.top, lc.top), (float)r.right,
                       (float)std::min(r.bottom, lc.bottom), (float)(r.bottom - r.top) / 2.f, sel ? aR : tR,
                       sel ? aG : tG, sel ? aB : tB, (unsigned)(fa * a));
            }
            const double ms = LibMaxScroll(LY);
            if (ms > 0.0) {
                const int lh = LY.list.bottom - LY.list.top;
                const int th = std::max(S(20), (int)(lh * (double)lh / (lh + ms)));
                const int ty = LY.list.top + (int)((lh - th) * g_libScroll / ms);
                RECT sb = {LY.list.right - S(3), ty, LY.list.right - S(1), ty + th};
                Box(sb, (float)S(1), tR, tG, tB, 0.30);
            }
        }
        if (lc.bottom > lc.top && g_libMode != 3 && LibMessage().empty()) {
            const int n = LibRowCount();
            const int first = std::max(0, (int)floor(g_libScroll / (double)LY.rowH));
            for (int i = first; i < n; i++) {
                const int rt = LY.list.top + i * LY.rowH - (int)std::lround(g_libScroll);
                if (rt >= lc.bottom) break;
                const int rb = rt + LY.rowH;
                if (rb <= lc.top) continue;
                const YmTrack* rowTr = LibRowTrack(i);
                const bool cur = g_haveCur && rowTr && rowTr->id == g_cur.id;
                const double a = (g_libHover == i ? 0.10 : 0.0) + (cur ? 0.16 : 0.0);
                if (a > 0.0) {
                    RECT rr = {LY.list.left, std::max((LONG)rt, lc.top), LY.list.right, std::min((LONG)rb, lc.bottom)};
                    FillRR(px, (float)rr.left, (float)rr.top, (float)rr.right, (float)rr.bottom, (float)S(8),
                           cur ? aR : tR, cur ? aG : tG, cur ? aB : tB, (unsigned)(fa * a));
                }
                {   // миниатюра обложки (пока не загрузилась — тусклая плитка с значком)
                    const int ts = ThumbSize();
                    const float thRad = LibRowArtist(i) ? (float)ts / 2.f : (float)S(6);  // исполнители — круглые
                    const int tx0 = LY.list.left + S(6), ty0 = rt + (LY.rowH - ts) / 2;
                    if (const Thumb* th = LibThumb(i)) {
                        DrawCover(px, tx0, ty0, ts, thRad, th->px, th->sz,
                                  std::clamp((float)libFade, 0.f, 1.f), (int)lc.top, (int)lc.bottom);
                    } else if (g_libMode == 0 && g_pls[(size_t)i].kind == "liked") {
                        const float op = std::clamp((float)libFade, 0.f, 1.f);
                        DrawGradTile(px, tx0, ty0, ts, thRad, 255, 94, 138, 124, 92, 255, op, (int)lc.top, (int)lc.bottom);
                        const float hs = (float)ts * 0.21f;  // белое сердечко по центру плитки
                        DrawHeart(px, tx0 + ts * 0.5f, ty0 + ts * 0.5f + 0.125f * hs, hs, 0.f, 255, 255, 255, fa,
                                  (int)lc.top, (int)lc.bottom);
                    } else {
                        RECT tr = {tx0, std::max((LONG)ty0, lc.top), tx0 + ts, std::min((LONG)(ty0 + ts), lc.bottom)};
                        if (tr.bottom > tr.top)
                            FillRR(px, (float)tr.left, (float)tr.top, (float)tr.right, (float)tr.bottom, thRad,
                                   tR, tG, tB, (unsigned)(fa * 0.10));
                    }
                }
            }
            const double ms = LibMaxScroll(LY);  // тонкая полоса прокрутки справа
            if (ms > 0.0) {
                const int lh = LY.list.bottom - LY.list.top;
                const int th = std::max(S(20), (int)(lh * (double)lh / (lh + ms)));
                const int ty = LY.list.top + (int)((lh - th) * g_libScroll / ms);
                RECT sb = {LY.list.right - S(3), ty, LY.list.right - S(1), ty + th};
                Box(sb, (float)S(1), tR, tG, tB, 0.30);
            }
        }
    }

    // ---- проход 1: компактный режим (R — заголовок, G — исполнитель, B — кнопки)
    if (showComp) {
        HFONT fTitle = MkFont(S(g_set.fontSize), FW_SEMIBOLD, L"Segoe UI");
        HFONT fArtist = MkFont(S(g_set.fontSize - 2), FW_NORMAL, L"Segoe UI");
        HFONT fIcon = MkFont(S(16), FW_NORMAL, L"Segoe MDL2 Assets");

        const int pad = S(8);
        const int gap = S(10);  // расстояние между текстом и кнопками
        const int lineH1 = S(g_set.fontSize) + S(6);
        const int lineH2 = S(g_set.fontSize - 2) + S(4);
        const int top = g_oyC + (g_hc - (lineH1 + lineH2)) / 2;
        const int textLeft = g_oxC + (g_cover.empty() ? pad : pad + CoverCompactSize() + S(8));

        auto measure = [&](HFONT f, const std::wstring& t) {
            HGDIOBJ old = SelectObject(mm, f);
            SIZE sz = {};
            GetTextExtentPoint32W(mm, t.c_str(), (int)t.size(), &sz);
            SelectObject(mm, old);
            return (int)sz.cx;
        };
        const std::wstring artistLine = (SourcePillOn() && g_haveCur)
                                            ? WavePillLabel() + L" \u00B7 " + g_artist
                                            : g_artist;
        int textW = std::max(measure(fTitle, g_title), measure(fArtist, artistLine));

        // блок кнопок «приклеен» к тексту, но не выходит за край компактного виджета
        int maxTextW = std::max(0, g_oxC + g_wc - pad - textLeft - 3 * CellW() - gap);
        int shownW = std::min(textW, maxTextW);
        g_btnLeft = textLeft + shownW + gap;

        double eh = EaseOutCubic(g_animT);
        int fullRight = std::min(g_oxC + g_wc - pad, textLeft + textW);
        int textRight = (int)(fullRight + (g_btnLeft - gap - fullRight) * eh + 0.5);
        textRight = std::max(textRight, textLeft);

        RECT r1 = {textLeft + tdx, top, textRight + tdx, top + lineH1};
        RECT r2 = {textLeft + tdx, top + lineH1, textRight + tdx, top + lineH1 + lineH2};
        DrawTextRect(mm, fTitle, RGB(I255(tt), 0, 0), g_title.c_str(), r1, DT_END_ELLIPSIS | DT_VCENTER);
        DrawTextRect(mm, fArtist, RGB(0, I255(tt * 170.0 / 255.0), 0), artistLine.c_str(), r2,
                     DT_END_ELLIPSIS | DT_VCENTER);

        // кнопки: fade-in + выезд слева направо, с небольшой задержкой для каждой
        if (g_animT > 0.0) {
            PCWSTR glyphs[3] = {L"\uE100", g_playing ? L"\uE103" : L"\uE102", L"\uE101"};
            for (int i = 0; i < 3; i++) {
                double t = EaseOutCubic((g_animT - i * 0.12) / 0.64);
                if (t <= 0.0) continue;
                int shift = (int)((1.0 - t) * S(10) + 0.5);
                int v = (int)(255 * t + 0.5);
                RECT rc = {g_btnLeft + i * CellW() - shift, g_oyC, g_btnLeft + (i + 1) * CellW() - shift,
                           g_oyC + g_hc};
                // мягкая круглая подсветка кнопки под курсором
                const double hv = g_hotT[i + 1] * t * compFade;
                if (hv > 0.01) {
                    float ccx = (rc.left + rc.right) * 0.5f, ccy = (rc.top + rc.bottom) * 0.5f;
                    float hr = (float)std::min(CellW() / 2 - S(3), g_hc / 2 - S(2)) * (0.9f + 0.1f * (float)g_hotT[i + 1]);
                    FillRR(px, ccx - hr, ccy - hr, ccx + hr, ccy + hr, hr, g_set.r, g_set.g, g_set.b,
                           (unsigned)(255.0 * 0.18 * hv + 0.5));
                }
                DrawTextRect(mm, fIcon, RGB(0, 0, v), glyphs[i], rc, DT_CENTER | DT_VCENTER);
            }
        }
        unsigned col[3][3];
        for (int k = 0; k < 3; k++) {
            col[k][0] = g_set.r;
            col[k][1] = g_set.g;
            col[k][2] = g_set.b;
        }
        Composite(col, (unsigned)(compFade * 256.0));
    }

    // ---- проход 2: развёрнутый плеер; у каждого элемента своя степень появления (яркость цвета в маске)
    if (showExp) {
        HFONT fT = MkFont(S(g_set.fontSize + 2), FW_SEMIBOLD, L"Segoe UI");
        HFONT fA = MkFont(S(g_set.fontSize), FW_NORMAL, L"Segoe UI");
        HFONT fTm = MkFont(S(std::max(9, g_set.fontSize - 3)), FW_NORMAL, L"Segoe UI");
        HFONT fIc = MkFont(S(18), FW_NORMAL, L"Segoe MDL2 Assets");
        HFONT fVi = MkFont(S(16), FW_NORMAL, L"Segoe MDL2 Assets");

        RECT rt = T(L.title, sT), ra = T(L.artist, sT);
        OffsetRect(&rt, tdx, 0);
        OffsetRect(&ra, tdx, 0);
        DrawTextRect(mm, fT, RGB(I255(sT * tt), 0, 0), g_title.c_str(), rt, DT_END_ELLIPSIS | DT_VCENTER);
        DrawTextRect(mm, fA, RGB(0, I255(sT * tt), 0), g_artist.c_str(), ra, DT_END_ELLIPSIS | DT_VCENTER);
        if (SourcePillOn() && L.wavePill.right > L.wavePill.left) {
            RECT wr = T(L.wavePill, sT);
            wr.left += S(24);
            DrawTextRect(mm, fTm, RGB(I255(sT), 0, 0), WavePillLabel().c_str(), wr, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
        }
        DrawTextRect(mm, fTm, RGB(0, I255(sB), 0), FmtTime(posShown).c_str(), T(L.tCur, sB), DT_LEFT | DT_VCENTER);
        if (dur > 0)
            DrawTextRect(mm, fTm, RGB(0, I255(sB), 0), FmtTime(dur).c_str(), T(L.tTot, sB), DT_RIGHT | DT_VCENTER);
        DrawTextRect(mm, fIc, RGB(0, 0, I255(sC)), L"\uE100", T(L.prev, sC), DT_CENTER | DT_VCENTER);
        DrawTextRect(mm, fIc, RGB(0, 0, I255(sC)), L"\uE101", T(L.next, sC), DT_CENTER | DT_VCENTER);
        DrawTextRect(mm, fVi, RGB(0, 0, I255(sV)), (g_muted || g_vol <= 0.001f) ? L"\uE74F" : L"\uE767",
                     T(L.volIcon, sV), DT_CENTER | DT_VCENTER);
        if (L.libBtn.right > L.libBtn.left)
            DrawTextRect(mm, fIc, RGB(0, 0, I255(sC)), L"\uE8F1", T(L.libBtn, sC), DT_CENTER | DT_VCENTER);

        if (lyVisible) {  // строки текста: яркость зависит от близости к текущей строке
            HFONT fL = MkFont(S(g_set.fontSize + 3), FW_SEMIBOLD, L"Segoe UI");
            const int padX = S(14);
            const int tw = std::max(10, (int)(lp.right - lp.left) - 2 * padX);
            HRGN rg = CreateRectRgn(lp.left, lp.top, lp.right, lp.bottom);
            SelectClipRgn(mm, rg);
            if (g_diagShow) {  // отчёт проверки источников
                const std::wstring t = L"Проверка источников\n" + g_diagText + L"\n(значок «i» ещё раз — закрыть)";
                HGDIOBJ oldF = SelectObject(mm, fTm);
                SetTextColor(mm, RGB(I255(lyFade), 0, 0));
                RECT rc = {lp.left + padX, lp.top + S(10), lp.right - padX - S(26), lp.bottom - S(6)};
                DrawTextW(mm, t.c_str(), -1, &rc, DT_WORDBREAK | DT_NOPREFIX | DT_LEFT);
                SelectObject(mm, oldF);
            } else if (g_lyState == 2 && !g_lyrics.empty()) {
                const int n = (int)g_lyrics.size();
                const int fpx = S(g_set.fontSize + 3);
                const int lineGap = S(10);
                HGDIOBJ oldF = SelectObject(mm, fL);
                if (g_lyHW != tw || g_lyHF != fpx || (int)g_lyH.size() != n) {
                    g_lyH.assign(n, 0);
                    for (int i = 0; i < n; i++) {
                        if (g_lyrics[i].text.empty()) {
                            g_lyH[i] = S(g_set.fontSize);
                            continue;
                        }
                        RECT mr = {0, 0, tw, 0};
                        DrawTextW(mm, g_lyrics[i].text.c_str(), -1, &mr,
                                  DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX | DT_LEFT);
                        g_lyH[i] = (mr.bottom - mr.top) + lineGap;
                    }
                    g_lyHW = tw;
                    g_lyHF = fpx;
                }
                std::vector<int> yv(n + 1, 0);
                for (int i = 0; i < n; i++) yv[i + 1] = yv[i] + g_lyH[i];
                // сноска «Источник: …» — в самом конце текста, мелким шрифтом
                const std::wstring srcNote = g_lySource.empty() ? std::wstring() : L"Источник: " + g_lySource;
                const int noteGap = S(8);
                int noteH = 0;
                if (!srcNote.empty()) {
                    RECT mr = {0, 0, tw, 0};
                    HGDIOBJ oldN = SelectObject(mm, fTm);
                    DrawTextW(mm, srcNote.c_str(), -1, &mr, DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX | DT_LEFT);
                    SelectObject(mm, oldN);
                    noteH = (mr.bottom - mr.top) + noteGap;
                }
                const double f = std::clamp(g_lyScroll, 0.0, (double)(n - 1));
                int base;
                if (g_lySynced) {  // текущая строка — по центру панели
                    int i0 = (int)floor(f);
                    double fr = f - i0;
                    double c0 = yv[i0] + (g_lyH[i0] - lineGap) * 0.5;
                    double c1 = i0 + 1 < n ? yv[i0 + 1] + (g_lyH[i0 + 1] - lineGap) * 0.5 : c0;
                    double cf = c0 + (c1 - c0) * fr;
                    // ручная прокрутка не выходит за пределы текста: сверху — первая строка не выше центра панели,
                    // снизу — конец текста (со сноской) не выше нижнего края панели
                    const double panelMid = lp.bottom - L.lyFull / 2;
                    const double maxM = std::max(0.0, cf - (g_lyH[0] - lineGap) * 0.5);
                    const double minM = std::min(0.0, (lp.bottom - S(20)) - (panelMid - cf + yv[n] + noteH));
                    g_lyManual = std::clamp(g_lyManual, minM, maxM);
                    base = (int)std::lround(panelMid - cf + g_lyManual);
                } else {  // обычный текст: с начала, колесо — прокрутка
                    double minM = std::min(0.0, (double)L.lyFull - yv[n] - noteH - S(20));
                    g_lyManual = std::clamp(g_lyManual, minM, 0.0);
                    base = lp.bottom - L.lyFull + S(10) + (int)std::lround(g_lyManual);
                }
                g_lyPosY.assign(n, 0);
                for (int i = 0; i < n; i++) {
                    int top = base + yv[i];
                    int h = g_lyH[i];
                    g_lyPosY[i] = top;
                    if (top + h < lp.top || top > lp.bottom || g_lyrics[i].text.empty()) continue;
                    double inten = g_lySynced ? 0.36 + 0.64 * std::max(0.0, 1.0 - fabs(i - f)) : 0.92;
                    double cyl = top + (h - lineGap) * 0.5;
                    double fade = std::clamp(std::min(cyl - lp.top, lp.bottom - cyl) / (double)S(34), 0.0, 1.0);
                    int a = I255(inten * fade * lyFade);
                    if (a <= 0) continue;
                    RECT rc = {lp.left + padX, top, lp.right - padX, top + h};
                    SetTextColor(mm, RGB(a, 0, 0));
                    DrawTextW(mm, g_lyrics[i].text.c_str(), -1, &rc, DT_WORDBREAK | DT_NOPREFIX | DT_LEFT);
                }
                if (noteH > 0) {
                    const int ntop = base + yv[n] + noteGap;
                    const double cyn = ntop + (noteH - noteGap) * 0.5;
                    const double fadeN = std::clamp(std::min(cyn - lp.top, lp.bottom - cyn) / (double)S(34), 0.0, 1.0);
                    const int an = I255(0.45 * fadeN * lyFade);
                    if (an > 0 && ntop < lp.bottom && ntop + noteH > lp.top) {
                        RECT rn = {lp.left + padX, ntop, lp.right - padX, ntop + noteH - noteGap};
                        SelectObject(mm, fTm);
                        SetTextColor(mm, RGB(an, 0, 0));
                        DrawTextW(mm, srcNote.c_str(), -1, &rn, DT_WORDBREAK | DT_NOPREFIX | DT_LEFT);
                    }
                }
                SelectObject(mm, oldF);
            } else {
                const wchar_t* msg = g_lyState == 1   ? L"Загружаю текст…"
                                     : g_lyState == 4 ? L"Инструментал"
                                                      : L"Текст не найден";
                if (g_lyState == 3 && !g_lyDiag.empty()) {  // заголовок + причина по каждому источнику
                    const int avail = std::max(10, (int)(lp.right - lp.left) - 2 * padX - S(30));
                    HGDIOBJ oldF = SelectObject(mm, fTm);
                    RECT mr = {0, 0, avail, 0};
                    DrawTextW(mm, g_lyDiag.c_str(), -1, &mr, DT_CALCRECT | DT_WORDBREAK | DT_NOPREFIX | DT_CENTER);
                    SelectObject(mm, oldF);
                    const int dh = mr.bottom - mr.top, hh = S(g_set.fontSize) + S(8);
                    const int y0 = lp.top + std::max(S(4), (L.lyFull - hh - dh) / 2);
                    RECT r1 = {lp.left + padX, y0, lp.right - padX, y0 + hh};
                    DrawTextRect(mm, fA, RGB(0, I255(lyFade), 0), msg, r1, DT_CENTER | DT_VCENTER);
                    RECT r2 = {lp.left + padX + S(15), y0 + hh, lp.right - padX - S(15), y0 + hh + dh};
                    oldF = SelectObject(mm, fTm);
                    SetTextColor(mm, RGB(0, I255(lyFade * 0.75), 0));
                    DrawTextW(mm, g_lyDiag.c_str(), -1, &r2, DT_WORDBREAK | DT_NOPREFIX | DT_CENTER);
                    SelectObject(mm, oldF);
                } else {
                    DrawTextRect(mm, fA, RGB(0, I255(lyFade), 0), msg, lp, DT_CENTER | DT_VCENTER);
                }
            }
            SelectClipRgn(mm, nullptr);
            DeleteObject(rg);
            if (L.lyChk.right > L.lyChk.left) {  // значок «i» — проверка источников
                RECT c = L.lyChk;
                OffsetRect(&c, g_oxE, g_oyE);
                DrawTextRect(mm, fVi, RGB(0, 0, I255(lyFade * (0.55 + 0.45 * g_hotT[8]))), L"\uE946", c,
                             DT_CENTER | DT_VCENTER);
            }
        }

        if (libOn) {  // библиотека: поиск, «Моя волна», вкладки, список
            HFONT fRT = MkFont(S(std::max(10, g_set.fontSize - 1)), FW_SEMIBOLD, L"Segoe UI");
            HFONT fRA = MkFont(S(std::max(9, g_set.fontSize - 3)), FW_NORMAL, L"Segoe UI");
            HFONT fTab = MkFont(S(std::max(9, g_set.fontSize - 2)), FW_SEMIBOLD, L"Segoe UI");
            const int fa = I255(libFade);
            HRGN rg = CreateRectRgn(lp.left, lp.top, lp.right, lp.bottom);
            SelectClipRgn(mm, rg);

            {   // поле поиска: значок лупы + запрос (или подсказка)
                RECT ic = LY.search;
                ic.right = ic.left + S(32);
                DrawTextRect(mm, fVi, RGB(0, 0, (int)(fa * 0.8)), L"\uE721", ic, DT_CENTER | DT_VCENTER);
                RECT tx = LY.search;
                tx.left += S(32);
                tx.right -= S(8);
                const std::wstring& shown = g_searchActive ? g_searchText : g_libQuery;
                int off = 0, caretX = 0;
                if (g_searchActive && !shown.empty()) {  // длинный запрос «уезжает» влево, чтобы курсор был виден
                    HGDIOBJ of = SelectObject(mm, fTab);
                    SIZE sz = {};
                    GetTextExtentPoint32W(mm, shown.c_str(), std::clamp(g_searchCaret, 0, (int)shown.size()), &sz);
                    SelectObject(mm, of);
                    caretX = sz.cx;
                    const int avail = std::max(1, (int)(tx.right - tx.left) - S(3));
                    if (caretX > avail) off = caretX - avail;
                }
                HRGN rgf = CreateRectRgn(tx.left, std::max(tx.top, lp.top), tx.right, std::min(tx.bottom, lp.bottom));
                SelectClipRgn(mm, rgf);
                if (shown.empty()) {
                    DrawTextRect(mm, fTab, RGB(0, (int)(fa * 0.6), 0),
                                 g_searchActive ? L"Введите запрос…" : L"Поиск…", tx,
                                 DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
                } else if (g_searchActive) {
                    RECT t2 = tx;
                    t2.left -= off;
                    t2.right = t2.left + 20000;  // без обрезки и многоточия
                    DrawTextRect(mm, fTab, RGB(fa, 0, 0), shown.c_str(), t2, DT_LEFT | DT_VCENTER);
                } else {
                    DrawTextRect(mm, fTab, RGB(fa, 0, 0), shown.c_str(), tx, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
                }
                if (g_searchActive && ((GetTickCount64() / 500) & 1) == 0) {  // мигающий курсор
                    const int caretH = S(std::max(9, g_set.fontSize - 2)) + S(4);
                    const int caretL = tx.left - off + caretX;
                    const int caretT = (int)(LY.search.top + LY.search.bottom - caretH) / 2;
                    RECT cr = {caretL, caretT, caretL + std::max(1, S(1)), caretT + caretH};
                    HBRUSH hb = CreateSolidBrush(RGB(fa, 0, 0));
                    FillRect(mm, &cr, hb);
                    DeleteObject(hb);
                }
                SelectClipRgn(mm, rg);
                DeleteObject(rgf);
            }
            DrawTextRect(mm, fTab, RGB(fa, 0, 0), L"Моя волна", LY.wave, DT_CENTER | DT_VCENTER);
            DrawTextRect(mm, fVi, RGB(0, 0, (int)(fa * 0.9)), L"", LY.tune, DT_CENTER | DT_VCENTER);   // настройки волны
            DrawTextRect(mm, fVi, RGB(0, 0, (int)(fa * 0.9)), L"", LY.shake, DT_CENTER | DT_VCENTER);  // встряхнуть
            DrawTextRect(mm, fTab, RGB(g_libMode == 0 ? fa : (int)(fa * 0.8), 0, 0),
                         g_libMode == 0 ? L"Плейлисты" : (g_libMode == 2 && g_libBack == 1) ? L"\u2039 Поиск" : L"\u2039 Плейлисты", LY.tabA, DT_CENTER | DT_VCENTER);
            DrawTextRect(mm, fTab, RGB(g_libMode == 1 || g_libMode == 2 ? fa : (int)(fa * 0.8), 0, 0),
                         g_libMode == 2 ? L"Играть всё" : L"Поиск", LY.tabB, DT_CENTER | DT_VCENTER);
            {
                const std::wstring wavT = L"Настройки волны";
                const std::wstring& tt2 = g_libMode == 1 ? g_libQuery : g_libMode == 2 ? g_plTitle : g_libMode == 3 ? wavT : std::wstring();
                if (!tt2.empty())
                    DrawTextRect(mm, fRA, RGB(0, (int)(fa * 0.75), 0), tt2.c_str(), LY.title,
                                 DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
            }

            // список: сообщение (загрузка/ошибка/пусто) или строки
            RECT lc = LY.list;
            lc.top = std::max(lc.top, lp.top);
            lc.bottom = std::min(lc.bottom, lp.bottom);
            if (lc.bottom > lc.top) {
                HRGN rg2 = CreateRectRgn(lc.left, lc.top, lc.right, lc.bottom);
                SelectClipRgn(mm, rg2);
                const std::wstring msg = LibMessage();
                if (g_libMode == 3) {  // настройки «Моей волны»: подписи групп и опций
                    const WaveLay WL = CalcWaveLay(LY);
                    const int ox = (int)LY.list.left, oy = (int)LY.list.top - (int)std::lround(g_libScroll);
                    if (g_pmode != PM_WAVE) {
                        RECT nr = {LY.list.left + S(6), oy + WL.noteY, LY.list.right - S(8), oy + WL.noteY + S(16)};
                        DrawTextRect(mm, fRA, RGB(0, (int)(fa * 0.6), 0), L"Применится при запуске «Моей волны»", nr,
                                     DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
                    }
                    for (int g = 0; g < 4; g++) {
                        RECT lr = {LY.list.left + S(6), oy + WL.labelY[g], LY.list.right - S(8),
                                   oy + WL.labelY[g] + WL.labelH};
                        DrawTextRect(mm, fRA, RGB(0, (int)(fa * 0.75), 0), kWaveGroups[g].title, lr,
                                     DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
                    }
                    for (size_t i = 0; i < WL.chips.size(); i++) {
                        RECT r = WL.chips[i].r;
                        OffsetRect(&r, ox, oy);
                        if (r.bottom <= lc.top || r.top >= lc.bottom) continue;
                        const bool sel = g_wCfg[WL.chips[i].g] == kWaveGroups[WL.chips[i].g].opts[WL.chips[i].o].id;
                        DrawTextRect(mm, fTab, sel ? RGB(fa, 0, 0) : RGB(0, (int)(fa * 0.9), 0),
                                     kWaveGroups[WL.chips[i].g].opts[WL.chips[i].o].label, r, DT_CENTER | DT_VCENTER);
                    }
                } else if (!msg.empty()) {
                    HGDIOBJ oldF = SelectObject(mm, fTab);
                    SetTextColor(mm, RGB(0, (int)(fa * 0.8), 0));
                    RECT mr = {LY.list.left + S(10), LY.list.top + S(10), LY.list.right - S(10), LY.list.bottom};
                    DrawTextW(mm, msg.c_str(), -1, &mr, DT_WORDBREAK | DT_NOPREFIX | DT_CENTER);
                    SelectObject(mm, oldF);
                } else {
                    const int n = LibRowCount();
                    const int lh1 = S(std::max(10, g_set.fontSize - 1)) + S(4), lh2 = S(std::max(9, g_set.fontSize - 3)) + S(3);
                    const int first = std::max(0, (int)floor(g_libScroll / (double)LY.rowH));
                    for (int i = first; i < n; i++) {
                        const int rt = LY.list.top + i * LY.rowH - (int)std::lround(g_libScroll);
                        if (rt >= lc.bottom) break;
                        const int rb = rt + LY.rowH;
                        if (rb <= lc.top) continue;
                        std::wstring t1, t2, t3;
                        if (g_libMode == 0) {
                            t1 = g_pls[(size_t)i].title;
                            if (g_pls[(size_t)i].count > 0) t2 = std::to_wstring(g_pls[(size_t)i].count) + L" тр.";
                        } else if (const YmArtist* ar = LibRowArtist(i)) {
                            t1 = ar->name;
                            t2 = L"Исполнитель";
                        } else if (const YmTrack* tk = LibRowTrack(i)) {
                            t1 = tk->title;
                            t2 = tk->artists;
                            t3 = FmtTime(tk->durMs * 10000LL);
                        }
                        int right = LY.list.right - S(8);
                        if (!t3.empty()) {
                            RECT r3 = {right - S(44), rt, right, rb};
                            DrawTextRect(mm, fRA, RGB(0, (int)(fa * 0.7), 0), t3.c_str(), r3, DT_RIGHT | DT_VCENTER);
                            right -= S(48);
                        }
                        const int ty = rt + (LY.rowH - lh1 - lh2) / 2;
                        const int tl = LY.list.left + S(6) + ThumbSize() + S(8);  // справа от миниатюры
                        if (!LibThumb(i) && !(g_libMode == 0 && g_pls[(size_t)i].kind == "liked")) {  // заглушка вместо обложки: значок
                            const int ts = ThumbSize();
                            RECT gr = {LY.list.left + S(6), rt + (LY.rowH - ts) / 2, LY.list.left + S(6) + ts,
                                       rt + (LY.rowH - ts) / 2 + ts};
                            const bool heart = g_libMode == 0 && g_pls[(size_t)i].kind == "liked";
                            DrawTextRect(mm, fVi, RGB(0, 0, (int)(fa * 0.55)), heart ? L"\uEB51" : LibRowArtist(i) ? L"\uE77B" : (g_libMode == 0 && !g_pls[(size_t)i].uid.empty()) ? L"\uE787" : L"\uE8D6", gr,
                                         DT_CENTER | DT_VCENTER);
                        }
                        RECT r1 = {tl, ty, right, ty + lh1};
                        RECT r2 = {tl, ty + lh1, right, ty + lh1 + lh2};
                        DrawTextRect(mm, fRT, RGB(fa, 0, 0), t1.c_str(), r1, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
                        if (!t2.empty())
                            DrawTextRect(mm, fRA, RGB(0, (int)(fa * 0.75), 0), t2.c_str(), r2,
                                         DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
                    }
                }
                SelectClipRgn(mm, nullptr);
                DeleteObject(rg2);
            }
            SelectClipRgn(mm, nullptr);
            DeleteObject(rg);
        }

        const Rgb* cs3[3] = {&P.text, &P.text2, &P.text};
        unsigned col[3][3];
        for (int k = 0; k < 3; k++) {
            col[k][0] = U8(cs3[k]->r);
            col[k][1] = U8(cs3[k]->g);
            col[k][2] = U8(cs3[k]->b);
        }
        Composite(col, 256);
    }

    POINT pos = {g_x, g_y};
    SIZE size = {g_w, g_h};
    POINT src = {0, 0};
    BLENDFUNCTION bf = {AC_SRC_OVER, 0, 255, AC_SRC_ALPHA};
    UpdateLayeredWindow(hwnd, screen, &pos, &size, mem, &src, 0, &bf, ULW_ALPHA);

    for (HFONT f : fonts) DeleteObject(f);
    SelectObject(mem, oldBmp);
    SelectObject(mm, oldBmpM);
    DeleteObject(bmp);
    DeleteObject(bmpM);
    DeleteDC(mem);
    DeleteDC(mm);
    ReleaseDC(nullptr, screen);
}

// ---------------------------------------------------------------- окно
static bool Layout() {
    HWND tray = FindWindowW(L"Shell_TrayWnd", nullptr);
    if (!tray || !IsWindowVisible(tray)) return false;
    RECT r;
    GetWindowRect(tray, &r);
    g_scale = GetDpiForWindow(tray) / 96.0;

    g_hc = r.bottom - r.top;                // компактная высота = высота панели
    g_wc = S(g_set.width);                  // компактная ширина
    g_we = std::max(g_wc, S(340));          // развёрнутые размеры
    g_he = std::max(g_hc, CalcExpLay().H);

    // текущий размер окна — между компактным и развёрнутым
    const double e = std::max(0.0, g_expT);  // пружина, может чуть «перелетать» за 1
    g_w = g_wc + (int)std::lround((g_we - g_wc) * e);
    g_h = g_hc + (int)std::lround((g_he - g_hc) * e);

    // панель в нижней половине экрана — растём вверх, иначе вниз
    g_bottom = true;
    HMONITOR mon = MonitorFromWindow(tray, MONITOR_DEFAULTTOPRIMARY);
    MONITORINFO mi = {sizeof(mi)};
    if (GetMonitorInfoW(mon, &mi))
        g_bottom = (r.top + r.bottom) / 2 >= (mi.rcMonitor.top + mi.rcMonitor.bottom) / 2;
    g_y = g_bottom ? r.bottom - g_h : r.top;

    int trayW = r.right - r.left;
    int base = 0;
    if (g_set.alignment == 1) base = (trayW - g_w) / 2;
    else if (g_set.alignment == 2) base = trayW - g_w;
    int x = base + SD(g_set.offsetX);  // сдвиг от масштаба плеера не зависит
    // не даём виджету уехать за пределы панели задач
    x = std::max(0, std::min(x, std::max(0, trayW - g_w)));
    g_x = r.left + x;

    // «кадры» содержимого (компактный и развёрнутый) прижаты к той же стороне, что и окно
    auto fx = [&](int fw) {
        return g_set.alignment == 1 ? (g_w - fw) / 2 : g_set.alignment == 2 ? g_w - fw : 0;
    };
    g_oxC = fx(g_wc);
    g_oxE = fx(g_we);
    g_oyC = g_bottom ? g_h - g_hc : 0;
    g_oyE = g_bottom ? g_h - g_he : 0;
    return true;
}

// Полноэкранное приложение именно на мониторе с панелью задач (игра, видео и т.п.).
// Не считаем таковыми: окна самого explorer (Alt-Tab, Task View, Пуск, рабочий стол),
// обычные развёрнутые окна (они не перекрывают панель задач) и всё, что открыто на другом мониторе.
static bool IsFullscreenOtherApp() {
    HWND fg = GetForegroundWindow();
    if (!fg || fg == g_hwnd) return false;
    if (!IsWindowVisible(fg) || IsIconic(fg)) return false;

    DWORD pid = 0;
    GetWindowThreadProcessId(fg, &pid);
    if (pid == GetCurrentProcessId()) return false;

    HWND tray = FindWindowW(L"Shell_TrayWnd", nullptr);
    if (!tray) return false;
    HMONITOR monTray = MonitorFromWindow(tray, MONITOR_DEFAULTTOPRIMARY);
    HMONITOR monFg = MonitorFromWindow(fg, MONITOR_DEFAULTTONEAREST);
    if (monTray != monFg) return false;

    RECT r;
    if (!GetWindowRect(fg, &r)) return false;
    MONITORINFO mi = {sizeof(mi)};
    if (!GetMonitorInfoW(monFg, &mi)) return false;
    const int tol = 2;  // небольшой допуск на округления/DPI
    bool covers = r.left <= mi.rcMonitor.left + tol && r.top <= mi.rcMonitor.top + tol &&
                  r.right >= mi.rcMonitor.right - tol && r.bottom >= mi.rcMonitor.bottom - tol;

    // Обычное (не развёрнутое) окно, закрывающее весь монитор: игра в безрамочном окне, плеер и т.п.
    if (covers && !IsZoomed(fg)) return true;

    // Браузер/плеер в полноэкранном режиме часто остаётся «развёрнутым» (IsZoomed), а обычное
    // развёрнутое окно панель задач не перекрывает. Различаем их по состоянию, которое определяет
    // сама оболочка Windows (полноэкранное приложение / D3D-игра / режим презентации).
    QUERY_USER_NOTIFICATION_STATE qs = QUNS_ACCEPTS_NOTIFICATIONS;
    bool shellFull = SUCCEEDED(SHQueryUserNotificationState(&qs)) &&
                     (qs == QUNS_BUSY || qs == QUNS_RUNNING_D3D_FULL_SCREEN ||
                      qs == QUNS_PRESENTATION_MODE);
    if (IsZoomed(fg)) return covers && shellFull;
    return shellFull;  // окно не точно по размеру монитора, но оболочка считает его полноэкранным
}

// Прячем только если «полноэкранность» держится дольше 500 мс —
// переходы между окнами (Alt-Tab, смена фокуса) на мгновение выглядят как fullscreen.
static ULONGLONG g_fsSince = 0;
static bool ShouldHide() {
    if (!IsFullscreenOtherApp()) {
        g_fsSince = 0;
        return false;
    }
    ULONGLONG now = GetTickCount64();
    if (!g_fsSince) g_fsSince = now;
    return now - g_fsSince > 500;
}

// Лёгкое обновление: видимость, позиция и z-порядок (без опроса медиа).
// Вызывается часто — поэтому SetWindowPos только если что-то реально сбилось.
static void Present(HWND h, bool redraw) {
    Layout();  // при неудаче остаётся прежняя геометрия
    if (g_w <= 0 || g_h <= 0 || ShouldHide()) {
        if (IsWindowVisible(h)) ShowWindow(h, SW_HIDE);
        return;
    }
    bool wasHidden = !IsWindowVisible(h);
    if (wasHidden) ShowWindow(h, SW_SHOWNOACTIVATE);

    RECT cur = {};
    GetWindowRect(h, &cur);
    bool moved = cur.left != g_x || cur.top != g_y || cur.right - cur.left != g_w ||
                 cur.bottom - cur.top != g_h;
    // GW_HWNDPREV != null — над нами оказалось другое окно (чаще всего сама панель задач)
    // пока открыто контекстное меню трека, наверх не поднимаемся — иначе виджет закроет само меню
    if (wasHidden || moved || (!g_menuOpen && GetWindow(h, GW_HWNDPREV)))
        SetWindowPos(h, HWND_TOPMOST, g_x, g_y, g_w, g_h, SWP_NOACTIVATE);
    if (redraw || wasHidden || moved) Render(h);
}

static void Tick(HWND h) {
    Poll();
    if (g_expand || g_expT > 0.0) PollVolume();
    LyEnsure();
    Present(h, true);
}

// События смены окон — сразу поправить z-порядок/видимость, не дожидаясь таймера
static void CALLBACK WinEventProc(HWINEVENTHOOK, DWORD, HWND, LONG, LONG, DWORD, DWORD) {
    if (g_hwnd) PostMessageW(g_hwnd, WM_APP + 3, 0, 0);
}

static void SetExpanded(HWND h, bool on) {
    g_expand = on;
    g_hot = 0;  // подсветка кнопок компактного режима не должна «переезжать» в развёрнутый
    KillTimer(h, 7);
    if (on) PollVolume();
    StartAnim(h);
}

// курсор ушёл с виджета; развёрнутый плеер сворачиваем через заданную паузу
static void OnLeave(HWND h) {
    g_hover = false;
    g_btn = 0;
    g_hot = 0;
    g_pressed = 0;
    g_libHover = -1;
    StartAnim(h);
    if (g_menuOpen) return;
    if (!g_expand || g_set.collapseMode == 2) return;
    if (g_searchActive) return;  // пока идёт ввод запроса, плеер не сворачиваем
    if (g_set.collapseMode == 0 || g_set.collapseDelay == 0) {
        g_expand = false;  // сворачиваем сразу
        StartAnim(h);
    } else {
        SetTimer(h, 7, g_set.collapseDelay * 1000, nullptr);
    }
}

// перетаскивание ползунка перемотки (1) или громкости (2)
static void UpdateDrag(HWND h, int mx) {
    ExpLay L = CalcExpLay();
    const RECT& b = g_drag == 1 ? L.bar : L.volBar;
    int w = b.right - b.left;
    if (w <= 0) return;
    double f = std::clamp((double)(mx - g_oxE - b.left) / (double)w, 0.0, 1.0);
    if (g_drag == 1) {
        g_dragFrac = f;
    } else {
        g_vol = (float)f;
        g_muted = false;
        ApplyVolume(false);
    }
    Render(h);
}

static bool ExpMode() { return g_expand && g_expT > 0.5; }

static LRESULT CALLBACK WndProc(HWND h, UINT m, WPARAM w, LPARAM l) {
    switch (m) {
        case WM_MOUSEACTIVATE:
            return MA_NOACTIVATE;
        case WM_MOUSEMOVE: {
            int mx = GET_X_LPARAM(l);
            if (g_drag) {
                UpdateDrag(h, mx);
                return 0;
            }
            KillTimer(h, 7);  // курсор вернулся — отменяем автосворачивание
            if (!g_hover) {
                g_hover = true;
                TRACKMOUSEEVENT t = {sizeof(t), TME_LEAVE, h, 0};
                TrackMouseEvent(&t);
                StartAnim(h);  // перерисовку делает таймер анимации
            }
            g_btn = ExpMode() ? 0 : HitBtn(mx);
            int hot = ExpMode() ? 0 : g_btn;  // в компактном режиме подсвечиваем кнопку под курсором (1..3)
            if (ExpMode()) {
                ExpLay L = CalcExpLay();
                int fx = mx - g_oxE, fy = GET_Y_LPARAM(l) - g_oyE;
                hot = In(L.prev, fx, fy) ? 1 : In(L.play, fx, fy) ? 2 : In(L.next, fx, fy) ? 3
                    : In(L.volIcon, fx, fy) ? 4 : (In(L.barHit, fx, fy) && g_durTicks > 0) ? 5
                    : In(L.volHit, fx, fy) ? 6 : In(L.lyBtn, fx, fy) ? 7
                    : (g_lyOpen && In(L.lyChk, fx, fy)) ? 8
                    : In(L.libBtn, fx, fy) ? 9 : In(L.like, fx, fy) ? 10 : 0;
                int hov = -1;
                if (g_libOpen && In(L.lyrics, fx, fy)) {  // внутри панели библиотеки
                    const LibLay Y = CalcLibLay(L.lyrics, L.libFull);
                    if (hot == 0)
                        hot = In(Y.search, fx, fy) ? 11 : In(Y.wave, fx, fy) ? 12 : In(Y.tabA, fx, fy) ? 13
                              : In(Y.tabB, fx, fy) ? 14 : In(Y.tune, fx, fy) ? 16 : In(Y.shake, fx, fy) ? 17 : 0;
                    hov = g_libMode == 3 ? WaveChipAt(Y, fx, fy) : LibRowAt(Y, fx, fy);  // в настройках волны — индекс чипа
                }
                if (hot == 0 && OwnMode() && g_haveCur && In(L.artist, fx, fy)) {  // имя исполнителя — кликабельно
                    const int ai = ArtistAtX(L, fx);
                    if (ai >= 0) {
                        g_artHover = ai;  // не сбрасываем при уходе — подчёркивание плавно гаснет
                        hot = 15;
                    }
                }
                if (hov != g_libHover) {
                    g_libHover = hov;
                    StartAnim(h);
                }
            }
            if (hot != g_hot) {
                g_hot = hot;
                StartAnim(h);
            }
            return 0;
        }
        case WM_SETCURSOR:
            if (LOWORD(l) == HTCLIENT && ExpMode() && (g_hot == 15 || (g_hot >= 12 && g_hot <= 14) || g_hot == 16 || g_hot == 17)) {
                SetCursor(LoadCursorW(nullptr, IDC_HAND));
                return TRUE;
            }
            break;
        case WM_MOUSELEAVE:
            if (g_drag) return 0;  // во время перетаскивания уход курсора не считаем
            OnLeave(h);
            return 0;
        case WM_LBUTTONDOWN: {
            if (!ExpMode()) return 0;
            ExpLay L = CalcExpLay();
            int fx = GET_X_LPARAM(l) - g_oxE, fy = GET_Y_LPARAM(l) - g_oyE;
            if (In(L.barHit, fx, fy) && g_durTicks > 0) g_drag = 1;
            else if (In(L.volHit, fx, fy)) g_drag = 2;
            if (g_drag) {
                g_hot = g_drag == 1 ? 5 : 6;
                SetCapture(h);
                UpdateDrag(h, GET_X_LPARAM(l));
            } else {
                g_pressed = In(L.prev, fx, fy) ? 1 : In(L.play, fx, fy) ? 2 : In(L.next, fx, fy) ? 3
                          : In(L.volIcon, fx, fy) ? 4 : In(L.lyBtn, fx, fy) ? 5
                          : In(L.libBtn, fx, fy) ? 6 : In(L.like, fx, fy) ? 7 : 0;
            }
            StartAnim(h);
            return 0;
        }
        case WM_LBUTTONUP: {
            if (g_pressed) {
                g_pressed = 0;
                StartAnim(h);
            }
            if (g_drag) {
                int d = g_drag;
                g_drag = 0;
                g_hot = 0;
                StartAnim(h);
                if (GetCapture() == h) ReleaseCapture();
                if (d == 1) DoSeek(g_dragFrac);
                else ApplyVolume(true);
                Render(h);
                // за время перетаскивания курсор мог уйти с виджета
                POINT pt;
                RECT wr;
                GetCursorPos(&pt);
                GetWindowRect(h, &wr);
                if (PtInRect(&wr, pt)) {
                    TRACKMOUSEEVENT t = {sizeof(t), TME_LEAVE, h, 0};
                    TrackMouseEvent(&t);
                } else {
                    OnLeave(h);
                }
                return 0;
            }
            if (ExpMode()) {
                ExpLay L = CalcExpLay();
                int fx = GET_X_LPARAM(l) - g_oxE, fy = GET_Y_LPARAM(l) - g_oyE;
                if (g_searchActive) {  // клик не по полю поиска — ввод закончен
                    const LibLay Y0 = CalcLibLay(L.lyrics, L.libFull);
                    if (!(g_libOpen && In(Y0.search, fx, fy))) SearchEnd(false);
                }
                int b = In(L.prev, fx, fy) ? 1 : In(L.play, fx, fy) ? 2 : In(L.next, fx, fy) ? 3 : 0;
                if (b) {
                    DoMediaCommand(b);
                    SetTimer(h, 2, 300, nullptr);  // быстро обновить состояние
                } else if (In(L.lyBtn, fx, fy)) {  // показать/скрыть текст песни
                    g_lyOpen = !g_lyOpen;
                    if (!g_lyOpen) g_diagShow = false;
                    if (g_lyOpen) {
                        if (g_libOpen) {  // панели делят одно место
                            g_libOpen = false;
                            CloseSearchPopup();
                        }
                        if (g_lyState == 3) g_lyKey.clear();  // прошлая попытка не удалась — пробуем ещё раз
                        LyEnsure();
                    }
                    StartAnim(h);
                } else if (In(L.libBtn, fx, fy)) {  // показать/скрыть библиотеку (поиск и плейлисты)
                    ToggleLibrary(h);
                    if (!g_libOpen) CloseSearchPopup();
                } else if (In(L.like, fx, fy)) {  // «нравится»
                    OwnToggleLike();
                    Render(h);
                } else if (g_libOpen && In(L.lyrics, fx, fy)) {  // клик внутри библиотеки
                    LibClick(h, L, fx, fy);
                } else if (g_lyOpen && In(L.lyChk, fx, fy)) {  // «i» — проверка токенов
                    ToggleCheck(h);
                } else if (g_lyOpen && !g_diagShow && g_lySynced && g_lyState == 2 && In(L.lyrics, fx, fy)) {
                    // клик по строке синхронизированного текста — перемотка к ней
                    int my = GET_Y_LPARAM(l);
                    if (g_durTicks > 0 && g_lyPosY.size() == g_lyrics.size() && g_lyH.size() == g_lyrics.size()) {
                        for (size_t i = 0; i < g_lyrics.size(); i++) {
                            if (my >= g_lyPosY[i] && my < g_lyPosY[i] + g_lyH[i]) {
                                DoSeek((double)g_lyrics[i].t / (double)g_durTicks);
                                g_lyManual = 0.0;
                                break;
                            }
                        }
                    }
                } else if (In(L.volIcon, fx, fy)) {
                    g_muted = !g_muted;
                    ApplyVolume(true);
                    Render(h);
                } else if (OwnMode() && g_haveCur && In(L.artist, fx, fy) && ArtistAtX(L, fx) >= 0) {
                    OpenArtistFromPlayer(h, ArtistAtX(L, fx));  // профиль исполнителя
                } else if (In(L.cover, fx, fy) || In(L.title, fx, fy) || In(L.artist, fx, fy)) {
                    if (!OwnMode()) ToggleApp();  // во встроенном режиме приложение не открываем
                }
                return 0;
            }
            int b = HitBtn(GET_X_LPARAM(l));
            if (b) {
                DoMediaCommand(b);
                SetTimer(h, 2, 300, nullptr);
            } else if (OwnMode()) {
                SetExpanded(h, true);  // встроенный плеер: ЛКМ разворачивает виджет
            } else {
                ToggleApp();
            }
            return 0;
        }
        case WM_RBUTTONUP: {  // ПКМ по треку — меню; в остальных местах — развернуть/свернуть плеер
            if (ExpMode() && OwnMode() && !g_drag) {
                ExpLay L = CalcExpLay();
                const int fx = GET_X_LPARAM(l) - g_oxE, fy = GET_Y_LPARAM(l) - g_oyE;
                if (g_libOpen && In(L.lyrics, fx, fy)) {  // строка трека в библиотеке
                    const LibLay Y = CalcLibLay(L.lyrics, L.libFull);
                    const int row = g_libMode == 3 ? -1 : LibRowAt(Y, fx, fy);
                    if (const YmTrack* t = LibRowTrack(row)) {
                        ShowTrackMenu(h, *t);
                        return 0;
                    }
                } else if (g_haveCur && (In(L.cover, fx, fy) || In(L.title, fx, fy) || In(L.artist, fx, fy) ||
                                         In(L.wavePill, fx, fy))) {  // текущий трек
                    ShowTrackMenu(h, g_cur);
                    return 0;
                }
            }
            SetExpanded(h, !g_expand);
            return 0;
        }
        case WM_MOUSEWHEEL:  // колесо: над панелью текста — прокрутка, иначе громкость приложения
            if (ExpMode() && !g_drag) {
                POINT wp = {GET_X_LPARAM(l), GET_Y_LPARAM(l)};
                ScreenToClient(h, &wp);
                ExpLay L = CalcExpLay();
                if (g_libOpen && In(L.lyrics, wp.x - g_oxE, wp.y - g_oyE)) {  // прокрутка списка библиотеки
                    const LibLay Y = CalcLibLay(L.lyrics, L.libFull);
                    g_libScroll = std::clamp(g_libScroll - GET_WHEEL_DELTA_WPARAM(w) / 120.0 * S(44), 0.0, LibMaxScroll(Y));
                    StartAnim(h);
                    return 0;
                }
                if (g_lyOpen && In(L.lyrics, wp.x - g_oxE, wp.y - g_oyE)) {
                    g_lyManual += GET_WHEEL_DELTA_WPARAM(w) / 120.0 * S(44);
                    if (g_lySynced) g_lyManualUntil = GetTickCount64() + 3000;
                    StartAnim(h);
                    return 0;
                }
                g_vol = std::clamp(g_vol + GET_WHEEL_DELTA_WPARAM(w) / 120.f * (g_set.volumeStep / 100.f), 0.f, 1.f);
                g_muted = false;
                ApplyVolume(true);
                Render(h);
            }
            return 0;
        case WM_CAPTURECHANGED:
            if (g_drag) {
                g_drag = 0;
                Render(h);
            }
            return 0;
        case WM_TIMER:
            if (w == 5) {  // страховка: быстро возвращаем виджет наверх / показываем после fullscreen
                Present(h, false);
                if (g_libOpen) LibRequestThumbs();  // подгружаем миниатюры видимых строк
                if (g_searchActive) {  // страховки: перехват клавиатуры не должен «залипнуть»
                    if (!IsWindowVisible(h) || !g_expand || !g_libOpen ||
                        GetTickCount64() - g_searchLastKey > 90000) {
                        SearchEnd(false);
                    } else if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {  // клик в другом месте экрана
                        POINT pt;
                        GetCursorPos(&pt);
                        if (!PtInRect(&g_searchScr, pt)) SearchEnd(false);
                    }
                }
                return 0;
            }
            if (w == 4) {
                if (!StepAnim()) KillTimer(h, 4);
                Layout();  // размер окна зависит от прогресса раскрытия
                Render(h);
                return 0;
            }
            if (w == 7) {  // автосворачивание
                KillTimer(h, 7);
                if (g_expand && !g_hover && !g_drag && !g_searchActive) {
                    g_expand = false;
                    StartAnim(h);
                }
                return 0;
            }
            if (w == 8) {  // плавное движение шкалы воспроизведения и смена строки текста
                if (g_searchActive && g_expand && IsWindowVisible(h)) {  // мигание курсора в поле поиска
                    Render(h);
                    return 0;
                }
                if (g_expand && g_expT >= 1.0 && g_stageT >= 1.0 && !g_drag && IsWindowVisible(h)) {
                    if (g_lyOpen && g_lyState == 2 && g_lySynced) {
                        int idx = LyCurIndex();
                        if (idx != g_lyIdx) {
                            g_lyIdx = idx;
                            StartAnim(h);  // плавная прокрутка
                            return 0;
                        }
                    }
                    if (g_playing) Render(h);
                }
                return 0;
            }
            if (w == 2 || w == 3) KillTimer(h, w);
            Tick(h);
            return 0;
        case WM_APP + 4: {  // текст песни загружен (или не найден)
            LyResult* r = (LyResult*)l;
            g_lyBusy = false;
            if (r->key == g_lyKey) {
                g_lyrics.swap(r->lines);
                g_lySynced = r->synced;
                g_lyState = r->state;
                g_lyH.clear();
                g_lyManual = 0.0;
                g_lyIdx = g_lySynced ? LyCurIndex() : 0;
                g_lyScroll = g_lyIdx;
                g_lyDiag = r->diag;
                g_lySource = r->source;
            }
            delete r;
            LyEnsure();  // трек мог смениться, пока шла загрузка
            StartAnim(h);
            return 0;
        }
        case WM_APP + 5: {  // результат проверки токенов
            std::wstring* r = (std::wstring*)l;
            g_chkBusy = false;
            g_diagText = *r;
            delete r;
            StartAnim(h);
            return 0;
        }
        case WM_APP + 6: {  // ответ фонового потока Яндекс Музыки
            YmRes* r = (YmRes*)l;
            OwnOnResult(r);
            delete r;
            LyEnsure();  // трек мог смениться
            StartAnim(h);
            return 0;
        }
        case WM_APP + 7: {  // из окна ввода пришёл поисковый запрос
            std::wstring* q = (std::wstring*)l;
            if (OwnMode()) LibSearch(*q);
            delete q;
            StartAnim(h);
            return 0;
        }
        case WM_APP + 8:  // события MediaPlayer: 1 — трек закончился, 2 — ошибка, 3 — открыт, 4/5 — «следующий»/«предыдущий»
            if (OwnMode()) {
                if (w == 1) OwnOnEnded();
                else if (w == 2) OwnOnFailed();
                else if (w == 3) OwnOnOpened();
                else if (w == 4) OwnNext();
                else if (w == 5) OwnPrev();
            }
            StartAnim(h);
            return 0;
        case WM_APP + 10:  // клавиша из хука ввода поиска: wParam — код (+0x100 при Ctrl), lParam — символ
            if (g_searchActive) {
                SearchKey((UINT)(w & 0xFF), (w & 0x100) != 0, (wchar_t)l);
                if (g_searchActive) Render(h);
            }
            return 0;
        case WM_APP + 9: {  // окно ввода запроса закрылось — если курсор не над плеером, запускаем обычное сворачивание
            POINT pt;
            RECT wr;
            GetCursorPos(&pt);
            GetWindowRect(h, &wr);
            if (g_expand && !PtInRect(&wr, pt)) OnLeave(h);
            StartAnim(h);
            return 0;
        }
        case WM_APP + 2:  // событие смены трека/состояния — небольшой debounce
            SetTimer(h, 2, 40, nullptr);
            return 0;
        case WM_APP + 3:  // смена активного/свёрнутого окна
        case WM_DISPLAYCHANGE:
        case WM_DPICHANGED:
            if (m == WM_APP + 3 && g_searchActive && GetTickCount64() - g_searchBeganAt > 500)
                SearchEnd(false);  // пользователь переключился в другое окно
            Present(h, false);
            return 0;
        case WM_APP + 1: {
            const std::string oldToken = g_set.geniusToken;
            const std::string oldYm = g_set.ymToken;
            const bool oldOwn = OwnMode();
            LoadSettings();
            if (g_set.geniusToken != oldToken && g_lyState == 3)
                g_lyKey.clear();  // токен поменяли — ищем текст заново
            if (g_set.ymToken != oldYm || OwnMode() != oldOwn) {  // токен или режим работы изменились
                CloseSearchPopup();
                OwnReconfigure();
            }
            Layout();  // новая геометрия нужна до Poll, чтобы обложка пересчиталась сразу
            Tick(h);
            return 0;
        }
        case WM_CLOSE:
            DestroyWindow(h);
            return 0;
        case WM_DESTROY:
            SearchShutdown();
            KillTimer(h, 1);
            KillTimer(h, 4);
            KillTimer(h, 5);
            KillTimer(h, 7);
            KillTimer(h, 8);
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcW(h, m, w, l);
}

static DWORD WINAPI UiThread(LPVOID) {
    winrt::init_apartment();

    // ждём таскбар и убеждаемся, что это нужный экземпляр explorer.exe
    HWND tray = nullptr;
    for (int i = 0; i < 60 && !g_stopping; i++) {
        tray = FindWindowW(L"Shell_TrayWnd", nullptr);
        if (tray) break;
        Sleep(1000);
    }
    if (!tray || g_stopping) return 0;
    DWORD pid = 0;
    GetWindowThreadProcessId(tray, &pid);
    if (pid != GetCurrentProcessId()) return 0;

    try {
        g_mgr = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
    } catch (...) {
        Wh_Log(L"SMTC недоступен");
    }

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.lpszClassName = L"WH_TaskbarMusicWidget";
    RegisterClassW(&wc);

    g_hwnd = CreateWindowExW(
        WS_EX_LAYERED | WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_NOACTIVATE,
        wc.lpszClassName, L"", WS_POPUP, 0, 0, 10, 10, nullptr, nullptr, wc.hInstance, nullptr);
    if (!g_hwnd) return 0;

    YmStart();  // фоновые потоки Яндекс Музыки
    if (OwnMode()) OwnReconfigure();
    SetTimer(g_hwnd, 1, 1000, nullptr);
    SetTimer(g_hwnd, 5, 250, nullptr);
    SetTimer(g_hwnd, 8, 250, nullptr);  // перерисовка шкалы в развёрнутом режиме
    HWINEVENTHOOK hookFg = SetWinEventHook(EVENT_SYSTEM_FOREGROUND, EVENT_SYSTEM_FOREGROUND, nullptr,
                                           WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
    HWINEVENTHOOK hookMin = SetWinEventHook(EVENT_SYSTEM_MINIMIZESTART, EVENT_SYSTEM_MINIMIZEEND,
                                            nullptr, WinEventProc, 0, 0, WINEVENT_OUTOFCONTEXT);
    Tick(g_hwnd);

    MSG msg;
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    if (hookFg) UnhookWinEvent(hookFg);
    if (hookMin) UnhookWinEvent(hookMin);
    Unsubscribe();
    YmStop();             // дожидаемся фоновых потоков
    OwnShutdownPlayer();  // и гасим MediaPlayer
    g_hwnd = nullptr;
    UnregisterClassW(wc.lpszClassName, wc.hInstance);
    g_mgr = nullptr;
    winrt::uninit_apartment();
    return 0;
}

// ---------------------------------------------------------------- Windhawk
BOOL Wh_ModInit() {
    LoadSettings();
    g_stopping = false;
    g_thread = CreateThread(nullptr, 0, UiThread, nullptr, 0, nullptr);
    return TRUE;
}

void Wh_ModSettingsChanged() {
    if (g_hwnd) PostMessageW(g_hwnd, WM_APP + 1, 0, 0);
}

void Wh_ModUninit() {
    g_stopping = true;
    if (g_hwnd) PostMessageW(g_hwnd, WM_CLOSE, 0, 0);
    if (g_thread) {
        WaitForSingleObject(g_thread, 20000);  // YmStop ждёт сетевые потоки
        CloseHandle(g_thread);
        g_thread = nullptr;
    }
    if (g_lyThread) {  // поток загрузки текста должен завершиться до выгрузки мода
        WaitForSingleObject(g_lyThread, 8000);
        CloseHandle(g_lyThread);
        g_lyThread = nullptr;
    }
    if (g_chkThread) {  // и поток проверки токенов
        WaitForSingleObject(g_chkThread, 8000);
        CloseHandle(g_chkThread);
        g_chkThread = nullptr;
    }
}