#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"


class Rect {
	sf::RectangleShape shape;
	float xSpeed, ySpeed = 0;
	std::string name, text = "";
	bool drawShape, drawText = true;

public:
	Rect() {}
	Rect(sf::RectangleShape rect, float x_speed, float y_speed, std::string id, 
		std::string label, bool showShape = true, bool showText = true) : 
		shape(rect), xSpeed(x_speed), ySpeed(y_speed), name(id), text(label), 
		drawShape(showShape), drawText(showText) {

	}

	sf::RectangleShape getShape() {
		return shape;
	}
	float getXSpeed() {
		return xSpeed;
	}
	float getYSpeed() {
		return ySpeed;
	}
	std::string getName() {
		return name;
	}
	std::string getText() {
		return text;
	}
	bool getDrawShape() {
		return drawShape;
	}
	bool getDrawText() {
		return drawText;
	}
	sf::Vector2f getShapePosition() {
		return shape.getPosition();
	}

	//believe it is correct to pass by ref the strings and not the floats
	//floats are max of 4 bytes whereas on 64bit systems this can be 8 for the reference
	//hence cheaper in resources to just copy the floats but put a memory cap on strings
	void setXSpeed(const float newXSpeed) {
		xSpeed = newXSpeed;
	}
	void setYSpeed(const float newYSpeed) {
		ySpeed = newYSpeed;
	}
	void setName(const std::string& newName) {
		name = newName;
	}
	void setText(const std::string& newText) {
		text = newText;
	}
	void setDrawShape() {
		drawShape = !drawShape;
	}
	void setDrawText() {
		drawText = !drawText;
	}
	void setShapePosition(float x, float y) {
		shape.setPosition(x, y);
	}
};

class Circle {
	sf::CircleShape shape;
	float xSpeed, ySpeed = 0;
	std::string name, text = "";
	bool drawShape, drawText = true;

public:
	Circle() {}
	Circle(sf::CircleShape circle, float x_speed, float y_speed, std::string id, 
		std::string label, bool showShape= true, bool showText = true) :
		shape(circle), xSpeed(x_speed), ySpeed(y_speed), name(id), text(label), 
		drawShape(showShape), drawText(showText) {

	}

	sf::CircleShape getShape(){
		return shape;
	}
	float getXSpeed() {
		return xSpeed;
	}
	float getYSpeed() {
		return ySpeed;
	}
	std::string getName() {
		return name;
	}
	std::string getText() {
		return text;
	}
	bool getDrawShape() {
		return drawShape;
	}
	bool getDrawText() {
		return drawText;
	}
	sf::Vector2f getShapePosition() {
		return shape.getPosition();
	}


	void setXSpeed(const float newXSpeed) {
		xSpeed = newXSpeed;
	}
	void setYSpeed(const float newYSpeed) {
		ySpeed = newYSpeed;
	}
	void setName(const std::string& newName) {
		name = newName;
	}
	void setText(const std::string& newText) {
		text = newText;
	}
	void setDrawShape() {
		drawShape = !drawShape;
	}
	void setDrawText() {
		drawText = !drawText;
	}
	void setShapePosition(float x, float y) {
		shape.setPosition(x, y);
	}
};

