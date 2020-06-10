#include "GSUtilWrapper.h"
#include <iostream>
#include "SystemWrapper.h"

const std::string GSUTIL("gsutil");
const std::string SUPRESS_OUTPUTS(" >0");

void GSUtilWrapper::SetBucket(std::string bucketname)
{
	_bucketname = bucketname;
}

void GSUtilWrapper::SetProject(std::string projectId)
{
	_projectId = projectId;
}

void GSUtilWrapper::ListFingerprintImages(std::vector<std::string> output)
{
	//gsutil ls -r gs://simprints-152315-images-eu/projects/fUBnpzDdbsCsMp0egCHB/**.wsq
	_system.run(GSUTIL + " ls -r gs://" + _bucketname + "/projects/" + _projectId + "/**.wsq");
}

void GSUtilWrapper::Download(std::string url)
{
}

bool GSUtilWrapper::hasPermissions()
{
	return false;
}

GSUtilWrapper::GSUtilWrapper()
{
	if (_system.run(GSUTIL + SUPRESS_OUTPUTS) != 0) {
		std::cout << "Error: need to install gsutil" << std::endl;
	}

}

//system("gsutil");
	//system("gsutil cp \"gs://simprints-152315-images-eu/projects/fUBnpzDdbsCsMp0egCHB/sessions/78ee802e-a352-49e6-bc10-1e221054250a/fingerprints/6797b090-c4a5-4480-b407-7515d71ece19.wsq\" .");