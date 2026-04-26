#ifndef SIMPLEBENCH_SIMPLEBENCH_UTILS_HPP_
#define SIMPLEBENCH_SIMPLEBENCH_UTILS_HPP_

#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <ostream>
#include <string>
#include <type_traits>

namespace simplebench {
namespace internal {
SIMPLEBENCH_API void UseCharPointer(char const volatile*);

template <typename Float>
inline constexpr const char* kFloatFmtStr = "%.*f";

template <>
inline constexpr const char* kFloatFmtStr<long double> = "%.*Lf";
}  // namespace internal

// From:
// 'https://github.com/google/benchmark/blob/main/include/benchmark/utils.h'
// =))))

#if (!defined(__GNUC__) && !defined(__clang__)) || defined(__pnacl__) || \
    defined(__EMSCRIPTEN__)
#define SIMPLEBENCH_HAS_NO_INLINE_ASSEMBLY
#endif

inline void ClobberMemory() {
  std::atomic_signal_fence(std::memory_order_acq_rel);
}

#ifndef SIMPLEBENCH_HAS_NO_INLINE_ASSEMBLY
#if !defined(__GNUC__) || defined(__llvm__) || defined(__INTEL_COMPILER)
template <typename Tp>
[[deprecated(
    "The const-ref version of this method can permit "
    "undesired compiler optimizations in benchmarks")]]
inline void DoNotOptimize(Tp const& value) {
  asm volatile("" : : "r,m"(value) : "memory");
}

template <typename Tp>
inline void DoNotOptimize(Tp& value) {
#if defined(__clang__)
  asm volatile("" : "+r,m"(value) : : "memory");
#else
  asm volatile("" : "+m,r"(value) : : "memory");
#endif
}

template <typename Tp>
inline void DoNotOptimize(Tp&& value) {
#if defined(__clang__)
  asm volatile("" : "+r,m"(value) : : "memory");
#else
  asm volatile("" : "+m,r"(value) : : "memory");
#endif
}
#elif (__GNUC__ >= 5)
template <typename Tp>
[[deprecated(
    "The const-ref version of this method can permit "
    "undesired compiler optimizations in benchmarks")]]
inline typename std::enable_if<std::is_trivially_copyable<Tp>::value &&
                               (sizeof(Tp) <= sizeof(Tp*))>::type
DoNotOptimize(Tp const& value) {
  asm volatile("" : : "r,m"(value) : "memory");
}

template <typename Tp>
[[deprecated(
    "The const-ref version of this method can permit "
    "undesired compiler optimizations in benchmarks")]]
inline typename std::enable_if<!std::is_trivially_copyable<Tp>::value ||
                               (sizeof(Tp) > sizeof(Tp*))>::type
DoNotOptimize(Tp const& value) {
  asm volatile("" : : "m"(value) : "memory");
}

template <typename Tp>
inline typename std::enable_if<std::is_trivially_copyable<Tp>::value &&
                               (sizeof(Tp) <= sizeof(Tp*))>::type
DoNotOptimize(Tp& value) {
  asm volatile("" : "+m,r"(value) : : "memory");
}

template <typename Tp>
inline typename std::enable_if<!std::is_trivially_copyable<Tp>::value ||
                               (sizeof(Tp) > sizeof(Tp*))>::type
DoNotOptimize(Tp& value) {
  asm volatile("" : "+m"(value) : : "memory");
}

template <typename Tp>
inline typename std::enable_if<std::is_trivially_copyable<Tp>::value &&
                               (sizeof(Tp) <= sizeof(Tp*))>::type
DoNotOptimize(Tp&& value) {
  asm volatile("" : "+m,r"(value) : : "memory");
}

template <typename Tp>
inline typename std::enable_if<!std::is_trivially_copyable<Tp>::value ||
                               (sizeof(Tp) > sizeof(Tp*))>::type
DoNotOptimize(Tp&& value) {
  asm volatile("" : "+m"(value) : : "memory");
}
#endif

#elif defined(_MSC_VER)
#include <intrin.h>

template <typename Tp>
[[deprecated(
    "The const-ref version of this method can permit "
    "undesired compiler optimizations in benchmarks")]]
inline void DoNotOptimize(Tp const& value) {
  internal::UseCharPointer(&reinterpret_cast<char const volatile&>(value));
  _ReadWriteBarrier();
}

template <typename Tp>
inline void DoNotOptimize(Tp& value) {
  internal::UseCharPointer(&reinterpret_cast<char const volatile&>(value));
  _ReadWriteBarrier();
}

template <typename Tp>
inline void DoNotOptimize(Tp&& value) {
  internal::UseCharPointer(&reinterpret_cast<char const volatile&>(value));
  _ReadWriteBarrier();
}
#else
template <typename Tp>
inline void DoNotOptimize(Tp&& value) {
  internal::UseCharPointer(&reinterpret_cast<char const volatile&>(value));
}
#endif

enum class TimeUnit : uint8_t {
  kNanosecond,
  kMicrosecond,
  kMillisecond,
  kSecond
};

SIMPLEBENCH_API const char* TimeUnitToString(const TimeUnit unit);

struct Precision {
  int precision;
};

inline Precision SetPrecision(int value) noexcept {
  return Precision{value};
}

class TextStream {
 public:
  TextStream() = default;
  ~TextStream() = default;

