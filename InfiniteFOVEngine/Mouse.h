#pragma once
#include <queue>

class __declspec(dllexport) Mouse
{
	friend class Window;
public:
	class Event
	{
	public:
		enum Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			MPress,
			MRelease,
			X1Press,
			X1Release,
			X2Press,
			X2Release,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			Invalid
		};
	private:
		Type type;
		bool leftPressed;
		bool rightPressed;
		bool middlePressed;
		bool x1Pressed;
		bool x2Pressed;
		int x;
		int y; 
	public:
		Event() noexcept
			:
			type(Type::Invalid),
			leftPressed(false),
			rightPressed(false),
			middlePressed(false),
			x1Pressed(false),
			x2Pressed(false),
			x(0),
			y(0)
		{}
		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			leftPressed(parent.leftPressed),
			rightPressed(parent.rightPressed),
			middlePressed(parent.middlePressed),
			x1Pressed(parent.x1Pressed),
			x2Pressed(parent.x2Pressed),
			x(parent.x),
			y(parent.y)
		{}
		bool IsValid() const noexcept
		{
			return type != Type::Invalid;
		}
		Type GetType()
		{
			return type;
		}
		std::pair<int, int> GetPos() const noexcept
		{
			return { x, y };
		}
		int GetX() const noexcept
		{
			return x;
		}
		int GetY() const noexcept
		{
			return y;
		}
		bool IsLeftPressed() const noexcept
		{
			return leftPressed;
		}
		bool IsRightPressed() const noexcept
		{
			return rightPressed;
		}
		bool IsMiddlePressed() const noexcept
		{
			return middlePressed;
		}
		bool IsX1Pressed() const noexcept
		{
			return x1Pressed;
		}
		bool IsX2Pressed() const noexcept
		{
			return x2Pressed;
		}
	};
public:
	Mouse() = default;
	Mouse(const Mouse&) = delete;
	Mouse& operator=(const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetX() const noexcept;
	int GetY() const noexcept;
	bool InWindow() const noexcept;
	bool IsLeftPressed() const noexcept;
	bool IsRightPressed() const noexcept;
	bool IsMiddlePressed() const noexcept;
	bool IsX1Pressed() const noexcept;
	bool IsX2Pressed() const noexcept;
	Mouse::Event Read() noexcept;
	bool IsEmpty() const noexcept
	{
		return buffer.empty();
	}
	void Clear() noexcept;
private:
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnMiddlePressed(int x, int y) noexcept;
	void OnMiddleReleased(int x, int y) noexcept;
	void OnX1Pressed(int x, int y) noexcept;
	void OnX1Released(int x, int y) noexcept;
	void OnX2Pressed(int x, int y) noexcept;
	void OnX2Released(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	int x;
	int y;
	bool leftPressed = false;
	bool rightPressed = false;
	bool middlePressed = false;
	bool x1Pressed = false;
	bool x2Pressed = false;
	bool inWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};