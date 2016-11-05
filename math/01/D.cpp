/************************************************************************
* File Name : math-01-D.cpp
* Purpose : Miller-Rabin Algorithm & Pollard-rho Algorithm
* Creation Date : 2016年11月04日
* Last Modified : 2016年11月05日 星期六 20时43分01秒
* Created By : gou4shi1@qq.com
************************************************************************/
#include <cstdio>
#include <ctime>
#include <cstdlib>
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
 * @brief factor decomposition of n (store in factor[])
 *
 * @param n
 */
void factorDecomposition(LL n) {
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
    factorDecomposition(p);
    factorDecomposition(n / p);
}

int main() {
    srand(time(0));
    int T;
    scanf("%d", &T);
    while (T--) {
        LL n;
        scanf("%lld", &n);
        if (Miller_Rabin(n))
            puts("Prime");
        else {
            factor_num = 0;
            factorDecomposition(n);
            LL ans = factor[0];
            for (int i = 0; i != factor_num; ++i)
                ans = min(ans, factor[i]);
            printf("%lld\n", ans);
        }
    }
    return 0;
}
