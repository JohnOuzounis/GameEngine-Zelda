#pragma once
#include <functional>
#include <vector>

namespace GameEngine {
namespace app {
class Game {
   public:
	using Action = std::function<void(void)>;
	using Predicate = std::function<bool(void)>;

   private:
	std::vector<Action> render, anim, input, ai, physics, destruct, collisions,
		user, quit;
	Predicate done;

	void InvokeAll(const std::vector<Action>& actions) {
		for (const auto& action : actions) {
			Invoke(action);
		}
	}

	void Invoke(const Action& action) {
		if (action)
			action();
	}

	void AddAction(std::vector<Action>& actions,
				   const Action& action,
				   bool first) {
		if (first)
			actions.insert(actions.begin(), action);
		else
			actions.push_back(action);
	}

   public:
	void AddRender(const Action& action, bool first) {
		AddAction(render, action, first);
	}

	void AddAnim(const Action& action, bool first) {
		AddAction(anim, action, first);
	}

	void AddInput(const Action& action, bool first) {
		AddAction(input, action, first);
	}

	void AddAI(const Action& action, bool first) {
		AddAction(ai, action, first);
	}

	void AddPhysics(const Action& action, bool first) {
		AddAction(physics, action, first);
	}

	void AddDestruct(const Action& action, bool first) {
		AddAction(destruct, action, first);
	}

	void AddCollisions(const Action& action, bool first) {
		AddAction(collisions, action, first);
	}

	void AddUser(const Action& action, bool first) {
		AddAction(user, action, first);
	}

	void AddQuit(const Action& action, bool first) {
		AddAction(quit, action, first);
	}

	void SetFinished(const Predicate& finished) { done = finished; }

	void Render() { InvokeAll(render); }
	void ProgressAnimations() { InvokeAll(anim); }
	void Input() { InvokeAll(input); }
	void AI() { InvokeAll(ai); }
	void Physics() { InvokeAll(physics); }
	void CollisionChecking() { InvokeAll(collisions); }
	void CommitDestructions() { InvokeAll(destruct); }
	void UserCode() { InvokeAll(user); }
	void Quit() { InvokeAll(quit); }

	bool IsFinished() { return (done) ? done() : true; }
	void MainLoop();
	void MainLoopIteration();
};
}  // namespace app
}  // namespace GameEngine