#include "SFML/Graphics.hpp"

#include "sfml_app.h"

#include <SimpleIni.h>

#define WORLD_SECTION_NAME "world"
#define LIVING_CELL_SECTION_NAME "living_cell"
#define DEAD_CELL_SECTION_NAME "dead_cell"

#ifdef _WIN32
#include <windows.h>
#endif

void hide_console()
{
#ifdef _WIN32
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
#endif
}

void readDataFromIniFile(const std::string& file_name,
	std::pair<unsigned int, unsigned int>& preferred_window_size,
	std::pair<unsigned int, unsigned int>& preferred_cell_size,
	unsigned& duration_in_millis_between_updates,
	unsigned& ticks,
	std::tuple<int, int, int>& livingCellColor,
	std::tuple<int, int, int>& deadCellColor)
{
	CSimpleIniA ini;
	ini.LoadFile(file_name.c_str());
	preferred_window_size.first = std::stoi(ini.GetValue(WORLD_SECTION_NAME, "window_size.x"));
	preferred_window_size.second = std::stoi(ini.GetValue(WORLD_SECTION_NAME, "window_size.y"));
	preferred_cell_size.first = std::stoi(ini.GetValue(WORLD_SECTION_NAME, "cell_size.x"));
	preferred_cell_size.second = std::stoi(ini.GetValue(WORLD_SECTION_NAME, "cell_size.y"));
	ticks = std::stoi(ini.GetValue(WORLD_SECTION_NAME, "ticks"));
	duration_in_millis_between_updates = std::stoi(ini.GetValue(WORLD_SECTION_NAME, "duration_between_updates"));
	livingCellColor = std::make_tuple(std::stoi(ini.GetValue(LIVING_CELL_SECTION_NAME, "red")),
		std::stoi(ini.GetValue(LIVING_CELL_SECTION_NAME, "green")),
		std::stoi(ini.GetValue(LIVING_CELL_SECTION_NAME, "blue")));
	deadCellColor = std::make_tuple(std::stoi(ini.GetValue(DEAD_CELL_SECTION_NAME, "red")),
		std::stoi(ini.GetValue(DEAD_CELL_SECTION_NAME, "green")),
		std::stoi(ini.GetValue(DEAD_CELL_SECTION_NAME, "blue")));
}

int main()
{
	hide_console();

	std::pair<unsigned int, unsigned int> preferred_window_size;
	std::pair<unsigned int, unsigned int> preferred_cell_size;
	unsigned duration_in_millis_between_updates, ticks;
	std::tuple<int, int, int> livingCellColor, deadCellColor;

	std::string file_name("../../data/23_sfml_game_of_life.ini");
	readDataFromIniFile(file_name, preferred_window_size, preferred_cell_size, duration_in_millis_between_updates, ticks, livingCellColor, deadCellColor);

	SfmlApp app(preferred_window_size, preferred_cell_size);
	app.init(livingCellColor, deadCellColor);
	app.run(duration_in_millis_between_updates, ticks);
}