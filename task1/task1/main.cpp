#include <SFML/Graphics.hpp>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <string>
#include <queue>
#include <iostream>

using namespace sf;
using namespace std;

// первая цифра, число вершнин, вторая-число связностей, дальше как соединины, например, 1 2 означает из второй вершину в третью (вершины нумеруются с 0)

const int width = 1000;
const int height = 1000;
const float r = 30;

const float pi = 3.1415926535f;

float Distance(Vector2f start, Vector2f end) {
	return sqrt((start.x - end.x) * (start.x - end.x) + (start.y - end.y) * (start.y - end.y));
}

float lenVector2f(Vector2f point) {
	return Distance(Vector2f(0, 0), point);
}


void arrow(RenderWindow* window, Vector2f A, Vector2f B, Color colour) {
	if (A != B) {
		VertexArray lines(LinesStrip, 2);
		lines[0].position = A;
		lines[0].color = colour;
		lines[1].position = B;
		lines[1].color = colour;
		float len = Distance(A, B);
		float len1 = min(50.f, len);
		Vector2f dir = (B - A) / abs(lenVector2f(B - A));
		lines[0].position += dir * r;
		lines[1].position -= dir * r;
		Vector2f cross = A + dir * (len - len1);
		Vector2f dir1 = Vector2f(1, -dir.x / dir.y);
		dir1 /= lenVector2f(dir1);
		Vector2f point1 = cross - dir1 * len1 * 0.1f;
		Vector2f point2 = cross + dir1 * len1 * 0.1f;
		ConvexShape triangle;
		triangle.setPointCount(3);
		triangle.setPoint(0, point1);
		triangle.setPoint(1, point2);
		triangle.setPoint(2, lines[1].position);
		triangle.setOutlineThickness(0);
		triangle.setFillColor(colour);
		(*window).draw(lines);
		(*window).draw(triangle);
	}
}

int main()
{
	setlocale(LC_ALL, "russian");
	std::cout << "In sfml window press Enter for start algorithm" << std::endl;


	ifstream fin("input.txt");
	ContextSettings settings;
	settings.antialiasingLevel = 8;
	RenderWindow window(VideoMode(width, height), "task1", Style::Default, settings);

	window.setKeyRepeatEnabled(true);

	CircleShape shape(100.f);
	shape.setFillColor(Color::Green);

	int vvod_1, vvod_2;
	fin >> vvod_1 >> vvod_2;
	Vector2f* points = new Vector2f[vvod_1];
	for (int i = 0; i < vvod_1; i++) {
		points[i] = Vector2f(0, 0);
		points[i].x = 400 * sin((float)i * 2.f / (float)vvod_1 * pi) + width / 2;
		points[i].y = -400 * cos((float)i * 2.f / (float)vvod_1 * pi) + height / 2;
		//	cout << points[i].x << " " << points[i].y << endl;
	}
	Font font;
	font.loadFromFile("Arial.ttf");
	Text* names = new Text[vvod_1];
	for (int i = 0; i < vvod_1; i++) {
		names[i] = Text(to_string(i + 1), font);
		if (i == 0) {
			names[i].setPosition(points[i] - Vector2f(names[i].getGlobalBounds().width / 2.f + 5.f, names[i].getGlobalBounds().height - 2.f));
		}
		else {
			names[i].setPosition(points[i] - Vector2f(names[i].getGlobalBounds().width / 2.f + 1.f, names[i].getGlobalBounds().height - 2.f));
		}
		//	cout << names[i].getLocalBounds().height << " " << names[i].getLocalBounds().width << endl;
	}
	CircleShape* circles = new CircleShape[vvod_1];
	for (int i = 0; i < vvod_1; i++) {
		circles[i] = CircleShape(r);
		circles[i].setPosition(points[i] - Vector2f(r, r));
		circles[i].setFillColor(Color(0, 0, 0, 0));
		circles[i].setOutlineColor(Color(255, 255, 255, 255));
		circles[i].setOutlineThickness(1.f);
	}


	int** pairs = new int* [vvod_2];
	for (int i = 0; i < vvod_2; i++) {
		pairs[i] = new int[2];
		fin >> pairs[i][0] >> pairs[i][1];
	}

	int** new_pairs = new int* [vvod_1 * vvod_1];
	for (int j = 0; j < vvod_1 * vvod_1; j++) {
		new_pairs[j] = new int[2];
		new_pairs[j][0] = 0;
		new_pairs[j][1] = 0;
	}

	int k_for_new_p = 0;

	for (int i = 0; i < vvod_1; i++) {
		queue<int> quer;
		quer.push(i);
		bool* used = new bool[vvod_1];
		for (int j = 0; j < vvod_1; j++) {
			used[j] = false;
		}
		int* p = new int[vvod_1];
		int* d = new int[vvod_1];
		for (int j = 0; j < vvod_1; j++) {
			d[j] = 0;
		}
		p[i] = -1;
		used[i] = true;
		for (int j = 0; j < vvod_1; j++) {
			d[j] = 0;
			//cout << d[i] << " ";
		}
		while (!quer.empty()) {
			int x = quer.front();
			quer.pop();
			for (int j = 0; j < vvod_2; j++) {
				if (pairs[j][0] == x) {
					int to = pairs[j][1];
					if (!used[to]) {
						used[to] = true;
						quer.push(to);
						d[to] = d[x] + 1;
						p[to] = x;
					}
				}
			}
		}

		for (int j = 0; j < vvod_1; j++) {
			if (d[j] > 0) {
				new_pairs[k_for_new_p][0] = i;
				new_pairs[k_for_new_p][1] = j;
			}
			k_for_new_p++;
		}
	}
	bool visible = false;

	while (window.isOpen())
	{
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		window.clear();

		if (Keyboard::isKeyPressed(Keyboard::Enter)) {
			visible = true;
		}
		if (Keyboard::isKeyPressed(Keyboard::BackSpace)) {
			visible = false;
		}

		if (visible) {
			for (int i = 0; i < vvod_1 * vvod_1; i++) {
				arrow(&window, points[new_pairs[i][0]], points[new_pairs[i][1]], Color::Red);
			}
		}

		for (int i = 0; i < vvod_1; i++) {
			window.draw(circles[i]);
			window.draw(names[i]);
		}

		for (int i = 0; i < vvod_2; i++) {
			arrow(&window, points[pairs[i][0]], points[pairs[i][1]], Color::White);
		}

		window.display();

	}
	return 0;
}