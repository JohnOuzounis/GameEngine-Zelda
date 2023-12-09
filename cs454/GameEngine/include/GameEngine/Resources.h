#pragma once
#include <string>

namespace GameEngine {
	class Resources final {
	private:
		std::string root;
		std::string assets;

		Resources() {}
		Resources(const Resources&) = delete;
		Resources& operator=(const Resources&) = delete;
	public:
		static Resources& Get(void) {
			static Resources src;
			return src;
		}

		void SetRootPath(const std::string& path) { root = path; }
		void SetAssetPath(const std::string& path) { assets = path; }

		std::string GetRootPath() { return root; }
		std::string GetAssetPath(void) { return assets; }

		std::string GetAsset(const std::string& path) { return assets + path; }
		std::string GetResource(const std::string& path) { return root+path; }
	};
}