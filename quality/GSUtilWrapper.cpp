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

void GsutilWrapper::Download(std::string url, std::string destination)
{
	//gsutil cp gs://simprints-152315-images-eu/projects/fUBnpzDdbsCsMp0egCHB/sessions/78ee802e-a352-49e6-bc10-1e221054250a/fingerprints/6797b090-c4a5-4480-b407-7515d71ece19.wsq .
	_system.run("gsutil cp " + url + " " + destination);
}

void GsutilWrapper::DownloadAllWsq(std::string destination)
{
	//gsutil -m cp gs://simprints-152315-images-eu/projects/fUBnpzDdbsCsMp0egCHB/**.wsq .
	_system.run("gsutil -m cp gs://" + _bucketname + "/projects/" + _projectId + "/**.wsq " + destination);
}

bool GsutilWrapper::getFilenameFromUrl(std::string url, std::string* filename) {
	std::size_t found = url.find_last_of("/\\");	
	*filename = url.substr(found + 1);

	return (filename->find(".wsq") != std::string::npos);
}

GsutilWrapper::GsutilWrapper()
{
	//gsutil
}
