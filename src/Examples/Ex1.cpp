#include "Parsing/DelimitedMessagesStreamParser.h"
int main(int argc, char **argv)
{
#ifdef EnEx
    /*����������*/
    vector<char> mesages;
    TestTask::Messages::WrapperMessage pl;
    /*���������� ������*/
    pl.mutable_slow_response()->set_connected_client_count(12);
    /*������������*/
    PointerToConstData A = serializemes<TestTask::Messages::WrapperMessage>(pl);
    /*���������� ������ ���������������� �����������*/
    mesages.insert(mesages.end(), (*A).begin(), (*A).end());
    /*��������������*/
    MesParser<TestTask::Messages::WrapperMessage> parser;
    list<shared_ptr<const TestTask::Messages::WrapperMessage>> parsedMessages = parser.parse(string(mesages.begin(), mesages.end()));
    /*�������� ������������ ������ ������*/
    cout << (*parsedMessages.front()).slow_response().connected_client_count() << endl;
#endif // EnEx
    getchar();
    return 0;
}
