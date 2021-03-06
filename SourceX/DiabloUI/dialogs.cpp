#include "DiabloUI/dialogs.h"

#include "controls/menu_controls.h"
#include "devilution.h"
#include "dx.h"
#include "DiabloUI/diabloui.h"
#include "DiabloUI/button.h"
#include "DiabloUI/fonts.h"
#include "DiabloUI/errorart.h"

namespace dvl {

extern HANDLE diabdat_mpq;
extern SDL_Surface *pal_surface;

namespace {

Art dialogArt;
Art progressArt;
char dialogText[256];
char dialogCaption[1024];
bool fontWasLoaded;
bool textInputWasActive;

UiItem *dialogItems;
std::size_t dialogItemsSize;

enum class State {
	DEFAULT = 0,
	OK,
	CANCEL,
};

State state;

void DialogActionOK()
{
	state = State::OK;
}

void DialogActionCancel()
{
	state = State::CANCEL;
}

constexpr auto DIALOG_ART_S = UiImage(&dialogArt, { 180, 168, 280, 144 });
constexpr auto DIALOG_ART_L = UiImage(&dialogArt, { 127, 100, 385, 280 });

UiItem OK_DIALOG[] = {
	DIALOG_ART_S,
	UiText(dialogText, { 200, 211, 240, 80 }, UIS_CENTER),
	MakeSmlButton("OK", &DialogActionOK, 265, 265),
};

UiItem OK_DIALOG_WITH_CAPTION[] = {
	DIALOG_ART_L,
	UiText(dialogText, SDL_Color{ 255, 255, 0, 0 }, { 147, 110, 345, 20 }, UIS_CENTER),
	UiText(dialogCaption, { 147, 141, 345, 190 }, UIS_CENTER),
	MakeSmlButton("OK", &DialogActionOK, 264, 335),
};

UiItem PROGRESS_DIALOG[] = {
	DIALOG_ART_S,
	UiText(dialogText, { 180, 177, 280, 43 }, UIS_CENTER),
	UiImage(&progressArt, { 205, 220, 228, 38 }),
	MakeSmlButton("Cancel", &DialogActionCancel, 330, 265),
};

UiListItem SELOK_DIALOG_ITEMS[] = {
	{ "OK", 0 }
};
UiItem SELOK_DIALOG[] = {
	UiText(dialogText, { 140, 210, 400, 168 }, UIS_CENTER),
	UiList(SELOK_DIALOG_ITEMS, 230, 390, 180, 35, UIS_CENTER),
};

UiItem SPAWNERR_DIALOG[] = {
	UiText("The Rogue and Sorcerer are only available in the full retail version of Diablo. For ordering information visit https://www.gog.com/game/diablo.", { 140, 199, 400, 177 }),
	UiArtTextButton("OK", &DialogActionOK, { 230, 407, 180, 43 }),
};

// clang-format off
#define BLANKCOLOR { 0, 0xFF, 0, 0 }
// clang-format on

void LoadFallbackPalette()
{
	// clang-format off
	static const PALETTEENTRY fallback_palette[256] = {
		{ 0x00, 0x00, 0x00, 0 },
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR,
		{ 0xff, 0xfd, 0x9f, 0 },
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		{ 0xe8, 0xca, 0xca, 0 },
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		{ 0x05, 0x07, 0x0c, 0 },
		{ 0xff, 0xe3, 0xa4, 0 },
		{ 0xee, 0xd1, 0x8c, 0 },
		{ 0xdd, 0xc4, 0x7e, 0 },
		{ 0xcc, 0xb7, 0x75, 0 },
		{ 0xbc, 0xa8, 0x6c, 0 },
		{ 0xab, 0x9a, 0x63, 0 },
		{ 0x98, 0x8b, 0x5d, 0 },
		{ 0x87, 0x7e, 0x54, 0 },
		{ 0x78, 0x6f, 0x49, 0 },
		{ 0x69, 0x60, 0x3f, 0 },
		{ 0x5b, 0x51, 0x34, 0 },
		{ 0x48, 0x40, 0x27, 0 },
		{ 0x39, 0x31, 0x1d, 0 },
		{ 0x31, 0x28, 0x16, 0 },
		{ 0x1a, 0x14, 0x08, 0 },
		{ 0x14, 0x0b, 0x00, 0 },
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR,
		{ 0xff, 0xbd, 0xbd, 0 },
		{ 0xf4, 0x96, 0x96, 0 },
		{ 0xe8, 0x7d, 0x7d, 0 },
		{ 0xe0, 0x6c, 0x6c, 0 },
		{ 0xd8, 0x5b, 0x5b, 0 },
		{ 0xcf, 0x49, 0x49, 0 },
		{ 0xc7, 0x38, 0x38, 0 },
		{ 0xbf, 0x27, 0x27, 0 },
		{ 0xa9, 0x22, 0x22, 0 },
		{ 0x93, 0x1e, 0x1e, 0 },
		{ 0x7c, 0x19, 0x19, 0 },
		{ 0x66, 0x15, 0x15, 0 },
		{ 0x4f, 0x11, 0x11, 0 },
		{ 0x39, 0x0d, 0x0d, 0 },
		{ 0x23, 0x09, 0x09, 0 },
		{ 0x0c, 0x05, 0x05, 0 },
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR,
		{ 0xf3, 0xf3, 0xf3, 0 },
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR,
		{ 0xff, 0xff, 0x00, 0 },
		BLANKCOLOR, BLANKCOLOR, BLANKCOLOR,
		BLANKCOLOR,
	};
	// clang-format on
	ApplyGamma(logical_palette, fallback_palette, 256);
}

void Init(const char *text, const char *caption, bool error)
{
	strcpy(dialogText, text);
	if (caption == nullptr) {
		LoadMaskedArt(error ? "ui_art\\srpopup.pcx" : "ui_art\\spopup.pcx", &dialogArt);
		dialogItems = OK_DIALOG;
		dialogItemsSize = size(OK_DIALOG);
	} else {
		if (error) {
			LoadArt(&dialogArt, popupData, DIALOG_ART_L.rect.w, DIALOG_ART_L.rect.h);
		} else {
			LoadMaskedArt("ui_art\\lpopup.pcx", &dialogArt);
		}
		strcpy(dialogCaption, caption);
		dialogItems = OK_DIALOG_WITH_CAPTION;
		dialogItemsSize = size(OK_DIALOG_WITH_CAPTION);
	}
	LoadSmlButtonArt();

	fontWasLoaded = font != nullptr;
	if (!fontWasLoaded)
		LoadTtfFont();
	textInputWasActive = SDL_IsTextInputActive();
	SDL_StopTextInput();
}

void Deinit()
{
	dialogArt.Unload();
	UnloadSmlButtonArt();
	if (!fontWasLoaded)
		UnloadTtfFont();
	if (textInputWasActive)
		SDL_StartTextInput();
	for (std::size_t i = 0; i < dialogItemsSize; ++i) {
		dialogItems[i].FreeCache();
	}
}

void DialogLoop(UiItem *items, std::size_t num_items, UiItem *render_behind, std::size_t render_behind_size)
{
	SDL_Event event;
	state = State::DEFAULT;
	if (render_behind_size == 0) {
		LoadBackgroundArt("ui_art\\black.pcx");
		if (ArtBackground.surface == nullptr) {
			LoadFallbackPalette();
		}
	}
	do {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				UiItemMouseEvents(&event, items, num_items);
				break;
			default:
				switch (GetMenuAction(event)) {
				case MenuAction::BACK:
				case MenuAction::SELECT:
					state = State::OK;
					break;
				default:
					break;
				}
				break;
			}
			UiHandleEvents(&event);
		}

