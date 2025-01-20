#include "pch.h"
#include "Mouse.h"

std::pair<int, int> Mouse::GetPos() const noexcept
{
	return { x, y };
}

int Mouse::GetX() const noexcept
{
	return x;
}

int Mouse::GetY() const noexcept
{
	return y;
}

bool Mouse::InWindow() const noexcept
{
	return inWindow;
}

bool Mouse::IsLeftPressed() const noexcept
{
	return leftPressed;
}

bool Mouse::IsRightPressed() const noexcept
{
	return rightPressed;
}

bool Mouse::IsMiddlePressed() const noexcept
{
	return middlePressed;
}

bool Mouse::IsX1Pressed() const noexcept
{
	return x1Pressed;
}

bool Mouse::IsX2Pressed() const noexcept
{
	return x2Pressed;
}

Mouse::Event Mouse::Read() noexcept
{
	if (buffer.size() > 0u)
	{
		Mouse::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else
	{
		return Mouse::Event();
	}
}

void Mouse::Clear() noexcept
{
	buffer = std::queue<Event>();
}

void Mouse::OnMouseMove(int x, int y) noexcept
{
	this->x = x;
	this->y = y;

	buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	inWindow = false;
	buffer.push(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	inWindow = true;
	buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	leftPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	leftPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	rightPressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	rightPressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	TrimBuffer();
}

void Mouse::OnMiddlePressed(int x, int y) noexcept
{
	middlePressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::MPress, *this));
	TrimBuffer();
}

void Mouse::OnMiddleReleased(int x, int y) noexcept
{
	middlePressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::MRelease, *this));
	TrimBuffer();
}

void Mouse::OnX1Pressed(int x, int y) noexcept
{
	x1Pressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::X1Press, *this));
	TrimBuffer();
}

void Mouse::OnX1Released(int x, int y) noexcept
{
	x1Pressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::X1Release, *this));
	TrimBuffer();
}

void Mouse::OnX2Pressed(int x, int y) noexcept
{
	x2Pressed = true;

	buffer.push(Mouse::Event(Mouse::Event::Type::X2Press, *this));
	TrimBuffer();
}

void Mouse::OnX2Released(int x, int y) noexcept
{
	x2Pressed = false;

	buffer.push(Mouse::Event(Mouse::Event::Type::X2Release, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	wheelDeltaCarry += delta;
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}