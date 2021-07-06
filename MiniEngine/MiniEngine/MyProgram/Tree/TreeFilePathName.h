#pragma once

namespace {
	enum EnTree {
		enTree_Alaska1,
		enTree_Num
	};

	namespace TREE {
		const char* Alaska1 = "Assets/modelData/tree/AlaskaCedar_1.tkm";

		const char* FILEPATH[enTree_Num] =
		{
			Alaska1
		};

		const char* LEVEL_FILEPATH = "Assets/level/CourseTreeLevel.tkl";
	}
}