/*
Var 11
Y = Ax^3 + Cx + D
x 0.0  0.4  0.8  1.2  1.6  2.0 
Y 1.2  2.2  3.0  6.0  7.7  13.6

Определить линию регрессии
Рассчитать коэффициент корреляции
Подобрать функциональную зависимость заданного вида
Вычислить коэффициент регрессии

Построить график экспериментальной зависимости,
линию регрессии, подобранной зависимости.

Определить суммарную квадратичную ошибку и
среднюю ошибку для линии регрессии и 
подобранной функциональной зависимости.
*/

#include <iostream>
#include <cmath>
#include "GnuP.h"

using namespace std;


int gauss(int n, double **A, double *B, double *solve)
{
	int i, j, k, r;
	double temp, M, max, s;
	double **a, *b;
	
	a = (double **) calloc(n, sizeof(double *));
	for(i = 0; i < n; i++)
		a[i] = (double *) calloc(n, sizeof(double));
	b = (double *) calloc(n, sizeof(double));
	
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
			a[i][j] = A[i][j];
	 for(i = 0; i < n; i++)
		b[i] = B[i];
	
	for(k = 0;k < n; k++)
	{
		//Поиск максимального по модулю элемента в k-ом столбце
		max = fabs(a[k][k]);
		r = k;
		
        for(i = k+1; i<n; i++)
			if (fabs(a[i][k]) > max)
			{
				max=fabs(a[i][k]);
				r=i;
			}
			
		//Меняем местами k-ую и r-ую строки
		for(j=0; j<n; j++)
		{
			temp = a[k][j];
			a[k][j] = a[r][j];
			a[r][j] = temp;
		}

		temp = b[k];
		b[k] = b[r];
		b[r] = temp;
		
		// Приведение матрицы к диагональному виду
		for(i=k+1; i<n; i++)
		{
			for(M = a[i][k] / a[k][k], j = k; j < n; j++)
				a[i][j] -= M * a[k][j];
			b[i] -= M * b[k];
		}
	}
	//Обратный ход метода Гаусса
	if (a[n-1][n-1] == 0)
		if(b[n-1] == 0)
			return -1; // бесконечное количество решений
		else return -2; // нет решений
	else
	{
		for(i=n-1; i>=0; i--)
		{
			for(s=0, j=i+1; j<n; j++)
				s += a[i][j] * solve[j];
			solve[i]=(b[i]-s) / a[i][i];
		}
		return 0; // единственное решение
	}
	
	for(i = 0; i < n; i++)
		free(a[i]);
	free(a);
	free(b);
	
}


// y = a0 + a1*x
double reg_line(double x, double a0, double a1)
{
    return a0 + a1*x;
}


// y = A*x^3 + C*x + D
double func(double x, double A, double C, double D)
{
    return A * pow(x, 3) + C * x + D;
}


