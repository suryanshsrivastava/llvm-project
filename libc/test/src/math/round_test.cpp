//===-- Unittests for round -----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "include/math.h"
#include "src/math/round.h"
#include "utils/FPUtil/FPBits.h"
#include "utils/FPUtil/TestHelpers.h"
#include "utils/MPFRWrapper/MPFRUtils.h"
#include "utils/UnitTest/Test.h"

using FPBits = __llvm_libc::fputil::FPBits<double>;

namespace mpfr = __llvm_libc::testing::mpfr;

static const double zero = FPBits::zero();
static const double negZero = FPBits::negZero();
static const double nan = FPBits::buildNaN(1);
static const double inf = FPBits::inf();
static const double negInf = FPBits::negInf();

// Zero tolerance; As in, exact match with MPFR result.
static constexpr mpfr::Tolerance tolerance{mpfr::Tolerance::floatPrecision, 0,
                                           0};
TEST(RoundTest, SpecialNumbers) {
  EXPECT_FP_EQ(zero, __llvm_libc::round(zero));
  EXPECT_FP_EQ(negZero, __llvm_libc::round(negZero));

  EXPECT_FP_EQ(inf, __llvm_libc::round(inf));
  EXPECT_FP_EQ(negInf, __llvm_libc::round(negInf));

  ASSERT_NE(isnan(nan), 0);
  ASSERT_NE(isnan(__llvm_libc::round(nan)), 0);
}

TEST(RoundTest, RoundedNumbers) {
  EXPECT_FP_EQ(1.0, __llvm_libc::round(1.0));
  EXPECT_FP_EQ(-1.0, __llvm_libc::round(-1.0));
  EXPECT_FP_EQ(10.0, __llvm_libc::round(10.0));
  EXPECT_FP_EQ(-10.0, __llvm_libc::round(-10.0));
  EXPECT_FP_EQ(1234.0, __llvm_libc::round(1234.0));
  EXPECT_FP_EQ(-1234.0, __llvm_libc::round(-1234.0));
}

TEST(RoundTest, Fractions) {
  EXPECT_FP_EQ(1.0, __llvm_libc::round(0.5));
  EXPECT_FP_EQ(-1.0, __llvm_libc::round(-0.5));
  EXPECT_FP_EQ(0.0, __llvm_libc::round(0.115));
  EXPECT_FP_EQ(-0.0, __llvm_libc::round(-0.115));
  EXPECT_FP_EQ(1.0, __llvm_libc::round(0.715));
  EXPECT_FP_EQ(-1.0, __llvm_libc::round(-0.715));
  EXPECT_FP_EQ(1.0, __llvm_libc::round(1.3));
  EXPECT_FP_EQ(-1.0, __llvm_libc::round(-1.3));
  EXPECT_FP_EQ(2.0, __llvm_libc::round(1.5));
  EXPECT_FP_EQ(-2.0, __llvm_libc::round(-1.5));
  EXPECT_FP_EQ(2.0, __llvm_libc::round(1.75));
  EXPECT_FP_EQ(-2.0, __llvm_libc::round(-1.75));
  EXPECT_FP_EQ(10.0, __llvm_libc::round(10.32));
  EXPECT_FP_EQ(-10.0, __llvm_libc::round(-10.32));
  EXPECT_FP_EQ(11.0, __llvm_libc::round(10.65));
  EXPECT_FP_EQ(-11.0, __llvm_libc::round(-10.65));
  EXPECT_FP_EQ(1234.0, __llvm_libc::round(1234.38));
  EXPECT_FP_EQ(-1234.0, __llvm_libc::round(-1234.38));
  EXPECT_FP_EQ(1235.0, __llvm_libc::round(1234.96));
  EXPECT_FP_EQ(-1235.0, __llvm_libc::round(-1234.96));
}

TEST(RoundTest, InDoubleRange) {
  using UIntType = FPBits::UIntType;
  constexpr UIntType count = 10000000;
  constexpr UIntType step = UIntType(-1) / count;
  for (UIntType i = 0, v = 0; i <= count; ++i, v += step) {
    double x = FPBits(v);
    if (isnan(x) || isinf(x))
      continue;

    ASSERT_MPFR_MATCH(mpfr::Operation::Round, x, __llvm_libc::round(x),
                      tolerance);
  }
}
