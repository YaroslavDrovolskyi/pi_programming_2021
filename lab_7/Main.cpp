#include <SFML/Graphics.hpp>

using namespace sf;


enum mode {
	creating, game
};

struct Point {
	int x;
	int y;
	bool is_destroyed;

	Point(int x, int y) {
		this->x = x;
		this->y = y;
		this->is_destroyed = false;
	}

	Point() {

	}
};

struct Ship {
	std::size_t size;
	bool is_destroyed;
	short int last_filled;
	Point* decks;

	Ship(std::size_t size) {
		this->size = size;
		this->is_destroyed = false;
		this->decks = new Point [this->size];
		this->last_filled = -1;
	}

	void add_deck(std::size_t x, std::size_t y) {
		if (last_filled >= this->size - 1) { return; }
		this->last_filled++;
		this->decks[this->last_filled] = Point(x, y);
	}
};

void set_deck(std::size_t x, std::size_t y, short int** matrix, Ship& ship, std::size_t size, std::size_t ship_number) {
	if (matrix[x][y] != -1) { return; }
	if (x >= size || y >= size) { return; }
	ship.add_deck(x, y);
	matrix[x][y] = ship_number;
}

int main() {
	short int field_user[10][10];
	short int field_ai[10][10];
	for (std::size_t i = 0; i < 10; i++) {
		for (std::size_t j = 0; j < 10; j++) {
			field_user[i][j] = -1;
			field_ai[i][j] = -1;
		}
	}
	const std::size_t width = 1200, height = 600, w = 32;
	RenderWindow window(VideoMode(width, height), "Sea Battle");
	Texture texture;
	texture.loadFromFile("img/1.jpg");
	short int mode = creating;

	// sprite creating
	Sprite sprite(texture);
	sprite.setTextureRect(IntRect(0, 0, 32, 32));
	std::size_t current_sheep = 0;


	while (window.isOpen()){

		Vector2i pos = Mouse::getPosition(window);
		int x = pos.x / w;
		int y = pos.y / w;

		if (mode == creating) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed) { window.close(); }
				//if (event.type == Event::MouseButtonPressed && x <= 9 || x >= 14 && y <= 11 && y >= 1) {
				if (event.type == Event::MouseButtonPressed) {
					if (event.key.code == Mouse::Left) {
						field_ai[x][y - 2] = 0;
					}
				}


			}



		}


		// Process queue of events
		


		// Установка цвета фона
		window.clear(Color(250, 220, 100, 0));

		// print Ai field
		for (std::size_t i = 0; i < 10; i++) {
			for (std::size_t j = 0; j < 10; j++) {
				
				if (field_ai[i][j] == -1) {
					sprite.setTextureRect(IntRect(32, 0, 32, 32));
				}
				else {
					sprite.setTextureRect(IntRect(0, 0, 32, 32));
				}
				sprite.setPosition(i * 32, j * 32);
				sprite.move(0, 64);
				window.draw(sprite);
			}
		}

		// print user field
		for (std::size_t i = 0; i < 10; i++) {
			for (std::size_t j = 0; j < 10; j++) {
				sprite.setTextureRect(IntRect(0, 0, 33, 32));
				sprite.setPosition(i * 32, j * 32);
				sprite.move(480, 64);
				window.draw(sprite);
			}
		}
		
		//window.draw(sprite);

		// window draw
		window.display();
	}

	return 0;
}