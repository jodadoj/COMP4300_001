#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"


int main(int argc, char* argv[]) {
	
	std::string filename = "config/config.txt";
	std::ifstream config(filename);

	if (!config.is_open()) {
		std::cerr << "Error opening configuration file!" << std::endl;
		exit(-1);
	}

	std::vector<std::vector<int>> circles;
	std::vector<std::vector<int>> rectangles;
	std::string line;
	
	int wWidth, wHeight;

	std::string fontPath;
	int fontSize, fontR, fontG, fontB;

	//new block - variables defined within this block do not exist outside hence all useful data must be defined above here

	if (std::getline(config, line)) {
		std::stringstream ss(line);
		std::string isWindow;

		ss >> isWindow;

		if (isWindow != "Window") {
			std::cerr << "Error: The configuration file does not specify a window!" << std::endl;
			exit(-1);
		}

		if (!(ss >> wWidth >> wHeight)) {
			std::cerr << "Error: The configuration file does not specify a window width and height!" << std::endl;
			exit(-1);
		}

	}

	if (std::getline(config, line)) {
		std::stringstream ss(line);
		std::string isFont;

		ss >> isFont;

		if (isFont != "Font") {
			std::cerr << "Error: The configuration file does not specify a font!" << std::endl;
			exit(-1);
		}

		if (!(ss >> fontPath >> fontSize >> fontR >> fontG >> fontB)) {
			std::cerr << "Error: The configuration file does not specify correct font data!" << std::endl;
			exit(-1);
		}

	}

	while (std::getline(config, line)) {
		std::stringstream ss(line);
		std::string name;
		int value;

		std::vector<int> values;

		ss >> name;

		if (name != "Circle") {
			ss >> name;

			while (ss >> value) {
				values.push_back(value);
			}

			circles.push_back(values);
		}
		if (name != "Rectangle") {
			ss >> name;

			while (ss >> value) {
				values.push_back(value);
			}

			rectangles.push_back(values);
		}
		else {
			std::cerr << "Error: The shape data was not properly configured!" << std::endl;
			exit(-1);
		}

	}
	
	config.close();

	//create window w*h
	//top left is 0,0 bot right is w,h
	//read from config
	sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");
	window.setFramerateLimit(60); //60fps cap

	//init imgui and create internal clock
	ImGui::SFML::Init(window);
	sf::Clock deltaClock;

	//scale imgui ui by given factor, sans text size
	ImGui::GetStyle().ScaleAllSizes(1.0f);

	//the imgui color wheel RGB requires floats from 0-1 rather than 0-255
	float c[3] = { 0.0f, 1.0f, 1.0f }; //RGB

	//make shape
	float circleRadius = 50;
	int circleSegments = 32;
	float circleSpeedX = 1.0f;
	float circleSpeedY = 0.5f;
	bool drawCircle = true;
	bool drawText = true;

	//create the sfml circle
	sf::CircleShape circle(circleRadius, circleSegments);
	circle.setPosition(0.0f, 10.0f);

	//set font
	sf::Font myFont;

	//loat font from file
	if (!myFont.loadFromFile(fontPath)) {
		//if we can't load the font
		std::cerr << "Could not load font!\n";
		exit(-1);
	}

	//set up text object
	sf::Text text("Sample Text", myFont, 24);

	//put text in bottom and shift up by height
	text.setPosition(0, wHeight - (float)text.getCharacterSize());

	//set up char array for text
	char displayString[255] = "Sample text";

	//main loop - renders each frame on window
	while (window.isOpen()){
		//event handling
		sf::Event event;
		while (window.pollEvent(event)) {
			//pass event to imgui to be parsed
			ImGui::SFML::ProcessEvent(window, event);

			//this event triggers when the window closes
			if (event.type == sf::Event::Closed) {
				window.close();
			}

			//trigger on key press
			if (event.type == sf::Event::KeyPressed) {
				//print the key that was pressed to console
				std::cout << "The key that was pressed was {  " << event.key.code << "  }\n";

				// example of what happens
				if (event.key.code == sf::Keyboard::X) {
					// reverse x direction
					circleSpeedX * -1.0f;
				}
			}

			// update imgui for this frame with the time that the last frame
			ImGui::SFML::Update(window, deltaClock.restart());

			// draw the UI
			// immediate mode passes in the pointer to the address and the UI directly alters value
			ImGui::Begin("Window title");
			ImGui::Text("Window text!");
			ImGui::Checkbox("Draw Circle", &drawCircle);
			ImGui::SameLine();
			ImGui::Checkbox("Draw Text", &drawText);
			ImGui::SliderFloat("Radius", &circleRadius, 0.0f, 300.0f);
			ImGui::SliderInt("Sides", &circleSegments, 3, 64);
			ImGui::ColorEdit3("Color Circle", c);
			ImGui::InputText("Text", displayString, 255);

			if (ImGui::Button("Set Text")) {
				text.setString(displayString);
			}
			ImGui::SameLine();

			if (ImGui::Button("Reset Circle")) {
				circle.setPosition(0, 0);
			}
			ImGui::End();

			//set the circle properties in case of update with the ui
			circle.setFillColor(sf::Color(c[0] * 255.0f, c[1] * 255.0f, c[2] * 255.0f));
			circle.setPointCount(circleSegments);
			circle.setRadius(circleRadius);

			//basic animation
			//move each frame if in fram
			circle.setPosition(circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY);

			// basic rendering
			window.clear(); 
			if (drawCircle) {
				window.draw(circle);
			}
			if (drawText) {
				window.draw(text);
			}
			//draws the ui last so it is correctly on top
			ImGui::SFML::Render(window);
			window.display();
		}


	}
}

//#include "sfml/graphics.hpp"
//
//int main()
//{
//    sf::renderwindow window(sf::videomode(800, 800), "window title");
//    sf::circleshape shape(200.f, 100);
//    shape.setfillcolor(sf::color(204, 77, 5)); // color circle
//    shape.setposition(200, 200); // center circle
//
//    while (window.isopen())
//    {
//        sf::event event;
//        while (window.pollevent(event))
//        {
//            if (event.type == sf::event::closed)
//                window.close();
//        }
//
//        window.clear(sf::color(18, 33, 43)); // color background
//        window.draw(shape);
//        window.display();
//    }
//
//    return 0;
//}

//// Specify the file path
//std::string filePath = "config/example.txt";
//
//// Open the file for writing
//std::ofstream outputFile(filePath);
//
//// Check if the file is open
//if (!outputFile.is_open()) {
//	std::cerr << "Error opening file for writing!" << std::endl;
//	return 1; // Return an error code to indicate failure
//}
//
//// Write some content to the file
//outputFile << "This is an example file created in the config folder.";
//
//// Close the file
//outputFile.close();
//
//std::cout << "File created successfully: " << filePath << std::endl;
