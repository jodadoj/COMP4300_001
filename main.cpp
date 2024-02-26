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
	//default constructor with a 0,0 shape
	Rect() {}
	//constructor with initializer list
	Rect(sf::RectangleShape rect, float x_speed, float y_speed, std::string id,
		std::string label, bool showShape = true, bool showText = true) :
		shape(rect), xSpeed(x_speed), ySpeed(y_speed), name(id), text(label),
		drawShape(showShape), drawText(showText) {}

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
		std::string label, bool showShape = true, bool showText = true) :
		shape(circle), xSpeed(x_speed), ySpeed(y_speed), name(id), text(label),
		drawShape(showShape), drawText(showText) {

	}

	sf::CircleShape getShape() {
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

	//the list of shapes to be rendered - currently require unique IDs
	std::unordered_map<std::string, Circle> circles;
	std::unordered_map<std::string, Rect> rects;
	std::unordered_map<std::string, sf::Text> labels;


	std::string line = "";

	int wWidth, wHeight = 0;

	std::string fontPath = "";
	int fontSize, fontR, fontG, fontB = 0;

	//new block - variables defined within this block do not exist outside
	//all useful data must be defined above here - below are temp variables
	while (std::getline(config, line)) {
		int count = 1;
		std::stringstream ss(line);
		std::string shapeType = "";
		int value = 0;

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
				circleR >> circleG >> circleB >> circleRadius)) {
				std::cerr << "Error: The configuration file does not specify correct circle data!" << std::endl;
				exit(-1);
			}

			//create the sfml circle
			sf::CircleShape circle(circleRadius, 32);
			circle.setPosition(circleX, circleY);
			circle.setFillColor(sf::Color(circleR, circleG, circleB));

			//create new circle in custom class
			Circle newCircle(circle, circleSpeedX, circleSpeedY, name, name);

			//add to our map
			circles.emplace(name, newCircle);
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

			Rect newRect(rect, rectSpeedX, rectSpeedY, name, name);

			rects.emplace(name, newRect);
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
	sf::Text text("Sample Text", myFont, 10);

	//put text in bottom and shift up by height
	text.setPosition(circle.getPosition().x + circle.getRadius() - text.getLocalBounds().width / 2,
		circle.getPosition().y + circle.getRadius() - text.getLocalBounds().height / 2);

	//set up char array for text
	char displayString[255] = "Sample text";

	//set up text labels
	for (auto& rectData : rects) {
		sf::RectangleShape curr = rectData.second.getShape();
		std::string name = rectData.first;

		labels.emplace(name, sf::Text(name, myFont, 10));
		labels[name].setPosition(
			curr.getPosition().x + curr.getLocalBounds().width / 2 - labels[name].getLocalBounds().width / 2,
			curr.getPosition().y + curr.getLocalBounds().height / 2 - labels[name].getLocalBounds().height / 2);

	}	for (auto& circleData : circles) {
		sf::CircleShape curr = circleData.second.getShape();
		std::string name = circleData.first;

		labels.emplace(name, sf::Text(name, myFont, 10));
		labels[name].setPosition(
			curr.getPosition().x + curr.getRadius() - labels[name].getLocalBounds().width / 2,
			curr.getPosition().y + curr.getRadius() - labels[name].getLocalBounds().height / 2);
	}

	//main loop - renders each frame on window
	while (window.isOpen()) {
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
			//move circle each frame update is visable
			circle.setPosition(circle.getPosition().x + circleSpeedX, circle.getPosition().y + circleSpeedY);

			text.setPosition(circle.getPosition().x + circle.getRadius() - text.getLocalBounds().width / 2,
				circle.getPosition().y + circle.getRadius() - text.getLocalBounds().height / 2);

			if (circle.getPosition().x < 0 ||
				circle.getPosition().x + 2 * circleRadius > wWidth) {
				circleSpeedX = -circleSpeedX;
			}
			if (circle.getPosition().y < 0 ||
				circle.getPosition().y + 2 * circleRadius > wHeight) {
				circleSpeedY = -circleSpeedY;
			}

			// basic rendering
			window.clear();

			// #pass by reference into new blocks
			// - auto x when working with copies and not altering original outside block scope
			// - auto &x when working with original with persistent alterations outside
			// - auto const &x when not loading copies OR altering the original
			for (auto& circleData : circles) {
				//make a shorter reference to the actual current shape
				Circle& curr = circleData.second;
				if (curr.getDrawShape()) {
					curr.setShapePosition(
						curr.getXSpeed() + curr.getShapePosition().x,
						curr.getYSpeed() + curr.getShapePosition().y
					);
					if (curr.getShapePosition().x < 0 ||
						curr.getShapePosition().x +
						2 * curr.getShape().getRadius() > wWidth) {
						curr.setXSpeed(-curr.getXSpeed());
					}
					if (curr.getShapePosition().y < 0 ||
						curr.getShapePosition().y +
						2 * curr.getShape().getRadius() > wHeight) {
						curr.setYSpeed(-curr.getYSpeed());
					}
					window.draw(curr.getShape());
				}
				if (curr.getDrawText()) {
					//const references as not editing, reducing memory overhead while increasing readability
					const std::string& name = circleData.first;
					const sf::CircleShape& currS = circleData.second.getShape();
					labels[name].setPosition(
						currS.getPosition().x + currS.getRadius() - labels[name].getLocalBounds().width / 2,
						currS.getPosition().y + currS.getRadius() - labels[name].getLocalBounds().height / 2);
					window.draw(labels[name]);
				}
			}
			for (auto& rectData : rects) {
				Rect& curr = rectData.second;
				if (curr.getDrawShape()) {
					curr.setShapePosition(
						curr.getXSpeed() + curr.getShapePosition().x,
						curr.getYSpeed() + curr.getShapePosition().y
					);
					if (curr.getShapePosition().x < 0 ||
						curr.getShapePosition().x +
						curr.getShape().getLocalBounds().width > wWidth) {
						curr.setXSpeed(-curr.getXSpeed());
					}
					if (curr.getShapePosition().y < 0 ||
						curr.getShapePosition().y +
						curr.getShape().getLocalBounds().height > wHeight) {
						curr.setYSpeed(-curr.getYSpeed());
					}
					window.draw(curr.getShape());
				}
				if (curr.getDrawText()) {
					const std::string& name = rectData.first;
					const sf::RectangleShape& currS = rectData.second.getShape();
					labels[name].setPosition(
						currS.getPosition().x + currS.getLocalBounds().width / 2 - labels[name].getLocalBounds().width / 2,
						currS.getPosition().y + currS.getLocalBounds().height / 2 - labels[name].getLocalBounds().height / 2);
					window.draw(labels[name]);
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

/*
		// Create the dropdown menu
		if (ImGui::BeginCombo("##combo", items[selectedItemIndex])) {
			for (int i = 0; i < IM_ARRAYSIZE(items); i++) {
				bool isSelected = (selectedItemIndex == i);
				if (ImGui::Selectable(items[i], isSelected))
					selectedItemIndex = i;

				// Set the initial focus when opening the combo
				if (isSelected)
					ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
*/