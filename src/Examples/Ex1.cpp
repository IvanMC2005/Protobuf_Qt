#include "Parsing/DelimitedMessagesStreamParser.h"
int main(int argc, char **argv)
{
#ifdef EnEx
    /*объявление*/
    vector<char> mesages;
    TestTask::Messages::WrapperMessage pl;
    /*заполнение данных*/
    pl.mutable_slow_response()->set_connected_client_count(12);
    /*сериализация*/
    PointerToConstData A = serializemes<TestTask::Messages::WrapperMessage>(pl);
    /*заполнение буфера сериализованными сообщениями*/
    mesages.insert(mesages.end(), (*A).begin(), (*A).end());
    /*десериализация*/
    MesParser<TestTask::Messages::WrapperMessage> parser;
    list<shared_ptr<const TestTask::Messages::WrapperMessage>> parsedMessages = parser.parse(string(mesages.begin(), mesages.end()));
    /*проверка корректности работы класса*/
    cout << (*parsedMessages.front()).slow_response().connected_client_count() << endl;
#endif // EnEx
    getchar();
    return 0;
}
