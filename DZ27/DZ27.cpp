
#include <iostream>
#include <cstring>
#include <utility>
#include <windows.h>

using namespace std;

class MyString
{
private:
    char* data;
    int length;

    void assign(const char* text)
    {
        if (text == nullptr) text = "";

        int len = (int)strlen(text);
        char* temp = new char[len + 1];
        strcpy_s(temp, len + 1, text);

        delete[] data;
        data = temp;
        length = len;
    }

public:
    MyString(const char* text) : data(nullptr), length(0)
    {
        assign(text);
    }

    MyString() : MyString("") {}

    MyString(const MyString& other) : MyString(other.data) {}

    MyString(MyString&& other) : data(other.data), length(other.length)
    {
        other.data = nullptr;
        other.length = 0;
    }

    ~MyString() { delete[] data; }

    MyString& operator=(const MyString& other)
    {
        if (this != &other) assign(other.data);
        return *this;
    }

    MyString& operator=(MyString&& other)
    {
        if (this != &other)
        {
            delete[] data;
            data = other.data;
            length = other.length;
            other.data = nullptr;
            other.length = 0;
        }
        return *this;
    }

    int getLength() const { return length; }

    char& operator[](int i) { return data[i]; }
    char operator[](int i) const { return data[i]; }

    MyString& operator+=(const MyString& other)
    {
        int size = length + other.length + 1;
        char* temp = new char[size];

        strcpy_s(temp, size, data == nullptr ? "" : data);
        strcat_s(temp, size, other.data == nullptr ? "" : other.data);

        delete[] data;
        data = temp;
        length = size - 1;
        return *this;
    }

    bool operator==(const MyString& other) const
    {
        return strcmp(data ? data : "", other.data ? other.data : "") == 0;
    }

    bool operator!=(const MyString& other) const
    {
        return !(*this == other);
    }

    bool operator<(const MyString& other) const
    {
        return strcmp(data ? data : "", other.data ? other.data : "") < 0;
    }

    explicit operator bool() const { return length > 0; }

    friend MyString operator+(const MyString& a, const MyString& b);
    friend ostream& operator<<(ostream& out, const MyString& s);
    friend istream& operator>>(istream& in, MyString& s);
};

MyString operator+(const MyString& a, const MyString& b)
{
    MyString result(a);
    result += b;
    return result;
}

ostream& operator<<(ostream& out, const MyString& s)
{
    if (s.data != nullptr) out << s.data;
    return out;
}

istream& operator>>(istream& in, MyString& s)
{
    char buffer[256];
    in.width(256);
    if (in >> buffer) s = buffer;
    return in;
}

int main()
{
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);

    MyString greeting("Hello");
    MyString copy = greeting;
    copy[0] = 'J';

    cout << greeting << " " << copy << endl;

    MyString empty;
    cout << "[" << empty << "] Длина: " << empty.getLength() << endl;

    MyString name("World");
    MyString message = greeting + ", " + name + "!";
    cout << message << " Длина: " << message.getLength() << endl;

    name += "!!!";
    cout << name << endl;

    cout << (greeting == "Hello") << endl;
    cout << (copy != greeting) << endl;

    MyString a("A"), b("B"), c("C");
    a = b = c;
    c = c;
    cout << a << " " << b << " " << c << endl;

    const MyString title("OOP");
    cout << title[0] << endl;
 
    cout << "Длина Привіт: " << MyString("Привіт").getLength() << endl;

    MyString names[5] = { "Олена", "Марко", "Анна", "Тарас", "Богдан" };

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4 - i; j++)
            if (names[j + 1] < names[j])
            {
                MyString temp = std::move(names[j]);
                names[j] = std::move(names[j + 1]);
                names[j + 1] = std::move(temp);
            }

    cout << "Сортировка:" << endl;
    for (int i = 0; i < 5; i++)
        cout << names[i] << endl;

    MyString hello = "Hello, " + name;
    cout << hello << endl;

    MyString input;
    cout << "Введите слово: ";
    if (cin >> input) cout << input << endl;

    if (!empty) cout << "Строка пустая" << endl;
    if (input) cout << "Строка не пустая" << endl;

    return 0;
}

/*
ОТВЕТЫ НА ВОПРОСЫ

1. Без explicit строка const char* может автоматически
   преобразовываться в MyString.
   

2. Стандартный operator= скопировал бы только указатель.
   Старая память потерялась бы, а два объекта
   указывали бы на один массив.
  

3. Одна версия [] позволяет менять символы,
   другая — читать их у const-объекта.
   

4. strlen считает байты, а не буквы.
   В UTF-8 русские и украинские буквы обычно занимают
   по 2 байта. Поэтому "Привіт" занимает 12 байт.

5. std::move позволяет передать указатель без копирования.
   

6. Метод operator+ требует MyString слева.
   У "Hello, " слева const char*, поэтому метод
   не подходит. Глобальная дружественная функция
   позволяет преобразовать оба операнда в MyString.
*/
