#pragma once
class GameState{
private:
	bool paused;
	bool game_over;
	bool victory;

public:
	GameState(): paused(false), game_over(false), victory(false) {}

	// getters
	bool isPaused() const;
	bool isGameOver() const;
	bool isVictory() const;

	// setters
	void setPaused(bool p);
	void setGameOver(bool go);
	void setVictory(bool v);

	// toggle pause
	void togglePause();
	void reset();

	// check if game active
	bool isActive() const;
};

