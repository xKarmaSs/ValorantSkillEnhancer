#pragma once
#include "windows.h"
#include "Settings.h"
#include "ScreenCapture.h"
#include <vector>

class EnemyScanner {
private:
	ScreenCapture* _sc;

public:
	EnemyScanner(ScreenCapture& sc);

	void update() {
		if (!_sc)
			return;
		_sc->screenshot();
	}

	//std::vector<int[]> getEnemyHeads()  const;

	bool isEnemyOutline(const std::vector<int>& pixel) const;

	bool isEnemyOutlineNeighbor(const std::vector<int>& pixel, const std::vector<std::vector<int>>& neighborPixels) const;

	std::vector<int> findClosestEnemyHead() const;

	std::vector<std::vector<int>> getNeighboringPixels(int coordx, int coordy) const;

	bool isEnemyAtCrosshair() const;



};