  void Clear() { text.clear(); }
  void Reserve(size_t n) { text.reserve(n); }
  const std::string& GetString() const { return text; }

  TextStream& operator<<(Precision value) {
    precision = value;
    return *this;
  }

  TextStream& operator<<(TimeUnit unit) {
    text.append(TimeUnitToString(unit));
    return *this;
  }

  TextStream& operator<<(const std::string& str) {
    text.append(str);
    return *this;
  }

  TextStream& operator<<(const char* value) {
    text.append(value);
    return *this;
  }

  TextStream& operator<<(char value) {
    text.append(1, value);
    return *this;
  }

  TextStream& operator<<(bool value) {
    if (value) {
      text.append("true", 4);
    } else {
      text.append("false", 5);
    }
    return *this;
  }

  template <typename Int>
  typename std::enable_if<std::is_integral<Int>::value, TextStream>::type&
  operator<<(Int value) {
    AppendInt<Int>(value);
    return *this;
  }

  template <typename Float>
  typename std::enable_if<std::is_floating_point<Float>::value,
                          TextStream>::type&
  operator<<(Float value) {
    AppendFloat<Float>(value);
    return *this;
  }

 private:
  std::string text;
  Precision precision{6};

  template <typename Int>
  void AppendInt(Int num) {
    static_assert(std::is_integral<Int>::value,
                  "AppendInt requires an integral type");

    if (num == 0) {
      text += '0';
      return;
    }

    if constexpr (std::is_signed<Int>::value) {
      if (num < 0) {
        text += '-';
        num = Int(-(num + 1)) + 1;
      }
    }

    char buffer[32];
    char* p = buffer + sizeof(buffer);
    int digit_count = 0;

    while (num > 0) {
      if (digit_count && digit_count % 3 == 0) {
        *--p = ',';
      }
      *--p = char('0' + (num % 10));
      num /= 10;
      ++digit_count;
    }

    text.append(
        p, static_cast<std::string::size_type>(buffer + sizeof(buffer) - p));
  }

  template <typename Float>
  void AppendFloat(Float num) {
    static_assert(std::is_floating_point<Float>::value,
                  "AppendFloat requires a floating-point type");

    if (std::isnan(num)) {
      text.append("NaN", 3);
      return;
    }
    if (std::isinf(num)) {
      text.append(num < 0 ? "-Inf" : "Inf", num < 0 ? 4 : 3);
      return;
    }

    if (num < 0) {
      text += '-';
      num = -num;
    }

    auto int_part = static_cast<long long>(num);
    Float frac_part = num - static_cast<Float>(int_part);
    AppendInt(int_part);
    if (precision.precision > 0) {
      text += '.';
      frac_part *= std::pow(10, precision.precision);
      long long frac_int = static_cast<long long>(frac_part + 0.5);
      char buffer[32];
      snprintf(buffer, sizeof(buffer), "%0*lld", precision.precision, frac_int);
      text.append(buffer);
    }
  }
};

using namespace std::chrono;
using Clock = steady_clock;

SIMPLEBENCH_API time_point<Clock> AddDuration(time_point<Clock> tp,
                                              TimeUnit unit,
                                              double value);

SIMPLEBENCH_API double ToDurationAsDouble(TimeUnit unit,
                                          time_point<Clock> start,
                                          time_point<Clock> end);
}  // namespace simplebench

inline std::ostream& operator<<(std::ostream& os,
                                const simplebench::TextStream& text) {
  os << text.GetString();
  return os;
}

#endif