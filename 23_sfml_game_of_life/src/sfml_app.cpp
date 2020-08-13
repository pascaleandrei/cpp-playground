#include "sfml_app.h"

#include <iostream>
#include <thread>

SfmlApp::SfmlApp(
	std::pair<unsigned int, unsigned int> window_size,
	std::pair<unsigned int, unsigned int> cell_size
)
	: window_({ window_size.first, window_size.second }, "My window")
	, cell_size_(cell_size)
{
}

SfmlApp::~SfmlApp()
{
}

std::string getMessageForActiveStatus(bool active)
{
	if (active)
	{
		return "Simulation is ACTIVE. Press <SPACE> to toggle the simulation. Press <ESC> to exit.";
	}

	return "Simulation is PAUSED. Press <SPACE> to toggle the simulation. Press <ESC> to exit.";
}

void SfmlApp::drawBlinker(int x, int y) {
	if (x >= world_size_.first - 2) {
		return;
	}
	setCell(x, y, 1);
	setCell(x + 1, y, 1);
	setCell(x + 2, y, 1);
}

void SfmlApp::init(std::tuple<int, int, int> livingCellColor, std::tuple<int, int, int> deadCellColor)
{
	this->living_cell_color_ = sf::Color(std::get<0>(livingCellColor), std::get<1>(livingCellColor), std::get<2>(livingCellColor));
	this->dead_cell_color_ = sf::Color(std::get<0>(deadCellColor), std::get<1>(deadCellColor), std::get<2>(deadCellColor));

	if (font_.loadFromFile("../../data/OpenSans-Regular.ttf"))
	{
		gui_text_.setFont(font_);
		gui_text_.setString(getMessageForActiveStatus(true));
		gui_text_.move(10, 2);
		gui_text_.setCharacterSize(24);
		gui_text_.setOutlineColor(sf::Color::White);
	}
	else
	{
		// error...cannot render GUI
	}

	// Note: you can also override the window size after it is initialized.
	// This could be useful to allow changing the resolution and the world size after the initialization
	// but is not really a priority for this exercise.
	//this->window.setSize(sf::Vector2u(1200, 600));

	// Initialize the vertices
	size_t max_width = static_cast<size_t>(window_.getView().getSize().x);
	size_t max_height = static_cast<size_t>(window_.getView().getSize().y);

	// Store the world size for later use.
	world_size_.first = max_width / cell_size_.first - 1;
	world_size_.second = max_height / cell_size_.second - 1;

	// Initialize the world map with vertices.
	for (size_t cell_y = 0; cell_y < max_height / cell_size_.first - 1; cell_y++)
	{
		for (size_t cell_x = 0; cell_x < max_width / cell_size_.second - 1; cell_x++)
		{
			this->addVertexQuad(cell_x, cell_y, cell_size_.first, cell_size_.second);
		}
	}

	matrix = new int* [world_size_.second];
	for (int index1 = 0; index1 < world_size_.second; ++index1) {
		matrix[index1] = new int[world_size_.first];
		for (int index2 = 0; index2 < world_size_.first; ++index2) {
			matrix[index1][index2] = 0;
		}
	}

	//Draw a blinker
	const int COORD = 20;
	drawBlinker(COORD, COORD);
}

void SfmlApp::run(unsigned duration_in_millis_between_updates, unsigned ticks)
{
	unsigned time_elapsed_since_update = 0;
	bool simulation_active(true);
	unsigned count = 0;

	// run the program as long as the window is open
	while (window_.isOpen() && count < ticks)
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window_.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
			{
				window_.close();
			}

			// quick close via the ESC key.
			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window_.close();
					return;
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					// Toggle simulation
					simulation_active = !simulation_active;
					gui_text_.setString(getMessageForActiveStatus(simulation_active));
				}
			}
			if (event.type == sf::Event::MouseButtonReleased)
			{
				if (event.mouseButton.button == sf::Mouse::Button::Left)
				{
					size_t view_width = static_cast<size_t>(window_.getView().getSize().x);
					size_t view_height = static_cast<size_t>(window_.getView().getSize().y);
					size_t win_width = static_cast<size_t>(window_.getSize().x);
					size_t win_height = static_cast<size_t>(window_.getSize().y);

					unsigned clicked_cell_x = event.mouseButton.x * view_width / (cell_size_.first * win_width);
					unsigned clicked_cell_y = event.mouseButton.y * view_height / (cell_size_.second * win_height);

					setCell(clicked_cell_x, clicked_cell_y, !matrix[clicked_cell_y][clicked_cell_x]);
				}
			}
		}

		if (time_elapsed_since_update > duration_in_millis_between_updates)
		{
			updateWorld();
			time_elapsed_since_update -= duration_in_millis_between_updates;
			count++;
		}

		// clear the window with black color
		window_.clear(sf::Color::Black);

		render();

		// end the current frame
		window_.display();

		// don't consume too many cpu cycles
		std::this_thread::sleep_for(std::chrono::duration<int, std::milli>(10));
		if (simulation_active)
		{
			time_elapsed_since_update += 10;
		}
	}
}

