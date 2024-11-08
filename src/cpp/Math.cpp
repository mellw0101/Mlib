/// @file Math.cpp
#include "../include/Math.h"
#include "../include/Assert.h"

using namespace std;

namespace Mlib ::Math {
  double clamp(double a, double b, double x) {
    ASSERT_STATEMENT(a < b);
    return x > b ? x = b : x < a ? x = a : x;
  }

  bool between(double a, double b, double x) {
    ASSERT_STATEMENT(a < b);
    return x > b ? false : x < a ? false : true;
  }

  double calculateBounceVelocity(double initialVelocity, double restitutionCoefficient) {
    return -initialVelocity * restitutionCoefficient;
  }
}
