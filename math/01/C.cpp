/************************************************************************
* File Name : poj1061.cpp
* Purpose : exgcd
* Creation Date : 2016年10月22日
* Last Modified : 2016年10月23日 星期日 00时19分35秒
* Created By : gou4shi1@qq.com
************************************************************************/
#include <cstdio>

using namespace std;

typedef long long LL;

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
    int x, y, m, n, l;
    scanf("%d%d%d%d%d", &x, &y, &m, &n, &l);
    LL ans = solve(n-m, l, x-y);
    if (ans == -1)
        printf("Impossible\n");
    else
        printf("%lld\n", ans);
    return 0;
}
