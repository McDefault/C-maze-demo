#include <iostream>
#include <string>
#include <vector>
#include <memory>

struct Tool {
    std::string name;
};

struct Kamer {
    std::string name;
    Tool requiredTool;
    std::vector<Tool> toolLoot;
    std::shared_ptr<Kamer> north;
    std::shared_ptr<Kamer> west;
    std::shared_ptr<Kamer> east;
    std::shared_ptr<Kamer> south;
};

struct Speler {
    std::shared_ptr<Kamer> currentPosition;
    std::vector<Tool> inventory;
};

int main() {
    // Create instances of Tool
    Tool hammer = {"Hammer"};
    Tool screwdriver = {"Screwdriver"};
    Tool wrench = {"Wrench"};
    Tool pliers = {"Pliers"};
    Tool saw = {"Saw"};
    Tool drill = {"Drill"};
    Tool tape = {"Tape"};
    Tool level = {"Level"};

    // Create instances of Kamer
    auto livingRoom = std::make_shared<Kamer>();
    livingRoom->name = "Living Room";
    livingRoom->requiredTool = hammer;

    auto kitchen = std::make_shared<Kamer>();
    kitchen->name = "Kitchen";
    kitchen->requiredTool = screwdriver;

    auto bedroom = std::make_shared<Kamer>();
    bedroom->name = "Bedroom";
    bedroom->requiredTool = wrench;

    auto bathroom = std::make_shared<Kamer>();
    bathroom->name = "Bathroom";
    bathroom->requiredTool = pliers;

    auto garage = std::make_shared<Kamer>();
    garage->name = "Garage";
    garage->requiredTool = saw;

    auto office = std::make_shared<Kamer>();
    office->name = "Office";
    office->requiredTool = drill;

    auto garden = std::make_shared<Kamer>();
    garden->name = "Garden";
    garden->requiredTool = tape;

    auto attic = std::make_shared<Kamer>();
    attic->name = "Attic";
    attic->requiredTool = level;

    // Set neighbors to create a circular path
    livingRoom->north = kitchen;
    kitchen->east = garage;
    garage->south = office;
    office->west = garden;
    garden->north = attic;
    attic->west = bathroom;
    bathroom->south = bedroom;
    bedroom->west = livingRoom;

    // Ensure consistency in neighbors
    kitchen->south = livingRoom;
    garage->west = kitchen;
    office->north = garage;
    garden->east = office;
    attic->south = garden;
    bathroom->east = attic;
    bedroom->north = bathroom;
    livingRoom->east = bedroom;

    // Add tool loot to some Kamers
    kitchen->toolLoot.push_back(pliers);
    garage->toolLoot.push_back(drill);
    garden->toolLoot.push_back(level);

    // Create an instance of Speler with Hammer in the inventory
    Speler player = {livingRoom, {hammer, screwdriver, wrench}};

    while (true) {
        // Display current location and neighboring Kamers
        std::cout << "Current location: " << player.currentPosition->name << std::endl;
        std::cout << "Neighbors:" << std::endl;
        if (player.currentPosition->north) {
            std::cout << "North: " << player.currentPosition->north->name
                    << " (Required tool: " << player.currentPosition->north->requiredTool.name << ")" << std::endl;
        }
        if (player.currentPosition->east) {
            std::cout << "East: " << player.currentPosition->east->name
                    << " (Required tool: " << player.currentPosition->east->requiredTool.name << ")" << std::endl;
        }
        if (player.currentPosition->south) {
            std::cout << "South: " << player.currentPosition->south->name
                    << " (Required tool: " << player.currentPosition->south->requiredTool.name << ")" << std::endl;
        }
        if (player.currentPosition->west) {
            std::cout << "West: " << player.currentPosition->west->name
                    << " (Required tool: " << player.currentPosition->west->requiredTool.name << ")" << std::endl;
        }

        // Display inventory
        std::cout << "Inventory:" << std::endl;
        for (const auto &tool: player.inventory) {
            std::cout << "Tool name: " << tool.name << std::endl;
        }

        std::string direction;
        std::cout << "Enter direction (north, east, south, west) or 'exit' to quit: ";
        std::cin >> direction;

        if (direction == "exit") {
            goto end;
        }

        if (direction == "north" && player.currentPosition->north) {
            for (const auto &tool: player.inventory) {
                if (tool.name == player.currentPosition->north->requiredTool.name) {
                    player.currentPosition->north->south = player.currentPosition;
                    player.currentPosition = player.currentPosition->north;
                    goto collect_loot;
                }
            }
            std::cout << "You do not have the required tool to move north." << std::endl;
            continue;
        } else if (direction == "east" && player.currentPosition->east) {
            for (const auto &tool: player.inventory) {
                if (tool.name == player.currentPosition->east->requiredTool.name) {
                    player.currentPosition->east->west = player.currentPosition;
                    player.currentPosition = player.currentPosition->east;
                    goto collect_loot;
                }
            }
            std::cout << "You do not have the required tool to move east." << std::endl;
            continue;
        } else if (direction == "south" && player.currentPosition->south) {
            for (const auto &tool: player.inventory) {
                if (tool.name == player.currentPosition->south->requiredTool.name) {
                    player.currentPosition->south->north = player.currentPosition;
                    player.currentPosition = player.currentPosition->south;
                    goto collect_loot;
                }
            }
            std::cout << "You do not have the required tool to move south." << std::endl;
            continue;
        } else if (direction == "west" && player.currentPosition->west) {
            for (const auto &tool: player.inventory) {
                if (tool.name == player.currentPosition->west->requiredTool.name) {
                    player.currentPosition->west->east = player.currentPosition;
                    player.currentPosition = player.currentPosition->west;
                    goto collect_loot;
                }
            }
            std::cout << "You do not have the required tool to move west." << std::endl;
            continue;
        } else {
            std::cout << "Invalid direction or no neighboring Kamer in that direction." << std::endl;
            continue;
        }

    collect_loot:
        // Add tool loot to the player's inventory
        for (const auto &tool: player.currentPosition->toolLoot) {
            player.inventory.push_back(tool);
            std::cout << "Collected tool: " << tool.name << std::endl;
        }
        player.currentPosition->toolLoot.clear();
    }

end:
    return 0;
}
