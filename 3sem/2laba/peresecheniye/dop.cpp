#include <bits\stdc++.h>
using namespace std;

struct ptr
{
    double x, y, z;
    ptr operator-(const ptr &other)
    {
        return ptr{x - other.x, y - other.y, z - other.z};
    }
    ptr operator+(const ptr &other)
    {
        return ptr{x + other.x, y + other.y, z + other.z};
    }
    friend istream &operator>>(istream &, ptr);
};

istream &operator>>(istream &in, ptr p)
{
    in >> p.x >> p.y >> p.z;
    return in;
}

struct str
{
    ptr p1, p2;
    int l, m, k;
    void set_straight(ptr A, ptr B)
    {
        p1 = A;
        p2 = B;
        l = p2.x - p1.x;
        m = p2.y - p1.y;
        k = p2.z - p1.z;
    }
};

struct pln
{
    int A, B, C, D;
    void set_plane(ptr &p1, ptr &p2, ptr &p3)
    {
        A = (p2.y - p1.y) * (p3.z - p1.z) - (p3.y - p1.y) * (p2.z - p1.z);
        B = -((p2.x - p1.x) * (p3.z - p1.z) - (p2.z - p1.z) * (p3.z - p1.z));
        C = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
        D = -p1.x * A - p1.y * B - p1.z * C;
    }
    int check_ptr(ptr &p)
    {
        int a = A * p.x + B * p.y + C * p.z + D;
        if (a < 0)
            return -1;
        if (a > 0)
            return 1;
        return 0;
    }
    double ptr_to_pln(ptr &p)
    {
        return A * p.x + B * p.y + C * p.z + D;
    }
};

bool chekc_pln_str(pln P, str S)
{
    if (P.A * S.l + P.B * S.m + P.C + S.k != 0)
        return true;
    return false;
}

ptr &cross_point(pln &P, str &S)
{
    double t;
    t = -(P.A * S.p1.x + P.B * S.p1.y + P.C * S.p1.z + P.D) / (P.A * (S.p1.x - S.p2.x) + P.B * (S.p1.y - S.p2.y) + P.C * (S.p1.z - S.p2.z));
    ptr crossing;
    crossing.x = S.p1.x + (S.p1.x - S.p2.x) * t;
    crossing.y = S.p1.y + (S.p1.y - S.p2.y) * t;
    crossing.z = S.p1.z + (S.p1.z - S.p2.z) * t;
    return crossing;
}

bool is_middle(ptr &p1, ptr &p2, ptr &p3)
{
    double ro1, ro2, ro3;
    ro1 = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) + (p1.z - p2.z) * (p1.z - p2.z);
    ro2 = (p1.x - p3.x) * (p1.x - p3.x) + (p1.y - p3.y) * (p1.y - p3.y) + (p1.z - p3.z) * (p1.z - p3.z);
    ro1 = (p3.x - p2.x) * (p3.x - p2.x) + (p3.y - p2.y) * (p3.y - p2.y) + (p3.z - p2.z) * (p3.z - p2.z);
    if (ro2 > ro3 || ro1 > ro3)
        return false;
    return true;
}

ptr &vect_mul(ptr v1, ptr v2)
{
    ptr res_v;
    res_v.x = v1.y * v2.z - v1.z * v2.y;
    res_v.y = v1.x * v2.z - v1.z * v2.x;
    res_v.z = v1.x * v2.y - v1.y * v2.x;
    return res_v;
}

double vect_abs(ptr v)
{
    return sqrt(v.x * v.x + v.y * v.y * +v.z * v.z);
}

double dist_ptr_str(str &S, ptr &M)
{
    double ro;
    ro = vect_abs(vect_mul(S.p1 - S.p2, S.p1 - M)) / vect_abs(S.p1 - S.p2);
    return ro;
}

bool in_edge(ptr &M, ptr &A, ptr &B, ptr &C, ptr &D)
{
    double ro1, ro2, ro3, ro4;
    str S;
    S.set_straight(A, D);
    ro1 = dist_ptr_str(S, M);
    S.set_straight(B, C);
    ro2 = dist_ptr_str(S, M);
    if (ro1 > vect_abs(A - B) || ro2 > vect_abs(A - B))
        return false;
    S.set_straight(A, B);
    ro3 = dist_ptr_str(S, M);
    S.set_straight(D, C);
    ro3 = dist_ptr_str(S, M);
    if (ro3 > vect_abs(C - B) || ro4 > vect_abs(C - B))
        return false;
    return true;
}

int main()
{
    ptr A, B, D, C1;
    ptr parall1[8], parall2[8];
    cin >> parall1[0];
    ptr v1, v2, v3;
    parall1[1] = parall1[0] + v1;
    parall1[3] = parall1[0] + v2;
    parall1[7] = parall1[0] + v3;
    parall1[2] = (parall1[2] - parall1[0]) + (parall1[3] - parall1[0]) + parall1[0];
}