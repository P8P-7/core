#include <goliath/controller/repositories/log_repository.h>
#include <repositories/LogRepository.pb.h>

#include <boost/log/core.hpp>
#include <boost/log/common.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/value_extraction.hpp>

using namespace goliath;
namespace logging = boost::log;

repositories::LogRepository::LogRepository(const size_t &bufferSize)
        : bufferSize(bufferSize),
          sink(boost::make_shared<SinkType>()) {
    sink->set_formatter([&](const logging::record_view &record, logging::formatting_ostream &stream) {
        this->formatter(record, stream);
    });

    logging::core::get()->add_sink(sink);
}

repositories::LogRepository::~LogRepository() {
    logging::core::get()->remove_sink(sink);
}

void repositories::LogRepository::formatter(const logging::record_view &record, logging::formatting_ostream &stream) {
    const auto severity = record[logging::trivial::severity];
    const auto message = record[logging::expressions::smessage];

    std::stringstream threadIdStream;
    threadIdStream << boost::this_thread::get_id();
    std::stringstream messageStream;
    messageStream << message;

    LogEntry entry{
            .timestamp = static_cast<std::uint64_t>(
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::system_clock::now().time_since_epoch()
                    ).count()
            ),
            .thread_id = threadIdStream.str(),
            .severity = severity.get(),
            .message = messageStream.str()
    };

    addEntry(entry);
}

std::unique_ptr<::google::protobuf::Message> repositories::LogRepository::getMessage() {
    std::lock_guard<std::mutex> lock(mutex);
    proto::repositories::LogRepository repo;

    for (const auto &entry : history) {
        auto *newEntry = repo.add_entries();
        newEntry->set_severity(static_cast<proto::repositories::LogSeverity>(entry.severity));
        newEntry->set_message(entry.message);
        newEntry->set_thread_id(entry.thread_id);
        newEntry->set_timestamp(entry.timestamp);
    }

    return std::make_unique<proto::repositories::LogRepository>(repo);
}

void repositories::LogRepository::addEntry(const repositories::LogRepository::LogEntry &entry) {
    std::lock_guard<std::mutex> lock(mutex);

    if (history.size() + 1 >= bufferSize) {
        history.pop_front();
    }
    history.emplace_back(entry);

    invalidate();
}
