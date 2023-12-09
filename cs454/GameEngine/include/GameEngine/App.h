#pragma once
#include <GameEngine/Game.h>

namespace GameEngine {

	namespace app {
		class App {
		protected:
			Game game;
		public:
			App() {}

			virtual void Initialise() = 0;
			virtual void Load() = 0;
			virtual void Clear() = 0;

			virtual void Run() { game.MainLoop(); }
			virtual void RunIteration() { game.MainLoopIteration(); }

			Game& GetGame() { return game; }
			const Game& GetGame() const { return game; }

			void Main() {
				Initialise();
				Load();
				Run();
				Clear();
			}
		};
	}
}