int main(int argc, char *argv[])
{
    int n = 6;
    double x[n] = {0, 0.4, 0.8, 1.2, 1.6, 2.0};   
    double y[n] = {1.2, 2.2, 3.0, 6.0, 7.7, 13.6};

    // Вычисляем коэффициенты линии регрессии
    double sum_x = 0, sum_y = 0, sum_x2 = 0, sum_x3 = 0, sum_x4 = 0, sum_x6 = 0, sum_xy = 0, sum_x3y = 0;

    for (int i = 0; i < n; i++)
    {
        sum_x += x[i];
        sum_y += y[i];
        sum_x2 += pow(x[i], 2);
        sum_x3 += pow(x[i], 3);
        sum_x4 += pow(x[i], 4);
        sum_x6 += pow(x[i], 6);
        sum_xy += x[i] * y[i];
        sum_x3y += pow(x[i], 3) * y[i];
    }

    double a1 = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    double a0 = (sum_y - a1 * sum_x) / n;

    // Вычисляем коэффициент корреляции
    double Mx = sum_x / n;
    double My = sum_y / n;
    double sum1 = 0, sum2 = 0, sum3 = 0, S = 0, D = 0;
    
    for (int i = 0; i < n; i++)
    {
        sum1 += (x[i] - Mx) * (y[i] - My);
        sum2 += (x[i] - Mx) * (x[i] - Mx);
        sum3 += (y[i] - My) * (y[i] - My);
    }

    double r = sum1 / sqrt(sum2 * sum3);

    // Подбираем функциональную зависимость вида Y = Ax^3 + Cx + D
    double count = 3;
    double **systemA, *massivB, *solve;
    
    systemA = (double **) calloc(count, sizeof(double *));
	for(int i = 0; i < count; i++)
		systemA[i] = (double *) calloc(count, sizeof(double));

    massivB = (double *) calloc(count, sizeof(double));
    solve = (double *) calloc(count, sizeof(double));

    systemA[0][0] = n;
    systemA[0][1] = sum_x;
    systemA[0][2] = sum_x3;
    systemA[1][0] = sum_x;
    systemA[1][1] = sum_x2;
    systemA[1][2] = sum_x4;
    systemA[2][0] = sum_x3;
    systemA[2][1] = sum_x4;
    systemA[2][2] = sum_x6;

    massivB[0] = sum_y;
    massivB[1] = sum_xy;
    massivB[2] = sum_x3y;

    int g = gauss(count, systemA, massivB, solve);

    // Вычисляем индекс корреляции
    double R, tmpsum1 = 0, tmpsum2 = 0;
    double y_r[n], y_f[n];
    for (int i = 0; i < n; i++)
    {
        y_r[i] = reg_line(x[i], a0, a1);
        y_f[i] = func(x[i], solve[2], solve[1], solve[0]);
        tmpsum1 += (y[i] - y_f[i])*(y[i] - y_f[i]);
        tmpsum2 += (y[i] - My)*(y[i] - My);
    }
    R = sqrt(1 - tmpsum1 / tmpsum2);

    
    double sse = 0, rmse = 0, fmse = 0;
    for(int i = 0; i < n; i++)
    {
        sse += pow((y[i] - My), 2);      // Вычисляем суммарную квадратичную ошибку
        rmse += pow((y[i] - y_r[i]), 2); // Вычисляем среднюю ошибку для линии регрессии
        fmse += pow((y[i] - y_f[i]), 2); // Вычисляем среднюю ошибку для функциональной зависимости    
    }
    sse = sqrtf(sse);
    rmse = sqrtf(rmse)/n;
    fmse = sqrtf(fmse)/n;
    

    cout << "Уравнение линии регрессии: " << "y = " << a0 << " + " << a1 << "*x" << endl;
    cout << "Коэффициент корреляции r: " << r << endl;
    cout << "Функциональная зависимость: " << "y = " << solve[2] << "*x^3 + " << solve[1] << "*x + " << solve[0] << endl;
    cout << "Индекс корреляции: " << R << endl;
    cout << "Суммарная квадратичная ошибка: " << sse << endl;
    cout << "Средняя ошибка для линии регрессии: " << rmse << endl;
    cout << "Средняя ошибка для функциональной зависимости: " << fmse << endl;
    cout << "y среднее: " << My << endl;
    cout << "Относительная ошибка для линиии регрессии: " << rmse / My * 100 << " %" << endl;
    cout << "Относительная ошибка для функциональной зависимости: " << fmse / My * 100 << " %" << endl;
    

    /*for(int i = 0; i < n; i++)
        cout << y_r[i] << " ";
    cout << endl;
    for(int i = 0; i < n; i++)
        cout << y_f[i] << " ";*/

    // отображаем точки на графике
    GnuP lin;

    double arrx[21], arry[21];
    double value = 0.0;
    for (int i = 0; i < 21; i++)
    {
        arrx[i] = value;
        arry[i] = func(arrx[i], solve[2], solve[1], solve[0]);
        value += 0.1;
    }

    lin.plotArrayPar(n, x, y, 1, 5, 1, "Экспериментальная зависимость");
    lin.plotArrayPar(n, x, y_r, 2, 3, 11, "Линия регрессии");
    lin.plotArrayPar(21, arrx, arry, 2, 3, 4, "Функциональная зависимость");

    lin.plot();

    return 0;
}