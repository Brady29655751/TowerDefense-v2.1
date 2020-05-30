#include <functional>
#include <memory>

#include "Collider.hpp"
#include "GameEngine.hpp"
#include "Image.hpp"
#include "ImageButton.hpp"
#include "Point.hpp"
#include "Resources.hpp"

namespace Engine {
	ImageButton::ImageButton(std::string img, std::string imgIn, float x, float y, float w, float h, float anchorX, float anchorY) :
		Image(img, x, y, w, h, anchorX, anchorY), imgOut(Resources::GetInstance().GetBitmap(img)), imgIn(Resources::GetInstance().GetBitmap(imgIn)) {
		Point mouse = GameEngine::GetInstance().GetMousePosition();
		mouseIn = Collider::IsPointInBitmap(Point((mouse.x - Position.x) * GetBitmapWidth() / Size.x + Anchor.x * GetBitmapWidth(), (mouse.y - Position.y) * GetBitmapHeight() / Size.y + Anchor.y * GetBitmapHeight()), bmp);
		if (!mouseIn || !Enabled) bmp = imgOut;
		else bmp = this->imgIn;
	}
	void ImageButton::SetOnClickCallback(std::function<void(void)> onClickCallback) {
		OnClickCallback = onClickCallback;
	}
	void ImageButton::OnMouseDown(int button, int mx, int my) {
		// A small change here. I want to disable the buttons whenever it's invisible.
		// So I add "Visible".
		if ((button & 1) && mouseIn && Visible && Enabled) {
			if (OnClickCallback)
				OnClickCallback();
		}
	}
	void ImageButton::OnMouseMove(int mx, int my) {
		mouseIn = Collider::IsPointInBitmap(Point((mx - Position.x) * GetBitmapWidth() / Size.x + Anchor.x * GetBitmapWidth(), (my - Position.y) * GetBitmapHeight() / Size.y + Anchor.y * GetBitmapHeight()), bmp);
		if (!mouseIn || !Enabled) bmp = imgOut;
		else bmp = imgIn;
	}
}
