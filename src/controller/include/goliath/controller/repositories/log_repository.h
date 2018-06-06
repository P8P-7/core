#pragma once

#include <goliath/foundation.h>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/sinks.hpp>
#include <repositories/LogRepository.pb.h>
#include <thread>

namespace goliath::repositories {
    class LogRepository : public Repository {
    public:
        LogRepository(const size_t &bufferSize);
        ~LogRepository();

        std::unique_ptr<::google::protobuf::Message> getMessage() override;

    private:
        using SinkType = boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend>;
        struct LogEntry {
            const std::uint64_t timestamp;
            const std::string thread_id;
            const boost::log::trivial::severity_level severity;
            const std::string message;
        };

        std::mutex mutex;

        size_t bufferSize;
        std::deque<LogEntry> history;

        boost::shared_ptr<SinkType> sink;

        void addEntry(const LogEntry &entry);
        void formatter(const boost::log::record_view &record, boost::log::formatting_ostream &stream);
    };
}