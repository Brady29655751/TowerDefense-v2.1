#include <allegro5/allegro_font.h>
#include <memory>

#include "IObject.hpp"
#include "ChineseLabel.hpp"
#include "Point.hpp"
#include "Resources.hpp"

namespace Engine {
	ChineseLabel::ChineseLabel(const wchar_t* text, const std::string& font, int fontSize, float x, float y, unsigned char r, unsigned char g, unsigned char b, unsigned char a, float anchorX, float anchorY) :
		IObject(x, y, 0, 0, anchorX, anchorY), font(Resources::GetInstance().GetFont(font, fontSize)), Chinese_Text(al_ustr_new_from_utf16((uint16_t*)text)), Color(al_map_rgba(r, g, b, a)) {
	}
	void ChineseLabel::Draw() const {
		al_draw_ustr(font.get(), Color, Position.x - Anchor.x * GetTextWidth(), Position.y - Anchor.y * GetTextHeight(), 0, Chinese_Text);
	}
	int ChineseLabel::GetTextWidth() const {
		return al_get_ustr_width(font.get(), Chinese_Text);
	}
	int ChineseLabel::GetTextHeight() const {
		return al_get_font_line_height(font.get());
	}
}
