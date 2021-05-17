#include <SFML/Graphics.hpp>
#include <iostream>
#include <ctime>

using namespace sf;


enum mode {
	creating, game
};
enum Direct {
	top, right, bottom, left
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
//	bool is_destroyed = false;
};

// function, that say, can we put only one deck there, that control previous deck
bool is_good(Cell** matrix, Point point, Point prev) {
	int x = point.x;
	int y = point.y;
	if (x >= 0 && y >= 0 && x <= 9 && y <= 9) { ; }
	else { return false; }
	if (x - 1 >= 0 && y - 1 >= 0) { // if this point exists
		if (matrix[x - 1][y - 1].ship >= 0) { return false; }
	}

	if (x - 1 >= 0 && y + 1 <= 9) { // if this point exists
		if (matrix[x - 1][y + 1].ship >= 0) { return false; }
	}

	if (x + 1 < 10 && y + 1 < 10) { // if this point exists
		if (matrix[x + 1][y + 1].ship >= 0) { return false; }
	}

	if (x + 1 < 10 && y - 1 >= 0) { // if this point exists
		if (matrix[x + 1][y - 1].ship >= 0) { return false; }
	}

	if (x >= 0 && y - 1 >= 0) { // if this point exists
		if (matrix[x][y - 1].ship >= 0 &&(x != prev.x || y - 1 != prev.y)) { return false; }
	}

	if (x + 1 < 10 && y >= 0) { // if this point exists
		if (matrix[x + 1][y].ship >= 0 && (x + 1 != prev.x || y != prev.y)) { return false; }
	}

	if (x >= 0 && y + 1 < 10) { // if this point exists
		if (matrix[x][y + 1].ship >= 0 && (x != prev.x || y + 1 != prev.y)) { return false; }
	}

	if (x - 1 >= 0 && y >= 0) { // if this point exists
		if (matrix[x - 1][y].ship >= 0 && (x - 1 != prev.x || y != prev.y)) { return false; }
	}

	return true;
}

// function, that say, can we put only one deck there
bool is_good(Cell** matrix, Point point) {
	int x = point.x;
	int y = point.y;
	if (x >= 0 && y >= 0 && x <= 9 && y <= 9) { ; }
	else { return false; }
	if (x - 1 >= 0 && y - 1 >= 0) { // if this point exists
		if (matrix[x - 1][y - 1].ship >= 0) { return false; }
	}

	if (x - 1 >= 0 && y + 1 <= 9) { // if this point exists
		if (matrix[x - 1][y + 1].ship >= 0) { return false; }
	}

	if (x + 1 < 10 && y + 1 < 10) { // if this point exists
		if (matrix[x + 1][y + 1].ship >= 0) { return false; }
	}

	if (x + 1 < 10 && y - 1 >= 0) { // if this point exists
		if (matrix[x + 1][y - 1].ship >= 0) { return false; }
	}

	if (x >= 0 && y - 1 >= 0) { // if this point exists
		if (matrix[x][y - 1].ship >= 0) { return false; }
	}

	if (x + 1 < 10 && y >= 0) { // if this point exists
		if (matrix[x + 1][y].ship >= 0) { return false; }
	}

	if (x >= 0 && y + 1 < 10) { // if this point exists
		if (matrix[x][y + 1].ship >= 0) { return false; }
	}

	if (x - 1 >= 0 && y >= 0) { // if this point exists
		if (matrix[x - 1][y].ship >= 0) { return false; }
	}

	return true;
}

void build_ship(std::vector <std::vector<Point>>& ships, Cell** matrix, Point start, std::size_t size, std::size_t direct) {
	std::vector<Point> ship;
	if (is_good(matrix, start)) {
		ship.push_back(start);
	}
	else {
		return;
	}
	Point prev(start);
	std::size_t i = size - 1;
	while (i != 0) {
		Point test_point(prev);
		if (direct == top) {
			test_point.y--;
		}
		if (direct == right) {
			test_point.x++;
		}
		if (direct == bottom) {
			test_point.y++;
		}
		if (direct == left) {
			test_point.x--;
		}

		if (is_good(matrix, test_point, prev)) {
			ship.push_back(test_point);
			prev = test_point;
		}
		else {
			break;
		}

		i--;
	}
	if (ship.size() == size) {
		ships.push_back(ship);
	}
}