int main(int argc, char* argv[]) {
	
	std::string filename = "config/config.txt";
	std::ifstream config(filename);

	/*
	Config file layout - any order
		Window 
			Width	Height
		Font
			Filepath	Font size	 RGB
		Rectangle
			Shape name	 Initial X,Y	 Initial speed X,Y	 RGB	 Width	 Height
		Circle
			Shape name	 Initial X,Y	 Initial speed X,Y	 RGB	 Radius
	*/

	if (!config.is_open()) {
		std::cerr << "Error: cannot open configuration file!" << std::endl;
		exit(-1);
	}

	//the list of shapes to be rendered - currently required unique IDs
	//std::unordered_map<std::string, std::string> shapeText;
	std::unordered_map<std::string, Circle> circles;
	std::unordered_map<std::string, Rect> rects;


	//std::unordered_map<std::string, sf::circleshape> circles;
	//std::unordered_map<std::string, sf::rectangleshape> rectangles;
	//std::unordered_map<std::string, std::vector<float>> circleData;
	//std::unordered_map<std::string, std::vector<float>> rectangleData;

	std::string line;
	
	int wWidth, wHeight;

	std::string fontPath;
	int fontSize, fontR, fontG, fontB;

	//new block - variables defined within this block do not exist outside
	//all useful data must be defined above here - below are temp variables
	while (std::getline(config, line)) {
		int count = 1;
		std::stringstream ss(line);
		std::string shapeType;
		int value;

		std::vector<int> values;

		ss >> shapeType;

		if (shapeType == "Window") {
			if (!(ss >> wWidth >> wHeight)) {
				std::cerr << "Error: The configuration file does not specify a window width and height!" << std::endl;
				exit(-1);
			}
		}
		else if (shapeType == "Font") {
			if (!(ss >> fontPath >> fontSize >> fontR >> fontG >> fontB)) {
				std::cerr << "Error: The configuration file does not specify correct font data!" << std::endl;
				exit(-1);
			}
		}
		else if (shapeType == "Circle") {
			std::string name;
			float circleX, circleY, circleSpeedX, circleSpeedY, circleR, circleG, circleB, circleRadius;

			if (!(ss >> name >> circleX >> circleY >> circleSpeedX >> circleSpeedY >>
				circleR >>circleG >>circleB >> circleRadius)) {
				std::cerr << "Error: The configuration file does not specify correct circle data!" << std::endl;
				exit(-1);
			}

			//create the sfml circle
			sf::CircleShape circle(circleRadius, 32);
			circle.setPosition(circleX, circleY);
			circle.setFillColor(sf::Color(circleR, circleG, circleB));

			//circleData[name].insert(circleData[name].end(),
			//	{circleX, circleY, circleSpeedX, circleSpeedY, circleR, circleG, circleB});

			//create new circle
			Circle newCircle(circle, circleSpeedX, circleSpeedY, name, name);

			circles.emplace(name, newCircle);
			//shapeText[name] = name;
		}
		else if (shapeType == "Rectangle") {
			std::string name;

			float rectX, rectY, rectSpeedX, rectSpeedY, rectR, rectG, rectB, rectWidth, rectHeight;

			if (!(ss >> name >> rectX >> rectY >> rectSpeedX >> rectSpeedY >> 
				rectR >> rectG >> rectB >> rectWidth >> rectHeight)) {
				std::cerr << "Error: The configuration file does not specify correct rectangle data!" << std::endl;
				exit(-1);
			}

			sf::RectangleShape rect(sf::Vector2f(rectWidth, rectHeight));
			rect.setPosition(rectX, rectY);
			rect.setFillColor(sf::Color(rectR, rectG, rectB));

			//rectangleData[name].insert(rectangleData[name].end(), 
			//	{rectX, rectY, rectSpeedX, rectSpeedY, rectR, rectG, rectB});
			Rect newRect(rect, rectSpeedX, rectSpeedY, name, name);

			rects.emplace(name, newRect);
			//shapeText[name] = name;
		}
		else {
			std::cerr << "Error: The shape data was not properly configured with " << 
				line << " on line " << count << std::endl;
			exit(-1);
		}
		count += 1;
		//can improve logging to include line numbers and config data on fail
	}
	
	config.close();

	//ensures no unnecessary memory held in before further loops begins
	//shapeID.shrink_to_fit();

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

	// #pass by reference into new blocks
	// - auto x when working with copies and not altering original outside block scope
	// - auto &x when working with original with persistent alterations outside
	// - auto const &x when not loading copies OR altering the original


	//for (const auto& circle : circles) {
		//for (int value : circleData.second) {


		//}
	//}

	//make shape
	float circleRadius = 50;
	int circleSegments = 32;
	float circleSpeedX = 5.0f;
	float circleSpeedY = 2.5f;
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
	text.setPosition(circle.getPosition().x, 
		circle.getPosition().y  - (float)text.getCharacterSize());

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

			text.setPosition(circle.getPosition().x,
				circle.getPosition().y - (float)text.getCharacterSize());

			if (circle.getPosition().x < 0 ||
				circle.getPosition().x + 2*circleRadius > wWidth) {
				circleSpeedX = -circleSpeedX;
			}
			if (circle.getPosition().y < 0 ||
				circle.getPosition().y + 2*circleRadius > wHeight) {
				circleSpeedY = -circleSpeedY;
			}

			// basic rendering
			window.clear(); 
			for (auto& circleData : circles) {
				if (circleData.second.getDrawShape()) {
					circleData.second.setShapePosition(
						circleData.second.getXSpeed() + circleData.second.getShapePosition().x,
						circleData.second.getYSpeed() + circleData.second.getShapePosition().y
					);
					if (circleData.second.getShapePosition().x < 0 ||
						circleData.second.getShapePosition().x + 
						2 * circleData.second.getShape().getRadius() > wWidth) {
						circleData.second.setXSpeed(-circleData.second.getXSpeed());
					}
					if (circleData.second.getShapePosition().y < 0 ||
						circleData.second.getShapePosition().y + 
						2 * circleData.second.getShape().getRadius() > wHeight) {
						circleData.second.setYSpeed(-circleData.second.getYSpeed());
					}
					window.draw(circleData.second.getShape());
				}
			}
			for (auto& rectData : rects) {
				if (rectData.second.getDrawShape()) {
					rectData.second.setShapePosition(
						rectData.second.getXSpeed() + rectData.second.getShapePosition().x,
						rectData.second.getYSpeed() + rectData.second.getShapePosition().y
					);
					if (rectData.second.getShapePosition().x < 0 ||
						rectData.second.getShapePosition().x + 
						rectData.second.getShape().getLocalBounds().width > wWidth) {
						rectData.second.setXSpeed(-rectData.second.getXSpeed());
					}
					if (rectData.second.getShapePosition().y < 0 ||
						rectData.second.getShapePosition().y + 
						rectData.second.getShape().getLocalBounds().height > wHeight) {
						rectData.second.setYSpeed(-rectData.second.getYSpeed());
					}
					window.draw(rectData.second.getShape());
				}
			}
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
