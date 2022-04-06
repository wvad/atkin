#include <napi.h>
#include <cmath>

using ulong = std::uint64_t;

void atkin(const ulong &N, bool *&is_prime) {
  if (N < 2) return;  
  ulong n, y, x;
  const auto sqrtN = std::sqrt(N);
  for (y = 1; y <= sqrtN; y += 6) {
    for (x = 1; x <= sqrtN && (n = 4 * x * x + y * y) <= N; x++)
      is_prime[n] = !is_prime[n];
    for (x = y + 1; x <= sqrtN && (n = 3 * x * x - y * y) <= N; x += 2)
      is_prime[n] = !is_prime[n];
  }
  for (y = 5; y <= sqrtN; y += 6) {
    for (x = 1; x <= sqrtN && (n = 4 * x * x + y * y) <= N; x++)
      is_prime[n] = !is_prime[n];
    for (x = y + 1; x <= sqrtN && (n = 3 * x * x - y * y) <= N; x += 2)
      is_prime[n] = !is_prime[n];
  }
  for (y = 2; y <= sqrtN; y += 6) {
    for (x = 1; x <= sqrtN && (n = 3 * x * x + y * y) <= N; x += 2)
      is_prime[n] = !is_prime[n];
    for (x = y + 1; x <= sqrtN && (n = 3 * x * x - y * y) <= N; x += 2)
      is_prime[n] = !is_prime[n];
  }
  for (y = 4; y <= sqrtN; y += 6) {
    for (x = 1; x <= sqrtN && (n = 3 * x * x + y * y) <= N; x += 2)
      is_prime[n] = !is_prime[n];
    for (x = y + 1; x <= sqrtN && (n = 3 * x * x - y * y) <= N; x += 2)
      is_prime[n] = !is_prime[n];
  }
  for (y = 3; y <= sqrtN; y += 6) {
    for (x = 1; x <= sqrtN && (n = 4 * x * x + y * y) <= N; x += 3)
      is_prime[n] = !is_prime[n];
    for (x = 2; x <= sqrtN && (n = 4 * x * x + y * y) <= N; x += 3)
      is_prime[n] = !is_prime[n];
  }
  for (n = 5; n <= sqrtN; n++) {
    if (is_prime[n]) {
      for (y = n * n; y <= N; y += n * n) {
        is_prime[y] = false;
      }
    }
  }
  is_prime[2] = is_prime[3] = true;
}

constexpr const ulong MAX_SAFE_INTEGER = 9007199254740991;
constexpr const std::int64_t MIN_SAFE_INTEGER = -9007199254740992;

Napi::Value ModuleMain(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (!info[0].IsNumber()) {
    Napi::TypeError::New(env, "Wrong argument: it has to be a number.")
    .ThrowAsJavaScriptException();
    return env.Null();
  }
  double length_b = info[0].As<Napi::Number>().DoubleValue();
  double tmp;
  if (std::modf(length_b, &tmp) != 0.0) {
    Napi::TypeError::New(env, "Wrong argument: it has to be an integer")
    .ThrowAsJavaScriptException();
    return env.Null();
  }
  if (
    length_b < MIN_SAFE_INTEGER || 
    MAX_SAFE_INTEGER < length_b
  ) {
    Napi::TypeError::New(env, "Wrong argument: it has to be safe")
    .ThrowAsJavaScriptException();
    return env.Null();
  }
  auto arr = Napi::Array::New(env);
  if (length_b < 2) {
    return arr;
  }
  ulong length = length_b;
  auto is_prime = new(std::nothrow) bool[length + 2];
  if (!is_prime) {
    Napi::Error::New(env, "Bad alloc").ThrowAsJavaScriptException();
    return env.Null();
  }

  for (ulong i = 0; i < length; i++) is_prime[i] = 0;
  atkin(length, is_prime);
  for (ulong i = 2, index = 0; i < length; i++) {
    if (is_prime[i]) {
      arr[index++] = Napi::Number::New(env, i);
    }
  }
  return arr;
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports) {
  exports = Napi::Function::New(env, ModuleMain);
  return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Initialize)
