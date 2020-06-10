#include "GSUtilWrapper.h"
#include <iostream>
#include "SystemWrapper.h"

void GsutilWrapper::SetBucket(std::string bucketname)
{
	_bucketname = bucketname;
}

void GsutilWrapper::SetProject(std::string projectId)
{
	_projectId = projectId;
}

std::string GsutilWrapper::ListFingerprintImages()
{
	//gsutil ls -r gs://simprints-152315-images-eu/projects/fUBnpzDdbsCsMp0egCHB/**.wsq
	std::cout << "Fetching list of fingerprint images..." << std::endl;
	return _system.run("gsutil ls -r gs://" + _bucketname + "/projects/" + _projectId + "/**.wsq");
}

void GsutilWrapper::Download(std::string url)
{
	//gsutil cp \"gs://simprints-152315-images-eu/projects/fUBnpzDdbsCsMp0egCHB/sessions/78ee802e-a352-49e6-bc10-1e221054250a/fingerprints/6797b090-c4a5-4480-b407-7515d71ece19.wsq\" .
}

GsutilWrapper::GsutilWrapper()
{
	//gsutil
	std::cout << "Starting gsutil..." << std::endl;
	_system.run("gsutil");

}

//system("gsutil");
	//system("gsutil cp \"gs://simprints-152315-images-eu/projects/fUBnpzDdbsCsMp0egCHB/sessions/78ee802e-a352-49e6-bc10-1e221054250a/fingerprints/6797b090-c4a5-4480-b407-7515d71ece19.wsq\" .");