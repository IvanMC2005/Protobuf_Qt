#include <iostream>
#include <typeinfo>
#include <vector>
#include <list>
#include <stdexcept>
#include <conio.h>
#include "mes.pb.h"
#if GOOGLE_PROTOBUF_VERSION >= 3012004
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSizeLong())
#else
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSize())
#endif
using namespace std;
using namespace ::google::protobuf;
typedef std::vector<char> Data;
typedef std::shared_ptr<const Data> PointerToConstData;
typedef std::shared_ptr<Data> PointerToData;
template <typename Message>
std::shared_ptr<Message> parsehelper(const void* data, size_t size, size_t* bytesConsumed = nullptr)
{
    google::protobuf::io::CodedInputStream parser(reinterpret_cast<google::protobuf::uint8 *>(const_cast<void *>(data)), (int)size);
    size_t i = parser.ReadTag();
    if (i == 0 || size - google::protobuf::io::CodedOutputStream::VarintSize32(i) < i)
        return nullptr;
    std::shared_ptr<Message> pl = make_shared<Message>();
    bool success_read = (*pl).ParseFromArray(reinterpret_cast<const char *>(data) + google::protobuf::io::CodedOutputStream::VarintSize32(i), i);
    if (success_read)
    {
        *bytesConsumed += google::protobuf::io::CodedOutputStream::VarintSize32(i) + i;
        return pl;
    }
    else
        throw std::runtime_error("Incorrect data was transmitted in message");
}
template <typename MessageType>
class MesParser
{
 public:
  typedef std::shared_ptr<const MessageType> PointerToConstValue;

  std::list<PointerToConstValue> parse(const std::string& data)
  {
      m_buffer.insert(m_buffer.end(), data.begin(), data.end());
      list<PointerToConstValue> result;
      size_t bytesConsumed = 0, data_len = m_buffer.size();
      PointerToConstValue one_message;
      one_message = parsehelper<MessageType>(static_cast<const void *>(m_buffer.data() + bytesConsumed), data_len - bytesConsumed, &bytesConsumed);
      while (one_message != nullptr)
      {
          result.push_back(one_message);
          one_message = parsehelper<MessageType>(static_cast<const void *>(m_buffer.data() + bytesConsumed), data_len - bytesConsumed, &bytesConsumed);
        }
      return result;
  }
 private:
  std::vector<char> m_buffer;
};
template <typename Message> PointerToConstData serializemes(const Message& msg)
{
    const size_t messageSize = PROTOBUF_MESSAGE_BYTE_SIZE(msg);
    const size_t headerSize = google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);
    const PointerToData& result = std::make_shared<Data>(headerSize + messageSize);
    google::protobuf::uint8* buffer = reinterpret_cast<google::protobuf::uint8 *>(&*result->begin());
    google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(messageSize, buffer);
    msg.SerializeWithCachedSizesToArray(buffer + headerSize);
    return result;
}
