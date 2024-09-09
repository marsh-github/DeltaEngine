#pragma once
#include <imgui.h>
#include <functional>

class Button
{
public:
    Button(const char* label, ImVec2 position, ImVec2 size = ImVec2(100, 30))
        : label(label), position(position), size(size), transparency(1.0f),
        textColor(ImVec4(1.0f, 1.0f, 1.0f, 1.0f)), backgroundColor(ImVec4(0.0f, 0.5f, 1.0f, 1.0f)),
        font(nullptr), clickCallback(nullptr), visible(true) {}

    void SetPosition(ImVec2 newPosition) { position = newPosition; }
    void SetSize(ImVec2 newSize) { size = newSize; }
    void SetLabel(const char* newLabel) { label = newLabel; }
    void SetTransparency(float alpha) { transparency = alpha; }
    void SetTextColor(ImVec4 color) { textColor = color; }
    void SetBackgroundColor(ImVec4 color) { backgroundColor = color; }
    void SetFont(ImFont* newFont) { font = newFont; }
    void SetVisibility(bool newVisible) { visible = newVisible; }

    void SetClickCallback(std::function<void()> callback) { clickCallback = callback; }

    bool Render()
    {
        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(0, 0));
        ImGui::Begin("Invisible", NULL, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

        ImGui::SetCursorScreenPos(position);
        ApplyStyles();
        bool clicked = ImGui::Button(label, size);
        RestoreStyles();

        ImGui::End();

        if (clicked && clickCallback && visible) {
            clickCallback();
        }

        return clicked;
    }

private:
    const char* label;
    ImVec2 position;
    ImVec2 size;
    float transparency;
    ImVec4 textColor;
    ImVec4 backgroundColor;
    ImFont* font;
    std::function<void()> clickCallback;
    bool visible;

    void ApplyStyles()
    {
        if (visible == true) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w * transparency));

            if (transparency < 1.0f)
            {
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(backgroundColor.x * 1.3f, backgroundColor.y * 1.3f, backgroundColor.z * 1.3f, (backgroundColor.w * transparency) * 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(backgroundColor.x * 0.9f, backgroundColor.y * 0.9f, backgroundColor.z * 0.9f, (backgroundColor.w * transparency) * 1.0f));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(backgroundColor.x * 1.3f, backgroundColor.y * 1.3f, backgroundColor.z * 1.3f, backgroundColor.w));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(backgroundColor.x * 0.9f, backgroundColor.y * 0.9f, backgroundColor.z * 0.9f, backgroundColor.w));
            }

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(textColor.x, textColor.y, textColor.z, textColor.w * transparency));
        }
        else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(backgroundColor.x, backgroundColor.y, backgroundColor.z, backgroundColor.w * 0.0f));

            if (transparency < 1.0f)
            {
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(backgroundColor.x * 1.3f, backgroundColor.y * 1.3f, backgroundColor.z * 1.3f, (backgroundColor.w * 0.0f) * 1.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(backgroundColor.x * 0.9f, backgroundColor.y * 0.9f, backgroundColor.z * 0.9f, (backgroundColor.w * 0.0f) * 1.0f));
            }
            else
            {
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(backgroundColor.x * 1.3f, backgroundColor.y * 1.3f, backgroundColor.z * 1.3f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(backgroundColor.x * 0.9f, backgroundColor.y * 0.9f, backgroundColor.z * 0.9f, 0.0f));
            }

            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(textColor.x, textColor.y, textColor.z, 0.0f));
        }

        if (font)
        {
            ImGui::PushFont(font);
        }
    }

    void RestoreStyles()
    {
        ImGui::PopStyleColor(4);

        if (font)
        {
            ImGui::PopFont();
        }
    }
};