		if (render_behind_size == 0) {
			SDL_FillRect(pal_surface, nullptr, 0);
		} else {
			UiRenderItems(render_behind, render_behind_size);
		}
		UiRenderItems(items, num_items);
		DrawMouse();
		UiFadeIn();
	} while (state == State::DEFAULT);
}

} // namespace

void UiOkDialog(const char *text, const char *caption, bool error, UiItem *render_behind, std::size_t render_behind_size)
{
	static bool inDialog = false;

	if (!gbActive || inDialog) {
		if (SDL_ShowCursor(SDL_ENABLE) <= -1) {
			SDL_Log(SDL_GetError());
		}
		if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, text, caption, NULL) <= -1) {
			SDL_Log(SDL_GetError());
			SDL_Log(text);
			SDL_Log(caption);
		}
		return;
	}

	inDialog = true;
	Init(text, caption, error);
	DialogLoop(dialogItems, dialogItemsSize, render_behind, render_behind_size);
	Deinit();
	inDialog = false;
}

void UiErrorOkDialog(const char *text, const char *caption, UiItem *render_behind, std::size_t render_behind_size)
{
	UiOkDialog(text, caption, /*error=*/true, render_behind, render_behind_size);
}

void UiErrorOkDialog(const char *text, const char *caption, bool error)
{
	UiOkDialog(text, caption, error, nullptr, 0);
}

void UiErrorOkDialog(const char *text, UiItem *render_behind, std::size_t render_behind_size)
{
	UiErrorOkDialog(text, nullptr, render_behind, render_behind_size);
}

} // namespace dvl
