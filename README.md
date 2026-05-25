# Component-for-basic-cryptographic-operations (CryptoComponent).
**CryptoComponent** — это кроссплатформенная библиотека на C++, предоставляющая единый API для выполнения основных криптографических задач: хеширование, симметричное/асимметричное шифрование, а также создание и проверку электронной подписи по ГОСТ Р 34.10-2012.

Библиотека может быть собрана как динамическая (DLL/so) или статическая, не зависит от сторонних библиотек (за исключением OpenSSL для алгоритмов MD5/SHA/HMAC) и легко интегрируется в любые C++ проекты.

# Основные возможности
- **Хеширование**: Вычисление контрольных сумм и хеш-значений данных.
  - MD5, SHA‑1, SHA‑256, SHA‑512 (через OpenSSL).
  - Стрибог‑256 и Стрибог‑512 (собственная реализация).
  - HMAC для MD5, SHA‑1, SHA‑256, SHA‑512 (через OpenSSL).

- **Шифрование и Дешифрование**:
  - **Симметричные алгоритмы** (RC4, DES, Atbash, Шифр Цезаря, XOR (с циклическим ключом, результат в hex)).
  - **Асимметричные алгоритмы** (RSA).

- **Электронная подпись**: Генерация и проверка электронной подписи.
  - ГОСТ Р 34.10‑2012 (генерация и проверка подписи, кривая CryptoPro‑A).
  - Упаковка подписи в формат CAdES‑BES.

# Структура компонента (библиотеки)
Весь публичный API доступен через единственный заголовочный файл:
``` c++
#include <CryptoComponent.h>
```
и пространство имён:
``` c++
cryptocomponent::CryptoComponent.
```

### Основные методы 
| Категория |	Примеры методов |
|:-|:-|
| Хеширование |	md5(), sha256(), stribog256Hex(), hmacSHA256() |
| Симметричное шифрование |	rc4String(), desEncryptBlock(), atbash(), caesar(), xorCipherString() |
| Асимметричное шифрование | rsaEncryptString(), rsaDecryptString() |
| Электронная подпись |	gostSign(), gostVerify(), cadesWrap() |

Полный список методов находится в файле *include/CryptoComponent.h*.

# Требования
- Компилятор с поддержкой C++17
- CMake 3.15 или новее
- Библиотека OpenSSL (если нужны MD5, SHA, HMAC). При отсутствии OpenSSL библиотека собирается без этих алгоритмов, все остальные функции работают.

# Установка и сборка
Клонирование репозитория:
``` cmd
git clone https://github.com/ваш-репозиторий/Component-for-basic-cryptographic-operations.git
cd Component-for-basic-cryptographic-operations
```

Сборка динамической библиотеки (по умолчанию):
``` cmd
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```
В результате будут получены:
- *libComponent_for_basic_cryptographic_operations.so* (Linux) или *.dll* (Windows).
- Примеры в папке *examples/*.

Сборка статической библиотеки:
``` cmd
cmake .. -DBUILD_SHARED_LIBS=OFF
```

Включение/выключение тестов:
По умолчанию тесты отключены. Для их сборки необходим Google Test.
``` cmd
cmake .. -DCPPCRYPTO_BUILD_TESTS=ON
```

Установка:
``` cmd
sudo make install   # Linux/macOS
```
После установки библиотеку можно подключить через find_package:
``` cmake
find_package(Component_for_basic_cryptographic_operations REQUIRED)
target_link_libraries(my_app PRIVATE Component_for_basic_cryptographic_operations::Component_for_basic_cryptographic_operations)
```

# Примеры использования
Хеширование строки (Стрибог‑256):
``` c++
#include <CryptoComponent.h>
#include <iostream>

int main() {
    std::string data = "Hello, world!";
    std::string hash = cryptocomponent::CryptoComponent::stribog256Hex(data);
    std::cout << "Stribog-256: " << hash << std::endl;
    return 0;
}
```

Шифрование RC4:
``` c++
std::string key = "secret";
std::string plain = "Secret message";
std::string encrypted = cryptocomponent::CryptoComponent::rc4String(key, plain);
std::string decrypted = cryptocomponent::CryptoComponent::rc4String(key, encrypted);
```

Генерация и проверка ГОСТ-подписи:
``` c++
std::vector<uint8_t> msg = {0x54, 0x65, 0x73, 0x74}; // "Test"
std::string privateKeyHex = "...";
std::vector<uint8_t> signature = cryptocomponent::CryptoComponent::gostSign(msg, privateKeyHex);
bool ok = cryptocomponent::CryptoComponent::gostVerify(msg, signature, publicKeyXHex, publicKeyYHex);
```
Больше примеров — в папке *examples/*.

# Тестирование
Юнит-тесты находятся в *tests/*. Для их запуска необходимо установить Google Test и включить опцию -DCPPCRYPTO_BUILD_TESTS=ON.
``` cmd
cd build
ctest --output-on-failure
```
В проекте также присутствует лёгкая самодельная реализация Google Test (tests/gtest.h), которая позволяет запускать тесты даже без установленного GTest.

# Лицензия
Проект распространяется под лицензией **MIT**. См. файл *LICENSE*.

# Обратная связь
По всем вопросам, предложениям и багам обращайтесь через **Issues** на GitHub.

# Благодарности
- Реализация ГОСТ подписи основана на коде [Andreyinthesky](https://github.com/Andreyinthesky/cpp-cryptography).
- BigInteger — [TheRunuwayDinosaur](https://github.com/TheRunuwayDinosaur/GOST-R-34.10-12-Elliptic-Curve-Cryptography-).
- DES, RC4, RSA, хеширование — оригинальные реализации в рамках проекта.
