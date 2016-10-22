/************************************************************************
* File Name : math-01-A.cpp
* Purpose : quick-matrix-power
* Creation Date : 2016年10月22日
* Last Modified : 2016年10月22日 星期六 15时30分34秒
* Created By : gou4shi1@qq.com
************************************************************************/
#include <cstdio>

using namespace std;

const int MAXN = 5;
const int MOD = 10000;

//2*2
struct Matrix {
    int v[MAXN][MAXN];
};

/**
 * @brief Matrix Multipication (2*2 Matrix)
 *
 * @param A
 * @param B
 *
 * @return A*B (mod MOD)
 */
Matrix operator* (Matrix A, Matrix B) {
    Matrix C;
    C.v[0][0] = (A.v[0][0]*B.v[0][0] + A.v[0][1]*B.v[1][0]) % MOD;
    C.v[0][1] = (A.v[0][0]*B.v[0][1] + A.v[0][1]*B.v[1][1]) % MOD;
    C.v[1][0] = (A.v[1][0]*B.v[0][0] + A.v[1][1]*B.v[1][0]) % MOD;
    C.v[1][1] = (A.v[1][0]*B.v[0][1] + A.v[1][1]*B.v[1][1]) % MOD;
    return C;
}

Matrix operator*= (Matrix &A, Matrix &B) {
    return A = A * B;
}

/**
 * @brief fast matrix power (2*2 Matrix)
 *
 * @param A
 * @param n (n >= 0)
 *
 * @return A^n (mod MOD)
 */
Matrix operator^ (Matrix A, int n) {
    Matrix ret;
    //initialize ret to Identity Matrix
    ret.v[0][0] = 1; ret.v[0][1] = 0;
    ret.v[1][0] = 0; ret.v[1][1] = 1;
    Matrix temp = A;

    //just as normal fast power
    for (; n; n >>= 1) {
        if (n & 1)
            ret *= temp;
        temp *= temp;
    }
    return ret;
}

int main() {
    //freopen("in", "r", stdin);
    int n;
    while (~scanf("%d", &n) && n != -1) {
        Matrix m;
        m.v[0][0] = 1; m.v[0][1] = 1;
        m.v[1][0] = 1; m.v[1][1] = 0;
        printf("%d\n", (m^n).v[0][1]);
    }
    return 0;
}
