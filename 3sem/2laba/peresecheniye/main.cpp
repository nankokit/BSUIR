#include <iostream>
#include <limits>

class Point
{
public:
    int x;
    int y;
    int z;

    void setParameters(int x, int y, int z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    void printPoint()
    {
        std::cout << "(" << x << " ," << y << " ," << z << ")\n";
    }

    Point operator-(const Point &other)
    {
        return Point{x - other.x, y - other.y, z - other.z};
    }

    Point operator+(const Point &other)
    {
        return Point{x + other.x, y + other.y, z + other.z};
    }
};

class Plane
{
public:
    int A;
    int B;
    int C;
    int D;

    Plane(Point &p1, Point &p2, Point &p3)
    {
        A = (p2.y - p1.y) * (p3.z - p1.z) - (p3.y - p1.y) * (p2.z - p1.z);
        B = -(p2.x - p1.x) * (p3.z - p1.z) + (p3.x - p1.x) * (p2.z - p1.z);
        C = (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
        D = -p1.x * A - p1.y * B - p1.z * C;
    }

    void printEquation()
    {
        std::cout << "X = " << A << "\nY = " << B << "\nZ = " << C << "\nD = " << D << std::endl;
    }

    bool isEnabled(Point p)
    {
        int result = A * p.x + B * p.y + C * p.z + D;
        if (result > 0)
            return 1;
        else if (result < 0)
            return -1;
        else
            return 0;
    }
};

class Straight
{
public:
    Point p1, p2;
    int l, m, k;

    void setStraight(Point A, Point B)
    {
        p1 = A;
        p2 = B;
        l = p2.x - p1.x;
        m = p2.y - p1.y;
        k = p2.z - p1.z;
    }
};

int checkInput()
{
    int num;
    std::cin >> num;
    while (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Incorrect\n";
        std::cin >> num;
    }
    return num;
}

void inputPoint(Point &newPoint)
{
    std::cout << "Enter x: ";
    newPoint.x = checkInput();
    std::cout << "Enter y: ";
    newPoint.y = checkInput();
    std::cout << "Enter z: ";
    newPoint.z = checkInput();
    std::cout << "Succesfully added a point ";
    newPoint.printPoint();
}

void inputBox(Point box[])
{
    std::cout << "Adding 1 point...\n";
    inputPoint(box[0]);
    std::cout << "Adding 2 point...\n";
    inputPoint(box[1]);
    std::cout << "Adding 4 point...\n";
    inputPoint(box[3]);
    std::cout << "Adding 5 point...\n";
    inputPoint(box[4]);
    std::cout << "Adding 3 point...\n";
    box[2] = box[1] - box[0] + box[3];
    std::cout << "Succesfully added a point ";
    box[2].printPoint();
    std::cout << "Adding 6 point...\n";
    box[5] = box[4] - box[0] + box[1];
    std::cout << "Succesfully added a point ";
    box[5].printPoint();
    std::cout << "Adding 7 point...\n";
    box[6] = box[4] - box[0] + box[2];
    std::cout << "Succesfully added a point ";
    box[6].printPoint();
    std::cout << "Adding 8 point...\n";
    box[7] = box[4] - box[0] + box[3];
    std::cout << "Succesfully added a point ";
    box[7].printPoint();
}

bool isEnabled(Point box1[], Point box2[])
{
    Plane abcd(box1[0], box1[1], box1[2]);
    Plane ABCD(box1[4], box1[5], box1[6]);
    Plane aADd(box1[0], box1[4], box1[7]);
    Plane bBCc(box1[1], box1[5], box1[6]);
    Plane aABb(box1[0], box1[4], box1[5]);
    Plane dDCc(box1[3], box1[7], box1[6]);
    for (int i = 0; i < 7; i++)
    {
        if (abcd.isEnabled(box1[i]) * ABCD.isEnabled(box1[i]) <= 0 && aADd.isEnabled(box1[i]) * bBCc.isEnabled(box1[i]) <= 0 && aABb.isEnabled(box1[i]) * dDCc.isEnabled(box1[i]) <= 0)
        {
            std::cout << "Point " << i + 1 << "is enabled\n";
            return true;
        }
    }
    return false;
}

bool isIntersect(Plane P, Straight S)
{
    if (P.A * S.l + P.B * S.m + P.C + S.k != 0)
        return true;
    return false;
}

Point intersectionPoint(Plane P, Straight S)
{
    double t;
    t = -(P.A * S.p1.x + P.B * S.p1.y + P.C * S.p1.z + P.D) / (P.A * (S.p1.x - S.p2.x) + P.B * (S.p1.y - S.p2.y) + P.C * (S.p1.z - S.p2.z));
    Point intersection;
    intersection.x = S.p1.x + (S.p1.x - S.p2.x) * t;
    intersection.y = S.p1.y + (S.p1.y - S.p2.y) * t;
    intersection.z = S.p1.z + (S.p1.z - S.p2.z) * t;
    return intersection;
}

// void creatingPlaneArray(Plane array[], Point box1[])
// {
//     Plane abcd(box1[0], box1[1], box1[2]);
//     array[0] = abcd;
//     Plane ABCD(box1[4], box1[5], box1[6]);
//     array[1] = ABCD;
//     Plane aADd(box1[0], box1[4], box1[7]);
//     array[2] = aADd;
//     Plane bBCc(box1[1], box1[5], box1[6]);
//     array[3] = bBCc;
//     Plane aABb(box1[0], box1[4], box1[5]);
//     array[4] = aABb;
//     Plane dDCc(box1[3], box1[7], box1[6]);
//     array[5] = dDCc;
// }

// void creatingStraightArray(Straight array)
// {
//     for (int i = 0; i < 12; i++)
//     {
//         array[i] = setStraight()
//     }

// }

// void checkIntersection(Point box1[], Point box2[])
// {
//     Plane *array1;
//     creatingPlaneArray(array1, box1);
//     if (isIntersect(array, box1))
// }

int main()
{
    Point box1[8];
    Point box2[8];
    std::cout << "Adding first box...\n";
    inputBox(box1);
    std::cout << "\nAdding second box...\n";
    inputBox(box2);
    if (isEnabled(box1, box2) || isEnabled(box2, box1))
    {
        std::cout << "The boxes intersect\n";
    }
    //
    return 1;
}
