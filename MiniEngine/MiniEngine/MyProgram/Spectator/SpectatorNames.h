#pragma once

namespace {
	//�ϋq�̗񋓁B
	enum EnSpectator {
		enShirtMale,
		enNathanMale,
		enSophiaWoman,
		enClaudiaWoman,
		enSpectatorNum
	};
	namespace FilePath {
		const char* shirtMale = "Assets/modelData/Human/shirtMale/shirtMale.tkm";
		const char* nathanMale = "Assets/modelData/Human/nathanMale/nathanMale.tkm";
		const char* suitMale = "Assets/modelData/Human/suitMale/suitMale.tkm";
		const char* sophiaWoman = "Assets/modelData/Human/sophiaWoman/sophiaWoman.tkm";
		const char* claudiaWoman = "Assets/modelData/Human/claudiaWoman/claudiaWoman.tkm";
		const char* suitWoman = "Assets/modelData/Human/suitWoman/suitWoman.tkm";

		const char* SPECTATOR_MODEL_PATH[enSpectatorNum] = {
			shirtMale,
			nathanMale,
			sophiaWoman,
			claudiaWoman
		};
	}
	namespace AnimPath {
		const char* clapAnim = "Assets/animData/Human/Clap.tka";
		const char* shirtMaleAnim = "Assets/animData/Human/shirtMaleClap.tka";
		const char* nathanMaleAnim = "Assets/animData/Human/nathanCheering.tka";
		const char* sophiaWomanAnim = "Assets/animData/Human/sophiaClap.tka";
		const char* claudiaWomanAnim = "Assets/animData/Human/claudiaClap.tka";

		const char* SPECTATOR_ANIM_PATH[enSpectatorNum] = {
			shirtMaleAnim,
			nathanMaleAnim,
			sophiaWomanAnim,
			claudiaWomanAnim
		};
	}
}
