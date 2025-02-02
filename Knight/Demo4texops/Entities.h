#pragma once

#include <string>

class Entity
{
	public:
		// method to render the entire level
		virtual void Create() = 0;
		virtual void Update(float elaspedTime) {};
		virtual void Draw2D() = 0;
		virtual void Release() = 0;

		bool isReady;
		string title;
		string description;

		Entity();
};


class SimpleDrawTextureDemo : public Entity
{
public:

	void Create() override;
	void Draw2D() override;
	void Release() override;

	SimpleDrawTextureDemo();

private:
	Texture2D texture;
};

class CompressTextureDemo : public Entity
{
public:

	void Create() override;
	void Draw2D() override;
	void Release() override;

	CompressTextureDemo();

private:
	Texture2D original;
	Texture2D compressed;
};

class DrawPartialRotateDemo : public Entity
{
public:

	void Create() override;
	void Update(float ellapsed) override;
	void Draw2D() override;
	void Release() override;

	DrawPartialRotateDemo();

private:
	Texture2D texture;
	float currentAngle;
	float timeDiff;
};

class ColorBlendingDemo : public Entity
{
public:

	void Create() override;
	void Update(float ellapsed) override;
	void Draw2D() override;
	void Release() override;

	ColorBlendingDemo();

protected:
	Texture2D texture;
	int currentIdx;
	float timeDiff;
};

class SmoothColorBlendingDemo : public ColorBlendingDemo
{
public:
	void Draw2D() override;

	SmoothColorBlendingDemo();
};

class SceneTransitionDemo : public Entity
{
public:

	void Create() override;
	void Update(float ellapsed) override;
	void Draw2D() override;
	void Release() override;

	SceneTransitionDemo();

private:
	Texture2D scene1;
	Texture2D scene2;
	float currentProgress;
	float elapsed_time;
};

class AnimatedTexDemo : public Entity
{
public:

	void Create() override;
	void Update(float ellapsed) override;
	void Draw2D() override;
	void Release() override;

	AnimatedTexDemo();

protected:
	Texture2D _spritesSheet;
	float _currentIdx;
	float _anim_time;
	float _anim_length;
};


class GlowDemo : public Entity
{
public:

	void Create() override;
	void Update(float ellapsed) override;
	void Draw2D() override;
	void Release() override;

	GlowDemo();

private:
	Texture2D background;
	Texture2D glow;
	float currentAngle;
};

class NPatchDemo : public Entity
{
public:

	void Create() override;
	void Update(float ellapsed) override;
	void Draw2D() override;
	void Release() override;

	NPatchDemo();

private:
	Texture2D _npatchTex;
	NPatchInfo _ninePatchInfo;
	Rectangle _dest;
	float elapsed_time;
};
