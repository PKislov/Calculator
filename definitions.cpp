#include "definitions.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////
///		Файл definitions.cpp - вспомогательные функции и макроопределения
///
///						АРИФМЕТИЧЕСКИЙ КАЛЬКУЛЯТОР 1.0
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

extern bool error;

//================ ПОЖКЛЮЧЕНИЕ СТАНДАРТНЫХ ЗАГОЛОВОЧНЫХ ФАЙЛОВ =============================================
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <cmath>
#ifdef WINDOWS
#include <wchar.h>
#include <locale.h>
#endif



double ToRadians (double corner)
{
    return M_PI * corner / 180.0;
}

double ToGrad (double corner)
{
    return corner * 180.0 / M_PI;
}

double getInfinity ()
{
    double n = 0.;
    return 1./n;
}

double fact (double i)
{
    double n = 1., j;
    i = ABS(round(i));
    for (j=1; j <= i; ) n = n*j, j += 1.;
    if (n == getInfinity()) // если слишком большое значение факториала
    {
        error = true; // глобальная переменная для класса StrToNum
        rprin ("\nОшибка: слишком большое значение факториала !\n");
    }
    return n;
}

int NOD (int x, int y) // Нахождение наибольшего общего делителя
{
    if (x == y)
        return abs(x);

    int count = abs(x) > abs(y) ? abs(x) : abs(y); //выбираем наибольшее из чисел
    for (; count > 1; --count)
        if ((!((x) % count)) && (!((y) % count))) // оба числа делятся на счетчик без остатка?
            return count;

    return 0; // НОД не найден
}
double round (double x)
{
    if (fabs(x) - fabs(int(x)) >= 0.5)
    {
        return x > 0. ? 1+(int)x : (int)x-1;
    }
    else
        return int(x);
}

// Функция завершает работу приложения выводя на экран message - собщение об ошибке
void FatalError (char *message)
{
    rprin ("\n\tИзвините, в ходе работы приложения произошла серьезная ошибка.\n\
\tПричина отказа: ");
    rputs (message);
    printf ("Press <Enter> to continue...");
    getchar ();
    exit (0);
}

// Функция записывает в файл out комплексное число xy с точностью exact знаков после запятой.
// Если exact равно 0, то программа устанавливает точность по своему усмотрению.
// Возвращает количество записаных байтов.
int WriteComplexNumberInFile (double x, double y, FILE *out, int exact)
{
    if (y == 0. && x == 0.)
    {
        return exact ? fprintf (out, "%.*f", exact, 0.) : fprintf (out, "0");
    }

    if (y == 0.)
    {
        return exact ? fprintf (out, "%.*f", exact, x) : fprintf (out, "%g", x);
    }

    if (x == 0.)
    {	if (y == 1.)
            return fprintf (out, "i");
        else
            if (y == -1.)
                return fprintf (out, "-i");
        return exact ? fprintf (out, "%.*fi", exact, y) : fprintf (out, "%gi", y);
    }

    if (y == 1.|| y == -1.)
        return exact ? fprintf (out, "%.*f%ci", exact, x, (y > 0. ? '+' : '-')) : fprintf (out, "%g%ci", x, (y > 0. ? '+' : '-'));
    return exact ? fprintf (out, "%.*f%c%.*fi", exact, x, (y > 0. ? '+' : '-'), exact, ABS(y)) : fprintf (out, "%g%c%gi", x, (y > 0. ? '+' : '-'), ABS(y));
}

#ifdef WINDOWS
inline int  rputcstd (int c)
{
    if (c >= -64 && c <= -17)
    {
        return putc (c - 64, stdout);
    }

    if (c >= -16 && c <= -1)
    {
        return putc (c - 16, stdout);
    }
    return putc (c, stdout);
}

int convert_utf8_to_windows1251(const char* utf8, char* windows1251, size_t n)
{
    int i = 0;
    int j = 0;
    for(; i < (int)n && utf8[i] != 0; ++i)
    {
        char prefix = utf8[i];
        char suffix = utf8[i+1];
        if ((prefix & 0x80) == 0)
        {
            windows1251[j] = (char)prefix;
            ++j;
        }
        else
            if ((~prefix) & 0x20)
            {
                int first5bit = prefix & 0x1F;
                first5bit <<= 6;
                int sec6bit = suffix & 0x3F;
                int unicode_char = first5bit + sec6bit;


                if ( unicode_char >= 0x410 && unicode_char <= 0x44F )
                {
                    windows1251[j] = (char)(unicode_char - 0x350);
                }
                else
                    if (unicode_char >= 0x80 && unicode_char <= 0xFF)
                    {
                        windows1251[j] = (char)(unicode_char);
                    }
                    else
                        if (unicode_char >= 0x402 && unicode_char <= 0x403) {
                            windows1251[j] = (char)(unicode_char - 0x382);
                        }
                        else
                        {
                            int count = sizeof(g_letters) / sizeof(Letter);
                            for (int k = 0; k < count; ++k)
                            {
                                if (unicode_char == g_letters[k].unicode)
                                {
                                    windows1251[j] = g_letters[k].win1251;
                                    goto NEXT_LETTER;
                                }
                            }
                            // can't convert this char
                            return 0;
                        }
NEXT_LETTER:
                ++i;
                ++j;
            }
            else
            {
                // can't convert this chars
                return 0;
            }
    }
    windows1251[j] = 0;
    return 1;
}

#endif

// Выводит на консоль строку с русским текстом:
int rprin (const char * const s)
{
    #ifdef WINDOWS
    const char *p = s;
    const int len = strlen(s) * 7; // один символ utf8 занимает до 6 байт, примем с запасом
    char *output = new char[len];
    if (len) output[len-1] = '\0';
    convert_utf8_to_windows1251(p, output, len); // перевести строку из кодировки UTF8 в С1251
    for (p = output; *p; rputcstd (*(p++)));
    delete output;
    return p-output;
    #else
    return printf(s);
    #endif
}

int rputs (const char * const s)
{
    const size_t cSize = rprin (s);
    putchar('\n');
    return 1+cSize;
}



