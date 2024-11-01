// Copyright [2024] <Abrar Fahim>
#include "include/imgui-SFML.h"
#include <deque>
#include <iostream>
#include <random>
// #include "include/imgui.h"
#include "include/round.hpp"
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window.hpp>

// Global Variables
sf::Color green = sf::Color(172, 204, 96, 255);
sf::Color dg = sf::Color(43, 51, 24, 255);
int offset = 75;
bool running = true;
int CellSize = 30;
int CellCount = 25;
sf::Vector2f Direction = sf::Vector2f(1, 0);
int Score;
class Food {
public:
  sf::Vector2f pos = GenerateRandomCell();
  sf::Sprite returntexture();
  sf::Texture txt;
  sf::Vector2f GenerateRandomPos(std::deque<sf::Vector2f>);
  sf::Vector2f GenerateRandomCell();
};
class Snake {
public:
  std::deque<sf::Vector2f> body{sf::Vector2f(6, 9), sf::Vector2f(5, 9),
                                sf::Vector2f(4, 9)};
  sf::RoundedRectangleShape part;
  void draw(sf::RenderWindow &win, sf::RoundedRectangleShape prt);
  sf::RoundedRectangleShape initsnakebody();
  bool AddSegment = false;
  void update();
  void reset();
  void CheckCollisionWithTail(Food food, sf::RenderWindow &win,sf::Sound sound);
  void playwallsound();
};

void showbeginningtext(sf::RenderWindow &win) {
  sf::Font font;
  if (!font.loadFromFile("bo.otf")) {
    std::cout << "Font not loaded";
  }
  sf::Text text;
  text.setFont(font);
  text.setString("Snake Game");
  text.setCharacterSize(70);
  text.setFillColor(sf::Color::Black);
  text.setPosition(75, -25);
  text.setStyle(sf::Text::Bold);
  win.draw(text);
}

void showscore(sf::RenderWindow &win, Snake snake) {
  Score = snake.body.size() - 3;
  sf::Font scorefont;
  if (!scorefont.loadFromFile("bo.otf")) {
    std::cout << "Font is not loading in score issue" << std::endl;
  }
  sf::Text score;

  score.setFont(scorefont);
  score.setString(std::to_string(Score));
  score.setCharacterSize(70);
  score.setFillColor(sf::Color::Black);
  score.setPosition(700, -25);
  score.setStyle(sf::Text::Bold);
  win.draw(score);
}
void showendtext(sf::RenderWindow &win) {
  sf::Font EndFont;
  if (!EndFont.loadFromFile("ES.otf")) {
    std::cout << "EndFontNotloading";
  }
  sf::Text EndText;
  EndText.setFont(EndFont);
  EndText.setCharacterSize(70);
  EndText.setFillColor(sf::Color::Black);
  // EndText.setString("Game Over \n Press any key to start again");
  EndText.setString("Done");
  EndText.setPosition(500, 300);
  EndText.setStyle(sf::Text::Bold);
  win.draw(EndText);
}
bool ElementInDeque(sf::Vector2f element, std::deque<sf::Vector2f> deque) {
  for (unsigned int i = 0; i < deque.size(); i++) {
    if (deque[i] == element) {
      return true;
    }
  }
  return false;
}

