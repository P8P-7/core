#include <goliath/controller/repositories/config_repository.h>

#include <fstream>
#include <sstream>

#include <repositories/ConfigRepository.pb.h>
#include <google/protobuf/util/json_util.h>

using namespace goliath;

repositories::ConfigRepository::ConfigRepository(std::string &configFile) {
    std::ifstream jsonFile(configFile);
    std::stringstream jsonBuffer;
    jsonBuffer << jsonFile.rdbuf();

    jsonString = jsonBuffer.str();
}

std::shared_ptr<::ConfigRepository> repositories::ConfigRepository::getConfig() {
    ::ConfigRepository configRepository;

    google::protobuf::util::JsonParseOptions options;
    google::protobuf::util::JsonStringToMessage(jsonString, &configRepository, options);

    return std::make_shared<::ConfigRepository>(configRepository);}

std::unique_ptr<::google::protobuf::Message> repositories::ConfigRepository::getMessage() {
    return std::make_unique<::ConfigRepository>(*getConfig());
}
