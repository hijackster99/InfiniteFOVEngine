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
		int x;
		int y; 
	public:
		Event() noexcept
			:
			type(Type::Invalid),
			leftPressed(false),
			rightPressed(false),
			x(0),
			y(0)
		{}
		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			leftPressed(parent.leftPressed),
			rightPressed(parent.rightPressed),
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
	bool inWindow = false;
	int wheelDeltaCarry = 0;
	std::queue<Event> buffer;
};