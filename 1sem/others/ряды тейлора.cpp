#include<iostream>
#include<cmath> 

long double privedenie1(long double X)
{
    long double t;
    t = X / (2.0 * M_PI);
    t = roundl(t);
    X -= t * 2 * M_PI;
    return X;
}

long double privedenie2(long double X)
{
    long double t;
    t = X / (2.0 * M_PI);
    t = roundl(t);
    X += t * 2 * M_PI;
    return X;
}


long double mypowl(long double a, long double b) 
{ 
    long double r = 1; 
    if (b == 0) return 1; 
    else if (b > 0) 
    { 
        for (int c = 0; c < b; c++) 
            r *= a; 
    } 
    else if (b < 0) 
    { 
        for (int c = 0; c > b; c--) 
            r *= a; 

        r = 1 / r; 
    } 
    return r; 
} 

long double mysqrtl(long double a)
{
    long double b = 0.5; 
    for (int i = 0; i < 50; i++) 
    { 
        b = b / 2 + a / (2 * b); 
    } 
    return b; 
}

long double myfabsl(long double X)
{
    return (X > 0 ? X : -X);
}

long double mysin(long double X) 
{ 
    long double answ = X; 
    long double factorial = 6, num = 4;  
    long double powl = -X * X * X; 
    for (int i = 0; i < 30; i++) 
    { 
        answ += powl / factorial; 
        factorial *= num * (num + 1); 
        num += 2; 
        powl *= -X * X; 
    } 
    return answ; 
}  

long double mycos(long double X) 
{ 
    long double answ = 1, factorial = 2, num = 3;  
    long double powl = -X * X; 
    for (int i = 0; i < 30; i++) 
    { 
        answ += powl / factorial; 
        factorial *= num * (num + 1); 
        num += 2; 
        powl *= -X * X; 
    } 
    return answ; 
} 

long double myln(long double X)
{
    if (X == 0 || myfabsl(X) == 1) return 0;
    long double e = 2.718281828459045235;
    long double  fx = myfabsl(X);
    long double num = 0;
    long double n = 10000;
    long double c = 0;
    while (fx > 2)
    {
        fx /= e;
        num++;
    }
    while (fx < 2) 
    {
        fx *= e;
        num--;
    }
    for (int i = 1; i <= n; i++) 
    {
        c += mypowl((fx - 1) / (fx + 1), 2 * i - 1) / (2 * i - 1);
    }

    return (2 * c + num);
}

int main ()
{
    long double x, y;
    std:: cin >> x;
    y = x;
   
    if (x > 2 * M_PI)
        x = privedenie1(x);
    else
        x = privedenie2(x);

    if (y == 0)
    {
        std:: cout << "sin(x) = " << mysin(x) << "\n";
        std:: cout << "cos (x) = " << mycos(x) << "\n";
        std:: cout << "ln (x) = error" << "\n";
        
        if (mysin(x) > mycos(x))
            std:: cout << "min = cos(x) = " << mycos(x);
        else if (mysin(x) < mycos(x))
            std:: cout << "min = sin(x) = " << mysin(x);
        else
            std:: cout << "min = sin(x) = cos(x) = " << mysin(x);
    }

    else 
    {
        std:: cout << "sin(x) = " << mysin(x) << "\n";
        std:: cout << "cos (x) = " << mycos(x) << "\n";
        std:: cout << "ln (x) = " << myln(y) << "\n";
        
        if (mysin(x) > mycos(x) && myln(x) > mycos(x))
            std:: cout << "min = cos(x) = " << mycos(x);
        else if (mysin(x) < mycos(x) && mysin(x) < myln(y))
            std:: cout << "min = sin(x) = " << mysin(x);
        else if (myln(x) < mycos(x) && myln(x) < mysin(x))
            std:: cout << "min = ln (x) = " << myln(y);
    }

}
