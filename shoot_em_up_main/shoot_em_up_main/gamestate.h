#pragma once
class GameState{
private:
	bool paused;
	bool game_over;
	bool victory;
	int score;

public:
	GameState(): paused(false), game_over(false), victory(false), score(0){}

	// getters
	bool isPaused() const;
	bool isGameOver() const;
	bool isVictory() const;
	int getScore() const;

	// setters
	void setPaused(bool p);
	void setGameOver(bool go);
	void setVictory(bool v);
	void addScore(int points);

	// toggle pause
	void togglePause();
	void reset();

	// check if game active
	bool isActive() const;
};

