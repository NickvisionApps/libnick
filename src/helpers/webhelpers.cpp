#include "helpers/webhelpers.h"
#include <fstream>
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

namespace Nickvision::Aura
{
	bool WebHelpers::getIsValidWebsite(const std::string& url)
	{
		cURLpp::Cleanup curlCleanup;
		cURLpp::Easy curl;
		curl.setOpt(cURLpp::Options::Url(url));
		curl.setOpt(cURLpp::Options::FollowLocation(true));
		curl.setOpt(cURLpp::Options::WriteFunction([](void*, size_t size, size_t nmemb)
		{
			return size * nmemb;
		}));
		try
		{
			curl.perform();
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	bool WebHelpers::downloadFile(const std::string& url, const std::filesystem::path& path, std::function<int(double dltotal, double dlnow, double ultotal, double ulnow)> progress, bool overwrite)
	{
		if (std::filesystem::exists(path) && !overwrite)
		{
			return false;
		}
		cURLpp::Cleanup curlCleanup;
		cURLpp::Easy curl;
		std::ofstream out{ path, std::ios::binary | std::ios::trunc };
		curl.setOpt(cURLpp::Options::FollowLocation(true));
		curl.setOpt(cURLpp::Options::Url(url));
		curl.setOpt(cURLpp::Options::WriteStream(&out));
		if (progress)
		{
			curl.setOpt(cURLpp::Options::NoProgress(false));
			curl.setOpt(cURLpp::Options::ProgressFunction(progress));
		}
		try
		{
			curl.perform();
			return true;
		}
		catch (...)
		{
			return false;
		}
	}

	std::string WebHelpers::fetchJsonString(const std::string& url)
	{
		cURLpp::Cleanup curlCleanup;
		cURLpp::Easy curl;
		std::stringstream stream;
		curl.setOpt(cURLpp::Options::FollowLocation(true));
		curl.setOpt(cURLpp::Options::Url(url));
		curl.setOpt(cURLpp::Options::UserAgent("Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:109.0) Gecko/20100101 Firefox/120.0"));
		curl.setOpt(cURLpp::Options::HttpHeader({ "Content-Type: application/json" }));
		curl.setOpt(cURLpp::Options::Header(false));
		curl.setOpt(cURLpp::Options::WriteStream(&stream));
		try
		{
			curl.perform();
			return stream.str();
		}
		catch (...)
		{
			return "";
		}
	}
}