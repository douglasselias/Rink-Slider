/// @todo: static reduce linker time
/// @todo: assert only on debug code, flag on compiler?
/// @todo: is all these functions correct?
/// @todo: add inline?

Vector2 operator+(const Vector2 lhs, const f32 value) {
  return {lhs.x + value, lhs.y + value};
}

Vector2 operator-(const Vector2 lhs, const f32 value) {
  return {lhs.x - value, lhs.y - value};
}

Vector2 operator*(const Vector2 lhs, f32 value) {
  return {lhs.x * value, lhs.y * value};
}

Vector2 operator/(const Vector2 lhs, const f32 value) {
  return {lhs.x / value, lhs.y / value};
}

Vector2 &operator+=(Vector2 &lhs, const f32 value) {
  lhs.x += value;
  lhs.y += value;
  return lhs;
}

Vector2 &operator-=(Vector2 &lhs, const f32 value) {
  lhs.x -= value;
  lhs.y -= value;
  return lhs;
}

Vector2 &operator*=(Vector2 &lhs, const f32 value) {
  lhs.x *= value;
  lhs.y *= value;
  return lhs;
}

Vector2 &operator/=(Vector2 &lhs, const f32 value) {
  lhs.x /= value;
  lhs.y /= value;
  return lhs;
}

Vector2 operator+(const Vector2 &lhs, const Vector2 &rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}

Vector2 operator-(const Vector2 &lhs, const Vector2 &rhs) {
  return {lhs.x - rhs.x, lhs.y - rhs.y};
}

Vector2 operator*(const Vector2 &lhs, const Vector2 &rhs) {
  return {lhs.x * rhs.x, lhs.y * rhs.y};
}

Vector2 operator/(const Vector2 &lhs, const Vector2 &rhs) {
  return {lhs.x / rhs.x, lhs.y / rhs.y};
}

Vector2 &operator+=(Vector2 &lhs, const Vector2 &rhs) {
  lhs.x += rhs.x;
  lhs.y += rhs.y;
  return lhs;
}

Vector2 &operator-=(Vector2 &lhs, const Vector2 &rhs) {
  lhs.x -= rhs.x;
  lhs.y -= rhs.y;
  return lhs;
}

Vector2 &operator*=(Vector2 &lhs, const Vector2 &rhs) {
  lhs.x *= rhs.x;
  lhs.y *= rhs.y;
  return lhs;
}

Vector2 &operator/=(Vector2 &lhs, const Vector2 &rhs) {
  lhs.x /= rhs.x;
  lhs.y /= rhs.y;
  return lhs;
}