std::vector <std::vector<Point>> build_ships(Cell** matrix, Point start, std::size_t size) {
	std::vector <std::vector<Point>> ships;
	for (std::size_t direct = 0; direct < 4; direct++) {
		build_ship(ships, matrix, start, size, direct);
	}
	return ships;
}



bool is_in_ships(std::vector <std::vector<Point>>& possible_ships, short int& ship_number, std::size_t index, Point point) {
	if (ship_number == -1) {
		for (std::size_t i = 0; i < possible_ships.size(); i++) {
			if (possible_ships[i][index].x == point.x && possible_ships[i][index].y == point.y) {
				if (index != 0){ ship_number = i; }
				
				return true;
			}
		}
	}
	else if (possible_ships[ship_number][index].x == point.x && possible_ships[ship_number][index].y == point.y && possible_ships.size() > 0) {
		return true;
	}
	if (possible_ships.size() == 0) {
		ship_number = -1;
	}
	
	return false;
}

bool set_deck(std::size_t x, std::size_t y, std::size_t size, std::vector <std::vector<Point>>& possible_ships, short int& ship_number, Cell** matrix, Fleet& fleet, std::size_t& filled, std::size_t& ship_index, std::size_t& max_decks) {
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
	if (filled == 0) {
		possible_ships = build_ships(matrix, Point(x, y), fleet.ships[ship_index].size);
		ship_number = -1;
	}
	if (is_in_ships(possible_ships, ship_number, filled, Point(x, y))) {
		filled++;
		fleet.add_deck(x, y, ship_index);
		matrix[x][y].ship = ship_index;

		return true;
	}

	return false;
}


Fleet generate_random_field(Cell** matrix, std::size_t size) {
	srand(time(0)); // initialize generator of random numbers by system time
	Fleet fleet(10);
	for (std::size_t i = 0; i < size; ) {
		std::size_t x = rand() % 10;
		std::size_t y = rand() % 10;
		std::vector <std::vector<Point>> possible_ships= build_ships(matrix, Point(x, y), fleet.ships[i].size);
		if (possible_ships.size() != 0) {
			std::vector<Point> ship = possible_ships[rand() % possible_ships.size()];
			for (std::size_t j = 0; j < ship.size(); j++) {
				fleet.add_deck(ship[j].x, ship[j].y, i);
				matrix[ship[j].x][ship[j].y].ship = i;
			}
			i++;
		}
	}
	return fleet;
}
Cell** create_matrix(std::size_t size) {
	Cell** matrix = new Cell * [size];
	for (std::size_t i = 0; i < size; i++) {
		matrix[i] = new Cell[size];
		for (std::size_t j = 0; j < size; j++) {
			matrix[i][j].ship = -1;
			matrix[i][j].is_visited = false;
		}
	}
	return matrix;
}
void clear_matrix(Cell** matrix, std::size_t size) {
	for (std::size_t i = 0; i < size; i++) {
		for (std::size_t j = 0; j < size; j++) {
//			matrix[i][j].is_destroyed = false;
			matrix[i][j].is_visited = false;
			matrix[i][j].ship = -1;
		}
	}
}

void remove_matrix(Cell** matrix, std::size_t size) {
	for (std::size_t i = 0; i < size; i++) {
		delete[] matrix[i];
	}
	delete matrix;
}

void clear_fleet(Fleet& fleet, std::size_t size) {

	for (std::size_t i = 0; i < size; i++) {
		delete[]fleet.ships[i].decks;
		fleet.ships[i].decks = new Point[fleet.ships[i].size];
		fleet.ships[i].last_filled = -1;
		fleet.ships[i].is_destroyed = false;
	}

}

