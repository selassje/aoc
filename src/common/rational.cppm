
export module aoc.rational;

import std;

export namespace aoc::rational {

struct Rational
{
  std::int32_t nom{ 0 };
  std::int32_t denom{ 1 };

  template<typename T>
  explicit Rational(const T& t)
    :  nom(static_cast<std::int32_t>(t))
  {
  }

  Rational()
    : nom(0)
    , denom(1) {};

  Rational(std::int32_t nom_, std::int32_t denom_)
    : nom(nom_)
    , denom(denom_)
  {
  }

  explicit operator double() const
  {
    return static_cast<double>(nom) / static_cast<double>(denom);
  }

  friend Rational operator*(const Rational& lhs, const Rational& rhs)
  {
    return Rational{ lhs.nom * rhs.nom, lhs.denom * rhs.denom };
  }

  friend Rational operator-(const Rational& lhs, const Rational& rhs)
  {
    const auto newDenom = lhs.denom * rhs.denom;
    const auto newNom = (lhs.nom * rhs.denom) - (lhs.denom * rhs.nom);
    return { newNom, newDenom };
  }

  friend Rational operator+(const Rational& lhs, const Rational& rhs)
  {
    const auto newDenom = lhs.denom * rhs.denom;
    const auto newNom = (lhs.nom * rhs.denom) + (lhs.denom * rhs.nom);
    return { newNom, newDenom };
  }

  friend Rational operator/(const Rational& lhs, const Rational& rhs)
  {
    return lhs * Rational{ rhs.denom, rhs.nom };
  }

  Rational& operator+=(const Rational& rhs)
  {
    auto result = rhs + *this;
    std::swap(*this, result);
    return *this;
  }

  Rational& operator-=(const Rational& rhs)
  {
    auto result = *this - rhs;
    std::swap(*this, result);
    return *this;
  }
  Rational operator-() const { return { -nom, denom }; }

  [[nodiscard]] bool isInteger() const { return nom % denom == 0; }

  void reduce()
  {
    if ((nom < 0 && denom < 0) || (nom >= 0 && denom < 0)) {
      nom = -nom;
      denom = -denom;
    }
    const auto gcd = std::gcd(nom, denom);
    nom /= gcd;
    denom /= gcd;
  }
};
}