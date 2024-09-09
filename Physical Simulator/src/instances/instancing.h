#pragma once

#include <iostream>
#include <initializer_list>
#include <any>
#include <string>
#include <algorithm>
#include <cctype>
#include "../graphics/model.h"
#include "../graphics/button.h"

vector<Model> modelsList;
vector<Button> buttonList;

class Instances {
public:

    int New(const char* type, std::initializer_list<std::any> argsList) {
        std::vector<std::any> args(argsList);

        try {
            if (toLower(type) == "model") {
                Model newModel(std::any_cast<const char*>(args[0]));
                modelsList.push_back(newModel);
                return modelsList.size() - 1;
            }
            if (toLower(type) == "button") {
                Button newButton(std::any_cast<const char*>(args[0]), std::any_cast<ImVec2>(args[1]), std::any_cast<ImVec2>(args[2]));
                buttonList.push_back(newButton);
                return buttonList.size() - 1;
            }
        }
        catch (const std::bad_any_cast& e) {
            std::cerr << "Bad any_cast: " << e.what() << std::endl;
        }
    }
private:
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
            [](unsigned char c) { return std::tolower(c); });
        return result;
    }
};

Instances instances;