void remove_fleet(Fleet& fleet, std::size_t size) {

	for (std::size_t i = 0; i < size; i++) {
		delete[]fleet.ships[i].decks;

	}
	delete[] fleet.ships;
}

int main() {
	const std::size_t width = 1200, height = 600, w = 32, SIZE = 10;
	Cell** field_user = create_matrix(SIZE); // -2 destroyed, -1 - empty, >=0 - some ship
	Cell** field_ai = create_matrix(SIZE);
	short int score_user = 0, score_ai = 0, ship_number = -1;
	std::vector<std::vector<Point>> possible_ships;
	
	///
	Font font;
	font.loadFromFile("fonts/Inter Regular.otf");
	Text cur_score("SCORE\n" + std::to_string(score_ai) +" : " + std::to_string(score_user), font, 40);
	cur_score.setFillColor(Color(0, 155, 155));
	cur_score.move(330, 50);

	RenderWindow window(VideoMode(width, height), "Sea Battle");
	Texture texture;
	texture.loadFromFile("img/1.jpg");
	RectangleShape button_start(sf::Vector2f(8 * w, 2 * w)), button_restart(sf::Vector2f(8 * w, 2 * w)), button_stop(sf::Vector2f(8 * w, 2 * w));
	Text text_start("Start!", font, 40), text_restart("New game", font, 40), text_stop("Exit!", font, 40);
	button_start.setPosition(15*w, 16*w);
	button_start.setFillColor(Color(204, 255, 255));
	text_start.setFillColor(Color(0, 51, 0));
	text_start.move(17 * w, 16 * w + 5);

	button_restart.setPosition(3 * w, 13 * w);
	button_restart.setFillColor(Color(204, 255, 255));
	text_restart.setFillColor(Color(0, 51, 0));
	text_restart.move(4 * w, 13 * w + 5);

	button_stop.setPosition(3 * w, 16 * w);
	button_stop.setFillColor(Color(204, 255, 255));
	text_stop.setFillColor(Color(0, 51, 0));
	text_stop.move(4 * w, 16 * w + 5);


	// sprite creating
	Sprite sprite(texture);
	sprite.setTextureRect(IntRect(0, 0, w, w));
	
	///
	short int mode = creating;
	std::size_t current_ship = 0, filled = 0, max_decks = 4;
	Text cur_ship("Ship " + std::to_string(current_ship) +": " + std::to_string(filled) + "/" + std::to_string(max_decks), font, 40);
	cur_ship.setFillColor(Color(0, 155, 155));
	cur_ship.move(16*w, 13*w);
	Fleet fleet_user(10);
	Fleet fleet_ai = generate_random_field(field_ai, SIZE);

	while (window.isOpen()){
		Vector2i pos = Mouse::getPosition(window);
		std::size_t x = pos.x / w;
		std::size_t y = pos.y / w;

		
		if (mode == creating) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed || event.type == Event::MouseButtonPressed && (x >= 3 && x <= 10) && (y >= 16 && y <= 18)) {
					remove_matrix(field_user, SIZE);
					remove_matrix(field_ai, SIZE);
					remove_fleet(fleet_user, SIZE);
					remove_fleet(fleet_ai, SIZE);
					window.close();
					return 0;
				}
				if (event.type == Event::MouseButtonPressed && (x >= 15 && x <= 24) && (y >= 2 && y <= 11)) {
					if (event.key.code == Mouse::Left) {
						set_deck(x - 15, y-2, SIZE, possible_ships, ship_number, field_user, fleet_user, filled, current_ship, max_decks);
					}
				}
				if (current_ship == SIZE-1 && filled == 1 && event.type == Event::MouseButtonPressed && (x >= 15 && x <= 22) && (y >= 16 && y <=  17)) {
					if (event.key.code == Mouse::Left) {
						mode = game;
					}
				}
				if (event.type == Event::MouseButtonPressed && (x >= 3 && x <= 11) && (y >= 13 && y <= 15)) { // button restart
					current_ship = 0; filled = 0; max_decks = 4;
					score_user = 0; score_ai = 0; ship_number = -1;
					clear_matrix(field_user, SIZE);
					clear_matrix(field_ai, SIZE);
					clear_fleet(fleet_user, SIZE);
					remove_fleet(fleet_ai, SIZE);
					fleet_ai = generate_random_field(field_ai, SIZE);
				}
			}
		}

		if (mode == game) {
			Event event;
			while (window.pollEvent(event)) {
				if (event.type == Event::Closed || event.type == Event::MouseButtonPressed && (x >= 3 && x <= 10) && (y >= 16 && y <= 18)) {
					remove_matrix(field_user, SIZE);
					remove_matrix(field_ai, SIZE);
					remove_fleet(fleet_user, SIZE);
					remove_fleet(fleet_ai, SIZE);
					window.close();
					return 0;
				}
				if (event.type == Event::MouseButtonPressed && (x >= 15 && x <= 24) && (y >= 2 && y <= 11)) {
					if (event.key.code == Mouse::Left) {
						set_deck(x - 15, y - 2, SIZE, possible_ships, ship_number, field_user, fleet_user, filled, current_ship, max_decks);
					}
				}
				if (current_ship == SIZE - 1 && filled == 1 && event.type == Event::MouseButtonPressed && (x >= 10 && x <= 24) && (y >= 2 && y <= 15)) {
					if (event.key.code == Mouse::Left) {
						mode = game;
					}
				}
				if (event.type == Event::MouseButtonPressed && (x >= 3 && x <= 11) && (y >= 13 && y <= 15)) { // button restart
					current_ship = 0; filled = 0; max_decks = 4;
					score_user = 0; score_ai = 0; ship_number = -1;
					clear_matrix(field_user, SIZE);
					clear_matrix(field_ai, SIZE);
					clear_fleet(fleet_user, SIZE);
					remove_fleet(fleet_ai, SIZE);
					fleet_ai = generate_random_field(field_ai, SIZE);
				}
			}
		}
		

		/*
		set_deck(2, 8, SIZE, possible_ships, ship_number, field_user, fleet_user, filled, current_ship, max_decks);
		set_deck(2, 7, SIZE, possible_ships, ship_number, field_user, fleet_user, filled, current_ship, max_decks);
		set_deck(4, 8, SIZE, possible_ships, ship_number, field_user, fleet_user, filled, current_ship, max_decks);
		*/

		// set background
		window.clear(Color(250, 220, 100, 0));

		// print Ai field
		for (std::size_t i = 0; i < SIZE; i++) {
			for (std::size_t j = 0; j < SIZE; j++) {
				
//				if (field_ai[i][j].is_visited == true) {
					if (field_ai[i][j].ship == -2) {  // -2 destroyed, -1 - empty, >=0 - some ship
						sprite.setTextureRect(IntRect(w, 0, w, w));
					}
					else if (field_ai[i][j].ship == -1) {
						sprite.setTextureRect(IntRect(w, 0, w, w));
					}
					else if (field_ai[i][j].ship >= 0) {
						sprite.setTextureRect(IntRect(2*w, 0, w, w));
					}
//				}
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
		
		if (mode == creating) {
			cur_ship.setString("Ship " + std::to_string(current_ship + 1) + ": " + std::to_string(filled) + "/" + std::to_string(max_decks));
			if (current_ship == SIZE - 1 && filled == 1) {
				window.draw(button_start);
				window.draw(text_start);
			}
			window.draw(cur_ship);
		}
		else if (mode == game) {
			cur_score.setString("SCORE\n" + std::to_string(score_ai) + " : " + std::to_string(score_user));
			window.draw(cur_score);
			window.draw(button_start);
			window.draw(button_restart);
			window.draw(button_stop);

			window.draw(text_start);
			window.draw(text_restart);
			window.draw(text_stop);
		}

		// window draw
		
		window.draw(button_restart);
		window.draw(text_restart);
		window.draw(button_stop);
		window.draw(text_stop);
		window.display();
	}

	return 0;
}


//if (event.type == Event::MouseButtonPressed && (x < 10 || x > 14) && y < 12 && y > 1) {