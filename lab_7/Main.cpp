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

	Ship() {
	}

	void add_deck(std::size_t x, std::size_t y) {
		if (last_filled >= int(this->size - 1)) { return; }
		this->last_filled++;
		this->decks[this->last_filled] = Point(x, y);
	}
};

struct Fleet {
	std::size_t size;
	Ship* ships;

	Fleet(std::size_t size = 10) {
		this->size = size;
		this->ships = new Ship[this->size];
		for (std::size_t i = 0; i < this->size; i++) {
			std::size_t ship_size = 0;
			if (i == 0) { ship_size = 4; }
			else if (i >= 1 && i <= 2) { ship_size = 3; }
			else if (i >= 3 && i <= 5) { ship_size = 2; }
			else { ship_size = 1; }
			this->ships[i] = Ship(ship_size);
		}
	}
	void add_deck(std::size_t x, std::size_t y, std::size_t ship_index) {
		this->ships[ship_index].add_deck(x, y);
	}
};

struct Cell {
	short int ship = -1;
	bool is_visited = false;
	bool is_destroyed = false;
};

bool set_deck(std::size_t x, std::size_t y, std::size_t size, Cell** matrix, Fleet& fleet, std::size_t& filled, std::size_t& ship_index, std::size_t& max_decks) {
	if (matrix[x][y].ship != -1) { return false; }
	if (x >= size || y >= size) { return false; }
	if (ship_index >= size) { return false; }
	if (fleet.ships[ship_index].last_filled == fleet.ships[ship_index].size - 1 && ship_index == size - 1) { return false; }

	if (fleet.ships[ship_index].last_filled == fleet.ships[ship_index].size - 1) {
		ship_index++;
		if (ship_index != size) {
			max_decks = fleet.ships[ship_index].size;
		}
		
		filled = 0;
	}
	filled++;
	fleet.add_deck(x, y, ship_index);
	matrix[x][y].ship = ship_index;

	return true;
}



int main() {
	const std::size_t width = 1200, height = 600, w = 32, SIZE = 10;
	Cell** field_user = new Cell*[SIZE]; // -2 destroyed, -1 - empty, >=0 - some ship
	Cell field_ai[10][10];
	short int score_user = 0, score_ai = 1000;
	for (std::size_t i = 0; i < SIZE; i++) {
		field_user[i] = new Cell[SIZE];
		for (std::size_t j = 0; j < SIZE; j++) {
			field_user[i][j].ship = -1;
			field_ai[i][j].ship = -1;

			field_user[i][j].is_visited = false;
			field_ai[i][j].is_visited = false;
		}
	}
	///
	Font font;
	font.loadFromFile("fonts/Inter Regular.otf");
	Text cur_score("SCORE\n" + std::to_string(score_ai) +" : " + std::to_string(score_user), font, 40);
	cur_score.setFillColor(Color(0, 155, 155));
	cur_score.move(330, 50);

	RenderWindow window(VideoMode(width, height), "Sea Battle");
	Texture texture;
	texture.loadFromFile("img/1.jpg");

	// sprite creating
	Sprite sprite(texture);
	sprite.setTextureRect(IntRect(0, 0, 32, 32));
	
	///
	short int mode = creating;
	std::size_t current_ship = 0, filled = 0, max_decks = 4;
	Text cur_ship("Ship " + std::to_string(current_ship) +": " + std::to_string(filled) + "/" + std::to_string(max_decks), font, 40);
	cur_ship.setFillColor(Color(0, 155, 155));
	cur_ship.move(16*w, 13*w);
	Fleet fleet_user(10);

	while (window.isOpen()){
		Vector2i pos = Mouse::getPosition(window);
		std::size_t x = pos.x / w;
		std::size_t y = pos.y / w;

		if (mode == creating) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed) { window.close(); }
				if (event.type == Event::MouseButtonPressed && (x >= 15 && x <= 24) && (y >= 2 && y <= 11)) {
					if (event.key.code == Mouse::Left) {
						set_deck(x - 15, y-2, SIZE, field_user, fleet_user, filled, current_ship, max_decks);
					}
				}


			}



		}


		// set background
		window.clear(Color(250, 220, 100, 0));

		// print Ai field
		for (std::size_t i = 0; i < SIZE; i++) {
			for (std::size_t j = 0; j < SIZE; j++) {
				
				if (field_ai[i][j].is_visited == true) {
					if (field_ai[i][j].ship == -2) {  // -2 destroyed, -1 - empty, >=0 - some ship
						sprite.setTextureRect(IntRect(w, 0, w, w));
					}
					else if (field_ai[i][j].ship == -1) {
						sprite.setTextureRect(IntRect(w, 0, w, w));
					}
					
				}
				else {
					sprite.setTextureRect(IntRect(0, 0, w, w));
				}
				sprite.setPosition(i * w, j * w);
				sprite.move(0, 2*w);
				window.draw(sprite);
			}
		}

		// print user field
		for (std::size_t i = 0; i < 10; i++) {
			for (std::size_t j = 0; j < 10; j++) {


				
			if (field_user[i][j].ship == -2) {  // -2 destroyed, -1 - empty, >=0 - some ship
				sprite.setTextureRect(IntRect(w, 0, w, w));
			}
			else if (field_user[i][j].ship == -1) {
				sprite.setTextureRect(IntRect(w, 0, w, w));
			}
			else if (field_user[i][j].ship >= 0) {
				sprite.setTextureRect(IntRect(2*w, 0, w, w));
			}
				sprite.setPosition(i * w + 15 * w, j * w + 2*w);
				window.draw(sprite);
			}
		}
		cur_score.setString("SCORE\n" + std::to_string(score_ai) + " : " + std::to_string(score_user));
		cur_ship.setString("Ship " + std::to_string(current_ship + 1) + ": " + std::to_string(filled) + "/" + std::to_string(max_decks));
		// window draw
		window.draw(cur_ship);
		window.draw(cur_score);
		window.display();
	}

	return 0;
}


//if (event.type == Event::MouseButtonPressed && (x < 10 || x > 14) && y < 12 && y > 1) {