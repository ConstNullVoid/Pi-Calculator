#include "pi.h"
#include <gmp.h>
#include <flint/fmpz.h>
#include <cmath>
#include <cstdint>
#include <string>

namespace {
struct BSResult {
    fmpz_t P;
    fmpz_t Q;
    fmpz_t T;
    BSResult() {
        fmpz_init(P);
        fmpz_init(Q);
        fmpz_init(T);
    }
    ~BSResult() {
        fmpz_clear(P);
        fmpz_clear(Q);
        fmpz_clear(T);
    }
    BSResult(const BSResult&) = delete;
    BSResult& operator=(const BSResult&) = delete;
};

constexpr unsigned long A = 13591409UL;
constexpr unsigned long B = 545140134UL;
constexpr unsigned long C = 640320UL;

void set_u64(fmpz_t result, std::uint64_t value) {
    fmpz_set_ui(result, static_cast<ulong>(value));
}

void binary_split(std::uint64_t a, std::uint64_t b, BSResult& r) {
    if (b - a == 1) {
        if (a == 0) {
            fmpz_one(r.P);
            fmpz_one(r.Q);
            fmpz_set_ui(r.T, A);
            return;
        }
        fmpz_t p, q, t, x;
        fmpz_init(p);
        fmpz_init(q);
        fmpz_init(t);
        fmpz_init(x);
        set_u64(x, 6 * a - 5);
        set_u64(p, 2 * a - 1);
        fmpz_mul(x, x, p);
        set_u64(p, 6 * a - 1);
        fmpz_mul(p, x, p);
        set_u64(q, a);
        fmpz_mul(q, q, q);
        set_u64(x, a);
        fmpz_mul(q, q, x);
        fmpz_mul_ui(q, q, C);
        fmpz_mul_ui(q, q, C);
        fmpz_mul_ui(q, q, C);
        fmpz_divexact_ui(q, q, 24UL);
        set_u64(x, a);
        fmpz_mul_ui(x, x, B);
        fmpz_add_ui(x, x, A);
        fmpz_mul(t, p, x);
        if (a & 1)
            fmpz_neg(t, t);
        fmpz_set(r.P, p);
        fmpz_set(r.Q, q);
        fmpz_set(r.T, t);
        fmpz_clear(p);
        fmpz_clear(q);
        fmpz_clear(t);
        fmpz_clear(x);
        return;
    }
    std::uint64_t m = a + (b - a) / 2;
    BSResult left;
    BSResult right;
    binary_split(a, m, left);
    binary_split(m, b, right);
    fmpz_mul(r.P, left.P, right.P);
    fmpz_mul(r.Q, left.Q, right.Q);
    fmpz_mul(r.T, left.T, right.Q);
    fmpz_t temp;
    fmpz_init(temp);
    fmpz_mul(temp, left.P, right.T);
    fmpz_add(r.T, r.T, temp);
    fmpz_clear(temp);
}

bool build_pi(std::uint64_t digits, std::string& output) {
    constexpr long double digits_per_term = 14.181647462725477L;
    std::uint64_t terms = static_cast<std::uint64_t>(std::ceil(static_cast<long double>(digits) / digits_per_term)) + 1;
    BSResult result;
    binary_split(0, terms, result);
    mp_bitcnt_t precision = static_cast<mp_bitcnt_t>(std::ceil(static_cast<long double>(digits) * 3.32192809488736234787L) + 512);
    mpz_t q_integer, t_integer;
    flint_mpz_init_set_readonly(q_integer, result.Q);
    flint_mpz_init_set_readonly(t_integer, result.T);
    mpf_t q, t, sqrt_c, numerator, pi;
    mpf_inits(q, t, sqrt_c, numerator, pi, nullptr);
    mpf_set_prec(q, precision);
    mpf_set_prec(t, precision);
    mpf_set_prec(sqrt_c, precision);
    mpf_set_prec(numerator, precision);
    mpf_set_prec(pi, precision);
    mpf_set_z(q, q_integer);
    mpf_set_z(t, t_integer);
    mpf_set_ui(sqrt_c, 10005UL);
    mpf_sqrt(sqrt_c, sqrt_c);
    mpf_mul_ui(numerator, sqrt_c, 426880UL);
    mpf_mul(numerator, numerator, q);
    mpf_div(pi, numerator, t);
    mp_exp_t exponent;
    char* raw = mpf_get_str(nullptr, &exponent, 10, static_cast<std::size_t>(digits) + 2, pi);
    if (!raw) {
        flint_mpz_clear_readonly(q_integer);
        flint_mpz_clear_readonly(t_integer);
        mpf_clears(q, t, sqrt_c, numerator, pi, nullptr);
        return false;
    }
    void (*free_function)(void*, size_t) = nullptr;
    mp_get_memory_functions(nullptr, nullptr, &free_function);
    std::string value(raw);
    if (value.empty()) {
        free_function(raw, value.size() + 1);
        flint_mpz_clear_readonly(q_integer);
        flint_mpz_clear_readonly(t_integer);
        mpf_clears(q, t, sqrt_c, numerator, pi, nullptr);
        return false;
    }
    output.clear();
    output.reserve(static_cast<std::size_t>(digits) + 3);
    output.push_back(value[0]);
    output.push_back('.');
    if (value.size() > 1)
        output.append(value.begin() + 1, value.end());
    if (output.size() > static_cast<std::size_t>(digits) + 2)
        output.resize(static_cast<std::size_t>(digits) + 2);
    output.push_back('\n');
    free_function(raw, value.size() + 1);
    flint_mpz_clear_readonly(q_integer);
    flint_mpz_clear_readonly(t_integer);
    mpf_clears(q, t, sqrt_c, numerator, pi, nullptr);
    return true;
}
}

bool calculate_pi(std::uint64_t digits, std::string& output) {
    return build_pi(digits, output);
}

long double estimate_file_size(std::uint64_t digits) {
    return static_cast<long double>(digits) + 3.0L;
}

long double estimate_ram(std::uint64_t digits) {
    long double bits = static_cast<long double>(digits) * 3.32192809488736234787L;
    long double limbs = std::ceil(bits / static_cast<long double>(GMP_NUMB_BITS));
    long double integer_bytes = limbs * sizeof(mp_limb_t);
    long double terms = std::ceil(static_cast<long double>(digits) / 14.181647462725477L);
    return integer_bytes * 18.0L + terms * 256.0L + 64.0L * 1024.0L * 1024.0L;
}