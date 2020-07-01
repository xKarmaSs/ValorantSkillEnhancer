#pragma once
#include <iostream>

#include "windows.h"
#include "SerialPort.h"

class AdruinoMouseController {
private:
	SerialPort sp;
public:
	AdruinoMouseController(const char* port);

	void moveTo(int x, int y);

	void click();

}; 
