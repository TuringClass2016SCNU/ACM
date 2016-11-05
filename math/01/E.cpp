/************************************************************************
* File Name : math-01-E.cpp
* Purpose : decryption of RSA
* Creation Date : 2016年11月05日
* Last Modified : 2016年11月05日 星期六 23时10分14秒
* Created By : gou4shi1@qq.com
************************************************************************/
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>

using namespace std;

typedef long long LL;

//Miller-Rabin Test

/**
 * @brief fast multipy with mod
 *
 * @param a
 * @param b
 * @param m
 *
 * @return a*b (mod m)
 */
LL mult_mod(LL a, LL b, LL m) {
    a %= m;
    b %= m;
    LL ret = 0, temp = a;
    //e.g. 3*5 = 3*(101) = 3*(100) + 3*(1)  (binary)
    while (b) {
        if (b & 1) {
            ret += temp;
            if  (ret > m)
                ret -= m; //minus is faster than mod
        }
        temp <<= 1;
        if (temp > m)
            temp -= m;
        b >>= 1;
    }
    return ret;
}

/**
 * @brief fast power with mod
 *
 * @param a
 * @param n
 * @param m
 *
 * @return a^n (mod m)
 */
LL pow_mod(LL a, LL n, LL m) {
    LL ret = 1, temp = a % m;
    //e.g. 3^5 = 3^(101) = 3^(100) * 3^(1)  (binary)
    while (n) {
        if (n & 1)
            ret = mult_mod(ret, temp, m);
        temp = mult_mod(temp, temp, m);
        n >>= 1;
    }
    return ret;
}

/**
 * @brief check whether n is a composite number (subroutine of Miller-Rabin Test)
 *
 * @param a
 * @param n
 * @param x
 * @param t n-1 = x * 2^t
 *
 * @return ture if n must be a composite number;
 *         false if n may be a prime number.
 */
bool check(LL a, LL n, LL x, LL t) {
    LL ret = pow_mod(a, x, n);
    LL last = ret;
    for (int i = 1; i <= t; ++i) {
        ret = mult_mod(ret, ret, n);
        if (ret == 1 && last != 1 && last != n-1)
            return true;
        last = ret;
    }
    return (ret != 1);
}

/**
 * @brief Miller-Rabin Test
 *
 * @param n
 *
 * @return true if n is a prime number (almost certainly)
 */
bool Miller_Rabin(LL n) {
    if (n < 2)
        return false;
    if (n == 2)
        return true;
    if (!(n & 1))
        return false;

    const static int S = 10;
    LL x = n - 1, t = 0;
    while (!(x & 1)) {
        x >>= 1;
        ++t;
    }

    for (int i = 0; i != S; ++i) {
        LL a = rand()%(n-1) + 1;
        if (check(a, n, x, t))
            return false;
    }
    return true;
}

//pollard-rho

/**
 * @brief Euclidean Algorithm
 *
 * @param a
 * @param b
 *
 * @return max common factor of a and b
 */
LL gcd(LL a, LL b) {
    LL t;
    while (b) {
        t = a;
        a = b;
        b = t % b;
    }
    return a >= 0 ? a : -a;
}

/**
 * @brief find a factor of x
 *
 * @param x
 * @param c a const in this function
 *
 * @return a factor fo x (may return x itself)
 */
LL pollard_rho(LL x, LL c) {
    LL i = 1, k = 2;
    LL x0 = rand()%(x-1) + 1;
    LL y = x0;

    while (true) {
        ++i;
        x0 = (mult_mod(x0, x0, x) + c) % x;
        LL d = gcd(y - x0, x);
        if (d != 1 && d != x)
            return d;
        if (y == x0)
            return x;
        if (i == k) {
            y = x0;
            k += k;
        }
    }
}

const int MAX_FACTOR_NUM = 100;
LL factor[MAX_FACTOR_NUM];
int factor_num;

/**
 * @brief subroutine of factorDecomposition
 *
 * @param n
 */
void findFactor(LL n) {
    if (n == 1)
        return;
    if (Miller_Rabin(n)) {
        factor[factor_num++] = n;
        return;
    }

    const static int C = 107;
    int c = C;
    LL p = n;
    while (p >= n)
        p = pollard_rho(p, c--);
    findFactor(p);
    findFactor(n / p);
}

/**
 * @brief factor decomposition of n (store in factor[])
 *
 * @param n
 */
void factorDecomposition(LL n) {
    factor_num = 0;
    findFactor(n);
}

/**
 * @brief solve ax + by = gcd
 *
 * @param a
 * @param b
 * @param x
 * @param y
 *
 * @return gcd = gcd(a, b)
 */
LL exgcd(LL a, LL b, LL &x, LL &y) {
    LL x0 = 1, y0 = 0, x1 = 0, y1 = 1;
    x = 0;
    y = 1;
    LL r = a % b;
    LL q = (a - r) / b;

    while (r) {
        x = x0 - q*x1; y = y0 - q*y1;
        x0 = x1; y0 = y1;
        x1 = x; y1 = y;
        a = b; b = r;
        r = a % b; q = (a - r) / b;
    }
    return b;
}

/**
 * @brief solve ax + by = d
 *
 * @param a
 * @param b
 * @param d
 *
 * @return minimal positive integral solution x
 */
LL solve(LL a, LL b, LL d) {
    LL x, y;
    LL gcd = exgcd(a, b, x, y);

    if (d % gcd)
        return -1;

    LL x0 = x * d / gcd;
    LL k = b / gcd;
    // x = x0 + k * t (t is integer)
    return (x0 % k + k) % k;
}

int main() {
    //freopen("in", "r", stdin);
    srand(time(0));
    LL c, e, n;
    while (~scanf("%lld%lld%lld", &c, &e, &n)) {
        factorDecomposition(n);
        LL p = factor[0], q = factor[1];
        LL t = (p-1) * (q-1);
        LL d = solve(e, t, 1);
        LL m = pow_mod(c, d, n);
        printf("%lld\n", m);
    }
    return 0;
}

