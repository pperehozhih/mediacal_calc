#pragma once

#include <imgui.h>
#include "TextEditor.h"
#include "DataSource.h"

namespace med_calc {
	class GeneralWindow {
	public:
		GeneralWindow() {
			text.SetImGuiChildIgnored(true);
			dataSource.OpenFile("data.sl3");
		}
		void Update();
	private:
		TextEditor text;
		DataSource dataSource;
		ImVec2 pos;
		bool textTooltip = false;
	};
}
