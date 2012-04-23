<!DOCTYPE TS><TS>
<context>
    <name>ShtrihFR</name>
    <message>
        <source>szCmdSize must be greater then zero.</source>
        <translation>Параметр szCmdSize должен быть больше нуля.</translation>
    </message>
    <message>
        <source>szCmdSize must be less then 256.</source>
        <translation>Параметр szCmdSize должен быть меньше 256.</translation>
    </message>
    <message>
        <source>pCommand must not be NULL, while szCmdSize&gt;0.</source>
        <translation>Параметр pCommand не должен быть равен NULL, если szCmdSize&gt;0.</translation>
    </message>
    <message>
        <source>szRespBufSize&lt;0</source>
        <translation>Параметр szRespBufSize не должен быть меньше 0</translation>
    </message>
    <message>
        <source>Port is closed.</source>
        <translation>Порт закрыт.</translation>
    </message>
    <message>
        <source>Timeout while sending ENQ byte.</source>
        <translation>Таймаут при посылке ENQ.</translation>
    </message>
    <message>
        <source>Timeout while waiting confirmation byte.</source>
        <translation type="obsolete">Таймаут при ожидании байта подтверждения от устройства. Проверьте правильно ли подключен фискальный регистратор.</translation>
    </message>
    <message>
        <source>Device is going to send data.</source>
        <translation>Устройство в режиме передачи данных.</translation>
    </message>
    <message>
        <source>No error</source>
        <translation>Нет ошибок</translation>
    </message>
    <message>
        <source>All retries failed. Device link is down.</source>
        <translation>Невозможно установить связь с устройством. Проверьте подключение фискального регистратора.</translation>
    </message>
    <message>
        <source>readData: Timeout while waiting for STX.</source>
        <translation>readData: Таймаут при ожидании STX.</translation>
    </message>
    <message>
        <source>readData: Timeout while waiting for length byte.</source>
        <translation>readData: Таймаут при ожидании байта длины.</translation>
    </message>
    <message>
        <source>readData: Timeout while waiting for message byte.</source>
        <translation>readData: Таймаут при ожидании байа сообщения.</translation>
    </message>
    <message>
        <source>readData: Timeout while waiting for checksum byte.</source>
        <translation>readData: Таймаут при ожидании байта контрольной суммы.</translation>
    </message>
    <message>
        <source>Device returns error code 0x%1. %2</source>
        <translation>Устройство вернуло код ошибки 0x%1. %2</translation>
    </message>
    <message>
        <source>Too short responce - less then 2 bytes.</source>
        <translation>Слишком короткий ответ устройва - меньше 2-х байт.</translation>
    </message>
    <message>
        <source>Insufficient buffer size. Device responce was truncated.</source>
        <translation>Недостаточен размер буфера. Ответ устройства сохранен не полностью.</translation>
    </message>
    <message>
        <source>NULL pointer was passed for operator&apos;s password</source>
        <translation>Параметр &quot;Пароль оператора&quot; не должен быть равен NULL</translation>
    </message>
    <message>
        <source>Invalid operation number %1</source>
        <translation>Ошибочная операция №%1</translation>
    </message>
    <message>
        <source>internalAddOperation: Unexpected buffer overrun.</source>
        <translation>internalAddOperation: Переполнение буфера.</translation>
    </message>
    <message>
        <source>fromDecimal: Conversion of negative values isn&apos;t supported.</source>
        <translation>fromDecimal: Отрицательные числа не могут быть преобразованы в формат устройства.</translation>
    </message>
    <message>
        <source>fromDecimal: pBuf is NULL.</source>
        <translation>fromDecimal: pBuf равен NULL.</translation>
    </message>
    <message>
        <source>fromDecimal: Cannot convert number into device representation. Device settings weren&apos;t read.</source>
        <translation>fromDecimal: Невозможно преобразовать число в формат устройсва. Вызовите функцию internalReadOptions(0).</translation>
    </message>
    <message>
        <source>fromDecimal: Invalid decimal type parameter value.</source>
        <translation>fromDecimal: Неверный задан тип преобразуемого значения.</translation>
    </message>
    <message>
        <source>fromDecimal: Overflow. Supplied buffer size is too small.</source>
        <translation>fromDecimal: Переполнение. Размер буфера недостаточен.</translation>
    </message>
    <message>
        <source>toDecimal: pdVal==0</source>
        <translation>toDecimal: pdVal==0</translation>
    </message>
    <message>
        <source>toDecimal: pBuf is NULL.</source>
        <translation>toDecimal: pBuf == NULL.</translation>
    </message>
    <message>
        <source>toDecimal: szSize should be greater than zero</source>
        <translation>toDecimal: szSize должен быть больше 0</translation>
    </message>
    <message>
        <source>toDecimal: Cannot convert number from device representation. Device settings weren&apos;t read.</source>
        <translation>toDecimal: Невозможно преобразовать число в формат устройсва. Вызовите функцию internalReadOptions(0).</translation>
    </message>
    <message>
        <source>toDecimal: Invalid decimal type parameter value.</source>
        <translation>toDecimal: Неверно задан тип преобразуемого значения.</translation>
    </message>
    <message>
        <source>toDecimal: ulDivisor==0</source>
        <translation>toDecimal: ulDivisor==0</translation>
    </message>
    <message>
        <source>Invalid password format. Enter digits only, please.</source>
        <translation>Используйте пароль, состоящий только из десятичных цифр.</translation>
    </message>
    <message>
        <source>Invalid password type.</source>
        <translation>Неверный тип (оператор/администратор) пароля.</translation>
    </message>
    <message>
        <source>Invalid port number.</source>
        <translation>Недопустимый номер порта.</translation>
    </message>
    <message>
        <source>Unsupported baud rate %1</source>
        <translation>Битрейт %1 не поддерживается</translation>
    </message>
    <message>
        <source>internalWriteField: pValue==NULL</source>
        <translation>internalWriteField: pValue==NULL</translation>
    </message>
    <message>
        <source>internalWriteField: value size greater than 40</source>
        <translation>internalWriteField: размер значения больше 40</translation>
    </message>
    <message>
        <source>Error while converting dCashsum into device format. %1</source>
        <translation>Ошибка преобразования dCashsum в формат устройства. %1</translation>
    </message>
    <message>
        <source>Error while converting dSumPT2 into device format. %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error while converting dSumPT3 into device format. %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error while converting dSumPT4 into device format. %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error while converting dDiscount into device format. %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error while converting sText into device format. %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error while converting dSum into device format. %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error while converting &apos;line&apos; into device format. %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Document number is out of range (1..65535).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Register number is out of range (0..255).</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Error while converting &apos;value&apos; to double. %1</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Pointer to license shouldn&apos;t be NULL.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Hour value (%1) is out of range (0..23)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Minute value (%1) is out of range (0..59)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Second&apos;s value (%1) is out of range (0..59)</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Year&apos;s value (%1) is out of range 2000..2255 or 0..255.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Invalid date %1-%2-%3.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>devNum value (%1) is out of range 0..255.</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>stringToByteArray: pBuf==0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>stringToByteArray: szSize&lt;=0</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unknown error 0x%1</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Ошибок нет</source>
        <translation>Ошибок нет</translation>
    </message>
    <message encoding="UTF-8">
        <source>Неисправен накопитель ФП1, ФП2 или часы</source>
        <translation>Неисправен накопитель ФП1, ФП2 или часы</translation>
    </message>
    <message encoding="UTF-8">
        <source>Отсутствует ФП1</source>
        <translation>Отсутствует ФП1</translation>
    </message>
    <message encoding="UTF-8">
        <source>Отсутствует ФП2</source>
        <translation>Отсутствует ФП2</translation>
    </message>
    <message encoding="UTF-8">
        <source>Некорректные параметры в команде обращения к ФП</source>
        <translation>Некорректные параметры в команде обращения к ФП</translation>
    </message>
    <message encoding="UTF-8">
        <source>Нет запрошенных данных</source>
        <translation>Нет запрошенных данных</translation>
    </message>
    <message encoding="UTF-8">
        <source>ФП в режиме вывода данных</source>
        <translation>ФП в режиме вывода данных</translation>
    </message>
    <message encoding="UTF-8">
        <source>Некорректные параметры в команде для данной реализации ФП</source>
        <translation>Некорректные параметры в команде для данной реализации ФП</translation>
    </message>
    <message encoding="UTF-8">
        <source>Команда не поддерживается в данной реализации ФП</source>
        <translation>Команда не поддерживается в данной реализации ФП</translation>
    </message>
    <message encoding="UTF-8">
        <source>Некорректная длина команды</source>
        <translation>Некорректная длина команды</translation>
    </message>
    <message encoding="UTF-8">
        <source>Не введена лицензия</source>
        <translation>Не введена лицензия</translation>
    </message>
    <message encoding="UTF-8">
        <source>Заводской номер уже введен</source>
        <translation>Заводской номер уже введен</translation>
    </message>
    <message encoding="UTF-8">
        <source>Текущая дата меньше даты последней записи в ФП</source>
        <translation>Текущая дата меньше даты последней записи в ФП</translation>
    </message>
    <message encoding="UTF-8">
        <source>Область сменных итогов ФП переполнена</source>
        <translation>Область сменных итогов ФП переполнена</translation>
    </message>
    <message encoding="UTF-8">
        <source>Смена уже открыта</source>
        <translation>Смена уже открыта</translation>
    </message>
    <message encoding="UTF-8">
        <source>Смена не открыта</source>
        <translation>Смена не открыта</translation>
    </message>
    <message encoding="UTF-8">
        <source>Номер первой смены больше номера последней смены</source>
        <translation>Номер первой смены больше номера последней смены</translation>
    </message>
    <message encoding="UTF-8">
        <source>Дата первой смены больше даты последней смены</source>
        <translation>Дата первой смены больше даты последней смены</translation>
    </message>
    <message encoding="UTF-8">
        <source>Нет данных в ФП</source>
        <translation>Нет данных в ФП</translation>
    </message>
    <message encoding="UTF-8">
        <source>Область перерегистраций в ФП переполнена</source>
        <translation>Область перерегистраций в ФП переполнена</translation>
    </message>
    <message encoding="UTF-8">
        <source>Заводской номер не введён</source>
        <translation>Заводской номер не введён</translation>
    </message>
    <message encoding="UTF-8">
        <source>В заданном диапазоне есть поврежденная запись</source>
        <translation>В заданном диапазоне есть поврежденная запись</translation>
    </message>
    <message encoding="UTF-8">
        <source>Повреждена последняя запись сменных итогов</source>
        <translation>Повреждена последняя запись сменных итогов</translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение денежного регистра при добавлении</source>
        <translation>Переполнение денежного регистра при добавлении</translation>
    </message>
    <message encoding="UTF-8">
        <source>Вычитаемая сумма больще содержимого денежного регистра</source>
        <translation>Вычитаемая сумма больще содержимого денежного регистра</translation>
    </message>
    <message encoding="UTF-8">
        <source>Неверная дата</source>
        <translation>Неверная дата</translation>
    </message>
    <message encoding="UTF-8">
        <source>Вносимая клиентом сумма меньше суммы чека</source>
        <translation>Вносимая клиентом сумма меньше суммы чека</translation>
    </message>
    <message encoding="UTF-8">
        <source>Невозможно отменить предыдущую команду</source>
        <translation>Невозможно отменить предыдущую команду</translation>
    </message>
    <message encoding="UTF-8">
        <source>Обнуленная касса (повторное гашение невозможно)</source>
        <translation>Обнуленная касса (повторное гашение невозможно)</translation>
    </message>
    <message encoding="UTF-8">
        <source>Сумма чека по секции меньше суммы сторно.</source>
        <translation>Сумма чека по секции меньше суммы сторно.</translation>
    </message>
    <message encoding="UTF-8">
        <source>В ФР нет денег для выплаты</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>ФР заблокирован, ждет ввода пароля налогового инспектора</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Требуется выполнение общего гашения</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Некорректный параметр в команде</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Нет данных</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Некорректный параметр при данных настройках</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Некорректные параметры в команде для данной реализации ФР</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Команда не поддерживается в данной реализации ФР</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Ошибка в ПЗУ</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Внутренняя ошибка ПО ФР</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение накопления по надбавкам в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение накопления в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Смена открыта - операция невозможна</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Смена не открыта - операция невозможна</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение накопления по секциям в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение накопления по скидкам в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение диапазона скидок</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение диапазона наличными</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение диапазона тип 2</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение диапазона тип 3</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение диапазона тип 4</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Сумма всех типов оплаты меньше итога чека</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Не хватает наличности в кассе</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение накопления по налогам в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение итога чека</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Операция невозможна в открытом чеке данного типа</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Открыт чек - операция невозможна</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Буфер чека переполнен</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение накопления по обороту налогов в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Вносимая безналичной оплатой сумма больше суммы чека</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Смена превысила 24 часа</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Неверный пароль</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Идет печать предыдущей команды</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение накоплений наличными в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение накоплений по типу оплаты 2 в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение накоплений по типу оплаты 3 в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение накоплений по типу оплаты 4 в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Чек закрыт - операция невозможна</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Нет документа для повтора</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Ожидание команды продолжения печати</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Документ открыт другим оператором</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Скидка превышает накопления в чеке</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение диапазона надбавок</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Таблица не определена</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Некорректная операция</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Отрицательный итог чека</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение при умножении</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение диапазона цены</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение диапазона количества</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение диапазона отдела</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>ФП отсутствует</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Не хватает денег в секции</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение денег в секции</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Не хватает денег по обороту налогов</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение денег по обороту налогов</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Нет чековой ленты</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Нет контрольной ленты</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Не хватает денег по налогу</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение денег по налогу</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение по выплате в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение ФП</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Ошибка отрезчика</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Команда не поддерживается в данном подрежиме</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Команда не поддерживается в данном режиме</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Ошибка ОЗУ</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Ошибка питания</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Ошибка принтера: нет импульсов с тахогенератора</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Ошибка принтера: нет сигналов с датчика</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Замена ПО</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Замена ФП</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Поле не редактируется</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Ошибка оборудования</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Не совпадает дата</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Неверный формат даты</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Неверное значение в поле длины</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение диапазона итога чека</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Ошибка связи с ФП</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение наличности</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение по продажам в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение по покупкам в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение по возвратам продаж в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение по возвратам покупок в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение по внесению в смене</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение по надбавкам в чеке</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Переполнение по скидкам в чеке</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Отрицательный итог по надбавкам в чеке</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Отрицательный итог по скидкам в чеке</source>
        <translation type="unfinished"></translation>
    </message>
    <message encoding="UTF-8">
        <source>Нулевой итог чека</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Unsupported document type.</source>
        <translation>Этот тип документа не поддерживается.</translation>
    </message>
    <message>
        <source>Check isn&apos;t opened. I cannot beginAdd().</source>
        <translation>Чек не открыт. Невозможно выполнить beginAdd().</translation>
    </message>
    <message>
        <source>Previous call to beginAdd() was not matched with endAdd().</source>
        <translation>Функция beginAdd() была вызвана без предварительного вызова endAdd().</translation>
    </message>
    <message>
        <source>ShtrihFR cannot use more then 4 taxes per item.</source>
        <translation>Штрих-ФР не поддерживает более 4-х налогов на позицию.</translation>
    </message>
    <message>
        <source>Unsupported operation type.</source>
        <translation>Неподдерживаемая операция.</translation>
    </message>
    <message>
        <source>There was no matching beginAdd() call.</source>
        <translation>Невозможно выполнить endAdd() без предварительного вызова beginAdd().</translation>
    </message>
    <message>
        <source>There was no setItem() call. Item isn&apos;t complete.</source>
        <translation>Функция setItem() не была вызвана. Текущая позиция незавершена.</translation>
    </message>
    <message>
        <source>This item was processed already. I cannot cancel it.</source>
        <translation>Невозможно отменить позицию, она уже была обработана.</translation>
    </message>
    <message>
        <source>Check isn&apos;t open. I cannot setPayment(%1,%2).</source>
        <translation>Чек не открыт. Невозможно выполнить setPayment(%1,%2).</translation>
    </message>
    <message>
        <source>Unsupported payment type %1. Valid range 1..4.</source>
        <translation>Неподдерживаемый тип оплаты %1. Допустимые значения 1..4.</translation>
    </message>
    <message>
        <source>Check isn&apos;t open. I cannot addCheckTax(%1).</source>
        <translation>Чек не открыт. Невозможно выполнить addCheckTax(%1).</translation>
    </message>
    <message>
        <source>ShtrihFR does not support more then 4 taxes per item. I cannot addCheckTax(%1).</source>
        <translation>Штрих-ФР не поддерживает более 4-х налогов на позицию. Невозможно выполнить addCheckTax(%1).</translation>
    </message>
    <message>
        <source>Check isn&apos;t open. I cannot cancelCheck().</source>
        <translation>Чек не открыт. Невозможно выполнить cancelCheck().</translation>
    </message>
    <message>
        <source>There&apos;s currently open check position, endAdd() or setItem() wasn&apos;t called. I cannot close check.</source>
        <translation>Невозможно закрыть чек - текущая позиция чека не закрыта. Вызовите endAdd() или setItem().</translation>
    </message>
    <message>
        <source>!!!There was error while adding check positions: %1. And I cannot cancel check due to error: %1.</source>
        <translation>!!!Ошибка при передаче позиций чека в ФР: %1. При попытке отмены чека возникла ошибка %2. Настоятельно рекомендуем прервать работу и обратиться к специалисту!</translation>
    </message>
    <message>
        <source>Item was closed by setItem(). Cannot add attribute</source>
        <translation type="unfinished"></translation>
    </message>
    <message>
        <source>Timeout while waiting confirmation byte.
