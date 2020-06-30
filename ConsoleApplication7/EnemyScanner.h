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

	bool isEnemyAtCrosshair() const;



};

