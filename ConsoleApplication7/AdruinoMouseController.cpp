#include <string>
#include <math.h>
#include<stdlib.h>


#include "AdruinoMouseController.h"
#include "Settings.h"
#include "Helpers.h"


AdruinoMouseController::AdruinoMouseController(const char* port) : sp(port) {}
char output[MAX_DATA_LENGTH];

void AdruinoMouseController::moveTo(int x, int y) 	{
	int center_x = WIDTH / 2;
	int center_y = HEIGHT / 2;
	std::string command = std::to_string(x- center_x) + "," + std::to_string(y- center_y);

	char* message = new char[command.size() + 1];
	copy(command.begin(), command.end(), message);
	message[command.size()] = '\n';
	sp.writeSerialPort(message, MAX_DATA_LENGTH);
	char output[MAX_DATA_LENGTH];
	sp.readSerialPort(output, MAX_DATA_LENGTH);
	std::cout << "adruino res: " << output << std::endl;
	delete[] message;

}

void AdruinoMouseController::click() {
	sp.writeByte((char)ADRUINO_MOUSE_CLICK_SIGNAL_1);
	sp.writeByte((char)ADRUINO_MOUSE_CLICK_SIGNAL_2);
}

void AdruinoMouseController::moveTowards(int x, int y) {
	int center_x = WIDTH / 2;
	int center_y = HEIGHT / 2;
	int distance = pow((center_x - x), 2) + pow(center_y - y, 2);
	sp.writeByte(roundtosignedchar((x - center_x)));
	sp.writeByte(roundtosignedchar((y - center_y)));
}
