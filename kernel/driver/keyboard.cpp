#include "intr/interruption.h"
#include "keyboard.h"
#include "io/io.h"
#include "sys/io.h"
#include "libs/keymap.h"

namespace os {
namespace keyboard {

namespace hidden {

constexpr uint16_t KEYBOARD_DATA = 0x60;

void keyboardHandler(uint32_t) {
	size_t scanCode = inb(KEYBOARD_DATA);
	if (scanCode & FLAG_BREAK) {
		os::intr::endInterruption();
		return;
	}
	uint32_t keyCode = keymap[scanCode * MAP_COLS];
	if (keyCode > FLAG_EXT) {
		os::intr::endInterruption();
		return;
	}
	char keyChar = static_cast<char>(keyCode);
	// os::io::cout << keyChar << os::io::endl;
	os::intr::endInterruption();
}

}

void initKeyboard() {
	os::intr::intrHandlers[os::intr::Interruption::keyboard] = hidden::keyboardHandler;
}

} // keyboard
} // os