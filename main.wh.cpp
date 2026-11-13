// ==WindhawkMod==
// @id              taskbar-music-widget
// @name            Taskbar Music Widget
// @name:ru-RU      Музыкальный виджет на панели задач
// @description     Now-playing widget for the taskbar with an expandable island: seek, per-app volume, synced lyrics and colors from the album art
// @description:ru-RU Виджет «сейчас играет» на панели задач с раскрывающимся островом: перемотка, громкость приложения, текст песни и цвета из обложки
// @version         1.9.0
// @author          str1nks
// @github          https://github.com/str1nks/taskbar-music-widget
// @include         explorer.exe
// @compilerOptions -lgdi32 -lshell32 -lole32 -loleaut32 -lruntimeobject -lwinhttp
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
- Lyrics panel (button next to the controls): synchronized lyrics with the current line highlighted and auto-scrolled, click a line to seek, mouse wheel to scroll. Lyrics come from [LRCLIB](https://lrclib.net).
- Spring-like expand animation, staggered content reveal, cover cross-fade, hover and press effects.
- Collapses instantly, after a delay, or never when the cursor leaves.

## Setup

1. Set **Path to the app exe** (used to open/hide the app and to find its audio session for the volume slider).
2. Set **Music source filter** to a part of the app's AppUserModelId (e.g. `yandex`, `spotify`). Leave empty to follow the current system media session.
3. Tweak alignment, offset, scale, width and colors to taste.

## Settings

| Setting | Description |
| --- | --- |
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

## Notes

- Works with a horizontal taskbar (bottom or top). A vertical taskbar is not supported.
- The volume slider needs the app to have an active audio session; a value set while paused is applied when sound starts.
- Seeking and the progress bar need the app to publish timeline info; without it the bar stays empty.
- Lyrics are downloaded from lrclib.net in the background. If the site is unreachable, the panel shows "Text not found". The system proxy settings are used.
- Not every track has lyrics in the LRCLIB database.
*/
// ==/WindhawkModReadme==

// ==WindhawkModSettings==
/*
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
#undef GetCurrentTime

#include <string>
#include <algorithm>
#include <vector>
#include <cmath>
#include <chrono>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Control.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Graphics.Imaging.h>

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
static double g_hotT[8] = {};    // наведение: 1 назад, 2 play, 3 вперёд, 4 значок громкости, 5 шкала, 6 громкость, 7 текст песни
static double g_pressT[6] = {};  // нажатие кнопок 1..5
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

static std::wstring g_title;
static std::wstring g_artist;
static bool g_playing = false;

// таймлайн трека (в тиках по 100 нс)
static long long g_durTicks = 0;      // длительность; 0 — приложение таймлайн не отдаёт
static long long g_startTicks = 0;    // начало шкалы (обычно 0)
static long long g_posBase = 0;       // позиция (от начала) на момент g_posBaseTick
static ULONGLONG g_posBaseTick = 0;
static ULONGLONG g_seekHold = 0;      // до этого момента не принимаем позицию от приложения (после перемотки)

static long long CurPos() {
    long long p = g_posBase;
    if (g_playing) p += (long long)(GetTickCount64() - g_posBaseTick) * 10000LL;
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
static bool LoadCover(GlobalSystemMediaTransportControlsSessionMediaProperties const& props, int size) {
    namespace Img = winrt::Windows::Graphics::Imaging;
    try {
        auto ref = props.Thumbnail();
        if (!ref) return false;
        auto stream = ref.OpenReadAsync().get();
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
    if (g_drag == 2) return;
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

// ---------------------------------------------------------------- текст песни (LRCLIB)
struct LyJob {
    std::wstring key, artist, title;
    long long durSec;
};
struct LyResult {
    std::wstring key;
    int state;  // 2 — готово, 3 — не найдено, 4 — инструментал
    bool synced;
    std::vector<LyLine> lines;
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

static std::string HttpGet(const wchar_t* host, const std::wstring& path) {
    std::string out;
    HINTERNET ses = WinHttpOpen(L"TaskbarMusicWidget/1.0 (Windhawk mod)", 4 /*AUTOMATIC_PROXY*/, nullptr,
                                nullptr, 0);
    if (!ses) return out;
    WinHttpSetTimeouts(ses, 4000, 4000, 5000, 6000);
    HINTERNET con = WinHttpConnect(ses, host, INTERNET_DEFAULT_HTTPS_PORT, 0);
    HINTERNET req = con ? WinHttpOpenRequest(con, L"GET", path.c_str(), nullptr, WINHTTP_NO_REFERER,
                                             WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE)
                        : nullptr;
    if (req && WinHttpSendRequest(req, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0) &&
        WinHttpReceiveResponse(req, nullptr)) {
        DWORD status = 0, sz = sizeof(status);
        WinHttpQueryHeaders(req, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
                            WINHTTP_HEADER_NAME_BY_INDEX, &status, &sz, WINHTTP_NO_HEADER_INDEX);
        if (status == 200) {
            DWORD avail = 0;
            while (WinHttpQueryDataAvailable(req, &avail) && avail) {
                std::string buf(avail, '\0');
                DWORD rd = 0;
                if (!WinHttpReadData(req, &buf[0], avail, &rd) || !rd) break;
                out.append(buf, 0, rd);
                if (out.size() > 2 * 1024 * 1024) break;
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
        std::string js = HttpGet(host, q);
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
            js = HttpGet(host, L"/api/search?q=" + UrlEnc(Utf8(a1 + L" " + job->title)));
            pick(js);
        }
        if (!synced.empty() || !plain.empty()) {
            ParseLyrics(synced, plain, res->lines, res->synced);
            if (!res->lines.empty()) res->state = 2;
        } else if (instrumental) {
            res->state = 4;
        }
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
    LyJob* job = new LyJob{g_lyKey, g_artist, g_title, (g_durTicks + 5000000) / 10000000};
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
    for (int i = 1; i < 8; i++) busy |= Approach(g_hotT[i], g_hot == i ? 1.0 : 0.0, ms / 140.0);
    for (int i = 1; i < 6; i++) busy |= Approach(g_pressT[i], g_pressed == i ? 1.0 : 0.0, ms / 90.0);

    // панель текста песни: открытие и плавная прокрутка к текущей строке
    if (!g_expand && g_lyOpen) g_lyOpen = false;  // при сворачивании плеера панель закрывается
    busy |= Approach(g_lyT, g_lyOpen ? 1.0 : 0.0, ms / 380.0);
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
    int lyFull;          // полная высота панели текста
    int H;
};

static ExpLay CalcExpLay() {
    ExpLay L = {};
    const int W = g_we;
    const int pad = S(14), cs = CoverExpSize();
    // место под панель текста песни сверху (растёт при её открытии)
    const int lyFull = S(176), lyGap = S(10);
    const int yo = (int)std::lround((lyFull + lyGap) * Smooth01(g_lyT));
    const int top = pad + yo;
    L.lyFull = lyFull;
    L.lyrics = {pad, pad, W - pad, pad + std::max(0, yo - lyGap)};
    L.cover = {pad, top, pad + cs, top + cs};

    const int tx = pad + cs + S(12);
    const int lh1 = S(g_set.fontSize + 2) + S(6), lh2 = S(g_set.fontSize) + S(4);
    const int ty = top + (cs - lh1 - lh2) / 2;
    L.title = {tx, ty, W - pad, ty + lh1};
    L.artist = {tx, ty + lh1, W - pad, ty + lh1 + lh2};

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

// обложка cs x cs в точке (cx,cy) со скруглением, билинейным масштабированием и прозрачностью
static void DrawCover(DWORD* px, int cx, int cy, int cs, float rad, const std::vector<DWORD>& src, int sp,
                      float opacity) {
    if (cs <= 0 || sp <= 0 || src.empty() || opacity <= 0.f) return;
    const float sc = (float)sp / (float)cs;
    for (int y = 0; y < cs; y++) {
        int dy = cy + y;
        if (dy < 0 || dy >= g_h) continue;
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
    const double lyFade = Smooth01((g_lyT - 0.25) / 0.75) * stage(0.10);
    RECT lp = L.lyrics;
    OffsetRect(&lp, g_oxE, g_oyE);
    const bool lyVisible = showExp && (lp.bottom - lp.top) > 2;
    if (showExp) {
        const unsigned tR = U8(P.text.r), tG = U8(P.text.g), tB = U8(P.text.b);
        if (lyVisible)
            FillRR(px, (float)lp.left, (float)lp.top, (float)lp.right, (float)lp.bottom, (float)S(14), 0, 0, 0,
                   (unsigned)I255(0.24 * lyFade));
        const unsigned fa = I255(stage(0.30));
        RECT b = T(L.lyBtn, stage(0.30));
        float cx = (b.left + b.right) * 0.5f, cy = (b.top + b.bottom) * 0.5f;
        float a = 0.06f + (float)(g_hotT[7] * 0.10 + g_pressT[5] * 0.12) + (float)lyGeo * 0.16f;
        FillRR(px, (float)b.left, (float)b.top, (float)b.right, (float)b.bottom, (float)S(10), tR, tG, tB,
               (unsigned)(fa * std::min(a, 1.f)));
        Rgb ic = MixRgb(P.text, P.accent, (float)lyGeo);
        float lw = (float)S(16), th = std::max(1.5f, (float)S(2)), sp = (float)S(5);
        float x0 = cx - lw * 0.5f;
        const float ws[3] = {1.0f, 0.62f, 0.84f};
        for (int k = 0; k < 3; k++) {
            float yy = cy + (k - 1) * sp;
            FillRR(px, x0, yy - th * 0.5f, x0 + lw * ws[k], yy + th * 0.5f, th * 0.5f, U8(ic.r), U8(ic.g), U8(ic.b), fa);
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
        int textW = std::max(measure(fTitle, g_title), measure(fArtist, g_artist));

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
        DrawTextRect(mm, fArtist, RGB(0, I255(tt * 170.0 / 255.0), 0), g_artist.c_str(), r2,
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
        DrawTextRect(mm, fTm, RGB(0, I255(sB), 0), FmtTime(posShown).c_str(), T(L.tCur, sB), DT_LEFT | DT_VCENTER);
        if (dur > 0)
            DrawTextRect(mm, fTm, RGB(0, I255(sB), 0), FmtTime(dur).c_str(), T(L.tTot, sB), DT_RIGHT | DT_VCENTER);
        DrawTextRect(mm, fIc, RGB(0, 0, I255(sC)), L"\uE100", T(L.prev, sC), DT_CENTER | DT_VCENTER);
        DrawTextRect(mm, fIc, RGB(0, 0, I255(sC)), L"\uE101", T(L.next, sC), DT_CENTER | DT_VCENTER);
        DrawTextRect(mm, fVi, RGB(0, 0, I255(sV)), (g_muted || g_vol <= 0.001f) ? L"\uE74F" : L"\uE767",
                     T(L.volIcon, sV), DT_CENTER | DT_VCENTER);

        if (lyVisible) {  // строки текста: яркость зависит от близости к текущей строке
            HFONT fL = MkFont(S(g_set.fontSize + 3), FW_SEMIBOLD, L"Segoe UI");
            const int padX = S(14);
            const int tw = std::max(10, (int)(lp.right - lp.left) - 2 * padX);
            HRGN rg = CreateRectRgn(lp.left, lp.top, lp.right, lp.bottom);
            SelectClipRgn(mm, rg);
            if (g_lyState == 2 && !g_lyrics.empty()) {
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
                const double f = std::clamp(g_lyScroll, 0.0, (double)(n - 1));
                int base;
                if (g_lySynced) {  // текущая строка — по центру панели
                    int i0 = (int)floor(f);
                    double fr = f - i0;
                    double c0 = yv[i0] + (g_lyH[i0] - lineGap) * 0.5;
                    double c1 = i0 + 1 < n ? yv[i0 + 1] + (g_lyH[i0 + 1] - lineGap) * 0.5 : c0;
                    double cf = c0 + (c1 - c0) * fr;
                    base = (int)std::lround((lp.bottom - L.lyFull / 2) - cf + g_lyManual);
                } else {  // обычный текст: с начала, колесо — прокрутка
                    double minM = std::min(0.0, (double)L.lyFull - yv[n] - S(20));
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
                SelectObject(mm, oldF);
            } else {
                const wchar_t* msg = g_lyState == 1   ? L"Загружаю текст…"
                                     : g_lyState == 4 ? L"Инструментал"
                                                      : L"Текст не найден";
                DrawTextRect(mm, fA, RGB(0, I255(lyFade), 0), msg, lp, DT_CENTER | DT_VCENTER);
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
// обычные развёрнутые окна и всё, что открыто на другом мониторе.
static bool IsFullscreenOtherApp() {
    HWND fg = GetForegroundWindow();
    if (!fg || fg == g_hwnd) return false;
    if (!IsWindowVisible(fg) || IsIconic(fg) || IsZoomed(fg)) return false;

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
    return r.left <= mi.rcMonitor.left && r.top <= mi.rcMonitor.top &&
           r.right >= mi.rcMonitor.right && r.bottom >= mi.rcMonitor.bottom;
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
    if (wasHidden || moved || GetWindow(h, GW_HWNDPREV))
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
    StartAnim(h);
    if (!g_expand || g_set.collapseMode == 2) return;
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
            int hot = 0;
            if (ExpMode()) {
                ExpLay L = CalcExpLay();
                int fx = mx - g_oxE, fy = GET_Y_LPARAM(l) - g_oyE;
                hot = In(L.prev, fx, fy) ? 1 : In(L.play, fx, fy) ? 2 : In(L.next, fx, fy) ? 3
                    : In(L.volIcon, fx, fy) ? 4 : (In(L.barHit, fx, fy) && g_durTicks > 0) ? 5
                    : In(L.volHit, fx, fy) ? 6 : In(L.lyBtn, fx, fy) ? 7 : 0;
            }
            if (hot != g_hot) {
                g_hot = hot;
                StartAnim(h);
            }
            return 0;
        }
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
                          : In(L.volIcon, fx, fy) ? 4 : In(L.lyBtn, fx, fy) ? 5 : 0;
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
                int b = In(L.prev, fx, fy) ? 1 : In(L.play, fx, fy) ? 2 : In(L.next, fx, fy) ? 3 : 0;
                if (b) {
                    DoMediaCommand(b);
                    SetTimer(h, 2, 300, nullptr);  // быстро обновить состояние
                } else if (In(L.lyBtn, fx, fy)) {  // показать/скрыть текст песни
                    g_lyOpen = !g_lyOpen;
                    if (g_lyOpen) {
                        if (g_lyState == 3) g_lyKey.clear();  // прошлая попытка не удалась — пробуем ещё раз
                        LyEnsure();
                    }
                    StartAnim(h);
                } else if (g_lyOpen && g_lySynced && g_lyState == 2 && In(L.lyrics, fx, fy)) {
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
                } else if (In(L.cover, fx, fy) || In(L.title, fx, fy) || In(L.artist, fx, fy)) {
                    ToggleApp();
                }
                return 0;
            }
            int b = HitBtn(GET_X_LPARAM(l));
            if (b) {
                DoMediaCommand(b);
                SetTimer(h, 2, 300, nullptr);
            } else {
                ToggleApp();
            }
            return 0;
        }
        case WM_RBUTTONUP:  // ПКМ — развернуть/свернуть плеер
            SetExpanded(h, !g_expand);
            return 0;
        case WM_MOUSEWHEEL:  // колесо: над панелью текста — прокрутка, иначе громкость приложения
            if (ExpMode() && !g_drag) {
                POINT wp = {GET_X_LPARAM(l), GET_Y_LPARAM(l)};
                ScreenToClient(h, &wp);
                ExpLay L = CalcExpLay();
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
                if (g_expand && !g_hover && !g_drag) {
                    g_expand = false;
                    StartAnim(h);
                }
                return 0;
            }
            if (w == 8) {  // плавное движение шкалы воспроизведения и смена строки текста
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
            }
            delete r;
            LyEnsure();  // трек мог смениться, пока шла загрузка
            StartAnim(h);
            return 0;
        }
        case WM_APP + 2:  // событие смены трека/состояния — небольшой debounce
            SetTimer(h, 2, 40, nullptr);
            return 0;
        case WM_APP + 3:  // смена активного/свёрнутого окна
        case WM_DISPLAYCHANGE:
        case WM_DPICHANGED:
            Present(h, false);
            return 0;
        case WM_APP + 1:
            LoadSettings();
            Layout();  // новая геометрия нужна до Poll, чтобы обложка пересчиталась сразу
            Tick(h);
            return 0;
        case WM_CLOSE:
            DestroyWindow(h);
            return 0;
        case WM_DESTROY:
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
        WaitForSingleObject(g_thread, 5000);
        CloseHandle(g_thread);
        g_thread = nullptr;
    }
    if (g_lyThread) {  // поток загрузки текста должен завершиться до выгрузки мода
        WaitForSingleObject(g_lyThread, 8000);
        CloseHandle(g_lyThread);
        g_lyThread = nullptr;
    }
}