Check FR&apos;s connectivity, please.
FR could be powered off, as well.</source>
        <translation>Превышен интервал ожидания байта подтверждения.
Проверьте включен ли ФР и подключен ли он к компьютеру.
Другой причиной ошибки может быть неправильное указание номера COM-порта.</translation>
    </message>
    <message>
        <source>
I cannot determine current FR mode however.
Please, call support for further information.</source>
        <translation>
Но драйвер не может определить текущее состояние ФР.
Пожалуйста, обратитесь в службу поддержки.</translation>
    </message>
    <message>
        <source>Shift cannot be longer than 24 hours.
Take Z-report, please.</source>
        <translation>Длительность смены превысила 24 часа.
Пожалуста снимите Z-отчёт.</translation>
    </message>
    <message>
        <source>FR is locked by wrong fiscal password.
Contact tax officer to resolve situation.</source>
        <translation>ФР заблокирован, так как был введён неправильный пароль налогового инспектора.
Пожалуйста обратитесь в службу поддержки.</translation>
    </message>
    <message>
        <source>There&apos;s open document in the FR.
First, make sure You have one copy of Open-POS running.
If no other copy is running,
then use FR control utility to cancel document.</source>
        <translation>На ФР уже открыт документ.
Проверьте, не запущена ли вторая копия программы.
Для продолжения работы закройте вторую копию программы.
Если вторая копия не запущена, обратитесь в службу поддержки.</translation>
    </message>
    <message>
        <source>payment: Sum must be positive.</source>
        <translation type="obsolete">payment(): Sum должна быть положительной.</translation>
    </message>
    <message>
        <source>
Continue print command was issued.
Try to print check again.</source>
        <translation>
Команда продолжения печати была отдана.
Попробуйте напечатать чек ещё раз.</translation>
    </message>
    <message>
        <source>payment(): Sum must be positive.</source>
        <translation>payment(): Сумма должна быть положительной.</translation>
    </message>
    <message>
        <source>payingin(): Sum must be positive.</source>
        <translation>payingin(): Сумма должна быть положительной.</translation>
    </message>
</context>
</TS>
