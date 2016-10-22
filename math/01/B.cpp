/************************************************************************
* File Name : math-01-B.cpp
* Purpose : quick-power-mod & quick-multipy-mod
* Creation Date : 2016年10月21日
* Last Modified : 2016年10月22日 星期六 15时33分27秒
* Created By : gou4shi1@qq.com
************************************************************************/
#include <cstdio>

using namespace std;

const int MOD = 1000;

typedef long long LL;

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
    for (; b; b >>= 1) {
        if (b & 1) {
            ret += temp;
            if  (ret > m)
                ret -= m; //minus is faster than mod
        }
        temp <<= 1;
        if (temp > m)
            temp -= m;
    }
    return ret;
}

/**
 * @brief fast power with mod
 *
 * @param a
 * @param n (n >= 0)
 * @param m
 *
 * @return a^n (mod m)
 */
LL pow_mod(LL a, LL n, LL m) {
    LL ret = 1, temp = a % m;
    //e.g. 3^5 = 3^(101) = 3^(100) * 3^(1)  (binary)
    for (; n; n >>= 1) {
        if (n & 1)
            ret = mult_mod(ret, temp, m);
        temp = mult_mod(temp, temp, m);
    }
    return ret;
}

int main() {
    //freopen("in", "r", stdin);
    int a, b;
    while (~scanf("%d%d", &a, &b) && !(a==0 && b==0))
        printf("%lld\n", pow_mod(a, b, MOD));
    return 0;
}
