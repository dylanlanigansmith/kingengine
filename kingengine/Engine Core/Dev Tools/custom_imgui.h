//
//  custom_imgui.h
//  kingengine
//
//  Created by Dylan Lanigan on 2022-01-17.
//  Copyright © 2022 Dylan Lanigan. All rights reserved.
//
//From: https://eliasdaler.github.io/using-imgui-with-sfml-pt2/#using-imgui-with-stl

#ifndef custom_imgui_h
#define custom_imgui_h
#include "imgui-SFML.h"
#include "imgui.h"
#include <vector>
#include <string>
namespace ImGui
{
static auto vector_getter = [](void* vec, int idx, const char** out_text)
{
    auto& vector = *static_cast<std::vector<std::string>*>(vec);
    if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
    *out_text = vector.at(idx).c_str();
    return true;
};

bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
{
    if (values.empty()) { return false; }
    return Combo(label, currIndex, vector_getter,
        static_cast<void*>(&values), values.size());
}

bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
{
    if (values.empty()) { return false; }
    return ListBox(label, currIndex, vector_getter,
        static_cast<void*>(&values), values.size());
}

}

#endif /* custom_imgui_h */
