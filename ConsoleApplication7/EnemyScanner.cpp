#include "EnemyScanner.h"


EnemyScanner::EnemyScanner(ScreenCapture& sc) {
	_sc = &sc;
}

bool EnemyScanner::isEnemyOutline(const std::vector<int>& pixel) const {
	if (pixel[0] > RED_RANGE[0] && pixel[0] < RED_RANGE[1] &&
		pixel[1] > GREEN_RANGE[0] && pixel[1] < GREEN_RANGE[1] &&
		pixel[2] > BLUE_RANGE[0] && pixel[2] < BLUE_RANGE[1])
		return true;
	return false;
}



bool EnemyScanner::isEnemyAtCrosshair() const
{
	int centerX = _sc->getWidth() / 2;
	int centerY = _sc->getHeight() / 2;
	int x, y;
	for (x = centerX - TRIGGERBOT_WINDOW; x < centerX + TRIGGERBOT_WINDOW; ++x) {
		for (y = centerY - TRIGGERBOT_WINDOW; y < centerY + TRIGGERBOT_WINDOW; ++y) {
			std::vector<int> pixel = _sc->getRGB(x, y);
			if (isEnemyOutline(pixel))
				return true;
		}
	}

	return false;
}