void game() {
  sf::RenderWindow window(sf::VideoMode(2 * offset + CellSize * CellCount,
                                        2 * offset + CellSize * CellCount),
                          "Snake Game omg", sf::Style::Titlebar | sf::Style::Close);
  window.setSize(sf::Vector2u(2 * offset + CellSize * CellCount, 2 * offset + CellSize * CellCount));
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);
  Food food;
  Snake snake;
  sf::RoundedRectangleShape bodi = snake.initsnakebody();
  sf::Sprite fodi = food.returntexture();
 sf::Clock deltaClock;
  sf::RectangleShape shape;
  shape.setSize(sf::Vector2f(750, 750));
  shape.setFillColor(sf::Color(sf::Color::Transparent));
  shape.setOutlineColor(dg);
  shape.setOutlineThickness(5);
  shape.setPosition(75, 75);
  sf::Clock UpdateClock;
  sf::RectangleShape rd;
  // Eating sounds beta
  sf::SoundBuffer buf;
  buf.loadFromFile("eat.wav");
  sf::Sound eatsound;

  sf::SoundBuffer WallBuff;
  WallBuff.loadFromFile("wall.wav");
  sf::Sound WallSound;
  WallSound.setBuffer(WallBuff);
  eatsound.setBuffer(buf);
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(window, event);
      if (event.type == sf::Event::Closed) {
        window.close();
      }
      if (event.type == sf::Event::KeyPressed) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
          if (Direction.y != 1)
            Direction = sf::Vector2f(0, -1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
          if (Direction.x != 1)
            Direction = sf::Vector2f(-1, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
          window.close();
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
          if (Direction.y != -1)
            Direction = sf::Vector2f(0, 1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
          if (Direction.x != -1)
            Direction = sf::Vector2f(1, 0);
        }
        running = true;
      }
    }
    ImGui::SFML::Update(window, deltaClock.restart());
    if (UpdateClock.getElapsedTime().asMilliseconds() >= (200 - Score * 5)) {
      if (running) {
        snake.update();
        UpdateClock.restart();
        if (snake.body[0] == food.pos) {
          //If snake hits food
          food.pos = food.GenerateRandomPos(snake.body);
          fodi.setPosition(offset + food.pos.x * CellSize,
                           offset + food.pos.y * CellSize);
          snake.AddSegment = true;
          if (eatsound.getStatus() != sf::Sound ::Playing)
            eatsound.play();
        }
        if (snake.body[0].x == CellCount || snake.body[0].x == -1) {
          // if snake hits the x axis border
          snake.reset();
          running = false;
          food.pos = food.GenerateRandomPos(snake.body);
          fodi.setPosition(offset + food.pos.x * CellSize,
                           offset + food.pos.y * CellSize);
          // snake.playwallsound();
          WallSound.play();
        }
        if (snake.body[0].y == CellCount || snake.body[0].y == -1) {
          // if the snake hits the y axis border
          snake.reset();
          food.pos = food.GenerateRandomPos(snake.body);
          fodi.setPosition(offset + food.pos.x * CellSize,
                           offset + food.pos.y * CellSize);
          running = false;
          // snake.playwallsound();
          WallSound.play();
        }
        snake.CheckCollisionWithTail(food, window,WallSound); 
      } else if (!running) {
        showendtext(window);
      }
    }
    window.clear(green); // creating the background
    window.draw(fodi); // drawing the food
    window.draw(shape); // drawing the snake
    showbeginningtext(window);
    showscore(window, snake);
    snake.draw(window, bodi);
    ImGui::SFML::Render(window);
    window.display();
  }
  ImGui::SFML::Shutdown();
}

int main() { game(); }

sf::Sprite Food::returntexture() {
  if (!txt.loadFromFile("food.png")) {
    std::cout << "Texture not loaded" << std::endl;
  }
  sf::Sprite sprite;
  sprite.setPosition(offset + pos.x * CellSize, offset + pos.y * CellSize);
  sprite.setTexture(txt);
  return sprite;
}

sf::Vector2f Food::GenerateRandomCell() {
  float lower_bound = 0;
  float upper_bound = (CellCount - 1);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dist(lower_bound, upper_bound);
  float x = dist(gen);
  float y = dist(gen);
  return sf::Vector2f(x, y);
}

sf::Vector2f Food::GenerateRandomPos(std::deque<sf::Vector2f> SnakeBody) {
  sf::Vector2f pos = GenerateRandomCell();
  while (ElementInDeque(pos, SnakeBody)) {
    pos = GenerateRandomCell();
  }
  return pos;
}

sf::RoundedRectangleShape Snake::initsnakebody() {
  part.setSize(sf::Vector2f(CellSize, CellSize));
  part.setFillColor(dg);
  return part;
}

void Snake::draw(sf::RenderWindow &win, sf::RoundedRectangleShape prt) {
  for (int i = 0; i < body.size(); i++) {
    float x = body[i].x;
    float y = body[i].y;
    part.setCornersRadius(12);
    part.setCornerPointCount(10);
    part.setPosition(
        sf::Vector2f(offset + x * CellSize, offset + y * CellSize));
    win.draw(part);
  }
}

void Snake::update() {
  body.push_front(body[0] + Direction);
  if (AddSegment != true) {
    body.pop_back();
  } else {
    AddSegment = false;
  }
}

void Snake::reset() {
  body = {sf::Vector2f(6, 9), sf::Vector2f(5, 9), sf::Vector2f(4, 9)};
  Direction = {1, 0};
}

void Snake::CheckCollisionWithTail(Food food, sf::RenderWindow &win,sf::Sound sound) {
  std::deque<sf::Vector2f> headlessbody = body;
  headlessbody.pop_front();
  if (ElementInDeque(body[0], headlessbody)) {
    reset();
    running = false;
  }
  food.pos = food.GenerateRandomPos(body);
  sound.play();
}
