#include "GeneralTable.h"

namespace med_calc {
	void GeneralWindow::Update() {
        ImGui::Begin("GeneraWindowContainer", 0, ImGuiWindowFlags_NoDecoration);
        auto size = ImGui::GetWindowSize();
        ImGui::BeginTable("GeneraWindow", 3, ImGuiTableFlags_BordersInner | ImGuiTableColumnFlags_DefaultHide, size);
        {
            for (auto&& item : dataSource.GetNames()) {
                bool tooltip = false;
                ImGui::TableNextColumn();
                ImGui::Text(item.ru.c_str());
                ImGui::TableNextColumn();
                if (item.index == 1 && ImGui::IsItemHovered()) {
                    tooltip = true;
                }
                ImGui::Text(item.en.c_str());
                ImGui::TableNextColumn();
                if (item.index == 1 && ImGui::IsItemHovered()) {
                    tooltip = true;
                }
                ImGui::Text(item.formula.c_str());
                ImGui::TableNextRow();
                if (tooltip || item.index == 1 && ImGui::IsItemHovered()) {
                    ImGui::BeginTooltip();
                    ImGui::Text(dataSource.GetDescription(item.index).c_str());
                    ImGui::EndTooltip();
                }
            }
            ImGui::EndTable();
        }
        ImGui::End();
	}
}
