#include "stringtonumber.h"
#include "string.h"

extern char ListNamesUnique [][10];

//=================== ОПИСАНИЕ КОНСТРУКТОРА КЛАССА =======================================

StringToNumber::StringToNumber ()
{
    countVariables = 0; // обнуление счетчика пользовательских переменных.
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
///		методы класса StringToNumber работы с двусвязным списком лексем.
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void StringToNumber::Del_2_Elements	()
{
    DelElementMyVersion (temp->next, &start, &end);
    DelElementMyVersion (temp->next, &start, &end);
    countToken -= 2;
}
//=============================================================
void StringToNumber::DeleteAll (struct token **start)
{
    struct token *temp, *del;
    temp = *start;
    while (temp) // просмотреть список, по очереди удалив все элементы
    {
        del = temp;
        temp = temp->next;
        delete del;
    }
    *start = NULL;
}
//=============================================================
void StringToNumber::AddInEndVersionShildt (struct token *add, struct token **end)
{
    if (!*end) // если список пуст
    {
        add->next = add->prior = NULL;
        *end = add;
        return;
    }
    else
        (*end)->next = add;
    add->next = NULL; // обозначить конец списка
    add->prior = *end; // если end нулевой (в списке еще не было элементов), то это утверждение обозначает начало списка,
    //иначе вставляемый элемент будет указывать на конец списка
    *end = add; // переместить указатель на конец массива
}
//=============================================================
void StringToNumber::DelElementMyVersion (struct token *del, struct token **start, struct token **end)
{
    if (del->prior)
        del->prior->next = del->next;
    else
    {
        // удаление первого элемента
        *start = del->next;
        if (*start) // если элементов больше одного
            (*start)->prior = NULL;
    }
    if (del->next)
        del->next->prior = del->prior;
    else // удаление последнего элемента списка
        *end = del->prior;
    delete del;
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////
///		методы класса StringToNumber вычисления операторов.
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void StringToNumber::UnMinus () // вычисление минуса числа, например -2
{
    if (IsNumberOrVariable (temp) && temp->prior) // если текущая лексема - число и слева есть лексема
        if (temp->prior->ThisIs == MINUS) // если слева лексема - оператор минус
            if (temp->prior->prior) // если перед минусом нет чисел или есть оператор MOD или DIV
            {
                if (!IsNumberOrVariable (temp->prior->prior) &&
                    temp->prior->prior->ThisIs != BRECKET_CLOSE ) // если здесь не вычитание
                {
                    goto unMinus;
                }
            }
            else // если число с минусом первое в списке, например -2 + 3
            {
                goto unMinus;
            }

    return;

unMinus:temp->number *= -1.; // поменять знак числа
        temp->Im_number *= -1.;
        temp->ThisIs = NUMBER;
        DelElementMyVersion (temp->prior, &start, &end); // удалить предыдущую лексему минус
        --countToken; // убрали минус, одной лексемой стало меньше
        temp = start;
}
//=============================================================
void StringToNumber::UnPlus ()
{
    if (IsNumberOrVariable (temp) && temp->prior) // аналогично плюс числа, например +2 + 4
        if (temp->prior->ThisIs == PLUS)
            if (temp->prior->prior)
            {
                if (!IsNumberOrVariable (temp->prior->prior) && temp->prior->prior->ThisIs != BRECKET_CLOSE  )
                {
                    goto unPlus;
                }
            }
            else
            {
                goto unPlus;
            }

    return;

unPlus:	temp->ThisIs = NUMBER;
        DelElementMyVersion (temp->prior, &start, &end); // удалить предыдущую лексему плюс
        --countToken;
        temp = start;
}
//=============================================================
void StringToNumber::Not	   ()
{
    if (IsNumberOrVariable (temp) && temp->prior) // если текущая лексема - число и слева есть лексема
        if (temp->prior->ThisIs == NOT)
        {
            DelElementMyVersion (temp->prior, &start, &end);
            --countToken;
            temp->Im_number = 0.;
            temp->number = (temp->number != 0.) ? 0. : 1.;
            temp->ThisIs = BOOL;
            temp = start;
        }
}
//=============================================================
void StringToNumber::Factorial ()
{
    if (temp->next) // если есть следующая лексема
        if (temp->next->ThisIs == FACTORIAL) // если эта лексема - оператор факториала
        {
            if (IsRealNumberOrVar (temp) || temp->ThisIs == BOOL) // если найдено число и справа есть лексема
            {
                if (temp->number < 0.)
                {
                    error = true;
                    rprin ("Нельзя вычислить факториал отрицательного числа!");
                    return;
                }
                temp->ThisIs = NUMBER;
                temp->number = fact (temp->number); // вычислить факториал
                DelElementMyVersion (temp->next, &start, &end); // удалить лексему факториал
                --countToken; // убрали !, одной лексемой стало меньше
                temp = start;
            }
            else
            {
                error = true;
                rprin ("Нельзя вычислить факториал комплексного числа!");
            }
        }
}
//=============================================================
void StringToNumber::DelBreckets (struct token *ptr)
{
    DelElementMyVersion (ptr->next, &start, &end);
    DelElementMyVersion (ptr->prior, &start, &end);
    countToken -= 2; // убрали две скобки
    temp = start;
}
//=============================================================
void StringToNumber::DelBrecketsAroundNumber () // удаляем скобки вокруг числа, например (2)
{
    if (IsNumberOrVariable (temp) && temp->next && temp->prior) // если найдено число и есть лексемы слева и справа
        if (temp->next->ThisIs  == BRECKET_CLOSE &&
            temp->prior->ThisIs == BRECKET_OPEN) // если лексема слева - открывабщаяся скобка, а лексема справа - закрывающаяся
            if (temp->prior->prior) // если есть вторая лексема слева от числа
            {
                if ( ! IsFunction (temp->prior->prior)) // если вторая лексема слева от числа не имя функции
                {
                    DelBreckets (temp);	// удалить скобки вокруг числа
                    temp = start;
                }
            }
            else
            {
                DelBreckets (temp); // удалить скобки если слева от открывабщейся нет лексем
                temp = start;
            }
}
//=============================================================
void StringToNumber::DelModulsAroundNumber () // аналогично удаляем модули вокруг числа, например |2| или |-2|
{
    if (temp->next &&
        temp->next->next) // если справа есть две лексемы
        if (temp->ThisIs == MODULE &&
            IsNumberOrVariable (temp->next) &&
            temp->next->next->ThisIs == MODULE) // если текущая лексема - модуль, следующая справа - число, вторая справа - модуль
                {
                    temp->ThisIs = NUMBER; // на место левого модуля вставляем абс. значение числа, меняя тип данных лексемы на числовой
                    temp_complex_number.Re = temp->next->number;
                    temp_complex_number.Im = temp->next->Im_number;
                    temp->number = temp_complex_number.module (temp_complex_number);
                    temp->Im_number = 0.;
                    temp->ThisIs = NUMBER;
                    Del_2_Elements	();
                    temp = start;
                }
}
//=============================================================
bool StringToNumber::IsSupremOperator (struct token *ptr)
{
    // возвратить истину если лексема является оператором наивысшего приоритета
    return  ptr->ThisIs == DEGREE	 ||
            ptr->ThisIs == MOD		 ||
            ptr->ThisIs == FACTORIAL ||
            ptr->ThisIs == DIV;
}
//=============================================================
bool StringToNumber::IsMiddleOperator (struct token *ptr)
{
    // возвратить истину если лексема является оператором среднего приоритета
    return  ptr->ThisIs == MULTIPLICATION ||
            ptr->ThisIs == DIVISION		  ||
            ptr->ThisIs == FRACTION;
}
//=============================================================
bool StringToNumber::IsUnOperator (struct token *ptr)
{
    return ptr->ThisIs == MINUS || ptr->ThisIs == PLUS || ptr->ThisIs == NOT;
}
//=============================================================
bool StringToNumber::OperatorSupremePrioritet (struct token *ptr)
{
    // возвратить истину если лексема является оператором высшего приоритета или функцией
    return IsSupremOperator (ptr) || IsFunction (ptr);
}
//=============================================================
bool StringToNumber::LeftOperatorSupremePrioritet ()
{
    // возвратить истину если лексема слева является оператором высшего приоритета или функцией
    return OperatorSupremePrioritet (temp->prior);
}
//=============================================================
bool StringToNumber::RightThirdOperatorSupremePrioritet ()
{
    // возвратить истину если третья справа лексема является оператором высшего приоритета или функцией
    return OperatorSupremePrioritet (temp->next->next->next);
}
//=============================================================
bool StringToNumber::RightFifthOperatorSupremePrioritet () // для дробей из трех чисел
{
    // возвратить истину если пятая справа лексема является оператором высшего приоритета или функцией
    return OperatorSupremePrioritet (temp->next->next->next->next->next);
}
//=============================================================
bool StringToNumber::LeftOperatorSupremePrioritet_2 ()
{
    // возвратить истину если лексема слева является оператором высшего или среднего приоритета
    return  IsSupremOperator (temp->prior) || IsMiddleOperator (temp->prior);
}
//=============================================================
bool StringToNumber::RightThirdOperatorSupremePrioritet_2 ()
{
    // возвратить истину если третья справа лексема является оператором высшего или среднего приоритета
    return  IsSupremOperator (temp->next->next->next) || IsMiddleOperator (temp->next->next->next);
}
//=============================================================
bool StringToNumber::RightSecondOperatorSupremePrioritet ()
{
    // возвратить истину если вторая справа лексема является оператором высшего приоритета
    return  IsSupremOperator (temp->next->next);
}
//=============================================================
bool StringToNumber::IsRealNumberOrVar (struct token *ptr)
{
    // возвратить истину если текущая лексема является вещественным числом или переменной
    return (ptr->ThisIs == NUMBER || ptr->ThisIs == VARIABLE || ptr->ThisIs == BOOL) && ptr->Im_number == 0.;
}
//=============================================================
bool StringToNumber::IsImNumberOrVar (struct token *ptr)
{
    // возвратить истину если текущая лексема является комплексным числом или переменной
    return (ptr->ThisIs == NUMBER || ptr->ThisIs == VARIABLE || ptr->ThisIs == BOOL) && ptr->Im_number != 0.;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////
///		описание методов класса StringToNumber работы с пользовательскими
///					 переменными.
///
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//===============================================================
// Метод используется при заполнении массива переменных.
// Возвращает истину, если строка str есть в списке имен функций/операторов/констант,
// т.е. не может быть именем переменной.
bool StringToNumber::EqualNamesUnique (char *str)
{
    int i = 0;
    for (; *ListNamesUnique[i]; ++i) // просмотреть список имен функций/операторов/констант
    {
        //if (i >= 27 && i <= 43)
        //{
            if (!strcmp (ListNamesUnique[i], str))
                return true;
        //}
        //else
            //if (!cmpstrnoregistr (ListNamesUnique[i], str)) // если не учитывая регистра строка str хоть с одним именем
                //return true;
    }
    return false;
}
//===============================================================
// Метод применяется в случае изменения переменной значения на Re_value и Im_value
// (инициализация, присваивание нового значения). На переменную ссылается
// указатель temp. Новое значение заносится в массив пользовательских
// переменных. Также все переменные в выражении с данным именем меняют свои значения.
void StringToNumber::SaveValue (double Re_value, double Im_value)
{
    ListVar[temp->IndexVar].ValueVar    = Re_value; // занести в массив переменыых новое значение переменной
    ListVar[temp->IndexVar].Im_ValueVar = Im_value;

    struct token *ptr = start; // указатель для навигации по списку лексем
// если в выражении несколько переменных с данным именем, то необходимо обновить их значения.
    while (ptr) // найти в списке все указанные переменные
    {
        if (ptr->ThisIs == VARIABLE && ptr->IndexVar == temp->IndexVar) // нужная переменная найдена
        {
            ptr->number    = Re_value; // обновить её значение
            ptr->Im_number = Im_value;
        }
        ptr = ptr->next; // перейти к следующей лексеме
    }
}
//===============================================================
// Метод вызывается при синтаксическом разборе выражения, когда с помощью метода EqualNamesUnique выяснилось
// что данная лексема является именем переменной.
void StringToNumber::AddVariableInList (char *add)
{
    int count = 0;
    for(; count < countVariables; ++count) // просмотреть список переменных
        if (!strcmp (ListVar[count].NameVar, add)) // если в списке есть переменная с именем add, то текущей лексеме присвоить значение существующей переменной, IndexVar ссылается на эту переменную
        {
            temp->number   = ListVar[count].ValueVar;
            temp->Im_number   = ListVar[count].Im_ValueVar;
            temp->IndexVar = count;
            return;
        }
        // иначе необходимо занести в список новую переменную:
        if (count == QUANTITY_VARIABLES)
        {
            rputs ("Количество переменных превысило допустимое значение!");
            error = true;
            return;
        }

        temp->IndexVar = count;					// записать индекс элемента списка переменных
        strcpy(ListVar[count].NameVar, add);	// записать в список переменных имя переменной
        countVariables = count;
        temp->ThisIs = VARIABLE;				// определить тип данных лексемы
        ListVar[count].ValueVar = 0;			// по умолчанию значение новой переменной - ноль
        ListVar[count].Im_ValueVar = 0;
        ++countVariables;						// одной переменной стало больше
        rprin("\rСоздана новая переменная ");
        rputs (add);
}
//===============================================================
// Метод вызывается, когда необходимо создать новую переменую и занести её в массив
// пользовательских переменных.
void StringToNumber::CreateVariable (char *add)
{
    int count = 0;
    for(; count < countVariables; ++count) // просмотреть список переменных
        if (!strcmp (ListVar[count].NameVar, add)) // если в списке есть переменная с именем add
        {
            return;
        }
        // иначе необходимо занести в список новую переменную:

        if (count == QUANTITY_VARIABLES)
        {
            rputs ("Количество переменных превысило допустимое значение!");
            error = true;
            return;
        }

        strcpy(ListVar[count].NameVar, add);	// записать в список переменных имя переменной
        countVariables = count;
        ListVar[count].ValueVar = 0;			// по умолчанию значение новой переменной - ноль
        ListVar[count].Im_ValueVar = 0;
        ++countVariables;						// одной переменной стало больше
}
//===============================================================
// Меняет значения переменной add на указаные в параметрах.
void StringToNumber::ChangeValueVariable (char *add, double Re_value, double Im_value)
{
    int count = 0;
    for(; count < countVariables; ++count) // просмотреть список переменных
        if (!strcmp (ListVar[count].NameVar, add)) // если в списке есть переменная с именем add, то текущей лексеме присвоить значение существующей переменной, IndexVar ссылается на эту переменную
        {
            ListVar[count].ValueVar = Re_value;
            ListVar[count].Im_ValueVar = Im_value;
            return;
        }
}
//===============================================================
// копирует массив пользовательских переменных в массив объекта t
void StringToNumber::CopyVariables (StringToNumber &t)
{
    for (int i = 0; i <= this->countVariables; i++)
    {
        t.ListVar[i] = this->ListVar[i];
    }
    t.countVariables = this->countVariables;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////
///		FillingListLexeme.h - описание метода класса StringToNumber создания
///							  двусвязного списка лексем, вызывается методом StrToNum,
///							  принимая его параметр.
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////


void StringToNumber::FillingListLexeme (char *str)
{

    char *p = str;						// указатель на строку-выражение
    int count_bytes = 0;
    bool begin = true;					// флаг определяет, первой ли по счету в список заносится лексема

    start = end = NULL;					// перед заполнением списка лексем обнулить указатели на начало и конец списка

    countToken = countString = 0;		// счетчик записанных в список лексем

    OLDcountVariables = countVariables; // сохранить прежнее количество переменных
    for (iteration = 0; iteration <= OLDcountVariables; ++iteration) // сохранить исходное состояние массива переменных на случай, если придется произвести откат
    {
        OLDListVar[iteration] = ListVar[iteration];
    }

    // Цикл заполнения списка лексем, т.е. синтаксического анализа строки и разделения её на отдельные лексемы
    for (iteration = 0; ; ++iteration) // счетчик итераций цикла
    {
        if (iteration == LIMITE_ERROR)  // если программа зациклилась из-за синтаксической ошибки, удалить список
                                        // из памяти и завершить работу функции. Данная ошибка никогда не возникнет
                                        // у пользователя, она возможна только у разработчика при добавлении новых
                                        // функций, операторов, т.е. ключевых слов.
        {
            rprin ("Синтаксическая ошибка: неизвестная команда!"); // вывести сообщение об ошибке
            DeleteAll (&start); // удалить список
            error = true;		// глобальная переменная-флаг определяет правильность работы функции
            return;// null;
        }

        SearchErrors (); // функция поиска синтаксических ошибок в формируемом списке лексем
        if (error) // если найдены синтаксические ошибки
        {
            DeleteAll (&start); // удалить список
            return;// null;
        }

get_lexeme:;
        if (!*p) // если достигли конца строки, завершить цикл
            break;

        while (isspace (*p) && *p++); // пропускать символы пробела между лексемами

        if (!*p) // если достигли конца строки, завершить цикл
            break;

        temp = new token; // после того как пропустили пробелы в строке, выделить память для нового элемента списка
        if (!temp) // если не удалось выделить память
            FatalError ("Ошибка выделения памяти."); // завершить приложение

        temp->number = temp->Im_number = 0.;

        ++countToken;			 // предполагается, что найдена именно лексема
//------------------------------------------------------------------------------
        // Определение и запись пользовательской переменной:
        if (isalpha (*p)	|| *p == '_') // если символ латинского  или русского алфавита или знака _
        {
            char box [SIZE_UNIQUE_WORD_AND_VARIABLES], // в этот массив скопируется имя предпологаемой переменной
                 *ptemp = p; // для проверки методом EqualNamesUnique, является ли её имя допустимым для переменной

            count_bytes = 0; // счетчик символов имени переменной
            while ( // пока не достигнем конца выражения или не закончатся буквы или _ или пока не достигнем конца
                    // массива box, копируем в массив box имя переменной
                    (isalpha (*ptemp)  || *ptemp == '_' || isdigit(*ptemp))
                    && *ptemp
                  ) // имя переменной может состоять из символов лат. и рус. алфавитов, цифр, знака _
            {
                if (count_bytes == SIZE_UNIQUE_WORD_AND_VARIABLES-1)
                {
                    rputs("Ошибка: слишком длинное имя!");
                    delete temp;
                    error = true;
                    DeleteAll (&start);
                    return;
                }
                box[count_bytes++] = *ptemp++;
            }
            box[count_bytes] = 0; // обозначить конец массива
            if (!EqualNamesUnique (box) && (
                !(*p != 'i' && !*(p+1)) || // если это не комплексное число i
                !(*p == 'i' && isdigit(*(p+1))))) // если это не комплексное число, например i3

                                                 // если найденное слово не совпадает ни с одним именем
                                                 // функций/операторов/констант без учета регистра, то записать
                                                 // в список новую переменную
            {
                AddVariableInList (box); // метод проверяет, имеется ли уже в списке данная переменная (с учетом регистра), если нет, переменная заносится в список,
                                         // ей присваивается нулевое значение, иначе текущей лексеме присваивается значение уже имеющейся переменной

                if (error) // если количество переменных превысило допустимое
                {
                    rputs("Ошибка: слишком много переменных!");
                    delete temp;
                    error = true;
                    DeleteAll (&start);
                    return;// null;
                }

                temp->ThisIs = VARIABLE;
                p = ptemp; // пропустить имя переменной
                goto add;  // добавление лексемы в список
            }
        } // if (isalpha_ (*p)	|| *p == '_')
//------------------------------------------------------------------------------
        switch (*p) // выяснить, является найденная лексема функцией, оператором или константой
        {
            case ';': // символ конца выражения
            delete temp; // освободить только что выделенную память
            --countToken;
            return; // перейти к циклу вычисления выражения
//------------------------------------------------------------------------------
            case '"': // строка, ограничена кавычками
                ++p;
                temp->ThisIs = STRING;
                temp->IndexString = countString++; // одной строкой стало больше. Все пользовательские строки хранятся в массиве strings
                if (countString > QUANTITY_STRINGS) // массив strings заполнен
                {
                    rputs ("Ошибка: слишком много строк в одном выражении!");
                    delete temp;
                    DeleteAll (&start);
                    error = true;
                    return;// null;
                }
                count_bytes = 0; // счетчик символов
                get_symbol: while (*p != '"') // запись в массив строк найденой строки
                {
                    if (!*p) // конец выражения?
                    {
                        rputs("Cинтаксическая ошибка: пропущен знак '\"'");
                        delete temp;
                        DeleteAll (&start); // удалить список
                        error = true;		// глобальная переменная-флаг определяет правильность работы функции
                        return;// null;
                    }
                    if (count_bytes == MAX_STRING-1)
                    {
                        rputs ("Ошибка: слишком длинная строка!");
                        delete temp;
                        DeleteAll (&start);
                        error = true;
                        return;// null;
                    }
                    strings [temp->IndexString][count_bytes++] = *p++; // запись строки в массив
                }
                if (*(p-1) == '\\') // сочетание символов \" означает, что строка еще не закончилась, а кавычки содержатся внутри строки
                {
                    count_bytes-=1;
                    strings [temp->IndexString][count_bytes++] = '"'; // убрать из строки косую черту перед кавычками
                    p++;
                    goto get_symbol; // записать следующий символ
                }
                strings [temp->IndexString][count_bytes] = 0; // обозначить конец строки
                p++;
            break;
//------------------------------------------------------------------------------
            case '=': // оператор присваивания
                ++p;
                temp->ThisIs = EQUAL;
                if (end)
                {
                    switch (end->ThisIs)
                    {
                    case EQUAL:
                        end->ThisIs = EGALE;
                        --countToken;
                        delete temp;
                        goto get_lexeme;
                    break;

                    case GREATER_THAN:
                        end->ThisIs = GREATER_OR_EQUAL;
                        --countToken;
                        delete temp;
                        goto get_lexeme;
                    break;

                    case LESS_THAN:
                        end->ThisIs = LESS_OR_EQUAL;
                        --countToken;
                        delete temp;
                        goto get_lexeme;
                    break;
                    }
                }
            break;
//------------------------------------------------------------------------------
            case ',': // разделитель аргументов в функциях
                ++p;  // перейти к следующему символу строки
                temp->ThisIs = COMMA;
            break;
//------------------------------------------------------------------------------
            case '.': // оператор точка, служит разделителем имени переменной и комплексной или действительной части значения переменной (как доступ к членам структуры в языке С++)
                ++p;
                temp->ThisIs = POINT;
            break;
//------------------------------------------------------------------------------
            case '+': // знак числа или оператор сложения
                ++p;
                temp->ThisIs = PLUS;
            break;
//------------------------------------------------------------------------------
            case '-': // знак числа или оператор вычитания
                ++p;
                temp->ThisIs = MINUS;
            break;
//------------------------------------------------------------------------------
            case '*': // оператор умножения
                ++p;
                temp->ThisIs = MULTIPLICATION;
            break;
//------------------------------------------------------------------------------
            case '#': // разделитель целой части, числителя и знаменателя в дроби
                ++p;
                temp->ThisIs = FRACTION;
            break;
//------------------------------------------------------------------------------
            case '/': // это знак деления или комментарии?
                ++p;  // перейти к следующей лексеме
                if (*p == '*') // если комментарии типа /* ... */
                {
                    --countToken; // комментарии в список не заносятся, зря выделили память и увеличили счетчик лексем
                    delete temp;  // освободить только что выделенную память
                    if (!(p = strstr (p+1, "*/"))) // если в строке не найден парный комментарий
                    {
                        rprin("Не хватает закрывающего комментария \"*/\" !");
                        DeleteAll (&start); // удалить список
                        error = true;
                        return;// null;
                    }
                    else // если найден закрывающий комментарий
                    {
                        p += 2;			 // пропустить в строке пару символов */, указатель перед ними поставило утверждение p = strstr (p+1, "*/")
                        goto get_lexeme; // перейти вверх к циклу пропуска пробелов в строке, найти новую лексему
                    }
                }
                if (*p == '/') // если комментарии типа //
                {
                    delete temp;			// освободить только что выделенную память
                    --countToken;
                    return;  // комментарии и всё что после них игнорируется, перейти к циклу вычисления выражения
                }
                temp->ThisIs = DIVISION; // остался один вариант: здесь имеется оператор деления
            break;
//------------------------------------------------------------------------------
            case '^': // оператор возведения в степень
                ++p;
                temp->ThisIs = DEGREE;
            break;
//------------------------------------------------------------------------------
            case '(': // открывающаяся скобка
            case '[':
            case '{':
                ++p;
                temp->ThisIs = BRECKET_OPEN;
            break;
//------------------------------------------------------------------------------
            case ')': // закрывающаяся скобка
            case ']':
            case '}':
                ++p;
                temp->ThisIs = BRECKET_CLOSE;
            break;
//------------------------------------------------------------------------------
            case '|': // оператор модуля числа
                ++p;
                temp->ThisIs = MODULE;
            break;
//------------------------------------------------------------------------------
            case '!': // оператор факториала числа
                ++p;
                temp->ThisIs = FACTORIAL;
            break;
//------------------------------------------------------------------------------
            case '>': // логический оператор больше
                ++p;
                temp->ThisIs = GREATER_THAN;
                if (end)
                    if (end->ThisIs == LESS_THAN)
                    {
                        delete temp;
                        --countToken;
                        end->ThisIs = NE_EGALE;
                        goto get_lexeme;
                    }
            break;
//------------------------------------------------------------------------------
            case '<': // логический оператор меньше
                ++p;
                temp->ThisIs = LESS_THAN;
            break;
//------------------------------------------------------------------------------
            case 'a':
            case 'A':
                if (tolower (*(1+p)) == 'n' && tolower (*(2+p)) == 's') // регистр символов не учитывать, метка ans - результат предыдущего вычисления
                {
                    temp->ThisIs = NUMBER;
                    temp->number = answer.Re;  // предыдущее значение вычисления берется из глобальной переменной
                    temp->Im_number = answer.Im;
                    p += 3;					// в слове ans три буквы, поэтому пропускаем их
                }
                else
                    if (tolower (*(1+p)) == 'b' && tolower (*(2+p)) == 's') // функция abs
                    {
                        temp->ThisIs = ABSVALUE;
                        p += 3;
                    }
                    else
                        if (tolower (*(1+p)) == 'c' && tolower (*(2+p)) == 'o' && tolower (*(3+p)) == 's') // функция acos
                        {
                            temp->ThisIs = ACOS;
                            p += 4;
                        }
                        else
                            if (tolower (*(1+p)) == 's' && tolower (*(2+p)) == 'i' && tolower (*(3+p)) == 'n') // функция asin
                            {
                                temp->ThisIs = ASIN;
                                p += 4;
                            }
                            else
                                if (tolower (*(1+p)) == 't' && tolower (*(2+p)) == 'g') // функция atg
                                {
                                    temp->ThisIs = ATAN;
                                    p += 3;
                                }
                                else
                                    if (tolower (*(1+p)) == 'c' && tolower (*(2+p)) == 't' && tolower (*(3+p)) == 'g') // функция actg
                                    {
                                        temp->ThisIs = ACTAN;
                                        p += 4;
                                    }
                                    else
                                        if (tolower (*(1+p)) == 'r' && tolower (*(2+p)) == 'g') // аргумент комплексного числа
                                        {
                                            temp->ThisIs = ARG;
                                            p += 3;
                                        }
                                        else
                                            if (tolower (*(1+p)) == 's' && tolower (*(2+p)) == 'h') // гиперболический арксинус
                                            {
                                                temp->ThisIs = ASH;
                                                p += 3;
                                            }
                                            else
                                                if (tolower (*(1+p)) == 'c' && tolower (*(2+p)) == 'h') // гиперболический арккосинус
                                                {
                                                    temp->ThisIs = ACH;
                                                    p += 3;
                                                }
                                                else
                                                    if (tolower (*(1+p)) == 't' && tolower (*(2+p)) == 'h') // гиперболический арктангенс
                                                    {
                                                        temp->ThisIs = ATH;
                                                        p += 3;
                                                    }
                                                    else
                                                        if (tolower (*(1+p)) == 'c' && tolower (*(2+p)) == 't' && tolower (*(3+p)) == 'h') // гиперболический арккотангенс
                                                        {
                                                            temp->ThisIs = ACTH;
                                                            p += 4;
                                                        }
                                                        else
                                                            if (tolower (*(1+p)) == 'n' && tolower (*(2+p)) == 'd') // логический оператор И
                                                            {
                                                                temp->ThisIs = AND;
                                                                p += 3;
                                                            }

            break;
//------------------------------------------------------------------------------
            case 'c':
            case 'C':
                if (tolower (*(1+p)) == 'o' && tolower (*(2+p)) == 's') // функция cos
                {
                    temp->ThisIs = COS;
                    p += 3;
                }
                else
                    if (tolower (*(1+p)) == 't' && tolower (*(2+p)) == 'g') // функция ctg
                    {
                        temp->ThisIs = CTG;
                        p += 3;
                    }

                        else
                            if (tolower (*(1+p)) == 'u' && tolower (*(2+p)) == 'b') // функция cub
                            {
                                temp->ThisIs = CUB;
                                p += 3;
                            }
                            else
                                if (tolower (*(1+p)) == 'b' && tolower (*(2+p)) == 'r' && tolower (*(3+p)) == 't') // функция cbrt
                                {
                                    temp->ThisIs = CBRT;
                                    p += 4;
                                }
                                else
                                    if (tolower (*(1+p)) == 'o' && tolower (*(2+p)) == 'n' && tolower (*(3+p)) == 'j') // функция комплесно-сопряженного conj
                                    {
                                        temp->ThisIs = CONJ;
                                        p += 4;
                                    }
                                    else
                                        if (tolower (*(1+p)) == 'h') // гиперболический косинус ch
                                        {
                                            temp->ThisIs = CH;
                                            p += 2;
                                        }
                                        else
                                            if (tolower (*(1+p)) == 't' && tolower (*(2+p)) == 'h') // гиперболический арктангенс ath
                                            {
                                                temp->ThisIs = CTH;
                                                p += 3;
                                            }
                                            else
                                                if (tolower (*(1+p)) == 's' && tolower (*(2+p)) == 'e' && tolower (*(3+p)) == 'c') // функция косеканс csec
                                                {
                                                    temp->ThisIs = CSEC;
                                                    p += 4;
                                                }
                                                else
                                                    if (*p == 'c') // константа с - скорость света, м/с
                                                    {
                                                        temp->ThisIs = NUMBER;
                                                        temp->number = k_c;
                                                        p++;
                                                    }
            break;
//------------------------------------------------------------------------------
            case 'd':
            case 'D':
                if (tolower (*(1+p)) == 'i' && tolower (*(2+p)) == 'v') // оператор div
                {
                    temp->ThisIs = DIV;
                    p += 3;
                }
            break;
//------------------------------------------------------------------------------
            case 'e':
            case 'E':
                if (tolower (*(1+p)) == 'x' && tolower (*(2+p)) == 'p') // функция exp
                {
                    temp->ThisIs = EXP;
                    p += 3;
                }
                else
                    if (*p == 'E' && (*(1+p)) == 'o') // константа "Эпсилон нулевое"
                    {
                        temp->ThisIs = NUMBER;
                        temp->number = k_e0;
                        p += 2;
                    }
                    else
                        if (*p == 'e') // константа е - заряд электрона
                        {
                            temp->ThisIs = NUMBER;
                            temp->number = k_e;
                            ++p;
                        }
            break;
//------------------------------------------------------------------------------
            case 'f':
            case 'F':
                if (tolower (*(1+p)) == 'a' && tolower (*(2+p)) == 'l' && tolower (*(3+p)) == 's' && tolower (*(4+p)) == 'e') // логическая константа false
                {
                    temp->ThisIs = BOOL;
                    p += 5;
                }
            break;
//------------------------------------------------------------------------------
            case 'G':
            case 'g':

                if (tolower (*(1+p)) == 'e' &&
                    tolower (*(2+p)) == 't' &&
                    tolower (*(3+p)) == 'r' &&
                    tolower (*(4+p)) == 'e') // функция getre
                {
                    temp->ThisIs = GETRE;
                    p += 5;
                }
                else
                    if (tolower (*(1+p)) == 'e' &&
                        tolower (*(2+p)) == 't' &&
                        tolower (*(3+p)) == 'i' &&
                        tolower (*(4+p)) == 'm') // функция getim
                    {
                        temp->ThisIs = GETIM;
                        p += 5;
                    }
                    else
                    if (*p == 'G')
                    {
                        temp->number = k_G; // гравитационная постоянная
                        temp->ThisIs = NUMBER;
                        ++p;
                    }
                    else
                        if (*p == 'g')// ускорение свободного падения
                        {
                            temp->number = k_g;
                            temp->ThisIs = NUMBER;
                            ++p;
                        }
            break;
//------------------------------------------------------------------------------
            case 'h':
            case 'H':
                if (tolower (*(1+p)) == 'y' && tolower (*(2+p)) == 'p' && tolower (*(3+p)) == 'o' && tolower (*(4+p)) == 't') // функция hypot
                {
                    temp->ThisIs = HYPOT;
                    p += 5;
                }
                else
                    if (*p == 'h') // постоянная Планка
                    {
                        temp->ThisIs = NUMBER;
                        temp->number = k_h;
                        ++p;
                    }
            break;
//------------------------------------------------------------------------------
            case 'I':
            case 'i':
                if (tolower (*(1+p)) == 'n' && tolower (*(2+p)) == 't' && tolower (*(3+p)) == 'e' && tolower (*(4+p)) == 'g' && tolower (*(5+p)) == 'r' && tolower (*(6+p)) == 'a' && tolower (*(7+p)) == 'l') // функция вычисления определенного интеграла Integral
                {
                    temp->ThisIs = INTEGRAL;
                    p += 8;
                }
                else
                    if (tolower (*p) == 'i' && tolower (*(1+p)) == 'm') // идентификатор комплексной части значения переменной
                    {
                        p += 2;
                        temp->ThisIs = MEMBER_STRUCT_IM;
                    }
                    if (*p == 'i')
                    {
                        ++p; // перейти к следующему символу
                        temp->Im_number = 1.;
                        temp->ThisIs = NUMBER;
                    }

            break;
//------------------------------------------------------------------------------
            case 'k': // постоянная Больцмана
                temp->number = k_k;
                temp->ThisIs = NUMBER;
                ++p;
            break;
//------------------------------------------------------------------------------
            case 'l':
            case 'L':
                if (tolower (*(1+p)) == 'n') // функция LN - натур. логарифм
                {
                    temp->ThisIs = LN;
                    p += 2;
                }
                else
                    if (tolower (*(1+p)) == 'g') // функция lg - десятичный логарифм
                    {
                        temp->ThisIs = LG;
                        p += 2;
                    }
                    else
                        if (tolower (*(1+p)) == 'o' && tolower (*(2+p)) == 'g') // функция log - логарифм по произвольному основанию
                        {
                            temp->ThisIs = LOG;
                            p += 3;
                        }
            break;
//------------------------------------------------------------------------------
            case 'm':
            case 'M':
                if (tolower (*(1+p)) == 'o' && tolower (*(2+p)) == 'd' && tolower (*(3+p)) == 'u' && tolower (*(4+p)) == 'l' && tolower (*(5+p)) == 'e')
                {
                    temp->ThisIs = COMPLEX_MODULE;
                    p += 6;
                }
                else
                if (tolower (*(1+p)) == 'o' && tolower (*(2+p)) == 'd') // оператор mod
                {
                    temp->ThisIs = MOD;
                    p += 3; // увеличить на число букв в названии функции
                }
                else
                    if (*p == 'm' && (*(1+p)) == 'p') // масса протона	mp
                    {
                        temp->number = k_mp;
                        temp->ThisIs = NUMBER;  // числовые костанты
                        p += 2;					// названия констант длиной 2 символа
                    }
                    else
                        if (*p == 'm' && (*(1+p)) == 'e') // масса электрона me
                        {
                            temp->number = k_me;
                            temp->ThisIs = NUMBER;
                            p += 2;
                        }
                        else
                            if (*p == 'm' && (*(1+p)) == 'n') // масса нейтрона mn
                            {
                                temp->number = k_mn;
                                temp->ThisIs = NUMBER;
                                p += 2;
                            }
                            else
                                if (*p == 'm' && (*(1+p)) == 'o') // константа mo - "мю нулевое"
                                {
                                    temp->number = k_m0;
                                    temp->ThisIs = NUMBER;
                                    p += 2;
                                }
                                else
                                    if (tolower (*(1+p)) == 'i' && tolower (*(2+p)) == 'd') // функция mid
                                    {
                                        temp->ThisIs = MIDDLE;
                                        p += 3;
                                    }
                                    else
                                        if (tolower (*(1+p)) == 'i' && tolower (*(2+p)) == 'n') // функция min
                                        {
                                            temp->ThisIs = MIN2;
                                            p += 3;
                                        }
                                        else
                                            if (tolower (*(1+p)) == 'a' && tolower (*(2+p)) == 'x') // функция max
                                            {
                                                temp->ThisIs = MAX2;
                                                p += 3;
                                            }

            break;
//------------------------------------------------------------------------------
            case 'N':
            case 'n':
                if (tolower (*(1+p)) == 'o' && tolower (*(2+p)) == 'd') // функция nod
                {
                    temp->ThisIs = NOD_DEL;
                    p += 3;
                }
                else
                    if (*p == 'N' && (*(1+p)) == 'a') // число Авогадро Na
                    {
                        temp->number = k_Na;
                        p += 2;
                        temp->ThisIs = NUMBER;
                    }
                    else
                        if (tolower (*(1+p)) == 'o' && tolower (*(2+p)) == 't') // оператор отрицания not
                        {
                            temp->ThisIs = NOT;
                            p += 3;
                        }
                        break;
//------------------------------------------------------------------------------
            case 'o':
            case 'O':
                if (tolower (*(1+p)) == 'r') // логический оператор or
                {
                    temp->ThisIs = OR;
                    p += 2;
                }
            break;
//------------------------------------------------------------------------------
            case 'P':
            case 'p':
                if (tolower (*(1+p)) == 'o' && tolower (*(2+p)) == 'w') // функция pow
                {
                    temp->ThisIs = POW;
                    p += 3;
                }
                else
                    if (*p == 'p' && (*(1+p)) == 'i') // константа pi = 3.14...
                    {
                        temp->number = M_PI;
                        p += 2;
                        temp->ThisIs = NUMBER;
                    }
            break;
//------------------------------------------------------------------------------
            case 'R':
            case 'r':
                if (tolower (*(1+p)) == 'o' && tolower (*(2+p)) == 'u' && tolower (*(3+p)) == 'n' && tolower (*(4+p)) == 'd') // функция round
                {
                    temp->ThisIs = ROUND;
                    p += 5;
                }
                else
                    if (tolower (*(1+p)) == 'a' && tolower (*(2+p)) == 'n' && tolower (*(3+p)) == 'd') // функция rand
                    {
                        temp->ThisIs = RAND;
                        p += 4;
                    }
                    else
                        if (tolower(*p) == 'r' && tolower(*(1+p)) == 'e') // идентификатор действительной части значения переменной
                        {
                            p += 2;
                            temp->ThisIs = MEMBER_STRUCT_RE;
                        }
                        else
                            if (*p == 'R') // универсальная газовая постоянная R
                            {
                                temp->ThisIs = NUMBER;
                                temp->number = k_R;
                                p++;
                            }
            break;
//------------------------------------------------------------------------------
            case 's':
            case 'S':
                if (tolower (*(1+p)) == 'i' && tolower (*(2+p)) == 'n') // функция sin
                {
                    temp->ThisIs = SIN;
                    p += 3;
                }
                else
                    if (tolower (*(1+p)) == 'q' && tolower (*(2+p)) == 'r' && tolower (*(3+p)) == 't') // функция sqrt
                    {
                        temp->ThisIs = SQRT;
                        p += 4;
                    }
                    else
                        if (tolower (*(1+p)) == 'q' && tolower (*(2+p)) == 'r') // функция sqr
                        {
                            temp->ThisIs = SQR;
                            p += 3;
                        }
                        else
                            if (tolower (*(1+p)) == 'h') // гиперболический синус sh
                            {
                                temp->ThisIs = SH_;
                                p += 2;
                            }
                            else
                                if (tolower (*(1+p)) == 'e' && tolower (*(2+p)) == 'c') // функция секанс sec
                                {
                                    temp->ThisIs = SEC;
                                    p += 3;
                                }
            break;
//------------------------------------------------------------------------------
            case 't':
            case 'T':
                if (tolower (*(1+p)) == 'g') // функция tg
                {
                    temp->ThisIs = TG;
                    p += 2;
                }
                else
                    if (tolower (*(p+1)) == 'h') // гиперболический тангенс th
                    {
                        temp->ThisIs = TH;
                        p += 2;
                    }
                    else
                        if (tolower (*(p+1)) == 'o' && tolower (*(p+2)) == 'r' && tolower (*(p+3)) == 'a' && tolower (*(p+4)) == 'd') // функция перевода в радианы torad
                        {
                            temp->ThisIs = TORAD;
                            p += 5;
                        }
                        else
                            if (tolower (*(p+1)) == 'o' && tolower (*(p+2)) == 'd' && tolower (*(p+3)) == 'e' && tolower (*(p+4)) == 'g') // функция перевода в градусы todeg
                            {
                                temp->ThisIs = TODEG;
                                p += 5;
                            }
                            else
                                if (tolower (*(p+1)) == 'r' && tolower (*(p+2)) == 'u' && tolower (*(p+3)) == 'e') // логическая константа true
                                {
                                    temp->ThisIs = BOOL;
                                    temp->number = 1.;
                                    p += 4;
                                }
            break;
//------------------------------------------------------------------------------
            case 'V':
                if (*(1+p) == 'm') // молярный объем газа при н.у.
                {
                    temp->number = k_Vm;
                    temp->ThisIs = NUMBER;
                }
                p += 2;
            break;
//------------------------------------------------------------------------------
            default: // число или неизвестная команда
                temp->ThisIs = NUMBER; // допустим, что число
                double temp_number;
                bool is_complex = false; // флаг определяет, комплексное это число или вещественное

                if (*p == 'i') // это мнимая единица?
                {
                    is_complex = true; // да, комплексное число
                    ++p; // перейти к следующему символу
                    temp->Im_number = 1.;
                    goto add;
                }
                if (!isdigit(*p))
                {
#ifdef WINDOWS
                    rprin("Неизвестная команда \"");
                    putchar(*p);
                    rprin("\" !");
#else
                    rprin("Неизвестная команда!");
#endif
                    DeleteAll (&start); // удалить список
                    error = true;
                    return;// null;
                }

                temp_number = atof (p); // точно, что число
                while (((isdigit (*p)) || *p == '.') && *p++); // пропустить число в строке и десятичную точку
                if (*p) // если не достигли конца строки
                {
                    if (*p == 'e'||*p == 'E') // если число записано в экспоненциальном виде
                    {
                        ++p; // пропустить экспоненту
                        if (*p)  // если не достигли конца строки
                            if (*p == '+' || *p == '-') // если достигли знака порядка числа после экспоненты, например 2е-3
                                p++; // пропустить знак
                        if (*p)  // если не достигли конца строки
                            while (isdigit (*p) && *p++); // пропустить пробелы в строке
                    }
                }
                if (*p == 'i') // если после числа стоит мнимая единица, например 2.5i
                {
                    is_complex = true;
                    ++p;
                }
                if (is_complex)
                    temp->Im_number = temp_number;
                else
                    temp->number = temp_number;
        } // switch (*p)
add:;
        AddInEndVersionShildt (temp, &end); // добавить найденную лексему в список лексем

        if (begin)			// если первая итерация цикла,
            start = temp;   // то инициализировать указатель начала списка
        begin = false;		// первая итерация цикла пройдена
    }
}


bool StringToNumber::IsFunction2Arg (token *ptr)
{
    return		ptr->ThisIs == HYPOT	||
                ptr->ThisIs == LOG		||
                ptr->ThisIs == RAND		||
                ptr->ThisIs == MIDDLE	||
                ptr->ThisIs == MAX2		||
                ptr->ThisIs == MIN2		||
                ptr->ThisIs == NOD_DEL	||
                ptr->ThisIs == POW;
}

bool StringToNumber::IsFunction3Arg (token *ptr)
{
    return ptr->ThisIs == INTEGRAL;
}

//=============================================================

complex StringToNumber::StrToNum (char *str)
// в параметрах передается строка содержащая арифметическое выражение.
// Возвращает комплексное число - результат вычислений.
{
    if (!*str) // если в функцию передано пустое выражение
    {
        //rprin ("\r\tПустое выражение\r");
        Write = false;
        ThisComment = true;
        return null;
    }

    ThisComment = error = Write = false; // предполагается, что выражение не состоит из только одного коммнтария и
                                         // что результат выражения не нужно выводить (на экран, в файл, ...).

    struct complex result;				// возвращаемый результат вычисления

    FillingListLexeme (str);			// создать список лексем
    if (error)
    {
        Undo (); // Откат
        return null;
    }
//-----------------------------------------------
//rprin ("\n%i - countToken\n", countToken); // отладочное утверждение

    // Цикл вычисления арифметического выражения. При циклическом просмотре списка лексем происходит последовательное упрощение
    // выражения путем удаления лишних скобок и заменой ряда совокупностей чисел и операторов/функций на результат их вычисления
    //при строгом соблюдении порядка приоритета операторов, таким образом список лексем шаг за шагом становится все короче, при
    // отсутствии синтаксических ошибок в выражении в списке лексем должно остаться один элемент - число, результат вычисления
    // выражения, т.е. выражение вырождается в одно число. При наличие синтаксических ошибок при достижении определенного числа
    // итераций цикла LIMITE_ERROR функция выводит сообщение об ошибке, удаляет список лексем из памяти, устанавливает в глобаль-
    // ную переменную-флаг error значение true и прекращает свою работу. То же самое происходит и при обнаружении арифметических
    // ошибок типа 3/0 или tg 90.

    if (countToken == 0) // список лексем может быть пустым в том случае, если выражение состоит только из одного комментария
    {
        //rprin ("\r\tПустое выражение\r");
        Write = false;
        ThisComment = true; // ни результат текущего выражения, ни выражение не нужно будет записывать в файл "Результат.txt"
        return null;
    }

    SearchBreckets (); // подсчитать, равно ли число открывающихся скобок числу закрывающихся
    if (error)
    {
        Undo (); // Откат
        return null;
    }

    for (temp = start; temp; temp = temp->next)
    {

        if (temp->ThisIs == POINT)
        {
            if (temp->prior && temp->next)
            {
                if (temp->prior->ThisIs != VARIABLE || !IsMemberStruct (temp->next))
                {
                    goto error_;
                }

                if (temp->next->next)
                    if (temp->next->next->ThisIs == EQUAL) // оператор присваивания
                    {
                        continue;
                    }

                temp = temp->prior; // temp указывает на имя переменной
                token *box;
                box = new token;
                if (!box) FatalError ("Ошибка выделения памяти");
                *box = *temp; // box хранит переменную
                ++countToken;
                box->prior = temp->next;
                box->next = temp->next->next;
                temp->next->next = box;
                box->next->prior = box;
                if (box->next->ThisIs == MEMBER_STRUCT_RE)
                {
                    temp->ThisIs = GETRE;
                }
                else
                {
                    temp->ThisIs = GETIM;
                }
                temp->next->ThisIs = BRECKET_OPEN;
                box->next->ThisIs = BRECKET_CLOSE;
            }
            else
            {
                goto error_;
            }
        }
    }
    iteration = 0;

    for (iteration = 0; ; ++iteration) // цикл вычисления значения выражения
    {
        anew: temp = start; // переход к началу списка лексем

        if (iteration == LIMITE_ERROR) // условие обнаружения синтаксических ошибок. Ошибка возникает, когда программа зацикливается
        {
error_:		rprin ("Синтаксическая ошибка!");
            Undo (); // Откат
            error = true;
            return null;
        }

        while (temp) // цикл просмотра списка
        {
loop:;
            if (countToken == 1) // условие окончания цикла вычисления, когда в списке осталась одна лексема
            {
                if (!IsNumberOrVariable(temp)) // если эта лексема не число и не переменная
                {
                    rprin ("Синтаксическая ошибка!");
                    error = true;
                    Undo (); // Откат
                    return null;
                }
                result.Re = start->number; // в переменную result записать ответ
                result.Im = start->Im_number;
                ThisBoolValue = (start->ThisIs == BOOL); // если ответ логический, установить флаг ThisBoolValue в true
                DeleteAll (&start);
                return result; // возвратить результат вычисления
            }

// --- Блок присваивания переменной значения

        if (!temp->prior && temp->next && temp->ThisIs == VARIABLE) // перечисление переменных a,d,b=45,h;
            if (temp->next->ThisIs == COMMA)
            {
                DelElementMyVersion (temp->next, &start, &end); // удалить запятую
                DelElementMyVersion (temp, &start, &end); // удалить переменную
                countToken -= 2;
                Write = false;
                ThisComment = true; // не печатать результат
                goto anew; // переход к началу списка лексем
            }

        if (temp->next && temp->next->next && (!temp->next->next->next)) // множественное или одиночное присваивание, например a=b=v=5 или a=5;
            if (temp->ThisIs == VARIABLE &&
                temp->next->ThisIs == EQUAL &&
                IsNumberOrVariable (temp->next->next))
            {
                SaveValue (temp->next->next->number, temp->next->next->Im_number); // сохранить новое значение в переменной
                Del_2_Elements (); // удалить из списка две следующие лексемы
                if (!temp->prior) // если слева нет лексем, т.е. в списке осталась одна лексема
                {
                    DeleteAll (&start);
                    Write = false;
                    ThisComment = true; // ни результат текущего выражения, ни выражение не нужно будет записывать в файл "Результат.txt"
                    return null;
                }
                goto anew; // переход к началу списка лексем
            }

        if (temp->next && temp->next->next && temp->next->next->next) // присваивание в cкобках (а=5), в т.ч. множественное
            if (temp->ThisIs == VARIABLE &&
                temp->next->ThisIs == EQUAL &&
                IsNumberOrVariable (temp->next->next) &&
                temp->next->next->next->ThisIs == BRECKET_CLOSE)
            {
                SaveValue (temp->next->next->number, temp->next->next->Im_number);
                Del_2_Elements ();
                goto anew;
            }

        if (temp->next && temp->next->next && temp->next->next->next) // присваивание через запятую a=3, b=a или просто объявление переменных;
            if (temp->ThisIs == VARIABLE &&
                temp->next->ThisIs == EQUAL &&
                IsNumberOrVariable (temp->next->next) &&
                temp->next->next->next->ThisIs == COMMA)
            {
                SaveValue (temp->next->next->number, temp->next->next->Im_number);
                Del_2_Elements ();
                ThisComment = true;
                goto anew;
            }

// --- Блок инициализации комплексной или действительной части переменной

        if (temp->next &&
            temp->next->next &&
            temp->next->next->next &&
            temp->next->next->next->next &&
            (!temp->next->next->next->next->next)) // множественное или одиночное присваивание, например a=b=v=5 или a=5;
            if (temp->ThisIs == VARIABLE &&
                temp->next->ThisIs == POINT &&
                IsMemberStruct (temp->next->next) &&
                temp->next->next->next->ThisIs == EQUAL &&
                IsNumberOrVariable (temp->next->next->next->next))
            {
                if (temp->next->next->ThisIs == MEMBER_STRUCT_RE)
                {
                    SaveValue (temp->next->next->next->next->number, temp->Im_number);
                }
                else
                {
                    SaveValue (temp->number, temp->next->next->next->next->Im_number);
                }

                Del_2_Elements (); // удалить из списка две следующие лексемы
                Del_2_Elements ();

                if (!temp->prior) // если слева нет лексем, т.е. в списке осталась одна лексема
                {
                    DeleteAll (&start);
                    Write = false;
                    ThisComment = true; // ни результат текущего выражения, ни выражение не нужно будет записывать в файл "Результат.txt"
                    return null;
                }
                goto anew; // переход к началу списка лексем
            }

        if (temp->next &&
            temp->next->next &&
            temp->next->next->next &&
            temp->next->next->next->next &&
            temp->next->next->next->next->next) // присваивание в cкобках (а=5), в т.ч. множественное
            if (temp->ThisIs == VARIABLE &&
                temp->next->ThisIs == POINT &&
                IsMemberStruct (temp->next->next) &&
                temp->next->next->next->ThisIs == EQUAL &&
                IsNumberOrVariable (temp->next->next->next->next) &&
                temp->next->next->next->next->next->ThisIs == BRECKET_CLOSE)
            {
                if (temp->next->next->ThisIs == MEMBER_STRUCT_RE)
                {
                    SaveValue (temp->next->next->next->next->number, temp->Im_number);
                }
                else
                {
                    SaveValue (temp->number, temp->next->next->next->next->Im_number);
                }
                Del_2_Elements ();
                Del_2_Elements ();
                goto anew;
            }

        if (temp->next &&
            temp->next->next &&
            temp->next->next->next &&
            temp->next->next->next->next &&
            temp->next->next->next->next->next) // присваивание через запятую a=3, b=a или просто объявление переменных;
            if (temp->ThisIs == VARIABLE &&
                temp->next->ThisIs == POINT &&
                IsMemberStruct (temp->next->next) &&
                temp->next->next->next->ThisIs == EQUAL &&
                IsNumberOrVariable (temp->next->next->next->next) &&
                temp->next->next->next->next->next->ThisIs == COMMA)
            {
                if (temp->next->next->ThisIs == MEMBER_STRUCT_RE)
                {
                    SaveValue (temp->next->next->next->next->number, temp->Im_number);
                }
                else
                {
                    SaveValue (temp->number, temp->next->next->next->next->Im_number);
                }
                Del_2_Elements ();
                Del_2_Elements ();
                ThisComment = true;
                goto anew;
            }
// --- Блок вычисления операторов наивысшего приоритета.

            UnMinus ();	// вычислить минус числа --2 -> 2
            UnPlus  ();	// вычислить плюс числа: +2+3 -> 2+3
            Not		(); // оператор логического отрицания

            Factorial               ();	// вычислить факториал
            DelBrecketsAroundNumber (); // удалить скобки вокруг числа, панример (2) -> 2
            DelModulsAroundNumber   ();	// найти модуль числа, например |-2| = 2

// --- Блок вычисления функций-операторов. Для всех функций единый алгоритм вычисления.

            Functions_Operators_Of1Arguments ();
            if (error) // если возникла ошибка
            {
                Undo (); // Откат
                return null;
            }

// --- Блок вычисления функций от одного аргумента.

            Functions_Of1Arguments ();
            if (error)
            {
                Undo (); // Откат
                return null;
            }

// --- Блок вычисления функций от двух аргументов.

            Functions_Of2Arguments ();
            if (error)
            {
                Undo (); // Откат
                return null;
            }

// --- Блок вычисления функций от трех аргументов.

            Functions_Of3Arguments ();
            if (error)
            {
                Undo (); // Откат
                return null;
            }

//---- Блок вычисления арифметических операторов.

            complex a, b, c;
            if (IsNumberOrVariable(temp) && temp->next && temp->next->next) // если найдено число и после числа есть две лексемы
                    if (IsNumberOrVariable(temp->next->next)) // если вторая лексема после числа также число
                        switch (temp->next->ThisIs) // узнать, какой оператор между числами
                        {
                            case MOD: // остаток по модулю

                                if (IsRealNumberOrVar (temp) && IsRealNumberOrVar (temp->next->next))
                                {
                                    if (!(int)temp->next->next->number)
                                    {
                                        rprin ("Вы попытались поделить на 0 !");
                                        Undo (); // Откат
                                        error = true;
                                        return null;
                                    }
                                    temp->number = ABS (((int)temp->number) % ((int)temp->next->next->number)); // вычислить остаток
                                    temp->ThisIs = NUMBER;
                                    Del_2_Elements	();
                                    goto anew; // начать сначала цикл вычисления выражения:
                                }
                                if (IsImNumberOrVar (temp) || IsImNumberOrVar (temp->next->next))
                                {
                                    rprin ("Оператор MOD можно использовать только с вещественными числами!");
                                    Undo (); // Откат
                                    error = true;
                                    return null;
                                }
                                goto anew;
//----------------------------------------------------------------------------------------------------------------------
                            case DIV: // целая часть от деления по модулю

                                if (IsRealNumberOrVar (temp) && IsRealNumberOrVar (temp->next->next))
                                {
                                    if (!(int)temp->next->next->number)
                                    {
                                        rprin("Вы попытались поделить на 0 !");
                                        error = true;
                                        Undo (); // Откат
                                        return null;
                                    }
                                    temp->number = (int)(ABS (((int)temp->number) / ((int)temp->next->next->number)));
                                    temp->ThisIs = NUMBER;
                                    Del_2_Elements	();
                                    goto anew;
                                }
                                if (IsImNumberOrVar (temp) || IsImNumberOrVar (temp->next->next))
                                {
                                    rprin ("Оператор DIV можно использовать только с вещественными числами!");
                                    Undo (); // Откат
                                    error = true;
                                    return null;
                                }
                                goto anew;
//----------------------------------------------------------------------------------------------------------------------
                            case DEGREE: // оператор степени

                                a.Re = temp->number;
                                a.Im = temp->Im_number;
                                b.Re = temp->next->next->number;
                                b.Im = temp->next->next->Im_number;
                                temp_complex_number.pow_ (a, b, &temp_complex_number);
                                temp->number = temp_complex_number.Re;
                                temp->Im_number = temp_complex_number.Im;
                                temp->ThisIs = NUMBER;
                                Del_2_Elements	();
                            goto anew;
//----------------------------------------------------------------------------------------------------------------------
                            case MULTIPLICATION: // умножение

                                    if (temp->prior)						 // если слева от числа есть лексема
                                        if (LeftOperatorSupremePrioritet ()) // и если приоритет этой лексемы выше. например слева от четверки: 2^4*3
                                            break;						 // не вычислять умножение

                                    if (temp->next->next->next)						// если справа от числа есть третья лексема
                                        if (RightThirdOperatorSupremePrioritet ())  // и если эта лесема имеет высший приоритет
                                                break;								// не вычислять умножение

                                    b.Re = temp->next->next->number;
                                    b.Im = temp->next->next->Im_number;
                                    a.Re = temp->number;
                                    a.Im = temp->Im_number;
                                    a.multiplication (a, b, &c);
                                    temp->number = c.Re;
                                    temp->Im_number = c.Im;
                                    temp->ThisIs = NUMBER;
                                    Del_2_Elements	();
                            goto anew;
//----------------------------------------------------------------------------------------------------------------------
                            case DIVISION: // аналогично что и умножение

                                if (temp->prior)
                                    if (LeftOperatorSupremePrioritet ())
                                        break;

                                if (temp->next->next->next)
                                    if (RightThirdOperatorSupremePrioritet ())
                                            break;
                                b.Re = temp->next->next->number;
                                b.Im = temp->next->next->Im_number;
                                a.Re = temp->number;
                                a.Im = temp->Im_number;
                                a.division (a, b, &c);
                                if (error)
                                {
                                    Undo (); // Откат
                                    return null;
                                }
                                temp->number = c.Re;
                                temp->Im_number = c.Im;
                                temp->ThisIs = NUMBER;
                                Del_2_Elements	();
                            goto anew;
//----------------------------------------------------------------------------------------------------------------------
                            case FRACTION: // вычисление дроби вида 3#2 или 1#3#2. У оператора дробь одинаковый приоритет, что и умножения

                                if (temp->prior)						 // если слева от числа есть лексема
                                    if (LeftOperatorSupremePrioritet ()) // и если приоритет этой лексемы выше. например слева от четверки: 2^4#3
                                        break;							 // не вычислять дробь

                                // выяснить, из трех ли чисел состоит дробь
                                if (temp->next->next->next)															// если есть третья лексема справа
                                    if (temp->next->next->next->ThisIs == FRACTION && temp->next->next->next->next) // если есть третья лексема справа является оператором дроби и есть четвертая лексема
                                        if (IsNumberOrVariable(temp->next->next->next->next))							// если четвертая лексема справа является числом
                                        {
                                            if (temp->next->next->next->next->next)									// если есть пятая справа лексема
                                            {
                                                if (!RightFifthOperatorSupremePrioritet ()) // если приоритет пятой справа лексемы не выше, то вычислить дробь
                                                    goto frac;
                                                else
                                                {
                                                    temp = temp->next;
                                                    continue;
                                                }
                                            }
                                            else // дробь - последняя в списке
                                                goto frac;
                                        }
                                        if (temp->next->next)
                                        {

                                            if (temp->next->next->next)
                                            if (temp->next->next->next->next)
                                            if (temp->next->next->next->next->ThisIs == BRECKET_OPEN ||
                                                IsUnOperator (temp->next->next->next->next))
                                            {
                                                temp = temp->next;
                                                continue;
                                            }
                                        }
                                // остался один вариант - дробь из двух чисел
                                if (temp->next->next->next) // проверяется если например справа от четверки: 2 # 4^3 - степень высшего приоритета, поэтому нельэя вычислять 2#2, или например exp 2^2 # 2: сначала вычисляется 2^2, затем ехр 1, затем (ехр 1)#2
                                    if (RightThirdOperatorSupremePrioritet ()) // если лексема после второго числа более высокого приоритета
                                        break; // также не вычислять дробь

                                // дробь типа a#b = a/b
                                    //rputs("odin");
                                if (temp->next->next->next)
                                    if (temp->next->next->next->ThisIs == FRACTION)
                                    {
                                        temp = temp->next->next->next;
                                        goto loop;
                                    }
                                a.Re = temp->number;
                                a.Im = temp->Im_number;
                                b.Re = temp->next->next->number;
                                b.Im = temp->next->next->Im_number;
                                a.division (a, b, &c);
                                if (error)
                                {
                                    Undo (); // Откат
                                    return null;
                                }
                                temp->number = c.Re;
                                temp->Im_number = c.Im;
                                temp->ThisIs = NUMBER;
                                Del_2_Elements	();
                                goto anew;

                                 // a#b#c = a + b/c
    frac:						 // значение дроби равно сумме целой части и отношения числителя и знаменателя дроби
                                a.Re = temp->number;
                                a.Im = temp->Im_number;
                                b.Re = temp->next->next->number;
                                b.Im = temp->next->next->Im_number;
                                c.Re = temp->next->next->next->next->number;
                                c.Im = temp->next->next->next->next->Im_number;
                                temp_complex_number.division (b, c, &temp_complex_number);
                                if (error)
                                {
                                    Undo (); // Откат
                                    return null;
                                }
                                temp->number = a.Re + temp_complex_number.Re;
                                temp->Im_number = a.Im + temp_complex_number.Im;
                                temp->ThisIs = NUMBER;
                                Del_2_Elements	(); // убрать два оператора дроби и числитель и знаменатель, осталось одно значение дроби на месте целой части
                                Del_2_Elements	();
                                goto anew;
//----------------------------------------------------------------------------------------------------------------------
                            case PLUS: // сложение

                                    if (!temp->prior && !temp->next->next->next) // если в списке простое выражение 2+2 и других лексем больше нет
                                        goto plus; // сложить числа

                                    if (temp->prior && temp->next->next->next) // если выражение в скобках, например (2+2)
                                        if (
                                                (temp->prior->ThisIs == BRECKET_OPEN			 ||
                                                 temp->prior->ThisIs == MODULE					 ||
                                                 temp->prior->ThisIs == COMMA					 ||
                                                 temp->prior->ThisIs == EQUAL)
                                                                                                 &&

                                                (temp->next->next->next->ThisIs == BRECKET_CLOSE ||
                                                 temp->next->next->next->ThisIs == MODULE		 ||
                                                 temp->next->next->next->ThisIs == COMMA)
                                            )
                                            goto plus;

                                    if (temp->prior) // если слева оператор более высокого приоритета, например слева от тройки: 2*3 + 4
                                        if (LeftOperatorSupremePrioritet_2 ())
                                            break;

                                    if (temp->next->next->next) // случай 2 + 3*4
                                        if (RightThirdOperatorSupremePrioritet_2 ())
                                            break;

                                    plus: temp->number += temp->next->next->number;
                                          temp->Im_number += temp->next->next->Im_number;
                                          temp->ThisIs = NUMBER;
                                    Del_2_Elements	();
                            goto anew;
//----------------------------------------------------------------------------------------------------------------------
                            case MINUS: // аналогично что и сложение

                                    if (!temp->prior && !temp->next->next->next)
                                        goto minus;

                                    if (temp->prior && temp->next->next->next)
                                        if (
                                                (temp->prior->ThisIs == BRECKET_OPEN			 ||
                                                 temp->prior->ThisIs == MODULE					 ||
                                                 temp->prior->ThisIs == COMMA)

                                                                                                 &&

                                                (temp->next->next->next->ThisIs == BRECKET_CLOSE ||
                                                 temp->next->next->next->ThisIs == MODULE		 ||
                                                 temp->next->next->next->ThisIs == COMMA)
                                            )
                                            goto minus;

                                    if (temp->prior)
                                        if (LeftOperatorSupremePrioritet_2 ())
                                            break;

                                    if (temp->next->next->next)
                                        if (RightThirdOperatorSupremePrioritet_2 ())
                                            break;

                                    minus: temp->number -= temp->next->next->number;
                                           temp->Im_number -= temp->next->next->Im_number;
                                            temp->ThisIs = NUMBER;
                                    Del_2_Elements	();
                            goto anew;
//----------------------------------------------------------------------------------------------------------------------
                            case AND: // логическое И

                                    if (!temp->prior && !temp->next->next->next)
                                        goto and_met;

                                    if (temp->prior && temp->next->next->next)
                                        if (
                                                (temp->prior->ThisIs == BRECKET_OPEN			 ||
                                                 temp->prior->ThisIs == MODULE					 ||
                                                 temp->prior->ThisIs == COMMA)

                                                                                                 &&

                                                (temp->next->next->next->ThisIs == BRECKET_CLOSE ||
                                                 temp->next->next->next->ThisIs == MODULE		 ||
                                                 temp->next->next->next->ThisIs == COMMA)
                                            )
                                            goto and_met;

                                    if (temp->prior)
                                        if (LeftOperatorSupremePrioritet_2 () ||
                                            temp->prior->ThisIs == PLUS ||
                                            temp->prior->ThisIs == MINUS ||
                                            temp->prior->ThisIs == EGALE ||
                                            temp->prior->ThisIs == NE_EGALE ||
                                            temp->prior->ThisIs == GREATER_THAN ||
                                            temp->prior->ThisIs == LESS_THAN ||
                                            temp->prior->ThisIs == GREATER_OR_EQUAL ||
                                            temp->prior->ThisIs == LESS_OR_EQUAL)
                                            break;

                                    if (temp->next->next->next)
                                        if (RightThirdOperatorSupremePrioritet_2 () ||
                                            temp->next->next->next->ThisIs == PLUS ||
                                            temp->next->next->next->ThisIs == MINUS ||
                                            temp->next->next->next->ThisIs == EGALE ||
                                            temp->next->next->next->ThisIs == NE_EGALE ||
                                            temp->next->next->next->ThisIs == GREATER_THAN ||
                                            temp->next->next->next->ThisIs == LESS_THAN ||
                                            temp->next->next->next->ThisIs == GREATER_OR_EQUAL ||
                                            temp->next->next->next->ThisIs == LESS_OR_EQUAL )
                                            break;

                                    and_met:  temp->number = (temp->number != 0. && temp->next->next->number != 0.);
                                    temp->Im_number = 0.;
                                    temp->ThisIs = BOOL;
                                    Del_2_Elements	();
                            goto anew;
//----------------------------------------------------------------------------------------------------------------------
                            case OR: // логическое ИЛИ

                                    if (!temp->prior && !temp->next->next->next)
                                        goto or_met;

                                    if (temp->prior && temp->next->next->next)
                                        if (
                                                (temp->prior->ThisIs == BRECKET_OPEN			 ||
                                                 temp->prior->ThisIs == MODULE					 ||
                                                 temp->prior->ThisIs == COMMA)

                                                                                                 &&

                                                (temp->next->next->next->ThisIs == BRECKET_CLOSE ||
                                                 temp->next->next->next->ThisIs == MODULE		 ||
                                                 temp->next->next->next->ThisIs == COMMA)
                                            )
                                            goto or_met;

                                    if (temp->prior)
                                        if (LeftOperatorSupremePrioritet_2 () ||
                                            temp->prior->ThisIs == PLUS ||
                                            temp->prior->ThisIs == MINUS ||
                                            temp->prior->ThisIs == EGALE ||
                                            temp->prior->ThisIs == NE_EGALE ||
                                            temp->prior->ThisIs == GREATER_THAN ||
                                            temp->prior->ThisIs == LESS_THAN ||
                                            temp->prior->ThisIs == GREATER_OR_EQUAL ||
                                            temp->prior->ThisIs == LESS_OR_EQUAL ||
                                            temp->prior->ThisIs == AND)
                                            break;

                                    if (temp->next->next->next)
                                        if (RightThirdOperatorSupremePrioritet_2 () ||
                                            temp->next->next->next->ThisIs == PLUS ||
                                            temp->next->next->next->ThisIs == MINUS ||
                                            temp->next->next->next->ThisIs == EGALE ||
                                            temp->next->next->next->ThisIs == NE_EGALE ||
                                            temp->next->next->next->ThisIs == GREATER_THAN ||
                                            temp->next->next->next->ThisIs == LESS_THAN ||
                                            temp->next->next->next->ThisIs == GREATER_OR_EQUAL ||
                                            temp->next->next->next->ThisIs == LESS_OR_EQUAL ||
                                            temp->next->next->next->ThisIs == AND)
                                            break;

                                    or_met:  temp->number = (temp->number != 0. || temp->next->next->number != 0.);
                                    temp->Im_number = 0.;
                                    temp->ThisIs = BOOL;
                                    Del_2_Elements	();
                            goto anew;
//----------------------------------------------------------------------------------------------------------------------
                            case EGALE: // сравнение на равенство

                                    if (!temp->prior && !temp->next->next->next)
                                        goto egale;

                                    if (temp->prior && temp->next->next->next)
                                        if (
                                                (temp->prior->ThisIs == BRECKET_OPEN			 ||
                                                 temp->prior->ThisIs == MODULE					 ||
                                                 temp->prior->ThisIs == COMMA)

                                                                                                 &&

                                                (temp->next->next->next->ThisIs == BRECKET_CLOSE ||
                                                 temp->next->next->next->ThisIs == MODULE		 ||
                                                 temp->next->next->next->ThisIs == COMMA)
                                            )
                                            goto egale;

                                    if (temp->prior)
                                        if (LeftOperatorSupremePrioritet_2 () ||
                                            temp->prior->ThisIs == PLUS ||
                                            temp->prior->ThisIs == MINUS)
                                            break;

                                    if (temp->next->next->next)
                                        if (RightThirdOperatorSupremePrioritet_2 () ||
                                            temp->next->next->next->ThisIs == PLUS ||
                                            temp->next->next->next->ThisIs == MINUS )
                                            break;

                                    egale:  if (temp->Im_number == 0. && temp->next->next->Im_number == 0.)
                                                temp->number = (temp->number == temp->next->next->number) ? 1. : 0.;
                                            else
                                                temp->number = (temp->number == temp->next->next->number && temp->Im_number == temp->next->next->Im_number);
                                    temp->Im_number = 0.;
                                    temp->ThisIs = BOOL;
                                    Del_2_Elements	();
                            goto anew;
//----------------------------------------------------------------------------------------------------------------------
                            case NE_EGALE: // сравнение на неравенство

                                    if (!temp->prior && !temp->next->next->next)
                                        goto ne_egale;

                                    if (temp->prior && temp->next->next->next)
                                        if (
                                                (temp->prior->ThisIs == BRECKET_OPEN			 ||
                                                 temp->prior->ThisIs == MODULE					 ||
                                                 temp->prior->ThisIs == COMMA)

                                                                                                 &&

                                                (temp->next->next->next->ThisIs == BRECKET_CLOSE ||
                                                 temp->next->next->next->ThisIs == MODULE		 ||
                                                 temp->next->next->next->ThisIs == COMMA)
                                            )
                                            goto ne_egale;

                                    if (temp->prior)
                                        if (LeftOperatorSupremePrioritet_2 () ||
                                            temp->prior->ThisIs == PLUS ||
                                            temp->prior->ThisIs == MINUS)
                                            break;

                                    if (temp->next->next->next)
                                        if (RightThirdOperatorSupremePrioritet_2 () ||
                                            temp->next->next->next->ThisIs == PLUS ||
                                            temp->next->next->next->ThisIs == MINUS )
                                            break;

                                    ne_egale: if (temp->Im_number == 0. && temp->next->next->Im_number == 0.)
                                                temp->number = (temp->number != temp->next->next->number) ? 1. : 0.;
                                            else
                                                temp->number = (temp->number != temp->next->next->number && temp->Im_number != temp->next->next->Im_number);
                                    temp->Im_number = 0.;
                                    temp->ThisIs = BOOL;
                                    Del_2_Elements	();
                            goto anew;
//----------------------------------------------------------------------------------------------------------------------
                                    case GREATER_THAN: // больше

                                    if (!temp->prior && !temp->next->next->next)
                                        goto greater_than;

                                    if (temp->prior && temp->next->next->next)
                                        if (
                                                (temp->prior->ThisIs == BRECKET_OPEN			 ||
                                                 temp->prior->ThisIs == MODULE					 ||
                                                 temp->prior->ThisIs == COMMA)

                                                                                                 &&

                                                (temp->next->next->next->ThisIs == BRECKET_CLOSE ||
                                                 temp->next->next->next->ThisIs == MODULE		 ||
                                                 temp->next->next->next->ThisIs == COMMA)
                                            )
                                            goto greater_than;

                                    if (temp->prior)
                                        if (LeftOperatorSupremePrioritet_2 () ||
                                            temp->prior->ThisIs == PLUS ||
                                            temp->prior->ThisIs == MINUS)
                                            break;

                                    if (temp->next->next->next)
                                        if (RightThirdOperatorSupremePrioritet_2 () ||
                                            temp->next->next->next->ThisIs == PLUS ||
                                            temp->next->next->next->ThisIs == MINUS )
                                            break;

                                    greater_than:  temp->number = (temp->number > temp->next->next->number);
                                    temp->Im_number = 0.;
                                    temp->ThisIs = BOOL;
                                    Del_2_Elements	();
                            goto anew;
//----------------------------------------------------------------------------------------------------------------------
                                    case LESS_THAN: // меньше

                                    if (!temp->prior && !temp->next->next->next)
                                        goto less_than;

                                    if (temp->prior && temp->next->next->next)
                                        if (
                                                (temp->prior->ThisIs == BRECKET_OPEN			 ||
                                                 temp->prior->ThisIs == MODULE					 ||
                                                 temp->prior->ThisIs == COMMA)

                                                                                                 &&

                                                (temp->next->next->next->ThisIs == BRECKET_CLOSE ||
                                                 temp->next->next->next->ThisIs == MODULE		 ||
                                                 temp->next->next->next->ThisIs == COMMA)
                                            )
                                            goto less_than;

                                    if (temp->prior)
                                        if (LeftOperatorSupremePrioritet_2 () ||
                                            temp->prior->ThisIs == PLUS ||
                                            temp->prior->ThisIs == MINUS)
                                            break;

                                    if (temp->next->next->next)
                                        if (RightThirdOperatorSupremePrioritet_2 () ||
                                            temp->next->next->next->ThisIs == PLUS ||
                                            temp->next->next->next->ThisIs == MINUS )
                                            break;

                                    less_than:  temp->number = (temp->number < temp->next->next->number);
                                    temp->Im_number = 0.;
                                    temp->ThisIs = BOOL;
                                    Del_2_Elements	();
                            goto anew;
//----------------------------------------------------------------------------------------------------------------------
                                    case GREATER_OR_EQUAL: // больше или равно

                                    if (!temp->prior && !temp->next->next->next)
                                        goto greater_or_equal;

                                    if (temp->prior && temp->next->next->next)
                                        if (
                                                (temp->prior->ThisIs == BRECKET_OPEN			 ||
                                                 temp->prior->ThisIs == MODULE					 ||
                                                 temp->prior->ThisIs == COMMA)

                                                                                                 &&

                                                (temp->next->next->next->ThisIs == BRECKET_CLOSE ||
                                                 temp->next->next->next->ThisIs == MODULE		 ||
                                                 temp->next->next->next->ThisIs == COMMA)
                                            )
                                            goto greater_or_equal;

                                    if (temp->prior)
                                        if (LeftOperatorSupremePrioritet_2 () ||
                                            temp->prior->ThisIs == PLUS ||
                                            temp->prior->ThisIs == MINUS)
                                            break;

                                    if (temp->next->next->next)
                                        if (RightThirdOperatorSupremePrioritet_2 () ||
                                            temp->next->next->next->ThisIs == PLUS ||
                                            temp->next->next->next->ThisIs == MINUS )
                                            break;

                                    greater_or_equal:  temp->number = (temp->number >= temp->next->next->number);
                                    temp->Im_number = 0.;
                                    temp->ThisIs = BOOL;
                                    Del_2_Elements	();
                            goto anew;
//----------------------------------------------------------------------------------------------------------------------
                                    case LESS_OR_EQUAL: // меньше или равно

                                    if (!temp->prior && !temp->next->next->next)
                                        goto less_or_equal;

                                    if (temp->prior && temp->next->next->next)
                                        if (
                                                (temp->prior->ThisIs == BRECKET_OPEN			 ||
                                                 temp->prior->ThisIs == MODULE					 ||
                                                 temp->prior->ThisIs == COMMA)

                                                                                                 &&

                                                (temp->next->next->next->ThisIs == BRECKET_CLOSE ||
                                                 temp->next->next->next->ThisIs == MODULE		 ||
                                                 temp->next->next->next->ThisIs == COMMA)
                                            )
                                            goto less_or_equal;

                                    if (temp->prior)
                                        if (LeftOperatorSupremePrioritet_2 () ||
                                            temp->prior->ThisIs == PLUS ||
                                            temp->prior->ThisIs == MINUS)
                                            break;

                                    if (temp->next->next->next)
                                        if (RightThirdOperatorSupremePrioritet_2 () ||
                                            temp->next->next->next->ThisIs == PLUS ||
                                            temp->next->next->next->ThisIs == MINUS )
                                            break;

                                    less_or_equal:  temp->number = (temp->number <= temp->next->next->number);
                                    temp->Im_number = 0.;
                                    temp->ThisIs = BOOL;
                                    Del_2_Elements	();
                            goto anew;

//----------------------------------------------------------------------------------------------------------------------
                            case EQUAL: // оператор присваивания, скобки и запятые пропустить
                                if (temp->next->next->next &&
                                    temp->next->next->next->ThisIs == EQUAL)
                                { // a = b = c
                                    temp=temp->next;
                                    continue;
                                }
                                if (temp->next->next->next &&
                                    temp->next->next->next->ThisIs == COMMA)
                                {
                                    temp = start;
                                     continue;
                                }
                                 //break;
                            case COMMA:
                            case BRECKET_OPEN:
                            case BRECKET_CLOSE:
                                temp = temp->next;
                                continue;
//----------------------------------------------------------------------------------------------------------------------
                            default: // некорректное выражение, например, например, три идущих подряд числа
                                rprin ("Синтаксическая ошибка: пропущен оператор!");
                                error = true;
                                Undo (); // Откат
                            return null;
                    } // switch
            temp = temp->next; // переход к следующей лексеме
        } // цикл просмотра списка
    } // цикл вычисления
} // главный метод

//=============================================================

void StringToNumber::Undo ()
{
    countVariables = OLDcountVariables; // удальть новые переменные
    DeleteAll (&start); // удалить список
    for (int i = 0; i <= OLDcountVariables; ++i) // сохранить исходное состояние массива переменных
    {
        ListVar[i] = OLDListVar[i];
    }
}


bool StringToNumber::IsFunction (token *ptr) // лексема является функцией?
{
    // возвратить истину если лексема является функцией или функцией-оператором
    return 	IsFunction1Arg (temp) || IsFunction2Arg (temp) || IsFunction3Arg (temp);
}



bool StringToNumber::IsFunction1Arg (token *ptr)
{
    return 		ptr->ThisIs == EXP				||
                ptr->ThisIs == LN				||
                ptr->ThisIs == SIN				||
                ptr->ThisIs == COS				||
                ptr->ThisIs == TG				||
                ptr->ThisIs == CTG				||
                ptr->ThisIs == ABSVALUE			||
                ptr->ThisIs == ASIN				||
                ptr->ThisIs == ACOS				||
                ptr->ThisIs == ATAN				||
                ptr->ThisIs == ACTAN			||
                ptr->ThisIs == SQRT				||
                ptr->ThisIs == SQR				||
                ptr->ThisIs == CUB				||
                ptr->ThisIs == CBRT				||
                ptr->ThisIs == ROUND			||
                ptr->ThisIs == LG				||
                ptr->ThisIs == ARG				||
                ptr->ThisIs == COMPLEX_MODULE	||
                ptr->ThisIs == CONJ				||
                ptr->ThisIs == SH_				||
                ptr->ThisIs == CH				||
                ptr->ThisIs == TH				||
                ptr->ThisIs == ASH				||
                ptr->ThisIs == ACH				||
                ptr->ThisIs == ATH				||
                ptr->ThisIs == CTH				||
                ptr->ThisIs == ACTH				||
                ptr->ThisIs == SEC				||
                ptr->ThisIs == CSEC				||
                ptr->ThisIs == TORAD			||
                ptr->ThisIs == TODEG			||
                ptr->ThisIs == GETRE			||
                ptr->ThisIs == GETIM;
}

//===============================================================================================================
// МЕТОДЫ ВЫЧИСЛЕНИЯ ФУНКЦИЙ ОТ ОДНОГО АРГУМЕНТА:

void StringToNumber::DelBrecketsAroundArgumentsOfFunction1Argument ()
{
    if (temp->next &&
        temp->next->next &&
        temp->next->next->next &&
        temp->next->next->next->next) // если справа от текущей лексемы есть 4 лексемы

        if (temp->ThisIs == BRECKET_OPEN &&
            temp->next->ThisIs == BRECKET_OPEN &&
            IsNumberOrVariable (temp->next->next) &&
            temp->next->next->next->ThisIs == BRECKET_CLOSE &&
            temp->next->next->next->next->ThisIs == BRECKET_CLOSE)
            {
                DelElementMyVersion (temp->next, &start, &end);
                DelElementMyVersion (temp->next->next, &start, &end);
                countToken -= 2;
                temp = start; // перейти к началу списка лексем
            }
}

void StringToNumber::Functions_Of1Arguments ()
{
    if (temp->next &&
        temp->next->next &&
        temp->next->next->next) // если справа от текущей лексемы есть три лексемы
        if (temp->next->ThisIs == BRECKET_OPEN &&
            IsNumberOrVariable(temp->next->next) &&
            temp->next->next->next->ThisIs == BRECKET_CLOSE) // и если справа от текущей первая лексема - открывающаяся скобка, вторая - число, третья - закрывающаяся скобка
                Function_Of1Argument(); // выяснить, является ли текущая лексема именем функции, и если нелбходимо, выполнить её
}

void StringToNumber::RunFunction1Arg (token *ptr_arg, int n)
{
        temp_complex_number.Re = ptr_arg->number;
        temp_complex_number.Im = ptr_arg->Im_number;
        switch (temp->ThisIs)
        {
        case EXP:			temp_complex_number.exp_				(&temp_complex_number); break; // экспонента
        case ABSVALUE:		temp_complex_number.complex_module		(&temp_complex_number); break; // модуль
        case LN:			temp_complex_number.ln_					(&temp_complex_number); break; // натуральный логарифм
        case SIN:			temp_complex_number.sin_				(&temp_complex_number); break; // синус
        case COS:			temp_complex_number.cos_				(&temp_complex_number); break; // косинус
        case TG:			if (ptr_arg->number && !(ptr_arg->number / 90. - (int)ptr_arg->number / 90) && ptr_arg->Im_number == 0.)
                            {
                                rprin ("Тангенса +/-90 градусов не существует!");
                                error = true;
                                return;
                            }
                            temp_complex_number.tg_					(&temp_complex_number); break; // тангенс
        case CTG:			if ((ptr_arg->number == 0. || !(ptr_arg->number / 180. - (int)ptr_arg->number / 180)) && ptr_arg->Im_number == 0.)
                            {
                                rprin ("Koтангенса +/-180 или 0 градусов не существует!");
                                error = true;
                                return;
                            }
                                temp_complex_number.ctg_			(&temp_complex_number); break; // котангенс
        case ACOS:				temp_complex_number.acos_			(&temp_complex_number); break; // арккосинус
        case ASIN:				temp_complex_number.asin_			(&temp_complex_number); break; // арксинус
        case ATAN:			if (ptr_arg->number == 0. && ((ptr_arg->Im_number) == 1. || ptr_arg->Im_number == -1.))
                            {
                                error = true;
                                rprin ("Atg(+/-i) не существует!");
                                return;
                            }
                                temp_complex_number.atg_			(&temp_complex_number); break; // арктангенс

        case ACTAN:			if (ptr_arg->number == 0. && ((ptr_arg->Im_number) == 1. || ptr_arg->Im_number == -1.))
                            {
                                error = true;
                                rprin ("Actg(+/-i) не существует!");
                                return;
                            }
                                temp_complex_number.actg_			(&temp_complex_number); break; // арккотангенс
        case SQRT:				temp_complex_number.sqrt_			(&temp_complex_number); break; // кв. корень
        case SQR:				temp_complex_number.sqr_			(&temp_complex_number); break; // квадрат
        case CUB:				temp_complex_number.cub_			(&temp_complex_number); break; // куб
        case CBRT:				temp_complex_number.cbrt_			(&temp_complex_number); break; // куб. корень
        case ROUND:				temp_complex_number.round_			(&temp_complex_number); break; // округление
        case LG:				temp_complex_number.log10_			(&temp_complex_number); break; // десятичный логарифм
        case ARG:				temp_complex_number.complex_arg		(&temp_complex_number); break; //
        case COMPLEX_MODULE:	temp_complex_number.complex_module	(&temp_complex_number); break; //
        case CONJ:				temp_complex_number.conj_			(&temp_complex_number); break; //
        case SH_:				temp_complex_number.sh_				(&temp_complex_number); break;
        case CH:				temp_complex_number.ch_				(&temp_complex_number); break;
        case TH:				temp_complex_number.th_				(&temp_complex_number); break;
        case CTH:				temp_complex_number.cth_			(&temp_complex_number); break;
        case ASH:				temp_complex_number.ash_			(&temp_complex_number); break;
        case ACH:				temp_complex_number.ach_			(&temp_complex_number); break;
        case ATH:				temp_complex_number.ath_			(&temp_complex_number); break;
        case ACTH:				temp_complex_number.acth_			(&temp_complex_number); break;
        case SEC:				temp_complex_number.sec_			(&temp_complex_number); break;
        case CSEC:				temp_complex_number.csec_			(&temp_complex_number); break;
        case TORAD:				temp_complex_number.torad_			(&temp_complex_number); break;
        case TODEG:				temp_complex_number.todeg_			(&temp_complex_number); break;
        case GETRE:				temp_complex_number.getre_			(&temp_complex_number); break;
        case GETIM:				temp_complex_number.getim_			(&temp_complex_number); break;
        }
        if (!error)
        {
            temp->number = temp_complex_number.Re;
            temp->Im_number = temp_complex_number.Im;
            temp->ThisIs = NUMBER;  // переопределить тип данных элемента лексемы EXP, на который указывает temp, в численный
            countToken -=n;			// теперь лексем в списке стало на n меньше
            for (int i=0; i < n; i++, DelElementMyVersion (temp->next, &start, &end));

            temp = start;
        }
}
// Пример для функций-операторов: ДО ВЫЗОВА ФУНКЦИИ
//		лексема а:		лексема b:		лексема с:
//
//		ЕХР				NUMBER			...
//		<нет записи>	1				...
//		next -> b		next -> с		next -> ...
//		... <- prior	a <- prior		b <- prior
//
//		 ПОСЛЕ ВЫЗОВА ФУНКЦИИ
//		лексема а:		лексема c:	(лексема b удалена)
//
//		NUMBER			...
//		2.712
//		next -> c		next -> ...
//		... <- prior	a <- prior}
// Для всех функций единый алгоритм.

void StringToNumber::Function_Of1Argument ()
{
    DelBrecketsAroundArgumentsOfFunction1Argument ();
    if (IsFunction1Arg (temp))
        RunFunction1Arg (temp->next->next, 3);
}

void StringToNumber::Functions_Operators_Of1Arguments () // вызывает целый ряд методов для определения и вычисления конкретной функции-оператора
{
    if (temp->next) // если есть справа лексема
        if (IsNumberOrVariable (temp->next)) // и если эта лексема - число
            if (temp->next->next) // если после числа есть лексема
            {
                if (!RightSecondOperatorSupremePrioritet ()) // если второй справа оператор имеет меньший приоритет, например ехр 2*2 - эквивалентно (ехр 2)*2
                    Function_OperatorOf1Argument (); // если temp - функция, вычислить функцию от числа
            }
            else
                Function_OperatorOf1Argument (); // если после числа нет лексем, точно необходимо вычислить функцию
}

void StringToNumber::Function_OperatorOf1Argument ()
{
    if (IsFunction1Arg (temp))
    {
        RunFunction1Arg (temp->next, 1);
    }
}

//===============================================================================================================
// МЕТОДЫ ВЫЧИСЛЕНИЯ ФУНКЦИЙ ОТ ДВУХ АРГУМЕНТОВ:

bool StringToNumber::IsFunctionOf2Arguments () // Лексема является функцией от двух аргументов?
{
    if (temp->next &&
        temp->next->next &&
        temp->next->next->next &&
        temp->next->next->next->next &&
        temp->next->next->next->next->next)
        if (temp->next->ThisIs == BRECKET_OPEN &&
            IsNumberOrVariable (temp->next->next) &&
            temp->next->next->next->ThisIs == COMMA &&
            IsNumberOrVariable (temp->next->next->next->next) &&
            temp->next->next->next->next->next->ThisIs == BRECKET_CLOSE)
            return true;
    return false;
}

void StringToNumber::Functions_Of2Arguments () // вызывает целый ряд методов для определения и вычисления конкретной функции.
{
    if (IsFunction2Arg (temp)) // если текущая лексема - имя функции
        if (IsFunctionOf2Arguments ()) // и если эта функция от двух аргументов, список её аргументов всегда в скобках, аргументы отделяются оператором запятая
        {
            switch (temp->ThisIs) // в зависимости от имени функции выбрать нужную функцию вычисления
            {
                case HYPOT:		hypot	();	break;
                case LOG:		log_	();	break;
                case RAND:		rand_	();	break;
                case MIDDLE:	middle	();	break;
                case MAX2:		max_	();	break;
                case MIN2:		min_	();	break;
                case NOD_DEL:	nod		();	break;
                case POW:		pow_	();	break;
            }
            temp->ThisIs = NUMBER; // в лексему имени функции записан результат вычислений, тип хранимых данных поменять на числовой
            DelElementMyVersion (temp->next, &start, &end); // удалить из списка открывающуюся скобку
            DelElementMyVersion (temp->next, &start, &end); // удалить из списка первый аргумент
            DelElementMyVersion (temp->next, &start, &end); // удалить из списка запятую-разделитель аргументов
            DelElementMyVersion (temp->next, &start, &end); // удалить из списка второй аргумент
            DelElementMyVersion (temp->next, &start, &end); // удалить из списка закрывающуюся скобку
            countToken -= 5; // пятью лексемами стало меньше
            temp = start; // перейти к началу списка лексем
        }
}

void StringToNumber::hypot ()
{
    complex compl2;
    temp_complex_number.Re = temp->next->next->number;
    temp_complex_number.Im = temp->next->next->Im_number;
    compl2.Re = temp->next->next->next->next->number;
    compl2.Im = temp->next->next->next->next->Im_number;
    temp_complex_number.hypot_ (&temp_complex_number, &compl2);
    temp->number = temp_complex_number.Re;
    temp->Im_number = temp_complex_number.Im;
}

void StringToNumber::log_ ()
{
    if (temp->next->next->next->next->number == 1. && temp->next->next->next->next->Im_number == 0.) // основание логарифма рано единице - абсурдно! Единица в любой степени равна единице.
    {
        rprin ("Основание логарифма не должно быть равным единице!");
        error = true;
        return;
    }
// логарифм по произвольному основанию равен отношению натуральных логарифмов аргумента и основания*/
    complex compl2;
    temp_complex_number.Re = temp->next->next->number;
    temp_complex_number.Im = temp->next->next->Im_number;
    compl2.Re = temp->next->next->next->next->number;
    compl2.Im = temp->next->next->next->next->Im_number;
    temp_complex_number.log_ (&temp_complex_number, &compl2);
    temp->number = temp_complex_number.Re;
    temp->Im_number = temp_complex_number.Im;
}

void StringToNumber::rand_ ()
{
    if (temp->next->next->Im_number != 0. || temp->next->next->next->next->Im_number != 0.)
    {
        error = true;
        rprin ("Функция rand генерирует только вещественные числа!");
    }
    temp->number = temp->next->next->number +
                  (temp->next->next->next->next->number - temp->next->next->number) / RAND_MAX * rand ();
}

void StringToNumber::middle ()
{
    temp->number = (temp->next->next->number + temp->next->next->next->next->number) / 2.;
    temp->Im_number = (temp->next->next->Im_number + temp->next->next->next->next->Im_number) / 2.;
}

void StringToNumber::max_ ()
{
    if (temp->next->next->Im_number != 0. || temp->next->next->next->next->Im_number != 0.)
    {
        error = true;
        rprin ("Нахождение наибольшего из комплексных чисел не имеет смысла!");
        return;
    }
    temp->number = MAX (temp->next->next->number,
                   temp->next->next->next->next->number);
}

void StringToNumber::min_ ()
{
    if (temp->next->next->Im_number != 0. || temp->next->next->next->next->Im_number != 0.)
    {
        error = true;
        rprin ("Нахождение наименьшего из комплексных чисел не имеет смысла!");
        return;
    }
    temp->number = MIN (temp->next->next->number, temp->next->next->next->next->number);
}

void StringToNumber::nod ()
{
    if (temp->next->next->Im_number != 0. || temp->next->next->next->next->Im_number != 0.)
    {
        error = true;
        rprin ("Нахождение НОД комплексных чисел не имеет смысла!");
        return;
    }
    temp->number = NOD (temp->next->next->number, temp->next->next->next->next->number);
}

void StringToNumber::pow_ ()
{
    complex compl2, res;
    temp_complex_number.Re = temp->next->next->number;
    temp_complex_number.Im = temp->next->next->Im_number;
    compl2.Re = temp->next->next->next->next->number;
    compl2.Im = temp->next->next->next->next->Im_number;
    temp_complex_number.pow_ (temp_complex_number, compl2, &res);
    temp->number = res.Re;
    temp->Im_number = res.Im;
}

//===============================================================================================================
// МЕТОДЫ ВЫЧИСЛЕНИЯ ФУНКЦИЙ ОТ ТРЕХ АРГУМЕНТОВ:

bool StringToNumber::IsFunctionOf3Arguments () // Лексема является функцией вычисления интеграла?
{
    if (temp->next &&
        temp->next->next &&
        temp->next->next->next &&
        temp->next->next->next->next &&
        temp->next->next->next->next->next &&
        temp->next->next->next->next->next->next &&
        temp->next->next->next->next->next->next->next)
        if (temp->next->ThisIs == BRECKET_OPEN &&
             (temp->next->next->ThisIs == STRING) && // в строке описана подинтегральная функция
            temp->next->next->next->ThisIs == COMMA &&
            IsNumberOrVariable (temp->next->next->next->next) &&
            temp->next->next->next->next->next->ThisIs == COMMA &&
            IsNumberOrVariable (temp->next->next->next->next->next->next) &&
            temp->next->next->next->next->next->next->next->ThisIs == BRECKET_CLOSE)
        {
            return true;
        }
    return false;
}

void StringToNumber::Functions_Of3Arguments () // вызывает целый ряд методов для определения и вычисления конкретной функции.
{
    if (IsFunction3Arg (temp)) // если текущая лексема - имя функции
        if (IsFunctionOf3Arguments ()) // и если эта функция от двух аргументов, список её аргументов всегда в скобках, аргументы отделяются оператором запятая
        {
            switch (temp->ThisIs) // в зависимости от имени функции выбрать нужную функцию вычисления
            {
                case INTEGRAL:		integral	();	break;
            }
            temp->ThisIs = NUMBER; // в лексему имени функции записан результат вычислений, тип хранимых данных поменять на числовой
            DelElementMyVersion (temp->next, &start, &end); // удалить из списка открывающуюся скобку
            DelElementMyVersion (temp->next, &start, &end); // удалить из списка первый аргумент
            DelElementMyVersion (temp->next, &start, &end); // удалить из списка запятую-разделитель аргументов
            DelElementMyVersion (temp->next, &start, &end); // удалить из списка второй аргумент
            DelElementMyVersion (temp->next, &start, &end); // удалить из списка закрывающуюся скобку
            DelElementMyVersion (temp->next, &start, &end); // удалить из списка закрывающуюся скобку
            DelElementMyVersion (temp->next, &start, &end); // удалить из списка закрывающуюся скобку
            countToken -= 7; // пятью лексемами стало меньше
            temp = start; // перейти к началу списка лексем
        }
}

// Вычисление определенного интеграла методом Симпсона.
void StringToNumber::integral ()
{
    StringToNumber Integral; // объект для разбора строки с подинтегральной функцией

    double a = temp->next->next->next->next->number, // нижний предел интеграла
           b = temp->next->next->next->next->next->next->number; // верхний предел

    this->CopyVariables (Integral); // передать в объект Integral все переменные текущего объекта
    Integral.CreateVariable ("x"); // создать переменную x. Подинтегральную функцию интегрируем по x.

    const double e = 1e-5; // точность вычисления интеграла
    double s = 0.0, w, x, h, n = 10.0;

    // на цикл вычисления интеграла отводится строго определенное время, поэтому необходимо
    // засечь момент начала интегрирования.
    time_t start = time (NULL), // начало засекли
           end; // время окончания интегрирования

    do // цикл вычисления интеграла
    {
        w = s; h = (b-a)/n;
        complex current;

        for (s = 0.0, x = a; x < b - h; x += 2.0*h)
        {
            Integral.ChangeValueVariable ("x", x, 0.); // изменить значение переменной
            current = Integral.StrToNum(strings[temp->next->next->IndexString]);
            if (error || ThisComment || Write)
                return;

            s += current.Re;

            // если цикл вычисления интеграла занимает слишком много времени:
            if (difftime (end=time(NULL), start) == TIME_CALCULATION_INTEGRALE)
            {
                rputs ("Не удалось точно вычислить интеграл, ваш компьютер не рассчитан на такие \nгромоздкие вычисления. Попробуйте уменьшить интервал интегрирования и/или \nупростить подинтегральную функцию.");
                goto end_calculation; // завершить интегрирование
            }

            Integral.ChangeValueVariable ("x", x+h,0.);
            current = Integral.StrToNum(strings[temp->next->next->IndexString]);
            if (error || ThisComment)
                return;

            s += 4.*current.Re;

            Integral.ChangeValueVariable ("x", x+2.*h,0.);
            current = Integral.StrToNum(strings[temp->next->next->IndexString]);
            if (error || ThisComment)
                return;

            s += current.Re;
        }
        s *= h/3.0;
        n *= 2.0;
    }
    while (fabs (s-w) > e);

end_calculation: int oldQuantityVar = this->countVariables;
    Integral.CopyVariables (*this);
    this->countVariables = oldQuantityVar;
    temp->number = s;
    temp->Im_number = 0.;
}

//===============================================================================================================
// методы поиска синтаксических ошибок в арифметическом выражении

void StringToNumber::SearchBreckets ()
{
    struct token *temp;
    temp = start;
    int CountOpenBreckets = 0, CountCloseBreckets = 0; // счетчики открывающихся и закрывающихся скобок
    while (temp) // просмотреть список лексем с начала
    {
        if (temp->ThisIs == BRECKET_OPEN) // если лексема - открывающаяся скобка
            ++CountOpenBreckets; // подсчет количества скобок
        else
            if (temp->ThisIs == BRECKET_CLOSE)
                ++CountCloseBreckets;

        temp = temp->next; // перейти к следующей лексеме
    }

    if (CountOpenBreckets != CountCloseBreckets) // если скобки несбалансированы
    {
        if (CountOpenBreckets > CountCloseBreckets) // открывающихся скобок больше закрывающихся
        {
            rprin ("Не хватает ");
            printf ("%i", CountOpenBreckets - CountCloseBreckets); // сколько не хватает нужных скобок
            rprin (" закрывающ(ейся)(ихся) скоб(ки)(ок)!");
        }
        else
        {
            rprin ("Не хватает ");
            printf ("%i", -CountOpenBreckets + CountCloseBreckets);
            rprin (" открывающ(ейся)(ихся) скоб(ки)(ок)!");
        }
        error = true;
    }

    CountOpenBreckets = 0; // теперь проверить, сколько знаков модуля | в выражении
    temp = start;
    while (temp) // снова просмотреть список с начала
    {
        if (temp->ThisIs == MODULE)
            ++CountOpenBreckets; // подсчитать количество знаков модуля
        temp = temp->next; // перейти к следующей лексеме
    }
    if (CountOpenBreckets % 2) // если число модулей нечетно (т.к. |-3| = 3 - всегда четно)
    {
        rprin ("Неправильно расставлены модули числа!");
        error = true;
    }
}

void StringToNumber::SearchErrors ()
{
    struct token *temp;
    temp = start;
    while (temp) // просмотреть список, по очереди удалив все элементы
    {
        if (temp->next && temp->ThisIs == VARIABLE)
            if (temp->next->ThisIs == BRECKET_OPEN)
            {
                rprin ("Синтаксическая ошибка");
                error = true;
                return;
            }
        if (temp->ThisIs == NUMBER || temp->ThisIs == BOOL)
            if (temp->next) // справа от числа есть лексема
            {
                switch (temp->next->ThisIs) // что за лексема?
                {
                case BRECKET_OPEN:
                    rprin ("Между числом '");
                    WriteComplexNumberInFile (temp->number, temp->Im_number, stdout);
                    rprin ("' и '(' нет оператора!");
                    error = true;
                    break;
                case NUMBER:
                case BOOL:
                    rprin ("Между числами '");
                    WriteComplexNumberInFile (temp->number, temp->Im_number, stdout);
                    rprin ("' и '");
                    WriteComplexNumberInFile (temp->next->number, temp->next->Im_number, stdout);
                    rprin ("' нет оператора!");
                    error = true;
                    break;
                case EQUAL:
                    if ((temp->next->next))
                    {
                        if (temp->next->next->ThisIs != EQUAL)
                        {
                            rprin ("Оператор '=' инициализирует только переменные! '==' - оператор сравнения.");
                            error = true;
                        }
                        else error = false;
                    }
                    break;
                }

                if (IsFunction1Arg (temp->next) || IsFunction2Arg (temp->next))
                {
                    rprin ("Пропущен оператор между числом '");
                    WriteComplexNumberInFile (temp->number, temp->Im_number, stdout);
                    rprin ("' и функцией!");
                    error = true;
                }
                if (temp->prior)
                {
                    if (temp->prior->ThisIs == BRECKET_CLOSE)
                    {
                        rprin ("Между ')' и числом '");
                        WriteComplexNumberInFile (temp->number, temp->Im_number, stdout);
                        rprin ("' нет оператора!");
                        error = true;
                    }
                }
                if (temp->prior)
                {
                    if (temp->prior->ThisIs == BRECKET_CLOSE)
                    {
                        rprin ("Между ')' и числом '");
                        WriteComplexNumberInFile (temp->number, temp->Im_number, stdout);
                        rprin ("' нет оператора!");
                        error = true;
                    }
                }
            }
        temp = temp->next;
    }
}
