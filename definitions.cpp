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


//================ МАКРООПРЕДЕЛЕНИЯ ========================================================================

// наибольшее из двух чисел
#define MAX(a,b) ( ((a) > (b)) ? (a) : (b) )

// наименьшее из двух чисел
#define MIN(a,b) (((a) > (b)) ? (b) : (a))

// модуль числа
#define ABS(x) ((x) >= 0.) ? (x) : -1.*(x)

// Физические константы:
#define k_c		299.792458E6
#define k_g		9.80665
#define k_G		66.725985E-12
#define k_Vm	22.4141E-3
#define k_Na	602.2136736E21
#define k_e		160.2177335E-21
#define k_me	910.9389754E-33
#define k_mp	1.67262311E-27
#define k_mn	1.67492861E-27
#define k_h		662.607554E-36
#define k_k		13.8065812E-24
#define k_R		8.3145107
#define k_m0	1.256637061E-6
#define k_e0	8.854187818E-12

// Математические константы:
#define M_E     2.71828182845904523536
#define M_PI    3.14159265358979323846



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

typedef class ConvLetter {
public:
        char    win1251;
        int             unicode;
        ConvLetter (char ch, int i)
        {
            win1251 = ch; unicode = i;
        }
} Letter;


static Letter g_letters[] = {
    ConvLetter(0x82, 0x201A), // SINGLE LOW-9 QUOTATION MARK
         ConvLetter(0x83, 0x0453), // CYRILLIC SMALL LETTER GJE
         ConvLetter(0x84, 0x201E), // DOUBLE LOW-9 QUOTATION MARK
         ConvLetter(0x85, 0x2026), // HORIZONTAL ELLIPSIS
         ConvLetter(0x86, 0x2020), // DAGGER
         ConvLetter(0x87, 0x2021), // DOUBLE DAGGER
         ConvLetter(0x88, 0x20AC), // EURO SIGN
         ConvLetter(0x89, 0x2030), // PER MILLE SIGN
         ConvLetter(0x8A, 0x0409), // CYRILLIC CAPITAL LETTER LJE
         ConvLetter(0x8B, 0x2039), // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
         ConvLetter(0x8C, 0x040A), // CYRILLIC CAPITAL LETTER NJE
         ConvLetter(0x8D, 0x040C), // CYRILLIC CAPITAL LETTER KJE
         ConvLetter(0x8E, 0x040B), // CYRILLIC CAPITAL LETTER TSHE
         ConvLetter(0x8F, 0x040F), // CYRILLIC CAPITAL LETTER DZHE
         ConvLetter(0x90, 0x0452), // CYRILLIC SMALL LETTER DJE
         ConvLetter(0x91, 0x2018), // LEFT SINGLE QUOTATION MARK
         ConvLetter(0x92, 0x2019), // RIGHT SINGLE QUOTATION MARK
         ConvLetter(0x93, 0x201C), // LEFT DOUBLE QUOTATION MARK
         ConvLetter(0x94, 0x201D), // RIGHT DOUBLE QUOTATION MARK
         ConvLetter(0x95, 0x2022), // BULLET
         ConvLetter(0x96, 0x2013), // EN DASH
         ConvLetter(0x97, 0x2014), // EM DASH
         ConvLetter(0x99, 0x2122), // TRADE MARK SIGN
         ConvLetter(0x9A, 0x0459), // CYRILLIC SMALL LETTER LJE
         ConvLetter(0x9B, 0x203A), // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
         ConvLetter(0x9C, 0x045A), // CYRILLIC SMALL LETTER NJE
         ConvLetter(0x9D, 0x045C), // CYRILLIC SMALL LETTER KJE
         ConvLetter(0x9E, 0x045B), // CYRILLIC SMALL LETTER TSHE
         ConvLetter(0x9F, 0x045F), // CYRILLIC SMALL LETTER DZHE
         ConvLetter(0xA0, 0x00A0), // NO-BREAK SPACE
         ConvLetter(0xA1, 0x040E), // CYRILLIC CAPITAL LETTER SHORT U
         ConvLetter(0xA2, 0x045E), // CYRILLIC SMALL LETTER SHORT U
         ConvLetter(0xA3, 0x0408), // CYRILLIC CAPITAL LETTER JE
         ConvLetter(0xA4, 0x00A4), // CURRENCY SIGN
         ConvLetter(0xA5, 0x0490), // CYRILLIC CAPITAL LETTER GHE WITH UPTURN
         ConvLetter(0xA6, 0x00A6), // BROKEN BAR
         ConvLetter(0xA7, 0x00A7), // SECTION SIGN
         ConvLetter(0xA8, 0x0401), // CYRILLIC CAPITAL LETTER IO
         ConvLetter(0xA9, 0x00A9), // COPYRIGHT SIGN
         ConvLetter(0xAA, 0x0404), // CYRILLIC CAPITAL LETTER UKRAINIAN IE
         ConvLetter(0xAB, 0x00AB), // LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
         ConvLetter(0xAC, 0x00AC), // NOT SIGN
         ConvLetter(0xAD, 0x00AD), // SOFT HYPHEN
         ConvLetter(0xAE, 0x00AE), // REGISTERED SIGN
         ConvLetter(0xAF, 0x0407), // CYRILLIC CAPITAL LETTER YI
         ConvLetter(0xB0, 0x00B0), // DEGREE SIGN
         ConvLetter(0xB1, 0x00B1), // PLUS-MINUS SIGN
         ConvLetter(0xB2, 0x0406), // CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
         ConvLetter(0xB3, 0x0456), // CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
         ConvLetter(0xB4, 0x0491), // CYRILLIC SMALL LETTER GHE WITH UPTURN
         ConvLetter(0xB5, 0x00B5), // MICRO SIGN
         ConvLetter(0xB6, 0x00B6), // PILCROW SIGN
         ConvLetter(0xB7, 0x00B7), // MIDDLE DOT
         ConvLetter(0xB8, 0x0451), // CYRILLIC SMALL LETTER IO
         ConvLetter(0xB9, 0x2116), // NUMERO SIGN
         ConvLetter(0xBA, 0x0454), // CYRILLIC SMALL LETTER UKRAINIAN IE
         ConvLetter(0xBB, 0x00BB), // RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
         ConvLetter(0xBC, 0x0458), // CYRILLIC SMALL LETTER JE
         ConvLetter(0xBD, 0x0405), // CYRILLIC CAPITAL LETTER DZE
         ConvLetter(0xBE, 0x0455), // CYRILLIC SMALL LETTER DZE
         ConvLetter(0xBF, 0x0457) // CYRILLIC SMALL LETTER YI

};

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

#define LINE_MAX 1024
#endif

// Выводит на консоль строку с русским текстом:
int rprin (const char * const s)
{
    #ifdef WINDOWS
    const char *p = s;
    char output[LINE_MAX] = {0};
    convert_utf8_to_windows1251(p, output, LINE_MAX); // перевести строку из кодировки UTF8 в С1251
    for (p = output; *p; rputcstd (*(p++)));
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



