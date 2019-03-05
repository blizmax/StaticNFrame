#include "NFProtobufCommon.h"
#include "NFLogMgr.h"

bool NFProtobufCommon::MessageToJsonString(const google::protobuf::Message& msg, std::string& json)
{
	google::protobuf::util::JsonPrintOptions options;
	options.always_print_enums_as_ints = true;
	options.preserve_proto_field_names = true;

	google::protobuf::util::Status status = google::protobuf::util::MessageToJsonString(msg, &json, options);
	if (!status.ok())
	{
		NFLogError("MessageToJsonString Error | {}", status.error_message().ToString());
	}
	return status.ok();
}

bool NFProtobufCommon::JsonStringToMessage(const string& json, google::protobuf::Message& msg)
{
	google::protobuf::util::JsonParseOptions options;
	options.ignore_unknown_fields = true;
	google::protobuf::util::Status status = google::protobuf::util::JsonStringToMessage(json, &msg, options);
	if (!status.ok())
	{
		NFLogError("JsonStringToMessage Error | {}", status.error_message().ToString());
	}
	return status.ok();
}
