#pragma once

#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>


struct NodeGraph{
  int x;
  int y;
  float gCost = std::numeric_limits<float>::infinity();
  float hCost = 0;
  float fCost() const { return gCost + hCost; }
  std::vector<NodeGraph*> neighbors;
  NodeGraph* parent = nullptr;
};

struct TmxObject
{
    int GetPropertyInt(const std::string &propertyName);
    float GetPropertyFloat(const std::string &propertyName);
    std::string GetPropertyString(const std::string &propertyName);

    TmxObject(const sf::Sprite sprite): sprite(sprite){};
    void MoveBy(const sf::Vector2f& movement);
    void MoveTo(const sf::Vector2f &position);

    std::string name;
    std::string type;
    sf::FloatRect rect;
    std::map<std::string, std::string> properties;

    sf::Sprite sprite;
};

// В картах TMX слой - это набор тайлов (спрайтов),
//  из которых складывается ландшафт карты.
// Слоёв может быть несколько, что позволяет нарисовать,
//  например, слой травы поверх слоя земли.
struct TmxLayer
{
    uint8_t opacity = 0;
    std::vector<sf::Sprite> tiles;
};

class TmxLevel
{
  public:
  // Загружает данные из TMX в память объекта.
  bool LoadFromFile(const std::string &filepath);
  
  TmxObject GetFirstObject(const std::string &name) const;
  std::vector<TmxObject> GetAllObjects(const std::string &name) const;
  sf::Vector2i GetTileSize() const;
  float GetTilemapWidth() const;
  float GetTilemapHeight() const;
  sf::Vector2f GetTilemapSize() const;
  void DrawLayers(sf::RenderTarget &target, int start, int end) const;
  
  bool isPassable(int, int) const;
  void makeGraph() const;
  bool inBound(int, int) const;
  void printGraph() const; 
  
  // Рисует все слои тайлов один за другим,
  //  но не рисует объекты (рисованием которых должна заниматься игра).
  // Принимает любую цель для рисования, например, sf::RenderWindow.
  void Draw(sf::RenderTarget &target) const;
  mutable std::vector<std::vector<NodeGraph*>> graph;
  
  private:
  int m_width = 0;
  int m_height = 0;
  int m_tileWidth = 0;
  int m_tileHeight = 0;
  int m_firstTileID = 0;
  sf::Texture m_tilesetImage;
  std::vector<TmxObject> m_objects;
  std::vector<TmxLayer> m_layers;
};
