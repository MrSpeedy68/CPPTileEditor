#include "imgui.h"
#include "imgui-SFML.h"
#include "Tilemap.h"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

#include "nlohmann/json.hpp"
#include <fstream>

using namespace std;
using json = nlohmann::json;

namespace Maps
{
    struct Map
    {
        unsigned int tileDimensions_x;
        unsigned int tileDimensions_y;
        vector<int> tileSet;
        int width;
        int height;
    };

    json Save(Map& map)
    {
        json jsonMap;

        jsonMap["tile_dimensions_x"] = map.tileDimensions_x;
        jsonMap["tile_dimensions_y"] = map.tileDimensions_y;
        jsonMap["tileSet"] = map.tileSet;
        jsonMap["width"] = map.width;
        jsonMap["height"] = map.height;

        return jsonMap;
    }

    Map Load(json& jsonMap)
    {
        Map map;
        jsonMap.at("tile_dimensions_x").get_to(map.tileDimensions_x);
        jsonMap.at("tile_dimensions_y").get_to(map.tileDimensions_y);
        jsonMap.at("tileSet").get_to(map.tileSet);
        jsonMap.at("width").get_to(map.width);
        jsonMap.at("height").get_to(map.height);

        return map;
    }
};

int main() {

    int tileValue = 0;
    char tileChar = 0;

    ifstream inJson;

    inJson.open("MapLevel.json"); //open file make stream

    json jsonMap;

    inJson >> jsonMap;

    Maps::Map levelMap = Maps::Load(jsonMap);
    //levelMap.tileDimensions_x * levelMap.width, levelMap.tileDimensions_y * levelMap.height
    sf::RenderWindow window(sf::VideoMode(1080,720), "ImGui + SFML = <3"); //Make renderview size of tileset
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    TileMap tileMap(sf::Vector2u(levelMap.tileDimensions_x, levelMap.tileDimensions_y), levelMap.tileSet, levelMap.width, levelMap.height);

    tileMap.load("tileset.png");

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            auto mousePosition = sf::Mouse::getPosition(window);

            cout << mousePosition.x / levelMap.tileDimensions_x << " , " << mousePosition.y / levelMap.tileDimensions_y << endl;

            tileMap.changeTileValue(sf::Vector2i(mousePosition.x / levelMap.tileDimensions_x, mousePosition.y / levelMap.tileDimensions_y), tileValue);
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Sprite Manager");
        if (ImGui::Button("Tile 1"))
            tileValue = 0;

        if (ImGui::Button("Tile 2"))
            tileValue = 1;

        if (ImGui::Button("Tile 3"))
            tileValue = 2;

        if (ImGui::Button("Tile 4"))
            tileValue = 3;


        if (ImGui::Button("Save Map"))
        {
            //Save
            ofstream outJson;

            outJson.open("MapLevel.json"); //open file make stream

            json outJsonMap = Maps::Save(levelMap);

            outJson << outJsonMap;

            outJson.close();

            cout << "Map Saved";
        }

        ImGui::End();

        window.clear();
        window.draw(shape);

        window.draw(tileMap);

        ImGui::SFML::Render(window);
        window.display();
    }


    ImGui::SFML::Shutdown();
}