void SfmlApp::setCellColor(unsigned cell_x, unsigned cell_y, sf::Color color)
{
	auto index = (cell_y * this->world_size_.first + cell_x) * 4;
	if (index >= cell_vertices_.size())
	{
		return;
	}

	cell_vertices_[index].color = color;

	for (int i = 0; i < 4; i++)
	{
		cell_vertices_[index + i].color = color;
	}
}

void SfmlApp::setCell(unsigned cell_x, unsigned cell_y, int newValue) {
	matrix[cell_y][cell_x] = newValue;
	setCellColor(cell_x, cell_y, newValue == 1 ? living_cell_color_ : dead_cell_color_);
}

void SfmlApp::addVertexQuad(unsigned cell_x, unsigned cell_y, unsigned width, unsigned height)
{
	sf::Vertex topLeft;
	sf::Vertex topRight;
	sf::Vertex bottomLeft;
	sf::Vertex bottomRight;

	float pixel_x = static_cast<float>(cell_x * width);
	float pixel_y = static_cast<float>(cell_y * height);

	topLeft.position = { pixel_x,			 pixel_y };
	topRight.position = { pixel_x + width,   pixel_y };
	bottomLeft.position = { pixel_x,           pixel_y + height };
	bottomRight.position = { pixel_x + width,   pixel_y + height };

	topLeft.color = dead_cell_color_;
	topRight.color = dead_cell_color_;
	bottomLeft.color = dead_cell_color_;
	bottomRight.color = dead_cell_color_;

	cell_vertices_.push_back(topLeft);
	cell_vertices_.push_back(bottomLeft);
	cell_vertices_.push_back(bottomRight);
	cell_vertices_.push_back(topRight);
}

void SfmlApp::render()
{
	// draw everything here...
	window_.draw(cell_vertices_.data(), cell_vertices_.size(), sf::Quads);

	// Add any GUI info.
	window_.draw(gui_text_);
}

int SfmlApp::getNeighboursCount(int x, int y) {
	int count = 0;
	if (x > 0) {
		if (y > 0) {
			count += matrix[y - 1][x - 1];
		}
		count += matrix[y][x - 1];
		if (y < world_size_.second - 1) {
			count += matrix[y + 1][x - 1];
		}
	}
	if (x < world_size_.first - 1) {
		if (y > 0) {
			count += matrix[y - 1][x + 1];
		}
		count += matrix[y][x + 1];
		if (y < world_size_.second - 1) {
			count += matrix[y + 1][x + 1];
		}
	}
	if (y > 0) {
		count += matrix[y - 1][x];
	}
	if (y < world_size_.second - 1) {
		count += matrix[y + 1][x];
	}
	return count;
}

void SfmlApp::updateWorld()
{
	int** temp, newValue;
	temp = new int* [world_size_.second];
	for (int index1 = 0; index1 < world_size_.second; ++index1) {
		temp[index1] = new int[world_size_.first];
		for (int index2 = 0; index2 < world_size_.first; ++index2) {
			temp[index1][index2] = getNeighboursCount(index2, index1);
		}
	}
	for (int index1 = 0; index1 < world_size_.second; ++index1) {
		for (int index2 = 0; index2 < world_size_.first; ++index2) {
			if (matrix[index1][index2] == 1) {
				newValue = temp[index1][index2] >= INFERIOR_LIMIT && temp[index1][index2] <= SUPERIOR_LIMIT;
			}
			else {
				newValue = temp[index1][index2] == SUPERIOR_LIMIT;
			}
			setCell(index2, index1, newValue);
		}
		delete[] temp[index1];
	}
	delete[] temp;
}