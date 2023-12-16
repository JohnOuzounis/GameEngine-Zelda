#pragma once
#include <functional>
#include <GameEngine/Animation/Animation.h>
#include <GameEngine/LatelyDestroyable.h>

namespace GameEngine {
class Animator : public LatelyDestroyable{
   public:
	enum State {
		FINISHED = 0,
		RUNNING = 1,
		STOPPED = 2
	};

	using OnFinish = std::function<void(Animator*)>;
	using OnStart = std::function<void(Animator*)>;
	using OnAction = std::function<void(Animator*, const Animation&)>;

   protected:
	double lastTime = 0;
	State state = FINISHED;
	OnFinish onFinish;
	OnStart onStart;
	OnAction onAction;
	void NotifyStopped(void);
	void NotifyStarted(void);
	void NotifyAction(const Animation&);
	void Finish(bool isForced = false);

   public:
	Animator(void);
	Animator(const Animator&) = delete;
	Animator(Animator&&) = delete;
	virtual ~Animator();

	void Stop(void);
	bool HasFinished(void) const { return state != RUNNING; }
	virtual void TimeShift(double offset);
	virtual void Progress(double currTime) = 0;

	template <typename Tfunc>
	void SetOnFinish(const Tfunc& f) {
		onFinish = f;
	}

	template <typename Tfunc>
	void SetOnStart(const Tfunc& f) {
		onStart = f;
	}

	template <typename Tfunc>
	void SetOnAction(const Tfunc& f) {
		onAction = f;
	}
};
}  // namespace GameEngine