#include <math.h>

#include "EnemyScanner.h"
#include "Helpers.h"

EnemyScanner::EnemyScanner(ScreenCaptureBase& sc) {
	_sc = &sc;
}

const int NEIGHBORCOORDS[8][2] = { {-1,-1}, {1, -1}, {-1, 1}, {1, 1}, {-1,0}, {1,0}, {0,-1}, {0, 1} };

bool EnemyScanner::isEnemyOutline(const std::vector<int>& pixel) const {
	float max = (float)max(pixel[0], pixel[1], pixel[2]);
	float dominance[3] = { pixel[0] / max, pixel[1] / max, pixel[2] / max };
	if (pixel[0] >= RED_RANGE[0] && pixel[0] <= RED_RANGE[1] &&
		pixel[1] >= GREEN_RANGE[0] && pixel[1] <= GREEN_RANGE[1] &&
		pixel[2] >= BLUE_RANGE[0] && pixel[2] <= BLUE_RANGE[1] && 
		dominance[0] >= RED_DOMINANCE_THRES[0] && dominance[0] <= RED_DOMINANCE_THRES[1] &&
		dominance[1] >= GREEN_DOMINANCE_THRES[0] && dominance[1] <= GREEN_DOMINANCE_THRES[1] &&
		dominance[2] >= BLUE_DOMINANCE_THRES[0] && dominance[2] <= BLUE_DOMINANCE_THRES[1])
		return true;


	return false;
}

bool EnemyScanner::isEnemyOutlineNeighbor(const std::vector<int>& pixel, const std::vector<std::vector<int>>& neighborPixels) const {
//	for (const std::vector<int>& neighborPixel : neighborPixels) {
//		if (pixel[0] - neighborPixel[0] >= RED_DOMINANCE_THRES &&
//			pixel[1] - neighborPixel[1] >= GREEN_DOMINANCE_THRES &&
//			pixel[2] - neighborPixel[2] >= BLUE_DOMINANCE_THRES)
//			return true;
//	}
	return false;
}

std::vector<int> EnemyScanner::findClosestEnemyHead() const {
	int centerX = _sc->getWidth() / 2;
	int centerY = _sc->getHeight() / 2;
	int x, y;
	for (y = centerY - AIMBOT_FOV; y < centerY + AIMBOT_FOV; ++y) {
		for (x = centerX - AIMBOT_FOV; x < centerX + AIMBOT_FOV; ++x) {
			if (isEnemyOutline(_sc->getRGB(x, y))) {
				return { x + HEAD_OFFSET_X, y + HEAD_OFFSET_Y};
			}
		}
	}
	return { -1, -1 };
}

std::vector<std::vector<int>> EnemyScanner::getNeighboringPixels(int coordx, int coordy) const {
	std::vector<std::vector<int>> result;
	int i, x, y;
	for (i = 0; i < 8; ++i) {
		x = max(0, min(_sc->getWidth() + NEIGHBORCOORDS[i][0], coordx + NEIGHBORCOORDS[i][0]));
		y = max(0, min(_sc->getHeight() + NEIGHBORCOORDS[i][1], coordy + NEIGHBORCOORDS[i][1]));
		result.push_back(_sc->getRGB(x, y));
	}
	return result;
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
