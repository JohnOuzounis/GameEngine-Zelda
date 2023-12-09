#pragma once
#include <functional>

namespace GameEngine {
	namespace app
	{
		class Game {
		public:
			using Action = std::function<void(void)>;
			using Predicate = std::function<bool(void)>;
		private:
			Action render, anim, input, ai, physics, destruct, collisions, user;
			Predicate done;
			void Invoke(const Action& action) { if (action) action(); }
		public:
			void SetRender(const Action& action) { render = action; }
			void SetAnim(const Action& action) { anim = action; }
			void SetInput(const Action& action) { input = action; }
			void SetAI(const Action& action) { ai = action; }
			void SetPhysics(const Action& action) { physics = action; }
			void SetDestruct(const Action& action) { destruct = action; }
			void SetCollisions(const Action& action) { collisions = action; }
			void SetUser(const Action& action) { user = action; }
			void SetFinished(const Predicate& finished) { done = finished; }

			void Render() { Invoke(render); }
			void ProgressAnimations() { Invoke(anim); }
			void Input() { Invoke(input); }
			void AI() { Invoke(ai); }
			void Physics() { Invoke(physics); }
			void CollisionChecking() { Invoke(collisions); }
			void CommitDestructions() { Invoke(destruct); }
			void UserCode() { Invoke(user); }

			bool IsFinished() { return (done) ? done() : true; }
			void MainLoop();
			void MainLoopIteration();
		};
	}
}