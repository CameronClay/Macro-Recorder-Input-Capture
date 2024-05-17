#include "File.h"

std::vector<std::string> File::GetFileList(const std::string& dir, const std::vector<std::string>& dirSkipList)
{
	std::vector<std::string> fileList;

	try 
	{
		if (fs::exists(dir) && fs::is_directory(dir))
		{
			fs::recursive_directory_iterator it{ dir }, end;

			while (it != end)
			{
				if (fs::is_directory(it->path()) && (std::find(dirSkipList.begin(), dirSkipList.end(), it->path().filename()) != dirSkipList.end()))
					it.disable_recursion_pending();
				else
					fileList.push_back(it->path().string());

				std::error_code ec;
				it.increment(ec);

				//if (ec) 
				//	std::cerr << "Error accessing : " << it->path().string() << " : " << ec.message() << '\n';
			}
		}
	}
	catch (...)
	{
		//std::cerr << "Exception: " << e.what();
	}

	return fileList;
}