#include <iostream>
#include <fstream>
#include <string>

using namespace std;

unsigned short calculate_crc16(const string& path)
{
    unsigned short crc16 = 0xFFFF;

    ifstream file(path, ios::binary);
    if (!file)
    {
        cout << "Ошибка открытия файла: " << path << endl;
        return 0;
    }

    char byte;
    while (file.get(byte))
    {
        crc16 ^= static_cast<unsigned short>(byte);
        for (int i = 0; i < 8; i++)
        {
            if (crc16 & 0x0001)
            {
                crc16 >>= 1;
                crc16 ^= 0xA001;
            }
            else
            {
                crc16 >>= 1;
            }
        }
    }

    return crc16;
}

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Russian");

    if (argc != 2)
    {
        cout << "Использование: /crc16 - это имя программы , <путь_к_файлу>" << endl;
        return 1;
    }

    string path = "hyeta.txt";
    unsigned short crc16 = calculate_crc16(path);
    cout << "CRC-16 контрольная сумма для файла " << path << ": 0x" << hex << crc16 << endl;

    return 0;
}
